/*
 *  GUCEF platform wide macros/defines for the main app entry point
 *  Copyright (C) 2002 - 2011.  Dinand Vanvelzen
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

 #ifndef GUCEF_OSMAIN_H
 #define GUCEF_OSMAIN_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <signal.h>

#ifndef GUCEF_PLATFORM_H
#include "gucef_platform.h"
#define GUCEF_PLATFORM_H
#endif /* GUCEF_PLATFORM_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

#define GUCEF_APP_TYPE_UNKNOWN                  0
#define GUCEF_APP_TYPE_BACKGROUND_PROCESSS      1
#define GUCEF_APP_TYPE_CONSOLE                  2
#define GUCEF_APP_TYPE_GUI                      3

/*-------------------------------------------------------------------------*/

/*
 *  You can override the App type in your project settings
 */
#undef GUCEF_AUTO_APP_TYPE_BACKGROUND_PROCESSS
#undef GUCEF_AUTO_APP_TYPE_CONSOLE
#undef GUCEF_AUTO_APP_TYPE_GUI
#ifdef GUCEF_AUTO_APP_TYPE_OVERRIDE
  #define GUCEF_AUTO_APP_TYPE_BACKGROUND_PROCESSS      GUCEF_AUTO_APP_TYPE_OVERRIDE
  #define GUCEF_AUTO_APP_TYPE_CONSOLE                  GUCEF_AUTO_APP_TYPE_OVERRIDE
  #define GUCEF_AUTO_APP_TYPE_GUI                      GUCEF_AUTO_APP_TYPE_OVERRIDE
#else
  #define GUCEF_AUTO_APP_TYPE_BACKGROUND_PROCESSS      GUCEF_APP_TYPE_BACKGROUND_PROCESSS
  #define GUCEF_AUTO_APP_TYPE_CONSOLE                  GUCEF_APP_TYPE_CONSOLE
  #define GUCEF_AUTO_APP_TYPE_GUI                      GUCEF_APP_TYPE_GUI
#endif

/*-------------------------------------------------------------------------*/

