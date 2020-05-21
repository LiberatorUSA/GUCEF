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

#include <assert.h>

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_CIOACCESS_H
#include "CIOAccess.h"
#define GUCEF_CORE_CIOACCESS_H
#endif /* GUCEF_CORE_CIOACCESS_H ? */

#ifndef GUCEF_CORE_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_ESSENTIALS_H
#endif /* GUCEF_CORE_ESSENTIALS_H ? */

#ifdef GUCEF_MSWIN_BUILD
#include <windows.h>
#include <wincon.h>          /* for COORD */
#include <conio.h>           /* need conio.h under WIN32 for clrscr() */
#endif /* GUCEF_MSWIN_BUILD ? */

#include "CMSWinConsoleLogger.h"

#ifdef GUCEF_MSWIN_BUILD
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

CMSWinConsoleLogger::CMSWinConsoleLogger( void )
    : CIConsoleLogger()                          ,
      m_minimalLogLevel( LOGLEVEL_BELOW_NORMAL ) ,
      m_consoleFptr( NULL )                      ,
      m_formatForUiPurpose( false )
{GUCEF_TRACE;

    AllocConsole();

    /* reopen stout handle as console window output */
    m_consoleFptr = freopen( "CONOUT$", "wb", stdout );
}

/*-------------------------------------------------------------------------*/

CMSWinConsoleLogger::~CMSWinConsoleLogger()
{GUCEF_TRACE;

    CCoreGlobal::Instance()->GetLogManager().RemoveLogger( this );
    FlushLog();

    FreeConsole();
    fclose( m_consoleFptr );
}

/*-------------------------------------------------------------------------*/

void
CMSWinConsoleLogger::SetFormatAsConsoleUI( bool formatForUiPurpose )
{GUCEF_TRACE;
    
    m_formatForUiPurpose = formatForUiPurpose;
}

/*-------------------------------------------------------------------------*/
    
bool
CMSWinConsoleLogger::GetFormatAsConsoleUI( void ) const
{GUCEF_TRACE;

    return m_formatForUiPurpose;
}

/*-------------------------------------------------------------------------*/

void
CMSWinConsoleLogger::Log( const TLogMsgType logMsgType ,
                          const Int32 logLevel         ,
                          const CString& logMessage    ,
                          const UInt32 threadId        )
{GUCEF_TRACE;

    if ( !m_formatForUiPurpose )
    {
        if ( logLevel >= m_minimalLogLevel )
        {
            CString actualLogMsg( FormatStdLogMessage( logMsgType ,
                                                       logLevel   ,
                                                       logMessage ,
                                                       threadId   ) + "\n" );

            fwrite( actualLogMsg.C_String(), actualLogMsg.Length(), 1, stdout );
            //fwrite( "\n", 1, 1, stdout );
        }
    }
    else
    {
        if ( logMsgType == CORE::CLogManager::LOG_CONSOLE )
        {
            fwrite( logMessage.C_String(), logMessage.Length(), 1, m_consoleFptr );
            //fwrite( "\n", 1, 1, m_consoleFptr );
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CMSWinConsoleLogger::LogWithoutFormatting( const TLogMsgType logMsgType ,
                                           const Int32 logLevel         ,
                                           const CString& logMessage    ,
                                           const UInt32 threadId        )
{GUCEF_TRACE;

    if ( !m_formatForUiPurpose )
    {
        if ( logLevel >= m_minimalLogLevel || m_formatForUiPurpose )
        {
            CString actualLogMsg( logMessage + "\n" );
            fprintf( m_consoleFptr, actualLogMsg.C_String() );
        }
    }
    else
    {
        if ( logMsgType == CORE::CLogManager::LOG_CONSOLE )
        {
            CString actualLogMsg( logMessage + "\n" );
            fprintf( m_consoleFptr, actualLogMsg.C_String() );
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CMSWinConsoleLogger::FlushLog( void )
{GUCEF_TRACE;

    fflush( m_consoleFptr );
}

/*-------------------------------------------------------------------------*/

void
CMSWinConsoleLogger::SetMinimalLogLevel( const Int32 minimalLogLevel )
{GUCEF_TRACE;

    m_minimalLogLevel = minimalLogLevel;
}

/*-------------------------------------------------------------------------*/

Int32
CMSWinConsoleLogger::GetMinimalLogLevel( void ) const
{GUCEF_TRACE;

    return m_minimalLogLevel;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
#endif /* GUCEF_MSWIN_BUILD ? */
