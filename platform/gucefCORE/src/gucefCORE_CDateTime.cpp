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

#include <string.h>

#include "gucefCORE_CDateTime.h"

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    #include <Windows.h>

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

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

class GUCEF_CORE_PRIVATE_CPP COSDateTimeUtils
{
    public:

    static UInt16 GetTimezoneOffsetInMins( void )
    {
        ::TIME_ZONE_INFORMATION tzInfo;
        memset( &tzInfo, 0, sizeof( tzInfo ) ); 
        if ( TIME_ZONE_ID_INVALID != ::GetTimeZoneInformation( &tzInfo ) )
        {
            return (Int16) tzInfo.Bias;    
        }
        return 0;
    }

    static void DateTimeToWin32SystemTime( const CDateTime& datetime, SYSTEMTIME& systemTime )
    {
        systemTime.wYear = (WORD) datetime.GetYear();
        systemTime.wMonth = (WORD) datetime.GetMonth();
        systemTime.wDay = (WORD) datetime.GetDay();
        systemTime.wHour = (WORD) datetime.GetHours();
        systemTime.wMinute = (WORD) datetime.GetMinutes();
        systemTime.wSecond = (WORD) datetime.GetSeconds();
        systemTime.wMilliseconds = (WORD) datetime.GetMilliseconds();
    }

    static void DateTimeToWin32FileTime( const CDateTime& datetime, FILETIME& fileTime )
    {
        ::SYSTEMTIME systemTime;
        DateTimeToWin32SystemTime( datetime, systemTime );

        if ( datetime.IsUTC() )
        {
            ::SystemTimeToFileTime( &systemTime, &fileTime );
        }
        else
        {
            ::TIME_ZONE_INFORMATION tz;
            memset( &tz, 0, sizeof( tz ) );
            tz.Bias = datetime.GeTimeZoneUTCOffsetInMins();
            
            ::SYSTEMTIME utcTime;
            ::TzSpecificLocalTimeToSystemTime( &tz, &systemTime, &utcTime );
            ::SystemTimeToFileTime( &utcTime, &fileTime ); 
        }
    }

    static void Win32SystemTimeToDateTime( SYSTEMTIME& systemTime, UInt16 timezoneOffsetInMins, CDateTime& datetime )
    {
        datetime = CDateTime( (Int16) systemTime.wYear          , 
                              (UInt8) systemTime.wMonth         , 
                              (UInt8) systemTime.wDay           , 
                              (UInt8) systemTime.wHour          , 
                              (UInt8) systemTime.wMinute        , 
                              (UInt8) systemTime.wSecond        ,
                              (UInt16) systemTime.wMilliseconds ,
                              timezoneOffsetInMins              );
    }

    static void Win32FileTimeToDateTime( const FILETIME& fileTime, CDateTime& datetime )
    {
        ::SYSTEMTIME systemTime;
        ::FileTimeToSystemTime( &fileTime, &systemTime );

        Win32SystemTimeToDateTime( systemTime, 0, datetime );

        DateTimeToWin32SystemTime( datetime, systemTime );
    }

    static CDateTime DateTimeToUtc( const CDateTime& datetime )
    {
        ::SYSTEMTIME systemTime;
        DateTimeToWin32SystemTime( datetime, systemTime );

        ::TIME_ZONE_INFORMATION tz;
        memset( &tz, 0, sizeof( tz ) );
        tz.Bias = datetime.GeTimeZoneUTCOffsetInMins();
            
        ::SYSTEMTIME utcTime;
        ::TzSpecificLocalTimeToSystemTime( &tz, &systemTime, &utcTime );

        CDateTime utcDatetime;
        Win32SystemTimeToDateTime( utcTime, 0, utcDatetime );
        return utcDatetime;
    }

    static Int32 CompareDateTime( const CDateTime& datetimeA, const CDateTime& datetimeB )
    {
        ::FILETIME fileTimeA;
        DateTimeToWin32FileTime( datetimeA, fileTimeA );

        ::FILETIME fileTimeB;
        DateTimeToWin32FileTime( datetimeB, fileTimeB );

        return ::CompareFileTime( &fileTimeA, &fileTimeB );
    }

