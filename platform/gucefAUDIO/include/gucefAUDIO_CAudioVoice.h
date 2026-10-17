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

#ifndef GUCEF_AUDIO_CAUDIOVOICE_H
#define GUCEF_AUDIO_CAUDIOVOICE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTSHAREDPTR_H
#include "CTSharedPtr.h"
#define GUCEF_CORE_CTSHAREDPTR_H
#endif /* GUCEF_CORE_CTSHAREDPTR_H ? */

#ifndef GUCEF_AUDIO_CAUDIOBUS_H
#include "gucefAUDIO_CAudioBus.h"
#define GUCEF_AUDIO_CAUDIOBUS_H
#endif /* GUCEF_AUDIO_CAUDIOBUS_H ? */

#ifndef GUCEF_AUDIO_IAUDIOSOURCE_H
#include "gucefAUDIO_IAudioSource.h"
#define GUCEF_AUDIO_IAUDIOSOURCE_H
#endif /* GUCEF_AUDIO_IAUDIOSOURCE_H ? */

#ifndef GUCEF_AUDIO_CLINEARRESAMPLER_H
#include "gucefAUDIO_CLinearResampler.h"
#define GUCEF_AUDIO_CLINEARRESAMPLER_H
#endif /* GUCEF_AUDIO_CLINEARRESAMPLER_H ? */

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

class CAudioMixer;

/*-------------------------------------------------------------------------*/

/** Where a voice is in its life */
enum EAudioVoiceState
{
    AUDIOVOICESTATE_IDLE = 0 ,      /**< made but not playing yet */
    AUDIOVOICESTATE_PLAYING  ,
    AUDIOVOICESTATE_PAUSED   ,
    AUDIOVOICESTATE_ENDED           /**< finished, stopped or lost: it will not play again */
};
typedef enum EAudioVoiceState TAudioVoiceState;

/** Why a voice ended */
enum EAudioVoiceEndReason
{
    AUDIOVOICEEND_NONE = 0 ,
    AUDIOVOICEEND_FINISHED ,        /**< the sound played to its end (all repeats included) */
    AUDIOVOICEEND_STOPPED  ,        /**< somebody stopped it */
    AUDIOVOICEEND_FAILED            /**< it could not be played (the source gave nothing at all, or does not fit the mixer) */
};
typedef enum EAudioVoiceEndReason TAudioVoiceEndReason;

/** How a sound is placed between the left and the right speaker */
enum EAudioPanLaw
{
    AUDIOPANLAW_CONSTANT_POWER = 0 ,    /**< a mono sound keeps its loudness as it moves, centred it is the same as without panning. The usual choice */
    AUDIOPANLAW_BALANCE                 /**< the side you move away from is turned down, the other side is left alone */
};
typedef enum EAudioPanLaw TAudioPanLaw;

/*-------------------------------------------------------------------------*/

/**
 *  One sound that is playing (or about to): a source, the bus it plays into and its settings.
 *  The settings can be changed from any thread at any time and are heard smoothly. What is going on inside
 *  (reading, converting, resampling) belongs to the mixer on the audio thread.
 */
class GUCEF_AUDIO_PUBLIC_CPP CAudioVoice
{
    public:

    typedef CORE::CTSharedPtr< CAudioVoice, MT::CMutex > TAudioVoicePtr;

    explicit CAudioVoice( const TAudioSourcePtr& source );

    ~CAudioVoice();

    const TAudioSourcePtr& GetSource( void ) const;

    /** The bus the voice plays into, the master bus when none is set. Set before the voice starts */
    void SetBus( CAudioBus* bus );
    CAudioBus* GetBus( void ) const;

    /** Linear volume, 1.0 is unchanged */
    void SetVolume( Float32 volume );
    Float32 GetVolume( void ) const;

    /** Position between the speakers, -1.0 all the way left, 0.0 centre, 1.0 all the way right */
    void SetPan( Float32 pan );
    Float32 GetPan( void ) const;

    /** Speed of playback as a factor: 1.0 normal, 2.0 twice as fast and an octave higher, 0.5 half speed. Limited to 0.01 .. 100 */
    void SetPlaybackRate( Float32 rate );
    Float32 GetPlaybackRate( void ) const;

