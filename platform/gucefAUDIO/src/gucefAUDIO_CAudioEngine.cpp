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

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_MT_DVMTOSWRAP_H
#include "gucefMT_dvmtoswrap.h"
#define GUCEF_MT_DVMTOSWRAP_H
#endif /* GUCEF_MT_DVMTOSWRAP_H ? */

#ifndef GUCEF_AUDIO_CAUDIOGLOBAL_H
#include "gucefAUDIO_CAudioGlobal.h"
#define GUCEF_AUDIO_CAUDIOGLOBAL_H
#endif /* GUCEF_AUDIO_CAUDIOGLOBAL_H ? */

#ifndef GUCEF_AUDIO_CAUDIODRIVERREGISTRY_H
#include "gucefAUDIO_CAudioDriverRegistry.h"
#define GUCEF_AUDIO_CAUDIODRIVERREGISTRY_H
#endif /* GUCEF_AUDIO_CAUDIODRIVERREGISTRY_H ? */

#ifndef GUCEF_AUDIO_CAUDIOCONVERTER_H
#include "gucefAUDIO_CAudioConverter.h"
#define GUCEF_AUDIO_CAUDIOCONVERTER_H
#endif /* GUCEF_AUDIO_CAUDIOCONVERTER_H ? */

#include "gucefAUDIO_CAudioEngine.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace AUDIO {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CEvent CAudioEngine::VoiceEndedEvent = "GUCEF::AUDIO::CAudioEngine::VoiceEndedEvent";
const CORE::CEvent CAudioEngine::DeviceLostEvent = "GUCEF::AUDIO::CAudioEngine::DeviceLostEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace {

/** The plain pointer of a voice that is held by a shared pointer, which is what the engine tracks voices by */
CAudioVoice*
RawVoice( const TAudioVoicePtr& voice )
{
    TAudioVoicePtr copy( voice );
    return copy.GetPointerAlways();
}

}

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  The thread of the engine in threaded mode: a task of the task manager that keeps a dedicated thread, which is
 *  how the task manager gets to know about every thread. It does one thing, call Update().
 */
class CAudioEngineUpdateTask : public CORE::CTaskConsumer
{
    public:

    CAudioEngineUpdateTask( CAudioEngine* engine, UInt32 intervalInMs )
        : CORE::CTaskConsumer(), m_engine( engine ), m_intervalInMs( intervalInMs ), m_lock(), m_stopped( false ), m_ended( false )
    {GUCEF_TRACE;
    }

