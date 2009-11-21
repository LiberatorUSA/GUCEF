/*
 *  ServerPortExtender: Generic networking tool for connecting clients
 *  to a server that is behind a NAT firewall.
 *
 *  Copyright (C) 2002 - 2009.  Dinand Vanvelzen
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

#ifndef CSERVERPORTEXTENDER_H
#include "CServerPortExtender.h"
#define CSERVERPORTEXTENDER_H
#endif /* CSERVERPORTEXTENDER_H ? */

#ifndef CSERVERPORTEXTENDERCLIENT_H
#include "CServerPortExtenderClient.h"
#define CSERVERPORTEXTENDERCLIENT_H
#endif /* CSERVERPORTEXTENDERCLIENT_H ? */

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#ifndef GUCEF_CORE_CVALUELIST_H
#include "CValueList.h"
#define GUCEF_CORE_CVALUELIST_H
#endif /* GUCEF_CORE_CVALUELIST_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifdef GUCEF_MSWIN_BUILD
#include <windows.h>
#endif /* GUCEF_MSWIN_BUILD ? */

#include "callstack.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

class SpeTestController : CORE::CObserver
{
    public:
    
    //GUCEF_INLINED_MSGEXCEPTION( 
    
    SpeTestController()
        : CORE::CObserver()          ,
          m_client( NULL )           ,
          m_server( NULL )           ,
          m_serverConnection( NULL ) ,
          m_spe( NULL )              ,
          m_speClient( NULL )
    {
        GUCEF::CORE::CGUCEFApplication* app = GUCEF::CORE::CGUCEFApplication::Instance();
        
        m_spe = new CServerPortExtender( app->GetPulseGenerator() );
        m_speClient = new CServerPortExtenderClient( app->GetPulseGenerator() );
        
        SubscribeTo( app );
        SubscribeTo( m_spe );
    }
    
    virtual ~SpeTestController()
    {
        delete m_spe;
        m_spe = NULL;
        
        delete m_speClient;
        m_speClient = NULL;
        
        delete m_client;
        m_client = NULL;
        
        delete m_server;
        m_server = NULL;
    }
    
    virtual void OnNotify( CORE::CNotifier* notifier           ,
                           const CORE::CEvent& eventid         ,
                           CORE::CICloneable* eventdata = NULL )
    {
        if ( GUCEF::CORE::CGUCEFApplication::AppInitEvent == eventid )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "SpeTestController: Application initializing..." );
            
            m_client = new COMCORE::CTCPClientSocket( false );
            m_server = new COMCORE::CTCPServerSocket( false );
            
            SubscribeTo( m_client );
            SubscribeTo( m_server );
            
            m_spe->ListenForClientsOnPort( 10001 );
            m_spe->ListenForReversedClientsOnPort( 10002 );
            m_spe->ListenForReversedControlClientOnPort( 10003 );
            
