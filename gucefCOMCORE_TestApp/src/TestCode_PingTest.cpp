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

#ifdef GUCEF_MSWIN_BUILD
  #include <windows.h>
#else
  #include <assert.h>
#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MSWIN_BUILD
  #define DebugBreak() assert( 0 )
#endif

#define ERRORHERE { GUCEF_ERROR_LOG( GUCEF::CORE::LOGLEVEL_NORMAL, GUCEF::CORE::CString( "Test failed @ " ) + GUCEF::CORE::CString( __FILE__ ) + ':' + GUCEF::CORE::Int32ToString( __LINE__ ) ); \
                    DebugBreak();                                                                                                                                     \
                  }

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#define MAX_PINGS    1
#define PING_BYTES   32
#define PING_TIMEOUT 1000
#define REMOTE_HOST  "www.google.com"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

using namespace GUCEF;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CPingTester : public CORE::CObserver
{
    public:
    
    /*---------------------------------------------------------------------*/
    
    CPingTester( void )
        : CORE::CObserver() ,
          m_pingCount( 0 )  ,
          m_ping()
    {GUCEF_TRACE;
    
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "**** STARTING THE PING TEST ****" );
        
        SubscribeTo( &m_ping );
    }
    
    /*---------------------------------------------------------------------*/
    
    virtual ~CPingTester()
    {GUCEF_TRACE;
    
    }
    
    /*---------------------------------------------------------------------*/
    
    protected:
    
    /*---------------------------------------------------------------------*/
    
    virtual void OnNotify( CORE::CNotifier* notifier           ,
                           const CORE::CEvent& eventid         ,
                           CORE::CICloneable* eventdata = NULL )
    {GUCEF_TRACE;
    
        if ( eventid == CORE::CGUCEFApplication::AppInitEvent )
        {   
            printf( "CPingTester: pinging %s for %d time(s) with %d bytes and a timeout of %d ms\n", REMOTE_HOST, MAX_PINGS, PING_BYTES, PING_TIMEOUT );
            if ( !m_ping.Start( REMOTE_HOST  , 
                                MAX_PINGS    ,
                                PING_BYTES   ,
                                PING_TIMEOUT ) )
            {
                // Failed to start the ping sequence
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CPingTester: Failed to start the ping sequence" );
                ERRORHERE;
            }
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "CPingTester: Ping start requested" );
        }
        else
        if ( eventid == CORE::CGUCEFApplication::AppShutdownEvent )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "CPingTester: The application is shutting down" ); 
        }
        else
        if ( eventid == COMCORE::CPing::PingReponseEvent )
        {
            COMCORE::CPing::CPingEventData* eData = static_cast< COMCORE::CPing::CPingEventData* >( eventdata );
            if ( NULL == eData )
            {
                // We should have received data with this event
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CPingTester: no event data for event PingReponseEvent" );
                ERRORHERE;
            }            
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "CPingTester: Received ping response from " + eData->GetHostAddress().GetHostname() + ": " + CORE::UInt32ToString( eData->GetRoundTripTime() ) + " ms" );
                        
            if ( m_pingCount > MAX_PINGS )
            {
                // check if the maximum ping count functionality is working
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CPingTester: we are getting more ping responses then we asked for\n" );
                ERRORHERE;
            }
            ++m_pingCount;
        }
        else
        if ( eventid == COMCORE::CPing::PingStartedEvent )
        {   
            COMCORE::CPing::CPingEventData* eData = static_cast< COMCORE::CPing::CPingEventData* >( eventdata );
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "CPingTester: started pinging " + eData->GetHostAddress().GetHostname() );
        }
        else
        if ( eventid == COMCORE::CPing::PingTimeoutEvent )
        {   
            // We are pinging REMOTE_HOST, we should not get a timeout (unless REMOTE_HOST is having a bad day)
            COMCORE::CPing::CPingEventData* eData = static_cast< COMCORE::CPing::CPingEventData* >( eventdata );
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CPingTester: timeout while pinging " + eData->GetHostAddress().GetHostname() + " this should not happen" );
            ERRORHERE;
        }
        else
        if ( eventid == COMCORE::CPing::PingFailedEvent )
        {   
            // some error occurred while attempting to ping
            COMCORE::CPing::CPingEventData* eData = static_cast< COMCORE::CPing::CPingEventData* >( eventdata );
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CPingTester: error while pinging " + eData->GetHostAddress().GetHostname() );
            ERRORHERE;
        }
        else
        if ( eventid == COMCORE::CPing::PingStoppedEvent )
        {
            // If we got here without any errors then we are finished
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "CPingTester: Stopped pinging" );
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "**** SUCCESSFULLY COMPLETED THE PING TEST ****" );
            CORE::CGUCEFApplication::Instance()->Stop();
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "CPingTester: Application stopping" );
        }                
        else
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "CPingTester: Received event: " + eventid.GetName() );
        }
    }
    
    /*---------------------------------------------------------------------*/
    
    private:
    
    CORE::UInt32 m_pingCount;
    COMCORE::CPing m_ping;
    static CPingTester* m_instance;   
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
PerformPingTest( void )
{   
    try
    {
        CORE::CGUCEFApplication* app = CORE::CGUCEFApplication::Instance();
        CPingTester pingTester;
        pingTester.SubscribeTo( app );
        app->main( 0, NULL, true );
    }
    catch ( CORE::CMsgException& e )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, CORE::CString( "unhandled GUCEF exception during Ping test: " ) + e.what() );
        ERRORHERE;
    }
    catch ( std::exception& e )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, CORE::CString( "unhandled std exception during Ping test: " ) + e.what() );
        ERRORHERE;
    }
    catch ( ... )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "unhandled exception during Ping test" );
        ERRORHERE;
    }
}

/*-------------------------------------------------------------------------*/