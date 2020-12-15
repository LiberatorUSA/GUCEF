/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
 *
 *  Copyright (C) 1998 - 2020.  Dinand Vanvelzen
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
