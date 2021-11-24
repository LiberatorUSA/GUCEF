/*
 *  pubsubpluginAWSSQS: Generic GUCEF COMCORE plugin for providing pubsub via AWS's SQS
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

#include <string.h>

// Windoze has a macro for SendMessage which messes up our API calls
#undef SendMessage

#include <aws/core/Aws.h>
#include <aws/sqs/SQSClient.h>
#include <aws/sqs/model/GetQueueUrlRequest.h>
#include <aws/sqs/model/GetQueueUrlResult.h>
#include <aws/sqs/model/SendMessageRequest.h>
#include <aws/sqs/model/SendMessageResult.h>
#include <aws/sqs/model/SendMessageBatchRequest.h>
#include <aws/sqs/model/SendMessageBatchResult.h>
#include <aws/sqs/model/ReceiveMessageRequest.h>
#include <aws/sqs/model/ReceiveMessageResult.h>
#include <aws/sqs/model/DeleteMessageRequest.h>
#include <iostream>

#ifndef GUCEF_MT_CSCOPEMUTEX_H
#include "gucefMT_CScopeMutex.h"
#define GUCEF_MT_CSCOPEMUTEX_H
#endif /* GUCEF_MT_CSCOPEMUTEX_H ? */

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H */

#ifndef GUCEF_CORE_METRICSMACROS_H
#include "gucefCORE_MetricsMacros.h"
#define GUCEF_CORE_METRICSMACROS_H
#endif /* GUCEF_CORE_METRICSMACROS_H ? */

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H */

#ifndef PUBSUBPLUGIN_AWSSQS_CAWSSQSPUBSUBCLIENT_H
#include "pubsubpluginAWSSQS_CAwsSqsPubSubClient.h"
#define PUBSUBPLUGIN_AWSSQS_CAWSSQSPUBSUBCLIENT_H
#endif /* PUBSUBPLUGIN_AWSSQS_CAWSSQSPUBSUBCLIENT_H ? */

#include "pubsubpluginAWSSQS_CAwsSqsPubSubClientTopic.h"

// Windoze has a macro for SendMessage which messes up our API calls
#undef SendMessage

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUBPLUGIN {
namespace AWSSQS {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#define SQSCLIENT_MAX_PAYLOAD_SIZE                  ( 256 * 1024 ) // 256KB

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CAwsSqsPubSubClientTopic::CAwsSqsPubSubClientTopic( CAwsSqsPubSubClient* client )
    : COMCORE::CPubSubClientTopic()
    , m_client( client )
    , m_redisShardHost()
    , m_config()
    , m_lock()
    , m_queueUrl()
    , m_publishBulkMsgRemapStorage()
    , m_currentPublishActionId( 1 )
    , m_currentReceiveActionId( 1 )
    , m_publishSuccessActionIds()
    , m_publishSuccessActionEventData()
    , m_publishFailureActionIds()
    , m_publishFailureActionEventData()
{GUCEF_TRACE;
        
    m_publishSuccessActionEventData.LinkTo( &m_publishSuccessActionIds );
    m_publishFailureActionEventData.LinkTo( &m_publishFailureActionIds );
    
    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

CAwsSqsPubSubClientTopic::~CAwsSqsPubSubClientTopic()
{GUCEF_TRACE;

    Disconnect();
}

/*-------------------------------------------------------------------------*/

COMCORE::CPubSubClient*
CAwsSqsPubSubClientTopic::GetClient( void )
{GUCEF_TRACE;

    return m_client;
}

/*-------------------------------------------------------------------------*/

void
CAwsSqsPubSubClientTopic::RegisterEventHandlers( void )
{GUCEF_TRACE;

    TEventCallback callback( this, &CAwsSqsPubSubClientTopic::OnPulseCycle );
    SubscribeTo( m_client->GetConfig().pulseGenerator ,
                 CORE::CPulseGenerator::PulseEvent    ,
                 callback                             );
}

/*-------------------------------------------------------------------------*/

bool
CAwsSqsPubSubClientTopic::IsPublishingSupported( void )
{GUCEF_TRACE;
    
    return m_config.needPublishSupport;
}

/*-------------------------------------------------------------------------*/

bool
CAwsSqsPubSubClientTopic::IsSubscribingSupported( void )
{GUCEF_TRACE;

    return true;
}

/*-------------------------------------------------------------------------*/

const CORE::CString& 
CAwsSqsPubSubClientTopic::GetTopicName( void ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return m_config.topicName;
}

/*-------------------------------------------------------------------------*/

Aws::String
CAwsSqsPubSubClientTopic::GetSqsQueueUrlForQueueName( const CORE::CString& queueName )
{GUCEF_TRACE;

    try
    {
        Aws::SQS::Model::GetQueueUrlRequest gqu_req;
        gqu_req.SetQueueName( queueName );

        Aws::SQS::Model::GetQueueUrlOutcome gqu_out = m_client->GetSqsClient().GetQueueUrl( gqu_req );
        if ( gqu_out.IsSuccess() ) 
        {
            const Aws::String& queueUrl = gqu_out.GetResult().GetQueueUrl();

            GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "AwsSqsPubSubClientTopic:GetSqsQueueUrlForQueueName: Resolved queue name \"" + queueName = "\" to URL: " + queueUrl );
            return queueUrl;
        } 
        else 
        {
            GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "AwsSqsPubSubClientTopic:GetSqsQueueUrlForQueueName: Error getting URL for queue name \"" + queueName = "\":" + gqu_out.GetError().GetMessage() );
        }
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_CRITICAL, CORE::CString( "AwsSqsPubSubClientTopic:GetSqsQueueUrlForQueueName: experienced an exception: " ) + e.what() );
    }
    catch ( ... )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_CRITICAL, "AwsSqsPubSubClientTopic:GetSqsQueueUrlForQueueName: experienced an unknown exception, your application may be unstable" );
    }

    return Aws::String();
}

