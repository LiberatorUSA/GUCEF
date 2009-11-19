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

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

class SpeTestController : CORE::CObserver
{
    public:
    
    SpeTestController()
    {
        GUCEF::CORE::CGUCEFApplication* app = GUCEF::CORE::CGUCEFApplication::Instance();
        
        m_spe = new CServerPortExtender( app->GetPulseGenerator() );
        
        SubscribeTo( app );
        SubscribeTo( m_spe );
    }
    
    virtual ~SpeTestController()
    {
        delete m_spe;
        m_spe = NULL;
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
            
            speClient.SetLocalServer( "localhost", 10000 );
            speClient.SetRemoteServerSocket( 10002 );
            m_server->ListenOnPort( 10000 );            
        }
        else
        if ( notifier == m_spe )
        {
            if ( CServerPortExtender::ControlSocketOpenedEvent == eventid )
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "SpeTestController: Connecting to control socket" );                
                speClient.ConnectToSPEControlSocket( "localhost", 10003 );
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
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "SpeTestController: Connecting test client to the extended server socket" );
                m_client->ConnectTo( "localhost", 10001 );
            }
        }
        else
        if ( notifier == m_server )
        {
            if ( GUCEF::COMCORE::CTCPServerSocket::ServerSocketOpenedEvent == eventid )
            {
                
            }
        }
        else
        if ( notifier == m_client )
        {
            if ( GUCEF::COMCORE::CTCPClientSocket::ConnectedEvent == eventid )
            {
                m_client->Send( "ThisIsATestString" );
            }
        }
    }
    
    COMCORE::CTCPClientSocket* m_client;
    COMCORE::CTCPServerSocket* m_server;
    CServerPortExtender* m_spe;
    CServerPortExtenderClient speClient;
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
main( int argc , char* argv[] )
{GUCEF_TRACE;

    CORE::CString logFilename = GUCEF::CORE::RelativePath( "$CURWORKDIR$" );
    CORE::AppendToPath( logFilename, "ServerPortExtender_Log.txt" );
    CORE::CFileAccess logFileAccess( logFilename, "w" );
    
    CORE::CStdLogger logger( logFileAccess );
    CORE::CLogManager::Instance()->AddLogger( &logger );
    
    #ifdef GUCEF_MSWIN_BUILD
    CORE::CMSWinConsoleLogger consoleOut;
    CORE::CLogManager::Instance()->AddLogger( &consoleOut );
    #endif /* GUCEF_MSWIN_BUILD ? */

    CORE::CValueList keyValueList;
    ParseParams( argc, argv, keyValueList );
                                              
    return TestSPE( argc, argv, keyValueList );
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
