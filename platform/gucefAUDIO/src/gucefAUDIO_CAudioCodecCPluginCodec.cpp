/*
 *  gucefAUDIO: GUCEF module providing audio playback, mixing and decoding
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

#ifndef GUCEF_AUDIO_CAUDIOBUFFER_H
#include "gucefAUDIO_CAudioBuffer.h"
#define GUCEF_AUDIO_CAUDIOBUFFER_H
#endif /* GUCEF_AUDIO_CAUDIOBUFFER_H ? */

#include "gucefAUDIO_CAudioCodecCPluginCodec.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace AUDIO {

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
class CAudioCodecCPluginInstance : public CIAudioCodecCPluginClient
{
    public:

    typedef CAudioCodecCPlugin::TAudioCodecCPluginPtr TAudioCodecCPluginPtr;

    CAudioCodecCPluginInstance( const TAudioCodecCPluginPtr& plugin )
        : CIAudioCodecCPluginClient()
        , m_plugin( plugin )
        , m_codecdata( GUCEF_NULL )
        , m_isCreated( false )
    {GUCEF_TRACE;
    }

    virtual ~CAudioCodecCPluginInstance() GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;

        MT::CScopeMutex lock( m_plugin->GetCallLock() );
        m_plugin->UnregisterClient( this );
        const TGucefAudioCodecPluginApi* api = m_plugin->GetLinkedApi();
        if ( m_isCreated && GUCEF_NULL != api )
            api->Destroy_Codec( m_plugin->GetPlugData(), &m_codecdata );
        m_isCreated = false;
    }

    /** Call with the call lock held */
    bool Create( const CString& codecType )
    {GUCEF_TRACE;

        const TGucefAudioCodecPluginApi* api = m_plugin->GetLinkedApi();
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

    virtual void OnPluginUnlink( const TGucefAudioCodecPluginApi& api, void** plugdata ) GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;

        if ( m_isCreated )
            api.Destroy_Codec( plugdata, &m_codecdata );
        m_isCreated = false;
        m_codecdata = GUCEF_NULL;
    }

    CAudioCodecCPlugin& Plugin( void )
    {GUCEF_TRACE;

        return *m_plugin;
    }

    private:

    TAudioCodecCPluginPtr m_plugin;
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

bool
FormatFromInfo( const TAudioFormatInfo& info, CAudioFormat& format )
{GUCEF_TRACE;

    if ( info.sampleType < (UInt32) SAMPLETYPE_UINT8 || info.sampleType > (UInt32) SAMPLETYPE_FLOAT64 )
        return false;

    format = CAudioFormat( info.sampleRate, info.channelCount, (TSampleType) info.sampleType, info.channelLayout );
    return format.IsValid();
}

/*-------------------------------------------------------------------------*/

void
InfoFromFormat( const CAudioFormat& format, TAudioFormatInfo& info )
{GUCEF_TRACE;

    info.sampleRate = format.GetSampleRate();
    info.channelCount = format.GetChannelCount();
    info.sampleType = (UInt32) format.GetSampleType();
    info.channelLayout = format.GetChannelLayout();
}

/*-------------------------------------------------------------------------*/

