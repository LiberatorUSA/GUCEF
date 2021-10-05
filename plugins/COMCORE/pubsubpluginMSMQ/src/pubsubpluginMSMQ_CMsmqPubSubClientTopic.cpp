/*
 *  pubsubpluginMSMQ: Generic GUCEF COMCORE plugin for providing pubsub via Microsoft MSMQ
 *
 *  Copyright (C) 1998 - 2020.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <Objbase.h>
#include <Mq.h>

#include <string>

#ifndef GUCEF_MT_CSCOPEMUTEX_H
#include "gucefMT_CScopeMutex.h"
#define GUCEF_MT_CSCOPEMUTEX_H
#endif /* GUCEF_MT_CSCOPEMUTEX_H ? */

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H */

#ifndef GUCEF_CORE_METRICSMACROS_H
#include "gucefCORE_MetricsMacros.h"
#define GUCEF_CORE_METRICSMACROS_H
#endif /* GUCEF_CORE_METRICSMACROS_H ? */

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H */

#ifndef PUBSUBPLUGIN_MSMQ_CMSMQPUBSUBCLIENT_H
#include "pubsubpluginMSMQ_CMsmqPubSubClient.h"
#define PUBSUBPLUGIN_MSMQ_CMSMQPUBSUBCLIENT_H
#endif /* PUBSUBPLUGIN_MSMQ_CMSMQPUBSUBCLIENT_H ? */

#include "pubsubpluginMSMQ_CMsmqPubSubClientTopic.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUBPLUGIN {
namespace MSMQ {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

std::wstring 
RetrieveWin32APIErrorMessage( DWORD dwErr )
{
    // from: https://docs.microsoft.com/en-us/windows/win32/seccrypto/retrieving-error-messages

    WCHAR   wszMsgBuff[512];  // Buffer for text.

    DWORD   dwChars;  // Number of chars returned.

    // Try to get the message from the system errors.
    dwChars = ::FormatMessageW( FORMAT_MESSAGE_FROM_SYSTEM |
                                FORMAT_MESSAGE_IGNORE_INSERTS,
                                NULL,
                                dwErr,
                                0,
                                wszMsgBuff,
                                512,
                                NULL );

    if (0 == dwChars)
    {
        // The error code did not exist in the system errors.
        // Try Ntdsbmsg.dll for the error code.

        HINSTANCE hInstNtdsbmsg = NULL;

        // Load the library.
        hInstNtdsbmsg = ::LoadLibraryA( "Ntdsbmsg.dll" );
        if ( NULL == hInstNtdsbmsg )
        {
            // cannot load Ntdsbmsg.dll for refinement
            return std::wstring();
        }

        // Try getting message text from ntdsbmsg.
        dwChars = FormatMessageW( FORMAT_MESSAGE_FROM_HMODULE |
                                 FORMAT_MESSAGE_IGNORE_INSERTS,
                                 hInstNtdsbmsg,
                                 dwErr,
                                 0,
                                 wszMsgBuff,
                                 512,
                                 NULL );

        // Free the library.
        ::FreeLibrary( hInstNtdsbmsg );

    }

    return wszMsgBuff;
}

/*-------------------------------------------------------------------------*/

CMsmqPubSubClientTopic::CMsmqPubSubClientTopic( CMsmqPubSubClient* client )
    : COMCORE::CPubSubClientTopic()
    , m_client( client )
    , m_msgSendMsg()
    , m_config()
    , m_msmqQueueFormatName()
    , m_syncReadTimer( GUCEF_NULL )
    , m_msmqMsgsQueued( -1 )
    , m_lock()
    , m_receiveQueueHandle( GUCEF_NULL )
    , m_sendQueueHandle( GUCEF_NULL )
    , m_currentPublishActionId( 1 )
    , m_currentReceiveActionId( 1 )
    , m_publishSuccessActionIds()
    , m_publishSuccessActionEventData()
    , m_publishFailureActionIds()
    , m_publishFailureActionEventData()
{GUCEF_TRACE;
        
    m_publishSuccessActionEventData.LinkTo( &m_publishSuccessActionIds );
    m_publishFailureActionEventData.LinkTo( &m_publishFailureActionIds );

    m_syncReadTimer = new CORE::CTimer( m_client->GetConfig().pulseGenerator, 25 );

    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

CMsmqPubSubClientTopic::~CMsmqPubSubClientTopic()
{GUCEF_TRACE;

    Disconnect();

    delete m_syncReadTimer;
    m_syncReadTimer = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

void
CMsmqPubSubClientTopic::RegisterEventHandlers( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_syncReadTimer )
    {
        TEventCallback callback( this, &CMsmqPubSubClientTopic::OnSyncReadTimerCycle );
        SubscribeTo( m_syncReadTimer                ,
                     CORE::CTimer::TimerUpdateEvent ,
                     callback                       );
    }

    TEventCallback callback( this, &CMsmqPubSubClientTopic::OnPulseCycle );
    SubscribeTo( m_client->GetConfig().pulseGenerator ,
                 CORE::CPulseGenerator::PulseEvent    ,
                 callback                             );
}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClientTopic::IsPublishingSupported( void )
{GUCEF_TRACE;
    
    return m_config.needPublishSupport;
}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClientTopic::IsSubscribingSupported( void )
{GUCEF_TRACE;

    return m_config.needSubscribeSupport;
}

/*-------------------------------------------------------------------------*/

const CORE::CString& 
CMsmqPubSubClientTopic::GetTopicName( void ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return m_config.topicName;
}

/*-------------------------------------------------------------------------*/

bool 
CMsmqPubSubClientTopic::Publish( CORE::UInt64& publishActionId, const COMCORE::CIPubSubMsg& msg, bool notify )
{GUCEF_TRACE;

    publishActionId = 0;
    MT::CScopeMutex lock( m_lock );
    
    // Each Message Queuing message can have no more than 4 MB of data.
    // We cannot do anything with a DateTime already set on the message since MSMQ fully controls the timestamps

    // Do we want to send a body per config?
    MSGPROPIDToUInt32Map::iterator n = m_msgSendPropMap.find( PROPID_M_BODY );
    if ( n != m_msgSendPropMap.end() )
    {
        // Do we have a body to send?
        const CORE::CVariant& primaryPayload = msg.GetPrimaryPayload();
        
        bool variantConversionSuccess = false; 
        if ( primaryPayload.IsInitialized() )
            variantConversionSuccess = VariantToMsmqProperty( PROPID_M_BODY, primaryPayload, m_msgSendMsg.aMsgPropVar[ (*n).second ] );
        else
            variantConversionSuccess = VariantToMsmqProperty( PROPID_M_BODY, CORE::CVariant::Empty, m_msgSendMsg.aMsgPropVar[ (*n).second ] );

        if ( !variantConversionSuccess )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "MsmqPubSubClientTopic:Publish: Cannot translate IPubSubMsg primary payload to a MSMQ BODY" ); 
            return false;
        }

        SetUInt32OnPropertyVariant( PROPID_M_BODY_SIZE, primaryPayload.ByteSize(), m_msgSendMsg );
    }
    
    const COMCORE::CIPubSubMsg::TKeyValuePairs& kvPairs = msg.GetKeyValuePairs();
    //@TODO: Find a way to handle these driven by config

    const COMCORE::CIPubSubMsg::TKeyValuePairs& metaDataKvPairs = msg.GetMetaDataKeyValuePairs();
    COMCORE::CIPubSubMsg::TKeyValuePairs::const_iterator i = metaDataKvPairs.begin();
    while ( i != metaDataKvPairs.end() )
    {
        MSGPROPID propId = (MSGPROPID) (*i).first.AsUInt32();
        MSGPROPIDToUInt32Map::iterator n = m_msgSendPropMap.find( propId );
        if ( n != m_msgSendPropMap.end() )
        {
            if ( !VariantToMsmqProperty( propId, (*i).second, m_msgSendMsg.aMsgPropVar[ (*n).second ] ) ) 
            {   
                // Cannot map this property's value
                if ( !m_config.ignoreUnmappableMetaDataFieldOnPublish )
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "MsmqPubSubClientTopic:Publish: Cannot translate IPubSubMsg meta-data value to a configured MSMQ VARIANT. MSGPROPID=" + CORE::ToString( (CORE::UInt32) propId ) ); 
                    return false;
                }
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "MsmqPubSubClientTopic:Publish: Cannot translate IPubSubMsg meta-data value to a configured MSMQ VARIANT. Ignoring it. MSGPROPID=" + CORE::ToString( (CORE::UInt32) propId ) ); 
            }
        }
        else
        {
            // Cannot map this property
            
            if ( !m_config.ignoreUnmappableMetaDataFieldOnPublish )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "MsmqPubSubClientTopic:Publish: Cannot map IPubSubMsg meta-data key to a configured MSMQ field. MSGPROPID=" + CORE::ToString( (CORE::UInt32) propId ) ); 
                return false;
            }
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "MsmqPubSubClientTopic:Publish: Cannot map IPubSubMsg meta-data key to a configured MSMQ field. Ignoring it. MSGPROPID=" + CORE::ToString( (CORE::UInt32) propId ) ); 
        }
        ++i;
    }

    // Nothing wrong with msg, we will make the publish attempt
    publishActionId = m_currentPublishActionId; 
    ++m_currentPublishActionId;
    
    bool success = false;
    // @TODO: Code the actual publish to MSMQ, not implemented yet

    if ( notify )
    {
        if ( success )
            m_publishSuccessActionIds.push_back( publishActionId );
        else
            m_publishFailureActionIds.push_back( publishActionId );
    }
    return success;
}

