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
    
    virtual void SetHours( Int8 hours );
    
    virtual Int8 GetHours( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual void SetMinutes( Int8 minutes );
    
    virtual Int8 GetMinutes( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual void SetSeconds( Int8 seconds );

    virtual Int8 GetSeconds( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual void SetMilliseconds( Int16 milliseconds );
    
    virtual Int16 GetMilliseconds( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool FromIso8601TimeString( const CString& source );
    virtual CString ToIso8601TimeString( bool includeDelimeters ) const;

    CTime& operator=( const CTime& src );

    bool operator==( const CTime& src ) const;

    bool operator!=( const CTime& src ) const;

    CTime( void );
    CTime( const time_t& src, bool isUtc );
    CTime( Int8 hours, Int8 minutes, Int8 seconds, Int16 milliseconds );
    CTime( const CTime& src );
    virtual ~CTime();
    
    protected:

    Int8 m_hours;
    Int8 m_minutes;
    Int8 m_seconds;
    Int16 m_milliseconds;
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
