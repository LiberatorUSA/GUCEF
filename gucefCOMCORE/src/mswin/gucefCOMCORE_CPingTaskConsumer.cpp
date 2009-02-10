/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
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

#ifndef GUCEF_MT_DVMTOSWRAP_H
#include "gucefMT_dvmtoswrap.h"
#define GUCEF_MT_DVMTOSWRAP_H
#endif /* GUCEF_MT_DVMTOSWRAP_H ? */

#ifndef GUCEF_CORE_CTASKDELEGATOR_H
#include "gucefCORE_CTaskDelegator.h"
#define GUCEF_CORE_CTASKDELEGATOR_H
#endif /* GUCEF_CORE_CTASKDELEGATOR_H ? */

#ifndef GUCEF_COMCORE_ICMPAPI_H
#include "gucefCOMCORE_icmpApi.h"
#define GUCEF_COMCORE_ICMPAPI_H
#endif /* GUCEF_COMCORE_ICMPAPI_H ? */

#ifndef GUCEF_COMCORE_CPING_H
#include "CPing.h"
#define GUCEF_COMCORE_CPING_H
#endif /* GUCEF_COMCORE_CPING_H ? */

#include "gucefCOMCORE_CPingTaskConsumer.h"

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

const CORE::CEvent CPingTaskConsumer::PingStartedEvent = "GUCEF::COMCORE::CPingTaskConsumer::PingStartedEvent";
const CORE::CEvent CPingTaskConsumer::PingReponseEvent = "GUCEF::COMCORE::CPingTaskConsumer::PingReponseEvent";
const CORE::CEvent CPingTaskConsumer::PingTimeoutEvent = "GUCEF::COMCORE::CPingTaskConsumer::PingTimeoutEvent";
const CORE::CEvent CPingTaskConsumer::PingFailedEvent = "GUCEF::COMCORE::CPingTaskConsumer::PingFailedEvent";
const CORE::CEvent CPingTaskConsumer::PingStoppedEvent = "GUCEF::COMCORE::CPingTaskConsumer::PingStoppedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CPingTaskConsumer::RegisterEvents( void )
{GUCEF_TRACE;

    PingStartedEvent.Initialize();
    PingReponseEvent.Initialize();
    PingTimeoutEvent.Initialize();
    PingFailedEvent.Initialize();
    PingStoppedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CPingTaskConsumer::CPingTaskData::CPingTaskData( const TStringVector& remoteHosts ,
                                                 const Int32 maxPings             ,
                                                 const UInt32 bytesToSend         ,
                                                 const UInt32 timeout             ,
                                                 const Int32 minimalPingDelta     )
    : CICloneable()                          ,
      m_hostList( remoteHosts )              ,
      m_maxPings( maxPings )                 ,
      m_bytesToSend( bytesToSend )           ,
      m_timeout( timeout )                   ,
      m_minimalPingDelta( minimalPingDelta )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
        
CPingTaskConsumer::CPingTaskData::CPingTaskData( const CPingTaskData& src )
    : CICloneable( src )                           ,
      m_hostList( src.m_hostList )                 ,
      m_maxPings( src.m_maxPings )                 ,
      m_bytesToSend( src.m_bytesToSend )           ,
      m_timeout( src.m_timeout )                   ,
      m_minimalPingDelta( src.m_minimalPingDelta )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
        
CORE::CICloneable*
CPingTaskConsumer::CPingTaskData::Clone( void ) const
{GUCEF_TRACE;
    
    return new CPingTaskData( *this );
}

/*-------------------------------------------------------------------------*/
        
void
CPingTaskConsumer::CPingTaskData::SetRemoteHosts( const TStringVector& hostList )
{GUCEF_TRACE;

    m_hostList = hostList;
}

/*-------------------------------------------------------------------------*/
        
const CPingTaskConsumer::TStringVector&
CPingTaskConsumer::CPingTaskData::GetRemoteHosts( void ) const
{GUCEF_TRACE;

    return m_hostList;
}

/*-------------------------------------------------------------------------*/
        
void
CPingTaskConsumer::CPingTaskData::SetMaxPings( const Int32 maxPings )
{GUCEF_TRACE;

    m_maxPings = maxPings;
}

/*-------------------------------------------------------------------------*/
        
Int32
CPingTaskConsumer::CPingTaskData::GetMaxPings( void ) const
{GUCEF_TRACE;

    return m_maxPings;
}

/*-------------------------------------------------------------------------*/
        
void
CPingTaskConsumer::CPingTaskData::SetBytesToSend( const UInt32 bytesToSend )
{GUCEF_TRACE;

    m_bytesToSend = bytesToSend;
}
        
/*-------------------------------------------------------------------------*/

UInt32
CPingTaskConsumer::CPingTaskData::GetBytesToSend( void ) const
{GUCEF_TRACE;

    return m_bytesToSend;
}

/*-------------------------------------------------------------------------*/
        
void
CPingTaskConsumer::CPingTaskData::SetTimeout( const UInt32 timeout )
{GUCEF_TRACE;

    m_timeout = timeout;
}
        
/*-------------------------------------------------------------------------*/

UInt32
CPingTaskConsumer::CPingTaskData::GetTimeout( void ) const
{GUCEF_TRACE;

    return m_timeout;
}

/*-------------------------------------------------------------------------*/
        
void
CPingTaskConsumer::CPingTaskData::SetMinimalPingDelta( const UInt32 minimalPingDelta )
{GUCEF_TRACE;

    m_minimalPingDelta = minimalPingDelta;
}
        
/*-------------------------------------------------------------------------*/        

UInt32
CPingTaskConsumer::CPingTaskData::GetMinimalPingDelta( void ) const
{GUCEF_TRACE;

    return m_minimalPingDelta;
}

/*-------------------------------------------------------------------------*/
    
CPingTaskConsumer::CEchoReceivedEventData::CEchoReceivedEventData( const CHostAddress& host   ,
                                                                   const UInt32 echoSize      ,
                                                                   const UInt32 roundTripTime )
    : CORE::CICloneable()              ,
      m_hostAddress( host )            ,
      m_echoSize( echoSize )           ,
      m_roundTripTime( roundTripTime )
{GUCEF_TRACE;
    
}

/*-------------------------------------------------------------------------*/

CPingTaskConsumer::CEchoReceivedEventData::CEchoReceivedEventData( const CEchoReceivedEventData& src )
    : CORE::CICloneable( src)                ,
      m_hostAddress( src.m_hostAddress )     ,
      m_echoSize( src.m_echoSize )           ,
      m_roundTripTime( src.m_roundTripTime )
{GUCEF_TRACE;    

}

/*-------------------------------------------------------------------------*/
        
CPingTaskConsumer::CEchoReceivedEventData::~CEchoReceivedEventData()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
                
CORE::CICloneable*
CPingTaskConsumer::CEchoReceivedEventData::Clone( void ) const
{GUCEF_TRACE;

    return new CEchoReceivedEventData( *this );
}

/*-------------------------------------------------------------------------*/
        
const CHostAddress&
CPingTaskConsumer::CEchoReceivedEventData::GetHostAddress( void ) const
{GUCEF_TRACE;

    return m_hostAddress;
}

/*-------------------------------------------------------------------------*/
        
UInt32
CPingTaskConsumer::CEchoReceivedEventData::GetEchoSize( void ) const
{GUCEF_TRACE;

    return m_echoSize;
}

/*-------------------------------------------------------------------------*/
        
UInt32
CPingTaskConsumer::CEchoReceivedEventData::GetRoundTripTime( void ) const
{GUCEF_TRACE;

    return m_roundTripTime;
}

/*-------------------------------------------------------------------------*/

CPingTaskConsumer::CPingTaskConsumer( void )
    : CTaskConsumer()      ,
      m_pingEvent( NULL )  ,
      m_icmpHandle( NULL ) ,
      m_notDone( true )    ,
      m_taskData( NULL )   ,
      m_pingCounters()
{GUCEF_TRACE;

    m_icmpHandle = IcmpCreateFile();    
    m_pingEvent = CreateEvent( NULL, FALSE, FALSE, NULL );    
    
    RegisterEvents();
}

/*-------------------------------------------------------------------------*/
    
CPingTaskConsumer::~CPingTaskConsumer()
{GUCEF_TRACE;

    CloseHandle( m_pingEvent );
    IcmpCloseHandle( m_icmpHandle );
}

/*-------------------------------------------------------------------------*/

const CString&
CPingTaskConsumer::GetTypeString( void )
{GUCEF_TRACE;

    static CString typeName = "PingTask";
    return typeName;
}
    
/*-------------------------------------------------------------------------*/

CString
CPingTaskConsumer::GetType( void ) const
{GUCEF_TRACE;

    return GetTypeString();
}

/*-------------------------------------------------------------------------*/
    
const CString&
CPingTaskConsumer::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CString typeName = "GUCEF::COMCORE::CPingTaskConsumer";
    return typeName;
}

