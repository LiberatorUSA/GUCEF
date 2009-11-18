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

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

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
    : CILogger()                           ,
      m_minimalLogLevel( LOGLEVEL_NORMAL )
{GUCEF_TRACE;
       
    AllocConsole();
    
    /* reopen stout handle as console window output */
    freopen( "CONOUT$", "wb", stdout );
}

/*-------------------------------------------------------------------------*/

CMSWinConsoleLogger::~CMSWinConsoleLogger()
{GUCEF_TRACE;

    FreeConsole();
}

/*-------------------------------------------------------------------------*/

void
CMSWinConsoleLogger::Log( const TLogMsgType logMsgType ,
                          const Int32 logLevel         ,
                          const CString& logMessage    )
{GUCEF_TRACE;

    if ( logLevel >= m_minimalLogLevel )
    {    
        CString actualLogMsg( "[" + CLogManager::GetLogMsgTypeString( logMsgType ) + "] [LVL " + LogLevelToString( logLevel ) + "] " + logMessage + "\n" );
        fprintf( stdout, actualLogMsg.C_String() );
    } 
}

/*-------------------------------------------------------------------------*/

void
CMSWinConsoleLogger::FlushLog( void )
{GUCEF_TRACE;

    fflush( stdout );
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