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

#include "gucefAUDIO_CAudioMixer.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace AUDIO {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CAudioMixer::CAudioMixer( const CAudioFormat& mixFormat, UInt32 maxBlockFrames, UInt32 maxVoices, UInt32 maxBuses )
    : m_mixFormat( mixFormat )
    , m_maxBlock( maxBlockFrames > 0 ? maxBlockFrames : 1 )
    , m_maxVoices( maxVoices )
    , m_maxBuses( maxBuses > 0 ? maxBuses : 1 )
    , m_buses()
    , m_voices()
    , m_listener( GUCEF_NULL )
    , m_panLaw( AUDIOPANLAW_CONSTANT_POWER )
    , m_totalFrames( 0 )
    , m_clipped( 0 )
    , m_limiterEnabled( false )
    , m_limiterGain( 1.0f )
    , m_limiterRelease( 1.0f - (Float32) exp( -1.0 / ( 0.05 * ( 0 != mixFormat.GetSampleRate() ? mixFormat.GetSampleRate() : 48000 ) ) ) )
    , m_limited( 0 )
{GUCEF_TRACE;

    // The mix format is always float
    m_mixFormat.SetSampleType( SAMPLETYPE_FLOAT32 );

    // Everything that may grow while mixing is given its room now
    m_buses.reserve( m_maxBuses );
    m_voices.reserve( m_maxVoices );

    TAudioBusPtr master( GUCEF_NEW CAudioBus( "master", GUCEF_NULL ) );
    master->m_buffer.assign( (size_t) m_maxBlock * m_mixFormat.GetChannelCount(), 0.0f );
    m_buses.push_back( master );
}

/*-------------------------------------------------------------------------*/

CAudioMixer::~CAudioMixer()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

const CAudioFormat&
CAudioMixer::GetMixFormat( void ) const
{GUCEF_TRACE;

    return m_mixFormat;
}

/*-------------------------------------------------------------------------*/

CAudioBus*
CAudioMixer::GetMasterBus( void )
{GUCEF_TRACE;

    return m_buses[ 0 ].GetPointerAlways();
}

/*-------------------------------------------------------------------------*/

CAudioBus*
CAudioMixer::CreateBus( const CString& name, CAudioBus* parent )
{GUCEF_TRACE;

    if ( GUCEF_NULL == parent )
        parent = GetMasterBus();

    TAudioBusPtr bus( GUCEF_NEW CAudioBus( name, parent ) );
    if ( !PrepareBus( bus.GetPointerAlways() ) || !AttachBus( bus ) )
        return GUCEF_NULL;
    return bus.GetPointerAlways();
}

/*-------------------------------------------------------------------------*/