    virtual ~CAudioEngineUpdateTask() GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;
    }

    virtual CORE::CString GetType( void ) const GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;
        return "GUCEF::AUDIO::CAudioEngineUpdateTask";
    }

    virtual const CORE::CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;
        static const CORE::CString typeName = "GUCEF::AUDIO::CAudioEngineUpdateTask";
        return typeName;
    }

    virtual bool OnTaskCycle( CORE::CTaskPtr ) GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;

        // The lock is held while the engine is updated, so once Disconnect() has the lock the engine is never touched again
        MT::CScopeMutex lock( m_lock );
        if ( m_stopped )
            return true;
        m_engine->Update();
        RequestTaskCycleDelayInMs( m_intervalInMs );
        return false;           // keep going until asked to stop
    }

    virtual void OnTaskEnded( CORE::CTaskPtr, bool ) GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;

        m_ended.Store( true );
    }

    /** After this returns the task will not call the engine any more, and ends itself at its next cycle */
    void Disconnect( void )
    {GUCEF_TRACE;

        MT::CScopeMutex lock( m_lock );
        m_stopped = true;
    }

    bool HasEnded( void ) const
    {GUCEF_TRACE;

        return m_ended.Load();
    }

    private:

    CAudioEngine* m_engine;
    UInt32 m_intervalInMs;
    MT::CMutex m_lock;
    bool m_stopped;
    MT::CTAtomic< bool > m_ended;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CAudioEngine::RegisterEvents( void )
{GUCEF_TRACE;

    VoiceEndedEvent.Initialize();
    DeviceLostEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CAudioEngine::CAudioEngine( void )
    : CORE::CTSGNotifier()
    , m_lock()
    , m_initialized( false )
    , m_params()
    , m_usedDriverName()
    , m_device()
    , m_deviceFormat()
    , m_mixer( GUCEF_NULL )
    , m_commands( GUCEF_NULL )
    , m_events( GUCEF_NULL )
    , m_scratch()
    , m_maxBlock( 0 )
    , m_directFloat( false )
    , m_deviceLost( false )
    , m_droppedEvents( 0 )
    , m_voices()
    , m_maxOutstanding( 0 )
    , m_buses()
    , m_threaded( false )
    , m_updateTask()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CAudioEngine::~CAudioEngine()
{GUCEF_TRACE;

    Shutdown();
}

/*-------------------------------------------------------------------------*/

bool
CAudioEngine::Initialize( const CAudioEngineParams& params )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    if ( m_initialized )
        return false;

    m_params = params;
    m_params.GetDeviceParams().SetDirection( AUDIODIRECTION_PLAYBACK );

    // Everything the audio thread will touch is made before the device can call it
    m_commands = GUCEF_NEW MT::CTSpscQueue< SCommand >( params.GetCommandQueueSize() );
    m_events = GUCEF_NEW MT::CTSpscQueue< SEvent >( params.GetEventQueueSize() );
    m_maxOutstanding = m_events->GetCapacity() / 3;     // a voice makes at most two events that must not be lost: this keeps the queue from overflowing
    m_deviceLost.Store( false );
    m_droppedEvents.Store( 0 );

    if ( !CAudioGlobal::Instance()->GetAudioDriverRegistry().OpenDevice( m_params.GetDriverName(), m_params.GetDeviceParams(), this, m_device, m_usedDriverName ) || m_device.IsNULL() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "AudioEngine: no audio device could be opened" );
        GUCEF_DELETE m_commands; m_commands = GUCEF_NULL;
        GUCEF_DELETE m_events; m_events = GUCEF_NULL;
        return false;
    }

    m_deviceFormat = m_device->GetFormat();
    if ( !m_deviceFormat.IsValid() )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "AudioEngine: the audio device reports no usable format" );
        m_device.Unlink();
        GUCEF_DELETE m_commands; m_commands = GUCEF_NULL;
        GUCEF_DELETE m_events; m_events = GUCEF_NULL;
        return false;
    }

    // The mixer works in float in the speakers and rate of the device, the conversion to the device sample type is the last step
    m_maxBlock = ( m_params.GetMaxBlockFrames() > 0 ) ? m_params.GetMaxBlockFrames() : 4096;
    m_mixer = GUCEF_NEW CAudioMixer( m_deviceFormat, m_maxBlock, m_params.GetMaxVoices(), m_params.GetMaxBuses() );
    m_mixer->SetListener( this );
    m_mixer->SetPanLaw( m_params.GetPanLaw() );
    m_directFloat = SAMPLETYPE_FLOAT32 == m_deviceFormat.GetSampleType();
    if ( !m_directFloat )
        m_scratch.assign( (size_t) m_maxBlock * m_deviceFormat.GetChannelCount(), 0.0f );

    m_buses.clear();
    m_initialized = true;

    // Threaded: a dedicated task keeps the futures and events moving
    m_threaded = false;
    TAudioThreadingMode mode = m_params.GetThreadingMode();
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_WASM_EMSCRIPTEN )
    if ( AUDIOTHREADING_AUTO == mode )
        mode = AUDIOTHREADING_COOPERATIVE;
    #endif
    if ( AUDIOTHREADING_COOPERATIVE != mode )
    {
        m_updateTask = CORE::CTSharedPtr< CAudioEngineUpdateTask, MT::CMutex >( GUCEF_NEW CAudioEngineUpdateTask( this, m_params.GetUpdateIntervalInMs() ) );
        CORE::ThreadPoolPtr pool = CORE::CCoreGlobal::Instance()->GetTaskManager().GetOrCreateThreadPool( m_params.GetThreadPoolName() );
        CORE::CFutureResult started = pool.IsNULL() ? CORE::CFutureResult( CORE::TTaskStatus::TASKSTATUS_SETUP_FAILED ) : pool->StartTaskWithConsumer( m_updateTask );
        if ( started.HasNoFuture() || started.IsReady() )
        {
            GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "AudioEngine: could not start the engine thread, continuing in cooperative mode" );
            m_updateTask.Unlink();
        }
        else
            m_threaded = true;
    }

    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "AudioEngine: initialized with driver \"" + m_usedDriverName + "\", device format " + m_deviceFormat.ToString() +
                      ( m_threaded ? ", threaded" : ", cooperative" ) );

    if ( m_params.GetStartDevice() )
        m_device->Start();
    return true;
}