/** A stream the plugin decodes ('sourcedata') */
class CAudioCodecCPluginSource : public IAudioSource ,
                                 public CIAudioCodecCPluginClient
{
    public:

    typedef CAudioCodecCPluginCodec::TCodecInstancePtr TCodecInstancePtr;
    typedef CIAudioCodec::TIOAccessPtr TIOAccessPtr;

    CAudioCodecCPluginSource( const TCodecInstancePtr& instance, CORE::CIOAccess* input, const TIOAccessPtr& owner )
        : IAudioSource()
        , CIAudioCodecCPluginClient()
        , m_instance( instance )
        , m_input( input )
        , m_inputOwner( owner )
        , m_sourcedata( GUCEF_NULL )
        , m_isOpen( false )
        , m_isEndOfStream( false )
        , m_canSeek( false )
        , m_format()
        , m_length( GUCEF_AUDIO_LENGTH_UNKNOWN )
        , m_position( 0 )
    {GUCEF_TRACE;
    }

    virtual ~CAudioCodecCPluginSource() GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;

        CAudioCodecCPlugin& plugin = m_instance->Plugin();
        MT::CScopeMutex lock( plugin.GetCallLock() );
        plugin.UnregisterClient( this );
        if ( m_isOpen && m_instance->IsUsable() )
            plugin.GetLinkedApi()->Src_Close( plugin.GetPlugData(), m_instance->GetCodecData(), &m_sourcedata );
        m_isOpen = false;
    }

    bool Open( const CORE::CValueList& options )
    {GUCEF_TRACE;

        CAudioCodecCPlugin& plugin = m_instance->Plugin();
        MT::CScopeMutex lock( plugin.GetCallLock() );
        if ( !m_instance->IsUsable() )
            return false;

        TStringMap optionMap;
        OptionsToStringMap( options, optionMap );
        TStringMapAdapter optionAdapter;
        optionAdapter.SetMapAccessPtr( (const TStringMap*) &optionMap );

        TAudioFormatInfo info;
        memset( &info, 0, sizeof( info ) );
        UInt64 length = GUCEF_AUDIOCODEC_LENGTH_UNKNOWN;
        const TGucefAudioCodecPluginApi* api = plugin.GetLinkedApi();
        if ( 0 == api->Src_Open( plugin.GetPlugData(), m_instance->GetCodecData(), &m_sourcedata, m_input->CStyleAccess(), optionAdapter.GetCStyleAccess(), &info, &length ) )
            return false;

        m_isOpen = true;
        if ( !FormatFromInfo( info, m_format ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "AudioCodecCPluginSource: The plugin \"" << plugin.GetName() << "\" opened a stream with an invalid format" );
            api->Src_Close( plugin.GetPlugData(), m_instance->GetCodecData(), &m_sourcedata );
            m_isOpen = false;
            return false;
        }

        m_length = length;
        m_canSeek = GUCEF_NULL != api->Src_Seek;
        plugin.RegisterClient( this, true );
        return true;
    }

    virtual const CAudioFormat& GetFormat( void ) const GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;

        return m_format;
    }

    virtual UInt32 ReadFrames( void* destination, UInt32 frameCapacity ) GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;

        if ( m_isEndOfStream || 0 == frameCapacity || GUCEF_NULL == destination )
            return 0;

        CAudioCodecCPlugin& plugin = m_instance->Plugin();
        MT::CScopeMutex lock( plugin.GetCallLock() );
        if ( !m_isOpen || !m_instance->IsUsable() )
        {
            m_isEndOfStream = true;
            return 0;
        }

        UInt32 framesRead = 0;
        if ( 0 == plugin.GetLinkedApi()->Src_Read_Frames( plugin.GetPlugData(), m_instance->GetCodecData(), &m_sourcedata, destination, frameCapacity, &framesRead ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "AudioCodecCPluginSource: The plugin \"" << plugin.GetName() << "\" failed to decode at frame " << m_position << ", ending the stream" );
            m_isEndOfStream = true;
            return 0;
        }
        if ( framesRead > frameCapacity )
            framesRead = frameCapacity;

        m_position += framesRead;
        if ( 0 == framesRead || ( GUCEF_AUDIO_LENGTH_UNKNOWN != m_length && m_position >= m_length ) )
            m_isEndOfStream = true;
        return framesRead;
    }

    virtual bool IsEndOfStream( void ) const GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;

        return m_isEndOfStream;
    }

    virtual bool IsSeekable( void ) const GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;

        return m_canSeek;
    }

    virtual bool SeekToFrame( UInt64 frame ) GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;

        if ( !m_canSeek || ( GUCEF_AUDIO_LENGTH_UNKNOWN != m_length && frame > m_length ) )
            return false;

        CAudioCodecCPlugin& plugin = m_instance->Plugin();
        MT::CScopeMutex lock( plugin.GetCallLock() );
        if ( !m_isOpen || !m_instance->IsUsable() )
            return false;

        if ( 0 == plugin.GetLinkedApi()->Src_Seek( plugin.GetPlugData(), m_instance->GetCodecData(), &m_sourcedata, frame ) )
            return false;

        m_position = frame;
        m_isEndOfStream = GUCEF_AUDIO_LENGTH_UNKNOWN != m_length && frame >= m_length;
        return true;
    }

    virtual UInt64 GetPositionInFrames( void ) const GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;

        return m_position;
    }

    virtual UInt64 GetLengthInFrames( void ) const GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;

        return m_length;
    }

    virtual void OnPluginUnlink( const TGucefAudioCodecPluginApi& api, void** plugdata ) GUCEF_VIRTUAL_OVERRIDE
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
    bool m_canSeek;
    CAudioFormat m_format;
    UInt64 m_length;
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
CAudioCodecCPluginCodec::Create( const TAudioCodecCPluginPtr& plugin, const CString& codecType, CORE::CTBasicSharedPtr< CIAudioCodec, MT::CMutex >& codec )
{GUCEF_TRACE;

    if ( plugin.IsNULL() )
        return false;

    TCodecInstancePtr instance( GUCEF_NEW CAudioCodecCPluginInstance( plugin ) );
    {
        MT::CScopeMutex lock( plugin->GetCallLock() );
        if ( !instance->Create( codecType ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "AudioCodecCPluginCodec: The plugin \"" << plugin->GetName() << "\" could not create a codec of type \"" << codecType << "\"" );
            return false;
        }
    }

    codec = CORE::CTSharedPtr< CIAudioCodec, MT::CMutex >( GUCEF_NEW CAudioCodecCPluginCodec( plugin, codecType, instance ) );
    return true;
}

/*-------------------------------------------------------------------------*/

