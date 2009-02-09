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

#ifndef GUCEF_COMCORE_CHOSTADDRESS_H
#include "CHostAddress.h"
#define GUCEF_COMCORE_CHOSTADDRESS_H
#endif /* GUCEF_COMCORE_CHOSTADDRESS_H ? */

#ifndef GUCEF_COMCORE_ICMPAPI_H
#include "gucefCOMCORE_icmpApi.h"
#define GUCEF_COMCORE_ICMPAPI_H
#endif /* GUCEF_COMCORE_ICMPAPI_H ? */

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
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

struct SIcmpCallbackData
{
    CPingTaskConsumer* taskConsumer;
    CHostAddress host;
    void* replyBuffer;
    UInt32 echoSize;
};
typedef struct SIcmpCallbackData TIcmpCallbackData;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/
    
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
      m_pulseGenerator()   ,
      m_pulseDriver()      ,
      m_pingEvent( NULL )  ,
      m_icmpHandle( NULL ) ,
      m_notDone( true )    ,
      m_taskData( NULL )
{GUCEF_TRACE;

    m_icmpHandle = TIcmpCreateFilePtr();    
    m_pingEvent = CreateEvent( NULL, FALSE, FALSE, NULL );
    m_pulseGenerator.SetPulseGeneratorDriver( &m_pulseDriver );    
}

/*-------------------------------------------------------------------------*/
    
CPingTaskConsumer::~CPingTaskConsumer()
{GUCEF_TRACE;

    CloseHandle( m_pingEvent );
    IcmpCloseHandle( m_icmpHandle );
}

/*-------------------------------------------------------------------------*/
    
const CString&
CPingTaskConsumer::GetTaskName( void ) const
{GUCEF_TRACE;

    static CString taskType = "PingTask";
    return taskType;
}
    
/*-------------------------------------------------------------------------*/

CString
CPingTaskConsumer::GetType( void ) const
{GUCEF_TRACE;

    static CString typeName = "GUCEF::COMCORE::CPingTaskConsumer";
    return typeName;
}

/*-------------------------------------------------------------------------*/
    
const CString&
CPingTaskConsumer::GetTypeString( void )
{GUCEF_TRACE;

    static CString typeName = "GUCEF::COMCORE::CPingTaskConsumer";
    return typeName;
}

/*-------------------------------------------------------------------------*/

void
CPingTaskConsumer::IcmpCallback( void* vdata )
{
    TIcmpCallbackData* data = static_cast< TIcmpCallbackData* >( vdata );
    
    UInt32 replyCount = IcmpParseReplies( data->replyBuffer, sizeof(ICMP_ECHO_REPLY) + data->echoSize ); 
    PICMP_ECHO_REPLY echoReply = (PICMP_ECHO_REPLY) data->replyBuffer;
    //echoReply->RoundTripTime
    
}

/*-------------------------------------------------------------------------*/
    
bool
CPingTaskConsumer::ProcessTask( CORE::CICloneable* taskData )
{GUCEF_TRACE;

    m_taskData = static_cast< CPingTaskData* >( taskData );
  
    UInt8* pingData = new UInt8[ m_taskData->GetBytesToSend() ];
    memset( pingData, 0, m_taskData->GetBytesToSend() );
    m_notDone = true;
    
    const TStringVector& hosts = m_taskData->GetRemoteHosts();
    for ( UInt32 i=0; i<hosts.size(); ++i )
    {
        TIcmpCallbackData* callbackdata = new TIcmpCallbackData;
        callbackdata.host.SetHostname( hosts[ i ] );
        callbackdata.taskConsumer = this;
        callbackdata.echoSize = m_taskData->GetBytesToSend();
        callbackdata.replyBuffer = new Int8[ sizeof(ICMP_ECHO_REPLY) + callbackdata.echoSize ];
        
        in_addr netIp;
        netIp.S_addr = callbackdata.host.GetAddress();
        
        IcmpSendEcho2( icmpHandle                                             ,
                       pingEvent                                              ,
                       &IcmpCallback                                          ,
                       callbackdata                                           ,
                       netIp                                                  ,
                       pingData                                               ,
                       theTaskData->GetBytesToSend()                          ,
                       NULL                                                   ,
                       callbackdata->replyBuffer                              ,
                       sizeof(ICMP_ECHO_REPLY) + m_taskData->GetBytesToSend() ,
                       m_taskData->GetTimeout()                               );
                           
    }
    
    // Now we have to stick around untill all pings are done or this task is cancelled
    CTaskDelegator* delegator = GetTaskDelegator();
    while ( m_notDone && delegator->IsActive() )
    {
        ThreadDelay( 10 );
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COMCORE_CPINGTASKCONSUMER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 29-12-2006 :
        - Dinand: Initial version

----------------------------------------------------------------------------*/