/*-------------------------------------------------------------------------*/

bool 
CMsmqPubSubClientTopic::SetupToSubscribe( COMCORE::CPubSubClientTopicConfig& config )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    m_config = config;
    PrepStorageForReadMsgs( m_config.maxMsmqMsgsToReadPerSyncCycle );    
    m_syncReadTimer->SetEnabled( true );
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CMsmqPubSubClientTopic::SaveConfig( COMCORE::CPubSubClientTopicConfig& config ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    config = m_config;
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CMsmqPubSubClientTopic::LoadConfig( const COMCORE::CPubSubClientTopicConfig& config )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    m_config = config;

    return true;
}

/*-------------------------------------------------------------------------*/

void
CMsmqPubSubClientTopic::PrepStorageForReadMsgs( CORE::UInt32 msgCount )
{GUCEF_TRACE;

    // reset size, note this does not dealloc the underlying memory
    TPubSubMsgsRefVector& msgRefs = m_pubsubMsgsRefs.msgs;
    msgRefs.clear();

    if ( msgCount > msgRefs.capacity() )
    {
        // Grow the actual storage, this is allowed to become larger than msgCount to limit
        // the nr of dynamic allocations
        msgRefs.reserve( msgCount );
    }

    if ( msgCount > m_pubsubMsgs.size() )
    {
        // Grow the actual storage plus count. These slots are pre-reserved to limit the nr of dynamic allocations
        // This helps make the code more performant
        m_pubsubMsgs.resize( msgCount );        
        m_msmqReceiveMsgs.resize( msgCount );
        PrepMsmqMsgsStorage();
    }
}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClientTopic::Disconnect( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    if ( GUCEF_NULL != m_receiveQueueHandle )
    {
        ::MQCloseQueue( m_receiveQueueHandle );
        m_receiveQueueHandle = GUCEF_NULL;
    }
    if ( GUCEF_NULL != m_sendQueueHandle )
    {
        ::MQCloseQueue( m_sendQueueHandle );
        m_sendQueueHandle = GUCEF_NULL;
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClientTopic::MsmqPathNameToMsmqQueueFormatName( const std::wstring& pathName   ,
                                                           std::wstring& queueFormatName  )
{GUCEF_TRACE;

    DWORD maxFormatNameSize = 512;
    WCHAR queueFormatNameTmp[ 512 ]; 
    HRESULT formatConversionResult = ::MQPathNameToFormatName( pathName.c_str()    ,    
                                                               queueFormatNameTmp  ,    
                                                               &maxFormatNameSize  );
    if ( !FAILED( formatConversionResult ) )
    {
        queueFormatName = queueFormatNameTmp;
        return true;
    }
    else
    {
        CORE::UInt32 errorCode =  HRESULT_CODE( formatConversionResult );
        std::wstring errMsg = RetrieveWin32APIErrorMessage( HRESULT_CODE( formatConversionResult ) );
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "MsmqPubSubClientTopic:MsmqPathNameToMsmqQueueFormatName: Failed to convert path to MSMQ format name. errorCode= " + CORE::ToString( errorCode ) + ". Error msg: " + CORE::ToString( errMsg ) ) ; 
        return false;
    }
}

/*-------------------------------------------------------------------------*/

const std::wstring&
CMsmqPubSubClientTopic::GetMsmqQueueFormatName( void ) const
{
    if ( !m_msmqQueueFormatName.empty() )
        return m_msmqQueueFormatName;

    std::wstring wQueueName;
    CORE::Utf8toUtf16( m_config.topicName, wQueueName );

    if ( !m_config.topicNameIsMsmqFormatName )
    {
        std::wstring wQueueFormatName;
        if ( MsmqPathNameToMsmqQueueFormatName( wQueueName, wQueueFormatName ) )
        {
            m_msmqQueueFormatName = wQueueFormatName;
        }
    }
    else
    {
        m_msmqQueueFormatName = wQueueName;
    }

    return m_msmqQueueFormatName; 
}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClientTopic::Subscribe( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    if ( GUCEF_NULL != m_receiveQueueHandle )
    {
        ::MQCloseQueue( m_receiveQueueHandle );
        m_receiveQueueHandle = GUCEF_NULL;
    }

    // MQOpenQueue only supports a format name
    // As such depending on how the setting is presented we need to translate it
    const std::wstring& queueFormatName = GetMsmqQueueFormatName();
    if ( queueFormatName.empty() )
        return false;

    ///* MQOpenQueue() access flags */
    //#define MQ_RECEIVE_ACCESS   0x00000001L
    //#define MQ_SEND_ACCESS      0x00000002L
    //#define MQ_MOVE_ACCESS      0x00000004L
    //#define MQ_PEEK_ACCESS      0x00000020L
    //#define MQ_ADMIN_ACCESS     0x00000080L

    HRESULT openQueueResult = ::MQOpenQueue( queueFormatName.c_str() ,  // IN:  Queue format name
                                             MQ_RECEIVE_ACCESS       ,  // IN:  Want to receive from queue
                                             0                       ,  // IN:  Allow sharing
                                             &m_receiveQueueHandle   ); // OUT: Handle of open queue

    if ( FAILED( openQueueResult ) )
    {
        CORE::UInt32 errorCode =  HRESULT_CODE( openQueueResult );
        std::wstring errMsg = RetrieveWin32APIErrorMessage( HRESULT_CODE( openQueueResult ) );
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "MsmqPubSubClientTopic:Subscribe: Failed to subscribe by opening a MSMQ queue for recieving. Topic Name: " + m_config.topicName +". errorCode= " + CORE::ToString( errorCode ) + ". Error msg: " + CORE::ToString( errMsg ) );
        return false;
    }

    PrepStorageForReadMsgs( m_config.maxMsmqMsgsToReadPerSyncCycle );
    m_syncReadTimer->SetEnabled( true );
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CMsmqPubSubClientTopic::SubscribeStartingAtMsgId( const CORE::CVariant& msgIdBookmark )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    // Currently not supported.
    // Could theoretically be implemented using a cursor combined with PEEK only reads
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CMsmqPubSubClientTopic::SubscribeStartingAtMsgIndex( const CORE::CVariant& msgIndexBookmark )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    // Currently not supported.
    // Could theoretically be implemented using a cursor?
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClientTopic::SubscribeStartingAtMsgDateTime( const CORE::CDateTime& msgDtBookmark )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    // Currently not supported.
    // Could theoretically be implemented using a cursor combined with PEEK only reads
    // For regular queue consuming reads you could perhaps ignore messages with a send time older then msgDtBookmark
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CMsmqPubSubClientTopic::SubscribeStartingAtBookmark( const COMCORE::CPubSubBookmark& bookmark ) 
{GUCEF_TRACE;

    // Currently not supported.
    return false;
}

/*-------------------------------------------------------------------------*/

COMCORE::CPubSubBookmark 
CMsmqPubSubClientTopic::GetCurrentBookmark( void )
{GUCEF_TRACE;

    // Not supported
    return COMCORE::CPubSubBookmark( COMCORE::CPubSubBookmark::BOOKMARK_TYPE_NOT_APPLICABLE );
}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClientTopic::AcknowledgeReceipt( const COMCORE::CIPubSubMsg& msg )
{GUCEF_TRACE;

    // Not supported
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClientTopic::AcknowledgeReceipt( const COMCORE::CPubSubBookmark& bookmark )
{GUCEF_TRACE;

    // Not supported
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CMsmqPubSubClientTopic::IsConnected( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClientTopic::InitializeConnectivity( void )
{GUCEF_TRACE;

    if ( m_config.needPublishSupport )
    {
        if ( GUCEF_NULL != m_sendQueueHandle )
        {
            ::MQCloseQueue( m_sendQueueHandle );
            m_sendQueueHandle = GUCEF_NULL;
        }

        // MQOpenQueue only supports a format name
        // As such depending on how the setting is presented we need to translate it
        const std::wstring& queueFormatName = GetMsmqQueueFormatName();
        if ( queueFormatName.empty() )
            return false;

        // Opening the handle counts as our 'connectivity'
        HRESULT openQueueResult = ::MQOpenQueue( queueFormatName.c_str() ,  // IN:  Queue format name
                                                 MQ_SEND_ACCESS          ,  // IN:  Want to send to queue
                                                 0                       ,  // IN:  Allow sharing
                                                 &m_sendQueueHandle      ); // OUT: Handle of open queue

        if ( FAILED( openQueueResult ) )
        {
            CORE::UInt32 errorCode =  HRESULT_CODE( openQueueResult );
            std::wstring errMsg = RetrieveWin32APIErrorMessage( HRESULT_CODE( openQueueResult ) );
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "MsmqPubSubClientTopic:InitializeConnectivity: Failed to init queue access for publishing. Topic Name: " + m_config.topicName +". errorCode= " + CORE::ToString( errorCode ) + ". Error msg: " + CORE::ToString( errMsg ) );
            return false;
        }
        else
        {
            PrepMsmqMsgsStorage();
        }
    }

    return true;
}

/*-------------------------------------------------------------------------*/

CORE::Int64
CMsmqPubSubClientTopic::GetCurrentNrOfMessagesInQueue( void ) const
{GUCEF_TRACE;

    // MQMgmtGetInfo only supports a format name
    // As such depending on how the setting is presented we need to translate it
    const std::wstring& queueFormatName = GetMsmqQueueFormatName();
    if ( queueFormatName.empty() )
        return -1;

    // Define the required constants and variables.  
    const int NUMBEROFPROPERTIES = 1;  
    DWORD cPropId = 0;  

    // Define an MQMGMTROPS structure.  
    ::MQMGMTPROPS mgmtprops;  
    ::MGMTPROPID aMgmtPropId[ NUMBEROFPROPERTIES ];  
    ::MQPROPVARIANT aMgmtPropVar[ NUMBEROFPROPERTIES ];  

    aMgmtPropId[ cPropId ] = PROPID_MGMT_QUEUE_MESSAGE_COUNT;    // Property identifier  
    aMgmtPropVar[ cPropId ].vt = VT_NULL;                        // Type indicator  
    ++cPropId;

    // Initialize the MQMGMTPROPS structure.  
    mgmtprops.cProp = cPropId;   // number of management properties  
    mgmtprops.aPropID = aMgmtPropId;// management property IDs  
    mgmtprops.aPropVar = aMgmtPropVar;// management property values  
    mgmtprops.aStatus  = NULL;// no storage for error codes   

    // Now that we formulated the request                // @TODO: L"QUEUE=
    // actually ask for the info
    HRESULT queueInfoFetchResult = ::MQMgmtGetInfo( NULL, queueFormatName.c_str(), &mgmtprops );
    if ( MQ_OK == queueInfoFetchResult )
    {
        return (CORE::Int64) aMgmtPropVar[ 0 ].ulVal;
    }

    CORE::UInt32 errorCode =  HRESULT_CODE( queueInfoFetchResult );
    std::wstring errMsg = RetrieveWin32APIErrorMessage( HRESULT_CODE( queueInfoFetchResult ) );
    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "MsmqPubSubClientTopic:GetCurrentNrOfMessagesInQueue: Failed to obain count of queued messages. Topic Name: " + m_config.topicName +". errorCode= " + CORE::ToString( errorCode ) + ". Error msg: " + CORE::ToString( errMsg ) );
    return -1;
}

/*-------------------------------------------------------------------------*/
   
// From: https://stackoverflow.com/questions/45149620/convert-ole-automation-date-oadate-double-to-struct-tm-without-using-variantti
// Slightly altered: used platform types to address ULL type concern

struct tm* SecondsSinceEpochToDateTime(struct tm* pTm, uint64_t SecondsSinceEpoch)
{
   uint64_t sec;
   unsigned int quadricentennials, centennials, quadrennials, annuals/*1-ennial?*/;
   unsigned int year, leap;
   unsigned int yday, hour, min;
   unsigned int month, mday, wday;
   static const unsigned int daysSinceJan1st[2][13]=
   {
      {0,31,59,90,120,151,181,212,243,273,304,334,365}, // 365 days, non-leap
      {0,31,60,91,121,152,182,213,244,274,305,335,366}  // 366 days, leap
  };
/*
   400 years:

   1st hundred, starting immediately after a leap year that's a multiple of 400:
   n n n l  \
   n n n l   } 24 times
   ...      /
   n n n l /
   n n n n

   2nd hundred:
   n n n l  \
   n n n l   } 24 times
   ...      /
   n n n l /
   n n n n

   3rd hundred:
   n n n l  \
   n n n l   } 24 times
   ...      /
   n n n l /
   n n n n

   4th hundred:
   n n n l  \
   n n n l   } 24 times
   ...      /
   n n n l /
   n n n L <- 97'th leap year every 400 years
*/

   // Re-bias from 1970 to 1601:
   // 1970 - 1601 = 369 = 3*100 + 17*4 + 1 years (incl. 89 leap days) =
   // (3*100*(365+24/100) + 17*4*(365+1/4) + 1*365)*24*3600 seconds
   sec = SecondsSinceEpoch + 11644473600ULL;

   wday = (CORE::UInt32)((sec / 86400 + 1) % 7); // day of week

   // Remove multiples of 400 years (incl. 97 leap days)
   quadricentennials = (CORE::UInt32)(sec / CORE::UInt64( 12622780800ULL ) ); // 400*365.2425*24*3600
   sec %= CORE::UInt64( 12622780800ULL );

   // Remove multiples of 100 years (incl. 24 leap days), can't be more than 3
   // (because multiples of 4*100=400 years (incl. leap days) have been removed)
   centennials = (CORE::UInt32)(sec / CORE::UInt64( 3155673600ULL ) ); // 100*(365+24/100)*24*3600
   if (centennials > 3)
   {
      centennials = 3;
   }
   sec -= centennials * CORE::UInt64( 3155673600 );

   // Remove multiples of 4 years (incl. 1 leap day), can't be more than 24
   // (because multiples of 25*4=100 years (incl. leap days) have been removed)
   quadrennials = (CORE::UInt32)(sec / 126230400); // 4*(365+1/4)*24*3600
   if (quadrennials > 24)
   {
      quadrennials = 24;
   }
   sec -= quadrennials * CORE::UInt64( 126230400ULL );

   // Remove multiples of years (incl. 0 leap days), can't be more than 3
   // (because multiples of 4 years (incl. leap days) have been removed)
   annuals = (CORE::UInt32)(sec / 31536000); // 365*24*3600
   if (annuals > 3)
   {
      annuals = 3;
   }
   sec -= annuals * CORE::UInt64( 31536000ULL );

   // Calculate the year and find out if it's leap
   year = 1601 + quadricentennials * 400 + centennials * 100 + quadrennials * 4 + annuals;
   leap = !(year % 4) && (year % 100 || !(year % 400));

   // Calculate the day of the year and the time
   yday = sec / 86400;
   sec %= 86400;
   hour = sec / 3600;
   sec %= 3600;
   min = sec / 60;
   sec %= 60;

   // Calculate the month
   for (mday = month = 1; month < 13; month++)
   {
      if (yday < daysSinceJan1st[leap][month])
      {
         mday += yday - daysSinceJan1st[leap][month - 1];
         break;
      }
   }

   // Fill in C's "struct tm"
   memset(pTm, 0, sizeof(*pTm));
   pTm->tm_sec = sec;          // [0,59]
   pTm->tm_min = min;          // [0,59]
   pTm->tm_hour = hour;        // [0,23]
   pTm->tm_mday = mday;        // [1,31]  (day of month)
   pTm->tm_mon = month - 1;    // [0,11]  (month)
   pTm->tm_year = year - 1900; // 70+     (year since 1900)
   pTm->tm_wday = wday;        // [0,6]   (day since Sunday AKA day of week)
   pTm->tm_yday = yday;        // [0,365] (day since January 1st AKA day of year)
   pTm->tm_isdst = -1;         // daylight saving time flag

   return pTm;
}

/*-------------------------------------------------------------------------*/
   
// From: https://stackoverflow.com/questions/45149620/convert-ole-automation-date-oadate-double-to-struct-tm-without-using-variantti
// Slightly altered, got rid of exception throw

struct tm* fromOADate(struct tm* p_Tm, double p_OADate)
{
   static const int64_t OA_UnixTimestamp = -2209161600; /* 30-Dec-1899 */

   if (!(   -109205 <= p_OADate               /* 01-Jan-1601 */
         &&            p_OADate <= 2958465))  /* 31-Dec-9999 */
   {
      return GUCEF_NULL;
   }

   int64_t OADatePassedDays = p_OADate;
   double  OADateDayTime    = p_OADate - OADatePassedDays;
   int64_t OADateSeconds    = OA_UnixTimestamp
                            + OADatePassedDays * 24LL * 3600LL
                            + OADateDayTime * 24.0 * 3600.0;

   return SecondsSinceEpochToDateTime(p_Tm, OADateSeconds);
}

/*-------------------------------------------------------------------------*/

CORE::Int32
CMsmqPubSubClientTopic::SizeOfVarType( VARTYPE vt )
{GUCEF_TRACE;

    switch ( vt )
    {
        case VT_VECTOR | VT_UI1:
        case VT_UI1:                    { return sizeof( UCHAR ); }
        
        case VT_VECTOR | VT_I2:
        case VT_I2:                     { return sizeof( SHORT ); }
        
        case VT_VECTOR | VT_UI2:
        case VT_UI2:                    { return sizeof( USHORT ); }

        case VT_VECTOR | VT_I4:
        case VT_I4:                     { return sizeof( LONG ); }

        case VT_UI4:                    { return sizeof( ULONG ); }
        case VT_UI8:                    { return sizeof( ULONGLONG ); }
        case VT_ERROR:                  { return sizeof( LONG ); }
                
        case VT_VECTOR | VT_BOOL:
        case VT_BOOL:                   { return sizeof( VARIANT_BOOL ); }

        case VT_VECTOR | VT_CLSID:
        case VT_CLSID:                  { return sizeof( CLSID ); }

        case VT_VECTOR | VT_BSTR:
        case VT_BSTR:                   { return sizeof( WCHAR ); }
        
        case VT_LPSTR:                  { return sizeof( CHAR ); }

        case VT_VECTOR | VT_LPWSTR:
        case VT_LPWSTR:                 { return sizeof( WCHAR ); }

        case VT_BLOB:                   { return sizeof( UCHAR ); }        
        case VT_DATE:                   { return sizeof( DATE ); }

        case VT_VECTOR | VT_VARIANT:    { return sizeof( VARIANT ); }
        
        case VT_EMPTY:                  { return 0; }

        default:
        {
            return -1;
        }
    }
}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClientTopic::MsmqPropertyToVariant( MQPROPVARIANT& msmqSourceVariant, CORE::CVariant& targetVariant, bool linkIfPossible, CORE::UInt32 lengthIfApplicable )
{GUCEF_TRACE;

    switch ( msmqSourceVariant.vt )
    {
        case VT_UI1:                    { targetVariant = (CORE::UInt8) msmqSourceVariant.bVal; return true; }
        case VT_I2:                     { targetVariant = (CORE::Int16) msmqSourceVariant.iVal; return true; }
        case VT_UI2:                    { targetVariant = (CORE::UInt16) msmqSourceVariant.uiVal; return true; }
        case VT_I4:                     { targetVariant = (CORE::Int32) msmqSourceVariant.lVal; return true; }
        case VT_UI4:                    { targetVariant = (CORE::UInt32) msmqSourceVariant.ulVal; return true; }
        case VT_UI8:                    { targetVariant = (CORE::UInt64) msmqSourceVariant.uhVal.QuadPart; return true; }
        case VT_ERROR:                  { targetVariant = (CORE::Int32) msmqSourceVariant.scode; return true; }        
        case VT_BOOL:                   { targetVariant = ( VARIANT_FALSE != msmqSourceVariant.boolVal ); return true; } // Actually stored as a short
        
        // The CLSID (GUID) record type is a C structure. The adapter does not need a size field for this record because it is a fixed length field. 
        // It is critical that the data must be exactly sizeof(CLSID) bytes (16) long for proper I/O with MSMQ.
        case VT_CLSID:                  { return targetVariant.Set( msmqSourceVariant.puuid, sizeof(CLSID), GUCEF_DATATYPE_BINARY_BLOB, linkIfPossible ); }

        // Rememeber that like the COM concept the length is actually stored at (BSTR pointer address)-4  
        // Since in the COM world this type is often used to shuttle binary data around we will mark it as a binary and not GUCEF_DATATYPE_UTF16_STRING
        case VT_BSTR:                   { return targetVariant.Set( msmqSourceVariant.pwszVal, SysStringLen( msmqSourceVariant.bstrVal ), GUCEF_DATATYPE_BINARY_BLOB, linkIfPossible ); } 

        case VT_LPSTR:                  { return targetVariant.Set( msmqSourceVariant.pszVal, lengthIfApplicable, GUCEF_DATATYPE_UTF8_STRING, linkIfPossible ); } 
        case VT_LPWSTR:                 { return targetVariant.Set( msmqSourceVariant.pwszVal, lengthIfApplicable, GUCEF_DATATYPE_UTF16_STRING, linkIfPossible ); } 
        case VT_BLOB:                   { return targetVariant.Set( msmqSourceVariant.blob.pBlobData, msmqSourceVariant.blob.cbSize, GUCEF_DATATYPE_BINARY_BLOB, linkIfPossible ); }
        
        case VT_DATE:                   
        { 
            // Sadly date is stored as a OLE Automation DATE which is a pain to convert
            struct tm tmStruct;
            if ( GUCEF_NULL != fromOADate( &tmStruct, msmqSourceVariant.date ) )
            {
                targetVariant = CORE::CDateTime( &tmStruct, true ).ToIso8601DateTimeString( true, true );
                return true;
            } 
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "MsmqPubSubClientTopic:MsmqPropertyToVariant: Unable to convert VT_DATE with value " + CORE::ToString( msmqSourceVariant.date ) );
                return false;             
            }
        }
        
        // We dont have support for a special type for more complex data arrangements like vectors
        // until (if ever) we do, just note these as BINARY data
        case VT_VECTOR | VT_UI1:
        case VT_VECTOR | VT_I2:
        case VT_VECTOR | VT_UI2:
        case VT_VECTOR | VT_BOOL:
        case VT_VECTOR | VT_I4:
        case VT_VECTOR | VT_CLSID:
        case VT_VECTOR | VT_BSTR:
        case VT_VECTOR | VT_LPWSTR:
        case VT_VECTOR | VT_VARIANT:
        {
            // msmqSourceVariant.caub.cElems contains the nr of elements possible in the available buffer not the nr of bytes actually used for elements inside said buffer
            // Hence we need to rely on the "lengthIfApplicable" param which obtains the size from a different property
            return targetVariant.Set( msmqSourceVariant.caub.pElems, lengthIfApplicable, GUCEF_DATATYPE_BINARY_BLOB, linkIfPossible ); 
        }

        case VT_EMPTY:
        {
            targetVariant.Clear();
            return true;
        }

        default:
        {
            targetVariant.Clear();
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "MsmqPubSubClientTopic:MsmqPropertyToVariant: Unable to convert VARIANT of type " + CORE::ToString( msmqSourceVariant.vt ) );
            return false;
        }
    }
}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClientTopic::VariantToMsmqProperty( MSGPROPID propId, const CORE::CVariant& sourceVariant, MQPROPVARIANT& msmqTargetVariant )
{GUCEF_TRACE;

    VARTYPE targetVarType = GetMsmqVariantTypeForMsmqProperty( propId );    
    msmqTargetVariant.vt = targetVarType;

    switch ( targetVarType )
    {
        case VT_UI1:                    { msmqTargetVariant.bVal = (UCHAR) sourceVariant.AsUInt8(); return true; }
        case VT_I2:                     { msmqTargetVariant.iVal = (SHORT) sourceVariant.AsInt16(); return true; }
        case VT_UI2:                    { msmqTargetVariant.uiVal = (USHORT) sourceVariant.AsUInt16(); return true; }
        case VT_I4:                     { msmqTargetVariant.lVal = (LONG) sourceVariant.AsInt32(); return true; }
        case VT_UI4:                    { msmqTargetVariant.ulVal = (ULONG) sourceVariant.AsUInt32(); return true; } 
        case VT_UI8:                    { msmqTargetVariant.uhVal.QuadPart = (ULONGLONG) sourceVariant.AsUInt64(); return true; }
        case VT_ERROR:                  { msmqTargetVariant.scode = (LONG) sourceVariant.AsInt32(); return true; }  
        case VT_BOOL:                   { msmqTargetVariant.boolVal = (VARIANT_BOOL) sourceVariant.AsBool() ? VARIANT_TRUE : VARIANT_FALSE; return true; } // Actually stored as a short
        
        // The CLSID (GUID) record type is a C structure. The adapter does not need a size field for this record because it is a fixed length field. 
        // It is critical that the data must be exactly sizeof(CLSID) bytes (16) long for proper I/O with MSMQ. No more no less or you get an error!
        case VT_CLSID:                  
        { 
            if ( sourceVariant.ByteSize() >= sizeof(CLSID) ) 
            { 
                msmqTargetVariant.puuid = (CLSID*) sourceVariant.AsVoidPtr(); 
                return true; 
            } 
            else
            { 
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "MsmqPubSubClientTopic:VariantToMsmqProperty: Data given for VT_CLSID field is of incorrect nr of bytes: " + CORE::ToString( sourceVariant.ByteSize() ) );
                return false; 
            } 
        }

        // Remember that like the COM concept the length is actually stored at (BSTR pointer address)-4  
        // Since in the COM world this type is often used to shuttle binary data around we will mark it as a binary and not GUCEF_DATATYPE_UTF16_STRING
        case VT_BSTR:                   
        { 
            // @TODO if needed, this would require a dynamic memory alloc
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "MsmqPubSubClientTopic:VariantToMsmqProperty: VT_BSTR is currently not supported" );
            return false; 
        } 

        case VT_LPSTR:                  { msmqTargetVariant.pszVal = (LPSTR) sourceVariant.AsCharPtr(); return true; }
        case VT_LPWSTR:                 { msmqTargetVariant.pwszVal = (LPWSTR) sourceVariant.AsVoidPtr(); return true; }
        case VT_BLOB:                   { msmqTargetVariant.blob.pBlobData = (BYTE*) sourceVariant.AsVoidPtr(); msmqTargetVariant.blob.cbSize = (ULONG) sourceVariant.ByteSize(); return true; }
        
        case VT_DATE:                  
        { 
            // Sadly date is stored as a OLE Automation DATE which is a pain to convert
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "MsmqPubSubClientTopic:VariantToMsmqProperty: VT_DATE is currently not supported" );
            return false;             
        }
        
        // We dont have support for a special type for more complex data arrangements like vectors
        // until (if ever) we do, just note these as BINARY data
        case VT_VECTOR | VT_UI1:        
        case VT_VECTOR | VT_I2:
        case VT_VECTOR | VT_UI2:
        case VT_VECTOR | VT_BOOL:
        case VT_VECTOR | VT_I4:
        case VT_VECTOR | VT_CLSID:
        case VT_VECTOR | VT_BSTR:
        case VT_VECTOR | VT_LPWSTR:
        case VT_VECTOR | VT_VARIANT:
        {
            // msmqSourceVariant.caub.cElems contains the nr of elements possible in the available buffer not the nr of bytes actually used for elements inside said buffer
            // Hence we need to rely on the "lengthIfApplicable" param which obtains the size from a different property
            CORE::Int32 elementSize = SizeOfVarType( targetVarType );
            if ( elementSize > 0 )
            {
                msmqTargetVariant.caub.cElems = (ULONG) ( sourceVariant.ByteSize() / elementSize ); 
                msmqTargetVariant.caub.pElems = (UCHAR*) sourceVariant.AsVoidPtr();
                return true;
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "MsmqPubSubClientTopic:VariantToMsmqProperty: Unable to determine size or vector element of type " + CORE::ToString( targetVarType ) );
                return false;
            }
        }
        
        case VT_EMPTY: return true;

        default:
        {
            msmqTargetVariant.vt = VT_NULL;
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "MsmqPubSubClientTopic:VariantToMsmqProperty: Unable to map property " + CORE::ToString( (CORE::UInt32) propId ) );
            return false;
        }
    }
}