    static Int64 SubtractBFromAndGetTimeDifferenceInMilliseconds( const CDateTime& datetimeA, const CDateTime& datetimeB )
    {
        ::FILETIME fileTimeA;
        DateTimeToWin32FileTime( datetimeA, fileTimeA );

        ::FILETIME fileTimeB;
        DateTimeToWin32FileTime( datetimeB, fileTimeB );

        ULARGE_INTEGER fileTimeUInt64A;
        fileTimeUInt64A.LowPart = fileTimeA.dwLowDateTime;
        fileTimeUInt64A.HighPart = fileTimeA.dwHighDateTime;
        
        ULARGE_INTEGER fileTimeUInt64B;
        fileTimeUInt64B.LowPart = fileTimeB.dwLowDateTime;
        fileTimeUInt64B.HighPart = fileTimeB.dwHighDateTime;

        Int64 quadDiff = fileTimeUInt64B.QuadPart - fileTimeUInt64A.QuadPart;
        quadDiff = ( quadDiff / 10000 );
        return quadDiff;
    }

    static CDateTime NowLocalDateTime( void )
    {
        ::SYSTEMTIME utcSystemTime;
        ::GetSystemTime( &utcSystemTime );
        ::SYSTEMTIME localSystemTime;
        ::SystemTimeToTzSpecificLocalTime( NULL, &utcSystemTime, &localSystemTime );

        CDateTime localDatetime;
        Win32SystemTimeToDateTime( localSystemTime, GetTimezoneOffsetInMins(), localDatetime );
        return localDatetime;
    }

    static CDateTime NowUTCDateTime( void )
    {
        ::SYSTEMTIME systemTime;
        ::GetSystemTime( &systemTime );

        CDateTime utcDatetime;
        Win32SystemTimeToDateTime( systemTime, 0, utcDatetime );
        return utcDatetime;
    }
};

#else

class GUCEF_CORE_PRIVATE_CPP COSDateTimeUtils
{
    public:

    static UInt16 GetTimezoneOffsetInMins( void )
    {
        return 0;
    }

    static CDateTime NowLocalDateTime( void )
    { 
        return CDateTime(); 
    }

    static CDateTime NowUTCDateTime( void ) 
    { 
        return CDateTime(); 
    }

    static CDateTime DateTimeToUtc( const CDateTime& datetime )
    {
        return datetime;    
    }

    static Int32 CompareDateTime( const CDateTime& datetimeA, const CDateTime& datetimeB )
    {
        return 0;
    }

    static Int64 SubtractBFromAndGetTimeDifferenceInMilliseconds( const CDateTime& datetimeA, const CDateTime& datetimeB )
    {
        return 0;
    }
};

#endif

/*-------------------------------------------------------------------------*/

CDateTime::CDateTime( const time_t src, bool isUtc )
    : CDate( src, isUtc )
    , CTime( src, isUtc )
    , m_timezoneOffsetInMins( 0 )
{GUCEF_TRACE;

    if ( !isUtc )
        m_timezoneOffsetInMins = COSDateTimeUtils::GetTimezoneOffsetInMins();
}

/*-------------------------------------------------------------------------*/

CDateTime::CDateTime( const CTime& src, bool isUtc )
    : CDate()
    , CTime( src )
    , m_timezoneOffsetInMins( 0 )
{GUCEF_TRACE;

    if ( !isUtc )
        m_timezoneOffsetInMins = COSDateTimeUtils::GetTimezoneOffsetInMins();
}

/*-------------------------------------------------------------------------*/

CDateTime::CDateTime( const CDate& src, bool isUtc )
    : CDate( src )
    , CTime()
    , m_timezoneOffsetInMins( 0 )
{GUCEF_TRACE;

    if ( !isUtc )
        m_timezoneOffsetInMins = COSDateTimeUtils::GetTimezoneOffsetInMins();
}

/*-------------------------------------------------------------------------*/

CDateTime::CDateTime( const CDate& dateSrc, const CTime& timeSrc, Int16 timezoneOffsetInMins )
    : CDate( dateSrc )
    , CTime( timeSrc)
    , m_timezoneOffsetInMins( timezoneOffsetInMins )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CDateTime::CDateTime( const CDateTime& src )
    : CDate( src )
    , CTime( src )
    , m_timezoneOffsetInMins( src.m_timezoneOffsetInMins )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CDateTime::CDateTime( Int16 year                 , 
                      UInt8 month                , 
                      UInt8 day                  , 
                      UInt8 hours                , 
                      UInt8 minutes              , 
                      UInt8 seconds              , 
                      UInt16 milliseconds        , 
                      Int16 timezoneOffsetInMins )
    : CDate( year, month, day )
    , CTime( hours, minutes, seconds, milliseconds )
    , m_timezoneOffsetInMins( timezoneOffsetInMins )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

CDateTime::CDateTime( const struct _FILETIME& src )
    : CDate()
    , CTime()
    , m_timezoneOffsetInMins( 0 )
{GUCEF_TRACE;

    COSDateTimeUtils::Win32FileTimeToDateTime( src, *this ); 
}

