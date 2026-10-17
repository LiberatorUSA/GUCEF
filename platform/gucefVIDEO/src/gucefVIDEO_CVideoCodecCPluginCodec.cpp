/*
 *  gucefVIDEO: GUCEF module providing video decoding and playback
 *
 *  Copyright (C) 1998 - 2026.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <string.h>

#ifndef GUCEF_MT_CSCOPEMUTEX_H
#include "gucefMT_CScopeMutex.h"
#define GUCEF_MT_CSCOPEMUTEX_H
#endif /* GUCEF_MT_CSCOPEMUTEX_H ? */

#ifndef GUCEF_CORE_CTVARIANTMAPADAPTER_CPP2C_H
#include "gucefCORE_CTVariantMapAdapter_Cpp2C.h"
#define GUCEF_CORE_CTVARIANTMAPADAPTER_CPP2C_H
#endif /* GUCEF_CORE_CTVARIANTMAPADAPTER_CPP2C_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#include "gucefVIDEO_CVideoCodecCPluginCodec.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace VIDEO {

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

typedef CORE::CTVariantMapAdapterCpp2C< CString, CString > TStringMapAdapter;
typedef TStringMapAdapter::TLinkedMapType TStringMap;

/*-------------------------------------------------------------------------*/

/**
 *  The codec state a plugin keeps per codec type ('codecdata'). Shared by the codec and the sources it opened,
 *  so it lives until the last of them is gone, or until the plugin is unlinked.
 */
class CVideoCodecCPluginInstance : public CIVideoCodecCPluginClient
{
    public:

    typedef CVideoCodecCPlugin::TVideoCodecCPluginPtr TVideoCodecCPluginPtr;

    CVideoCodecCPluginInstance( const TVideoCodecCPluginPtr& plugin )
        : CIVideoCodecCPluginClient()
        , m_plugin( plugin )
        , m_codecdata( GUCEF_NULL )
        , m_isCreated( false )
    {GUCEF_TRACE;
    }

    virtual ~CVideoCodecCPluginInstance() GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;

        MT::CScopeMutex lock( m_plugin->GetCallLock() );
        m_plugin->UnregisterClient( this );
        const TGucefVideoCodecPluginApi* api = m_plugin->GetLinkedApi();
        if ( m_isCreated && GUCEF_NULL != api )
            api->Destroy_Codec( m_plugin->GetPlugData(), &m_codecdata );
        m_isCreated = false;
    }

    /** Call with the call lock held */
    bool Create( const CString& codecType )
    {GUCEF_TRACE;

        const TGucefVideoCodecPluginApi* api = m_plugin->GetLinkedApi();
        if ( GUCEF_NULL == api )
            return false;

        if ( 0 == api->Create_Codec( m_plugin->GetPlugData(), codecType.C_String(), &m_codecdata ) )
            return false;

        m_isCreated = true;
        m_plugin->RegisterClient( this, false );
        return true;
    }

    /** True while the plugin can be called for this instance. Call with the call lock held */
    bool IsUsable( void ) const
    {GUCEF_TRACE;

        return m_isCreated && GUCEF_NULL != m_plugin->GetLinkedApi();
    }

    void** GetCodecData( void )
    {GUCEF_TRACE;

        return &m_codecdata;
    }

    virtual void OnPluginUnlink( const TGucefVideoCodecPluginApi& api, void** plugdata ) GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;

        if ( m_isCreated )
            api.Destroy_Codec( plugdata, &m_codecdata );
        m_isCreated = false;
        m_codecdata = GUCEF_NULL;
    }

    CVideoCodecCPlugin& Plugin( void )
    {GUCEF_TRACE;

        return *m_plugin;
    }

    private:

    TVideoCodecCPluginPtr m_plugin;
    void* m_codecdata;
    bool m_isCreated;
};

/*-------------------------------------------------------------------------*/

