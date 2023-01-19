/*
 *  gucefCORE_TestApp: GUCEF test application for the CORE module
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

#include <iostream>

#ifndef GUCEF_CORE_CSTRING_H
#include "gucefCORE_CString.h"
#define GUCEF_CORE_CSTRING_H
#endif /* GUCEF_CORE_CSTRING_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_DVCPPOSWRAP_H
#include "DVCPPOSWRAP.h"
#define GUCEF_CORE_DVCPPOSWRAP_H
#endif /* GUCEF_CORE_DVCPPOSWRAP_H ? */

#include "TestVars.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

#if GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX || GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID
  #define DEBUGBREAK __builtin_trap()
#elif GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN
  #define DEBUGBREAK DebugBreak()
#else
  #define DEBUGBREAK
#endif

#define ERRORHERE { std::cout << "Test failed @ " << __FILE__ << "(" << __LINE__ << ")\n"; DEBUGBREAK; }
#define ASSERT_TRUE( test ) if ( !(test) ) { ERRORHERE; } 
#define ASSERT_FALSE( test ) if ( (test) ) { ERRORHERE; }

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

using namespace GUCEF;

/*-------------------------------------------------------------------------*/

void
PerformVarsTests( void )
{
    std::cout << "\n\n**** COMMENCING variable resolution TESTS ****\n";
    
    try
    {
        // $CURWORKDIR$ -> Is resolved to the current working directory
        
        CORE::CString currentWorkingDir = CORE::ResolveVars( "$CURWORKDIR$" );
        ASSERT_TRUE( !currentWorkingDir.IsNULLOrEmpty() );
        ASSERT_TRUE( currentWorkingDir.IsFormattingValid() );

        // $MODULEDIR$  -> Is resolved to the directory this module is located in
        
        CORE::CString moduleDir = CORE::ResolveVars( "$MODULEDIR$" );
        ASSERT_TRUE( !moduleDir.IsNULLOrEmpty() );
        ASSERT_TRUE( moduleDir.IsFormattingValid() );

        // $HOSTNAME$  -> Is resolved to the hostname of the system
        
//        CORE::CString hostname = CORE::ResolveVars( "$HOSTNAME$" );
//        ASSERT_TRUE( !hostname.IsNULLOrEmpty() );
//        ASSERT_TRUE( hostname.IsFormattingValid() );

        // $PID$  -> Is resolved to the process ID of the current process
        
        CORE::CString pidStr = CORE::ResolveVars( "$PID$" );
        ASSERT_TRUE( !pidStr.IsNULLOrEmpty() );
        ASSERT_TRUE( pidStr.IsFormattingValid() );
        ASSERT_TRUE( -1 != CORE::StringToInt64( pidStr, -1 ) );

        // $ENVVAR:FOO$ -> Resolves to the environment variable FOO's content
        
        CORE::CString envVarStr = CORE::ResolveVars( "$ENVVAR:__envVarWhichDoesNotExist__$" );
        ASSERT_TRUE( envVarStr.IsNULLOrEmpty() );
        ASSERT_TRUE( envVarStr.IsFormattingValid() );
        
        #if GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX || GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID
          envVarStr = CORE::ResolveVars( "$ENVVAR:PWD" );
          ASSERT_TRUE( !envVarStr.IsNULLOrEmpty() );
          ASSERT_TRUE( envVarStr.IsFormattingValid() );
          ASSERT_TRUE( CORE::IsPathValid( envVarStr ) );
          ASSERT_TRUE( envVarStr == CORE::GetEnv( "PWD" ) );
        #elif GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN
          envVarStr = CORE::ResolveVars( "$ENVVAR:windir$" );
          ASSERT_TRUE( !envVarStr.IsNULLOrEmpty() );
          ASSERT_TRUE( envVarStr.IsFormattingValid() );
          ASSERT_TRUE( CORE::IsPathValid( envVarStr ) );
          ASSERT_TRUE( envVarStr == CORE::GetEnv( "windir" ) );
        #endif
        
        ASSERT_TRUE( 0 != CORE::SetEnv( "testEnvironmentVariable", "testEnvironmentVariableValue" ) );
        envVarStr = CORE::ResolveVars( "$ENVVAR:testEnvironmentVariable$" );
        ASSERT_TRUE( !envVarStr.IsNULLOrEmpty() );
        ASSERT_TRUE( envVarStr.IsFormattingValid() );
        ASSERT_TRUE( "testEnvironmentVariableValue" == envVarStr );

        ASSERT_TRUE( 0 != CORE::SetEnv( "testEnvironmentVariable_FOO", "FOO" ) );
        ASSERT_TRUE( 0 != CORE::SetEnv( "testEnvironmentVariable_BAR", "BAR" ) );
        envVarStr = CORE::ResolveVars( "$ENVVAR:testEnvironmentVariable_FOO$:$ENVVAR:testEnvironmentVariable_BAR$" );
        ASSERT_TRUE( !envVarStr.IsNULLOrEmpty() );
        ASSERT_TRUE( envVarStr.IsFormattingValid() );
        ASSERT_TRUE( "FOO:BAR" == envVarStr );

    }
    catch( ... )
    {
        ERRORHERE;
    }

    std::cout << "\n\n**** FINISHED variable resolution TESTS ****\n";
}

/*-------------------------------------------------------------------------*/