/*-------------------------------------------------------------------------*/

VARTYPE
CMsmqPubSubClientTopic::GetMsmqVariantTypeForMsmqProperty( PROPID propertyId )
{GUCEF_TRACE;
    
    switch ( propertyId )
    {
        case PROPID_M_HASH_ALG: { return VT_UI4; }
        case PROPID_M_ENCRYPTION_ALG: { return VT_UI4; }
        case PROPID_M_SENDER_CERT: { return VT_VECTOR | VT_UI1; } 
        case PROPID_M_SENDER_CERT_LEN: { return VT_UI4; } 
        case PROPID_M_SECURITY_CONTEXT: { return VT_UI4; } 
        case PROPID_M_XACT_STATUS_QUEUE: { return VT_LPWSTR; } 
        case PROPID_M_XACT_STATUS_QUEUE_LEN: { return VT_UI4; } 
        case PROPID_M_DEST_SYMM_KEY: { return VT_VECTOR | VT_UI1; }
        case PROPID_M_DEST_SYMM_KEY_LEN: { return VT_UI4; }
        case PROPID_M_SIGNATURE: { return VT_VECTOR | VT_UI1; }
        case PROPID_M_SIGNATURE_LEN: { return VT_UI4; }
        case PROPID_M_PROV_TYPE: { return VT_UI4; }
        case PROPID_M_PROV_NAME: { return VT_LPWSTR; }
        case PROPID_M_PROV_NAME_LEN: { return VT_UI4; }
        case PROPID_M_PRIV_LEVEL: { return VT_UI4; }
        case PROPID_M_SENDERID: { return VT_VECTOR | VT_UI1; } 
        case PROPID_M_SENDERID_LEN: { return VT_UI4; } 
        case PROPID_M_SENDERID_TYPE: { return VT_UI4; } 
        case PROPID_M_ABORT_COUNT: { return VT_UI4; }
        case PROPID_M_ACKNOWLEDGE: { return VT_UI1; }
        case PROPID_M_ADMIN_QUEUE: { return VT_LPWSTR; }                // Format name string of the administration queue.
        case PROPID_M_ADMIN_QUEUE_LEN: { return VT_UI4; }
        case PROPID_M_APPSPECIFIC: { return VT_UI4; }
        case PROPID_M_ARRIVEDTIME: { return VT_UI4; }
        case PROPID_M_AUTH_LEVEL: { return VT_UI4; }
        case PROPID_M_AUTHENTICATED: { return VT_UI1; }
        case PROPID_M_AUTHENTICATED_EX: { return VT_UI1; }
        case PROPID_M_BODY: { return VT_VECTOR | VT_UI1; }
        case PROPID_M_BODY_SIZE: { return VT_UI4; }
        case PROPID_M_BODY_TYPE: { return VT_UI4; }
        case PROPID_M_CLASS: { return VT_UI2; }
        case PROPID_M_COMPOUND_MESSAGE: { return VT_VECTOR | VT_UI1; }  // An array of bytes that represents the entire contents of an SRMP message, including both the SOAP envelope and the SOAP attachments (MIME binary attachments) associated with it.
        case PROPID_M_COMPOUND_MESSAGE_SIZE: { return VT_UI4; }         // The size (in bytes) of the entire SRMP message, including both the SOAP envelope and the SOAP attachments, provided by PROPID_M_COMPOUND_MESSAGE.
        case PROPID_M_CONNECTOR_TYPE: { return VT_CLSID; }              // Pointer to a connector application identifier.
        case PROPID_M_CORRELATIONID: { return VT_VECTOR | VT_UI1; }     // Application-defined 20-byte message identifier (default is 0).
        case PROPID_M_DEADLETTER_QUEUE: { return VT_LPWSTR; }           // The path name of an application-specific DLQ.
        case PROPID_M_DEADLETTER_QUEUE_LEN: { return VT_UI4; }          // Input: The length of the application-specific dead letter queue (DLQ) name buffer (in Unicode characters) allocated by the receiving application. | Return: The length (in Unicode characters) of the application-specific DLQ name string (including the null-terminating character) returned by PROPID_M_DEADLETTER_QUEUE.
        case PROPID_M_DELIVERY: { return VT_UI1; }                      // The PROPID_M_DELIVERY property specifies how Message Queuing delivers the message to the queue.
        case PROPID_M_DEST_FORMAT_NAME: { return VT_LPWSTR; }           // String that contains a public, private, direct, distribution list, or multiple-element format name.
        case PROPID_M_DEST_FORMAT_NAME_LEN: { return VT_UI4; }          // On input: The length of the format name buffer (in Unicode characters) allocated by the receiving application. | On return: The length (in Unicode characters) of the format name string (including the null-terminating character) returned in PROPID_M_DEST_FORMAT_NAME.
        case PROPID_M_DEST_QUEUE: { return VT_LPWSTR; }                 // Format name of the original destination queue.
        case PROPID_M_DEST_QUEUE_LEN: { return VT_UI4; }                // The length (in Unicode characters) of the supplied format name buffer OR length of the returned format name string for the destination queue
        case PROPID_M_EXTENSION: { return VT_UI1 | VT_VECTOR; }         // This property provides a place to put additional application-defined information that is associated with the message.
        case PROPID_M_EXTENSION_LEN: { return VT_UI4; }                 // This property specifies the length of the information in the PROPID_M_EXTENSION property.
        case PROPID_M_FIRST_IN_XACT: { return VT_UI1; }                 // This property indicates whether the message was the first message sent in a transaction.
        case PROPID_M_JOURNAL: { return VT_UI1; }                       // This property specifies the source journaling level for the message.
        case PROPID_M_LABEL: { return VT_LPWSTR; }                      // This property specifies a label of the message.
        case PROPID_M_LABEL_LEN: { return VT_UI4; }                     // This property identifies the length (in Unicode characters) of the message label buffer.
        case PROPID_M_LAST_IN_XACT: { return VT_UI1; }                  // This property indicates whether the message was the last message sent in a transaction.
        case PROPID_M_MSGID: { return VT_VECTOR | VT_UI1; }             // This property indicates the MSMQ-generated identifier of the message. MSMQ generates a 20-byte message identifier when the message is sent.
        case PROPID_M_PRIORITY: { return VT_UI1; }                      // This property specifies the priority of the message. A low number means low priority.
        case PROPID_M_RESP_QUEUE: { return VT_LPWSTR; }                 // This property specifies the queue where application-generated response messages are returned.
        case PROPID_M_RESP_QUEUE_LEN: { return VT_UI4; }                // This property indicates the length (in Unicode characters) of the response queue format name buffer.
        case PROPID_M_SENTTIME: { return VT_UI4; }                      // This property indicates the date and time that the message was sent by the source Queue Manager.
        case PROPID_M_SOAP_BODY: { return VT_LPWSTR; }                  // This property specifies additional application-generated body elements for inclusion in the SOAP envelope of an HTTP message.
        case PROPID_M_SOAP_ENVELOPE: { return VT_LPWSTR; }              // This property provides the SOAP envelope of an HTTP message and does not include binary attachments.
        case PROPID_M_SOAP_ENVELOPE_LEN: { return VT_UI4; }             // This property provides the size of the SOAP envelope of an HTTP message.
        case PROPID_M_SOAP_HEADER: { return VT_LPWSTR; }                // This property specifies additional application-generated header elements for inclusion in the SOAP envelope of an HTTP message.
        case PROPID_M_SRC_MACHINE_ID: { return VT_CLSID; }              // This property specifies the computer where the message originated.
        case PROPID_M_TIME_TO_BE_RECEIVED: { return VT_UI4; }           // This property specifies the total time (in seconds) the message is allowed to live. This includes the time it spends getting to the destination queue plus the time spent waiting in the queue before it is retrieved by an application.
        case PROPID_M_TIME_TO_REACH_QUEUE: { return VT_UI4; }           // This property specifies a time limit (in seconds) for the message to reach the queue.
        case PROPID_M_TRACE: { return VT_UI1; }                         // This property specifies where report messages are sent when tracing a message.
        case PROPID_M_VERSION: { return VT_UI4; }                       // This property specifies the version of MSMQ used to send the message.
        case PROPID_M_XACTID: { return VT_VECTOR | VT_UI1; }            // This property indicates which transaction sent the message.
        case PROPID_Q_AUTHENTICATE: { return VT_UI1; }                  // Optional. This property specifies whether the queue accepts only authenticated messages.
        case PROPID_Q_CREATE_TIME: { return VT_I4; }                    // Optional read-only. This property indicates the time and date when the queue was created.
        case PROPID_Q_JOURNAL: { return VT_UI1; }                       // Optional. This property specifies the journaling level of the queue.
        case PROPID_Q_JOURNAL_QUOTA: { return VT_UI4; }                 // Optional. This property specifies the maximum size (in kilobytes) of the queue journal.
        case PROPID_Q_LABEL: { return VT_LPWSTR; }                      // Optional. This property specifies a description of the queue.
        case PROPID_Q_MODIFY_TIME: { return VT_I4; }                    // Optional. This property indicates the last time the properties of a queue were modified.
        case PROPID_Q_PATHNAME: { return VT_LPWSTR; }                   // Required (to create the queue). This property specifies the MSMQ path name of the queue. The maximum length of the name of the queue is 124 Unicode characters
        case PROPID_Q_PRIV_LEVEL: { return VT_UI4; }                    // Optional. This property specifies the privacy level required by the queue.
        case PROPID_Q_QUOTA: { return VT_UI4; }                         // Optional. This property specifies the maximum size (in kilobytes) of the queue. The default is INFINITE.
        case PROPID_Q_TRANSACTION: { return VT_UI1; }                   // Optional. This property specifies whether the queue is a transactional queue or a nontransactional queue.
        case PROPID_QM_MACHINE_ID: { return VT_CLSID; }                 // This property identifies the computer. The PROPID_QM_MACHINE_ID property is set by MSMQ when the computer is defined.
        
        default: 
        { 
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "MsmqPubSubClientTopic:GetMsmqVariantTypeForMsmqProperty: Unable to map property " + CORE::ToString( (CORE::UInt32) propertyId ) );
            return VT_EMPTY; 
        }
    }
}