namespace {

/*-------------------------------------------------------------------------*/

/** Options as the plugin reads them: text keys and text values */
void
OptionsToStringMap( const CORE::CValueList& options, TStringMap& map )
{GUCEF_TRACE;

    UInt32 keyCount = options.GetKeyCount();
    for ( UInt32 i=0; i<keyCount; ++i )
        map[ options.GetKey( i ).AsString() ] = options.GetValue( i ).AsString();
}

/*-------------------------------------------------------------------------*/

IMAGE::TPixelStorageFormat
PixelFormatFromInfo( UInt32 pixelFormat )
{GUCEF_TRACE;

    switch ( pixelFormat )
    {
        case GUCEF_VIDEOCODEC_PIXELFORMAT_PALETTE8: return IMAGE::PSF_PALETTE_INDICES;
        case GUCEF_VIDEOCODEC_PIXELFORMAT_RGB24: return IMAGE::PSF_RGB;
        case GUCEF_VIDEOCODEC_PIXELFORMAT_RGBA32: return IMAGE::PSF_RGBA;
        case GUCEF_VIDEOCODEC_PIXELFORMAT_BGRA32: return IMAGE::PSF_BGRA;
        default: return IMAGE::PSF_UNKNOWN;
    }
}

/*-------------------------------------------------------------------------*/

bool
AudioFormatFromInfo( const TAudioFormatInfo& info, AUDIO::CAudioFormat& format )
{GUCEF_TRACE;

    if ( info.sampleType < (UInt32) AUDIO::SAMPLETYPE_UINT8 || info.sampleType > (UInt32) AUDIO::SAMPLETYPE_FLOAT64 )
        return false;

    format = AUDIO::CAudioFormat( info.sampleRate, info.channelCount, (AUDIO::TSampleType) info.sampleType, info.channelLayout );
    return format.IsValid();
}

/*-------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX
OnPluginAudio( void* userData, UInt32 track, const void* frames, UInt32 frameCount ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;

    IVideoAudioSink* sink = static_cast< IVideoAudioSink* >( userData );
    if ( GUCEF_NULL != sink && GUCEF_NULL != frames && frameCount > 0 )
        sink->OnVideoAudio( track, frames, frameCount );
}

/*-------------------------------------------------------------------------*/

/** A video the plugin decodes ('sourcedata') */
class CVideoCodecCPluginSource : public IVideoSource ,
                                 public CIVideoCodecCPluginClient
{
    public:

    typedef CVideoCodecCPluginCodec::TCodecInstancePtr TCodecInstancePtr;
    typedef CIVideoCodec::TIOAccessPtr TIOAccessPtr;
    typedef GUCEF::vector< AUDIO::CAudioFormat > TAudioFormatVector;

    CVideoCodecCPluginSource( const TCodecInstancePtr& instance, CORE::CIOAccess* input, const TIOAccessPtr& owner )
        : IVideoSource()
        , CIVideoCodecCPluginClient()
        , m_instance( instance )
        , m_input( input )
        , m_inputOwner( owner )
        , m_sourcedata( GUCEF_NULL )
        , m_isOpen( false )
        , m_isEndOfStream( false )
        , m_hasFailed( false )
        , m_canSeek( false )
        , m_format()
        , m_audioFormats()
        , m_position( 0 )
    {GUCEF_TRACE;
    }

    virtual ~CVideoCodecCPluginSource() GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;

        CVideoCodecCPlugin& plugin = m_instance->Plugin();
        MT::CScopeMutex lock( plugin.GetCallLock() );
        plugin.UnregisterClient( this );
        if ( m_isOpen && m_instance->IsUsable() )
            plugin.GetLinkedApi()->Src_Close( plugin.GetPlugData(), m_instance->GetCodecData(), &m_sourcedata );
        m_isOpen = false;
    }