/*-------------------------------------------------------------------------*/

bool 
CAwsSqsPubSubClientTopic::Publish( CORE::UInt64& publishActionId, const COMCORE::CIPubSubMsg& msg, bool notify )
{GUCEF_TRACE;

    bool success = false;
    MT::CScopeMutex lock( m_lock );

    if ( 0 == publishActionId )
    {
        publishActionId = m_currentPublishActionId; 
        ++m_currentPublishActionId;
    }

    try
    {  
        Aws::SQS::Model::SendMessageRequest sm_req;
        sm_req.SetQueueUrl( m_queueUrl );
               
        CORE::UInt32 msgByteSize = 0;                     
        if ( TranslateToSqsMsg< Aws::SQS::Model::SendMessageRequest >( sm_req, &msg, msgByteSize ) )
        {
            Aws::SQS::Model::SendMessageOutcome sm_out = m_client->GetSqsClient().SendMessage( sm_req );
            if ( sm_out.IsSuccess() )
            {
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "AwsSqsPubSubClientTopic:Publish: Success sending message to \"" + m_queueUrl + "\"" );
                success = true;
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "AwsSqsPubSubClientTopic:Publish: Error sending message to \"" + m_queueUrl + "\". Error Msg: " + sm_out.GetError().GetMessage() );
            } 
        }
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_CRITICAL, CORE::CString( "AwsSqsPubSubClientTopic:Publish: experienced an exception: " ) + e.what() );
    }
    catch ( ... )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_CRITICAL, "AwsSqsPubSubClientTopic:Publish: experienced an unknown exception, your application may be unstable" );
    }
    
    if ( notify )
    {
        if ( success )
        {           
            m_publishSuccessActionIds.push_back( publishActionId );
        }
        else
        {
            m_publishFailureActionIds.push_back( publishActionId );
        }
    }
    return success;
}

/*-------------------------------------------------------------------------*/

bool 
CAwsSqsPubSubClientTopic::Publish( TPublishActionIdVector& publishActionIds, const COMCORE::CBasicPubSubMsg::TBasicPubSubMsgVector& msgs, bool notify )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    m_publishBulkMsgRemapStorage.resize( msgs.size() );
    COMCORE::CBasicPubSubMsg::TBasicPubSubMsgVector::const_iterator i = msgs.begin();
    while ( i != msgs.end() )    
    {
        m_publishBulkMsgRemapStorage.push_back( &(*i) );
        ++i;
    }
    return Publish( publishActionIds, m_publishBulkMsgRemapStorage, notify );
}

