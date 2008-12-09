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

//-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------//

#ifndef GUCEF_CORE_CGUCEFAPPLICATION_H
#include "CGUCEFApplication.h"
#define GUCEF_CORE_CGUCEFAPPLICATION_H
#endif /* GUCEF_CORE_CGUCEFAPPLICATION_H ? */

#include "CPing.h"           /* declaration of the class */

#ifdef GUCEF_MSWIN_BUILD

  /* #include <Icmpapi.h> -> this is the header for the functions that we dynamicly link */ 
   
  #ifndef GUCEF_COMCORE_ICMPAPI_H
  #include "gucefCOMCORE_icmpApi.h"
  #define GUCEF_COMCORE_ICMPAPI_H
  #endif /* GUCEF_COMCORE_ICMPAPI_H ? */
  
  #ifndef GUCEF_CORE_DVOSWRAP_H
  #include "DVOSWRAP.h"
  #define GUCEF_CORE_DVOSWRAP_H
  #endif /* GUCEF_CORE_DVOSWRAP_H ? */
  
  #ifndef GUCEF_MT_CACTIVEOBJECT_H
  #include "gucefMT_CActiveObject.h"
  #define GUCEF_MT_CACTIVEOBJECT_H
  #endif /* GUCEF_MT_CACTIVEOBJECT_H ? */

  #ifndef GUCEF_CORE_CTSGNOTIFIER_H
  #include "CTSGNotifier.h"
  #define GUCEF_CORE_CTSGNOTIFIER_H
  #endif /* GUCEF_CORE_CTSGNOTIFIER_H ? */

  #ifndef GUCEF_MT_CMUTEX_H
  #include "gucefMT_CMutex.h"
  #define GUCEF_MT_CMUTEX_H
  #endif /* GUCEF_MT_CMUTEX_H ? */
  
  #ifndef GUCEF_COMCORE_CSOCKET_H
  #include "CSocket.h"
  #define GUCEF_COMCORE_CSOCKET_H
  #endif /* GUCEF_COMCORE_CSOCKET_H ? */    

#endif /* GUCEF_MSWIN_BUILD ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CEvent CPing::PingStartedEvent = "GUCEF::COMCORE::CPing::PingStartedEvent";
const CORE::CEvent CPing::PingReponseEvent = "GUCEF::COMCORE::CPing::PingReponseEvent";
const CORE::CEvent CPing::PingTimeoutEvent = "GUCEF::COMCORE::CPing::PingTimeoutEvent";
const CORE::CEvent CPing::PingFailedEvent = "GUCEF::COMCORE::CPing::PingFailedEvent";
const CORE::CEvent CPing::PingStoppedEvent = "GUCEF::COMCORE::CPing::PingStoppedEvent";

bool icmpLinked = false;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef GUCEF_MSWIN_BUILD

/**
 *  private task class for handling ping behavior on ms windows
 */
