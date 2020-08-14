/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <time.h>
#include <string.h>

#include "gucefCORE_CTime.h"

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    #include <Windows.h>

#elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID )

    #include <sys/time.h>

#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CTime::CTime( void )
    : CITime()
    , m_hours( 0 )
    , m_minutes( 0 )
    , m_seconds( 0 )
    , m_milliseconds( 0 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CTime::CTime( const time_t& src, bool isUtc )
    : CITime()
    , m_hours( 0 )
    , m_minutes( 0 )
    , m_seconds( 0 )
    , m_milliseconds( 0 )
{GUCEF_TRACE;

    struct tm* brokenDownTime = GUCEF_NULL;
    if ( isUtc )
        brokenDownTime = gmtime( &src );
    else
        brokenDownTime = localtime( &src );

    m_hours = (UInt8) brokenDownTime->tm_hour;
    m_minutes = (UInt8) brokenDownTime->tm_min;
    m_seconds = (UInt8) brokenDownTime->tm_sec;
}

/*-------------------------------------------------------------------------*/

CTime::CTime( const CTime& src )
    : CITime( src )
    , m_hours( src.m_hours )
    , m_minutes( src.m_minutes )
    , m_seconds( src.m_seconds )
    , m_milliseconds( src.m_milliseconds )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CTime::CTime( UInt8 hours, UInt8 minutes, UInt8 seconds, UInt16 milliseconds )
    : CITime()
    , m_hours( hours )
    , m_minutes( minutes )
    , m_seconds( seconds )
    , m_milliseconds( milliseconds )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CTime::~CTime()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CTime
CTime::NowUTCTime( void )
{GUCEF_TRACE;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    ::SYSTEMTIME systemTime;
    ::GetSystemTime( &systemTime );
    return CTime( (UInt8) systemTime.wHour, (UInt8) systemTime.wMinute, (UInt8) systemTime.wSecond, (UInt16) systemTime.wMilliseconds );

    #elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID )

    struct timespec time;
    ::memset( &time, 0, sizeof(time) );
    ::clock_gettime( CLOCK_REALTIME, &time );
    struct tm* utcTime = ::gmtime( &time.tv_sec );
    return CTime( (UInt8) utcTime->tm_hour, (UInt8) utcTime->tm_min, (UInt8) utcTime->tm_sec, (UInt16) ( time.tv_nsec / 1000000 ) );

    #else

    // Not supported for the current platform
    return CTime();
    #endif
}

/*-------------------------------------------------------------------------*/

CTime
CTime::NowLocalTime( void )
{GUCEF_TRACE;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    ::SYSTEMTIME systemTime;
    ::GetLocalTime( &systemTime );
    return CTime( (UInt8) systemTime.wHour, (UInt8) systemTime.wMinute, (UInt8) systemTime.wSecond, (UInt8) systemTime.wMilliseconds );

    #elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID )

    struct timespec time;
    ::memset( &time, 0, sizeof(time) );
    ::clock_gettime( CLOCK_REALTIME, &time );
    struct tm* utcTime = ::localtime( &time.tv_sec );
    return CTime( (UInt8) utcTime->tm_hour, (UInt8) utcTime->tm_min, (UInt8) utcTime->tm_sec, (UInt16) ( time.tv_nsec / 1000000 ) );

    #else

    // Not supported for the current platform
    return CTime();
    #endif
}

/*-------------------------------------------------------------------------*/

UInt8
CTime::GetHours( void ) const
{GUCEF_TRACE;

    return m_hours;
}

/*-------------------------------------------------------------------------*/

UInt8
CTime::GetMinutes( void ) const
{GUCEF_TRACE;

    return m_minutes;
}

/*-------------------------------------------------------------------------*/

UInt8
CTime::GetSeconds( void ) const
{GUCEF_TRACE;

    return m_seconds;
}

/*-------------------------------------------------------------------------*/

UInt16
CTime::GetMilliseconds( void ) const
{GUCEF_TRACE;

    return m_milliseconds;
}

/*-------------------------------------------------------------------------*/

CTime&
CTime::operator=( const CTime& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        m_hours = src.m_hours;
        m_minutes = src.m_minutes;
        m_seconds = src.m_seconds;
        m_milliseconds = src.m_milliseconds;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
CTime::operator==( const CTime& src ) const
{GUCEF_TRACE;

    return m_hours == src.m_hours && m_minutes == src.m_minutes && m_seconds == src.m_seconds && m_milliseconds == src.m_milliseconds;
}

/*-------------------------------------------------------------------------*/

bool
CTime::operator!=( const CTime& src ) const
{GUCEF_TRACE;

    return m_hours != src.m_hours || m_minutes != src.m_minutes || m_seconds != src.m_seconds|| m_milliseconds != src.m_milliseconds;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
