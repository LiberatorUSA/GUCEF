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

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

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
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CDateTime CDateTime::Empty;

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

    static bool DateTimeToWin32FileTime( const CDateTime& datetime, FILETIME& fileTime )
    {
        ::SYSTEMTIME systemTime;
        DateTimeToWin32SystemTime( datetime, systemTime );

        if ( datetime.IsUTC() )
        {
            return ::SystemTimeToFileTime( &systemTime, &fileTime ) == TRUE;
        }
        else
        {
            ::TIME_ZONE_INFORMATION tz;
            memset( &tz, 0, sizeof( tz ) );
            tz.Bias = datetime.GetTimeZoneUTCOffsetInMins();

            ::SYSTEMTIME utcTime;
            if ( ::TzSpecificLocalTimeToSystemTime( &tz, &systemTime, &utcTime ) == TRUE )
                return ::SystemTimeToFileTime( &utcTime, &fileTime ) == TRUE;
            else
                return false;
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

    static CDateTime GetFutureMaxAsDateTime( void )
    {
        ::ULARGE_INTEGER fileTimeUInt64;
        fileTimeUInt64.QuadPart = 0x7fff35f4f06c58f0;
        ::FILETIME filetime;
        filetime.dwHighDateTime = fileTimeUInt64.HighPart;      
        filetime.dwLowDateTime = fileTimeUInt64.LowPart;
        
        CDateTime dtFormat;
        Win32FileTimeToDateTime( filetime, dtFormat );
        return dtFormat;
    }

    static CDateTime GetPastMaxAsDateTime( void )
    {
        ::ULARGE_INTEGER fileTimeUInt64;
        fileTimeUInt64.QuadPart = 0;
        ::FILETIME filetime;
        filetime.dwHighDateTime = fileTimeUInt64.HighPart;      
        filetime.dwLowDateTime = fileTimeUInt64.LowPart;
        
        CDateTime dtFormat;
        Win32FileTimeToDateTime( filetime, dtFormat );
        return dtFormat;
    }

    static CDateTime DateTimeToUtc( const CDateTime& datetime )
    {
        ::SYSTEMTIME systemTime;
        DateTimeToWin32SystemTime( datetime, systemTime );

        ::TIME_ZONE_INFORMATION tz;
        memset( &tz, 0, sizeof( tz ) );
        tz.Bias = datetime.GetTimeZoneUTCOffsetInMins();

        ::SYSTEMTIME utcTime;
        ::TzSpecificLocalTimeToSystemTime( &tz, &systemTime, &utcTime );

        CDateTime utcDatetime;
        Win32SystemTimeToDateTime( utcTime, 0, utcDatetime );
        return utcDatetime;
    }

    // -1 = First datetime is earlier than second datetime.
    //  0 = First datetime is equal to second datetime.
    //  1 = First datetime is later than second datetime.
    static Int32 CompareDateTime( const CDateTime& datetimeA, const CDateTime& datetimeB )
    {
        ::FILETIME fileTimeA;
        DateTimeToWin32FileTime( datetimeA, fileTimeA );

        ::FILETIME fileTimeB;
        DateTimeToWin32FileTime( datetimeB, fileTimeB );

        return ::CompareFileTime( &fileTimeA, &fileTimeB );
    }

    static Int64 GetTimeDifferenceInMillisecondsFromAtowardsB( const CDateTime& datetimeA, const CDateTime& datetimeB )
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

    static void SetFromUnixEpochBasedTicksInMillisecs( UInt64 unixDtInMsTicks, CDateTime& dt )
    {
        // Convert to FILETIME compatible resolution (ms to nanos) and compensate for different epoch offset
        UINT64 filettimeDt = ( 10000 * unixDtInMsTicks ) + 116444736000000000;

        ULARGE_INTEGER memoryAllignedUInt64;
        memoryAllignedUInt64.QuadPart = filettimeDt;

        FILETIME filetime;
        filetime.dwLowDateTime = memoryAllignedUInt64.LowPart;
        filetime.dwHighDateTime = memoryAllignedUInt64.HighPart;

        Win32FileTimeToDateTime( filetime, dt );
    }

    static UInt64 ToUnixEpochBasedTicksInMillisecs( const CDateTime& datetime )
    {
        FILETIME fileTime;
        DateTimeToWin32FileTime( datetime, fileTime );

        ULARGE_INTEGER memoryAllignedUInt64;
        memoryAllignedUInt64.LowPart = fileTime.dwLowDateTime;
        memoryAllignedUInt64.HighPart = fileTime.dwHighDateTime; 

        // Convert from FILETIME compatible resolution (nanos to ms) and compensate for different epoch offset
        UInt64 unixDtInMsTicks = ( memoryAllignedUInt64.QuadPart - 116444736000000000 ) / 10000;       
        return unixDtInMsTicks;
    }
};

#elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID )

class GUCEF_CORE_PRIVATE_CPP COSDateTimeUtils
{
    public:

    static UInt16 GetTimezoneOffsetInMins( void )
    {
        struct timespec time;
        ::memset( &time, 0, sizeof(time) );
        ::clock_gettime( CLOCK_REALTIME, &time );
        struct tm hereTime;
        ::localtime_r( &time.tv_sec, &hereTime );
        return hereTime.tm_gmtoff / 60;
    }

    static void DateTimeToTm( const CDateTime& datetime, struct ::tm& time )
    {
        ::memset( &time, 0, sizeof(time) );
        time.tm_year = datetime.GetYear() - 1900;
        time.tm_mon = datetime.GetMonth() - 1;
        time.tm_mday = datetime.GetDay() - 1;
        time.tm_hour = datetime.GetHours();
        time.tm_min = datetime.GetMinutes();
        time.tm_sec = datetime.GetSeconds();
        time.tm_gmtoff = datetime.GeTimeZoneUTCOffsetInMins() * 60;
    }

    static void DateTimeToTimespec( const CDateTime& datetime, struct ::timespec& tspec )
    {
        struct ::tm time;
        DateTimeToTm( datetime, time );  // <- this converts keeping the timezone intact with gmtoff

        tspec.tv_sec = ::timegm( &time ) - time.tm_gmtoff; // <- this converts to UTC
        tspec.tv_nsec = datetime.GetMilliseconds() * 1000000;
    }

    static void TimespecToDateTime( const struct ::timespec& tspec, CDateTime& datetime )
    {
        struct tm utcTime;
        ::gmtime_r( &tspec.tv_sec, &utcTime );
        datetime.Set( (Int16) utcTime.tm_year+1900,
                      (UInt8) utcTime.tm_mon+1,
                      (UInt8) utcTime.tm_mday+1,
                      (UInt8) utcTime.tm_hour,
                      (UInt8) utcTime.tm_min,
                      (UInt8) utcTime.tm_sec,
                      (UInt16) ( tspec.tv_nsec / 1000000 ),
                      0 );
    }

    static CDateTime NowLocalDateTime( void )
    {
        struct timespec time;
        ::memset( &time, 0, sizeof(time) );
        ::clock_gettime( CLOCK_REALTIME, &time );
        struct tm hereTime;
        ::localtime_r( &time.tv_sec, &hereTime );
        return CDateTime( (Int16) hereTime.tm_year+1900,
                          (UInt8) hereTime.tm_mon+1,
                          (UInt8) hereTime.tm_mday+1,
                          (UInt8) hereTime.tm_hour,
                          (UInt8) hereTime.tm_min,
                          (UInt8) hereTime.tm_sec,
                          (UInt16) ( time.tv_nsec / 1000000 ),
                          hereTime.tm_gmtoff / 60 );
    }

    static CDateTime NowUTCDateTime( void )
    {
        struct timespec time;
        ::memset( &time, 0, sizeof(time) );
        ::clock_gettime( CLOCK_REALTIME, &time );
        struct tm utcTime;
        ::gmtime_r( &time.tv_sec, &utcTime );
        return CDateTime( (Int16) utcTime.tm_year+1900,
                          (UInt8) utcTime.tm_mon+1,
                          (UInt8) utcTime.tm_mday+1,
                          (UInt8) utcTime.tm_hour,
                          (UInt8) utcTime.tm_min,
                          (UInt8) utcTime.tm_sec,
                          (UInt16) ( time.tv_nsec / 1000000 ),
                          0 );
    }

    static CDateTime DateTimeToUtc( const CDateTime& datetime )
    {
        // Converting to timespec also converts to UTC by turning it into a UTC based offset
        struct ::timespec tspec;
        DateTimeToTimespec( datetime, tspec );

        CDateTime utcDatetime;
        TimespecToDateTime( tspec, utcDatetime );
        return utcDatetime;
    }

    // -1 = First datetime is earlier than second datetime.
    //  0 = First datetime is equal to second datetime.
    //  1 = First datetime is later than second datetime.
    static Int32 CompareDateTime( const CDateTime& datetimeA, const CDateTime& datetimeB )
    {
        // Converting to timespec also converts to UTC by turning it into a UTC based offset
        // This allows for easy comparison

        struct ::timespec tspecA;
        DateTimeToTimespec( datetimeA, tspecA );

        struct ::timespec tspecB;
        DateTimeToTimespec( datetimeB, tspecB );

        if ( tspecA.tv_sec > tspecB.tv_sec )
            return 1;
        if ( tspecA.tv_sec < tspecB.tv_sec )
            return -1;

        // seconds are the same, but they can still differ on the remainder nanoseconds
        if ( tspecA.tv_nsec > tspecB.tv_nsec )
            return 1;
        if ( tspecA.tv_nsec < tspecB.tv_nsec )
            return -1;
        return 0;
    }

    static Int64 GetTimeDifferenceInMillisecondsFromAtowardsB( const CDateTime& datetimeA, const CDateTime& datetimeB )
    {
        // Converting to timespec also converts to UTC by turning it into a UTC based offset
        // This allows for easy subtraction

        struct ::timespec tspecA;
        DateTimeToTimespec( datetimeA, tspecA );

        struct ::timespec tspecB;
        DateTimeToTimespec( datetimeB, tspecB );

        Int64 deltaSecs = tspecA.tv_sec - tspecB.tv_sec;
        Int64 deltaNanoSecs = tspecA.tv_nsec - tspecB.tv_nsec;
        return ( deltaSecs * 1000 ) + ( deltaNanoSecs / 1000000 );
    }

    static void SetFromUnixEpochBasedTicksInMillisecs( UInt64 unixDtInMsTicks, CDateTime& dt )
    {
        // Break the milliseconds total into seconds plus the nanoseconds remainder
        // This allows us to use the timespec conversion function
        struct timespec time;
        time.tv_sec = (time_t) unixDtInMsTicks / 1000;
        time.tv_nsec = ( unixDtInMsTicks - ( time.tv_sec * 1000 ) ) * 1000000;

        TimespecToDateTime( time, dt );
    }

    static UInt64 ToUnixEpochBasedTicksInMillisecs( const CDateTime& datetime )
    {
        struct timespec time;
        DateTimeToTimespec( datetime, time );

        UInt64 unixDtInMsTicks = ( time.tv_sec * 1000 ) + (UInt64)( time.tv_nsec / 1000000 );
        return unixDtInMsTicks;
    }

    static CDateTime GetFutureMaxAsDateTime( void )
    {
        struct timespec time;
        time.tv_sec = (time_t) 0;
        time.tv_sec -= 1; // intentionally wrap around the unsigned 
        time.tv_nsec = LONG_MAX;

        CDateTime dtFormat;
        TimespecToDateTime( time, dtFormat );
        return dtFormat;
    }

    static CDateTime GetPastMaxAsDateTime( void )
    {
        struct timespec time;
        time.tv_sec = (time_t) 0;
        time.tv_nsec = 0;

        CDateTime dtFormat;
        TimespecToDateTime( time, dtFormat );
        return dtFormat;
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

    static UInt64 ToUnixEpochBasedTicksInMillisecs( const CDateTime& datetime )
    {
        return 0;
    }

    static CDateTime GetFutureMaxAsDateTime( void )
    {
        return CDateTime( GUCEF_MT_INT16MAX, UInt8( 12 ), UInt8( 31 ), UInt8( 23 ), UInt8( 59 ), UInt8( 59 ), UInt16( 999 ) );
    }

    static CDateTime GetPastMaxAsDateTime( void )
    {
        return CDateTime( GUCEF_MT_INT16MIN, UInt8( 0 ), UInt8( 0 ), UInt8( 0 ), UInt8( 0 ), UInt8( 0 ), UInt16( 0 ) );
    }
};

#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CDateTime CDateTime::PastMax = COSDateTimeUtils::GetPastMaxAsDateTime();
const CDateTime CDateTime::FutureMax = COSDateTimeUtils::GetFutureMaxAsDateTime();

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CDateTime::CDateTime( const time_t src, bool isUtc )
    : CDate( src, isUtc )
    , CTime( src, isUtc )
    , m_timezoneOffsetInMins( 0 )
{GUCEF_TRACE;

    if ( !isUtc )
        m_timezoneOffsetInMins = COSDateTimeUtils::GetTimezoneOffsetInMins();
}

/*-------------------------------------------------------------------------*/

CDateTime::CDateTime( const struct tm* tmStruct, bool isUtc )
    : CDate()
    , CTime()
    , m_timezoneOffsetInMins( 0 )
{GUCEF_TRACE;

    FromTmStruct( tmStruct, isUtc );
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

void
CDateTime::Set( Int16 year                 ,
                UInt8 month                ,
                UInt8 day                  ,
                UInt8 hours                ,
                UInt8 minutes              ,
                UInt8 seconds              ,
                UInt16 milliseconds        ,
                Int16 timezoneOffsetInMins )
{GUCEF_TRACE;

    m_year = year;
    m_month = month;
    m_day = day;
    m_hours = hours;
    m_minutes = minutes;
    m_seconds = seconds;
    m_milliseconds = milliseconds;
    m_timezoneOffsetInMins = timezoneOffsetInMins;
}

/*-------------------------------------------------------------------------*/

UInt64
CDateTime::ToUnixEpochBasedTicksInMillisecs( void ) const
{GUCEF_TRACE;

    return COSDateTimeUtils::ToUnixEpochBasedTicksInMillisecs( *this );
}

/*-------------------------------------------------------------------------*/

void
CDateTime::FromUnixEpochBasedTicksInMillisecs( UInt64 unixDtInMsTicks )
{GUCEF_TRACE;

    COSDateTimeUtils::SetFromUnixEpochBasedTicksInMillisecs( unixDtInMsTicks, *this );
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
CDateTime::CompileDateTime( const char* __date__Macro , 
                            const char* __time__Macro )
{GUCEF_TRACE;

    // __DATE__ is a preprocessor macro that expands to current date (at compile time) in the form mmm dd yyyy (e.g. "Jan 14 2012"), as a string. The __DATE__ macro can be used to provide information about the particular moment a binary was built.    
    // __TIME__ is a preprocessor macro that expands to current time (at compile time) in the form hh:mm:ss in 24 hour time (e.g. "22:29:12"), as a string. The __TIME__ macro can be used to provide information about the particular moment a binary was built.

    CORE::CDateTime dt;
    
    CORE::Int32 month=0, day=0, year=0;
    char month3Letter[ 5 ] = { '\0', '\0', '\0', '\0', '\0' };
    static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
    CORE::Int32 hour=0, minutes=0, seconds=0;

    sscanf( __date__Macro, "%s %d %d", month3Letter, &day, &year );
    month = ( (CORE::Int32)( strstr( month_names, month3Letter ) - month_names ) ) / 3;
    sscanf( __time__Macro, "%d%*c%d%*c%d", &hour, &minutes, &seconds );

    dt.Set( (CORE::Int16) year    ,
            (CORE::UInt8) month   ,
            (CORE::UInt8) day     ,
            (CORE::UInt8) hour    ,
            (CORE::UInt8) minutes ,
            (CORE::UInt8) seconds );

    return dt;
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

bool
CDateTime::operator>=( const CDateTime& other ) const
{GUCEF_TRACE;

    return (*this) > other || (*this) == other;
}

/*-------------------------------------------------------------------------*/

bool
CDateTime::operator<=( const CDateTime& other ) const
{GUCEF_TRACE;

    return (*this) < other || (*this) == other;
}

/*-------------------------------------------------------------------------*/

Int16
CDateTime::GetTimeZoneUTCOffsetInMins( void ) const
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
CDateTime::GetTimeDifferenceInMillisecondsTowards( const CDateTime& other ) const
{GUCEF_TRACE;

    return COSDateTimeUtils::GetTimeDifferenceInMillisecondsFromAtowardsB( *this, other );
}

/*-------------------------------------------------------------------------*/

Int64
CDateTime::GetTimeDifferenceInMillisecondsToNow( void ) const
{GUCEF_TRACE;

    return COSDateTimeUtils::GetTimeDifferenceInMillisecondsFromAtowardsB( *this, CDateTime::NowUTCDateTime() );
}

/*-------------------------------------------------------------------------*/

CDate&
CDateTime::GetDate( void )
{GUCEF_TRACE;

    return *this;
}

/*-------------------------------------------------------------------------*/

const CDate&
CDateTime::GetDate( void ) const
{GUCEF_TRACE;

    return *this;
}

/*-------------------------------------------------------------------------*/

CTime&
CDateTime::GetTime( void )
{GUCEF_TRACE;

    return *this;
}

/*-------------------------------------------------------------------------*/

const CTime&
CDateTime::GetTime( void ) const
{GUCEF_TRACE;

    return *this;
}

/*-------------------------------------------------------------------------*/

Int32
TimezoneOffsetInMinsFromIso8601DateTimeStringRemnant( const char* sourceBuffer, UInt32 sourceBufferSize, Int16& tzOffset )
{GUCEF_TRACE;
    
    // We are looking for Z or +hh:mm or -hh:mm
    if ( sourceBufferSize > 0 )
    {
        if ( *sourceBuffer == 'Z' )
        {
            tzOffset = 0;
            return 1;
        }
        if ( sourceBufferSize > 5 )
        {
            Int32 hours = 0;
            Int32 minutes = 0;
            int readParts = sscanf_s( sourceBuffer+1, "%02i%*c%02i", &hours, &minutes );
            if ( readParts == 2 )
            {
                minutes += hours * 60;
                if ( *sourceBuffer == '-' )
                    minutes = -1 * minutes;
                
                tzOffset = (Int16) minutes;
                return 6;
            }
        }
    }
    return -1;    
}

/*-------------------------------------------------------------------------*/

Int32
CDateTime::FromIso8601DateTimeString( const void* sourceBuffer, UInt32 sourceBufferSize )
{GUCEF_TRACE;
            
    // Reset content
    Set();
    
    // initial input sanity check
    if ( sourceBufferSize < 14 || GUCEF_NULL == sourceBuffer )
        return -1;

    // check what features we are dealing with
    const char* dtBuffer = static_cast< const char* >( sourceBuffer );
    const char* dashPtr = (const char*) memchr( dtBuffer, '-', sourceBufferSize );
    bool includesDelimeters = GUCEF_NULL != dashPtr && dashPtr < dtBuffer+5;
    bool includesMilliseconds = GUCEF_NULL != memchr( dtBuffer+13, '.', sourceBufferSize-13 ) || GUCEF_NULL != memchr( dtBuffer+13, ',', sourceBufferSize-13 );    

    Int32 year=0;
    UInt32 month=0, day=0, hours=0, minutes=0, seconds=0, milliseconds=0;
    Int16 tzOffset=0;

    if ( includesDelimeters )
    {
        if ( includesMilliseconds )
        {            
            // Length = date(4+1+2+1+2)+1+time(2+1+2+1+2+1+3) = 23 chars, 13 total parts of which 7 to parse
            if ( sourceBufferSize >= 23 )
            {
                int readParts = sscanf_s( dtBuffer, "%04d%*c%02u%*c%02u%*c%02u%*c%02u%*c%02u%*c%03u", &year, &month, &day, &hours, &minutes, &seconds, &milliseconds );
                if ( readParts == 7 )
                {
                    Int32 tzBytes = TimezoneOffsetInMinsFromIso8601DateTimeStringRemnant( dtBuffer+23, sourceBufferSize-23, tzOffset );
                    Set( (Int16) year, (UInt8) month, (UInt8) day, (UInt8) hours, (UInt8) minutes, (UInt8) seconds, (UInt16) milliseconds, tzOffset );
                    return 23 + tzBytes > 0 ? tzBytes : 0;
                }
            }
            return -1;
        }
        else
        {
            // Length = date(4+1+2+1+2)+1+time(2+1+2+1+2) = 19 chars, 11 total parts of which 6 to parse
            if ( sourceBufferSize >= 19 )
            {
                int readParts = sscanf_s( dtBuffer, "%04d%*c%02u%*c%02u%*c%02u%*c%02u%*c%02u", &year, &month, &day, &hours, &minutes, &seconds );
                if ( readParts == 6 )
                {
                    Int32 tzBytes = TimezoneOffsetInMinsFromIso8601DateTimeStringRemnant( dtBuffer+19, sourceBufferSize-19, tzOffset );
                    Set( (Int16) year, (UInt8) month, (UInt8) day, (UInt8) hours, (UInt8) minutes, (UInt8) seconds, (UInt16) milliseconds, tzOffset );
                    return 19 + tzBytes > 0 ? tzBytes : 0;
                }
            }
            return -1;
        }
    }
    else
    {
        if ( includesMilliseconds )
        {
            // Length = date(4+2+2)+time(2+2+2+1+3) = 18 chars, 8 total parts of which 7 to parse
            if ( sourceBufferSize >= 18 )
            {
                int readParts = sscanf_s( dtBuffer, "%04d%02u%02u%02u%02u%02u%*c%03u", &year, &month, &day, &hours, &minutes, &seconds, &milliseconds );
                if ( readParts == 7 )
                {
                    Int32 tzBytes = TimezoneOffsetInMinsFromIso8601DateTimeStringRemnant( dtBuffer+18, sourceBufferSize-18, tzOffset );
                    Set( (Int16) year, (UInt8) month, (UInt8) day, (UInt8) hours, (UInt8) minutes, (UInt8) seconds, (UInt16) milliseconds, tzOffset );
                    return 18 + tzBytes > 0 ? tzBytes : 0;
                }
            }
            return -1;
        }
        else
        {
            if ( sourceBufferSize >= 14 )
            {
                // Length = date(4+2+2)+time(2+2+2) = 14 chars, 6 total parts of which 6 to parse
                int readParts = sscanf_s( dtBuffer, "%04d%02u%02u%02u%02u%02u", &year, &month, &day, &hours, &minutes, &seconds );
                if ( readParts == 6 )
                {
                    Int32 tzBytes = TimezoneOffsetInMinsFromIso8601DateTimeStringRemnant( dtBuffer+14, sourceBufferSize-14, tzOffset );
                    Set( (Int16) year, (UInt8) month, (UInt8) day, (UInt8) hours, (UInt8) minutes, (UInt8) seconds, (UInt16) milliseconds, tzOffset );
                    return 14 + tzBytes > 0 ? tzBytes : 0;
                }
            }
            return -1;
        }
    }  
}

/*-------------------------------------------------------------------------*/

Int32
CDateTime::FromIso8601DateTimeString( const CDynamicBuffer& source, UInt32 sourceBufferOffset )
{GUCEF_TRACE;

    return FromIso8601DateTimeString( source.GetConstBufferPtr( sourceBufferOffset ), source.GetDataSize() - sourceBufferOffset );
}

/*-------------------------------------------------------------------------*/

bool
CDateTime::FromIso8601DateTimeString( const CString& source )
{GUCEF_TRACE;

    return 0 < FromIso8601DateTimeString( source.C_String(), source.ByteSize() );
}

/*-------------------------------------------------------------------------*/

Int32
CDateTime::ToIso8601DateTimeString( void* targetBuffer, UInt32 targetBufferSize, bool includeDelimeters, bool includeMilliseconds ) const
{GUCEF_TRACE;

    if ( !IsUTC() )
    {
        return ToUTC().ToIso8601DateTimeString( targetBuffer, targetBufferSize, includeDelimeters, includeMilliseconds );
    }

    char* dtBuffer = static_cast< char* >( targetBuffer );
    
    // The ISO DateTime range is likelyt different from the host O/S range
    // As such we should always clamp
    Int16 isoClampedYear = m_year;
    if ( isoClampedYear > 9999 )
        isoClampedYear = 9999;
    else
    if ( isoClampedYear < 0000 )    
        isoClampedYear = 0000;

    if ( includeDelimeters )
    {
        if ( includeMilliseconds )
        {            
            // Length = date(4+1+2+1+2)+1+time(2+1+2+1+2+1+3) = 24 chars
            return sprintf_s( dtBuffer, targetBufferSize, "%04d-%02u-%02uT%02u:%02u:%02u.%03uZ", isoClampedYear, m_month, m_day, m_hours, m_minutes, m_seconds, m_milliseconds );
        }
        else
        {
            // Length = date(4+1+2+1+2)+1+time(2+1+2+1+2) = 20 chars
            return sprintf_s( dtBuffer, targetBufferSize, "%04d-%02u-%02uT%02u:%02u:%02uZ", isoClampedYear, m_month, m_day, m_hours, m_minutes, m_seconds );
        }
    }
    else
    {
        if ( includeMilliseconds )
        {
            // Length = date(4+2+2)+time(2+2+2+1+3) = 18 chars
            return sprintf_s( dtBuffer, targetBufferSize, "%04d%02u%02u%02u%02u%02u.%03uZ", isoClampedYear, m_month, m_day, m_hours, m_minutes, m_seconds, m_milliseconds );
        }
        else
        {
            // Length = date(4+2+2)+time(2+2+2) = 14 chars
            return sprintf_s( dtBuffer, targetBufferSize, "%04d%02u%02u%02u%02u%02u", isoClampedYear, m_month, m_day, m_hours, m_minutes, m_seconds );
        }
    }    
}

/*-------------------------------------------------------------------------*/

Int32
CDateTime::ToIso8601DateTimeString( CDynamicBuffer& target, UInt32 targetBufferOffset, bool includeDelimeters, bool includeMilliseconds ) const
{GUCEF_TRACE;

    if ( target.GetAutoEnlarge() && target.GetRemainingBufferSize( targetBufferOffset ) < 25 )
    {
        if ( !target.SetBufferSize( target.GetBufferSize() + 25 ) )
            return -1;
    }
    
    return ToIso8601DateTimeString( target.GetBufferPtr( targetBufferOffset )           , 
                                    target.GetRemainingBufferSize( targetBufferOffset ) ,
                                    includeDelimeters                                   ,
                                    includeMilliseconds                                 );
}

/*-------------------------------------------------------------------------*/

CString
CDateTime::ToIso8601DateTimeString( bool includeDelimeters, bool includeMilliseconds ) const
{GUCEF_TRACE;

    char dtBuffer[ 25 ];
    if ( 0 < ToIso8601DateTimeString( dtBuffer, sizeof(dtBuffer), includeDelimeters, includeMilliseconds ) )
        return dtBuffer;
    return CString::Empty;
}

/*-------------------------------------------------------------------------*/

void 
CDateTime::FromTmStruct( const struct tm* tmStruct, bool isUtc )
{GUCEF_TRACE;
    
    Set();
    if ( GUCEF_NULL == tmStruct )
        return;

    m_year = (Int16) tmStruct->tm_year + 1900; // years since 1900 for tm struct
    m_month = (UInt8) tmStruct->tm_mon + 1;    // zero based for tm struct
    m_day = (UInt8) tmStruct->tm_wday + 1;     // zero based for tm struct
    m_hours = (UInt8) tmStruct->tm_hour;
    m_minutes = (UInt8) tmStruct->tm_min;
    m_seconds = (UInt8) tmStruct->tm_sec;

    // We dont currently take DST into account
    if ( !isUtc )
        m_timezoneOffsetInMins = GetTimeZoneUTCOffsetInMins();
}

/*-------------------------------------------------------------------------*/

bool 
CDateTime::IsWithinRange( const CDateTime& a, const CDateTime& b, bool includingBoundaryItself ) const
{GUCEF_TRACE;

    const CDateTime* minDt = GUCEF_NULL;
    const CDateTime* maxDt = GUCEF_NULL;
    
    if ( a < b )
    {
        minDt = &a;
        maxDt = &b;
    }
    else
    {
        minDt = &b;
        maxDt = &a;
    }

    if ( includingBoundaryItself )
    {
        return (*this) >= (*minDt) && (*this) <= (*maxDt);
    }
    else
    {
        return (*this) > (*minDt) && (*this) < (*maxDt);
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