/*-------------------------------------------------------------------------*/

PROPID
CMsmqPubSubClientTopic::GetPayloadPropertyForPayloadSizeProperty( PROPID payloadSizePropId )
{GUCEF_TRACE;

    switch ( payloadSizePropId )
    {
        case PROPID_M_ADMIN_QUEUE_LEN:       { return PROPID_M_ADMIN_QUEUE; }
        case PROPID_M_BODY_SIZE:             { return PROPID_M_BODY; }
        case PROPID_M_COMPOUND_MESSAGE_SIZE: { return PROPID_M_COMPOUND_MESSAGE; }
        case PROPID_M_DEADLETTER_QUEUE_LEN:  { return PROPID_M_DEADLETTER_QUEUE; }
        case PROPID_M_DEST_FORMAT_NAME_LEN:  { return PROPID_M_DEST_FORMAT_NAME; }
        case PROPID_M_DEST_QUEUE_LEN:        { return PROPID_M_DEST_QUEUE; }
        case PROPID_M_EXTENSION_LEN:         { return PROPID_M_EXTENSION; }
        case PROPID_M_LABEL_LEN:             { return PROPID_M_LABEL; }
        case PROPID_M_RESP_QUEUE_LEN:        { return PROPID_M_RESP_QUEUE; }
        case PROPID_M_SOAP_ENVELOPE_LEN:     { return PROPID_M_SOAP_ENVELOPE; }
                
        default: 
        { 
            return PROPID_M_BASE; 
        }
    }
}   