            m_speClient->SetLocalServer( "localhost", 10000 );
            m_speClient->SetRemoteServerSocket( 10002 );
            m_server->ListenOnPort( 10000 );            
        }
        else
        if ( notifier == m_spe )
        {
            if ( CServerPortExtender::ControlSocketOpenedEvent == eventid )
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "SpeTestController: Connecting to control socket" );                
                m_speClient->ConnectToSPEControlSocket( "localhost", 10003 );
            }
            else
            if ( CServerPortExtender::ReversedSocketOpenedEvent == eventid )
            {
                
            }
            else
            if ( CServerPortExtender::ControlConnectionEstablishedEvent == eventid )
            {
                // now that we have a control connection lets try connecting a client
                // to the extended server socket
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "SpeTestController: SPE control connection established" );
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "SpeTestController: Connecting test client to the extended server socket" );
                m_client->ConnectTo( "localhost", 10001 );
            }
        }
        else
        if ( notifier == m_server )
        {
            if ( COMCORE::CTCPServerSocket::ClientConnectedEvent == eventid )
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "SpeTestController: A client connected to the test server" );
                
                // Get the connection object
                COMCORE::CTCPServerSocket::TClientConnectedEventData* eData = static_cast< GUCEF::COMCORE::CTCPServerSocket::TClientConnectedEventData* >( eventdata );
                COMCORE::CTCPServerSocket::TConnectionInfo& connectionInfo = eData->GetData();
                
                // keep a convenience pointer to the connection object which is ok knowing how the test works
                m_serverConnection = connectionInfo.connection;
                
                SubscribeTo( connectionInfo.connection );
            }
        }
        else
        if ( notifier == m_client )
        {
            if ( GUCEF::COMCORE::CTCPClientSocket::ConnectedEvent == eventid )
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "SpeTestController: Sending test string from the test client" );
                m_client->SendString( "Client:ThisIsATestString" );
            }
            else
            if ( GUCEF::COMCORE::CTCPClientSocket::DataRecievedEvent == eventid )
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "SpeTestController: recieved data on the test client" );
                
                // Get the data buffer
                COMCORE::CTCPClientSocket::TDataRecievedEventData* eData = static_cast< COMCORE::CTCPClientSocket::TDataRecievedEventData* >( eventdata );
                const CORE::CDynamicBuffer& data = eData->GetData();
                
                CORE::CString testString;
                testString.Scan( (const char*)data.GetConstBufferPtr(), data.GetDataSize() );
                
                if ( testString == "Server:ThisIsATestString" )
                {
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "SpeTestController: data was successfully communicated from the test server to the test client using the SPE functionality!!!" );
                    
                    // Now see if we can do it again with a data packet of a different size
                     m_client->SendString( "Client:ThisIsTheSecondTestString" );
                }
                else
                if ( testString == "Server:ThisIsTheSecondTestString" )
                {
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "SpeTestController: data was successfully communicated from the test server to the test client using the SPE functionality for the second time!!!" );                    
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "SpeTestController: Finished the test successfully, we can now shut down" );
                    GUCEF::CORE::CGUCEFApplication::Instance()->Stop();
                }
                else
                {
                    // error
                }
            }
        }
        else
        if ( notifier == m_serverConnection )
        {
            if ( COMCORE::CTCPServerConnection::DataRecievedEvent == eventid )
            {
                // Get the data buffer
                COMCORE::CTCPServerConnection::TDataRecievedEventData* eData = static_cast< COMCORE::CTCPServerConnection::TDataRecievedEventData* >( eventdata );
                const CORE::CDynamicBuffer& data = eData->GetData();
                
                CORE::CString testString;
                testString.Scan( (const char*)data.GetConstBufferPtr(), data.GetDataSize() );

                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "SpeTestController: Received data on the test server: " + testString );
                
                if ( testString == "Client:ThisIsATestString" )
                {
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "SpeTestController: data was successfully communicated from the test client to the test server using the SPE functionality!!!" );
                    
                    m_serverConnection->SendString( "Server:ThisIsATestString" );
                }
                else
                if ( testString == "Client:ThisIsTheSecondTestString" )
                {
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "SpeTestController: data was successfully communicated from the test client to the test server using the SPE functionality for the second time!!!" );
                    
                    m_serverConnection->SendString( "Server:ThisIsTheSecondTestString" );
                }
                else
                {
                    // error
                }
            }
        }
    }
    
    COMCORE::CTCPClientSocket* m_client;
    COMCORE::CTCPServerSocket* m_server;
    COMCORE::CTCPServerConnection* m_serverConnection;
    CServerPortExtender* m_spe;
    CServerPortExtenderClient* m_speClient;
};


/*-------------------------------------------------------------------------*/

int
TestSPE( int argc , char* argv[], CORE::CValueList& keyValueList )
{GUCEF_TRACE;

    SpeTestController testController;
    return GUCEF::CORE::CGUCEFApplication::Instance()->main( argc, argv, true );
}

/*-------------------------------------------------------------------------*/

void
ParseParams( const int argc                 , 
             char* argv[]                   ,
             CORE::CValueList& keyValueList )
{GUCEF_TRACE;
    
    keyValueList.DeleteAll();
    GUCEF::CORE::CString argString;
    if ( argc > 0 )
    {
        argString = *argv;

        // Combine the argument strings back into a single string because we don't want to use
        // a space as the seperator
        for ( int i=1; i<argc; ++i )
        {
            argString += ' ' + CORE::CString( argv[ i ] );
        }
        
        // Parse the param list based on the ' symbol
        keyValueList.SetMultiple( argString, '\'' );
    }
}

/*-------------------------------------------------------------------------*/