    bool Open( const CORE::CValueList& options )
    {GUCEF_TRACE;

        CVideoCodecCPlugin& plugin = m_instance->Plugin();
        MT::CScopeMutex lock( plugin.GetCallLock() );
        if ( !m_instance->IsUsable() )
            return false;

        TStringMap optionMap;
        OptionsToStringMap( options, optionMap );
        TStringMapAdapter optionAdapter;
        optionAdapter.SetMapAccessPtr( (const TStringMap*) &optionMap );

        TVideoFormatInfo info;
        memset( &info, 0, sizeof( info ) );
        UInt64 frameCount = GUCEF_VIDEOCODEC_FRAMECOUNT_UNKNOWN;
        const TGucefVideoCodecPluginApi* api = plugin.GetLinkedApi();
        if ( 0 == api->Src_Open( plugin.GetPlugData(), m_instance->GetCodecData(), &m_sourcedata, m_input->CStyleAccess(), optionAdapter.GetCStyleAccess(), &info, &frameCount ) )
            return false;

        m_isOpen = true;
        m_format = CVideoFormat( info.width, info.height, PixelFormatFromInfo( info.pixelFormat ), info.frameRateNumerator, info.frameRateDenominator,
                                 GUCEF_VIDEOCODEC_FRAMECOUNT_UNKNOWN == frameCount ? GUCEF_VIDEO_FRAMECOUNT_UNKNOWN : frameCount );
        bool valid = m_format.IsValid();
        for ( UInt32 track=0; valid && track<info.audioTrackCount; ++track )
        {
            TAudioFormatInfo audioInfo;
            memset( &audioInfo, 0, sizeof( audioInfo ) );
            AUDIO::CAudioFormat audioFormat;
            valid = 0 != api->Src_Get_Audio_Track_Format( plugin.GetPlugData(), m_instance->GetCodecData(), &m_sourcedata, track, &audioInfo ) &&
                    AudioFormatFromInfo( audioInfo, audioFormat );
            m_audioFormats.push_back( audioFormat );
        }
        if ( !valid )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VideoCodecCPluginSource: The plugin \"" << plugin.GetName() << "\" opened a video with an invalid format" );
            api->Src_Close( plugin.GetPlugData(), m_instance->GetCodecData(), &m_sourcedata );
            m_isOpen = false;
            return false;
        }

        m_canSeek = GUCEF_NULL != api->Src_Seek_Frame;
        plugin.RegisterClient( this, true );
        return true;
    }

    virtual const CVideoFormat& GetFormat( void ) const GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;

        return m_format;
    }

    virtual UInt32 GetAudioTrackCount( void ) const GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;

        return (UInt32) m_audioFormats.size();
    }

    virtual bool GetAudioTrackFormat( UInt32 track, AUDIO::CAudioFormat& format ) const GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;

        if ( track >= m_audioFormats.size() )
            return false;
        format = m_audioFormats[ track ];
        return true;
    }

    virtual bool DecodeNextFrame( CVideoFrame& frame, IVideoAudioSink* audioSink ) GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;

        if ( m_isEndOfStream || !frame.Prepare( m_format ) )
            return false;

        CVideoCodecCPlugin& plugin = m_instance->Plugin();
        MT::CScopeMutex lock( plugin.GetCallLock() );
        if ( !m_isOpen || !m_instance->IsUsable() )
        {
            m_isEndOfStream = true;
            return false;
        }

        TVideoPictureBuffer picture;
        picture.pixels = frame.GetPicture().GetDataPtr();
        picture.pitch = frame.GetPitch();
        picture.palette = m_format.IsPalettized() ? frame.GetPalette() : GUCEF_NULL;
        picture.paletteChanged = 0;

        TVideoAudioOutput audio;
        audio.onAudio = &OnPluginAudio;
        audio.userData = audioSink;

        UInt32 endOfStream = 0;
        if ( 0 == plugin.GetLinkedApi()->Src_Decode_Frame( plugin.GetPlugData(), m_instance->GetCodecData(), &m_sourcedata, &picture, GUCEF_NULL != audioSink ? &audio : GUCEF_NULL, &endOfStream ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VideoCodecCPluginSource: The plugin \"" << plugin.GetName() << "\" failed to decode frame " << m_position << ", ending the video" );
            m_isEndOfStream = true;
            m_hasFailed = true;
            return false;
        }
        if ( 0 != endOfStream )
        {
            m_isEndOfStream = true;
            return false;
        }

        frame.SetFrameIndex( m_position );
        frame.SetTimeInMicroSeconds( m_format.GetFrameTimeInMicroSeconds( m_position ) );
        frame.SetPaletteChanged( 0 != picture.paletteChanged );
        ++m_position;
        return true;
    }

    virtual bool IsEndOfStream( void ) const GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;

        return m_isEndOfStream;
    }

    virtual bool HasFailed( void ) const GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;

        return m_hasFailed;
    }

    virtual UInt64 GetPositionInFrames( void ) const GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;

        return m_position;
    }

    virtual bool IsSeekable( void ) const GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;

        return m_canSeek;
    }

    virtual bool SeekToFrame( UInt64 frame ) GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;

        if ( !m_canSeek || ( GUCEF_VIDEO_FRAMECOUNT_UNKNOWN != m_format.GetFrameCount() && frame > m_format.GetFrameCount() ) )
            return false;

        CVideoCodecCPlugin& plugin = m_instance->Plugin();
        MT::CScopeMutex lock( plugin.GetCallLock() );
        if ( !m_isOpen || !m_instance->IsUsable() )
            return false;

        if ( 0 == plugin.GetLinkedApi()->Src_Seek_Frame( plugin.GetPlugData(), m_instance->GetCodecData(), &m_sourcedata, frame ) )
            return false;

        m_position = frame;
        m_hasFailed = false;
        m_isEndOfStream = GUCEF_VIDEO_FRAMECOUNT_UNKNOWN != m_format.GetFrameCount() && frame >= m_format.GetFrameCount();
        return true;
    }

    virtual void OnPluginUnlink( const TGucefVideoCodecPluginApi& api, void** plugdata ) GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;

        if ( m_isOpen )
            api.Src_Close( plugdata, m_instance->GetCodecData(), &m_sourcedata );
        m_isOpen = false;
        m_isEndOfStream = true;
    }

    private:

    TCodecInstancePtr m_instance;
    CORE::CIOAccess* m_input;
    TIOAccessPtr m_inputOwner;
    void* m_sourcedata;
    bool m_isOpen;
    bool m_isEndOfStream;
    bool m_hasFailed;
    bool m_canSeek;
    CVideoFormat m_format;
    TAudioFormatVector m_audioFormats;
    UInt64 m_position;
};

