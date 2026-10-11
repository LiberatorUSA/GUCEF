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

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#include "gucefCORE_CPlatformNativeConsoleLogger.h"

#if GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN

#ifndef GUCEF_CORE_CMSWINCONSOLELOGGER_H
#include "CMSWinConsoleLogger.h"
#define GUCEF_CORE_CMSWINCONSOLELOGGER_H
#endif /* GUCEF_CORE_CMSWINCONSOLELOGGER_H ? */

#elif GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX

#include <stdlib.h>
#include <string.h>

#ifndef GUCEF_CORE_CXTERMCONSOLELOGGER_H
#include "CXTermConsoleLogger.h"
#define GUCEF_CORE_CXTERMCONSOLELOGGER_H
#endif /* GUCEF_CORE_CXTERMCONSOLELOGGER_H ? */

#ifndef GUCEF_CORE_CTERMINALCONSOLELOGGER_H
#include "gucefCORE_CTerminalConsoleLogger.h"
#define GUCEF_CORE_CTERMINALCONSOLELOGGER_H
#endif /* GUCEF_CORE_CTERMINALCONSOLELOGGER_H ? */

#ifndef GUCEF_CORE_CDESKTOPTERMINALCONSOLELOGGER_H
#include "gucefCORE_CDesktopTerminalConsoleLogger.h"
#define GUCEF_CORE_CDESKTOPTERMINALCONSOLELOGGER_H
#endif /* GUCEF_CORE_CDESKTOPTERMINALCONSOLELOGGER_H ? */

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
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CPlatformNativeConsoleLogger::CPlatformNativeConsoleLogger( void )
    : m_nativeConsoleLogger( GUCEF_NULL )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPlatformNativeConsoleLogger::~CPlatformNativeConsoleLogger()
{GUCEF_TRACE;

    // The logger may have been registered with the log manager: it must not be invoked after it is deleted
    if ( GUCEF_NULL != m_nativeConsoleLogger )
        CCoreGlobal::Instance()->GetLogManager().RemoveLogger( m_nativeConsoleLogger );
    GUCEF_DELETE m_nativeConsoleLogger;
    m_nativeConsoleLogger = NULL;
}

/*-------------------------------------------------------------------------*/

#if GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX

/**
 *  Linux has no console that every application gets, so what is best depends on how the application was started:
 *  from a terminal the log goes to that terminal, from a desktop session a terminal window is opened for it
 *  and in a headless environment there is no console to log to at all.
 *  The environment variable GUCEF_CONSOLE_LOGGER can force a choice: terminal, desktop, xterm or none.
 */
static CIConsoleLogger*
CreateLinuxConsoleLogger( void )
{GUCEF_TRACE;

    const char* forced = getenv( "GUCEF_CONSOLE_LOGGER" );
    bool useTerminal = false;
    bool useDesktop = false;
    bool useXTerm = false;
    if ( GUCEF_NULL != forced && '\0' != forced[ 0 ] && 0 != strcmp( forced, "auto" ) )
    {
        if ( 0 == strcmp( forced, "terminal" ) )
            useTerminal = true;
        else
        if ( 0 == strcmp( forced, "desktop" ) )
            useDesktop = true;
        else
        if ( 0 == strcmp( forced, "xterm" ) )
            useXTerm = true;
        else
            return GUCEF_NULL;      // "none" or anything we do not know
    }
    else
    {
        if ( CTerminalConsoleLogger::IsRunningInTerminal() )
            useTerminal = true;
        else
        if ( CDesktopTerminalConsoleLogger::IsDesktopSessionAvailable() )
            useDesktop = true;
        else
            return GUCEF_NULL;      // headless, file and other loggers are not affected
    }

    if ( useTerminal )
    {
        if ( !CTerminalConsoleLogger::IsRunningInTerminal() )
            return GUCEF_NULL;
        return GUCEF_NEW CTerminalConsoleLogger();
    }
    if ( useDesktop )
    {
        CDesktopTerminalConsoleLogger* logger = GUCEF_NEW CDesktopTerminalConsoleLogger();
        if ( !logger->IsActive() )
        {
            // no terminal emulator to be had or it is switched off
            GUCEF_DELETE logger;
            return GUCEF_NULL;
        }
        return logger;
    }
    if ( useXTerm )
        return GUCEF_NEW CXTermConsoleLogger();
    return GUCEF_NULL;
}

#endif /* GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ? */

/*-------------------------------------------------------------------------*/

CIConsoleLogger*
CPlatformNativeConsoleLogger::GetLogger( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL == m_nativeConsoleLogger )
    {
        #if GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN
        m_nativeConsoleLogger = GUCEF_NEW CMSWinConsoleLogger();
        #elif GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX
        m_nativeConsoleLogger = CreateLinuxConsoleLogger();
        #endif
    }
    return m_nativeConsoleLogger;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
