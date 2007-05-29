/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2007.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL, 
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER 
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF 
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT 
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
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