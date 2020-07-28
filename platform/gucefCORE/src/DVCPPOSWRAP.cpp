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
#include <iostream>
#include <stdexcept>
#include <string>
#include <stdio.h>
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

/*-------------------------------------------------------------------------*/

bool
CommandLineExecute( const CString& command , 
                    CString& result        , 
                    bool waitForExit       ) 
{GUCEF_TRACE;

    result.Clear();
    if ( command.IsNULLOrEmpty() )
        return false;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    
    HANDLE hPipeRead = NULL;
    HANDLE hPipeWrite = NULL;

    SECURITY_ATTRIBUTES saAttr = {sizeof(SECURITY_ATTRIBUTES)};
    saAttr.bInheritHandle = TRUE; // Pipe handles are inherited by child process.
    saAttr.lpSecurityDescriptor = NULL;

    // Create a pipe to get results from child's stdout.
    if ( FALSE == ::CreatePipe( &hPipeRead, &hPipeWrite, &saAttr, 0 ) )
    {
        return false;
    }

    STARTUPINFOA si = {sizeof(STARTUPINFOW)};
    si.dwFlags     = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    si.hStdOutput  = hPipeWrite;
    si.hStdError   = hPipeWrite;
    si.wShowWindow = SW_HIDE; // <- Prevents cmd window from flashing, this requires STARTF_USESHOWWINDOW in dwFlags.

    PROCESS_INFORMATION pi = { 0 };
    BOOL fSuccess = ::CreateProcessA( NULL, (LPSTR) command.C_String(), NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi );
    if ( FALSE == fSuccess )
    {
        ::CloseHandle( hPipeWrite );
        ::CloseHandle( hPipeRead );
        return false;
    }

    if ( waitForExit )
    {
        bool bProcessEnded = false;
        while ( !bProcessEnded )
        {
            // Give some timeslice (50 ms), so we won't waste 100% CPU.
            bProcessEnded = WaitForSingleObject( pi.hProcess, 50) == WAIT_OBJECT_0;

            // Even if process exited - we continue reading, if
            // there is some data available over pipe.
            for (;;)
            {
                char buf[1024];
                DWORD dwRead = 0;
                DWORD dwAvail = 0;

                if ( !::PeekNamedPipe( hPipeRead, NULL, 0, NULL, &dwAvail, NULL ) )
                    break;

                if (!dwAvail) // No data available, return
                    break;

                if ( !::ReadFile( hPipeRead, buf, SMALLEST( sizeof(buf)-1, dwAvail ), &dwRead, NULL ) || !dwRead )
                {
                    // Error, the child process might have ended
                    break;
                }

                buf[ dwRead ] = 0;
                result += buf;
            }
        } 
    }

    ::CloseHandle( hPipeWrite );
    ::CloseHandle( hPipeRead );
    ::CloseHandle( pi.hProcess );
    ::CloseHandle( pi.hThread );
    return true;

    #else

    char buffer[ 128 ];
    std::string result = "";
    FILE* pipe = popen( command.C_String(), "r" );
    if ( NULL == pipe )
        return false;

    try 
    {
        while ( fgets( buffer, sizeof buffer, pipe ) != NULL ) 
        {
            result += buffer;
        }
    }
    catch ( const std::exception& e ) 
    {
        pclose( pipe );
        throw e;
    }
    catch ( ... ) 
    {
        pclose( pipe );
        throw;
    }
    pclose( pipe );
    return true;

    #endif
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