/*-------------------------------------------------------------------------*/

PROPID
CMsmqPubSubClientTopic::GetPayloadSizePropertyForPayloadProperty( PROPID payloadPropId )
{GUCEF_TRACE;

    switch ( payloadPropId )
    {
        case PROPID_M_ADMIN_QUEUE:       { return PROPID_M_ADMIN_QUEUE_LEN; }
        case PROPID_M_BODY:              { return PROPID_M_BODY_SIZE; }
        case PROPID_M_COMPOUND_MESSAGE:  { return PROPID_M_COMPOUND_MESSAGE_SIZE; }
        case PROPID_M_DEADLETTER_QUEUE:  { return PROPID_M_DEADLETTER_QUEUE_LEN; }
        case PROPID_M_DEST_FORMAT_NAME:  { return PROPID_M_DEST_FORMAT_NAME_LEN; }
        case PROPID_M_DEST_QUEUE:        { return PROPID_M_DEST_QUEUE_LEN; }
        case PROPID_M_EXTENSION:         { return PROPID_M_EXTENSION_LEN; }
        case PROPID_M_LABEL:             { return PROPID_M_LABEL_LEN; }
        case PROPID_M_RESP_QUEUE:        { return PROPID_M_RESP_QUEUE_LEN; }
        case PROPID_M_SOAP_ENVELOPE:     { return PROPID_M_SOAP_ENVELOPE_LEN; }
                
        default: 
        { 
            return PROPID_M_BASE; 
        }
    }
}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClientTopic::SetUInt32OnPropertyVariant( PROPID propertyId, CORE::UInt32 valueToSet, TMsmqMsg& msgData )
{GUCEF_TRACE;
    
    if ( PROPID_M_BASE == propertyId )
        return true;

    for ( DWORD i=0; i<msgData.msgprops.cProp; ++i )
    {
        if ( propertyId == msgData.msgprops.aPropID[ i ] )
        {
            msgData.msgprops.aPropVar[ i ].ulVal = (ULONG) valueToSet;
            return true;
        }
    }

    return false;
}

