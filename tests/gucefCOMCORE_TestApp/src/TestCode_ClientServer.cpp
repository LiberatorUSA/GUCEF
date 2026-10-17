/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
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

#include <string>

#ifndef GUCEF_CORE_H
#include "gucefCORE.h"
#define GUCEF_CORE_H
#endif /* GUCEF_CORE_H ? */

#ifndef GUCEF_COMCORE_H
#include "gucefCOMCORE.h"
#define GUCEF_COMCORE_H
#endif /* GUCEF_COMCORE_H ? */

#ifndef GUCEF_TEST_FRAMEWORK_H
#include "gucef_test_framework.h"
#define GUCEF_TEST_FRAMEWORK_H
#endif /* GUCEF_TEST_FRAMEWORK_H ? */

#include "TestCode_Pump.h"

#include "TestCode_ClientServer.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

#define ERRORHERE       GUCEF_TESTFW_ERRORHERE
#define ASSERT_TRUE(t)  GUCEF_TESTFW_ASSERT_TRUE(t)
#define ASSERT_FALSE(t) GUCEF_TESTFW_ASSERT_FALSE(t)

using namespace GUCEF;
using namespace GUCEF::COMCORE;

static const UInt16 TestServerPort = 47610;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  A TCP server socket and a TCP client socket which talk to each other, and the record of what they did.
 *  The server answers every message it gets with the reply it was given.
 */
class CTestTCPClientServer : public CORE::CObserver
{
    public:

    CTestTCPClientServer( const std::string& reply )
        : CORE::CObserver()
        , m_serverSock( false )
        , m_clientSock( false )
        , m_serverConnection( GUCEF_NULL )
        , m_reply( reply )
        , m_clientConnected( false )
        , m_clientDisconnected( false )
        , m_serverSawClient( false )
        , m_serverSawDisconnect( false )
        , m_serverReceived()
        , m_clientReceived()
    {GUCEF_TRACE;

        SubscribeTo( &m_serverSock );
        SubscribeTo( &m_clientSock );
    }

    virtual ~CTestTCPClientServer()
    {GUCEF_TRACE;

        UnsubscribeAllFromObserver();
    }

    virtual const CORE::CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE
    {
        static const CORE::CString classTypeName = "CTestTCPClientServer";
        return classTypeName;
    }

    bool Start( UInt16 port )
    {GUCEF_TRACE;

        return m_serverSock.ListenOnPort( port ) && m_clientSock.ConnectTo( "127.0.0.1", port );
    }

    CTCPClientSocket& Client( void ) { return m_clientSock; }
    CTCPServerSocket& Server( void ) { return m_serverSock; }

    bool m_clientConnected;
    bool m_clientDisconnected;
    bool m_serverSawClient;
    bool m_serverSawDisconnect;
    std::string m_serverReceived;
    std::string m_clientReceived;

    protected:

    virtual void OnNotify( CORE::CNotifier* notifier           ,
                           const CORE::CEvent& eventid         ,
                           CORE::CICloneable* eventdata = GUCEF_NULL ) GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;

        if ( notifier == &m_serverSock )
        {
            if ( CTCPServerSocket::ClientConnectedEvent == eventid )
            {
                CTCPServerSocket::TClientConnectedEventData* data = static_cast< CTCPServerSocket::TClientConnectedEventData* >( eventdata );
                m_serverConnection = data->GetData().connection;
                SubscribeTo( m_serverConnection );
                m_serverSawClient = true;
            }
            else
            if ( CTCPServerSocket::ClientDisconnectedEvent == eventid )
            {
                m_serverConnection = GUCEF_NULL;
                m_serverSawDisconnect = true;
            }
        }
        else
        if ( notifier == &m_clientSock )
        {
            if ( CTCPClientSocket::ConnectedEvent == eventid )
            {
                m_clientConnected = true;
            }
            else
            if ( CTCPClientSocket::DisconnectedEvent == eventid )
            {
                m_clientDisconnected = true;
            }
            else
            if ( CTCPClientSocket::DataRecievedEvent == eventid )
            {
                CTCPClientSocket::TDataRecievedEventData* data = static_cast< CTCPClientSocket::TDataRecievedEventData* >( eventdata );
                m_clientReceived.append( data->GetData().AsConstTypePtr< char >(), data->GetData().GetDataSize() );
            }
        }
        else
        if ( CTCPConnection::DataRecievedEvent == eventid && notifier == m_serverConnection )
        {
            CTCPConnection::TDataRecievedEventData* data = static_cast< CTCPConnection::TDataRecievedEventData* >( eventdata );
            m_serverReceived.append( data->GetData().AsConstTypePtr< char >(), data->GetData().GetDataSize() );
            m_serverConnection->Send( m_reply.data(), (UInt32) m_reply.size() );
        }
    }

    private:

    CTCPServerSocket m_serverSock;
    CTCPClientSocket m_clientSock;
    CTCPServerConnection* m_serverConnection;
    std::string m_reply;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
PerformClientServerTests( void )
{GUCEF_TRACE;

    GUCEF_TESTFW_SUITE_SCOPE( "ClientServer" );

    GUCEF_TESTFW_TESTCASE( "Test: A TCP client connects to a TCP server, they exchange data and the client disconnects" )
        try
        {
            CTestTCPClientServer test( "pong" );
            ASSERT_TRUE( test.Start( TestServerPort ) );

            ASSERT_TRUE( PumpUntil( [&test]() { return test.m_clientConnected && test.m_serverSawClient; }, 5000 ) );

            ASSERT_TRUE( test.Client().Send( "ping", 4 ) );
            ASSERT_TRUE( PumpUntil( [&test]() { return test.m_serverReceived == "ping"; }, 5000 ) );
            ASSERT_TRUE( PumpUntil( [&test]() { return test.m_clientReceived == "pong"; }, 5000 ) );

            test.Client().Close();
            ASSERT_TRUE( PumpUntil( [&test]() { return test.m_serverSawDisconnect; }, 5000 ) );
        }
        catch ( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END

    GUCEF_TESTFW_TESTCASE( "Test: A TCP client reports an error when nothing is listening" )
        try
        {
            CTCPClientSocket client( false );
            bool errorOrClosed = false;
            struct CErrorObserver : public CORE::CObserver
            {
                bool* flag;
                CErrorObserver( bool* f ) : flag( f ) {}
                virtual const CORE::CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE { static const CORE::CString n = "CErrorObserver"; return n; }
                virtual void OnNotify( CORE::CNotifier*, const CORE::CEvent& eventid, CORE::CICloneable* ) GUCEF_VIRTUAL_OVERRIDE
                {
                    if ( CTCPClientSocket::SocketErrorEvent == eventid || CTCPClientSocket::DisconnectedEvent == eventid )
                        *flag = true;
                }
            } observer( &errorOrClosed );
            observer.SubscribeTo( &client );

            // the port is not used by any test
            client.ConnectTo( "127.0.0.1", TestServerPort + 50 );
            ASSERT_TRUE( PumpUntil( [&errorOrClosed]() { return errorOrClosed; }, 5000 ) );
        }
        catch ( ... )
        {
            ERRORHERE;
        }
    GUCEF_TESTFW_TESTCASE_END
}

/*-------------------------------------------------------------------------*/