/*-------------------------------------------------------------------------*/

void
CAudioEngine::StopUpdateThread( void )
{GUCEF_TRACE;

    if ( !m_updateTask.IsNULL() )
    {
        CORE::CTSharedPtr< CAudioEngineUpdateTask, MT::CMutex > task = m_updateTask;
        m_updateTask.Unlink();
        m_threaded = false;

        // Not under the engine lock: the thread takes it in Update(). Disconnect() waits for an update in progress
        // and guarantees there is none after it, even if the thread has not got round to starting yet
        m_lock.Unlock();
        task->Disconnect();
        task->RequestTaskToStop( false );
        UInt32 waitedInMs = 0;
        while ( !task->HasEnded() && waitedInMs < 3000 )
        {
            MT::PrecisionDelay( 1 );
            ++waitedInMs;
        }
        m_lock.Lock( GUCEF_MT_INFINITE_LOCK_TIMEOUT );
    }
    m_threaded = false;
}

void
CAudioEngine::Shutdown( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    if ( !m_initialized )
        return;

    StopUpdateThread();
    if ( !m_initialized )
        return;                 // somebody else did it while the lock was open
    m_initialized = false;

    // When Stop() returns the audio thread is no longer calling us
    if ( !m_device.IsNULL() )
    {
        m_device->Stop();
        m_device.Unlink();
    }

    CompleteAllPending();
    m_voices.clear();
    m_buses.clear();

    GUCEF_DELETE m_mixer;       m_mixer = GUCEF_NULL;
    GUCEF_DELETE m_commands;    m_commands = GUCEF_NULL;
    GUCEF_DELETE m_events;      m_events = GUCEF_NULL;
    m_scratch.clear();
    m_usedDriverName.Clear();
    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "AudioEngine: shut down" );
}

/*-------------------------------------------------------------------------*/

void
CAudioEngine::CompleteAllPending( void )
{GUCEF_TRACE;

    // Whatever somebody is still waiting for will never happen now
    for ( TVoiceMap::iterator i=m_voices.begin(); i!=m_voices.end(); ++i )
    {
        SVoiceEntry& entry = (*i).second;
        if ( !entry.startSource.IsNULL() )  entry.startSource->Cancel();
        if ( !entry.stopSource.IsNULL() )   entry.stopSource->Cancel();
        if ( !entry.pauseSource.IsNULL() )  entry.pauseSource->Cancel();
        if ( !entry.endedSource.IsNULL() )  entry.endedSource->Cancel();
    }
}

/*-------------------------------------------------------------------------*/

bool
CAudioEngine::IsInitialized( void ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return m_initialized;
}

/*-------------------------------------------------------------------------*/

CString
CAudioEngine::GetDriverName( void ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return m_usedDriverName;
}

/*-------------------------------------------------------------------------*/

CAudioFormat
CAudioEngine::GetDeviceFormat( void ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return m_deviceFormat;
}

/*-------------------------------------------------------------------------*/

CAudioFormat
CAudioEngine::GetMixFormat( void ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return m_initialized ? m_mixer->GetMixFormat() : CAudioFormat();
}

/*-------------------------------------------------------------------------*/

TAudioDevicePtr
CAudioEngine::GetDevice( void ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return m_device;
}

/*-------------------------------------------------------------------------*/