class CMSWinPingTask : public MT::CActiveObject   ,
                       private CORE::CTSGNotifier
{
    protected:
    
    /*---------------------------------------------------------------------*/
    
    virtual bool OnTaskStart( void* taskdata )
    {GUCEF_TRACE;
    
        if ( m_replyBuffer == NULL )
        {
            m_icmpHandle = IcmpCreateFile();
            if ( m_icmpHandle == NULL )
            {
                // failed to create ICMP access
                return false;
            }
           
            // resolve the DNS name if we have one and get the network byte order ip
            if ( CSocket::ConvertToIPAddress( m_remoteHost ,
                                              80           ,
                                              m_ip         ) )
            {
                // Allocate the buffers we will need
                m_replyBuffer = (VOID*) malloc( m_replyBufferSize );
                m_sendBuffer = (VOID*) malloc( m_bytesToSend );
            }
            else
            {
                NotifyObserversFromThread( CPing::PingFailedEvent );
                return false;
            }
            
            NotifyObserversFromThread( CPing::PingStartedEvent );
            
            return true;
        }
        return false;
    }
    
    /*---------------------------------------------------------------------*/

    virtual bool OnTaskCycle( void* taskdata )
    {GUCEF_TRACE;
    
        if ( ( m_maxPings == 0 )              ||
             ( m_pingsCompleted < m_maxPings ) )
        {
            // perform the actual 'ping', this is a blocking call
            DWORD dwRetVal;
            IPAddr ipAddr;
            ipAddr.S_un.S_addr = m_ip.GetAddress();
            if ( ( dwRetVal = IcmpSendEcho( (HANDLE)m_icmpHandle    ,
                                            ipAddr                  ,
                                            m_sendBuffer            ,
                                            (WORD)m_bytesToSend     ,
                                            NULL                    , 
                                            m_replyBuffer           ,
                                            (WORD)m_replyBufferSize ,
                                            (DWORD)m_timeout        ) ) > 0 ) 
            {
                // Get the RTT (Round Trip Time)
                ICMP_ECHO_REPLY& reply = ( *(ICMP_ECHO_REPLY*) m_replyBuffer );
                
                if ( ( reply.Status == 0  )               ||
                     ( reply.Status == IP_SUCCESS )       || 
                     ( reply.Status == IP_SOURCE_QUENCH ) )
                {
                    // Notify our observers with a ping response event
                    CPing::TPingReponseEventData eData( reply.RoundTripTime );
                    NotifyObserversFromThread( CPing::PingReponseEvent, &eData );
                }
                else
                {
                    if ( ( reply.Status == IP_TIME_EXCEEDED ) || ( reply.Status == IP_REQ_TIMED_OUT ) )
                    {
                         NotifyObserversFromThread( CPing::PingTimeoutEvent );
                    }
                    else
                    {
                        NotifyObserversFromThread( CPing::PingFailedEvent );
                    }
                }
            }
            else 
            {
                NotifyObserversFromThread( CPing::PingFailedEvent );
            }
            
            ++m_pingsCompleted;        
            return false;
        }
        else
        {
            // we are done, end the task
            return true;
        }
    }
    
    /*---------------------------------------------------------------------*/

    virtual void OnTaskEnd( void* taskdata )
    {GUCEF_TRACE;

        IcmpCloseHandle( m_icmpHandle );
        
        free( m_replyBuffer );
        m_replyBuffer = NULL;
        free( m_sendBuffer );
        m_sendBuffer = NULL;
        
        NotifyObserversFromThread( CPing::PingStoppedEvent );
    }
    
    /*---------------------------------------------------------------------*/
            
    private:
    friend class CPing;
    
    /*---------------------------------------------------------------------*/
    
    CMSWinPingTask( void )
        : CActiveObject()                                                          ,
          CTSGNotifier( CORE::CGUCEFApplication::Instance()->GetPulseGenerator() ) ,
          m_replyBuffer( NULL )                                                    ,
          m_sendBuffer( NULL )      ,
          m_icmpHandle( NULL )      ,
          m_remoteHost()            ,
          m_maxPings( 0 )           ,
          m_bytesToSend( 32 )       ,
          m_replyBufferSize( 0 )    ,
          m_timeout( 1000 )         ,
          m_pingsCompleted( 0 )
    {GUCEF_TRACE;
    }
    
    /*---------------------------------------------------------------------*/
    
    virtual ~CMSWinPingTask()
    {GUCEF_TRACE;
    }

    /*---------------------------------------------------------------------*/
        
    bool SetTaskData( const CORE::CString& remoteHost     ,
                      const UInt32 maxPings = 0           ,
                      const UInt32 bytesToSend = 32       ,
                      const UInt32 timeout = 1000         )
    {GUCEF_TRACE;
    
        if ( !IsActive() )
        {
            m_remoteHost = remoteHost;
            m_maxPings = maxPings;
            m_timeout = timeout;
            m_bytesToSend = bytesToSend;
            m_replyBufferSize = sizeof( ICMP_ECHO_REPLY ) + LARGEST( m_bytesToSend, 8 );
            free( m_replyBuffer );
            m_replyBuffer = NULL;
            free( m_sendBuffer );
            m_sendBuffer = NULL;
            return true;
        }
        return false;
    }
    
    /*---------------------------------------------------------------------*/
    
    private:
    
    CORE::CString m_remoteHost;
    UInt32 m_maxPings;
    UInt32 m_pingsCompleted;
    UInt32 m_bytesToSend;
    UInt32 m_replyBufferSize;
    UInt32 m_timeout;
    LPVOID m_replyBuffer;
    LPVOID m_sendBuffer;
    HANDLE m_icmpHandle;
    CIPAddress m_ip;
};

