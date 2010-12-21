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

#ifndef GUCEF_CORE_CIOACCESS_H
#include "CIOAccess.h"
#define GUCEF_CORE_CIOACCESS_H
#endif /* GUCEF_CORE_CIOACCESS_H ? */

#include "GucefLogServiceLib_CLogSvcServerFileLogger.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace LOGSERVICELIB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CLogSvcServerFileLogger::CLogSvcServerFileLogger( void )
    : CILogSvcServerLogger()                           ,
      m_output( NULL )                                 ,
      m_minimalLogLevel( CORE::LOGLEVEL_BELOW_NORMAL ) ,
      m_logAppName( true )                             ,
      m_logProcessName( true )                         ,
      m_logProcessId( true )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CLogSvcServerFileLogger::CLogSvcServerFileLogger( CORE::CIOAccess& output )
    : CILogSvcServerLogger()                           ,
      m_output( &output )                              ,
      m_minimalLogLevel( CORE::LOGLEVEL_BELOW_NORMAL ) ,
      m_logAppName( true )                             ,
      m_logProcessName( true )                         ,
      m_logProcessId( true )
{GUCEF_TRACE;

    assert( &output != NULL );
    assert( m_output != NULL );
}

/*-------------------------------------------------------------------------*/

CLogSvcServerFileLogger::~CLogSvcServerFileLogger()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CLogSvcServerFileLogger::Log( const CLogSvcServer::TClientInfo& clientInfo ,
                              const TLogMsgType logMsgType                 ,
                              const CORE::Int32 logLevel                   ,
                              const CORE::CString& logMessage              ,
                              const CORE::UInt32 threadId                  )
{GUCEF_TRACE;

    if ( m_output != NULL )
    {
        if ( logLevel >= m_minimalLogLevel )
        {
            CORE::CString actualLogMsg = FormatStdLogMessage( m_logAppName     ,
                                                              m_logProcessName ,
                                                              m_logProcessId   ,
                                                              clientInfo       ,
                                                              logMsgType       ,
                                                              logLevel         ,
                                                              logMessage       ,
                                                              threadId         );

            m_output->Write( actualLogMsg.C_String() ,
                             actualLogMsg.Length()   ,
                             1                       );
        }
    }
}


/*-------------------------------------------------------------------------*/

void
CLogSvcServerFileLogger::SetWhetherProcessNameIsLogged( bool logProcessName )
{GUCEF_TRACE;

    m_logProcessName = logProcessName;
}

/*-------------------------------------------------------------------------*/
    
bool
CLogSvcServerFileLogger::GetWhetherProcessNameIsLogged( void ) const
{GUCEF_TRACE;

    return m_logProcessName;
}


/*-------------------------------------------------------------------------*/

void
CLogSvcServerFileLogger::SetWhetherAppNameIsLogged( bool logAppName )
{GUCEF_TRACE;

    m_logAppName = logAppName;
}

/*-------------------------------------------------------------------------*/
    
bool
CLogSvcServerFileLogger::GetWhetherAppNameIsLogged( void ) const
{GUCEF_TRACE;

    return m_logAppName;
}

/*-------------------------------------------------------------------------*/

void
CLogSvcServerFileLogger::SetWhetherProcessIdIsLogged( bool logProcessId )
{GUCEF_TRACE;

    m_logProcessId = logProcessId;
}

/*-------------------------------------------------------------------------*/
    
bool
CLogSvcServerFileLogger::GetWhetherProcessIdIsLogged( void ) const
{GUCEF_TRACE;

    return m_logProcessId;
}

/*-------------------------------------------------------------------------*/
                      
void
CLogSvcServerFileLogger::FlushLog( void )
{GUCEF_TRACE;

    if ( m_output != NULL )
    {
        m_output->Flush();
    }
}

/*-------------------------------------------------------------------------*/

void
CLogSvcServerFileLogger::SetOutput( CORE::CIOAccess& output )
{GUCEF_TRACE;

    assert( &output != NULL );
    m_output = &output;
    m_output->Open();
}

/*-------------------------------------------------------------------------*/

void
CLogSvcServerFileLogger::SetMinimalLogLevel( const CORE::Int32 minimalLogLevel )
{GUCEF_TRACE;

    m_minimalLogLevel = minimalLogLevel;
}

/*-------------------------------------------------------------------------*/

CORE::Int32
CLogSvcServerFileLogger::GetMinimalLogLevel( void ) const
{GUCEF_TRACE;

    return m_minimalLogLevel;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace LOGSERVICELIB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/