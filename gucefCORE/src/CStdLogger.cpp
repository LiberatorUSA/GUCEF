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

#include "CStdLogger.h"

#ifndef GUCEF_CORE_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_ESSENTIALS_H
#endif /* GUCEF_CORE_ESSENTIALS_H ? */

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

CStdLogger::CStdLogger( void )
    : CILogger()       ,
      m_output( NULL )
{TRACE;

}

/*-------------------------------------------------------------------------*/

CStdLogger::CStdLogger( CIOAccess& output )
    : CILogger()          ,
      m_output( &output )
{TRACE;

    assert( &m_output != NULL );
    assert( m_output != NULL );
}

/*-------------------------------------------------------------------------*/

CStdLogger::~CStdLogger()
{TRACE;

}

/*-------------------------------------------------------------------------*/

void
CStdLogger::SetOutput( CIOAccess& output )
{TRACE;

    assert( &m_output != NULL );
    m_output = &output;
    m_output->Open();
}

/*-------------------------------------------------------------------------*/

CString
CStdLogger::GetLogMsgTypeString( const TLogMsgType logMsgType ) const
{TRACE;

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
CStdLogger::Log( const TLogMsgType logMsgType ,
                 const Int32 logLevel         ,
                 const CString& logMessage    ) const
{TRACE;

    if ( m_output != NULL )
    {
        CString actualLogMsg( "[" + GetLogMsgTypeString( logMsgType ) + "] [LVL " + Int32ToString( logLevel ) + "] " + logMessage + "\n" );
        m_output->Write( actualLogMsg.C_String() ,
                         actualLogMsg.Length()   ,
                         1                       );
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/