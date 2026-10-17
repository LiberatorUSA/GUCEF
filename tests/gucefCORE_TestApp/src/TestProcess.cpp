/*
 *  gucefCORE_TestApp: GUCEF test application for the CORE module
 *  Copyright (C) 2002 - 2026.  Dinand Vanvelzen
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

#ifndef GUCEF_CORE_DVCPPOSWRAP_H
#include "DVCPPOSWRAP.h"
#define GUCEF_CORE_DVCPPOSWRAP_H
#endif /* GUCEF_CORE_DVCPPOSWRAP_H ? */

#ifndef GUCEF_CORE_CPROCESS_H
#include "gucefCORE_CProcess.h"
#define GUCEF_CORE_CPROCESS_H
#endif /* GUCEF_CORE_CPROCESS_H ? */

#ifndef GUCEF_CORE_DVFILEUTILS_H
#include "dvfileutils.h"
#define GUCEF_CORE_DVFILEUTILS_H
#endif /* GUCEF_CORE_DVFILEUTILS_H ? */

#ifndef GUCEF_CORE_DVCPPFILEUTILS_H
#include "dvcppfileutils.h"
#define GUCEF_CORE_DVCPPFILEUTILS_H
#endif /* GUCEF_CORE_DVCPPFILEUTILS_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_MT_DVMTOSWRAP_H
#include "gucefMT_dvmtoswrap.h"
#define GUCEF_MT_DVMTOSWRAP_H
#endif /* GUCEF_MT_DVMTOSWRAP_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_TEST_FRAMEWORK_H
#include "gucef_test_framework.h"
#define GUCEF_TEST_FRAMEWORK_H
#endif /* GUCEF_TEST_FRAMEWORK_H ? */

#include "TestProcess.h"

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
  #include <windows.h>
#elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
  #include <sys/types.h>
  #include <sys/wait.h>
  #include <signal.h>
  #include <unistd.h>
#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

#define ERRORHERE       GUCEF_TESTFW_ERRORHERE
#define ASSERT_TRUE(t)  GUCEF_TESTFW_ASSERT_TRUE(t)
#define ASSERT_FALSE(t) GUCEF_TESTFW_ASSERT_FALSE(t)

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

using namespace GUCEF;

/*-------------------------------------------------------------------------*/

static CORE::TProcessId
GetOwnProcessId( void )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    return (CORE::TProcessId) ::GetCurrentProcessId();
    #else
    return (CORE::TProcessId) ::getpid();
    #endif
}

/*-------------------------------------------------------------------------*/

/**
 *  Describes a process that does nothing for a long time
 */
static void
GetSleeperCommand( CORE::CString& exe, CORE::CProcess::TStringVector& args )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    exe = "ping";
    args.push_back( "-n" );
    args.push_back( "60" );
    args.push_back( "127.0.0.1" );
    #else
    exe = "sleep";
    args.push_back( "60" );
    #endif
}

/*-------------------------------------------------------------------------*/

/**
 *  Describes a process that exits right away with the given exit code
 */
static void
GetExitCodeCommand( CORE::CString& exe, CORE::CProcess::TStringVector& args, UInt32 exitCode )
{
    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    exe = "cmd.exe";
    args.push_back( "/c" );
    args.push_back( "exit " + CORE::ToString( exitCode ) );
    #else
    exe = "sh";
    args.push_back( "-c" );
    args.push_back( "exit " + CORE::ToString( exitCode ) );
    #endif
}

/*-------------------------------------------------------------------------*/