int
main( int argc, char* argv[] )
{GUCEF_TRACE;

    #ifdef GUCEF_CORE_DEBUG_MODE
    CORE::GUCEF_LogStackTo( "SPE_Callstack.cvs" );
    CORE::GUCEF_SetStackLoggingInCvsFormat( 1 );
    CORE::GUCEF_SetStackLogging( 1 );
    #endif /* GUCEF_CORE_DEBUG_MODE ? */    
    
    CORE::CString logFilename = GUCEF::CORE::RelativePath( "$CURWORKDIR$" );
    CORE::AppendToPath( logFilename, "ServerPortExtender_Log.txt" );
    CORE::CFileAccess logFileAccess( logFilename, "w" );
    
    CORE::CStdLogger logger( logFileAccess );
    CORE::CLogManager::Instance()->AddLogger( &logger );
    
    // Parse the application parameters
    CORE::CValueList keyValueList;
    ParseParams( argc, argv, keyValueList );

    // Do we want to display the console window?
    #ifdef GUCEF_MSWIN_BUILD    
    CORE::CMSWinConsoleLogger* consoleOut = NULL;    
    bool showConsole = true;    
    if ( keyValueList.HasKey( "showConsole" ) )
    {
        showConsole = CORE::StringToBool( keyValueList.GetValue( "showConsole" ) );
    }
    if ( showConsole )
    {
        consoleOut = new CORE::CMSWinConsoleLogger();
        CORE::CLogManager::Instance()->AddLogger( consoleOut );
        
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "SPE: Enabled console output" );
    }
    #endif /* GUCEF_MSWIN_BUILD ? */

    // Check if we want to do a diagnostic test
    bool runTest = false;
    if ( keyValueList.HasKey( "runTest" ) ) 
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "SPE: Found request for diagnostic test,.. running test" );
        runTest = CORE::StringToBool( keyValueList.GetValue( "runTest" ) );
    }
    if ( runTest )
    {
        TestSPE( argc, argv, keyValueList );
    }
    
    // Check for the application role
    bool actAsServer = true;
    if ( keyValueList.HasKey( "appRole" ) ) 
    {
        CORE::CString appRole = keyValueList.GetValue( "appRole" );
        if ( appRole == "client" )
        {
            actAsServer = false;            
        }
    }
    
    if ( actAsServer )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "SPE: application role set to server" );
        
        CORE::UInt16 extendedServerPort = 10234;
        if ( keyValueList.HasKey( "extendedServerPort" ) ) 
        {
            extendedServerPort = CORE::StringToUInt16( keyValueList.GetValue( "extendedServerPort" ) );
        }
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "SPE: extended server port set to " + CORE::UInt16ToString( extendedServerPort ) );
        
        CORE::UInt16 serverControlPort = 10236;
        if ( keyValueList.HasKey( "serverControlPort" ) ) 
        {
            serverControlPort = CORE::StringToUInt16( keyValueList.GetValue( "serverControlPort" ) );
        }
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "SPE: server control port set to " + CORE::UInt16ToString( serverControlPort ) );
        
        CORE::UInt16 reversedServerPort = 10235;
        if ( keyValueList.HasKey( "reversedServerPort" ) ) 
        {
            reversedServerPort = CORE::StringToUInt16( keyValueList.GetValue( "reversedServerPort" ) );
        }
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "SPE: reversed server port set to " + CORE::UInt16ToString( reversedServerPort ) );
        
        CORE::CGUCEFApplication* app = CORE::CGUCEFApplication::Instance();
        CServerPortExtender serverPortExtender( app->GetPulseGenerator() );
        
        serverPortExtender.ListenForReversedControlClientOnPort( serverControlPort );
        serverPortExtender.ListenForReversedClientsOnPort( reversedServerPort );
        serverPortExtender.ListenForClientsOnPort( extendedServerPort );        
        
        return app->main( argc, argv, true );
    }
    else
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "SPE: application role set to client" );

        CORE::UInt16 actualServerPort = 10234;
        if ( keyValueList.HasKey( "actualServerPort" ) ) 
        {
            actualServerPort = CORE::StringToUInt16( keyValueList.GetValue( "actualServerPort" ) );
        }
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "SPE: actual server port set to " + CORE::UInt16ToString( actualServerPort ) );
        
        CORE::CString actualServerHostname = "localhost";
        if ( keyValueList.HasKey( "actualServerHostname" ) ) 
        {
            actualServerHostname = keyValueList.GetValue( "actualServerHostname" );
        }
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "SPE: actual server hostname set to " + actualServerHostname );
        
        CORE::UInt16 reversedServerPort = 10235;
        if ( keyValueList.HasKey( "reversedServerPort" ) ) 
        {
            reversedServerPort = CORE::StringToUInt16( keyValueList.GetValue( "reversedServerPort" ) );
        }
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "SPE: reversed server port set to " + CORE::UInt16ToString( reversedServerPort ) );

        CORE::CString speServerHostname = "localhost";
        if ( keyValueList.HasKey( "speServerHostname" ) ) 
        {
            speServerHostname = keyValueList.GetValue( "speServerHostname" );
        }
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "SPE: SPE server hostname set to " + speServerHostname );

        CORE::UInt16 speServerPort = 10236;
        if ( keyValueList.HasKey( "speServerControlPort" ) ) 
        {
            speServerPort = CORE::StringToUInt16( keyValueList.GetValue( "speServerControlPort" ) );
        }
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "SPE: SPE server port set to " + CORE::UInt16ToString( speServerPort ) );
                        
        CORE::CGUCEFApplication* app = CORE::CGUCEFApplication::Instance();
        CServerPortExtenderClient serverPortExtenderClient( app->GetPulseGenerator() );
        
        serverPortExtenderClient.SetLocalServer( actualServerHostname, actualServerPort );
        serverPortExtenderClient.SetRemoteServerSocket( reversedServerPort );
        
        serverPortExtenderClient.ConnectToSPEControlSocket( speServerHostname, speServerPort );
        return app->main( argc, argv, true );
    }
    

    #ifdef GUCEF_MSWIN_BUILD
    delete consoleOut;
    consoleOut = NULL;
    #endif /* GUCEF_MSWIN_BUILD ? */
    
    return 0;
}

/*---------------------------------------------------------------------------*/

#ifdef GUCEF_MSWIN_BUILD

int __stdcall
WinMain( HINSTANCE hinstance     ,
         HINSTANCE hprevinstance ,
         LPSTR lpcmdline         ,
         int ncmdshow            )
{GUCEF_TRACE;

    int argc = 0;
    char** argv = &lpcmdline;
    if ( lpcmdline != NULL )
    {
        if ( *lpcmdline != '\0' )
        {
            argc = 1;
        }
    }

    return main( argc, argv );
}

#endif

/*---------------------------------------------------------------------------*/
