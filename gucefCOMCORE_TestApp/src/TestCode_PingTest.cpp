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

#define ERRORHERE { printf( "Test failed @ %s(%d)\n", __FILE__, __LINE__ ); DebugBreak(); }

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#define MAX_PINGS    50
#define PING_BYTES   32
#define PING_TIMEOUT 1000

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

class CPingTester : public CORE::CGUCEFAppSubSystem
{
    public:
    
    /*---------------------------------------------------------------------*/
    
    static CPingTester* Instance( void )
    {TRACE;
        
        if ( NULL == m_instance )
        {
            m_instance = new CPingTester();
        }
        return m_instance;
    }
    
    /*---------------------------------------------------------------------*/
    
    private:
    
    /*---------------------------------------------------------------------*/
    
    static void Deinstance( void )
    {TRACE;
    
        delete m_instance;
        m_instance = NULL;
    }
    
    /*---------------------------------------------------------------------*/
    
    CPingTester( void )
        : CORE::CGUCEFAppSubSystem( true ) ,
          m_pingCount( 0 )                 ,
          m_ping()
    {TRACE;
    
        SubscribeTo( &m_ping );
    }
    
    /*---------------------------------------------------------------------*/
    
    virtual ~CPingTester()
    {TRACE;
    
    }
    
    /*---------------------------------------------------------------------*/
    
    protected:
    
    /*---------------------------------------------------------------------*/
    
    virtual void OnNotify( CORE::CNotifier* notifier           ,
                           const CORE::CEvent& eventid         ,
                           CORE::CICloneable* eventdata = NULL )
    {TRACE;
    
        if ( eventid == CORE::CGUCEFApplication::AppInitEvent )
        {
            printf( "**** STARTING THE PING TEST ****\n" );
            
            if ( !m_ping.Start( "127.0.0.1"  , 
                                MAX_PINGS    ,
                                PING_BYTES   ,
                                PING_TIMEOUT ) )
            {
                // Failed to start the ping sequence
                printf( "ERROR: CPingTester: Failed to start the ping sequence\n" );
                ERRORHERE;
            }
        }
        else
        if ( eventid == CORE::CGUCEFApplication::AppShutdownEvent )
        {
            Deinstance();
        }
        else
        if ( eventid == COMCORE::CPing::PingReponseEvent )
        {
            COMCORE::CPing::TPingReponseEventData* eData = static_cast< COMCORE::CPing::TPingReponseEventData* >( eventdata );
            if ( NULL == eData )
            {
                // We should have received data with this event
                printf( "ERROR: CPingTester: no event data for event PingReponseEvent\n" );
                ERRORHERE;
            }            
            printf( "CPingTester: Received ping response: %d ms\n", eData->GetData() );
            ++m_pingCount;
            
            if ( m_pingCount == MAX_PINGS )
            {
                printf( "**** SUCCESSFULLY COMPLETED THE PING TEST ****\n" );
                CORE::CGUCEFApplication::Instance()->Stop();
            }
        }
        else
        if ( eventid == COMCORE::CPing::PingTimeoutEvent )
        {   
            // We are pinging the localhost, we should not get a timeout
            printf( "ERROR: CPingTester: timeout while pinging localhost\n" );
            ERRORHERE;
        }
        else
        {
            printf( "CPingTester: Received event: %s\n", COMCORE::CPing::PingStartedEvent.GetName() );
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
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

CPingTester* CPingTester::m_instance = NULL;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
PerformPingTest( void )
{
    CPingTester::Instance();
    CORE::CGUCEFApplication::Instance()->main( 0, NULL, true );
}

/*-------------------------------------------------------------------------*/