/*-------------------------------------------------------------------------*/

void
CPingTaskConsumer::IcmpCallback( void* vdata )
{GUCEF_TRACE;

    TIcmpCallbackData* data = static_cast< TIcmpCallbackData* >( vdata );
    
    UInt32 replyCount = IcmpParseReplies( data->replyBuffer, sizeof(ICMP_ECHO_REPLY) + data->echoSize ); 
    PICMP_ECHO_REPLY echoReply = (PICMP_ECHO_REPLY) data->replyBuffer;
    
    if ( echoReply->Status == IP_SUCCESS )
    {
        CEchoReceivedEventData eventData( *data->host, data->echoSize, echoReply->RoundTripTime );
        data->taskConsumer->NotifyObservers( PingReponseEvent, &eventData );
    }
    else
    if ( ( echoReply->Status == IP_REQ_TIMED_OUT )       ||
         ( echoReply->Status == IP_TTL_EXPIRED_TRANSIT ) ||
         ( echoReply->Status == IP_TTL_EXPIRED_REASSEM )  )
    {
        CEchoReceivedEventData eventData( *data->host, data->echoSize, echoReply->RoundTripTime );
        data->taskConsumer->NotifyObservers( PingTimeoutEvent, &eventData );        
    }
    else
    {
        CEchoReceivedEventData eventData( *data->host, data->echoSize, echoReply->RoundTripTime );
        data->taskConsumer->NotifyObservers( PingFailedEvent, &eventData );
    }
}

