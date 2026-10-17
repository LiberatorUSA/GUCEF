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

#include "gucefAUDIO_CAudioEngineParams.h"

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

CAudioEngineParams::CAudioEngineParams( void )
    : m_driverName()
    , m_deviceParams()
    , m_maxVoices( 128 )
    , m_maxBuses( 32 )
    , m_maxBlockFrames( 4096 )
    , m_commandQueueSize( 1024 )
    , m_eventQueueSize( 2048 )
    , m_threadingMode( AUDIOTHREADING_AUTO )
    , m_updateIntervalInMs( 5 )
    , m_threadPoolName( "AudioEngine" )
    , m_startDevice( true )
    , m_panLaw( AUDIOPANLAW_CONSTANT_POWER )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CAudioEngineParams::CAudioEngineParams( const CAudioEngineParams& src )
    : m_driverName( src.m_driverName )
    , m_deviceParams( src.m_deviceParams )
    , m_maxVoices( src.m_maxVoices )
    , m_maxBuses( src.m_maxBuses )
    , m_maxBlockFrames( src.m_maxBlockFrames )
    , m_commandQueueSize( src.m_commandQueueSize )
    , m_eventQueueSize( src.m_eventQueueSize )
    , m_threadingMode( src.m_threadingMode )
    , m_updateIntervalInMs( src.m_updateIntervalInMs )
    , m_threadPoolName( src.m_threadPoolName )
    , m_startDevice( src.m_startDevice )
    , m_panLaw( src.m_panLaw )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CAudioEngineParams&
CAudioEngineParams::operator=( const CAudioEngineParams& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        m_driverName = src.m_driverName;
        m_deviceParams = src.m_deviceParams;
        m_maxVoices = src.m_maxVoices;
        m_maxBuses = src.m_maxBuses;
        m_maxBlockFrames = src.m_maxBlockFrames;
        m_commandQueueSize = src.m_commandQueueSize;
        m_eventQueueSize = src.m_eventQueueSize;
        m_threadingMode = src.m_threadingMode;
        m_updateIntervalInMs = src.m_updateIntervalInMs;
        m_threadPoolName = src.m_threadPoolName;
        m_startDevice = src.m_startDevice;
        m_panLaw = src.m_panLaw;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CAudioEngineParams::~CAudioEngineParams()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

const CString&
CAudioEngineParams::GetDriverName( void ) const
{GUCEF_TRACE;

    return m_driverName;
}

/*-------------------------------------------------------------------------*/

void
CAudioEngineParams::SetDriverName( const CString& driverName )
{GUCEF_TRACE;

    m_driverName = driverName;
}

/*-------------------------------------------------------------------------*/

const CAudioDeviceParams&
CAudioEngineParams::GetDeviceParams( void ) const
{GUCEF_TRACE;

    return m_deviceParams;
}

/*-------------------------------------------------------------------------*/

CAudioDeviceParams&
CAudioEngineParams::GetDeviceParams( void )
{GUCEF_TRACE;

    return m_deviceParams;
}

/*-------------------------------------------------------------------------*/

UInt32
CAudioEngineParams::GetMaxVoices( void ) const
{GUCEF_TRACE;

    return m_maxVoices;
}

/*-------------------------------------------------------------------------*/

void
CAudioEngineParams::SetMaxVoices( UInt32 maxVoices )
{GUCEF_TRACE;

    m_maxVoices = maxVoices;
}

/*-------------------------------------------------------------------------*/

UInt32
CAudioEngineParams::GetMaxBuses( void ) const
{GUCEF_TRACE;

    return m_maxBuses;
}

/*-------------------------------------------------------------------------*/

void
CAudioEngineParams::SetMaxBuses( UInt32 maxBuses )
{GUCEF_TRACE;

    m_maxBuses = maxBuses;
}

/*-------------------------------------------------------------------------*/

UInt32
CAudioEngineParams::GetMaxBlockFrames( void ) const
{GUCEF_TRACE;

    return m_maxBlockFrames;
}

/*-------------------------------------------------------------------------*/

void
CAudioEngineParams::SetMaxBlockFrames( UInt32 frames )
{GUCEF_TRACE;

    m_maxBlockFrames = frames;
}

/*-------------------------------------------------------------------------*/

UInt32
CAudioEngineParams::GetCommandQueueSize( void ) const
{GUCEF_TRACE;

    return m_commandQueueSize;
}

/*-------------------------------------------------------------------------*/

void
CAudioEngineParams::SetCommandQueueSize( UInt32 size )
{GUCEF_TRACE;

    m_commandQueueSize = size;
}

/*-------------------------------------------------------------------------*/

UInt32
CAudioEngineParams::GetEventQueueSize( void ) const
{GUCEF_TRACE;

    return m_eventQueueSize;
}

/*-------------------------------------------------------------------------*/

void
CAudioEngineParams::SetEventQueueSize( UInt32 size )
{GUCEF_TRACE;

    m_eventQueueSize = size;
}

/*-------------------------------------------------------------------------*/

TAudioThreadingMode
CAudioEngineParams::GetThreadingMode( void ) const
{GUCEF_TRACE;

    return m_threadingMode;
}

/*-------------------------------------------------------------------------*/

void
CAudioEngineParams::SetThreadingMode( TAudioThreadingMode mode )
{GUCEF_TRACE;

    m_threadingMode = mode;
}

/*-------------------------------------------------------------------------*/

UInt32
CAudioEngineParams::GetUpdateIntervalInMs( void ) const
{GUCEF_TRACE;

    return m_updateIntervalInMs;
}

/*-------------------------------------------------------------------------*/

void
CAudioEngineParams::SetUpdateIntervalInMs( UInt32 milliseconds )
{GUCEF_TRACE;

    m_updateIntervalInMs = milliseconds;
}

/*-------------------------------------------------------------------------*/

const CString&
CAudioEngineParams::GetThreadPoolName( void ) const
{GUCEF_TRACE;

    return m_threadPoolName;
}

/*-------------------------------------------------------------------------*/

void
CAudioEngineParams::SetThreadPoolName( const CString& name )
{GUCEF_TRACE;

    m_threadPoolName = name;
}

/*-------------------------------------------------------------------------*/

bool
CAudioEngineParams::GetStartDevice( void ) const
{GUCEF_TRACE;

    return m_startDevice;
}

/*-------------------------------------------------------------------------*/

void
CAudioEngineParams::SetStartDevice( bool start )
{GUCEF_TRACE;

    m_startDevice = start;
}

/*-------------------------------------------------------------------------*/

TAudioPanLaw
CAudioEngineParams::GetPanLaw( void ) const
{GUCEF_TRACE;

    return m_panLaw;
}

/*-------------------------------------------------------------------------*/

void
CAudioEngineParams::SetPanLaw( TAudioPanLaw panLaw )
{GUCEF_TRACE;

    m_panLaw = panLaw;
}


/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace AUDIO */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
