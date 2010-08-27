/*
 *  GucefLogServiceLib: Library containing the main logic for the Logging service
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
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

#ifndef GUCEF_LOGSERVICELIB_PROTOCOL_H
#include "GucefLogServiceLib_protocol.h"
#define GUCEF_LOGSERVICELIB_PROTOCOL_H
#endif /* GUCEF_LOGSERVICELIB_PROTOCOL_H ? */

#include "GucefLogServiceLib_CLogSvcClient.h"

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

CLogSvcClient::CLogSvcClient( void )
    : CILogger()    ,
      m_tcpClient()
{GUCEF_TRACE;

}
    
/*-------------------------------------------------------------------------*/

CLogSvcClient::CLogSvcClient( CORE::CPulseGenerator& pulseGenerator )
    : CILogger()                    ,
      m_tcpClient( pulseGenerator )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CLogSvcClient::~CLogSvcClient()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CLogSvcClient::ConnectTo( const CORE::CString& address , 
                          CORE::UInt16 port            )
{GUCEF_TRACE;

    return m_tcpClient.ConnectTo( address, port );
}

/*-------------------------------------------------------------------------*/

bool
CLogSvcClient::ConnectTo( const CORE::CHostAddress& address )
{GUCEF_TRACE;

    return m_tcpClient.ConnectTo( address );
}

/*-------------------------------------------------------------------------*/

bool
CLogSvcClient::Reconnect( void )
{GUCEF_TRACE;

    m_tcpClient.Reconnect();
}

/*-------------------------------------------------------------------------*/
    
void
CLogSvcClient::Close( void )
{GUCEF_TRACE;

    m_tcpClient.Close();
}

/*-------------------------------------------------------------------------*/
    
void
CLogSvcClient::Log( const TLogMsgType logMsgType    ,
                    const CORE::Int32 logLevel      ,
                    const CORE::CString& logMessage ,
                    const CORE::UInt32 threadId     )
{GUCEF_TRACE;

    CORE::Int16 logMsgTypeValue = logMsgType;
    CORE::Int8 msgHeader[ 1+2+4+4+4 ];
    
    msgHeader[ 0 ] = LOGSVCMSGTYPE_LOGMSG;          // set TCP msg type: 1 byte
    memcpy( msgHeader+1, &logMsgTypeValue, 2 );     // set log msg type: 2 bytes
    memcpy( msgHeader+3, &logLevel, 4 );            // set log level: 4 bytes
    memcpy( msgHeader+7, &threadId, 4 );            // set thread id: 4 bytes
    memcpy( msgHeader+11, logMessage.Length(), 4 ); // set length of log msg: 4 bytes
    
    // Send the logging msg header
    if ( m_tcpClient.Send( msgHeader, 15 ) )
    {
        // Now send the logging text
        m_tcpClient.Send( logMessage.C_String() , 
                          logMessage.Length()   );
    }
}

/*-------------------------------------------------------------------------*/

void
CLogSvcClient::FlushLog( void )
{GUCEF_TRACE;

    CORE::Int8 msg = LOGSVCMSGTYPE_FLUSHLOG;
    m_tcpClient.Send( msg, 1 );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace LOGSERVICELIB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
