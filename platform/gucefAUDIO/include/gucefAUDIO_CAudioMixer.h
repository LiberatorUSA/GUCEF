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

#ifndef GUCEF_AUDIO_CAUDIOMIXER_H
#define GUCEF_AUDIO_CAUDIOMIXER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_AUDIO_CAUDIOVOICE_H
#include "gucefAUDIO_CAudioVoice.h"
#define GUCEF_AUDIO_CAUDIOVOICE_H
#endif /* GUCEF_AUDIO_CAUDIOVOICE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace AUDIO {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/** Told by the mixer when a voice has ended. Called on the thread that mixes, so the implementation has to be quick and must not wait */
class GUCEF_AUDIO_PUBLIC_CPP IAudioMixerListener
{
    public:

    virtual ~IAudioMixerListener() {}

    virtual void OnVoiceEnded( CAudioVoice* voice, TAudioVoiceEndReason reason ) = 0;
};

/*-------------------------------------------------------------------------*/

/**
 *  Adds up all playing voices through the bus tree into one stream of float audio. This is the engine's
 *  inner loop: no locks, no allocation and no IO in Mix(), only the arithmetic. Everything that needs memory is
 *  set up beforehand (voices are prepared when they are added, buses when they are attached, and room for the
 *  maximum number of voices and buses is reserved), so Mix() is safe to call from a real time audio thread.
 *
 *  For the same voices and settings Mix() always makes the same audio, whatever size the pieces are that it is
 *  asked for (up to rounding in the last bit), as long as no setting changes in between: a change in volume or
 *  pan is spread over the piece of audio that is mixed next, so how the audio is cut up decides the shape of
 *  the fade. The tests rely on this.
 *
 *  Mixing is single threaded: AddVoice(), AttachBus() and Mix() are called by one thread at a time. The engine
 *  puts the thread rules around it. The voices and buses are owned by the caller and must outlive their time in the mixer.
 */
class GUCEF_AUDIO_PUBLIC_CPP CAudioMixer
{
    public:

    /**
     *  @param mixFormat        what Mix() produces: float32 and the speaker layout of the output. The rate is the rate of the output
     *  @param maxBlockFrames   largest piece mixed in one go (larger requests are mixed in pieces)
     *  @param maxVoices        number of voices that can play at the same time
     *  @param maxBuses         number of buses, the master bus included
     */
    CAudioMixer( const CAudioFormat& mixFormat, UInt32 maxBlockFrames = 4096, UInt32 maxVoices = 256, UInt32 maxBuses = 64 );

    ~CAudioMixer();

    const CAudioFormat& GetMixFormat( void ) const;

    /** The top of the bus tree, always there */
    CAudioBus* GetMasterBus( void );

    /** Makes a bus below 'parent' (the master bus when null). Owner thread, not while Mix() runs. Null when the bus limit is reached */
    CAudioBus* CreateBus( const CString& name, CAudioBus* parent = GUCEF_NULL );

    /** Allocates what a bus needs to be mixed. Done on the owner thread, before AttachBus() */
    bool PrepareBus( CAudioBus* bus );

    /**
     *  Adds a prepared bus to the tree below its parent, which has to be in the tree already. Allocates nothing,
     *  so it is safe on the audio thread. False when the bus is not prepared, the parent is unknown or the limit is reached.
     */
    bool AttachBus( const TAudioBusPtr& bus );

    CAudioBus* FindBus( const CString& name );

    UInt32 GetBusCount( void ) const;

    /** Prepares a voice for this mixer. Done on the owner thread, before AddVoice() */
    bool PrepareVoice( CAudioVoice* voice );

    /** Starts mixing a prepared voice. False when the maximum number of voices play, or the voice was not prepared */
    bool AddVoice( CAudioVoice* voice );

    UInt32 GetVoiceCount( void ) const;

    void SetListener( IAudioMixerListener* listener );

    void SetPanLaw( TAudioPanLaw panLaw );

    /**
     *  With the limiter on, the master output is turned down just enough when it would go over full level (instantly,
     *  for the frames that need it) and comes back up over about 50 ms, instead of being cut off at full level, which
     *  is heard as distortion. Off by default here; the engine switches it on by default (CAudioEngineParams)
     */
    void SetLimiterEnabled( bool enabled );
    bool IsLimiterEnabled( void ) const;

    /** Mixes 'frames' frames of interleaved float audio into 'output' (replaces what is there). Samples are limited to -1.0 .. 1.0 */
    void Mix( Float32* output, UInt32 frames );

    /** Total frames mixed so far */
    UInt64 GetTotalFramesMixed( void ) const;

    /** Number of samples of the master output that had to be cut off because they went over full level */
    UInt64 GetClippedSampleCount( void ) const;

    /** Number of frames the limiter turned down */
    UInt64 GetLimitedFrameCount( void ) const;

    private:

    CAudioMixer( const CAudioMixer& src );              /**< not implemented */
    CAudioMixer& operator=( const CAudioMixer& src );   /**< not implemented */

    void MixBlock( Float32* output, UInt32 frames );

    private:

    CAudioFormat m_mixFormat;
    UInt32 m_maxBlock;
    UInt32 m_maxVoices;
    UInt32 m_maxBuses;
    GUCEF::vector< TAudioBusPtr > m_buses;      /**< parents come before their children */
    GUCEF::vector< CAudioVoice* > m_voices;
    IAudioMixerListener* m_listener;
    TAudioPanLaw m_panLaw;
    UInt64 m_totalFrames;
    UInt64 m_clipped;
    bool m_limiterEnabled;
    Float32 m_limiterGain;                      /**< the gain the limiter applies now, 1 when it does nothing */
    Float32 m_limiterRelease;                   /**< how much of the way back to 1 the gain goes per frame */
    UInt64 m_limited;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace AUDIO */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_AUDIO_CAUDIOMIXER_H ? */