CAudioCodecCPluginCodec::CAudioCodecCPluginCodec( const TAudioCodecCPluginPtr& plugin, const CString& codecType, const TCodecInstancePtr& instance )
    : CIAudioCodec()
    , m_plugin( plugin )
    , m_codecType( codecType )
    , m_instance( instance )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CAudioCodecCPluginCodec::CAudioCodecCPluginCodec( const CAudioCodecCPluginCodec& src )
    : CIAudioCodec( src )
    , m_plugin( src.m_plugin )
    , m_codecType( src.m_codecType )
    , m_instance( src.m_instance )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CAudioCodecCPluginCodec::~CAudioCodecCPluginCodec()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CAudioCodecCPluginCodec::OpenSourceImpl( CORE::CIOAccess* input, const TIOAccessPtr& owner, const CORE::CValueList& options, TAudioSourcePtr& source )
{GUCEF_TRACE;

    if ( GUCEF_NULL == input )
        return false;

    CAudioCodecCPluginSource* pluginSource = GUCEF_NEW CAudioCodecCPluginSource( m_instance, input, owner );
    if ( !pluginSource->Open( options ) )
    {
        GUCEF_DELETE pluginSource;
        return false;
    }
    source = TAudioSourcePtr( pluginSource );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CAudioCodecCPluginCodec::DecodeMetaData( CORE::CIOAccess& encodedInput, CORE::CValueList& metaData )
{GUCEF_TRACE;

    UInt64 startOffset = encodedInput.Tell();

    // The format and the length are what opening the stream tells
    {
        CAudioCodecCPluginSource source( m_instance, &encodedInput, TIOAccessPtr() );
        if ( !source.Open( CORE::CValueList() ) )
            return false;

        const CAudioFormat& format = source.GetFormat();
        metaData.Set( "sampleRate", CORE::UInt32ToString( format.GetSampleRate() ) );
        metaData.Set( "channels", CORE::UInt32ToString( format.GetChannelCount() ) );
        metaData.Set( "sampleType", CString( CAudioFormat::SampleTypeToString( format.GetSampleType() ) ) );
        metaData.Set( "channelLayout", CORE::UInt32ToString( format.GetChannelLayout() ) );
        UInt64 length = source.GetLengthInFrames();
        if ( GUCEF_AUDIO_LENGTH_UNKNOWN != length )
        {
            metaData.Set( "frames", CORE::UInt64ToString( length ) );
            metaData.Set( "durationInMs", CORE::UInt64ToString( length * 1000 / format.GetSampleRate() ) );
        }
    }

    // Tags, if the plugin reads them
    MT::CScopeMutex lock( m_plugin->GetCallLock() );
    const TGucefAudioCodecPluginApi* api = m_plugin->GetLinkedApi();
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

bool
CAudioCodecCPluginCodec::CanEncode( void ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_plugin->GetCallLock() );
    const TGucefAudioCodecPluginApi* api = m_plugin->GetLinkedApi();
    return GUCEF_NULL != api && GUCEF_NULL != api->Encode && m_instance->IsUsable();
}

/*-------------------------------------------------------------------------*/

bool
CAudioCodecCPluginCodec::Encode( const CAudioBuffer& audio, CORE::CIOAccess& encodedOutput, const CORE::CValueList& options )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_plugin->GetCallLock() );
    const TGucefAudioCodecPluginApi* api = m_plugin->GetLinkedApi();
    if ( GUCEF_NULL == api || GUCEF_NULL == api->Encode || !m_instance->IsUsable() )
        return false;

    TAudioFormatInfo info;
    InfoFromFormat( audio.GetFormat(), info );

    TStringMap optionMap;
    OptionsToStringMap( options, optionMap );
    TStringMapAdapter optionAdapter;
    optionAdapter.SetMapAccessPtr( (const TStringMap*) &optionMap );

    return 0 != api->Encode( m_plugin->GetPlugData(), m_instance->GetCodecData(), &info, audio.GetDataPtr(), audio.GetFrameCount(),
                             encodedOutput.CStyleAccess(), optionAdapter.GetCStyleAccess() );
}

/*-------------------------------------------------------------------------*/

CString
CAudioCodecCPluginCodec::GetType( void ) const
{GUCEF_TRACE;

    return m_codecType;
}

/*-------------------------------------------------------------------------*/

const CString&
CAudioCodecCPluginCodec::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CString typeName = "GUCEF::AUDIO::CAudioCodecCPluginCodec";
    return typeName;
}

/*-------------------------------------------------------------------------*/

CORE::CICloneable*
CAudioCodecCPluginCodec::Clone( void ) const
{GUCEF_TRACE;

    return GUCEF_NEW CAudioCodecCPluginCodec( *this );
}

/*-------------------------------------------------------------------------*/

const CAudioCodecCPluginCodec::TAudioCodecCPluginPtr&
CAudioCodecCPluginCodec::GetPlugin( void ) const
{GUCEF_TRACE;

    return m_plugin;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace AUDIO */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