#endif /* GUCEF_MSWIN_BUILD ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CPing::RegisterEvents( void )
{GUCEF_TRACE;
    
    PingStartedEvent.Initialize();
    PingReponseEvent.Initialize();
    PingTimeoutEvent.Initialize();
    PingFailedEvent.Initialize();
    PingStoppedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CPing::CPing( void )
    : CForwardingNotifier()    ,
      m_isActive( false )      ,
      m_remoteHost()           ,
      m_maxPings( 0 )          ,
      m_bytesToSend( 32 )      ,
      m_timeout( 1000 )        ,
      m_minimalPingDelta( 500 )
{GUCEF_TRACE;

    RegisterEvents();    
    
    #ifdef GUCEF_MSWIN_BUILD
    icmpLinked = LinkICMP() > 0;

    
    AddForwardingForEvent( PingStartedEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( PingReponseEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( PingTimeoutEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( PingFailedEvent, EVENTORIGINFILTER_TRANSFER );
    AddForwardingForEvent( PingStoppedEvent, EVENTORIGINFILTER_TRANSFER );
    #endif /* GUCEF_MSWIN_BUILD ? */
}

/*-------------------------------------------------------------------------*/
    
CPing::~CPing()
{GUCEF_TRACE;

    Stop();
    
    #ifdef GUCEF_MSWIN_BUILD
    UnlinkICMP();
    #endif /* GUCEF_MSWIN_BUILD ? */
}

/*-------------------------------------------------------------------------*/

bool
CPing::Start( const CORE::CString& remoteHost           ,
              const UInt32 maxPings /* = 0 */           ,
              const UInt32 bytesToSend /* = 32 */       ,
              const UInt32 timeout /* = 1000 */         ,
              const UInt32 minimalPingDelta /* = 500 */ )
{GUCEF_TRACE;

    TStringVector hostList;
    hostList.push_back( remoteHost );
    return Start( hostList         ,
                  maxPings         ,
                  bytesToSend      ,
                  timeout          ,
                  minimalPingDelta );
}              

/*-------------------------------------------------------------------------*/

bool
CPing::Start( const TStringVector& remoteHosts          ,
              const UInt32 maxPings /* = 0 */           ,
              const UInt32 bytesToSend /* = 32 */       ,
              const UInt32 timeout /* = 1000 */         ,
              const UInt32 minimalPingDelta /* = 500 */ )
{GUCEF_TRACE;

    if ( !m_isActive && ( remoteHosts.size() > 0 ) )
    {
        m_remoteHost = remoteHosts[ 0 ];
        m_maxPings = maxPings;
        m_timeout = timeout;
        m_bytesToSend = bytesToSend;
        m_minimalPingDelta = minimalPingDelta;
        
        #ifdef GUCEF_MSWIN_BUILD
        
        CMSWinPingTask* pingTask = new CMSWinPingTask();
        if ( pingTask != NULL )
        {
            if ( icmpLinked )
            {
                m_isActive = true;
                
                // configure our task and subscribe to it
                m_osData = pingTask;
                pingTask->SetTaskData( m_remoteHost     ,
                                       maxPings         ,
                                       bytesToSend      ,
                                       timeout          );                                   
                SubscribeTo( pingTask );
                
                // here we go...
                if ( minimalPingDelta > 0 )
                {
                    pingTask->Activate( NULL                 ,
                                        minimalPingDelta / 2 ,
                                        minimalPingDelta     );
                }
                else
                {
                    pingTask->Activate();
                }
                return true;
            }
            
            // This windows version does not support this functionality
            delete pingTask;
            return false;
        }
        
        #else /* GUCEF_MSWIN_BUILD ? */
        
        // No implementation is available for the target platform
        return false;
        #endif
    }
    return false;
}

/*-------------------------------------------------------------------------*/

void
CPing::Stop( void )
{GUCEF_TRACE;
    
    if ( m_isActive )
    {
        #ifdef GUCEF_MSWIN_BUILD        
        static_cast< CMSWinPingTask* >( m_osData )->Deactivate( true );
        delete  static_cast< CMSWinPingTask* >( m_osData );       
        #endif /* GUCEF_MSWIN_BUILD ? */
        
        m_osData = NULL;
    }
}

/*-------------------------------------------------------------------------*/
    
bool
CPing::IsActive( void ) const
{GUCEF_TRACE;
    
    return m_isActive;
}

/*-------------------------------------------------------------------------*/
    
const CORE::CString&
CPing::GetRemoteHost( void ) const
{GUCEF_TRACE;

    return m_remoteHost;
}

/*-------------------------------------------------------------------------*/
    
UInt32
CPing::GetMaxPings( void ) const
{GUCEF_TRACE;

    return m_maxPings;
}

/*-------------------------------------------------------------------------*/

void
CPing::SetUserData( void* userData )
{
    m_userData = userData; 
}

/*-------------------------------------------------------------------------*/

void*
CPing::GetUserData( void ) const
{
    return m_userData;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/