bool
CAudioMixer::PrepareBus( CAudioBus* bus )
{GUCEF_TRACE;

    if ( GUCEF_NULL == bus )
        return false;
    bus->m_buffer.assign( (size_t) m_maxBlock * m_mixFormat.GetChannelCount(), 0.0f );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CAudioMixer::AttachBus( const TAudioBusPtr& bus )
{GUCEF_TRACE;

    if ( bus.IsNULL() || m_buses.size() >= m_maxBuses )
        return false;
    if ( bus->m_buffer.size() != (size_t) m_maxBlock * m_mixFormat.GetChannelCount() )
        return false;

    // a parent has to be one of ours, the order in the list is what makes mixing children before parents work
    bool known = false;
    for ( size_t i=0; i<m_buses.size(); ++i )
        if ( m_buses[ i ].GetPointerAlways() == bus->GetParent() )
            known = true;
    if ( !known )
        return false;

    m_buses.push_back( bus );
    return true;
}

/*-------------------------------------------------------------------------*/

CAudioBus*
CAudioMixer::FindBus( const CString& name )
{GUCEF_TRACE;

    for ( size_t i=0; i<m_buses.size(); ++i )
        if ( m_buses[ i ]->GetName() == name )
            return m_buses[ i ].GetPointerAlways();
    return GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

UInt32
CAudioMixer::GetBusCount( void ) const
{GUCEF_TRACE;

    return (UInt32) m_buses.size();
}

/*-------------------------------------------------------------------------*/

bool
CAudioMixer::PrepareVoice( CAudioVoice* voice )
{GUCEF_TRACE;

    if ( GUCEF_NULL == voice )
        return false;
    if ( GUCEF_NULL == voice->GetBus() )
        voice->SetBus( GetMasterBus() );
    return voice->Prepare( m_mixFormat, m_maxBlock );
}

/*-------------------------------------------------------------------------*/

bool
CAudioMixer::AddVoice( CAudioVoice* voice )
{GUCEF_TRACE;

    if ( GUCEF_NULL == voice || !voice->m_prepared || m_voices.size() >= m_maxVoices )
        return false;
    if ( GUCEF_NULL == voice->GetBus() )
        voice->SetBus( GetMasterBus() );

    voice->m_state.Store( AUDIOVOICESTATE_PLAYING, MT::MEMORDER_RELEASE );
    m_voices.push_back( voice );
    return true;
}

/*-------------------------------------------------------------------------*/

UInt32
CAudioMixer::GetVoiceCount( void ) const
{GUCEF_TRACE;

    return (UInt32) m_voices.size();
}

/*-------------------------------------------------------------------------*/

void
CAudioMixer::SetListener( IAudioMixerListener* listener )
{GUCEF_TRACE;

    m_listener = listener;
}

/*-------------------------------------------------------------------------*/

void
CAudioMixer::SetPanLaw( TAudioPanLaw panLaw )
{GUCEF_TRACE;

    m_panLaw = panLaw;
}

/*-------------------------------------------------------------------------*/

UInt64
CAudioMixer::GetTotalFramesMixed( void ) const
{GUCEF_TRACE;

    return m_totalFrames;
}

/*-------------------------------------------------------------------------*/

void
CAudioMixer::SetLimiterEnabled( bool enabled )
{GUCEF_TRACE;

    m_limiterEnabled = enabled;
    m_limiterGain = 1.0f;
}

/*-------------------------------------------------------------------------*/

bool
CAudioMixer::IsLimiterEnabled( void ) const
{GUCEF_TRACE;

    return m_limiterEnabled;
}

/*-------------------------------------------------------------------------*/

UInt64
CAudioMixer::GetLimitedFrameCount( void ) const
{GUCEF_TRACE;

    return m_limited;
}

/*-------------------------------------------------------------------------*/

UInt64
CAudioMixer::GetClippedSampleCount( void ) const
{GUCEF_TRACE;

    return m_clipped;
}

/*-------------------------------------------------------------------------*/

void
CAudioMixer::Mix( Float32* output, UInt32 frames )
{GUCEF_TRACE;

    const UInt32 channels = m_mixFormat.GetChannelCount();
    while ( frames > 0 )
    {
        const UInt32 block = ( frames < m_maxBlock ) ? frames : m_maxBlock;
        MixBlock( output, block );
        output += (size_t) block * channels;
        frames -= block;
    }
}

/*-------------------------------------------------------------------------*/

void
CAudioMixer::MixBlock( Float32* output, UInt32 frames )
{GUCEF_TRACE;

    const UInt32 channels = m_mixFormat.GetChannelCount();
    const size_t samples = (size_t) frames * channels;

    // 1. every bus starts the block empty
    for ( size_t b=0; b<m_buses.size(); ++b )
        memset( &m_buses[ b ]->m_buffer[ 0 ], 0, samples * sizeof( Float32 ) );

    // 2. the voices add themselves to the bus they play into. Voices that are over are taken out and reported
    size_t v = 0;
    while ( v < m_voices.size() )
    {
        CAudioVoice* voice = m_voices[ v ];
        CAudioBus* bus = voice->GetBus();
        voice->RenderBlock( &bus->m_buffer[ 0 ], frames, m_panLaw );

        if ( AUDIOVOICESTATE_ENDED == voice->GetState() )
        {
            m_voices.erase( m_voices.begin() + v );
            if ( GUCEF_NULL != m_listener )
                m_listener->OnVoiceEnded( voice, voice->GetEndReason() );
        }
        else
            ++v;
    }

    // 3. buses pass what they hold on to their parent, the children first. The master bus is the output
    for ( size_t b=m_buses.size(); b>0; --b )
    {
        CAudioBus* bus = m_buses[ b - 1 ].GetPointerAlways();
        const Float32 target = bus->GetEffectiveVolume();
        const Float32 from = bus->m_gainKnown ? bus->m_lastGain : target;
        bus->m_lastGain = target;
        bus->m_gainKnown = true;

        Float32* data = &bus->m_buffer[ 0 ];
        const Float32 step = ( target - from ) / (Float32) frames;
        if ( from == target )
        {
            if ( 1.0f != target )
                for ( size_t i=0; i<samples; ++i )
                    data[ i ] *= target;
        }
        else
        {
            for ( UInt32 f=0; f<frames; ++f )
            {
                const Float32 g = from + step * (Float32) ( f + 1 );
                for ( UInt32 c=0; c<channels; ++c )
                    data[ (size_t) f * channels + c ] *= g;
            }
        }

        if ( GUCEF_NULL != bus->GetParent() )
        {
            Float32* parent = &bus->GetParent()->m_buffer[ 0 ];
            for ( size_t i=0; i<samples; ++i )
                parent[ i ] += data[ i ];
        }
    }

    // 4. the master bus is what comes out, limited to what a speaker can be given
    Float32* master = &m_buses[ 0 ]->m_buffer[ 0 ];
    if ( m_limiterEnabled )
    {
        // just below full level, so what the limiter lets through is never cut off below
        static const Float32 ceiling = 0.98f;
        for ( UInt32 f=0; f<frames; ++f )
        {
            Float32* frame = master + (size_t) f * channels;
            Float32 peak = 0.0f;
            for ( UInt32 c=0; c<channels; ++c )
            {
                const Float32 level = frame[ c ] < 0.0f ? -frame[ c ] : frame[ c ];
                if ( level > peak )
                    peak = level;
            }

            // down at once as far as this frame needs, back up gradually
            m_limiterGain += ( 1.0f - m_limiterGain ) * m_limiterRelease;
            if ( peak * m_limiterGain > ceiling )
                m_limiterGain = ceiling / peak;
            if ( m_limiterGain < 1.0f )
            {
                ++m_limited;
                for ( UInt32 c=0; c<channels; ++c )
                    frame[ c ] *= m_limiterGain;
            }
        }
    }
    for ( size_t i=0; i<samples; ++i )
    {
        Float32 sample = master[ i ];
        if ( sample > 1.0f )
        {
            sample = 1.0f;
            ++m_clipped;
        }
        else if ( sample < -1.0f )
        {
            sample = -1.0f;
            ++m_clipped;
        }
        else if ( sample != sample )
            sample = 0.0f;
        output[ i ] = sample;
    }
    m_totalFrames += frames;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace AUDIO */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
