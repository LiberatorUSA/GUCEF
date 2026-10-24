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

#include <stdlib.h>
#include <string.h>

#include "gucefCORE_CPlatformNativeConsoleLogger.h"

#if GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN

#ifndef GUCEF_CORE_CMSWINCONSOLELOGGER_H
#include "CMSWinConsoleLogger.h"
#define GUCEF_CORE_CMSWINCONSOLELOGGER_H
#endif /* GUCEF_CORE_CMSWINCONSOLELOGGER_H ? */

#elif GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX

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

CPlatformNativeConsoleLogger::CPlatformNativeConsoleLogger( bool ownWindow )
    : m_nativeConsoleLogger( GUCEF_NULL )
    , m_ownWindow( ownWindow )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPlatformNativeConsoleLogger::EConsoleKind
CPlatformNativeConsoleLogger::ChooseConsoleKind( const char* forced, bool runningInTerminal, bool desktopSessionAvailable, bool ownWindow )
{GUCEF_TRACE;

    if ( GUCEF_NULL != forced && '\0' != forced[ 0 ] && 0 != strcmp( forced, "auto" ) )
    {
        if ( 0 == strcmp( forced, "terminal" ) )
            return runningInTerminal ? CONSOLEKIND_TERMINAL : CONSOLEKIND_NONE;
        if ( 0 == strcmp( forced, "desktop" ) )
            return CONSOLEKIND_DESKTOP_TERMINAL;
        if ( 0 == strcmp( forced, "xterm" ) )
            return CONSOLEKIND_XTERM;
        return CONSOLEKIND_NONE;        // "none" or anything we do not know
    }
    if ( ownWindow )
    {
        // a window of its own; the terminal only when the desktop cannot give one
        if ( desktopSessionAvailable )
            return CONSOLEKIND_DESKTOP_TERMINAL;
        return runningInTerminal ? CONSOLEKIND_TERMINAL : CONSOLEKIND_NONE;
    }
    if ( runningInTerminal )
        return CONSOLEKIND_TERMINAL;
    if ( desktopSessionAvailable )
        return CONSOLEKIND_DESKTOP_TERMINAL;
    return CONSOLEKIND_NONE;            // headless, file and other loggers are not affected
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
 *  Linux has no console that every application gets, so what is best depends on how the application was started and on
 *  what it asks for: the terminal it was started from, or a terminal window of its own in the desktop session; in a
 *  headless environment there is no console to log to at all. See ChooseConsoleKind()
 */
static CIConsoleLogger*
CreateLinuxConsoleLogger( bool ownWindow )
{GUCEF_TRACE;

    const bool runningInTerminal = CTerminalConsoleLogger::IsRunningInTerminal();
    CPlatformNativeConsoleLogger::EConsoleKind kind =
        CPlatformNativeConsoleLogger::ChooseConsoleKind( getenv( "GUCEF_CONSOLE_LOGGER" ), runningInTerminal, CDesktopTerminalConsoleLogger::IsDesktopSessionAvailable(), ownWindow );

    if ( CPlatformNativeConsoleLogger::CONSOLEKIND_DESKTOP_TERMINAL == kind )
    {
        CDesktopTerminalConsoleLogger* logger = GUCEF_NEW CDesktopTerminalConsoleLogger();
        if ( logger->IsActive() )
            return logger;

        // no terminal emulator to be had or it is switched off: the terminal it was started from, if any
        GUCEF_DELETE logger;
        kind = runningInTerminal ? CPlatformNativeConsoleLogger::CONSOLEKIND_TERMINAL : CPlatformNativeConsoleLogger::CONSOLEKIND_NONE;
    }
    if ( CPlatformNativeConsoleLogger::CONSOLEKIND_TERMINAL == kind )
        return GUCEF_NEW CTerminalConsoleLogger();
    if ( CPlatformNativeConsoleLogger::CONSOLEKIND_XTERM == kind )
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
        m_nativeConsoleLogger = GUCEF_NEW CMSWinConsoleLogger( m_ownWindow );
        #elif GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX
        m_nativeConsoleLogger = CreateLinuxConsoleLogger( m_ownWindow );
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