void
PerformProcessTests( void )
{
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "COMMENCING Process TESTS" );

    GUCEF_TESTFW_SUITE_SCOPE( "Process" );

    // Regression: on Linux CheckOnProcessAliveStatus() never set the status output
    // and reported failure instead of 'not alive' for processes that no longer exist
    GUCEF_TESTFW_TESTCASE( "Test: CheckOnProcessAliveStatus for the own process" )
        try
        {
            bool isAlive = false;
            ASSERT_TRUE( CORE::CheckOnProcessAliveStatus( GetOwnProcessId(), isAlive ) );
            ASSERT_TRUE( isAlive );
        }
        catch( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    #if ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    GUCEF_TESTFW_TESTCASE( "Test: CheckOnProcessAliveStatus for a process that exited" )
        try
        {
            pid_t childPid = ::fork();
            if ( 0 == childPid )
                ::_exit( 0 );
            ASSERT_TRUE( childPid > 0 );

            int childStatus = 0;
            ASSERT_TRUE( childPid == ::waitpid( childPid, &childStatus, 0 ) );

            bool isAlive = true;
            ASSERT_TRUE( CORE::CheckOnProcessAliveStatus( (CORE::TProcessId) childPid, isAlive ) );
            ASSERT_FALSE( isAlive );
        }
        catch( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    #endif

    GUCEF_TESTFW_TESTCASE( "Test: CProcess::SplitCommandLine" )
        try
        {
            CORE::CProcess::TStringVector args = CORE::CProcess::SplitCommandLine( "add \"my dir/file name.txt\"  -m 'a message' plain\\x" );
            ASSERT_TRUE( 5 == args.size() );
            if ( 5 == args.size() )
            {
                ASSERT_TRUE( args[ 0 ] == "add" );
                ASSERT_TRUE( args[ 1 ] == "my dir/file name.txt" );
                ASSERT_TRUE( args[ 2 ] == "-m" );
                ASSERT_TRUE( args[ 3 ] == "a message" );
                ASSERT_TRUE( args[ 4 ] == "plain\\x" );
            }
            ASSERT_TRUE( CORE::CProcess::SplitCommandLine( "   " ).empty() );
            ASSERT_TRUE( CORE::CProcess::SplitCommandLine( "" ).empty() );

            // an empty quoted argument is still an argument
            args = CORE::CProcess::SplitCommandLine( "a \"\" b" );
            ASSERT_TRUE( 3 == args.size() );
            args = CORE::CProcess::SplitCommandLine( "say \"he said \\\"hi\\\"\"" );
            ASSERT_TRUE( 2 == args.size() );
            if ( 2 == args.size() )
                ASSERT_TRUE( args[ 1 ] == "he said \"hi\"" );
        }
        catch( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( "Test: CProcess launch, running state and forced terminate" )
        try
        {
            CORE::CString exe;
            CORE::CProcess::TStringVector args;
            GetSleeperCommand( exe, args );

            CORE::CProcess process;
            ASSERT_TRUE( 0 == process.GetProcessId() );
            ASSERT_FALSE( process.IsRunning() );
            ASSERT_TRUE( process.Launch( exe, args ) );
            ASSERT_TRUE( process.GetProcessId() > 0 );
            ASSERT_TRUE( process.IsRunning() );

            bool isAlive = false;
            ASSERT_TRUE( CORE::CheckOnProcessAliveStatus( process.GetProcessId(), isAlive ) );
            ASSERT_TRUE( isAlive );

            // it is still running so waiting must time out
            Int32 exitCode = 0;
            ASSERT_FALSE( process.WaitForExit( 100, exitCode ) );

            // a second launch while the first one is running is refused
            ASSERT_FALSE( process.Launch( exe, args ) );

            ASSERT_TRUE( process.Terminate( false ) );
            ASSERT_FALSE( process.IsRunning() );
            ASSERT_TRUE( process.WaitForExit( 0, exitCode ) );

            ASSERT_TRUE( CORE::CheckOnProcessAliveStatus( process.GetProcessId(), isAlive ) );
            ASSERT_FALSE( isAlive );
        }
        catch( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( "Test: CProcess reports the exit code of a process that ends by itself" )
        try
        {
            CORE::CString exe;
            CORE::CProcess::TStringVector args;
            GetExitCodeCommand( exe, args, 7 );

            CORE::CProcess process;
            ASSERT_TRUE( process.Launch( exe, args ) );
            Int32 exitCode = 0;
            ASSERT_TRUE( process.WaitForExit( 10000, exitCode ) );
            ASSERT_TRUE( 7 == exitCode );
            ASSERT_FALSE( process.IsRunning() );

            // the same instance can be used for another process
            args.clear();
            GetExitCodeCommand( exe, args, 0 );
            ASSERT_TRUE( process.Launch( exe, args ) );
            ASSERT_TRUE( process.WaitForExit( 10000, exitCode ) );
            ASSERT_TRUE( 0 == exitCode );
        }
        catch( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( "Test: CProcess launch of a missing executable fails cleanly" )
        try
        {
            CORE::CProcess process;
            ASSERT_FALSE( process.Launch( "gucef_this_executable_does_not_exist_anywhere", CORE::CProcess::TStringVector() ) );
            ASSERT_TRUE( 0 == process.GetProcessId() );
            ASSERT_FALSE( process.IsRunning() );
            ASSERT_FALSE( process.Launch( "", CORE::CProcess::TStringVector() ) );
        }
        catch( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( "Test: CProcess destructor terminates a non-detached process" )
        try
        {
            CORE::CString exe;
            CORE::CProcess::TStringVector args;
            GetSleeperCommand( exe, args );

            UInt32 pid = 0;
            {
                CORE::CProcess process;
                ASSERT_TRUE( process.Launch( exe, args ) );
                pid = process.GetProcessId();
            }
            ASSERT_TRUE( pid > 0 );
            bool isAlive = true;
            ASSERT_TRUE( CORE::CheckOnProcessAliveStatus( pid, isAlive ) );
            ASSERT_FALSE( isAlive );
        }
        catch( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    #if ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    GUCEF_TESTFW_TESTCASE( "Test: CProcess graceful terminate asks nicely first" )
        try
        {
            CORE::CString exe;
            CORE::CProcess::TStringVector args;
            GetSleeperCommand( exe, args );

            CORE::CProcess process;
            ASSERT_TRUE( process.Launch( exe, args ) );
            ASSERT_TRUE( process.Terminate( true, 5000 ) );

            // sleep honors SIGTERM so we expect that signal to have been the cause: 128 + 15
            Int32 exitCode = 0;
            ASSERT_TRUE( process.WaitForExit( 0, exitCode ) );
            ASSERT_TRUE( 143 == exitCode );
        }
        catch( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( "Test: CProcess graceful terminate falls back to force for a process that ignores the request" )
        try
        {
            CORE::CProcess::TStringVector args;
            args.push_back( "-c" );
            args.push_back( "trap '' TERM; while :; do sleep 1; done" );

            CORE::CProcess process;
            ASSERT_TRUE( process.Launch( "sh", args ) );
            MT::ThreadDelay( 200 );    // give the shell time to install the trap
            ASSERT_TRUE( process.Terminate( true, 300 ) );
            ASSERT_FALSE( process.IsRunning() );

            Int32 exitCode = 0;
            ASSERT_TRUE( process.WaitForExit( 0, exitCode ) );
            ASSERT_TRUE( 137 == exitCode );     // 128 + SIGKILL
        }
        catch( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( "Test: CProcess working dir and environment overrides reach the child" )
        try
        {
            CORE::CString dir = CORE::CombinePath( CORE::TempDir(), "gucefCORE_TestApp_ProcessDir" );
            ASSERT_TRUE( CORE::CreateDirs( dir ) );

            // exit 0 only if both the env var and the working dir are as expected
            CORE::CProcess::TStringVector args;
            args.push_back( "-c" );
            args.push_back( "[ \"$GUCEF_TEST_ENV\" = \"hello world\" ] && [ \"$(pwd -P)\" = \"$(cd '" + dir + "' && pwd -P)\" ] && exit 0; exit 3" );

            CORE::CProcess::TStringMap env;
            env[ "GUCEF_TEST_ENV" ] = "hello world";

            CORE::CProcess process;
            ASSERT_TRUE( process.Launch( "sh", args, dir, env ) );
            Int32 exitCode = -1;
            ASSERT_TRUE( process.WaitForExit( 10000, exitCode ) );
            ASSERT_TRUE( 0 == exitCode );

            // without the override the check must fail, which proves the check itself works
            ASSERT_TRUE( process.Launch( "sh", args, dir ) );
            ASSERT_TRUE( process.WaitForExit( 10000, exitCode ) );
            ASSERT_TRUE( 3 == exitCode );

            // an unusable working dir is a launch failure
            ASSERT_FALSE( process.Launch( "sh", args, CORE::CombinePath( dir, "does/not/exist" ) ) );
        }
        catch( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( "Test: CProcess detached process outlives the CProcess object" )
        try
        {
            CORE::CString exe;
            CORE::CProcess::TStringVector args;
            GetSleeperCommand( exe, args );

            UInt32 pid = 0;
            {
                CORE::CProcess process;
                ASSERT_TRUE( process.Launch( exe, args, CORE::CString(), CORE::CProcess::TStringMap(), true ) );
                pid = process.GetProcessId();
            }
            ASSERT_TRUE( pid > 0 );

            bool isAlive = false;
            ASSERT_TRUE( CORE::CheckOnProcessAliveStatus( pid, isAlive ) );
            ASSERT_TRUE( isAlive );

            // it got its own session so it is not part of our process group
            ASSERT_TRUE( ::getpgid( (pid_t) pid ) != ::getpgid( 0 ) );

            // clean up after ourselves
            ::kill( (pid_t) pid, SIGKILL );
            int status = 0;
            ::waitpid( (pid_t) pid, &status, 0 );
        }
        catch( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    // F3: Execute_Program was an empty stub on Linux
    GUCEF_TESTFW_TESTCASE( "Test: Execute_Program launches programs" )
        try
        {
            ASSERT_TRUE( 0 != CORE::Execute_Program( "sh", "-c 'exit 0'" ) );
            ASSERT_TRUE( 0 == CORE::Execute_Program( "gucef_this_executable_does_not_exist_anywhere", GUCEF_NULL ) );
            ASSERT_TRUE( 0 == CORE::Execute_Program( GUCEF_NULL, GUCEF_NULL ) );

            // the program really ran: it creates a file
            CORE::CString dir = CORE::CombinePath( CORE::TempDir(), "gucefCORE_TestApp_ExecuteProgram" );
            ASSERT_TRUE( CORE::CreateDirs( dir ) );
            CORE::CString marker = CORE::CombinePath( dir, "marker.txt" );
            if ( CORE::FileExists( marker ) )
                CORE::DeleteFile( marker );
            ASSERT_TRUE( 0 != CORE::Execute_Program( "sh", ( "-c \"echo done > '" + marker + "'\"" ).C_String() ) );

            bool markerSeen = false;
            for ( UInt32 i=0; i<200 && !markerSeen; ++i )
            {
                markerSeen = CORE::FileExists( marker );
                if ( !markerSeen )
                    MT::ThreadDelay( 25 );
            }
            ASSERT_TRUE( markerSeen );
        }
        catch( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    #endif

    CORE::CLogStreamScope::FlushLogs();
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "ALL Process TESTS COMPLETED" );
}

/*-------------------------------------------------------------------------*/
