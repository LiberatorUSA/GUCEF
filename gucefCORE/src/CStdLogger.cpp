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
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CStdLogger::CStdLogger( CIOAccess& output )
    : CILogger()          ,
      m_output( &output )
{GUCEF_TRACE;

    assert( &m_output != NULL );
    assert( m_output != NULL );
}

/*-------------------------------------------------------------------------*/

CStdLogger::~CStdLogger()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CStdLogger::SetOutput( CIOAccess& output )
{GUCEF_TRACE;

    assert( &m_output != NULL );
    m_output = &output;
    m_output->Open();
}

/*-------------------------------------------------------------------------*/

CString
CStdLogger::GetLogMsgTypeString( const TLogMsgType logMsgType ) const
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
CStdLogger::Log( const TLogMsgType logMsgType ,
                 const Int32 logLevel         ,
                 const CString& logMessage    )
{GUCEF_TRACE;

    if ( m_output != NULL )
    {
        CString actualLogMsg( "[" + GetLogMsgTypeString( logMsgType ) + "] [LVL " + Int32ToString( logLevel ) + "] " + logMessage + "\n" );
        m_output->Write( actualLogMsg.C_String() ,
                         actualLogMsg.Length()   ,
                         1                       );
    }
}

/*-------------------------------------------------------------------------*/

void
CStdLogger::FlushLog( void )
{GUCEF_TRACE;
    
    if ( m_output != NULL )
    {
        m_output->Flush();
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