/*
 *  Here we will define the GUCEF_OSMAIN macro which defines a entry point
 *  and makes the following variables available and application parameters:
 *      int argc
 *      char** argv
 *
 *  This makes the parsing of application parameters uniform
 */

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    #include <string.h>
    #include <windows.h>
    #undef min
    #undef max

    #define GUCEF_OSMAIN_BEGIN                                                                         \
                                                                                                       \
    static const unsigned char GUCEF_APP_TYPE = GUCEF_AUTO_APP_TYPE_CONSOLE;                           \
                                                                                                       \
    typedef void ( __cdecl *gucef_installed_signal_handler_ptr)( int );                                \
    static gucef_installed_signal_handler_ptr gucef_installed_signal_handler = NULL;                   \
                                                                                                       \
    void                                                                                               \
    gucef_signal_handler( int signal )                                                                 \
    {                                                                                                  \
        if ( NULL != gucef_installed_signal_handler )                                                  \
        {                                                                                              \
            (*gucef_installed_signal_handler)( signal );                                               \
        }                                                                                              \
    }                                                                                                  \
                                                                                                       \
    BOOL WINAPI Win32ConsoleHandlerRoutine( DWORD dwCtrlType )                                         \
    {                                                                                                  \
        if ( CTRL_C_EVENT == dwCtrlType  )                                                             \
        {                                                                                              \
            gucef_signal_handler( SIGINT );                                                            \
            return TRUE;                                                                               \
        }                                                                                              \
        else                                                                                           \
        if ( CTRL_BREAK_EVENT == dwCtrlType    ||                                                      \
             CTRL_CLOSE_EVENT == dwCtrlType    ||                                                      \
             CTRL_SHUTDOWN_EVENT == dwCtrlType )                                                       \
        {                                                                                              \
            gucef_signal_handler( SIGTERM );                                                           \
            return TRUE;                                                                               \
        }                                                                                              \
        return FALSE;                                                                                  \
    }                                                                                                  \
                                                                                                       \
    int WINAPI                                                                                         \
    WinMain( HINSTANCE hinstance     ,                                                                 \
             HINSTANCE hprevinstance ,                                                                 \
             LPSTR lpcmdline         ,                                                                 \
             int ncmdshow            )                                                                 \
    {                                                                                                  \
                                                                                                       \
                                                                                                       \
        int argc = 0;                                                                                  \
        char** argv = &lpcmdline;                                                                      \
        if ( lpcmdline != NULL )                                                                       \
        {                                                                                              \
            if ( *lpcmdline != '\0' )                                                                  \
            {                                                                                          \
                argc = 1;                                                                              \
            }                                                                                          \
        }

    #define GUCEF_OSMAIN_END }

    #define GUCEF_OSSERVICEMAIN_BEGIN( serviceName )                                                   \
                                                                                                       \
    static const unsigned char GUCEF_APP_TYPE = GUCEF_AUTO_APP_TYPE_BACKGROUND_PROCESSS;               \
                                                                                                       \
    typedef void ( __cdecl *gucef_installed_signal_handler_ptr)( int );                                \
    static gucef_installed_signal_handler_ptr gucef_installed_signal_handler = NULL;                   \
                                                                                                       \
    void                                                                                               \
    gucef_signal_handler( int signal )                                                                 \
    {                                                                                                  \
        if ( NULL != gucef_installed_signal_handler )                                                  \
        {                                                                                              \
            (*gucef_installed_signal_handler)( signal );                                               \
        }                                                                                              \
    }                                                                                                  \
                                                                                                       \
    BOOL WINAPI Win32ConsoleHandlerRoutine( DWORD dwCtrlType )                                         \
    {                                                                                                  \
        if ( CTRL_C_EVENT == dwCtrlType  )                                                             \
        {                                                                                              \
            gucef_signal_handler( SIGINT );                                                            \
            return TRUE;                                                                               \
        }                                                                                              \
        else                                                                                           \
        if ( CTRL_BREAK_EVENT == dwCtrlType    ||                                                      \
             CTRL_CLOSE_EVENT == dwCtrlType    ||                                                      \
             CTRL_SHUTDOWN_EVENT == dwCtrlType )                                                       \
        {                                                                                              \
            gucef_signal_handler( SIGTERM );                                                           \
            return TRUE;                                                                               \
        }                                                                                              \
        return FALSE;                                                                                  \
    }                                                                                                  \
                                                                                                       \
    ::SERVICE_STATUS_HANDLE g_win32ServiceStatusHandle = NULL;                                         \
    ::HANDLE g_win32StopEvent = NULL;                                                                  \
    ::DWORD g_win32CurrentState = 0;                                                                   \
    bool g_win32SystemShutdown = false;                                                                \
                                                                                                       \
    void WINAPI Win32ServiceMain( DWORD argc, LPTSTR* argv );                                          \
    int Win32ServiceInitializer( int argc, char** argv );                                              \
                                                                                                       \
    int __stdcall                                                                                      \
    WinMain( HINSTANCE hinstance     ,                                                                 \
             HINSTANCE hprevinstance ,                                                                 \
             LPSTR lpcmdline         ,                                                                 \
             int ncmdshow            )                                                                 \
    {                                                                                                  \
                                                                                                       \
        int argc = 0;                                                                                  \
        char** argv = &lpcmdline;                                                                      \
        if ( lpcmdline != NULL )                                                                       \
        {                                                                                              \
            if ( *lpcmdline != '\0' )                                                                  \
            {                                                                                          \
                argc = 1;                                                                              \
                if ( NULL != strstr( lpcmdline, "--console" ) )                                        \
                {                                                                                      \
                    return Win32ServiceInitializer( argc, argv );                                      \
                }                                                                                      \
            }                                                                                          \
        }                                                                                              \
                                                                                                       \
        ::SERVICE_TABLE_ENTRY win32ServiceTable[] = {                                                  \
            { "", &Win32ServiceMain },                                                                 \
            { NULL, NULL }                                                                             \
        };                                                                                             \
                                                                                                       \
        if ( ::StartServiceCtrlDispatcher( win32ServiceTable ) )                                       \
            return 0;                                                                                  \
        else if ( ERROR_FAILED_SERVICE_CONTROLLER_CONNECT == ::GetLastError() )                        \
            return -1;                                                                                 \
        else                                                                                           \
            return -2;                                                                                 \
    }                                                                                                  \
                                                                                                       \
    void                                                                                               \
    Win32ReportErrorStatus( DWORD errorCode )                                                          \
    {                                                                                                  \
        g_win32CurrentState = SERVICE_STOPPED;                                                         \
        SERVICE_STATUS serviceStatus = {                                                               \
            SERVICE_WIN32_OWN_PROCESS,                                                                 \
            g_win32CurrentState,                                                                       \
            0,                                                                                         \
            ERROR_SERVICE_SPECIFIC_ERROR,                                                              \
            errorCode,                                                                                 \
            0,                                                                                         \
            0,                                                                                         \
        };                                                                                             \
        ::SetServiceStatus( g_win32ServiceStatusHandle, &serviceStatus );                              \
    }                                                                                                  \
                                                                                                       \
    void                                                                                               \
    Win32ReportStatus( DWORD state )                                                                   \
    {                                                                                                  \
        g_win32CurrentState = state;                                                                   \
        SERVICE_STATUS serviceStatus = {                                                               \
            SERVICE_WIN32_OWN_PROCESS,                                                                 \
            g_win32CurrentState,                                                                       \
            state == SERVICE_START_PENDING ? 0 : SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN,        \
            NO_ERROR,                                                                                  \
            0,                                                                                         \
            0,                                                                                         \
            0,                                                                                         \
        };                                                                                             \
        ::SetServiceStatus( g_win32ServiceStatusHandle, &serviceStatus );                              \
    }                                                                                                  \
                                                                                                       \
    DWORD WINAPI Win32HandlerEx( DWORD control, DWORD eventType, void* eventData, void* context )      \
    {                                                                                                  \
        switch ( control )                                                                             \
        {                                                                                              \
            case SERVICE_CONTROL_SHUTDOWN:                                                             \
            {                                                                                          \
                g_win32SystemShutdown = true;                                                          \
                break;                                                                                 \
            }                                                                                          \
            case SERVICE_CONTROL_STOP:                                                                 \
            {                                                                                          \
                Win32ReportStatus( SERVICE_STOP_PENDING );                                             \
                ::SetEvent( g_win32StopEvent );                                                        \
                ::GUCEF::CORE::CCoreGlobal::Instance()->GetApplication().Stop();                       \
                break;                                                                                 \
            }                                                                                          \
            default:                                                                                   \
            {                                                                                          \
                Win32ReportStatus( g_win32CurrentState );                                              \
                break;                                                                                 \
            }                                                                                          \
        }                                                                                              \
        return NO_ERROR;                                                                               \
    }                                                                                                  \
                                                                                                       \
    void WINAPI Win32ServiceMain( DWORD argc, LPTSTR *argv )                                           \
    {                                                                                                  \
        g_win32ServiceStatusHandle =                                                                   \
            ::RegisterServiceCtrlHandlerEx( serviceName, &Win32HandlerEx, NULL);                       \
                                                                                                       \
        Win32ReportStatus( SERVICE_RUNNING );                                                          \
        g_win32StopEvent = ::CreateEvent( NULL, TRUE, FALSE, NULL );                                   \
        int appResult = Win32ServiceInitializer( argc, argv );                                         \
        ::CloseHandle( g_win32StopEvent );                                                             \
                                                                                                       \
        if ( appResult != 0 )                                                                          \
            Win32ReportErrorStatus( (DWORD)appResult );                                                \
        else                                                                                           \
            Win32ReportStatus( SERVICE_STOPPED );                                                      \
    }                                                                                                  \
                                                                                                       \
    int                                                                                                \
    Win32ServiceInitializer( int argc, char** argv )                                                   \
    {

    #define GUCEF_OSSERVICEMAIN_END }

    #define GUCEF_OSMAIN_SIGNAL_HANDLER( signalHandlerFunc )                                           \
    {                                                                                                  \
        if ( GUCEF_APP_TYPE_CONSOLE == GUCEF_APP_TYPE )                                                \
            ::SetConsoleCtrlHandler( &Win32ConsoleHandlerRoutine, TRUE );                              \
        gucef_installed_signal_handler = &signalHandlerFunc;                                           \
        signal( SIGINT, &signalHandlerFunc );                                                          \
        signal( SIGTERM, &signalHandlerFunc );                                                         \
    }

#elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    #include <string.h>
    #include <unistd.h>

    #define GUCEF_OSMAIN_BEGIN_APPTYPE( appType )                                                      \
                                                                                                       \
    static const unsigned char GUCEF_APP_TYPE = appType;                                               \
    typedef void ( *gucef_installed_signal_handler_ptr)( int );                                        \
    static gucef_installed_signal_handler_ptr gucef_installed_signal_handler = NULL;                   \
                                                                                                       \
    void linux_sig_term_handler( int signum, siginfo_t* info, void* ptr )                              \
    {                                                                                                  \
        if ( NULL != gucef_installed_signal_handler )                                                  \
            gucef_installed_signal_handler( signum );                                                  \
    }                                                                                                  \
                                                                                                       \
    int                                                                                                \
    main( int argc, char* argv[] )                                                                     \
    {

    #define GUCEF_OSMAIN_BEGIN      GUCEF_OSMAIN_BEGIN_APPTYPE( GUCEF_AUTO_APP_TYPE_CONSOLE )

    #define GUCEF_OSMAIN_END }

    #define GUCEF_OSSERVICEMAIN_BEGIN( serviceName )    GUCEF_OSMAIN_BEGIN_APPTYPE( GUCEF_AUTO_APP_TYPE_BACKGROUND_PROCESSS )
    #define GUCEF_OSSERVICEMAIN_END                     GUCEF_OSMAIN_END

    #define GUCEF_OSMAIN_SIGNAL_HANDLER( signalHandlerFunc )                                           \
    {                                                                                                  \
        gucef_installed_signal_handler = &signalHandlerFunc;                                           \
                                                                                                       \
        static struct sigaction _sigact;                                                               \
        memset( &_sigact, 0, sizeof(_sigact) );                                                        \
        _sigact.sa_flags = SA_SIGINFO;                                                                 \
        _sigact.sa_sigaction = linux_sig_term_handler;                                                 \
                                                                                                       \
        sigaction( SIGINT, &_sigact, NULL );                                                           \
        sigaction( SIGTERM, &_sigact, NULL );                                                          \
    }

#else

    #define GUCEF_OSMAIN_BEGIN_APPTYPE( appType )                                                      \
                                                                                                       \
    static const unsigned char GUCEF_APP_TYPE = appType;                                               \
    typedef void ( *gucef_installed_signal_handler_ptr)( int );                                        \
    static gucef_installed_signal_handler_ptr gucef_installed_signal_handler = NULL;                   \
                                                                                                       \
    int                                                                                                \
    main( int argc, char* argv[] )                                                                     \
    {

    #define GUCEF_OSMAIN_BEGIN      GUCEF_OSMAIN_BEGIN_APPTYPE( GUCEF_AUTO_APP_TYPE_CONSOLE )

    #define GUCEF_OSMAIN_END }

    #define GUCEF_OSSERVICEMAIN_BEGIN( serviceName )    GUCEF_OSMAIN_BEGIN_APPTYPE( GUCEF_AUTO_APP_TYPE_BACKGROUND_PROCESSS )
    #define GUCEF_OSSERVICEMAIN_END                     GUCEF_OSMAIN_END

    #define GUCEF_OSMAIN_SIGNAL_HANDLER( signalHandlerFunc )

#endif

/*-------------------------------------------------------------------------*/

 #endif /* GUCEF_OSMAIN_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 02-01-2007 :
       - Initial version of this file.

---------------------------------------------------------------------------*/
