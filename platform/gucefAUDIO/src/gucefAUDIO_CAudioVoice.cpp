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

#include <math.h>
#include <string.h>

#ifndef GUCEF_AUDIO_CAUDIOCONVERTER_H
#include "gucefAUDIO_CAudioConverter.h"
#define GUCEF_AUDIO_CAUDIOCONVERTER_H
#endif /* GUCEF_AUDIO_CAUDIOCONVERTER_H ? */

#include "gucefAUDIO_CAudioVoice.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace AUDIO {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace {

/** Frames read from a source in one go: fixed so the memory of a voice does not depend on how much it is asked for at once */
const UInt32 SourceChunkFrames = 2048;

const Float32 MinPlaybackRate = 0.05f;
const Float32 MaxPlaybackRate = 16.0f;

Int32
IndexOfSpeaker( UInt32 layout, UInt32 speaker )
{
    Int32 index = 0;
    for ( UInt32 bit=0; bit<32; ++bit )
    {
        const UInt32 mask = 1U << bit;
        if ( layout & mask )
        {
            if ( mask == speaker )
                return index;
            ++index;
        }
    }
    return -1;
}

}

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CAudioVoice::CAudioVoice( const TAudioSourcePtr& source )
    : m_source( source )
    , m_bus( GUCEF_NULL )
    , m_volume( 1.0f )
    , m_pan( 0.0f )
    , m_playbackRate( 1.0f )
    , m_loopCount( 1 )
    , m_state( AUDIOVOICESTATE_IDLE )
    , m_endReason( AUDIOVOICEEND_NONE )
    , m_position( 0 )
    , m_stopRequested( false )
    , m_pauseRequested( false )
    , m_sourceFormat()
    , m_mixFormat()
    , m_matrix()
    , m_rawBuffer()
    , m_rawFloatBuffer()
    , m_convertedBuffer()
    , m_blockBuffer()
    , m_resampler()
    , m_maxBlock( 0 )
    , m_pendingInput( 0 )
    , m_playsDone( 0 )
    , m_framesThisPlay( 0 )
    , m_playedPosition( 0.0 )
    , m_lastGains()
    , m_gainsKnown( false )
    , m_prepared( false )
    , m_sourceEnded( false )
    , m_resamplerRatio( 0.0 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CAudioVoice::~CAudioVoice()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

const TAudioSourcePtr&
CAudioVoice::GetSource( void ) const
{GUCEF_TRACE;

    return m_source;
}

/*-------------------------------------------------------------------------*/

void
CAudioVoice::SetBus( CAudioBus* bus )
{GUCEF_TRACE;

    m_bus = bus;
}

/*-------------------------------------------------------------------------*/

CAudioBus*
CAudioVoice::GetBus( void ) const
{GUCEF_TRACE;

    return m_bus;
}

/*-------------------------------------------------------------------------*/

void
CAudioVoice::SetVolume( Float32 volume )
{GUCEF_TRACE;

    m_volume.Store( volume < 0.0f ? 0.0f : volume );
}

/*-------------------------------------------------------------------------*/

Float32
CAudioVoice::GetVolume( void ) const
{GUCEF_TRACE;

    return m_volume.Load();
}

/*-------------------------------------------------------------------------*/

void
CAudioVoice::SetPan( Float32 pan )
{GUCEF_TRACE;

    m_pan.Store( pan < -1.0f ? -1.0f : ( pan > 1.0f ? 1.0f : pan ) );
}

/*-------------------------------------------------------------------------*/

Float32
CAudioVoice::GetPan( void ) const
{GUCEF_TRACE;

    return m_pan.Load();
}

/*-------------------------------------------------------------------------*/

void
CAudioVoice::SetPlaybackRate( Float32 rate )
{GUCEF_TRACE;

    m_playbackRate.Store( rate < MinPlaybackRate ? MinPlaybackRate : ( rate > MaxPlaybackRate ? MaxPlaybackRate : rate ) );
}

/*-------------------------------------------------------------------------*/

Float32
CAudioVoice::GetPlaybackRate( void ) const
{GUCEF_TRACE;

    return m_playbackRate.Load();
}

/*-------------------------------------------------------------------------*/

void
CAudioVoice::SetLoopCount( UInt32 plays )
{GUCEF_TRACE;

    m_loopCount.Store( plays, MT::MEMORDER_RELAXED );
}

/*-------------------------------------------------------------------------*/

UInt32
CAudioVoice::GetLoopCount( void ) const
{GUCEF_TRACE;

    return m_loopCount.Load( MT::MEMORDER_RELAXED );
}

/*-------------------------------------------------------------------------*/

TAudioVoiceState
CAudioVoice::GetState( void ) const
{GUCEF_TRACE;

    return (TAudioVoiceState) m_state.Load( MT::MEMORDER_ACQUIRE );
}

/*-------------------------------------------------------------------------*/

TAudioVoiceEndReason
CAudioVoice::GetEndReason( void ) const
{GUCEF_TRACE;

    return (TAudioVoiceEndReason) m_endReason.Load( MT::MEMORDER_ACQUIRE );
}

/*-------------------------------------------------------------------------*/

UInt64
CAudioVoice::GetPositionInFrames( void ) const
{GUCEF_TRACE;

    return m_position.Load( MT::MEMORDER_RELAXED );
}

/*-------------------------------------------------------------------------*/

void
CAudioVoice::RequestStop( void )
{GUCEF_TRACE;

    m_stopRequested.Store( true, MT::MEMORDER_RELEASE );
}

/*-------------------------------------------------------------------------*/

void
CAudioVoice::RequestPause( bool paused )
{GUCEF_TRACE;

    m_pauseRequested.Store( paused, MT::MEMORDER_RELEASE );
}

/*-------------------------------------------------------------------------*/

bool
CAudioVoice::IsPauseRequested( void ) const
{GUCEF_TRACE;

    return m_pauseRequested.Load( MT::MEMORDER_ACQUIRE );
}

/*-------------------------------------------------------------------------*/

bool
CAudioVoice::IsStopRequested( void ) const
{GUCEF_TRACE;

    return m_stopRequested.Load( MT::MEMORDER_ACQUIRE );
}

/*-------------------------------------------------------------------------*/

bool
CAudioVoice::Prepare( const CAudioFormat& mixFormat, UInt32 maxBlockFrames )
{GUCEF_TRACE;

    m_prepared = false;
    if ( m_source.IsNULL() || !mixFormat.IsValid() || 0 == maxBlockFrames )
        return false;

    m_sourceFormat = m_source->GetFormat();
    if ( !m_sourceFormat.IsValid() )
        return false;
    if ( !CAudioConverter::BuildChannelMatrix( m_sourceFormat, mixFormat, m_matrix ) )
        return false;

    m_mixFormat = mixFormat;
    m_maxBlock = maxBlockFrames;
    const UInt32 mixChannels = mixFormat.GetChannelCount();
    m_rawBuffer.assign( (size_t) SourceChunkFrames * m_sourceFormat.GetBytesPerFrame(), 0 );
    m_rawFloatBuffer.assign( (size_t) SourceChunkFrames * m_sourceFormat.GetChannelCount(), 0.0f );
    m_convertedBuffer.assign( (size_t) SourceChunkFrames * mixChannels, 0.0f );
    m_blockBuffer.assign( (size_t) maxBlockFrames * mixChannels, 0.0f );
    m_lastGains.assign( mixChannels, 1.0f );
    m_resampler.Configure( mixChannels, (Float64) m_sourceFormat.GetSampleRate(), (Float64) mixFormat.GetSampleRate() );
    m_resamplerRatio = 0.0;
    m_pendingInput = 0;
    m_playsDone = 0;
    m_framesThisPlay = 0;
    m_playedPosition = 0.0;
    m_sourceEnded = false;
    m_gainsKnown = false;
    m_prepared = true;
    return true;
}

/*-------------------------------------------------------------------------*/

void
CAudioVoice::End( TAudioVoiceEndReason reason )
{GUCEF_TRACE;

    m_endReason.Store( (UInt32) reason, MT::MEMORDER_RELEASE );
    m_state.Store( AUDIOVOICESTATE_ENDED, MT::MEMORDER_RELEASE );
}

/*-------------------------------------------------------------------------*/

UInt32
CAudioVoice::ReadAndConvert( UInt32 frames, bool sameChannels )
{GUCEF_TRACE;

    if ( frames > SourceChunkFrames )
        frames = SourceChunkFrames;
    const UInt32 got = m_source->ReadFrames( &m_rawBuffer[ 0 ], frames );
    if ( 0 == got )
        return 0;

    const UInt32 sourceChannels = m_sourceFormat.GetChannelCount();
    const UInt32 mixChannels = m_mixFormat.GetChannelCount();
    if ( sameChannels )
    {
        CAudioConverter::ConvertToFloat32( &m_rawBuffer[ 0 ], m_sourceFormat.GetSampleType(), &m_convertedBuffer[ 0 ], (UInt64) got * sourceChannels );
    }
    else
    {
        CAudioConverter::ConvertToFloat32( &m_rawBuffer[ 0 ], m_sourceFormat.GetSampleType(), &m_rawFloatBuffer[ 0 ], (UInt64) got * sourceChannels );
        CAudioConverter::RemixChannels( &m_rawFloatBuffer[ 0 ], sourceChannels, &m_convertedBuffer[ 0 ], mixChannels, got, m_matrix );
    }
    return got;
}

/*-------------------------------------------------------------------------*/

bool
CAudioVoice::HandleSourceEnd( void )
{GUCEF_TRACE;

    if ( !m_source->IsEndOfStream() )
        return false;                       // nothing right now but not over (a live source): the block stays short

    ++m_playsDone;
    const UInt32 plays = m_loopCount.Load( MT::MEMORDER_RELAXED );
    const bool again = ( 0 == plays || m_playsDone < plays ) && m_framesThisPlay > 0 && m_source->IsSeekable();
    if ( again && m_source->SeekToFrame( 0 ) )
    {
        m_framesThisPlay = 0;
        m_playedPosition = 0.0;
        return true;
    }
    m_sourceEnded = true;
    return false;
}

/*-------------------------------------------------------------------------*/

UInt32
CAudioVoice::GatherBlock( UInt32 frames )
{GUCEF_TRACE;

    const UInt32 mixChannels = m_mixFormat.GetChannelCount();
    const UInt32 sourceChannels = m_sourceFormat.GetChannelCount();
    const bool sameChannels = ( sourceChannels == mixChannels && m_sourceFormat.GetChannelLayout() == m_mixFormat.GetChannelLayout() );

    // Speed: how many source frames pass for each frame that is mixed
    const Float64 rate = (Float64) m_playbackRate.Load();
    const bool resample = m_sourceFormat.GetSampleRate() != m_mixFormat.GetSampleRate() || rate != 1.0;
    if ( resample )
    {
        const Float64 inputRate = (Float64) m_sourceFormat.GetSampleRate() * rate;
        if ( inputRate != m_resamplerRatio )
        {
            m_resampler.SetRates( inputRate, (Float64) m_mixFormat.GetSampleRate() );
            m_resamplerRatio = inputRate;
        }
    }

    UInt32 produced = 0;
    Float32* out = &m_blockBuffer[ 0 ];
    UInt32 stalled = 0;

    while ( produced < frames && !m_sourceEnded )
    {
        if ( resample )
        {
            if ( 0 == m_pendingInput )
            {
                m_pendingInput = ReadAndConvert( SourceChunkFrames, sameChannels );
                m_framesThisPlay += m_pendingInput;
            }
            if ( 0 == m_pendingInput )
            {
                if ( !HandleSourceEnd() )
                    break;
                continue;
            }

            UInt32 consumed = 0, made = 0;
            m_resampler.Process( &m_convertedBuffer[ 0 ], m_pendingInput, consumed, out + (size_t) produced * mixChannels, frames - produced, made );
            produced += made;
            m_playedPosition += (Float64) made * ( m_resamplerRatio / (Float64) m_mixFormat.GetSampleRate() );   // source frames passed
            if ( consumed > 0 )
                m_pendingInput = 0;
            else if ( 0 == made && ++stalled > 2 )
                break;                      // the resampler cannot go on with this, do not spin
        }
        else
        {
            const UInt32 want = ( frames - produced < SourceChunkFrames ) ? ( frames - produced ) : SourceChunkFrames;
            const UInt32 got = ReadAndConvert( want, sameChannels );
            if ( 0 == got )
            {
                if ( !HandleSourceEnd() )
                    break;
                continue;
            }
            m_framesThisPlay += got;
            m_playedPosition += (Float64) got;
            memcpy( out + (size_t) produced * mixChannels, &m_convertedBuffer[ 0 ], (size_t) got * mixChannels * sizeof( Float32 ) );
            produced += got;
        }
    }
    return produced;
}

/*-------------------------------------------------------------------------*/

void
CAudioVoice::RenderBlock( Float32* busBuffer, UInt32 frames, TAudioPanLaw panLaw )
{GUCEF_TRACE;

    if ( !m_prepared || frames > m_maxBlock || 0 == frames )
    {
        End( AUDIOVOICEEND_FAILED );
        return;
    }

    if ( m_stopRequested.Load( MT::MEMORDER_ACQUIRE ) )
    {
        End( AUDIOVOICEEND_STOPPED );
        return;
    }

    // Pausing holds everything where it is: nothing is read, nothing is heard
    if ( m_pauseRequested.Load( MT::MEMORDER_ACQUIRE ) )
    {
        m_state.Store( AUDIOVOICESTATE_PAUSED, MT::MEMORDER_RELEASE );
        return;
    }
    if ( AUDIOVOICESTATE_PAUSED == m_state.Load( MT::MEMORDER_RELAXED ) )
        m_state.Store( AUDIOVOICESTATE_PLAYING, MT::MEMORDER_RELEASE );

    const UInt32 mixChannels = m_mixFormat.GetChannelCount();
    const UInt32 produced = GatherBlock( frames );
    if ( produced < frames )
        memset( &m_blockBuffer[ (size_t) produced * mixChannels ], 0, (size_t) ( frames - produced ) * mixChannels * sizeof( Float32 ) );
    m_position.Store( (UInt64) m_playedPosition, MT::MEMORDER_RELAXED );

    // The gain of every channel: volume, and for the front speakers the position between them
    const UInt32 gainCount = (UInt32) m_lastGains.size();
    Float32 gains[ 64 ];
    const UInt32 limited = ( gainCount < 64 ) ? gainCount : 64;
    const Float32 volume = m_volume.Load();
    for ( UInt32 c=0; c<limited; ++c )
        gains[ c ] = volume;
    const UInt32 layout = m_mixFormat.GetChannelLayout();
    const Int32 left = IndexOfSpeaker( layout, SPEAKER_FRONT_LEFT );
    const Int32 right = IndexOfSpeaker( layout, SPEAKER_FRONT_RIGHT );
    const Float32 pan = m_pan.Load();
    if ( left >= 0 && right >= 0 && (UInt32) left < limited && (UInt32) right < limited && 0.0f != pan )
    {
        Float32 gl = 1.0f, gr = 1.0f;
        if ( AUDIOPANLAW_CONSTANT_POWER == panLaw && 1 == m_sourceFormat.GetChannelCount() )
        {
            const Float32 angle = ( pan + 1.0f ) * 0.78539816339f;       // pi / 4, so 0 .. pi / 2 across the range
            gl = 1.41421356f * cosf( angle );
            gr = 1.41421356f * sinf( angle );
        }
        else
        {
            if ( pan > 0.0f ) gl = 1.0f - pan;
            if ( pan < 0.0f ) gr = 1.0f + pan;
        }
        gains[ left ] *= gl;
        gains[ right ] *= gr;
    }

    // Changes in gain are spread over the block, so they do not click
    if ( !m_gainsKnown )
    {
        for ( UInt32 c=0; c<limited; ++c )
            m_lastGains[ c ] = gains[ c ];
        m_gainsKnown = true;
    }
    const Float32* block = &m_blockBuffer[ 0 ];
    const Float32 inverseFrames = 1.0f / (Float32) frames;
    for ( UInt32 f=0; f<frames; ++f )
    {
        const Float32 t = (Float32) ( f + 1 ) * inverseFrames;
        for ( UInt32 c=0; c<mixChannels; ++c )
        {
            const Float32 from = ( c < limited ) ? m_lastGains[ c ] : volume;
            const Float32 to = ( c < limited ) ? gains[ c ] : volume;
            busBuffer[ (size_t) f * mixChannels + c ] += block[ (size_t) f * mixChannels + c ] * ( from + ( to - from ) * t );
        }
    }
    for ( UInt32 c=0; c<limited; ++c )
        m_lastGains[ c ] = gains[ c ];

    if ( m_sourceEnded )
        End( AUDIOVOICEEND_FINISHED );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace AUDIO */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
