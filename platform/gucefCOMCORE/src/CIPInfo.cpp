/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
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

#include "CIPInfo.h"  

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CIPInfo::CIPInfo( void )
    : ip()
    , subnet()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CIPInfo::CIPInfo( const CString& ipAsString, const CString& subnetAsString, bool resolveDns )
    : ip()
    , subnet()
{GUCEF_TRACE;

    if ( resolveDns )
    {
        ip.ResolveDNS( ipAsString, 0 );
        subnet.ResolveDNS( subnetAsString, 0 );
    }
    else
    {
        ip.SetAddress( ipAsString );
        subnet.SetAddress( subnetAsString );
    }
}

/*-------------------------------------------------------------------------*/

CIPInfo::CIPInfo( const CIPInfo& src )
    : ip( src.ip )
    , subnet( src.subnet )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CIPInfo::~CIPInfo()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CIPInfo& 
CIPInfo::operator=( const CIPInfo& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        ip = src.ip;
        subnet = src.subnet;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
CIPInfo::operator<( const CIPInfo& other ) const
{GUCEF_TRACE;

    return ( ip < other.ip ) && ( subnet < other.subnet );
}

/*-------------------------------------------------------------------------*/

void
CIPInfo::Clear( void )
{GUCEF_TRACE;

    ip.Clear();
    subnet.Clear();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