#endif

/*-------------------------------------------------------------------------*/

CDateTime::~CDateTime()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CDateTime 
CDateTime::NowUTCDateTime( void )
{GUCEF_TRACE;

    return COSDateTimeUtils::NowUTCDateTime();
}

/*-------------------------------------------------------------------------*/

CDateTime 
CDateTime::NowLocalDateTime( void )
{GUCEF_TRACE;

    return COSDateTimeUtils::NowLocalDateTime();
}

/*-------------------------------------------------------------------------*/

CDateTime 
CDateTime::UnixEpochDateTime( void )
{GUCEF_TRACE;

    return CDateTime( 1970, 1, 1 );
}

/*-------------------------------------------------------------------------*/

CDateTime 
CDateTime::WindowsNTEpochDateTime( void )
{GUCEF_TRACE;

    return CDateTime( 1601, 1, 1 );
}

/*-------------------------------------------------------------------------*/

bool 
CDateTime::operator>( const CDateTime& other ) const
{GUCEF_TRACE;

    return COSDateTimeUtils::CompareDateTime( *this, other ) > 0;
}

/*-------------------------------------------------------------------------*/

bool 
CDateTime::operator<( const CDateTime& other ) const
{GUCEF_TRACE;

    return COSDateTimeUtils::CompareDateTime( *this, other ) < 0;
}

/*-------------------------------------------------------------------------*/

CDateTime& 
CDateTime::operator=( const CDateTime& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        CDate::operator=( src );
        CTime::operator=( src );
        m_timezoneOffsetInMins = src.m_timezoneOffsetInMins;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool 
CDateTime::operator==( const CDateTime& other ) const
{GUCEF_TRACE;

    return 0 == COSDateTimeUtils::CompareDateTime( *this, other );
}

/*-------------------------------------------------------------------------*/

bool 
CDateTime::operator!=( const CDateTime& other ) const
{GUCEF_TRACE;

    return 0 != COSDateTimeUtils::CompareDateTime( *this, other );
}

/*-------------------------------------------------------------------------*/

Int16
CDateTime::GeTimeZoneUTCOffsetInMins( void ) const
{GUCEF_TRACE;

    return m_timezoneOffsetInMins;
}

/*-------------------------------------------------------------------------*/

bool 
CDateTime::IsUTC( void ) const
{GUCEF_TRACE;

    return 0 == m_timezoneOffsetInMins;
}

/*-------------------------------------------------------------------------*/

CDateTime 
CDateTime::ToUTC( void ) const
{GUCEF_TRACE;

    if ( 0 != m_timezoneOffsetInMins )
        return COSDateTimeUtils::DateTimeToUtc( *this );
    return *this;
}

/*-------------------------------------------------------------------------*/

Int64 
CDateTime::SubtractAndGetTimeDifferenceInMilliseconds( const CDateTime& other ) const
{GUCEF_TRACE;

    return COSDateTimeUtils::SubtractBFromAndGetTimeDifferenceInMilliseconds( *this, other );
}

/*-------------------------------------------------------------------------*/

CDate 
CDateTime::GetDate( void ) const
{GUCEF_TRACE;

    return *this;
}

/*-------------------------------------------------------------------------*/

CTime 
CDateTime::GetTime( void ) const
{GUCEF_TRACE;

    return *this;
}

/*-------------------------------------------------------------------------*/

CString 
CDateTime::ToIso8601DateTimeString( bool includeDelimeters, bool includeMilliseconds ) const
{GUCEF_TRACE;

    if ( !IsUTC() )
    {
        return ToUTC().ToIso8601DateTimeString( includeDelimeters, includeMilliseconds );
    }
    
    if ( includeDelimeters )
    {
        if ( includeMilliseconds )
        {
            char dtBuffer[ 25 ];
            sprintf( dtBuffer, "%04d-%02u-%02uT%02u:%02u:%02u.%03uZ", m_year, m_month, m_day, m_hours, m_minutes, m_seconds, m_milliseconds );
            return dtBuffer;
        }
        else
        {
            char dtBuffer[ 25 ];
            sprintf( dtBuffer, "%04d-%02u-%02uT%02u:%02u:%02uZ", m_year, m_month, m_day, m_hours, m_minutes, m_seconds );
            return dtBuffer;
        }
    }
    else
    {
        char dtBuffer[ 25 ];
        sprintf( dtBuffer, "%04d%02u%02u%02u%02u%02u", m_year, m_month, m_day, m_hours, m_minutes, m_seconds );
        return dtBuffer;
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