/*-------------------------------------------------------------------------*/

void 
CMsmqPubSubClientTopic::PrepMsmqVariantStorageForProperty( PROPID propertyId, MQPROPVARIANT& msmqVariant, TMsmqMsg& msgData )
{GUCEF_TRACE;
    
    msmqVariant.vt = GetMsmqVariantTypeForMsmqProperty( propertyId );

    switch ( msmqVariant.vt )
    {
        case VT_LPWSTR:
        { 
            // Why 64: 
            // "Queue names longer than 124 Unicode characters are not supported. Using names longer than 64 Unicode characters for public queue names may cause a slight reduction in performance."
            // Queue names are the most common usage for VT_LPWSTR

            CORE::CDynamicBuffer& buffer = msgData.propBuffers[ propertyId ];
            buffer.SetDataSize( 64*sizeof(WCHAR) ); 
            msmqVariant.pwszVal = buffer.AsTypePtr<WCHAR>();
            SetUInt32OnPropertyVariant( GetPayloadSizePropertyForPayloadProperty( propertyId ), buffer.GetBufferSize(), msgData );
            break;
        } 
        case VT_LPSTR:
        { 
            // Why 64: Since we picked 64 for unicode as the max, seems like a good default for this one as well
            CORE::CDynamicBuffer& buffer = msgData.propBuffers[ propertyId ];
            buffer.SetDataSize( 64*sizeof(CHAR) ); 
            msmqVariant.pszVal = buffer.AsTypePtr<CHAR>();
            SetUInt32OnPropertyVariant( GetPayloadSizePropertyForPayloadProperty( propertyId ), buffer.GetBufferSize(), msgData );  
            break;
        }
        case VT_CLSID:
        { 
            CORE::CDynamicBuffer& buffer = msgData.propBuffers[ propertyId ];
            buffer.SetDataSize( sizeof(CLSID) ); 
            msmqVariant.puuid = buffer.AsTypePtr<CLSID>();  
            break;
        }
        case VT_VECTOR | VT_UI1:
        case VT_VECTOR | VT_UI2:
        case VT_VECTOR | VT_UI4:
        {
            CORE::CDynamicBuffer& buffer = msgData.propBuffers[ propertyId ];

            switch ( propertyId )
            {
                // BODY is by far the largest in allowing up to 4MB per msg so we handle that as a special case
                // This prevents what would likely be common resizes. Half the max allowed seems like a decent default
                case PROPID_M_BODY: { buffer.SetDataSize( 1024 * 1024 * 2 ); break; }

                // Fields containing a MSGID MUST be exactly PROPID_M_MSGID_SIZE (20 bytes)
                // MSDN notes: "The buffer must consist of exactly 20 unsigned characters. Specifying either a smaller or larger buffer will result in an error."
                case PROPID_M_CORRELATIONID:
                case PROPID_M_MSGID: { buffer.SetDataSize( PROPID_M_MSGID_SIZE ); break; }

                default: { buffer.SetDataSize( 128 ); break; }
            }          
             
            msmqVariant.caub.cElems = (ULONG) buffer.GetBufferSize();
            msmqVariant.caub.pElems = buffer.AsTypePtr<UCHAR>();  
            break;
        } 
        default:
            break;
    }
}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClientTopic::OnMsmqMsgBufferTooSmall( TMsmqMsg& msgsData )
{GUCEF_TRACE;
    
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "MsmqPubSubClientTopic:OnMsmqMsgBufferTooSmall: The to-be-mapped MSMQ message reportedly needs larger buffer space to copy the data" );
    
    // If this happens we need cycle through the message properties and find any mismatch between
    // size/length of the property as relayed vs the buffer of the associated paired property
    CORE::UInt32 foundIssues = 0;
    for ( DWORD i=0; i<msgsData.msgprops.cProp; ++i )
    {
        PROPID relatedPayloadProperty = GetPayloadPropertyForPayloadSizeProperty( msgsData.msgprops.aPropID[ i ] );
        if ( relatedPayloadProperty != PROPID_M_BASE )
        {
            CORE::UInt32 requiredSize = msgsData.msgprops.aPropVar[ i ].ulVal; 
            CORE::CDynamicBuffer& buffer = msgsData.propBuffers[ relatedPayloadProperty ];

            if ( buffer.GetBufferSize() < requiredSize )
            {
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "MsmqPubSubClientTopic::OnMsmqMsgBufferTooSmall: Buffer size for payload property " + CORE::ToString( (CORE::UInt32) relatedPayloadProperty ) + " as specified by property " + 
                    CORE::ToString( (CORE::UInt32) msgsData.msgprops.aPropID[ i ] ) + " is insufficient at " + CORE::ToString( buffer.GetBufferSize() ) + " bytes. It needs " + CORE::ToString( requiredSize ) + " bytes"  );

                msgsData.propBuffers[ relatedPayloadProperty ].SetDataSize( requiredSize );
                ++foundIssues;
            }
        }
    }

    if ( 0 == foundIssues )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "MsmqPubSubClientTopic::OnMsmqMsgBufferTooSmall: Unable to find any issue with a size mismatch on buffers vs reported needs of the received message, unable to fix the issue" );
        return false;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

