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
    : CILogger()
{GUCEF_TRACE;
       
    AllocConsole();

    /* reopen stdin handle as console window input */
    freopen( "CONIN$", "rb", stdin );
    
    /* reopen stout handle as console window output */
    freopen( "CONOUT$", "wb", stdout );
    
    /* reopen stderr handle as console window output */
    freopen( "CONOUT$", "wb", stderr );
}

/*-------------------------------------------------------------------------*/

CMSWinConsoleLogger::~CMSWinConsoleLogger()
{GUCEF_TRACE;

    FreeConsole();
}

/*-------------------------------------------------------------------------*/

CString
CMSWinConsoleLogger::GetLogMsgTypeString( const TLogMsgType logMsgType ) const
{GUCEF_TRACE;

    switch ( logMsgType )
    {
        case CLogManager::LOG_ERROR : return "ERROR";
        case CLogManager::LOG_STANDARD : return "STANDARD";
        case CLogManager::LOG_USER : return "USER";
        case CLogManager::LOG_SYSTEM : return "SYSTEM";
        case CLogManager::LOG_DEV : return "DEV";
        case CLogManager::LOG_DEBUG : return "DEBUG";
        case CLogManager::LOG_SERVICE : return "SERVICE";
        case CLogManager::LOG_PROTECTED : return "PROTECTED";
        case CLogManager::LOG_CALLSTACK : return "CALLSTACK";
        default : return "";
    }
}

/*-------------------------------------------------------------------------*/

void
CMSWinConsoleLogger::Log( const TLogMsgType logMsgType ,
                          const Int32 logLevel         ,
                          const CString& logMessage    ) const
{GUCEF_TRACE;

    CString actualLogMsg( "[" + GetLogMsgTypeString( logMsgType ) + "] [LVL " + Int32ToString( logLevel ) + "] " + logMessage + "\n" );
    fprintf( stdout, actualLogMsg.C_String() ); 
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