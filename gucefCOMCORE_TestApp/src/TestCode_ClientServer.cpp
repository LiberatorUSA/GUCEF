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

#ifndef GUCEFCORE_H
#include "gucefCORE.h"          /* GUCEF CORE library API */
#define GUCEFCORE_H
#endif /* GUCEFCORE_H ? */

#ifndef GUCEFCOMCORE_H
#include "gucefCOMCORE.h"       /* GUCEF COMCORE library API */
#define GUCEFCOMCORE_H
#endif /* GUCEFCOMCORE_H ? */


/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

using namespace GUCEF;
using namespace GUCEF::MT;
using namespace GUCEF::CORE;
using namespace GUCEF::COMCORE;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CTestTCPClientServer : public CGUCEFAppSubSystem
{
        public:
        
        static CTestTCPClientServer* Instance( bool block = false );
        
        CTestTCPClientServer( bool block = false );
        
        protected:
                   
        virtual void OnNotify( CORE::CNotifier* notifier           ,
                               const CORE::CEvent& eventid         ,
                               CORE::CICloneable* eventdata = NULL );
        
        /**
         *      Called each event pump update cycle after the events for 
         *      the current cycle have been pumped to all clients.
         *
         *      @param tickcount the tick count when the Update process commenced.
         *      @param deltaticks ticks since the last Update process commenced.
         */        
        virtual void OnUpdate( const CORE::UInt64 tickcount               ,
                               const CORE::Float64 updateDeltaInMilliSecs );
        
        private:
        
        static void Deinstance( void );
        
        private:
        CTCPServerSocket m_serverSock; 
        CTCPClientSocket m_clientSock;
        static CTestTCPClientServer* s_instance;                              
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

CTestTCPClientServer* CTestTCPClientServer::s_instance = NULL;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CTestTCPClientServer*
CTestTCPClientServer::Instance( bool block /* = false */ )
{TRACE;

    if ( NULL == s_instance )
    {
        s_instance = new CTestTCPClientServer( block );
    }
    return s_instance;
}

/*-------------------------------------------------------------------------*/

CTestTCPClientServer::CTestTCPClientServer( bool block /* = false */ )
        : CGUCEFAppSubSystem( true ) ,
          m_serverSock( block )      ,
          m_clientSock( block )
{TRACE;

}

/*-------------------------------------------------------------------------*/

void 
CTestTCPClientServer::OnNotify( CORE::CNotifier* notifier                 ,
                                const CORE::CEvent& eventid               ,
                                CORE::CICloneable* eventdata /* = NULL */ )
{TRACE;

    if ( eventid == CGUCEFApplication::AppInitEvent )
    {
        m_clientSock.Subscribe( &AsObserver() );
        m_serverSock.Subscribe( &AsObserver() );

        m_serverSock.ListenOnPort( 10000 );
        m_clientSock.ConnectTo( "127.0.0.1" ,
                                10000       );                                                      
    }
    else
    if ( eventid == CGUCEFApplication::AppShutdownEvent )
    {
        // The test application is shutting down se we can cleanup our
        // test sub-system
        Deinstance();
    }
    else
    if ( notifier == &m_serverSock )                    
    {
        if ( eventid == CTCPServerSocket::ClientConnectedEvent )
        {
            printf( "%s\r\n", eventid.GetName() );
            
           // CTCPServerSocket* serverSocket = static_cast< CTCPServerSocket* >( notifier );
           // serverSocket->
        }
        else
        {
            printf( "%s - UNHANDLED\r\n", eventid.GetName() );
        }
    }
    else
    {
        printf( "%s - UNHANDLED\r\n", eventid.GetName() );
    }    
}

/*-------------------------------------------------------------------------*/

void
CTestTCPClientServer::OnUpdate( const CORE::UInt64 tickcount               ,
                                const CORE::Float64 updateDeltaInMilliSecs )
{TRACE;
        /* don't hogg the CPU */
       // ThreadDelay( 10 );
}

/*-------------------------------------------------------------------------*/

void
CTestTCPClientServer::Deinstance( void )
{TRACE;
    
    delete s_instance;
    s_instance = NULL;
}        

/*-------------------------------------------------------------------------*/

void
SetupClientServerTest( void )
{TRACE;

    // Create our test application sub-system
    CTestTCPClientServer::Instance();
}

/*-------------------------------------------------------------------------*/
