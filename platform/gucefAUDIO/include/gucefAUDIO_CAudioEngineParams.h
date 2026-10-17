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

#ifndef GUCEF_AUDIO_CAUDIOENGINEPARAMS_H
#define GUCEF_AUDIO_CAUDIOENGINEPARAMS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_AUDIO_CAUDIODEVICEPARAMS_H
#include "gucefAUDIO_CAudioDeviceParams.h"
#define GUCEF_AUDIO_CAUDIODEVICEPARAMS_H
#endif /* GUCEF_AUDIO_CAUDIODEVICEPARAMS_H ? */

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

/** How the engine gets its work done on threads */
enum EAudioThreadingMode
{
    AUDIOTHREADING_AUTO = 0 ,       /**< threaded where the platform has threads, cooperative where it has not */
    AUDIOTHREADING_THREADED ,       /**< a dedicated thread (known to the task manager) completes futures and raises events by itself */
    AUDIOTHREADING_COOPERATIVE      /**< nothing runs but the audio thread of the device: the owner calls CAudioEngine::Update() regularly for futures and events */
};
typedef enum EAudioThreadingMode TAudioThreadingMode;

/**
 *  What the engine is started with. All of it has a default that works, a program that does not care
 *  about the details sets nothing.
 */
class GUCEF_AUDIO_PUBLIC_CPP CAudioEngineParams
{
    public:

    CAudioEngineParams( void );

    CAudioEngineParams( const CAudioEngineParams& src );

    CAudioEngineParams& operator=( const CAudioEngineParams& src );

    ~CAudioEngineParams();

    /** The audio driver to use ("alsa", "null", ...), empty for the best one that works on this machine */
    const CString& GetDriverName( void ) const;
    void SetDriverName( const CString& driverName );

    /** What is asked of the device. The engine adapts to the format it gets */
    const CAudioDeviceParams& GetDeviceParams( void ) const;
    CAudioDeviceParams& GetDeviceParams( void );

    /** Number of sounds that can play at once */
    UInt32 GetMaxVoices( void ) const;
    void SetMaxVoices( UInt32 maxVoices );

    /** Number of buses, the master bus included */
    UInt32 GetMaxBuses( void ) const;
    void SetMaxBuses( UInt32 maxBuses );

    /** Largest piece of audio mixed in one go, in frames. Memory per sound and bus depends on it */
    UInt32 GetMaxBlockFrames( void ) const;
    void SetMaxBlockFrames( UInt32 frames );

    /** Room for commands on their way to the audio thread, and for events on their way back. Both are fixed for the life of the engine */
    UInt32 GetCommandQueueSize( void ) const;
    void SetCommandQueueSize( UInt32 size );
    UInt32 GetEventQueueSize( void ) const;
    void SetEventQueueSize( UInt32 size );

    TAudioThreadingMode GetThreadingMode( void ) const;
    void SetThreadingMode( TAudioThreadingMode mode );

    /** Threaded mode: how often the engine thread looks for futures to complete and events to raise */
    UInt32 GetUpdateIntervalInMs( void ) const;
    void SetUpdateIntervalInMs( UInt32 milliseconds );

    /** Threaded mode: the name of the task manager thread pool the engine thread lives in */
    const CString& GetThreadPoolName( void ) const;
    void SetThreadPoolName( const CString& name );

    /** True (default) to start the device when the engine is initialized */
    bool GetStartDevice( void ) const;
    void SetStartDevice( bool start );

    TAudioPanLaw GetPanLaw( void ) const;
    void SetPanLaw( TAudioPanLaw panLaw );

    private:

    CString m_driverName;
    CAudioDeviceParams m_deviceParams;
    UInt32 m_maxVoices;
    UInt32 m_maxBuses;
    UInt32 m_maxBlockFrames;
    UInt32 m_commandQueueSize;
    UInt32 m_eventQueueSize;
    TAudioThreadingMode m_threadingMode;
    UInt32 m_updateIntervalInMs;
    CString m_threadPoolName;
    bool m_startDevice;
    TAudioPanLaw m_panLaw;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace AUDIO */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_AUDIO_CAUDIOENGINEPARAMS_H ? */
