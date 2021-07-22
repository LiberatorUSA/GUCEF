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
{GUCEF_TRACE;
        
    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

CAwsSqsPubSubClientTopic::~CAwsSqsPubSubClientTopic()
{GUCEF_TRACE;

    Disconnect();
}


/*-------------------------------------------------------------------------*/

void
CAwsSqsPubSubClientTopic::RegisterEventHandlers( void )
{GUCEF_TRACE;

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

CORE::CString
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

    return CORE::CString::Empty;
}

/*-------------------------------------------------------------------------*/

bool 
CAwsSqsPubSubClientTopic::Publish( const COMCORE::CIPubSubMsg& msg )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    m_publishBulkMsgRemapStorage.clear();
    m_publishBulkMsgRemapStorage.push_back( &msg );
    return Publish( m_publishBulkMsgRemapStorage );
}

/*-------------------------------------------------------------------------*/

bool 
CAwsSqsPubSubClientTopic::Publish( const COMCORE::CBasicPubSubMsg::TBasicPubSubMsgVector& msgs )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    m_publishBulkMsgRemapStorage.resize( msgs.size() );
    COMCORE::CBasicPubSubMsg::TBasicPubSubMsgVector::const_iterator i = msgs.begin();
    while ( i != msgs.end() )    
    {
        m_publishBulkMsgRemapStorage.push_back( &(*i) );
        ++i;
    }
    return Publish( m_publishBulkMsgRemapStorage );
}

/*-------------------------------------------------------------------------*/

bool
CAwsSqsPubSubClientTopic::Publish( const COMCORE::CIPubSubMsg::TIPubSubMsgConstRawPtrVector& msgs )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    bool totalSuccess = true;
    try
    {            
        Aws::SQS::Model::SendMessageBatchRequest sm_req;
        sm_req.SetQueueUrl( m_queueUrl );
       
        COMCORE::CIPubSubMsg::TIPubSubMsgConstRawPtrVector::const_iterator i = msgs.begin();
        while ( i != msgs.end() )
        {        
            const COMCORE::CIPubSubMsg* msg = (*i);
            if ( GUCEF_NULL == msg )
            {
                GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "AwsSqsPubSubClientTopic:Publish: NULL Message passed, ignoring" );
                totalSuccess = false;
                ++i; continue;
            }
            
            Aws::SQS::Model::SendMessageBatchRequestEntry sqsMsg;
                        
            const CORE::CVariant& bodyPayload = msg->GetPrimaryPayload();
            if ( bodyPayload.IsInitialized() )
            {
                // A message can include only XML, JSON, and unformatted text. The following Unicode characters are allowed:
                // #x9 | #xA | #xD | #x20 to #xD7FF | #xE000 to #xFFFD | #x10000 to #x10FFFF
                // The minimum size is one character. The maximum size is 256 KB.
            
                // We request the payload as a string. Note that this auto converts
                // Binary is Base64 encoded
                CORE::CString bodyPayloadStr = bodyPayload.AsString();
            
                if ( bodyPayloadStr.ByteSize() >= 1 && bodyPayloadStr.ByteSize() <= SQSCLIENT_MAX_PAYLOAD_SIZE )
                {
                    sqsMsg.SetMessageBody( bodyPayloadStr );
                }
                else
                {
                    GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "AwsSqsPubSubClientTopic:Publish: Message body size as string has an invalid size. Must be between 1-256KB. Cannot publish" );
                    totalSuccess = false;
                    ++i; continue;
                }
            }
            else
            {
                GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "AwsSqsPubSubClientTopic:Publish: Message does not have a body which SQS does not allow. Cannot publish" );
                totalSuccess = false;
                ++i; continue;
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
                        Aws::SQS::Model::MessageAttributeValue& sqsAttribValue = attribs[ kvKey.AsString() ];  // if there are dupicate keys last one wins

                        if ( kvValue.IsBinary() )
                        {
                            sqsAttribValue.SetDataType( "Binary" );
                            sqsAttribValue.SetBinaryValue( Aws::Utils::ByteBuffer( static_cast< const unsigned char* >( kvValue.AsVoidPtr() ), kvValue.ByteSize() ) );
                        }
                        else
                        if ( kvValue.IsString() )
                        {
                            sqsAttribValue.SetDataType( "String" );
                            sqsAttribValue.SetStringValue( kvValue.AsString() );
                        }
                        else
                        {
                            sqsAttribValue.SetDataType( "StringValue" );
                            sqsAttribValue.SetStringValue( kvValue.AsString() );
                        }
                    }
                    else
                    {
                        GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "AwsSqsPubSubClientTopic:Publish: Uninitialized key and/or value set on msg, ignoring" );
                    }
                    ++n;
                }
            }
        
            sm_req.AddEntries( sqsMsg );

            ++i;
        }

        if ( !sm_req.GetEntries().empty() )
        {
            Aws::SQS::Model::SendMessageBatchOutcome sm_out = m_client->GetSqsClient().SendMessageBatch( sm_req );
            if ( sm_out.IsSuccess() )
            {
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "AwsSqsPubSubClientTopic:Publish: Success sending message to \"" + m_queueUrl + "\"" );
                return totalSuccess;
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "AwsSqsPubSubClientTopic:Publish: Error sending message to \"" + m_queueUrl + "\". Error Msg: " + sm_out.GetError().GetMessage() );
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
    TPubSubMsgsRefVector& msgRefs = m_pubsubMsgsRefs.GetData();
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
CAwsSqsPubSubClientTopic::SubscribeStartingAtMsgId( const CORE::CVariant& msgIdBookmark )
{GUCEF_TRACE;

    // Not supported
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CAwsSqsPubSubClientTopic::SubscribeStartingAtMsgDateTime( const CORE::CDateTime& msgDtBookmark )
{GUCEF_TRACE;

    // Not supported
    return false;
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