bool
CAudioEngine::IsThreaded( void ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return m_threaded;
}

/*-------------------------------------------------------------------------*/

bool
CAudioEngine::StartDevice( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return m_initialized && m_device->Start();
}

/*-------------------------------------------------------------------------*/

bool
CAudioEngine::StopDevice( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return m_initialized && m_device->Stop();
}

/*-------------------------------------------------------------------------*/

CAudioBus*
CAudioEngine::GetMasterBus( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return m_initialized ? m_mixer->GetMasterBus() : GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

CAudioBus*
CAudioEngine::CreateBus( const CString& name, CAudioBus* parent )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    if ( !m_initialized || m_buses.size() + 1 >= m_params.GetMaxBuses() )
        return GUCEF_NULL;

    CAudioBus* master = m_mixer->GetMasterBus();
    if ( GUCEF_NULL == parent )
        parent = master;
    bool known = ( parent == master );
    for ( size_t i=0; !known && i<m_buses.size(); ++i )
        known = ( m_buses[ i ].GetPointerAlways() == parent );
    if ( !known )
        return GUCEF_NULL;

    // The bus is made here and handed over to the audio thread, which links it into the tree. Commands arrive in order,
    // so a voice that is started on this bus afterwards finds it there
    TAudioBusPtr bus( GUCEF_NEW CAudioBus( name, parent ) );
    if ( !m_mixer->PrepareBus( bus.GetPointerAlways() ) )
        return GUCEF_NULL;
    SCommand command;
    command.type = SCommand::COMMAND_ATTACH_BUS;
    command.bus = bus;
    if ( !m_commands->Push( command ) )
        return GUCEF_NULL;

    m_buses.push_back( bus );
    return bus.GetPointerAlways();
}

/*-------------------------------------------------------------------------*/

CAudioBus*
CAudioEngine::FindBus( const CString& name )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    if ( !m_initialized )
        return GUCEF_NULL;
    if ( name == m_mixer->GetMasterBus()->GetName() )
        return m_mixer->GetMasterBus();
    for ( size_t i=0; i<m_buses.size(); ++i )
        if ( m_buses[ i ]->GetName() == name )
            return m_buses[ i ].GetPointerAlways();
    return GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

TAudioVoicePtr
CAudioEngine::CreateVoice( const TAudioSourcePtr& source, CAudioBus* bus )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    TAudioVoicePtr voice( GUCEF_NEW CAudioVoice( source ) );
    if ( m_initialized )
        voice->SetBus( GUCEF_NULL != bus ? bus : m_mixer->GetMasterBus() );
    return voice;
}

/*-------------------------------------------------------------------------*/

TAudioVoicePtr
CAudioEngine::CreateVoice( const TAudioClipPtr& clip, CAudioBus* bus )
{GUCEF_TRACE;

    if ( clip.IsNULL() )
        return TAudioVoicePtr();
    return CreateVoice( clip->CreateSource(), bus );
}

/*-------------------------------------------------------------------------*/

CORE::CFutureResult
CAudioEngine::MakeFailedFuture( CORE::TTaskStatus status )
{GUCEF_TRACE;

    return CORE::CFutureResult( status );
}

/*-------------------------------------------------------------------------*/

CORE::CFutureResult
CAudioEngine::MakeReadyFuture( void )
{GUCEF_TRACE;

    CORE::CFutureResultSource done;
    done.Complete();
    return done.GetFuture();
}

/*-------------------------------------------------------------------------*/

