/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
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

#include <map>
#include <string>
#include <assert.h>

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_MT_CSCOPEMUTEX_H
#include "gucefMT_CScopeMutex.h"
#define GUCEF_MT_CSCOPEMUTEX_H
#endif /* GUCEF_MT_CSCOPEMUTEX_H ? */

#ifndef GUCEF_CORE_DVCPPOSWRAP_H
#include "DVCPPOSWRAP.h"
#define GUCEF_CORE_DVCPPOSWRAP_H
#endif /* GUCEF_CORE_DVCPPOSWRAP_H ? */

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

  #include <WinSock2.h>

#elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

  #include <unistd.h>

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
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

typedef std::map< CString, CString > TStringMap;
typedef std::vector< CString > TStringVector;
typedef std::pair< CString, CString > TStringPair;

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

MT::CMutex g_envOverridesLock;
TStringMap g_envOverrides;

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

UInt32
SetEnv( const CString& key   ,
        const CString& value )
{GUCEF_TRACE;

    return GUCEFSetEnv( key.C_String(), value.C_String() );
}

/*--------------------------------------------------------------------------*/

void
SetEnvOverride( const CString& key   ,
                const CString& value )
{GUCEF_TRACE;
    
    MT::CScopeMutex scopeLock( g_envOverridesLock );
    g_envOverrides[ key ] = value;
}

/*--------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP void
SetEnvOverrides( const CString& allKeysAndValues )
{GUCEF_TRACE;

    TStringMap envOverrides;
    TStringVector elements = allKeysAndValues.ParseElements( '\n', false );
    TStringVector::iterator i = elements.begin();
    while ( i != elements.end() )
    {
        Int32 sepIndex = (*i).HasChar( '=', 0, true );
        if ( sepIndex >= 0 )
        {
            envOverrides.insert( TStringPair( (*i).SubstrToIndex( sepIndex, true ), (*i).SubstrToIndex( sepIndex, false ) ) );
        }
        ++i;
    }

    {
        MT::CScopeMutex scopeLock( g_envOverridesLock );
        TStringMap::iterator n = envOverrides.begin();
        while ( n != envOverrides.end() )
        {
            g_envOverrides[ (*n).first ] = (*n).second;
            ++n;
        }
    }
}

/*--------------------------------------------------------------------------*/

CString
GetEnv( const CString& key )
{GUCEF_TRACE;

    if ( key.IsNULLOrEmpty() ) return CString();
    
    {
        MT::CScopeMutex scopeLock( g_envOverridesLock );
        TStringMap::iterator i = g_envOverrides.find( key );
        if ( i != g_envOverrides.end() )
        {
            // we only allow this right now since there is no way to override
            return (*i).second;
        }
    }
    return GUCEFGetEnv( key.C_String() );
}

/*-------------------------------------------------------------------------*/

CString
GetHostname( void )
{GUCEF_TRACE;

#if ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    // Use the POSIX function gethostname()
    char nameBuffer[ 256 ];
    if ( 0 == gethostname( nameBuffer, sizeof(nameBuffer) ) )
    {
        nameBuffer[ 255 ] = 0;
        return nameBuffer;
    }

#endif    
    
    return CString(); 
}

/*-------------------------------------------------------------------------*/

bool
GetExeNameForProcessId( TProcessId* pid  , 
                        CString& exeName )
{GUCEF_TRACE;

    UInt32 nameLength = 0;
    char* nameBuffer = exeName.Reserve( 1024 );
    if ( OSWRAP_TRUE == GetExeNameForProcessId( pid, nameBuffer, 1024, &nameLength ) )
    {
        exeName.SetLength( nameLength );
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
