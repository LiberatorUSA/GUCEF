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

#include <string.h>

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

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
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CString CLogSvcClient::ClientVersion = "1.0";

const CORE::CEvent CLogSvcClient::ConnectingEvent = "GUCEF::LOGSERVICELIB::CLogSvcClient::ConnectingEvent";
const CORE::CEvent CLogSvcClient::ConnectedEvent = "GUCEF::LOGSERVICELIB::CLogSvcClient::ConnectedEvent";
const CORE::CEvent CLogSvcClient::DisconnectedEvent = "GUCEF::LOGSERVICELIB::CLogSvcClient::DisconnectedEvent";
const CORE::CEvent CLogSvcClient::SocketErrorEvent = "GUCEF::LOGSERVICELIB::CLogSvcClient::SocketErrorEvent";
const CORE::CEvent CLogSvcClient::ConnectionInitializedEvent = "GUCEF::LOGSERVICELIB::CLogSvcClient::ConnectionInitializedEvent";
const CORE::CEvent CLogSvcClient::IncompatibleWithServerEvent = "GUCEF::LOGSERVICELIB::CLogSvcClient::IncompatibleWithServerEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CLogSvcClient::RegisterEvents( void )
{GUCEF_TRACE;

    ConnectingEvent.Initialize();
    ConnectedEvent.Initialize();
    DisconnectedEvent.Initialize();
    SocketErrorEvent.Initialize();
    ConnectionInitializedEvent.Initialize();
    IncompatibleWithServerEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CLogSvcClient::CLogSvcClient( void )
    : CILogger()                       ,
      m_tcpClient( false )             ,
      m_appName( "Unknown" )           ,
      m_connectionInitialized( false )
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CLogSvcClient::CLogSvcClient( CORE::CPulseGenerator& pulseGenerator )
    : CILogger()                           ,
      m_tcpClient( pulseGenerator, false ) ,
      m_appName( "Unknown" )               ,
      m_connectionInitialized( false )
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CLogSvcClient::~CLogSvcClient()
{GUCEF_TRACE;

    Close();
}

/*-------------------------------------------------------------------------*/

bool
CLogSvcClient::ConnectTo( const CORE::CString& address ,
                          CORE::UInt16 port            )
{GUCEF_TRACE;

    m_connectionInitialized = false;
    return m_tcpClient.ConnectTo( address, port );
}

/*-------------------------------------------------------------------------*/

bool
CLogSvcClient::ConnectTo( const COMCORE::CHostAddress& address )
{GUCEF_TRACE;

    m_connectionInitialized = false;
    return m_tcpClient.ConnectTo( address );
}

/*-------------------------------------------------------------------------*/

bool
CLogSvcClient::Reconnect( void )
{GUCEF_TRACE;

    m_connectionInitialized = false;
    return m_tcpClient.Reconnect();
}

/*-------------------------------------------------------------------------*/

void
CLogSvcClient::Close( void )
{GUCEF_TRACE;

    m_connectionInitialized = false;
    m_tcpClient.Close();
}

/*-------------------------------------------------------------------------*/

void
CLogSvcClient::LogWithoutFormatting( const TLogMsgType logMsgType    ,
                                     const CORE::Int32 logLevel      ,
                                     const CORE::CString& logMessage ,
                                     const CORE::UInt32 threadId     )
{GUCEF_TRACE;

    Log( logMsgType ,
         logLevel   ,
         logMessage ,
         threadId   );
}

/*-------------------------------------------------------------------------*/

void
CLogSvcClient::Log( const TLogMsgType logMsgType    ,
                    const CORE::Int32 logLevel      ,
                    const CORE::CString& logMessage ,
                    const CORE::UInt32 threadId     )
{GUCEF_TRACE;

    CORE::Int16 logMsgTypeValue = logMsgType;
    CORE::Int8 msgHeader[ 16 ];  // 16 = 1+4+1+2+4+4
    CORE::UInt32 logMsgLength = logMessage.Length() + 16;


    msgHeader[ 0 ] = (CORE::Int8) LOGSVCMSGTYPE_DELIMITER;   // set delimiter for message: 1 byte
    memcpy( msgHeader+1, &logMsgLength, 4 );                 // set the total message length : 4 bytes
    msgHeader[ 5 ] = (CORE::Int8) LOGSVCMSGTYPE_LOGMSG;      // set TCP msg type: 1 byte
    memcpy( msgHeader+6, &logMsgTypeValue, 2 );              // set log msg type: 2 bytes
    memcpy( msgHeader+8, &logLevel, 4 );                     // set log level: 4 bytes
    memcpy( msgHeader+12, &threadId, 4 );                    // set thread id: 4 bytes

    if ( m_connectionInitialized )
    {
        // Send the logging msg header
        if ( m_tcpClient.Send( msgHeader, 16 ) )
        {
            // Now send the logging text
            m_tcpClient.Send( logMessage.C_String() ,
                              logMessage.Length()   );
        }
    }
    else
    {
        // The logging connection is not initialized yet
        // queue the msg until the connection is initialized

        TLogMessage queueItem;
        memcpy( queueItem.msgHeader, msgHeader, 16 );
        queueItem.logMsg = logMessage;

        m_logQueue.push_back( queueItem );
    }
}

/*-------------------------------------------------------------------------*/

void
CLogSvcClient::FlushLog( void )
{GUCEF_TRACE;

    if ( m_connectionInitialized )
    {
        char msg[ 6 ];
        msg[ 0 ] = LOGSVCMSGTYPE_DELIMITER;   // set delimiter for message: 1 byte
        CORE::UInt32 size = 6;
        memcpy( msg+1, &size, 4 );            // set the total message length : 4 bytes
        msg[ 5 ] = LOGSVCMSGTYPE_FLUSHLOG;    // set message type for message: 1 byte
        m_tcpClient.Send( &msg, 6 );
    }
}

/*-------------------------------------------------------------------------*/

void
CLogSvcClient::SetApplicationName( const CORE::CString& applicationName )
{GUCEF_TRACE;

    m_appName = applicationName;
}

/*-------------------------------------------------------------------------*/

const CORE::CString&
CLogSvcClient::GetApplicationName( void ) const
{GUCEF_TRACE;

    return m_appName;
}

/*-------------------------------------------------------------------------*/

void
CLogSvcClient::SendAllQueuedItems( void )
{GUCEF_TRACE;

    // We will copy the queue because the actual sending of the log
    // messages can cause more items to be queued
    TLogMsgStack logQueueCopy = m_logQueue;

    // Go through queue and send all items
    TLogMsgStack::reverse_iterator i = logQueueCopy.rbegin();
    while ( i != logQueueCopy.rend() )
    {
        TLogMessage& logMessage = (*i);

        // Send the logging msg header
        if ( m_tcpClient.Send( logMessage.msgHeader, 15 ) )
        {
            // Now send the logging text
            m_tcpClient.Send( logMessage.logMsg.C_String() ,
                              logMessage.logMsg.Length()   );
        }
        ++i;
    }

    // Clear the queue.
    // Any log messages that where added while sending will be dropped
    m_logQueue.clear();
}

/*-------------------------------------------------------------------------*/

void
CLogSvcClient::OnNotify( CORE::CNotifier* notifier    ,
                         const CORE::CEvent& eventid  ,
                         CORE::CICloneable* eventdata )
{GUCEF_TRACE;


    if ( &m_tcpClient == notifier )
    {
        if ( COMCORE::CTCPClientSocket::ConnectedEvent == eventid )
        {
            // Now that we connected we should send the inital info
            // introducing who we are to the logging service
            // First we gather the info we need
            static const CORE::UInt8 delimiter = LOGSVCMSGTYPE_DELIMITER;
            static const CORE::UInt8 msgType = LOGSVCMSGTYPE_CLIENTINFO;
            CORE::CString processName = "Unknown";   // @TODO
            CORE::CString processId = "Unknown";

            CORE::UInt32 totalMsgSize = processName.Length()   +
                                        processId.Length()     +
                                        m_appName.Length()     +
                                        ClientVersion.Length() +
                                        17;   // 1 + str lengths + 4 bytes for size per str

            CORE::CDynamicBuffer buffer( totalMsgSize, true );

            // Construct the message
            CORE::UInt32 strLength = 0;
            CORE::UInt32 offset = 0;
            buffer.CopyFrom( offset, 1, &delimiter );
            offset += 5; // <- we will offset an extra 4 bytes here to reserve room for the message length
            buffer.CopyFrom( offset, 1, &msgType );
            offset += 1;
            strLength = ClientVersion.Length();
            buffer.CopyFrom( offset, 4, &strLength );
            offset += 4;
            buffer.CopyFrom( offset, strLength, ClientVersion.C_String() );
            offset += strLength;
            strLength = m_appName.Length();
            buffer.CopyFrom( offset, 4, &strLength );
            offset += 4;
            buffer.CopyFrom( offset, strLength, m_appName.C_String() );
            offset += strLength;
            strLength = processName.Length();
            buffer.CopyFrom( offset, 4, &strLength );
            offset += 4;
            buffer.CopyFrom( offset, strLength, processName.C_String() );
            offset += strLength;
            strLength = processId.Length();
            buffer.CopyFrom( offset, 4, &strLength );
            offset += 4;
            buffer.CopyFrom( offset, strLength, processId.C_String() );

            // Now that we contructed the whole message write the message length itself into
            // its reserved slot
            CORE::UInt32 msgSize = buffer.GetDataSize();
            buffer.CopyFrom( 1, 4, &msgSize );

            // Now actually send the message
            m_tcpClient.Send( buffer.GetBufferPtr(), totalMsgSize );
        }
    }
    else
    if ( COMCORE::CTCPClientSocket::DisconnectedEvent == eventid )
    {
        m_connectionInitialized = false;
    }
    else
    if ( COMCORE::CTCPClientSocket::DataRecievedEvent == eventid )
    {
        // Get access to the data from the event
        COMCORE::CTCPClientSocket::TDataRecievedEventData* eData = static_cast< COMCORE::CTCPClientSocket::TDataRecievedEventData* >( eventdata );
        const CORE::CDynamicBuffer& buffer = eData->GetData();

        // The only reponse we expect back from the server is a 1 byte status code
        // thus we just check 1 byte for that code
        CORE::UInt8 code = buffer.AsConstType< CORE::UInt8 >( 0 );
        if ( LOGSVCMSGTYPE_INITIALIZED == code )
        {
            SendAllQueuedItems();
            NotifyObservers( ConnectionInitializedEvent );
            m_connectionInitialized = true;
        }
        else
        if ( LOGSVCMSGTYPE_INCOMPATIBLE == code )
        {
            NotifyObservers( IncompatibleWithServerEvent );
            m_tcpClient.Close();
        }
    }
    else
    if ( COMCORE::CTCPClientSocket::ConnectingEvent == eventid )
    {
        NotifyObservers( ConnectingEvent );
    }
    else
    if ( COMCORE::CTCPClientSocket::SocketErrorEvent == eventid )
    {
        NotifyObservers( SocketErrorEvent );
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace LOGSERVICELIB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