    /** How many times the sound plays: 1 once (default), 3 three times in a row, 0 until stopped. Needs a source that can seek */
    void SetLoopCount( UInt32 plays );
    UInt32 GetLoopCount( void ) const;

    TAudioVoiceState GetState( void ) const;

    TAudioVoiceEndReason GetEndReason( void ) const;

    /** How far the sound has played, in frames of the source, counting from the start of the current repeat. Updated every piece of audio that is mixed */
    UInt64 GetPositionInFrames( void ) const;

    /** Asks for the voice to stop. The mixer ends it at its next piece of audio */
    void RequestStop( void );

    /** Asks for the voice to pause or to go on again */
    void RequestPause( bool paused );

    bool IsPauseRequested( void ) const;

    bool IsStopRequested( void ) const;

    private:
    friend class CAudioMixer;

    CAudioVoice( const CAudioVoice& src );              /**< not implemented */
    CAudioVoice& operator=( const CAudioVoice& src );   /**< not implemented */

    /** Sets up what the voice needs to be mixed in the given format: owner thread, allocates. False if the source cannot be mixed in it */
    bool Prepare( const CAudioFormat& mixFormat, UInt32 maxBlockFrames );

    /** Mixes the next 'frames' frames of the voice into 'busBuffer' (adds). Audio thread only, never allocates */
    void RenderBlock( Float32* busBuffer, UInt32 frames, TAudioPanLaw panLaw );

    void End( TAudioVoiceEndReason reason );

    /** Fills m_blockBuffer with 'frames' frames in the mix format, as far as the source gives them. Returns the number of frames made */
    UInt32 GatherBlock( UInt32 frames );

    /** Reads up to 'frames' frames from the source into m_convertedBuffer as floats in the mix channels. Returns the number read */
    UInt32 ReadAndConvert( UInt32 frames, bool sameChannels );

    /**
     *  Called when the source gave nothing: starts the next repeat or notes that the sound is over.
     *  Returns true when there is new audio to read, false when the block has to be left as it is.
     */
    bool HandleSourceEnd( void );

    private:

    TAudioSourcePtr m_source;
    CAudioBus* m_bus;
    CAtomicFloat m_volume;
    CAtomicFloat m_pan;
    CAtomicFloat m_playbackRate;
    MT::CTAtomic< UInt32 > m_loopCount;
    MT::CTAtomic< UInt32 > m_state;
    MT::CTAtomic< UInt32 > m_endReason;
    MT::CTAtomic< UInt64 > m_position;
    MT::CTAtomic< bool > m_stopRequested;
    MT::CTAtomic< bool > m_pauseRequested;

    // mixer side, audio thread only after Prepare()
    CAudioFormat m_sourceFormat;
    CAudioFormat m_mixFormat;
    GUCEF::vector< Float32 > m_matrix;
    GUCEF::vector< UInt8 > m_rawBuffer;
    GUCEF::vector< Float32 > m_rawFloatBuffer;
    GUCEF::vector< Float32 > m_convertedBuffer;     /**< source frames converted to float and remixed to the mix channels */
    GUCEF::vector< Float32 > m_blockBuffer;         /**< the block that is mixed, in the mix format */
    CLinearResampler m_resampler;
    UInt32 m_maxBlock;
    UInt32 m_pendingInput;                          /**< frames waiting in m_convertedBuffer for the resampler */
    UInt32 m_playsDone;
    UInt64 m_framesThisPlay;                        /**< frames read from the source since this repeat started */
    Float64 m_playedPosition;                       /**< where in the source the sound is, in source frames, the part that has actually been mixed */
    GUCEF::vector< Float32 > m_lastGains;           /**< gain per mix channel at the end of the previous block, for smooth changes */
    bool m_gainsKnown;
    bool m_prepared;
    bool m_sourceEnded;
    Float64 m_resamplerRatio;
};

typedef CAudioVoice::TAudioVoicePtr TAudioVoicePtr;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace AUDIO */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_AUDIO_CAUDIOVOICE_H ? */
