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

#ifndef GUCEF_CORE_CTIME_H
#define GUCEF_CORE_CTIME_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_CSTRING_H
#include "gucefCORE_CString.h"
#define GUCEF_CORE_CSTRING_H
#endif /* GUCEF_CORE_CSTRING_H ? */

#ifndef GUCEF_CORE_CITIME_H
#include "gucefCORE_CITime.h"
#define GUCEF_CORE_CITIME_H
#endif /* GUCEF_CORE_CITIME_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_CORE_PUBLIC_CPP CTime : public CITime
{
    public:

    static CTime NowUTCTime( void );

    static CTime NowLocalTime( void );
    
    virtual UInt8 GetHours( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual UInt8 GetMinutes( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual UInt8 GetSeconds( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual void SetMilliseconds( UInt16 milliseconds );
    
    virtual UInt16 GetMilliseconds( void ) const GUCEF_VIRTUAL_OVERRIDE;

    CTime& operator=( const CTime& src );

    bool operator==( const CTime& src ) const;

    bool operator!=( const CTime& src ) const;

    CTime( void );
    CTime( const time_t& src, bool isUtc );
    CTime( UInt8 hours, UInt8 minutes, UInt8 seconds, UInt16 milliseconds );
    CTime( const CTime& src );
    virtual ~CTime();
    
    protected:

    UInt8 m_hours;
    UInt8 m_minutes;
    UInt8 m_seconds;
    UInt16 m_milliseconds;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTIME_H ? */