/*-------------------------------------------------------------------------*/
    
bool
CPingTaskConsumer::ProcessTask( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    m_taskData = static_cast< CPingTaskData* >( taskData );
    m_notDone = true;
    
    // Prepare our ping packet
    UInt8* pingData = new UInt8[ m_taskData->GetBytesToSend() ];
    memset( pingData, 'x', m_taskData->GetBytesToSend() );
        
    // Setup all ping counters
    const TStringVector& hosts = m_taskData->GetRemoteHosts();
    for ( UInt32 i=0; i<hosts.size(); ++i )
    {
        CHostAddress host;
        host.SetHostname( hosts[ i ] );
        
        TPingEntry pingEntry;
        pingEntry.pingCount = 0;
        pingEntry.ticksAtLastPing = MT::PrecisionTickCount();
        pingEntry.callbackData.echoSize = m_taskData->GetBytesToSend();
        pingEntry.callbackData.replyBuffer = new Int8[ sizeof(ICMP_ECHO_REPLY) + pingEntry.callbackData.echoSize ];
        pingEntry.callbackData.taskConsumer = this;
        
        m_pingCounters.insert( std::pair< CHostAddress, TPingEntry >( host, pingEntry ) );
        TPingCounters::iterator n = m_pingCounters.find( host );
        (*n).second.callbackData.host = &(*n).first;
    }
    
    // Now we have to stick around untill all pings are done or this task is cancelled
    CORE::CTaskDelegator* delegator = GetTaskDelegator();
    while ( m_notDone && delegator->IsActive() )
    {
        m_notDone = false;
        TPingCounters::iterator i = m_pingCounters.begin();
        while ( i != m_pingCounters.end() && delegator->IsActive() )
        {
            TPingEntry& pingEntry = (*i).second;
            
            // Make sure we don't ping a host to often
            if ( m_taskData->GetMaxPings() < 1 || pingEntry.pingCount < (UInt32)m_taskData->GetMaxPings() )
            {
                m_notDone = true;
                
                // Make sure we wait the desired time before sending out another ping
                if ( (UInt32)( MT::PrecisionTickCount() - pingEntry.ticksAtLastPing ) > m_taskData->GetMinimalPingDelta() )
                {
                    if ( pingEntry.pingCount == 0 )
                    {
                        // This is the first time we are pinging this host, send out a ping start event
                        CEchoReceivedEventData eventData( *pingEntry.callbackData.host, pingEntry.callbackData.echoSize, 0 );
                        NotifyObservers( PingStartedEvent, &eventData );
                    }
                                            
                    IPAddr netIp;
                    netIp.S_un.S_addr = pingEntry.callbackData.host->GetAddress();
                    
                    DWORD result = IcmpSendEcho2( m_icmpHandle                                           ,
                                                  m_pingEvent                                            ,
                                                  &IcmpCallback                                          ,
                                                  &pingEntry.callbackData                                ,
                                                  netIp                                                  ,
                                                  pingData                                               ,
                                                  (WORD) m_taskData->GetBytesToSend()                    ,
                                                  NULL                                                   ,
                                                  pingEntry.callbackData.replyBuffer                     ,
                                                  sizeof(ICMP_ECHO_REPLY) + m_taskData->GetBytesToSend() ,
                                                  m_taskData->GetTimeout()                               );
                                                  
                    if ( ERROR_IO_PENDING == result || ERROR_SUCCESS == result )
                    {                                   
                        ++pingEntry.pingCount;
                        pingEntry.ticksAtLastPing = MT::PrecisionTickCount();
                        
                        if ( pingEntry.pingCount == (UInt32)m_taskData->GetMaxPings() )
                        {
                            // We reached the max number of pings for this host,.. send a ping stopped event
                            CEchoReceivedEventData eventData( *pingEntry.callbackData.host, pingEntry.callbackData.echoSize, 0 );
                            NotifyObservers( PingStoppedEvent, &eventData );                            
                        }
                    }
                    else
                    {
                        ++pingEntry.pingCount;
                        pingEntry.ticksAtLastPing = MT::PrecisionTickCount();

                        // We did not get the expected return value,.. something went wrong
                        CEchoReceivedEventData eventData( *pingEntry.callbackData.host, pingEntry.callbackData.echoSize, 0 );
                        NotifyObservers( PingFailedEvent, &eventData );
                    }
                }
            }            
            ++i;
        }
        
        // Idle a bit before we try another round of pings (yield the thread)
        if ( delegator->IsActive() )
        {
            MT::ThreadDelay( 10 );
        }
    }
    
    // Cleanup all ping counters
    TPingCounters::iterator i = m_pingCounters.begin();
    while ( i != m_pingCounters.end() )
    {
        TPingEntry& pingEntry = (*i).second;
        delete[] pingEntry.callbackData.replyBuffer;
        ++i;        
    }
    m_pingCounters.clear();
    
    return true;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 29-12-2006 :
        - Dinand: Initial version

----------------------------------------------------------------------------*/