/*-------------------------------------------------------------------------*/

bool
CAwsSqsPubSubClientTopic::AcknowledgeReceipt( const COMCORE::CIPubSubMsg& msg )
{GUCEF_TRACE;

    // Does not apply to SQS
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CAwsSqsPubSubClientTopic::AcknowledgeReceipt( const COMCORE::CPubSubBookmark& bookmark )
{GUCEF_TRACE;

    // Does not apply to SQS
    return false;
}

/*-------------------------------------------------------------------------*/

template< class T >
bool
CAwsSqsPubSubClientTopic::TranslateToSqsMsg( T& sqsMsg, const COMCORE::CIPubSubMsg* msg, CORE::UInt32& msgByteSize )
{GUCEF_TRACE;

    if ( GUCEF_NULL == msg )
    {
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "AwsSqsPubSubClientTopic:TranslateToSqsMsg: NULL Message passed" );
        return false;
    }

    const CORE::CVariant& bodyPayload = msg->GetPrimaryPayload();
    if ( bodyPayload.IsInitialized() )
    {
        // A message can include only XML, JSON, and unformatted text. The following Unicode characters are allowed:
        // #x9 | #xA | #xD | #x20 to #xD7FF | #xE000 to #xFFFD | #x10000 to #x10FFFF
        // The minimum size is one character. The maximum size is 256 KB.
            
        // We request the payload as a string. Note that this auto converts
        // Binary is Base64 encoded. For SQS strings are Unicode with UTF-8 binary encoding
        CORE::CUtf8String bodyPayloadStr = bodyPayload.AsUtf8String();
            
        if ( bodyPayloadStr.ByteSize() >= 1 && bodyPayloadStr.ByteSize() <= SQSCLIENT_MAX_PAYLOAD_SIZE )
        {
            sqsMsg.SetMessageBody( bodyPayloadStr );
        }
        else
        {
            GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "AwsSqsPubSubClientTopic:TranslateToSqsMsg: Message body size as string has an invalid size. Must be between 1-256KB. Cannot translate" );
            return false;
        }
    }
    else
    {
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "AwsSqsPubSubClientTopic:TranslateToSqsMsg: Message does not have a body which SQS does not allow. Cannot publish" );
        return false;
    }

    const COMCORE::CIPubSubMsg::TKeyValuePairs& kvPairs = msg->GetKeyValuePairs();
    if ( !kvPairs.empty() )
    {
        typedef Aws::Map< Aws::String, Aws::SQS::Model::MessageAttributeValue > TSqsMsgAttributeMap;

        TSqsMsgAttributeMap attribs;
        COMCORE::CIPubSubMsg::TKeyValuePairs::const_iterator n = kvPairs.begin();
        while ( n != kvPairs.end() )
        {
            const COMCORE::CIPubSubMsg::TKeyValuePair& kvPair = (*n);
            const CORE::CVariant& kvKey = kvPair.first;
            const CORE::CVariant& kvValue = kvPair.second;

            if ( kvKey.IsInitialized() && kvValue.IsInitialized() )
            {                    
                Aws::SQS::Model::MessageAttributeValue sqsAttribValue;

                if ( kvValue.IsBinary() )
                {
                    sqsAttribValue.SetDataType( "Binary" );
                    sqsAttribValue.SetBinaryValue( Aws::Utils::ByteBuffer( static_cast< const unsigned char* >( kvValue.AsVoidPtr() ), kvValue.ByteSize() ) );
                }
                else
                if ( kvValue.IsString() )
                {
                    sqsAttribValue.SetDataType( "String" );
                    sqsAttribValue.SetStringValue( kvValue.AsUtf8String() );
                }
                else
                {
                    sqsAttribValue.SetDataType( "StringValue" );
                    sqsAttribValue.SetStringValue( kvValue.AsUtf8String() );
                }
                    
                sqsMsg.AddMessageAttributes( kvKey.AsUtf8String(), sqsAttribValue ); // if there are dupicate keys last one wins                    
            }
            else
            {
                GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "AwsSqsPubSubClientTopic:Publish: Uninitialized key and/or value set on msg, ignoring" );
            }

            ++n;
        }    
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CAwsSqsPubSubClientTopic::Publish( TPublishActionIdVector& publishActionIds                       , 
                                   const COMCORE::CIPubSubMsg::TIPubSubMsgConstRawPtrVector& msgs ,
                                   bool notify                                                    )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    bool totalSuccess = true;
    try
    {            
        if ( m_config.tryToUseSendMessageBatch )
        {
            Aws::SQS::Model::SendMessageBatchRequest sm_req;
            sm_req.SetQueueUrl( m_queueUrl );
       
            size_t preExistingActionIds = publishActionIds.size(); size_t n=0;
            COMCORE::CIPubSubMsg::TIPubSubMsgConstRawPtrVector::const_iterator i = msgs.begin();
            while ( i != msgs.end() )
            {        
                CORE::UInt64 publishActionId = 0;
                if ( preExistingActionIds > n )
                {
                    publishActionId = publishActionIds[ n ];
                    if ( 0 == publishActionId )
                    {
                        publishActionId = m_currentPublishActionId; 
                        ++m_currentPublishActionId;
                        publishActionIds[ n ] = publishActionId;
                    }
                }
                else
                {
                    publishActionId = m_currentPublishActionId; 
                    ++m_currentPublishActionId;
                    publishActionIds.push_back( publishActionId );
                }

                const COMCORE::CIPubSubMsg* msg = (*i);            
                CORE::UInt32 msgByteSize = 0;
                Aws::SQS::Model::SendMessageBatchRequestEntry sqsMsg;                        
                if ( TranslateToSqsMsg< Aws::SQS::Model::SendMessageBatchRequestEntry >( sqsMsg, msg, msgByteSize ) )
                {
                    sm_req.AddEntries( sqsMsg );
                }
                ++i; ++n;
            }

            if ( msgs.size() != sm_req.GetEntries().size() )
            {
                GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "AwsSqsPubSubClientTopic:Publish: Unable accept " + CORE::ToString( msgs.size() - sm_req.GetEntries().size() ) + " messages out of " + CORE::ToString( msgs.size() ) );
            }

            if ( !sm_req.GetEntries().empty() )
            {
                Aws::SQS::Model::SendMessageBatchOutcome sm_out = m_client->GetSqsClient().SendMessageBatch( sm_req );
                if ( sm_out.IsSuccess() )
                {
                    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "AwsSqsPubSubClientTopic:Publish: Success sending batch message to \"" + m_queueUrl + "\"" );

                    if ( notify && !msgs.empty() )
                        for ( size_t i=0; i<msgs.size(); ++i )    
                            m_publishSuccessActionIds.push_back( publishActionIds[ preExistingActionIds + i ] );
                }
                else
                {
                    totalSuccess = false;
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "AwsSqsPubSubClientTopic:Publish: Error sending batch message to \"" + m_queueUrl + "\". Error Msg: " + sm_out.GetError().GetMessage() );

                    if ( notify && !msgs.empty() )
                        for ( size_t i=0; i<msgs.size(); ++i )    
                            m_publishFailureActionIds.push_back( publishActionIds[ preExistingActionIds + i ] );
                }   
            }
        }
        else
        {
            Aws::SQS::Model::SendMessageRequest sm_req;
            sm_req.SetQueueUrl( m_queueUrl );
       
            size_t preExistingActionIds = publishActionIds.size(); size_t n=0;
            COMCORE::CIPubSubMsg::TIPubSubMsgConstRawPtrVector::const_iterator i = msgs.begin();
            while ( i != msgs.end() )
            {        
                CORE::UInt64 publishActionId = 0;
                if ( preExistingActionIds > n )
                {
                    publishActionId = publishActionIds[ n ];
                    if ( 0 == publishActionId )
                    {
                        publishActionId = m_currentPublishActionId; 
                        ++m_currentPublishActionId;
                        publishActionIds[ n ] = publishActionId;
                    }
                }
                else
                {
                    publishActionId = m_currentPublishActionId; 
                    ++m_currentPublishActionId;
                    publishActionIds.push_back( publishActionId );
                }

                bool success = Publish( publishActionId, *(*i), false );

                if ( notify )
                {
                    if ( success )
                        m_publishSuccessActionIds.push_back( publishActionId );
                    else
                        m_publishFailureActionIds.push_back( publishActionId );
                }

                totalSuccess = success && totalSuccess;                
                ++i; ++n;
            }
        }
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_CRITICAL, CORE::CString( "AwsSqsPubSubClientTopic:GetSqsQueueUrlForQueueName: experienced an exception: " ) + e.what() );
    }
    catch ( ... )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_CRITICAL, "AwsSqsPubSubClientTopic:GetSqsQueueUrlForQueueName: experienced an unknown exception, your application may be unstable" );
    }

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

