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
 
#ifndef GUCEF_PLATFORM_H
#include "gucef_platform.h"
#define GUCEF_PLATFORM_H
#endif /* GUCEF_PLATFORM_H ? */
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/ 

/*
 *  Here we will define the GUCEF_OSMAIN macro which defines a entry point
 *  and makes the following variables available and application parameters:
 *      int argc
 *      char** argv
 *
 *  This makes the parsing of application parameters uniform
 */
 
#if GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN

    #include <string.h>
    #include <windows.h>
    #undef min
    #undef max

    #define GUCEF_OSMAIN_BEGIN                \
                                              \
    int __stdcall                             \
    WinMain( HINSTANCE hinstance     ,        \
             HINSTANCE hprevinstance ,        \
             LPSTR lpcmdline         ,        \
             int ncmdshow            )        \
    {                                         \
                                              \
        int argc = 0;                         \
        char** argv = &lpcmdline;             \
        if ( lpcmdline != NULL )              \
        {                                     \
            if ( *lpcmdline != '\0' )         \
            {                                 \
                argc = 1;                     \
            }                                 \
        }
                                              
    #define GUCEF_OSMAIN_END }

    #define GUCEF_OSSERVICEMAIN_BEGIN( serviceName )                                                   \
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

#else

    #define GUCEF_OSMAIN_BEGIN                \
                                              \
    int                                       \
    main( int argc, char* argv[] )            \
    {
    
    #define GUCEF_OSMAIN_END }

    #define GUCEF_OSSERVICEMAIN_BEGIN( serviceName )    GUCEF_OSMAIN_BEGIN
    #define GUCEF_OSSERVICEMAIN_END                     GUCEF_OSMAIN_END

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