CORE::CFutureResult
CAudioEngine::Start( const TAudioVoicePtr& voice )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    if ( !m_initialized || voice.IsNULL() )
        return MakeFailedFuture( CORE::TTaskStatus::TASKSTATUS_SETUP_FAILED );
    if ( AUDIOVOICESTATE_IDLE != voice->GetState() || m_voices.find( RawVoice( voice ) ) != m_voices.end() )
        return MakeFailedFuture( CORE::TTaskStatus::TASKSTATUS_TASK_WITH_TYPE_EXISTS );     // already started: a voice plays once
    if ( m_voices.size() >= m_maxOutstanding )
        return MakeFailedFuture( CORE::TTaskStatus::TASKSTATUS_RESOURCE_LIMIT_REACHED );

    CAudioVoice* raw = RawVoice( voice );
    if ( GUCEF_NULL == raw->GetBus() )
        raw->SetBus( m_mixer->GetMasterBus() );
    if ( !m_mixer->PrepareVoice( raw ) )
        return MakeFailedFuture( CORE::TTaskStatus::TASKSTATUS_SETUP_FAILED );

    SCommand command;
    command.type = SCommand::COMMAND_START_VOICE;
    command.voice = raw;
    if ( !m_commands->Push( command ) )
        return MakeFailedFuture( CORE::TTaskStatus::TASKSTATUS_QUEUEING_FAILED );

    SVoiceEntry& entry = m_voices[ raw ];
    entry.voice = voice;
    entry.startSource = TSourcePtr( GUCEF_NEW CORE::CFutureResultSource() );
    return entry.startSource->GetFuture();
}

/*-------------------------------------------------------------------------*/

CORE::CFutureResult
CAudioEngine::Play( const TAudioClipPtr& clip, CAudioBus* bus, TAudioVoicePtr* voiceOut )
{GUCEF_TRACE;

    TAudioVoicePtr voice = CreateVoice( clip, bus );
    if ( GUCEF_NULL != voiceOut )
        *voiceOut = voice;
    if ( voice.IsNULL() )
        return MakeFailedFuture( CORE::TTaskStatus::TASKSTATUS_SETUP_FAILED );
    return Start( voice );
}

/*-------------------------------------------------------------------------*/

CORE::CFutureResult
CAudioEngine::Stop( const TAudioVoicePtr& voice )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    if ( voice.IsNULL() )
        return MakeFailedFuture( CORE::TTaskStatus::TASKSTATUS_SETUP_FAILED );

    TVoiceMap::iterator i = m_voices.find( RawVoice( voice ) );
    if ( i == m_voices.end() )
        return MakeReadyFuture();               // not playing (never started, or over already)

    SVoiceEntry& entry = (*i).second;
    RawVoice( voice )->RequestStop();
    if ( entry.stopSource.IsNULL() )
        entry.stopSource = TSourcePtr( GUCEF_NEW CORE::CFutureResultSource() );
    return entry.stopSource->GetFuture();
}

/*-------------------------------------------------------------------------*/

CORE::CFutureResult
CAudioEngine::Pause( const TAudioVoicePtr& voice )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    if ( voice.IsNULL() )
        return MakeFailedFuture( CORE::TTaskStatus::TASKSTATUS_SETUP_FAILED );
    TVoiceMap::iterator i = m_voices.find( RawVoice( voice ) );
    if ( i == m_voices.end() )
        return MakeFailedFuture( CORE::TTaskStatus::TASKSTATUS_SETUP_FAILED );

    SVoiceEntry& entry = (*i).second;
    if ( !entry.pauseSource.IsNULL() )
        entry.pauseSource->Cancel();            // a newer request replaces the one that was waiting
    RawVoice( voice )->RequestPause( true );
    entry.pauseTarget = true;
    entry.pauseSource = TSourcePtr( GUCEF_NEW CORE::CFutureResultSource() );
    return entry.pauseSource->GetFuture();
}

/*-------------------------------------------------------------------------*/

CORE::CFutureResult
CAudioEngine::Resume( const TAudioVoicePtr& voice )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    if ( voice.IsNULL() )
        return MakeFailedFuture( CORE::TTaskStatus::TASKSTATUS_SETUP_FAILED );
    TVoiceMap::iterator i = m_voices.find( RawVoice( voice ) );
    if ( i == m_voices.end() )
        return MakeFailedFuture( CORE::TTaskStatus::TASKSTATUS_SETUP_FAILED );

    SVoiceEntry& entry = (*i).second;
    if ( !entry.pauseSource.IsNULL() )
        entry.pauseSource->Cancel();
    RawVoice( voice )->RequestPause( false );
    entry.pauseTarget = false;
    entry.pauseSource = TSourcePtr( GUCEF_NEW CORE::CFutureResultSource() );
    return entry.pauseSource->GetFuture();
}

