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

#include "gucefCORE_CDate.h"

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

CDate::CDate( void )
    : CIDate()
    , m_year( 0 )
    , m_month( 0 )
    , m_day( 0 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CDate::CDate( const time_t& src, bool isUtc )
    : CIDate()
    , m_year( 0 )
    , m_month( 0 )
    , m_day( 0 )
{GUCEF_TRACE;

    struct tm* brokenDownTime = GUCEF_NULL;
    if ( isUtc )
        brokenDownTime = gmtime( &src );
    else
        brokenDownTime = localtime( &src );

    m_year = (Int16) brokenDownTime->tm_year;
    m_month = (UInt8) brokenDownTime->tm_mon+1;
    m_day = (UInt8) brokenDownTime->tm_mday;
}

/*-------------------------------------------------------------------------*/

CDate::CDate( const CDate& src )
    : CIDate( src )
    , m_year( src.m_year )
    , m_month( src.m_month )
    , m_day( src.m_day )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CDate::CDate( UInt16 year, UInt8 month, UInt8 day )
    : CIDate()
    , m_year( year )
    , m_month( month )
    , m_day( day )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CDate::~CDate()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CDate 
CDate::TodaysUTCDate( void )
{GUCEF_TRACE;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    ::SYSTEMTIME systemTime;
    ::GetSystemTime( &systemTime );
    return CDate( (UInt16) systemTime.wYear, (UInt8) systemTime.wMonth, (UInt8) systemTime.wDay );
    
    //#elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX )
    #else

    // Not supported for the current platform
    return CDate( 0, 0, 0 );
    #endif
}

/*-------------------------------------------------------------------------*/

CDate 
CDate::TodaysLocalDate( void )
{GUCEF_TRACE;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    ::SYSTEMTIME systemTime;
    ::GetLocalTime( &systemTime );
    return CDate( (UInt16) systemTime.wYear, (UInt8) systemTime.wMonth, (UInt8) systemTime.wDay );
    
    //#elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX )
    #else

    // Not supported for the current platform
    return CDate( 0, 0, 0 );
    #endif
}

/*-------------------------------------------------------------------------*/

CDate 
CDate::UnixEpochDate( void )
{GUCEF_TRACE;

    return CDate( 1970, 1, 1 );
}

/*-------------------------------------------------------------------------*/

Int16 
CDate::GetYear( void ) const
{GUCEF_TRACE;
    
    return m_year;
}

/*-------------------------------------------------------------------------*/

UInt8 
CDate::GetMonth( void ) const
{GUCEF_TRACE;

    return m_month;
}

/*-------------------------------------------------------------------------*/

UInt8 
CDate::GetDay( void ) const
{GUCEF_TRACE;

    return m_day;
}

/*-------------------------------------------------------------------------*/

CDate& 
CDate::operator=( const CDate& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        m_year = src.m_year;
        m_month = src.m_month;
        m_day = src.m_day;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool 
CDate::operator==( const CDate& src ) const
{GUCEF_TRACE;

    return m_year == src.m_year && m_month == src.m_month && m_day == src.m_day;
}

/*-------------------------------------------------------------------------*/

bool 
CDate::operator!=( const CDate& src ) const
{GUCEF_TRACE;

    return m_year != src.m_year || m_month != src.m_month || m_day != src.m_day;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