void
CAwsSqsPubSubClientTopic::OnPulseCycle( CORE::CNotifier* notifier    ,
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

bool 
CAwsSqsPubSubClientTopic::SaveConfig( COMCORE::CPubSubClientTopicConfig& config ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    config = m_config;
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CAwsSqsPubSubClientTopic::LoadConfig( const COMCORE::CPubSubClientTopicConfig& config )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    m_config = config;

    if ( m_config.topicNameIsQueueName )
        m_queueUrl = GetSqsQueueUrlForQueueName( m_config.topicName );
    else
        m_queueUrl = m_config.topicName;

    return true;
}

/*-------------------------------------------------------------------------*/

void
CAwsSqsPubSubClientTopic::PrepStorageForReadMsgs( CORE::UInt32 msgCount )
{GUCEF_TRACE;

    // reset size, note this does not dealloc the underlying memory
    m_pubsubMsgs.clear();
    TPubSubMsgsRefVector& msgRefs = m_pubsubMsgsRefs;
    msgRefs.clear();

    if ( msgCount > m_pubsubMsgs.capacity() )
    {
        // Grow the actual storage, this is allowed to become larger than msgCount to limit
        // the nr of dynamic allocations
        m_pubsubMsgs.reserve( msgCount );        
        msgRefs.reserve( msgCount );
    }
}

/*-------------------------------------------------------------------------*/

bool
CAwsSqsPubSubClientTopic::Disconnect( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );


    return true;
}

