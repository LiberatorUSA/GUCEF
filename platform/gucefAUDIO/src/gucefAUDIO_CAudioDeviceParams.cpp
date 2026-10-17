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

#include "gucefAUDIO_CAudioDeviceParams.h"

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

CAudioDeviceParams::CAudioDeviceParams( void )
    : m_deviceId()
    , m_direction( AUDIODIRECTION_PLAYBACK )
    , m_preferredFormat()
    , m_bufferSizeInFrames( 0 )
    , m_shareMode( AUDIOSHAREMODE_SHARED )
    , m_options()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CAudioDeviceParams::CAudioDeviceParams( const CAudioDeviceParams& src )
    : m_deviceId( src.m_deviceId )
    , m_direction( src.m_direction )
    , m_preferredFormat( src.m_preferredFormat )
    , m_bufferSizeInFrames( src.m_bufferSizeInFrames )
    , m_shareMode( src.m_shareMode )
    , m_options( src.m_options )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CAudioDeviceParams&
CAudioDeviceParams::operator=( const CAudioDeviceParams& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        m_deviceId = src.m_deviceId;
        m_direction = src.m_direction;
        m_preferredFormat = src.m_preferredFormat;
        m_bufferSizeInFrames = src.m_bufferSizeInFrames;
        m_shareMode = src.m_shareMode;
        m_options = src.m_options;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CAudioDeviceParams::~CAudioDeviceParams()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

const CString&
CAudioDeviceParams::GetDeviceId( void ) const
{GUCEF_TRACE;

    return m_deviceId;
}

/*-------------------------------------------------------------------------*/

void
CAudioDeviceParams::SetDeviceId( const CString& deviceId )
{GUCEF_TRACE;

    m_deviceId = deviceId;
}

/*-------------------------------------------------------------------------*/

TAudioDirection
CAudioDeviceParams::GetDirection( void ) const
{GUCEF_TRACE;

    return m_direction;
}

/*-------------------------------------------------------------------------*/

void
CAudioDeviceParams::SetDirection( TAudioDirection direction )
{GUCEF_TRACE;

    m_direction = direction;
}

/*-------------------------------------------------------------------------*/

const CAudioFormat&
CAudioDeviceParams::GetPreferredFormat( void ) const
{GUCEF_TRACE;

    return m_preferredFormat;
}

/*-------------------------------------------------------------------------*/

void
CAudioDeviceParams::SetPreferredFormat( const CAudioFormat& format )
{GUCEF_TRACE;

    m_preferredFormat = format;
}

/*-------------------------------------------------------------------------*/

UInt32
CAudioDeviceParams::GetBufferSizeInFrames( void ) const
{GUCEF_TRACE;

    return m_bufferSizeInFrames;
}

/*-------------------------------------------------------------------------*/

void
CAudioDeviceParams::SetBufferSizeInFrames( UInt32 frames )
{GUCEF_TRACE;

    m_bufferSizeInFrames = frames;
}

/*-------------------------------------------------------------------------*/

TAudioShareMode
CAudioDeviceParams::GetShareMode( void ) const
{GUCEF_TRACE;

    return m_shareMode;
}

/*-------------------------------------------------------------------------*/

void
CAudioDeviceParams::SetShareMode( TAudioShareMode shareMode )
{GUCEF_TRACE;

    m_shareMode = shareMode;
}

/*-------------------------------------------------------------------------*/

const CORE::CValueList&
CAudioDeviceParams::GetOptions( void ) const
{GUCEF_TRACE;

    return m_options;
}

/*-------------------------------------------------------------------------*/

CORE::CValueList&
CAudioDeviceParams::GetOptions( void )
{GUCEF_TRACE;

    return m_options;
}


/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace AUDIO */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