void 
CMsmqPubSubClientTopic::PrepMsmqPropIdToPropIndexMap( MSGPROPIDToUInt32Map& propIndexMapToBuild, const MSGPROPIDVector& msmqPropIdsToUse )
{GUCEF_TRACE;

    CORE::UInt32 propIndex = 0;
    MSGPROPIDVector::const_iterator i = msmqPropIdsToUse.begin();
    while ( i != msmqPropIdsToUse.end() )
    {
        propIndexMapToBuild[ (*i) ] = propIndex;
        ++i; ++propIndex;
    }
}

/*-------------------------------------------------------------------------*/

void 
CMsmqPubSubClientTopic::PrepMsmqMsgStorage( TMsmqMsg& msg, MSGPROPIDVector& msmqPropIdsToUse )
{GUCEF_TRACE;

    if ( msg.aMsgPropId.size() != msmqPropIdsToUse.size() )
    {       
        msg.aMsgPropId.resize( msmqPropIdsToUse.size() );
        msg.aMsgPropVar.resize( msmqPropIdsToUse.size() );
        msg.aStatus.resize( msmqPropIdsToUse.size() );

        msg.msgprops.cProp = (DWORD) msmqPropIdsToUse.size();   // Number of properties we want to receive or send per message
        msg.msgprops.aPropID = &msg.aMsgPropId[ 0 ];            // pointer to MSMQ PROPID array
        msg.msgprops.aPropVar = &msg.aMsgPropVar[ 0 ];          // pointer to MSMQ VARIANT struct array
        msg.msgprops.aStatus = &msg.aStatus[ 0 ];               // Error reports on a per property basis

        CORE::UInt32 propIndex = 0;
        MSGPROPIDVector::iterator p = msmqPropIdsToUse.begin();
        while ( p != msmqPropIdsToUse.end() )
        {
            msg.aMsgPropId[ propIndex ] = (*p);
            PrepMsmqVariantStorageForProperty( (*p), msg.msgprops.aPropVar[ propIndex ], msg );
            ++p; ++propIndex;
        }
    }
}

/*-------------------------------------------------------------------------*/

void 
CMsmqPubSubClientTopic::PrepMsmqMsgsStorage( void )
{GUCEF_TRACE;

    if ( m_config.needSubscribeSupport )
    {
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "MsmqPubSubClientTopic:PrepMsmqMsgsStorage: Allocating storage for subscription related activity" );
        
        m_pubsubMsgs.resize( m_config.maxMsmqMsgsToReadPerSyncCycle );
        m_msmqReceiveMsgs.resize( m_config.maxMsmqMsgsToReadPerSyncCycle );
        MsmqMsgVector::iterator i = m_msmqReceiveMsgs.begin();
        while ( i != m_msmqReceiveMsgs.end() )
        {
            TMsmqMsg& msg = (*i);
            PrepMsmqMsgStorage( msg, m_config.msmqPropIdsToReceive );
            ++i;
        }
    }

    if ( m_config.needPublishSupport )
    {        
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "MsmqPubSubClientTopic:PrepMsmqMsgsStorage: Allocating storage for publishing related activity" );
        
        PrepMsmqMsgStorage( m_msgSendMsg, m_config.msmqPropIdsToSend );
        PrepMsmqPropIdToPropIndexMap( m_msgSendPropMap, m_config.msmqPropIdsToSend );
    }
}

/*-------------------------------------------------------------------------*/

