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

#ifndef GUCEF_AUDIO_CAUDIOENGINE_H
#define GUCEF_AUDIO_CAUDIOENGINE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <map>

#ifndef GUCEF_CORE_CTSGNOTIFIER_H
#include "CTSGNotifier.h"
#define GUCEF_CORE_CTSGNOTIFIER_H
#endif /* GUCEF_CORE_CTSGNOTIFIER_H ? */

#ifndef GUCEF_CORE_CFUTURERESULTSOURCE_H
#include "gucefCORE_CFutureResultSource.h"
#define GUCEF_CORE_CFUTURERESULTSOURCE_H
#endif /* GUCEF_CORE_CFUTURERESULTSOURCE_H ? */

#ifndef GUCEF_CORE_CTASKCONSUMER_H
#include "gucefCORE_CTaskConsumer.h"
#define GUCEF_CORE_CTASKCONSUMER_H
#endif /* GUCEF_CORE_CTASKCONSUMER_H ? */

#ifndef GUCEF_MT_CTSPSCQUEUE_H
#include "gucefMT_CTSpscQueue.h"
#define GUCEF_MT_CTSPSCQUEUE_H
#endif /* GUCEF_MT_CTSPSCQUEUE_H ? */

#ifndef GUCEF_AUDIO_CAUDIOENGINEPARAMS_H
#include "gucefAUDIO_CAudioEngineParams.h"
#define GUCEF_AUDIO_CAUDIOENGINEPARAMS_H
#endif /* GUCEF_AUDIO_CAUDIOENGINEPARAMS_H ? */

#ifndef GUCEF_AUDIO_CAUDIOMIXER_H
#include "gucefAUDIO_CAudioMixer.h"
#define GUCEF_AUDIO_CAUDIOMIXER_H
#endif /* GUCEF_AUDIO_CAUDIOMIXER_H ? */

#ifndef GUCEF_AUDIO_CAUDIODEVICE_H
#include "gucefAUDIO_CAudioDevice.h"
#define GUCEF_AUDIO_CAUDIODEVICE_H
#endif /* GUCEF_AUDIO_CAUDIODEVICE_H ? */

#ifndef GUCEF_AUDIO_CAUDIOCLIP_H
#include "gucefAUDIO_CAudioClip.h"
#define GUCEF_AUDIO_CAUDIOCLIP_H
#endif /* GUCEF_AUDIO_CAUDIOCLIP_H ? */

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

class CAudioEngineUpdateTask;

/*-------------------------------------------------------------------------*/

/**
 *  Plays sounds: owns an audio device, the mixer with its buses and the voices that play, and keeps the
 *  thread of the application and the real time audio thread apart.
 *
 *  Calls are of three kinds, named by how they work:
 *   - inline: done when the call returns (CreateVoice(), CreateBus(), the settings of voices and buses)
 *   - commands for the audio thread, which return a CFutureResult that is ready when the audio thread has
 *     carried the command out: Start(), Stop(), Pause(), Resume()
 *   - events, raised as notifier events when something happens in the audio thread: VoiceEndedEvent, DeviceLostEvent
 *
 *  The audio thread never waits for the application and never allocates. The application never waits for the
 *  audio thread, except in WaitFor() which is for tests and simple programs.
 *
 *  In threaded mode a dedicated thread, known to the task manager, completes futures and raises events as they
 *  happen. In cooperative mode that is done in Update(), which the application calls regularly (every frame, say).
 *  Everything else works the same, and the audio itself never depends on Update() being called.
 *
 *  All member functions can be called from any thread.
 */