/*-------------------------------------------------------------------------*/

}; /* anonymous namespace */

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

bool
CVideoCodecCPluginCodec::Create( const TVideoCodecCPluginPtr& plugin, const CString& codecType, CORE::CTBasicSharedPtr< CIVideoCodec, MT::CMutex >& codec )
{GUCEF_TRACE;

    if ( plugin.IsNULL() )
        return false;

    TCodecInstancePtr instance( GUCEF_NEW CVideoCodecCPluginInstance( plugin ) );
    {
        MT::CScopeMutex lock( plugin->GetCallLock() );
        if ( !instance->Create( codecType ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VideoCodecCPluginCodec: The plugin \"" << plugin->GetName() << "\" could not create a codec of type \"" << codecType << "\"" );
            return false;
        }
    }

    codec = CORE::CTSharedPtr< CIVideoCodec, MT::CMutex >( GUCEF_NEW CVideoCodecCPluginCodec( plugin, codecType, instance ) );
    return true;
}

/*-------------------------------------------------------------------------*/

CVideoCodecCPluginCodec::CVideoCodecCPluginCodec( const TVideoCodecCPluginPtr& plugin, const CString& codecType, const TCodecInstancePtr& instance )
    : CIVideoCodec()
    , m_plugin( plugin )
    , m_codecType( codecType )
    , m_instance( instance )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CVideoCodecCPluginCodec::CVideoCodecCPluginCodec( const CVideoCodecCPluginCodec& src )
    : CIVideoCodec( src )
    , m_plugin( src.m_plugin )
    , m_codecType( src.m_codecType )
    , m_instance( src.m_instance )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CVideoCodecCPluginCodec::~CVideoCodecCPluginCodec()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CVideoCodecCPluginCodec::OpenSourceImpl( CORE::CIOAccess* input, const TIOAccessPtr& owner, const CORE::CValueList& options, TVideoSourcePtr& source )
{GUCEF_TRACE;

    if ( GUCEF_NULL == input )
        return false;

    CVideoCodecCPluginSource* pluginSource = GUCEF_NEW CVideoCodecCPluginSource( m_instance, input, owner );
    if ( !pluginSource->Open( options ) )
    {
        GUCEF_DELETE pluginSource;
        return false;
    }
    source = TVideoSourcePtr( pluginSource );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CVideoCodecCPluginCodec::DecodeMetaData( CORE::CIOAccess& encodedInput, CORE::CValueList& metaData )
{GUCEF_TRACE;

    UInt64 startOffset = encodedInput.Tell();

    // The format, the length and the tracks are what opening the video tells
    {
        CVideoCodecCPluginSource source( m_instance, &encodedInput, TIOAccessPtr() );
        if ( !source.Open( CORE::CValueList() ) )
            return false;

        const CVideoFormat& format = source.GetFormat();
        metaData.Set( "width", CORE::UInt32ToString( format.GetWidth() ) );
        metaData.Set( "height", CORE::UInt32ToString( format.GetHeight() ) );
        metaData.Set( "pixelFormat", format.IsPalettized() ? CString( "palette8" ) : ( IMAGE::PSF_RGB == format.GetPixelFormat() ? CString( "rgb24" ) :
                                     ( IMAGE::PSF_RGBA == format.GetPixelFormat() ? CString( "rgba32" ) : CString( "bgra32" ) ) ) );
        metaData.Set( "frameRate", CORE::DoubleToString( format.GetFrameRate() ) );
        if ( GUCEF_VIDEO_FRAMECOUNT_UNKNOWN != format.GetFrameCount() )
        {
            metaData.Set( "frames", CORE::UInt64ToString( format.GetFrameCount() ) );
            metaData.Set( "durationInMs", CORE::UInt64ToString( format.GetDurationInMs() ) );
        }
        metaData.Set( "audioTracks", CORE::UInt32ToString( source.GetAudioTrackCount() ) );
        for ( UInt32 track=0; track<source.GetAudioTrackCount(); ++track )
        {
            AUDIO::CAudioFormat audioFormat;
            source.GetAudioTrackFormat( track, audioFormat );
            const CString prefix = "audioTrack" + CORE::UInt32ToString( track ) + ".";
            metaData.Set( prefix + "sampleRate", CORE::UInt32ToString( audioFormat.GetSampleRate() ) );
            metaData.Set( prefix + "channels", CORE::UInt32ToString( audioFormat.GetChannelCount() ) );
            metaData.Set( prefix + "sampleType", CString( AUDIO::CAudioFormat::SampleTypeToString( audioFormat.GetSampleType() ) ) );
        }
    }

    // Tags, if the plugin reads them
    MT::CScopeMutex lock( m_plugin->GetCallLock() );
    const TGucefVideoCodecPluginApi* api = m_plugin->GetLinkedApi();
    if ( GUCEF_NULL != api && GUCEF_NULL != api->Decode_Meta_Data && m_instance->IsUsable() )
    {
        if ( 0 != encodedInput.Setpos( startOffset ) )
            return true;

        TStringMap tags;
        TStringMapAdapter tagAdapter;
        tagAdapter.SetMapAccessPtr( &tags );
        if ( 0 != api->Decode_Meta_Data( m_plugin->GetPlugData(), m_instance->GetCodecData(), encodedInput.CStyleAccess(), tagAdapter.GetCStyleAccess() ) )
        {
            for ( TStringMap::const_iterator i = tags.begin(); i != tags.end(); ++i )
                metaData.Set( (*i).first, (*i).second );
        }
    }
    return true;
}

/*-------------------------------------------------------------------------*/

CString
CVideoCodecCPluginCodec::GetType( void ) const
{GUCEF_TRACE;

    return m_codecType;
}

/*-------------------------------------------------------------------------*/

const CString&
CVideoCodecCPluginCodec::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CString typeName = "GUCEF::VIDEO::CVideoCodecCPluginCodec";
    return typeName;
}

/*-------------------------------------------------------------------------*/

CORE::CICloneable*
CVideoCodecCPluginCodec::Clone( void ) const
{GUCEF_TRACE;

    return GUCEF_NEW CVideoCodecCPluginCodec( *this );
}

/*-------------------------------------------------------------------------*/

const CVideoCodecCPluginCodec::TVideoCodecCPluginPtr&
CVideoCodecCPluginCodec::GetPlugin( void ) const
{GUCEF_TRACE;

    return m_plugin;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace VIDEO */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