/*-------------------------------------------------------------------------*/

CORE::CFutureResult
CAudioEngine::GetEndedResult( const TAudioVoicePtr& voice )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    if ( voice.IsNULL() )
        return MakeFailedFuture( CORE::TTaskStatus::TASKSTATUS_SETUP_FAILED );

    TVoiceMap::iterator i = m_voices.find( RawVoice( voice ) );
    if ( i == m_voices.end() )
        return AUDIOVOICESTATE_ENDED == voice->GetState() ? MakeReadyFuture() : MakeFailedFuture( CORE::TTaskStatus::TASKSTATUS_SETUP_FAILED );

    SVoiceEntry& entry = (*i).second;
    if ( entry.endedSource.IsNULL() )
        entry.endedSource = TSourcePtr( GUCEF_NEW CORE::CFutureResultSource() );
    return entry.endedSource->GetFuture();
}

/*-------------------------------------------------------------------------*/

void
CAudioEngine::SetMasterVolume( Float32 volume )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    if ( m_initialized )
        m_mixer->GetMasterBus()->SetVolume( volume );
}

/*-------------------------------------------------------------------------*/

Float32
CAudioEngine::GetMasterVolume( void ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return m_initialized ? m_mixer->GetMasterBus()->GetVolume() : 0.0f;
}

/*-------------------------------------------------------------------------*/

UInt32
CAudioEngine::GetActiveVoiceCount( void ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return (UInt32) m_voices.size();
}

/*-------------------------------------------------------------------------*/

UInt64
CAudioEngine::GetDroppedEventCount( void ) const
{GUCEF_TRACE;

    return m_droppedEvents.Load();
}

/*-------------------------------------------------------------------------*/

UInt32
CAudioEngine::Update( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    if ( !m_initialized )
        return 0;

    UInt32 handled = 0;
    SEvent event;
    while ( m_events->Pop( event ) )
    {
        HandleEvent( event );
        ++handled;
    }

    // Pausing and resuming are state changes of the voice, noticed here without any event from the audio thread
    for ( TVoiceMap::iterator i=m_voices.begin(); i!=m_voices.end(); ++i )
    {
        SVoiceEntry& entry = (*i).second;
        if ( !entry.pauseSource.IsNULL() )
        {
            const TAudioVoiceState state = entry.voice->GetState();
            if ( ( entry.pauseTarget && AUDIOVOICESTATE_PAUSED == state ) || ( !entry.pauseTarget && AUDIOVOICESTATE_PLAYING == state ) )
            {
                entry.pauseSource->Complete();
                entry.pauseSource.Unlink();
            }
        }
    }

    if ( m_deviceLost.Exchange( false ) )
    {
        ++handled;
        NotifyObserversFromThread( DeviceLostEvent );
    }
    return handled;
}

/*-------------------------------------------------------------------------*/

void
CAudioEngine::HandleEvent( const SEvent& event )
{GUCEF_TRACE;

    TVoiceMap::iterator i = m_voices.find( event.voice );
    if ( i == m_voices.end() )
        return;
    SVoiceEntry& entry = (*i).second;

    switch ( event.type )
    {
        case SEvent::EVENT_VOICE_STARTED :
        {
            if ( !entry.startSource.IsNULL() )
            {
                entry.startSource->Complete();
                entry.startSource.Unlink();
            }
            break;
        }
        case SEvent::EVENT_VOICE_START_FAILED :
        {
            if ( !entry.startSource.IsNULL() )
                entry.startSource->Fail( CORE::TTaskStatus::TASKSTATUS_STARTUP_FAILED, "no free voice in the mixer" );
            if ( !entry.stopSource.IsNULL() )   entry.stopSource->Complete();
            if ( !entry.pauseSource.IsNULL() )  entry.pauseSource->Cancel();
            if ( !entry.endedSource.IsNULL() )  entry.endedSource->Cancel();
            m_voices.erase( i );
            break;
        }
        case SEvent::EVENT_VOICE_ENDED :
        {
            TAudioVoicePtr voice = entry.voice;
            if ( !entry.startSource.IsNULL() )  entry.startSource->Complete();
            if ( !entry.stopSource.IsNULL() )   entry.stopSource->Complete();
            if ( !entry.pauseSource.IsNULL() )  entry.pauseSource->Cancel();
            if ( !entry.endedSource.IsNULL() )  entry.endedSource->Complete();
            m_voices.erase( i );

            TVoiceEventData data( voice );
            NotifyObserversFromThread( VoiceEndedEvent, &data );
            break;
        }
        default :
            break;
    }
}

