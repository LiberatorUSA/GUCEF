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

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H ? */

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
  
  #ifndef GUCEF_COMCORE_CPINGTASKCONSUMER_H
  #include "gucefCOMCORE_CPingTaskConsumer.h"
  #define GUCEF_COMCORE_CPINGTASKCONSUMER_H
  #endif /* GUCEF_COMCORE_CPINGTASKCONSUMER_H ? */

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
    
CPing::CPingEventData::CPingEventData( const CHostAddress& host   ,
                                       const UInt32 echoSize      ,
                                       const UInt32 roundTripTime )
    : CORE::CICloneable()              ,
      m_hostAddress( host )            ,
      m_echoSize( echoSize )           ,
      m_roundTripTime( roundTripTime )
{GUCEF_TRACE;
    
}

/*-------------------------------------------------------------------------*/

CPing::CPingEventData::CPingEventData( const CPingEventData& src )
    : CORE::CICloneable( src)                ,
      m_hostAddress( src.m_hostAddress )     ,
      m_echoSize( src.m_echoSize )           ,
      m_roundTripTime( src.m_roundTripTime )
{GUCEF_TRACE;    

}

/*-------------------------------------------------------------------------*/
        
CPing::CPingEventData::~CPingEventData()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
                
CORE::CICloneable*
CPing::CPingEventData::Clone( void ) const
{GUCEF_TRACE;

    return new CPingEventData( *this );
}

/*-------------------------------------------------------------------------*/
        
const CHostAddress&
CPing::CPingEventData::GetHostAddress( void ) const
{GUCEF_TRACE;

    return m_hostAddress;
}

/*-------------------------------------------------------------------------*/
        
UInt32
CPing::CPingEventData::GetEchoSize( void ) const
{GUCEF_TRACE;

    return m_echoSize;
}

/*-------------------------------------------------------------------------*/
        
UInt32
CPing::CPingEventData::GetRoundTripTime( void ) const
{GUCEF_TRACE;

    return m_roundTripTime;
}

/*-------------------------------------------------------------------------*/

CPing::CPing( CORE::CPulseGenerator& pulsGenerator )
    : CTSGNotifier( pulsGenerator ) ,
      m_isActive( false )           ,
      m_remoteHosts()               ,
      m_maxPings( 0 )               ,
      m_bytesToSend( 32 )           ,
      m_timeout( 1000 )             ,
      m_minimalPingDelta( 500 )     ,
      m_taskId( 0 )
{GUCEF_TRACE;

    RegisterEvents();    
    
    #ifdef GUCEF_MSWIN_BUILD
    icmpLinked = LinkICMP() > 0;
    #endif /* GUCEF_MSWIN_BUILD ? */    
}

/*-------------------------------------------------------------------------*/

CPing::CPing( void )
    : CTSGNotifier()            ,
      m_isActive( false )       ,
      m_remoteHosts()           ,
      m_maxPings( 0 )           ,
      m_bytesToSend( 32 )       ,
      m_timeout( 1000 )         ,
      m_minimalPingDelta( 500 ) ,
      m_taskId( 0 )
{GUCEF_TRACE;

    RegisterEvents();    
    
    #ifdef GUCEF_MSWIN_BUILD
    icmpLinked = LinkICMP() > 0;
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
              const Int32 maxPings /* = 0 */            ,
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
              const Int32 maxPings /* = 0 */            ,
              const UInt32 bytesToSend /* = 32 */       ,
              const UInt32 timeout /* = 1000 */         ,
              const UInt32 minimalPingDelta /* = 500 */ )
{GUCEF_TRACE;

    if ( !m_isActive && ( remoteHosts.size() > 0 ) )
    {
        m_remoteHosts = remoteHosts;
        m_maxPings = maxPings;
        m_timeout = timeout;
        m_bytesToSend = bytesToSend;
        m_minimalPingDelta = minimalPingDelta;
        
        CPingTaskConsumer::CPingTaskData taskData( remoteHosts, maxPings, bytesToSend, timeout, minimalPingDelta ); 
        return CORE::CTaskManager::Instance()->StartTask( CPingTaskConsumer::GetTypeString() , 
                                                          &taskData                          , 
                                                          &AsObserver()                      ,
                                                          &m_taskId                          );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

void
CPing::Stop( void )
{GUCEF_TRACE;
    
    if ( m_isActive )
    {
       CORE::CTaskManager::Instance()->RequestTaskToStop( m_taskId );
    }
}

/*-------------------------------------------------------------------------*/

void
CPing::OnPumpedNotify( CORE::CNotifier* notifier    ,
                       const CORE::CEvent& eventid  ,
                       CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    if ( CPingTaskConsumer::PingStartedEvent == eventid )
    {
        CPingTaskConsumer::CEchoReceivedEventData* taskData = static_cast< CPingTaskConsumer::CEchoReceivedEventData* >( eventdata );
        CPingEventData eventData( taskData->GetHostAddress(), taskData->GetEchoSize(), taskData->GetRoundTripTime() );
        NotifyObservers( PingStartedEvent, &eventData );
    }
    else
    if ( CPingTaskConsumer::PingReponseEvent == eventid )
    {
        CPingTaskConsumer::CEchoReceivedEventData* taskData = static_cast< CPingTaskConsumer::CEchoReceivedEventData* >( eventdata );
        CPingEventData eventData( taskData->GetHostAddress(), taskData->GetEchoSize(), taskData->GetRoundTripTime() );
        NotifyObservers( PingReponseEvent, &eventData );
    }
    else
    if ( CPingTaskConsumer::PingTimeoutEvent == eventid )
    {
        CPingTaskConsumer::CEchoReceivedEventData* taskData = static_cast< CPingTaskConsumer::CEchoReceivedEventData* >( eventdata );
        CPingEventData eventData( taskData->GetHostAddress(), taskData->GetEchoSize(), taskData->GetRoundTripTime() );
        NotifyObservers( PingTimeoutEvent, &eventData );
    }
    else
    if ( CPingTaskConsumer::PingFailedEvent == eventid )
    {
        CPingTaskConsumer::CEchoReceivedEventData* taskData = static_cast< CPingTaskConsumer::CEchoReceivedEventData* >( eventdata );
        CPingEventData eventData( taskData->GetHostAddress(), taskData->GetEchoSize(), taskData->GetRoundTripTime() );
        NotifyObservers( PingFailedEvent, &eventData );
    }
    else
    if ( CPingTaskConsumer::PingStoppedEvent == eventid )
    {
        CPingTaskConsumer::CEchoReceivedEventData* taskData = static_cast< CPingTaskConsumer::CEchoReceivedEventData* >( eventdata );
        CPingEventData eventData( taskData->GetHostAddress(), taskData->GetEchoSize(), taskData->GetRoundTripTime() );
        NotifyObservers( PingStoppedEvent, &eventData );
    }        
}

/*-------------------------------------------------------------------------*/
    
bool
CPing::IsActive( void ) const
{GUCEF_TRACE;
    
    return m_isActive;
}

/*-------------------------------------------------------------------------*/
    
const CPing::TStringVector&
CPing::GetRemoteHosts( void ) const
{GUCEF_TRACE;

    return m_remoteHosts;
}

/*-------------------------------------------------------------------------*/
    
Int32
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