void
CMsmqPubSubClientTopic::OnMsmqMsgReceived( const MQMSGPROPS& msg, CORE::UInt32 msgCycleIndex, bool linkIfPossible )
{GUCEF_TRACE;

    COMCORE::CBasicPubSubMsg& translatedMsg = m_pubsubMsgs[ msgCycleIndex ];
    translatedMsg.Clear();

    CORE::UInt32 bodyType = VT_EMPTY;
    CORE::UInt32 bodySize = 0;

    for ( CORE::UInt32 i=0; i<msg.cProp; ++i )
    {
        // Depending on what the property is we may perform special handling
        // Note that we rely here on config loading having enforced an ordering in the message
        // properties that ensures that things like length/size or type come before the actual payload

        CORE::CVariant keyVar( (CORE::UInt64) msg.aPropID[ i ] );
        CORE::CVariant ValueVar;

        switch ( msg.aPropID[ i ] )
        {
            
            case PROPID_M_BODY_TYPE: { MsmqPropertyToVariant( msg.aPropVar[ i ], ValueVar, linkIfPossible ); bodyType = ValueVar.AsUInt32( bodyType ); break; }
            case PROPID_M_BODY_SIZE: { MsmqPropertyToVariant( msg.aPropVar[ i ], ValueVar, linkIfPossible ); bodySize = ValueVar.AsUInt32( bodySize ); break; }
            case PROPID_M_BODY:
            {
                if ( MsmqPropertyToVariant( msg.aPropVar[ i ], ValueVar, linkIfPossible, bodySize ) )
                {                
                    if ( linkIfPossible )
                        translatedMsg.GetPrimaryPayload().LinkTo( ValueVar );
                    else
                        ValueVar.TransferOwnershipTo( translatedMsg.GetPrimaryPayload() );
                }
                else
                {
                    // ID should always be available since its MSMQ generated
                    GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "MsmqPubSubClientTopic::OnMsmqMsgReceived: Unable to translate MSMQ PROPID_M_BODY. Translated message won't have a body" );
                    translatedMsg.GetPrimaryPayload().Clear();
                }
                break;
            }

            case PROPID_M_MSGID:
            {
                if ( MsmqPropertyToVariant( msg.aPropVar[ i ], ValueVar, linkIfPossible ) )
                {
                    if ( linkIfPossible )
                        translatedMsg.GetMsgId().LinkTo( ValueVar );
                    else
                        ValueVar.TransferOwnershipTo( translatedMsg.GetMsgId() );
                }
                else
                {
                    // ID should always be available since its MSMQ generated
                    GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "MsmqPubSubClientTopic::OnMsmqMsgReceived: Unable to translate MSMQ PROPID_M_MSGID. Translated message won't have an ID" );
                    translatedMsg.GetMsgId().Clear();
                }
                break;
            }

            case PROPID_M_SENTTIME: 
            {
                if ( MsmqPropertyToVariant( msg.aPropVar[ i ], ValueVar, linkIfPossible ) )
                {
                    // Time is Unix epoch based in second resolution
                    // It is stored as a UInt32 but we use a UInt64 because we need the headroom to convert to milliseconds
                    translatedMsg.GetMsgDateTime().FromUnixEpochBasedTicksInMillisecs( ValueVar.AsUInt64() * 1000 );
                }
                else
                {
                    // PROPID_M_SENTTIME should always be available since its MSMQ generated
                    GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "MsmqPubSubClientTopic::OnMsmqMsgReceived: Unable to translate MSMQ PROPID_M_SENTTIME. Translated message won't have a DateTime timestamp originating from MSMQ, falling back to 'now' dt" );
                    translatedMsg.GetMsgDateTime() = CORE::CDateTime::NowUTCDateTime();
                }
                break;
            }
                        
            default:
            {
                // No special processing, treat as meta-data

                if ( !MsmqPropertyToVariant( msg.aPropVar[ i ], ValueVar, linkIfPossible ) )
                {
                    // Skip this property, continue best effort
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "MsmqPubSubClientTopic::OnMsmqMsgReceived: Unable to translate MSMQ meta-data propery value variant for property " + keyVar.AsString() +
                        " which is property " + CORE::ToString( i ) + " of " + CORE::ToString( (CORE::UInt32) msg.cProp ) );
                    continue;
                }

                if ( linkIfPossible )
                    translatedMsg.AddLinkedMetaDataKeyValuePair( keyVar, ValueVar );
                else
                    translatedMsg.AddMetaDataKeyValuePair( keyVar, ValueVar );
                break;
            }
        }
    }    
}

/*-------------------------------------------------------------------------*/

void
CMsmqPubSubClientTopic::OnSyncReadTimerCycle( CORE::CNotifier* notifier    ,
                                              const CORE::CEvent& eventId  ,
                                              CORE::CICloneable* eventData )
{GUCEF_TRACE;
        
    DWORD dwAction = MQ_ACTION_RECEIVE;                 // could support peeking: isPeekOnly ? MQ_ACTION_PEEK_CURRENT : MQ_ACTION_RECEIVE;
    DWORD dwTimeout = 0;                                // We dont wait for messages during sync reads, we get what we can this cycle and thats it
    ITransaction* pTransaction = MQ_NO_TRANSACTION;     // could support transactions: isTransactional ? MQ_SINGLE_MESSAGE : MQ_NO_TRANSACTION;
    CORE::UInt32 msgsRead = 0;
    for ( CORE::UInt32 i=0; i<m_config.maxMsmqMsgsToReadPerSyncCycle; ++i )
    {
        HRESULT receiveHResult = ::MQReceiveMessage( m_receiveQueueHandle             ,  // Handle to the destination queue
                                                     dwTimeout                        ,  // Time out interval
                                                     dwAction                         ,  // Peek?  or Dequeue.  Receive action
                                                     &m_msmqReceiveMsgs[ i ].msgprops ,  // Pointer to the MQMSGPROPS structure
                                                     NULL                             ,  // No OVERLAPPED structure etc.
                                                     NULL                             ,  // No OVERLAPPED structure etc.
                                                     NULL                             ,  // No OVERLAPPED structure etc.
                                                     pTransaction                     ); // MQ_SINGLE_MESSAGE | MQ_MTS_TRANSACTION | MQ_XA_TRANSACTION   

        CORE::UInt64 receiveResult = (CORE::UInt64) receiveHResult;
        switch ( receiveResult )
        {
            case (CORE::UInt64) MQ_OK:
            {
                // Successfully received a message
                // Since we are doing a sync cycle we can use linking
                OnMsmqMsgReceived( m_msmqReceiveMsgs[ i ].msgprops, i, true );
                
                ++msgsRead;
                m_pubsubMsgsRefs.msgs.push_back( TPubSubMsgRef( &m_pubsubMsgs[ i ] ) );
                
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "MsmqPubSubClientTopic:OnSyncReadTimerCycle: Received and mapped a MSMQ message" );
                break;
            }
            case (CORE::UInt64) MQ_INFORMATION_PROPERTY:
            {
                // Received a message but some warnings were triggered
                // Since we are doing a sync cycle we can use linking
                OnMsmqMsgReceived( m_msmqReceiveMsgs[ i ].msgprops, i, true );
                
                ++msgsRead;
                m_pubsubMsgsRefs.msgs.push_back( TPubSubMsgRef( &m_pubsubMsgs[ i ] ) );
                
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "MsmqPubSubClientTopic:OnSyncReadTimerCycle: Received and mapped a MSMQ message but with warnings" );
                break;
            }

            case (CORE::UInt64) MQ_ERROR_BUFFER_OVERFLOW:
            case (CORE::UInt64) MQ_ERROR_FORMATNAME_BUFFER_TOO_SMALL:
            {
                // Use the length/size given to resize buffers as needed
                // When this error happens the msg is not removed so we can resize and try again
                if ( OnMsmqMsgBufferTooSmall( m_msmqReceiveMsgs[ i ] ) )
                    --i; // retry the same message slot, hopefully this time obtaining all the payloads properly
                break;
            }

            case (CORE::UInt64) MQ_ERROR_IO_TIMEOUT: 
            {
                // No more messages are available, stop trying to read more
                i = m_config.maxMsmqMsgsToReadPerSyncCycle; 
                break; 
            }
            
            default:
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "MsmqPubSubClientTopic::OnSyncReadTimerCycle: Unhandled issue retrieving message " + CORE::ToString( i ) + 
                    " of the current cycle. HRESULT= " + CORE::ToString( receiveHResult ) + 
                    ". Win32 Error message: " + CORE::ToString( RetrieveWin32APIErrorMessage( HRESULT_CODE( receiveHResult ) ) ) );
                break;
            }
        }
    }

    if ( msgsRead > 0 )
    {
        // Communicate all the messages received via an event notification
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "MsmqPubSubClientTopic:OnSyncReadTimerCycle: Received and mapped a total of " + CORE::ToString( msgsRead ) + 
            " messages out of a max of " + CORE::ToString( m_config.maxMsmqMsgsToReadPerSyncCycle ) );

        m_pubsubMsgsRefs.receiveActionId = m_currentReceiveActionId;
        ++m_currentReceiveActionId;

        if ( !NotifyObservers( MsgsRecievedEvent, &m_pubsubMsgsRefs ) ) return;
        m_pubsubMsgsRefs.msgs.clear();
    }
}

/*-------------------------------------------------------------------------*/

void
CMsmqPubSubClientTopic::OnPulseCycle( CORE::CNotifier* notifier    ,
                                      const CORE::CEvent& eventId  ,
                                      CORE::CICloneable* eventData )
{GUCEF_TRACE;
                      
    if ( !m_publishSuccessActionIds.empty() )
    {
        if ( !NotifyObservers( MsgsPublishedEvent, &m_publishSuccessActionEventData ) ) return;
        m_publishSuccessActionIds.clear();
    }
    if ( !m_publishFailureActionIds.empty() )
    {
        if ( !NotifyObservers( MsgsPublishFailureEvent, &m_publishFailureActionEventData ) ) return;
        m_publishFailureActionIds.clear();
    }
}

/*-------------------------------------------------------------------------*/

void
CMsmqPubSubClientTopic::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                             const CORE::CEvent& eventId  ,
                                             CORE::CICloneable* eventData )
{GUCEF_TRACE;

    // If m_msmqMsgsQueued is -1 dont bother trying again as we cannot solve config/permission issues here
    if ( m_msmqMsgsQueued >= 0 )
    {
        m_msmqMsgsQueued = GetCurrentNrOfMessagesInQueue();
    }
}

/*-------------------------------------------------------------------------*/

const MT::CILockable* 
CMsmqPubSubClientTopic::AsLockable( void ) const
{GUCEF_TRACE;

    return this;
}

/*-------------------------------------------------------------------------*/

 bool
 CMsmqPubSubClientTopic::Lock( CORE::UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return m_lock.Lock( lockWaitTimeoutInMs );
}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClientTopic::Unlock( void ) const
{GUCEF_TRACE;

    return m_lock.Unlock();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MSMQ */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