/*-------------------------------------------------------------------------*/

bool
CAudioEngine::WaitFor( const CORE::CFutureResult& future, UInt32 timeoutInMs )
{GUCEF_TRACE;

    const UInt64 startTicks = MT::PrecisionTickCount();
    for ( ;; )
    {
        if ( future.IsReady() )
            return true;
        if ( !IsThreaded() )
            Update();
        if ( future.IsReady() )
            return true;
        if ( MT::PrecisionTimerTicksToMs( MT::PrecisionTickCount() - startTicks ) >= timeoutInMs )
            return false;
        MT::PrecisionDelay( 1 );
    }
}

/*-------------------------------------------------------------------------*/

void
CAudioEngine::ProcessCommands( void )
{
    SCommand command;
    while ( m_commands->Pop( command ) )
    {
        switch ( command.type )
        {
            case SCommand::COMMAND_START_VOICE :
            {
                if ( m_mixer->AddVoice( command.voice ) )
                    PostEvent( SEvent::EVENT_VOICE_STARTED, command.voice );
                else
                    PostEvent( SEvent::EVENT_VOICE_START_FAILED, command.voice );
                break;
            }
            case SCommand::COMMAND_ATTACH_BUS :
            {
                m_mixer->AttachBus( command.bus );
                break;
            }
            default :
                break;
        }
        command = SCommand();       // lets go of the reference to the bus
    }
}

/*-------------------------------------------------------------------------*/

void
CAudioEngine::PostEvent( SEvent::EType type, CAudioVoice* voice )
{
    SEvent event;
    event.type = type;
    event.voice = voice;
    if ( !m_events->Push( event ) )
        m_droppedEvents.FetchAdd( 1, MT::MEMORDER_RELAXED );
}

/*-------------------------------------------------------------------------*/

void
CAudioEngine::OnRenderAudio( void* buffer, UInt32 frameCount )
{
    ProcessCommands();

    const UInt32 channels = m_deviceFormat.GetChannelCount();
    const UInt32 frameBytes = m_deviceFormat.GetBytesPerFrame();
    UInt8* out = static_cast< UInt8* >( buffer );
    while ( frameCount > 0 )
    {
        const UInt32 block = ( frameCount < m_maxBlock ) ? frameCount : m_maxBlock;
        if ( m_directFloat )
            m_mixer->Mix( reinterpret_cast< Float32* >( out ), block );
        else
        {
            m_mixer->Mix( &m_scratch[ 0 ], block );
            CAudioConverter::ConvertFromFloat32( &m_scratch[ 0 ], out, m_deviceFormat.GetSampleType(), (UInt64) block * channels );
        }
        out += (size_t) block * frameBytes;
        frameCount -= block;
    }
}

/*-------------------------------------------------------------------------*/

void
CAudioEngine::OnCaptureAudio( const void*, UInt32 )
{
    // the engine only plays
}

/*-------------------------------------------------------------------------*/

void
CAudioEngine::OnDeviceLost( void )
{
    // may come from any thread: only noted here, Update() makes something of it
    m_deviceLost.Store( true );
}

/*-------------------------------------------------------------------------*/

void
CAudioEngine::OnVoiceEnded( CAudioVoice* voice, TAudioVoiceEndReason )
{
    PostEvent( SEvent::EVENT_VOICE_ENDED, voice );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace AUDIO */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