/*-------------------------------------------------------------------------*/

bool
CAwsSqsPubSubClientTopic::Subscribe( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CAwsSqsPubSubClientTopic::SubscribeStartingAtBookmark( const COMCORE::CPubSubBookmark& bookmark ) 
{GUCEF_TRACE;

    // Not supported
    return false;
}

/*-------------------------------------------------------------------------*/

COMCORE::CPubSubBookmark 
CAwsSqsPubSubClientTopic::GetCurrentBookmark( void )
{GUCEF_TRACE;

    // Not supported
    return COMCORE::CPubSubBookmark( COMCORE::CPubSubBookmark::BOOKMARK_TYPE_NOT_APPLICABLE );
}

/*-------------------------------------------------------------------------*/

bool 
CAwsSqsPubSubClientTopic::IsConnected( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CAwsSqsPubSubClientTopic::InitializeConnectivity( void )
{GUCEF_TRACE;

    Disconnect();
    
    MT::CScopeMutex lock( m_lock );

    return false;
}

/*-------------------------------------------------------------------------*/

void
CAwsSqsPubSubClientTopic::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                             const CORE::CEvent& eventId  ,
                                             CORE::CICloneable* eventData )
{GUCEF_TRACE;


}

/*-------------------------------------------------------------------------*/

const MT::CILockable* 
CAwsSqsPubSubClientTopic::AsLockable( void ) const
{GUCEF_TRACE;

    return this;
}

/*-------------------------------------------------------------------------*/

 bool
 CAwsSqsPubSubClientTopic::Lock( CORE::UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return m_lock.Lock( lockWaitTimeoutInMs );
}

/*-------------------------------------------------------------------------*/

bool
CAwsSqsPubSubClientTopic::Unlock( void ) const
{GUCEF_TRACE;

    return m_lock.Unlock();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace AWSSQS */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