class GUCEF_AUDIO_PUBLIC_CPP CAudioEngine : public CORE::CTSGNotifier ,
                                            private IAudioDeviceCallback ,
                                            private IAudioMixerListener
{
    public:

    /** Raised (from Update, in the thread that runs it, or passed on to the main thread) when a voice has ended. Data: a TVoiceEventData */
    static const CORE::CEvent VoiceEndedEvent;

    /** Raised when the device is gone (unplugged, driver failure). The engine stays as it is, a new engine can be started for another device */
    static const CORE::CEvent DeviceLostEvent;

    typedef CORE::CTCloneableObj< TAudioVoicePtr > TVoiceEventData;

    static void RegisterEvents( void );

    CAudioEngine( void );

    virtual ~CAudioEngine() GUCEF_VIRTUAL_OVERRIDE;

    /** Opens the device and starts the engine. False if no device could be opened or the engine is running already */
    bool Initialize( const CAudioEngineParams& params = CAudioEngineParams() );

    /** Stops everything. Futures that are not done yet are cancelled. Called by the destructor as well */
    void Shutdown( void );

    bool IsInitialized( void ) const;

    /** Name of the driver that is in use, empty when the engine is not initialized */
    CString GetDriverName( void ) const;

    /** The format the device plays in, and the format the mixer works in: float, with the speakers and rate of the device */
    CAudioFormat GetDeviceFormat( void ) const;
    CAudioFormat GetMixFormat( void ) const;

    TAudioDevicePtr GetDevice( void ) const;

    bool IsThreaded( void ) const;

    bool StartDevice( void );
    bool StopDevice( void );

    /** The top of the bus tree. Valid until Shutdown() */
    CAudioBus* GetMasterBus( void );

    /** Makes a bus below 'parent' (the master bus when null). Null when the limit of buses is reached or the parent is not a bus of this engine */
    CAudioBus* CreateBus( const CString& name, CAudioBus* parent = GUCEF_NULL );

    CAudioBus* FindBus( const CString& name );

    /** A voice for a source (or a clip), to play into 'bus' (the master bus when null). Nothing is audible until Start() */
    TAudioVoicePtr CreateVoice( const TAudioSourcePtr& source, CAudioBus* bus = GUCEF_NULL );
    TAudioVoicePtr CreateVoice( const TAudioClipPtr& clip, CAudioBus* bus = GUCEF_NULL );

    /**
     *  Starts a voice. The future is ready when the audio thread has started mixing it. When it cannot be started the
     *  future is in an error status, so HasNoFuture() is true:
     *   - TASKSTATUS_RESOURCE_LIMIT_REACHED at once when too many voices are on their way or playing: a throttle, it is
     *     worth trying again later, and the future stays pending as for the thread pool
     *   - TASKSTATUS_QUEUEING_FAILED at once when the command queue to the audio thread is full
     *   - TASKSTATUS_SETUP_FAILED / TASKSTATUS_TASK_WITH_TYPE_EXISTS at once for an engine that is not running, a voice that
     *     cannot be prepared, or one that was started before
     *   - TASKSTATUS_STARTUP_FAILED later, when the mixer on the audio thread had no free voice
     */
    CORE::CFutureResult Start( const TAudioVoicePtr& voice );

    /** Makes a voice and starts it. The voice is given back when asked for */
    CORE::CFutureResult Play( const TAudioClipPtr& clip, CAudioBus* bus = GUCEF_NULL, TAudioVoicePtr* voice = GUCEF_NULL );

    /** Ends a voice. The future is ready when the voice has ended. For a voice that is not playing it is ready at once */
    CORE::CFutureResult Stop( const TAudioVoicePtr& voice );

    /** The future is ready when the voice is paused (or resumed). Needs a running device to take effect */
    CORE::CFutureResult Pause( const TAudioVoicePtr& voice );
    CORE::CFutureResult Resume( const TAudioVoicePtr& voice );

    /** A future that is ready when the voice has ended, for a voice that was started. Failed for one that was not */
    CORE::CFutureResult GetEndedResult( const TAudioVoicePtr& voice );

    /** Master volume, same as on the master bus */
    void SetMasterVolume( Float32 volume );
    Float32 GetMasterVolume( void ) const;

    /**
     *  Completes the futures and raises the events that the audio thread has made ready. Done by the engine thread
     *  in threaded mode, so calling it there is optional (and harmless). Returns the number of events that were handled.
     */
    UInt32 Update( void );

    /**
     *  Waits until the future is ready or the time is up. In cooperative mode it keeps calling Update() while it waits, so it is
     *  the way to wait for audio work on the thread that would otherwise have to pump. True when the future is ready.
     */
    bool WaitFor( const CORE::CFutureResult& future, UInt32 timeoutInMs );

    /** Number of voices that are started and not yet reported ended */
    UInt32 GetActiveVoiceCount( void ) const;

    /** Events from the audio thread that were dropped because the event queue was full. Should be 0, a non zero count is a sign the queue sizes are too small */
    UInt64 GetDroppedEventCount( void ) const;

    private:
    friend class CAudioEngineUpdateTask;

    CAudioEngine( const CAudioEngine& src );            /**< not implemented */
    CAudioEngine& operator=( const CAudioEngine& src ); /**< not implemented */

    typedef CORE::CTSharedPtr< CORE::CFutureResultSource, MT::CMutex > TSourcePtr;

    struct SCommand
    {
        enum EType { COMMAND_NONE = 0, COMMAND_START_VOICE, COMMAND_ATTACH_BUS };
        EType type;
        CAudioVoice* voice;
        TAudioBusPtr bus;
        SCommand( void ) : type( COMMAND_NONE ), voice( GUCEF_NULL ), bus() {}
    };

    struct SEvent
    {
        enum EType { EVENT_NONE = 0, EVENT_VOICE_STARTED, EVENT_VOICE_START_FAILED, EVENT_VOICE_ENDED };
        EType type;
        CAudioVoice* voice;
        SEvent( void ) : type( EVENT_NONE ), voice( GUCEF_NULL ) {}
    };

    struct SVoiceEntry
    {
        TAudioVoicePtr voice;
        TSourcePtr startSource;
        TSourcePtr stopSource;
        TSourcePtr pauseSource;
        TSourcePtr endedSource;
        bool pauseTarget;               /**< what the pending pause future waits for: paused (true) or playing again (false) */
        SVoiceEntry( void ) : voice(), startSource(), stopSource(), pauseSource(), endedSource(), pauseTarget( false ) {}
    };

    typedef std::map< CAudioVoice*, SVoiceEntry > TVoiceMap;

    // IAudioDeviceCallback: the audio thread
    virtual void OnRenderAudio( void* buffer, UInt32 frameCount ) GUCEF_VIRTUAL_OVERRIDE;
    virtual void OnCaptureAudio( const void* buffer, UInt32 frameCount ) GUCEF_VIRTUAL_OVERRIDE;
    virtual void OnDeviceLost( void ) GUCEF_VIRTUAL_OVERRIDE;

    // IAudioMixerListener: also the audio thread
    virtual void OnVoiceEnded( CAudioVoice* voice, TAudioVoiceEndReason reason ) GUCEF_VIRTUAL_OVERRIDE;

    void ProcessCommands( void );
    void PostEvent( SEvent::EType type, CAudioVoice* voice );
    void HandleEvent( const SEvent& event );
    void CompleteAllPending( void );
    CORE::CFutureResult MakeFailedFuture( CORE::TTaskStatus status );
    CORE::CFutureResult MakeReadyFuture( void );
    void StopUpdateThread( void );

    private:

    mutable MT::CMutex m_lock;
    bool m_initialized;
    CAudioEngineParams m_params;
    CString m_usedDriverName;
    TAudioDevicePtr m_device;
    CAudioFormat m_deviceFormat;
    CAudioMixer* m_mixer;
    MT::CTSpscQueue< SCommand >* m_commands;
    MT::CTSpscQueue< SEvent >* m_events;
    GUCEF::vector< Float32 > m_scratch;
    UInt32 m_maxBlock;
    bool m_directFloat;
    MT::CTAtomic< bool > m_deviceLost;
    MT::CTAtomic< UInt64 > m_droppedEvents;
    TVoiceMap m_voices;
    UInt32 m_maxOutstanding;
    GUCEF::vector< TAudioBusPtr > m_buses;
    bool m_threaded;
    CORE::CTSharedPtr< CAudioEngineUpdateTask, MT::CMutex > m_updateTask;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace AUDIO */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_AUDIO_CAUDIOENGINE_H ? */
