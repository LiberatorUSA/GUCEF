/*
 *  pubsubpluginSTORAGE: Generic GUCEF COMCORE plugin for providing pubsub storage
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

#ifndef GUCEF_CORE_METRICSMACROS_H
#include "gucefCORE_MetricsMacros.h"
#define GUCEF_CORE_METRICSMACROS_H
#endif /* GUCEF_CORE_METRICSMACROS_H ? */

#ifndef GUCEF_COMCORE_CBASICPUBSUBMSG_H
#include "gucefCOMCORE_CBasicPubSubMsg.h"
#define GUCEF_COMCORE_CBASICPUBSUBMSG_H
#endif /* GUCEF_COMCORE_CBASICPUBSUBMSG_H ? */

#ifndef GUCEF_VFS_CVFSGLOBAL_H
#include "gucefVFS_CVfsGlobal.h"
#define GUCEF_VFS_CVFSGLOBAL_H
#endif /* GUCEF_VFS_CVFSGLOBAL_H ? */

#ifndef GUCEF_VFS_CVFS_H
#include "gucefVFS_CVFS.h"
#define GUCEF_VFS_CVFS_H
#endif /* GUCEF_VFS_CVFS_H ? */

#ifndef PUBSUBPLUGIN_STORAGE_CSTORAGEPUBSUBCLIENT_H
#include "pubsubpluginSTORAGE_CStoragePubSubClient.h"
#define PUBSUBPLUGIN_STORAGE_CSTORAGEPUBSUBCLIENT_H
#endif /* PUBSUBPLUGIN_STORAGE_CSTORAGEPUBSUBCLIENT_H ? */

#include "pubsubpluginSTORAGE_CStoragePubSubClientTopic.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUBPLUGIN {
namespace STORAGE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CStoragePubSubClientTopic::StorageToPubSubRequest::StorageToPubSubRequest( void )
    : startDt()
    , endDt()
    , vfsPubSubMsgContainersToPush()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CStoragePubSubClientTopic::StorageToPubSubRequest::StorageToPubSubRequest( const CORE::CDateTime& startDt, const CORE::CDateTime& endDt )
    : startDt( startDt )
    , endDt( endDt )
    , vfsPubSubMsgContainersToPush()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CStoragePubSubClientTopic::StorageToPubSubRequest::StorageToPubSubRequest( const StorageToPubSubRequest& src )
    : startDt( src.startDt )
    , endDt( src.endDt )
    , vfsPubSubMsgContainersToPush( src.vfsPubSubMsgContainersToPush )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

const CORE::CString& 
CStoragePubSubClientTopic::StorageToPubSubRequest::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CORE::CString classTypeName = "StorageToPubSubRequest";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

bool                                                
CStoragePubSubClientTopic::StorageToPubSubRequest::SaveConfig( CORE::CDataNode & tree ) const
{
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CStoragePubSubClientTopic::StorageToPubSubRequest::LoadConfig( const CORE::CDataNode & treeroot )
{
    return false;
}

/*-------------------------------------------------------------------------*/

CStoragePubSubClientTopic::CStoragePubSubClientTopic( CStoragePubSubClient* client )
    : COMCORE::CPubSubClientTopic()
    , m_client( client )
    , m_config()
    , m_syncVfsOpsTimer( GUCEF_NULL )
    , m_reconnectTimer( GUCEF_NULL )
    , m_lock()
    , m_currentPublishActionId( 1 )
    , m_currentReceiveActionId( 1 )
    , m_publishSuccessActionIds()
    , m_publishSuccessActionEventData()
    , m_publishFailureActionIds()
    , m_publishFailureActionEventData()
    , m_metrics()
    , m_metricFriendlyTopicName()
    , m_vfsOpsThread()
{GUCEF_TRACE;
        
    m_publishSuccessActionEventData.LinkTo( &m_publishSuccessActionIds );
    m_publishFailureActionEventData.LinkTo( &m_publishFailureActionIds );

    m_syncVfsOpsTimer = new CORE::CTimer( m_client->GetConfig().pulseGenerator, 25 );

    if ( m_client->GetConfig().desiredFeatures.supportsAutoReconnect )
    {
        m_reconnectTimer = new CORE::CTimer( m_client->GetConfig().pulseGenerator, m_client->GetConfig().reconnectDelayInMs );
    }

    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

CStoragePubSubClientTopic::~CStoragePubSubClientTopic()
{GUCEF_TRACE;

    Disconnect();

    delete m_syncVfsOpsTimer;
    m_syncVfsOpsTimer = GUCEF_NULL;

    delete m_reconnectTimer;
    m_reconnectTimer = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

COMCORE::CPubSubClient*
CStoragePubSubClientTopic::GetClient( void )
{GUCEF_TRACE;

    return m_client;
}

/*-------------------------------------------------------------------------*/

void
CStoragePubSubClientTopic::RegisterEventHandlers( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_reconnectTimer )
    {
        TEventCallback callback( this, &CStoragePubSubClientTopic::OnReconnectTimerCycle );
        SubscribeTo( m_reconnectTimer               ,
                     CORE::CTimer::TimerUpdateEvent ,
                     callback                       );
    }

    if ( GUCEF_NULL != m_syncVfsOpsTimer )
    {
        TEventCallback callback( this, &CStoragePubSubClientTopic::OnSyncVfsOpsTimerCycle );
        SubscribeTo( m_syncVfsOpsTimer              ,
                     CORE::CTimer::TimerUpdateEvent ,
                     callback                       );
    }
    

    TEventCallback callback( this, &CStoragePubSubClientTopic::OnPulseCycle );
    SubscribeTo( m_client->GetConfig().pulseGenerator ,
                 CORE::CPulseGenerator::PulseEvent    ,
                 callback                             );
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::IsPublishingSupported( void )
{GUCEF_TRACE;
    
    return m_config.needPublishSupport;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::IsSubscribingSupported( void )
{GUCEF_TRACE;

    return m_config.needSubscribeSupport;
}

/*-------------------------------------------------------------------------*/

const CORE::CString& 
CStoragePubSubClientTopic::GetTopicName( void ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return m_config.topicName;
}

/*-------------------------------------------------------------------------*/

bool 
CStoragePubSubClientTopic::Publish( CORE::UInt64& publishActionId    , 
                                    const COMCORE::CIPubSubMsg& msg  , 
                                    bool notify                      )
{GUCEF_TRACE;

    TIPubSubMsgConstRawPtrVector msgs;
    msgs.push_back( &msg );

    TPublishActionIdVector publishActionIds;
    bool result = Publish( publishActionIds, msgs, notify );

    if ( !publishActionIds.empty() )
    {
        publishActionId = publishActionIds[ 0 ];
    }

    return result;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::Publish( TPublishActionIdVector& publishActionIds                    , 
                                    const COMCORE::CBasicPubSubMsg::TBasicPubSubMsgVector& msgs , 
                                    bool notify                                                 )
{GUCEF_TRACE;

    CORE::UInt64 msgCount = msgs.size(); 
    TIPubSubMsgConstRawPtrVector msgsPtrs( msgCount );
    
    for ( CORE::UInt64 i=0; i<msgCount; ++i )
    {
        msgsPtrs[ i ] = &msgs[ i ];
    }

    return PublishViaMsgPtrs< const COMCORE::CIPubSubMsg* >( publishActionIds, msgsPtrs, notify );
}

/*-------------------------------------------------------------------------*/

bool 
CStoragePubSubClientTopic::Publish( TPublishActionIdVector& publishActionIds , 
                                    const TIPubSubMsgConstRawPtrVector& msgs , 
                                    bool notify                              )
{GUCEF_TRACE;

    return PublishViaMsgPtrs< const COMCORE::CIPubSubMsg* >( publishActionIds, msgs, notify );
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::Publish( TPublishActionIdVector& publishActionIds ,  
                                    const TIPubSubMsgRawPtrVector& msgs      , 
                                    bool notify                              )
{GUCEF_TRACE;

    return PublishViaMsgPtrs< COMCORE::CIPubSubMsg* >( publishActionIds, msgs, notify );
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::Publish( TPublishActionIdVector& publishActionIds , 
                                    const TIPubSubMsgSPtrVector& msgs        , 
                                    bool notify                              )
{GUCEF_TRACE;

    return PublishViaMsgPtrs< COMCORE::CIPubSubMsg::TNoLockSharedPtr >( publishActionIds, msgs, notify );
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::Publish( TPublishActionIdVector& publishActionIds , 
                                    const TPubSubMsgsRefVector& msgs         , 
                                    bool notify                              )
{GUCEF_TRACE;

    return PublishViaMsgPtrs< TPubSubMsgRef >( publishActionIds, msgs, notify );
}

/*-------------------------------------------------------------------------*/

template < typename T >
bool
CStoragePubSubClientTopic::PublishViaMsgPtrs( TPublishActionIdVector& publishActionIds , 
                                              const std::vector< T >& msgs             , 
                                              bool notify                              )
{GUCEF_TRACE;

    try
    {
        MT::CScopeMutex lock( m_lock );
        
        std::vector< T >::const_iterator i = msgs.begin();
        const CORE::CDateTime& firstMsgDt = (*i)->GetMsgDateTime();

        bool firstBlock = m_lastWriteBlockCompletion == CORE::CDateTime::Empty;
        if ( firstBlock )
        {                
            m_lastWriteBlockCompletion = CORE::CDateTime::NowUTCDateTime();
        }
        
        TStorageBufferMetaData* bufferMetaData = GUCEF_NULL;
        if ( GUCEF_NULL == m_msgReceiveBuffer )
        {
            m_msgReceiveBuffer = m_buffers.GetNextWriterBuffer( firstMsgDt, true, GUCEF_MT_INFINITE_LOCK_TIMEOUT );
            if ( GUCEF_NULL == m_msgReceiveBuffer )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "StoragePubSubClientTopic(" + CORE::PointerToString( this ) +
                    "):PublishViaMsgPtrs: Failed to obtain new message serialization buffer" );
                return false;
            }

            CORE::UInt32 newBytesWritten = 0;
            if ( !COMCORE::CPubSubMsgContainerBinarySerializer::SerializeHeader( m_config.pubsubBinarySerializerOptions, 0, *m_msgReceiveBuffer, newBytesWritten ) )
            {
                // We carry on best effort but this is really bad
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "StoragePubSubClientTopic(" + CORE::PointerToString( this ) +
                    "):PublishViaMsgPtrs: Failed to write container header at start of new pub-sub msg container" );
            }
            
            // We are (re)using a new buffer, as such make sure we reset the meta-data that may be pre-existing
            bufferMetaData = &( m_storageBufferMetaData[ m_msgReceiveBuffer ] );
            bufferMetaData->msgOffsetIndex.clear();
            bufferMetaData->publishActionIds.clear();
        }
        else
        {
            // fetch the meta-data for this buffer
            bufferMetaData = &( m_storageBufferMetaData[ m_msgReceiveBuffer ] );
        }

        CORE::UInt32 bufferOffset = m_msgReceiveBuffer->GetDataSize();        
        CORE::UInt64 publishActionId = 0;
        while ( i != msgs.end() )    
        {
            // see if we have a pre-existing action id (retry)
            TPublishActionIdVector::iterator publishActionIdIttr = publishActionIds.begin();
            if ( publishActionIdIttr != publishActionIds.end() )
                publishActionId = (*publishActionIdIttr);

            // Assign a new action id if we dont yet have one
            if ( 0 == publishActionId )
            {
                publishActionId = m_currentPublishActionId; 
                ++m_currentPublishActionId;
            }
            
            // Persist the action id generated on the output param
            if ( publishActionIdIttr == publishActionIds.end() )
                publishActionIds.push_back( publishActionId );
            else
                (*publishActionIdIttr) = publishActionId;

            // Serialize the message
            CORE::UInt32 ticks = CORE::GUCEFGetTickCount();
            CORE::UInt32 msgBytesWritten = 0;
            if ( COMCORE::CPubSubMsgBinarySerializer::Serialize( m_config.pubsubBinarySerializerOptions, *(*i), bufferOffset, *m_msgReceiveBuffer, msgBytesWritten ) )
            {
                // add to our meta-data tracking
                bufferMetaData->msgOffsetIndex.push_back( bufferOffset );
                bufferMetaData->publishActionIds.push_back( publishActionId );

                ticks = CORE::GUCEFGetTickCount() - ticks;
                bufferOffset += msgBytesWritten;
                m_msgReceiveBuffer->SetDataSize( bufferOffset );

                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "StoragePubSubClientTopic(" + CORE::PointerToString( this ) +
                    "):PublishViaMsgPtrs: Serialized a message with serialized size " + CORE::ToString( msgBytesWritten ) + 
                    ". This took " + CORE::ToString( ticks ) + "ms. publishActionId=" + CORE::ToString( publishActionId ) );
            }
            else
            {
                // End of the road for this message
                m_publishFailureActionIds.push_back( publishActionId );
                
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "StoragePubSubClientTopic(" + CORE::PointerToString( this ) +
                    "):PublishViaMsgPtrs: Failed to serialize a message. publishActionId=" + CORE::ToString( publishActionId ) );
            }

            ++i;
        }

        // Check to see if we have gathered enough data or enough time has passed to consider the current container complete
        if ( m_msgReceiveBuffer->GetDataSize() >= m_config.desiredMinimalSerializedBlockSize ||
            ( !firstBlock && m_lastWriteBlockCompletion.GetTimeDifferenceInMillisecondsToNow() >= m_config.desiredMaxTimeToWaitToGrowSerializedBlockSizeInMs ) )
        {
            // The current container is now considered to have enough content.
            // Let's wrap things up...
                
            CORE::UInt32 newBytesWritten = 0;
            if ( !COMCORE::CPubSubMsgContainerBinarySerializer::SerializeFooter( bufferMetaData->msgOffsetIndex, bufferOffset, *m_msgReceiveBuffer, newBytesWritten ) )
            {
                // We carry on best effort but this is really bad
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "StoragePubSubClientTopic(" + CORE::PointerToString( this ) +
                    "):PublishViaMsgPtrs: Failed to write container footer at end of current pub-sub msg container" );
            }

            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic(" + CORE::PointerToString( this ) +
                "):PublishViaMsgPtrs: Completed a serialized msg data block of size " + CORE::ToString( bufferOffset ) );                
                
            m_msgReceiveBuffer = GUCEF_NULL;
            m_lastWriteBlockCompletion = CORE::CDateTime::NowUTCDateTime();
        }            

        return true;
    }
    catch ( const std::exception& e )
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic(" + CORE::PointerToString( this ) + "):PublishViaMsgPtrs: exception: " + CORE::CString( e.what() ) );
        return false;
    }
}

/*-------------------------------------------------------------------------*/

bool 
CStoragePubSubClientTopic::SetupToSubscribe( COMCORE::CPubSubClientTopicConfig& config )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    if ( LoadConfig( config ) )
    {

        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CStoragePubSubClientTopic::SaveConfig( COMCORE::CPubSubClientTopicConfig& config ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    config = m_config;
    return true;
}

/*-------------------------------------------------------------------------*/

CORE::CString
CStoragePubSubClientTopic::GenerateMetricsFriendlyTopicName( const CORE::CString& topicName )
{GUCEF_TRACE;

    // Let's avoid non-ASCII stumbling blocks and force the down to ASCII
    CORE::CAsciiString asciiMetricsFriendlyTopicName = topicName.ForceToAscii( '_' );
    
    // Replace special chars
    static const char disallowedChars[] = { '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '=', '+', ',', '.', '<', '>', '/', '?', '`', '~', '\\', '|', '{', '}', '[', ']', ';', ':', '\'', '\"' };
    asciiMetricsFriendlyTopicName = asciiMetricsFriendlyTopicName.ReplaceChars( disallowedChars, sizeof(disallowedChars)/sizeof(char), '_' );

    // Back to the platform wide string convention format
    CORE::CString metricsFriendlyTopicName = CORE::ToString( asciiMetricsFriendlyTopicName );
    return metricsFriendlyTopicName;
}

/*-------------------------------------------------------------------------*/

bool 
CStoragePubSubClientTopic::LoadConfig( const COMCORE::CPubSubClientTopicConfig& config )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    m_config = config;

    m_metricFriendlyTopicName = GenerateMetricsFriendlyTopicName( m_config.topicName );

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::Disconnect( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    m_syncVfsOpsTimer->SetEnabled( false );

    m_buffers.SignalEndOfWriting();

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::BeginVfsOps( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    if ( !m_config.performVfsOpsASync )
    {
        if ( GUCEF_NULL != m_syncVfsOpsTimer )
            m_syncVfsOpsTimer->SetEnabled( true );
    }
    else
    if ( m_config.performVfsOpsInDedicatedThread )
    {
        if ( m_vfsOpsThread.IsNULL() )
        {
            m_vfsOpsThread = CStoragePubSubClientTopicVfsTaskPtr( new CStoragePubSubClientTopicVfsTask( this ) );
        }
        if ( !m_vfsOpsThread.IsNULL() )
        {
            if ( !m_client->GetThreadPool()->StartTask( m_vfsOpsThread ) )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "StoragePubSubClientTopic(" + CORE::PointerToString( this ) + "):BeginVfsOps: Failed to start dedicated VFS thread for async operations" );
                return false;
            }
        }
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::Subscribe( void )
{GUCEF_TRACE;

    bool success = BeginVfsOps();
    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:Subscribe: 'subscribe to topic' for Topic Name: " + m_config.topicName );
    return success;
}

/*-------------------------------------------------------------------------*/

bool 
CStoragePubSubClientTopic::SubscribeStartingAtMsgId( const CORE::CVariant& msgIdBookmark )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    // Currently not supported.
    // Could theoretically be implemented using a cursor combined with PEEK only reads
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::SubscribeStartingAtMsgDateTime( const CORE::CDateTime& msgDtBookmark )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    // Currently not supported.
    // Could theoretically be implemented using a cursor combined with PEEK only reads
    // For regular queue consuming reads you could perhaps ignore messages with a send time older then msgDtBookmark
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CStoragePubSubClientTopic::SubscribeStartingAtBookmark( const COMCORE::CPubSubBookmark& bookmark ) 
{GUCEF_TRACE;

    // Currently not supported.
    return false;
}

/*-------------------------------------------------------------------------*/

COMCORE::CPubSubBookmark 
CStoragePubSubClientTopic::GetCurrentBookmark( void )
{GUCEF_TRACE;

    // Not supported
    return COMCORE::CPubSubBookmark( COMCORE::CPubSubBookmark::BOOKMARK_TYPE_NOT_APPLICABLE );
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::AcknowledgeReceipt( const COMCORE::CIPubSubMsg& msg )
{GUCEF_TRACE;

    COMCORE::CPubSubBookmark bookmark( COMCORE::CPubSubBookmark::BOOKMARK_TYPE_TOPIC_INDEX, msg.GetMsgIndex() );
    return AcknowledgeReceiptImpl( bookmark, msg.GetReceiveActionId() );
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::AcknowledgeReceipt( const COMCORE::CPubSubBookmark& bookmark )
{GUCEF_TRACE;
    
    return AcknowledgeReceiptImpl( bookmark, 0 );
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::AcknowledgeReceiptImpl( const COMCORE::CPubSubBookmark& bookmark , 
                                                   CORE::UInt64 receiveActionId             )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    const CStoragePubSubClientConfig& clientConfig = m_client->GetConfig();

    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CStoragePubSubClientTopic::IsConnected( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
           
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::InitializeConnectivity( void )
{GUCEF_TRACE;

    m_buffers.SetMinimalBufferSize( m_config.desiredMinimalSerializedBlockSize );
    
    if ( m_config.performVfsOpsInDedicatedThread )
    {
        
    }
    
    return true;
}

/*-------------------------------------------------------------------------*/

const CStoragePubSubClientTopicConfig& 
CStoragePubSubClientTopic::GetTopicConfig( void ) const
{GUCEF_TRACE;

    return m_config;
}

/*-------------------------------------------------------------------------*/

void
CStoragePubSubClientTopic::OnReconnectTimerCycle( CORE::CNotifier* notifier    ,
                                                  const CORE::CEvent& eventId  ,
                                                  CORE::CICloneable* eventData )
{GUCEF_TRACE;

    bool totalSuccess = true;
    if ( m_config.needPublishSupport )
        totalSuccess = InitializeConnectivity() && totalSuccess;
    if ( m_config.needSubscribeSupport )
        totalSuccess = Subscribe() && totalSuccess;

    m_reconnectTimer->SetEnabled( !totalSuccess );
}

/*-------------------------------------------------------------------------*/

void
CStoragePubSubClientTopic::PerformASyncVfsWork( void )
{GUCEF_TRACE;
    
    switch ( m_config.mode )
    {
        case TChannelMode::CHANNELMODE_STORAGE_TO_PUBSUB:
        {
            ProcessNextStorageToPubSubRequest();
            TransmitNextPubSubMsgBuffer();
            return;
        }
        case TChannelMode::CHANNELMODE_PUBSUB_TO_STORAGE:
        {
            StoreNextReceivedPubSubBuffer();
            return;
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CStoragePubSubClientTopic::OnSyncVfsOpsTimerCycle( CORE::CNotifier* notifier    ,
                                                   const CORE::CEvent& eventId  ,
                                                   CORE::CICloneable* eventData )
{GUCEF_TRACE;

    switch ( m_config.mode )
    {
        case TChannelMode::CHANNELMODE_STORAGE_TO_PUBSUB:
        {
            ProcessNextStorageToPubSubRequest();
            TransmitNextPubSubMsgBuffer();
            return;
        }
        case TChannelMode::CHANNELMODE_PUBSUB_TO_STORAGE:
        {
            StoreNextReceivedPubSubBuffer();
            return;
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CStoragePubSubClientTopic::OnPulseCycle( CORE::CNotifier* notifier    ,
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

CStoragePubSubClientTopic::TopicMetrics::TopicMetrics( void )
    : queuedReadyToReadBuffers( 0 )
    , smallestBufferSizeInBytes( 0 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

const CStoragePubSubClientTopic::TopicMetrics& 
CStoragePubSubClientTopic::GetMetrics( void ) const
{GUCEF_TRACE;

    return m_metrics;
}

/*-------------------------------------------------------------------------*/

const CORE::CString& 
CStoragePubSubClientTopic::GetMetricFriendlyTopicName( void ) const
{GUCEF_TRACE;

    return m_metricFriendlyTopicName;
}

/*-------------------------------------------------------------------------*/

void
CStoragePubSubClientTopic::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                                const CORE::CEvent& eventId  ,
                                                CORE::CICloneable* eventData )
{GUCEF_TRACE;

    const COMCORE::CPubSubClientConfig& clientConfig = m_client->GetConfig();
    
    m_metrics.queuedReadyToReadBuffers = m_buffers.GetBuffersQueuedToRead();
    m_metrics.smallestBufferSizeInBytes = m_buffers.GetSmallestBufferSize();
    
    if ( clientConfig.desiredFeatures.supportsPublishing )
    {
    }
    if ( clientConfig.desiredFeatures.supportsSubscribing )
    {
    }
}

/*-------------------------------------------------------------------------*/

const MT::CILockable* 
CStoragePubSubClientTopic::AsLockable( void ) const
{GUCEF_TRACE;

    return this;
}

/*-------------------------------------------------------------------------*/

 bool
 CStoragePubSubClientTopic::Lock( CORE::UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return m_lock.Lock( lockWaitTimeoutInMs );
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::Unlock( void ) const
{GUCEF_TRACE;

    return m_lock.Unlock();
}

/*-------------------------------------------------------------------------*/

CORE::CString
CStoragePubSubClientTopic::GetPathToLastWrittenPubSubStorageFile( CORE::UInt32 lastOffset ) const
{GUCEF_TRACE;

    VFS::CVFS& vfs = VFS::CVfsGlobal::Instance()->GetVfs();
    
    CORE::CString fileFilter = '*' + m_vfsFilePostfix;
    VFS::CVFS::TStringSet index;
    vfs.GetList( index, m_config.vfsStorageRootPath, false, true, fileFilter, true, false );

    // The index is already alphabetically ordered and since we use the datetime as the part of filename we can leverage that
    // to get the last produced file
    if ( !index.empty() )
    {        
        VFS::CVFS::TStringSet::reverse_iterator f = index.rbegin();
        CORE::UInt32 n=0;
        while ( n<lastOffset && f != index.rend() )   
        {
            ++f; ++n;
        }

        if ( f != index.rend() )
        {
            const CORE::CString& lastFilename = (*f);            
            return lastFilename;
        }
    }
    return CORE::CString::Empty;
}

/*-------------------------------------------------------------------------*/

bool 
CStoragePubSubClientTopic::GetLastPersistedMsgAttributes( CORE::Int32 channelId          , 
                                                          const CORE::CString& topicName , 
                                                          CORE::CVariant& msgId          , 
                                                          CORE::CDateTime& msgDt         )
{GUCEF_TRACE;

    bool success = true;
    CORE::UInt32 lastFileOffset = 0;
    bool fileExistedButHasIssue = false;
    do
    {
        success = GetLastPersistedMsgAttributesWithOffset( channelId              ,
                                                           topicName              ,
                                                           msgId                  ,
                                                           msgDt                  ,
                                                           lastFileOffset         ,
                                                           fileExistedButHasIssue );
        ++lastFileOffset;
    }
    while ( !success && fileExistedButHasIssue );
    return success;
}

/*-------------------------------------------------------------------------*/

bool 
CStoragePubSubClientTopic::GetPersistedBookmark( CORE::Int32 channelId              , 
                                                 const CORE::CString& topicName     , 
                                                 COMCORE::CPubSubBookmark& bookmark )
{GUCEF_TRACE;

    // @TODO: Update to use dedicated bookmark persistance

    CORE::CVariant msgId;
    CORE::CDateTime msgDt; 
    if ( GetLastPersistedMsgAttributes( channelId, topicName, msgId, msgDt ) )
    {
        if ( msgId.IsInitialized() )
        {
            bookmark.SetBookmarkData( msgId );
            bookmark.SetBookmarkType( COMCORE::CPubSubBookmark::BOOKMARK_TYPE_MSG_ID );
        }
        else
        {
            CORE::CVariant dtStrVar = msgDt.ToIso8601DateTimeString( true, true );
            bookmark.SetBookmarkData( dtStrVar );
            bookmark.SetBookmarkType( COMCORE::CPubSubBookmark::BOOKMARK_TYPE_MSG_DATETIME );
        }
        return true;
    }

    bookmark.SetBookmarkType( COMCORE::CPubSubBookmark::BOOKMARK_TYPE_NOT_AVAILABLE );
    return false;
}

/*-------------------------------------------------------------------------*/

CORE::CDynamicBufferSwap& 
CStoragePubSubClientTopic::GetSerializedMsgBuffers( void )
{GUCEF_TRACE;

    return m_buffers;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::LoadStorageFile( const CORE::CString& vfsPath       ,
                                            CORE::CDynamicBuffer& targetBuffer )
{GUCEF_TRACE;


    VFS::CVFS& vfs = VFS::CVfsGlobal::Instance()->GetVfs();
        
    if ( !m_config.decodeCodecFamily.IsNULLOrEmpty() && !m_config.decodeCodecName.IsNULLOrEmpty() )
    {
        CORE::Float32 encodeRatio = m_encodeSizeRatio < 0 ? m_config.defaultCodecDecodeGrowthRatioExpectation : m_encodeSizeRatio;
        CORE::UInt32 estimatedApproxDecodedSize = (CORE::UInt32) ( vfs.GetFileSize( vfsPath ) * encodeRatio );
        targetBuffer.SetBufferSize( estimatedApproxDecodedSize, false );

        if ( !vfs.DecodeAsFile( targetBuffer, 0, vfsPath, m_config.decodeCodecFamily, m_config.decodeCodecName ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:LoadStorageFile: Cannot decode and load persisted file. CodeFamily:" + m_config.decodeCodecFamily +
                " CodecName: " + m_config.decodeCodecName + ". VFS File: " + vfsPath );
            return false;
        }

        if ( targetBuffer.GetDataSize() > 0 )
            m_encodeSizeRatio = (CORE::Float32) ( targetBuffer.GetDataSize() / vfs.GetFileSize( vfsPath ) );
    }
    else
    {
        // Not using any encoding, load the file as-is
        if ( !vfs.LoadFile( targetBuffer, vfsPath, "rb" ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:LoadStorageFile: Cannot load last persisted file. VFS File: " + vfsPath );
            return false;
        }
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CStoragePubSubClientTopic::GetLastPersistedMsgAttributesWithOffset( CORE::Int32 channelId          , 
                                                                    const CORE::CString& topicName , 
                                                                    CORE::CVariant& msgId          , 
                                                                    CORE::CDateTime& msgDt         ,
                                                                    CORE::UInt32 lastFileOffset    ,
                                                                    bool& fileExistedButHasIssue   )
{GUCEF_TRACE;

    // @TODO: topic name segregation
    
    if ( m_lastPersistedMsgId.IsNULLOrEmpty() && m_lastPersistedMsgDt == CORE::CDateTime::Empty )
    {
        CORE::CString lastWrittenFilePath = GetPathToLastWrittenPubSubStorageFile( lastFileOffset );
        if ( lastWrittenFilePath.IsNULLOrEmpty() )
        {
            fileExistedButHasIssue = false;
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:GetLastWrittenPubSubMsgId: Cannot obtain path to last written file with offset " + CORE::ToString( lastFileOffset ) );
            return false;
        }

        CORE::CDynamicBuffer lastStorageFileContent;
        if ( !LoadStorageFile( lastWrittenFilePath, lastStorageFileContent ) )
        {
            fileExistedButHasIssue = false;
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:GetLastWrittenPubSubMsgId: Unable to load file from storage. Loading using last offset " + CORE::ToString( lastFileOffset ) );
            return false;
        }

        if ( 0 == lastStorageFileContent.GetDataSize() )
        {
            fileExistedButHasIssue = true;
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:GetLastPersistedMsgAttributes: last persisted file is empty. VFS File: " + lastWrittenFilePath );
            return false;
        }

        bool isCorrupted = false;
        COMCORE::CBasicPubSubMsg msg;
        if ( !COMCORE::CPubSubMsgContainerBinarySerializer::DeserializeMsgAtIndex( msg, true, lastStorageFileContent, 0, false, isCorrupted ) )
        {
            if ( isCorrupted )
            {
                // Attempt to recover what we can with an index rebuild
                // This could effectively move the "last" message received to the actually non-corrupt persisted message as the new "last"
                GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:GetLastPersistedMsgAttributes: Failed to deserialize the last message, will attempt an index rebuild of the corrupt container" );
               
                CORE::UInt32 bytesRead = 0;
                COMCORE::CPubSubMsgContainerBinarySerializer::TMsgOffsetIndex newRecoveredIndex;
                if ( COMCORE::CPubSubMsgContainerBinarySerializer::IndexRebuildScan( newRecoveredIndex, lastStorageFileContent, bytesRead ) )
                {
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:GetLastPersistedMsgAttributes: Successfully performed an index rebuild of the corrupt container, discovered " + CORE::ToString( newRecoveredIndex.size() ) + " messages. Will attempt to add a new footer" );
                    
                    CORE::UInt32 bytesWritten = 0;
                    if ( COMCORE::CPubSubMsgContainerBinarySerializer::SerializeFooter( newRecoveredIndex, lastStorageFileContent.GetDataSize()-1, lastStorageFileContent, bytesWritten ) )
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:GetLastPersistedMsgAttributes: Successfully serialized a new footer to the previously corrupt container. Will attempt to persist the amended container" );

                        VFS::CVFS& vfs = VFS::CVfsGlobal::Instance()->GetVfs();
                        
                        if ( m_config.encodeCodecFamily.IsNULLOrEmpty() || m_config.encodeCodecName.IsNULLOrEmpty() )
                        {
                            if ( vfs.StoreAsFile( lastWrittenFilePath, lastStorageFileContent, 0, true ) )
                            {
                                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:GetLastPersistedMsgAttributes: Successfully stored rebuild pub-sub message container at: " + lastWrittenFilePath );
                            }
                            else
                            {
                                fileExistedButHasIssue = true;
                                GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "StoragePubSubClientTopic:GetLastPersistedMsgAttributes: StoreAsFile() Failed for rebuild message container" );
                                return false;
                            }
                        }
                        else
                        {
                            if ( vfs.EncodeAsFile( lastStorageFileContent, 0, lastWrittenFilePath, true, m_config.encodeCodecFamily, m_config.encodeCodecName ) )
                            {
                                m_encodeSizeRatio = (CORE::Float32) ( lastStorageFileContent.GetDataSize() / vfs.GetFileSize( lastWrittenFilePath ) );
                                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:GetLastPersistedMsgAttributes: Successfully encoded and stored rebuild pub-sub message container resource at: \"" + lastWrittenFilePath + "\" with a encoded size ratio of " + CORE::ToString( m_encodeSizeRatio ) );
                            }
                            else
                            {
                                fileExistedButHasIssue = true;
                                GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "StoragePubSubClientTopic:GetLastPersistedMsgAttributes: EncodeAsFile() Failed for rebuild message container" );
                                return false;
                            }
                        }
                    }
                }

                // Lets try again, hopefully its fixed now best effort...
                if ( !COMCORE::CPubSubMsgContainerBinarySerializer::DeserializeMsgAtIndex( msg, true, lastStorageFileContent, 0, false, isCorrupted ) )
                {
                    // This should not happen, something is seriously wrong here.
                    fileExistedButHasIssue = true;
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "StoragePubSubClientTopic:GetLastPersistedMsgAttributes: Failed to load last message even after a successfull rebuild. isCorrupted=" + CORE::ToString( isCorrupted ) );
                    return false;
                }
            }
        }

        m_lastPersistedMsgId = msg.GetMsgId();
        m_lastPersistedMsgDt = msg.GetMsgDateTime();
    }

    msgId = m_lastPersistedMsgId;
    msgDt = m_lastPersistedMsgDt;
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::GetStartAndEndFromContainerFilename( const CORE::CString& fullPath ,
                                                                CORE::CDateTime& startDt      ,
                                                                CORE::CDateTime& endDt        ) const
{GUCEF_TRACE;

    // first strip the extra stuff from the full path to get the string form timestamps
    CORE::CString segment = CORE::ExtractFilename( fullPath );
    segment = segment.CutChars( m_vfsFilePostfix.Length(), false, 0 );
    CORE::CString startDtSegment = segment.SubstrToChar( '_', true );
    CORE::CString endDtSegment = segment.SubstrToChar( '_', false );

    // Try to parse what is left as a valid ISO 8601 DateTime
    if ( startDt.FromIso8601DateTimeString( startDtSegment ) && endDt.FromIso8601DateTimeString( endDtSegment ) )
        return true;
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::GetPathsToPubSubStorageFiles( const CORE::CDateTime& startDt  ,
                                                         const CORE::CDateTime& endDt    ,
                                                         CORE::CString::StringSet& files ) const
{GUCEF_TRACE;
   
    
    VFS::CVFS& vfs = VFS::CVfsGlobal::Instance()->GetVfs();
    
    CORE::CString fileFilter = '*' + m_vfsFilePostfix;
    VFS::CVFS::TStringSet index;
    vfs.GetList( index, m_config.vfsStorageRootPath, false, true, fileFilter, true, false );

    VFS::CVFS::TStringSet::iterator i = index.begin();
    while ( i != index.end() )
    {        
        CORE::CDateTime containerFileFirstMsgDt;
        CORE::CDateTime containerFileLastMsgDt;
        if ( GetStartAndEndFromContainerFilename( (*i), containerFileFirstMsgDt, containerFileLastMsgDt ) )
        {
            // Check the container first messgage dt against the our time range
            // It is assumed here that the containers have messages chronologically ordered
            if ( containerFileFirstMsgDt.IsWithinRange( startDt, endDt ) || containerFileLastMsgDt.IsWithinRange( startDt, endDt ) )
            {
                files.insert( (*i) );
            }
        }
        ++i;
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::StoreNextReceivedPubSubBuffer( void )
{GUCEF_TRACE;

    CORE::CDynamicBufferSwap& buffers = GetSerializedMsgBuffers();
        
    CORE::CDateTime msgBatchDt;
    m_msgReceiveBuffer = buffers.GetNextReaderBuffer( msgBatchDt, false, 25 );
    if ( GUCEF_NULL != m_msgReceiveBuffer )
    {
        // Get the timestamp of the last message in the buffer.
        // This is not as expensive an operation as it would appear because we just link to the bytes in the buffer we dont copy them
        bool isCorrupted = false;
        COMCORE::CBasicPubSubMsg lastMsg;
        COMCORE::CPubSubMsgContainerBinarySerializer::DeserializeMsgAtIndex( lastMsg, true, *m_msgReceiveBuffer, 0, false, isCorrupted );

        CORE::CString vfsFilename = msgBatchDt.ToIso8601DateTimeString( false, true ) + '_' + lastMsg.GetMsgDateTime().ToIso8601DateTimeString( false, true ) + m_vfsFilePostfix;
        CORE::CString vfsStoragePath = CORE::CombinePath( m_config.vfsStorageRootPath, vfsFilename );
            
        VFS::CVFS& vfs = VFS::CVfsGlobal::Instance()->GetVfs();

        if ( m_config.encodeCodecFamily.IsNULLOrEmpty() || m_config.encodeCodecName.IsNULLOrEmpty() )
        {
            if ( vfs.StoreAsFile( vfsStoragePath, *m_msgReceiveBuffer, 0, true ) )
            {
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:StoreNextReceivedPubSubBuffer: Successfully stored pub-sub mesage block at: " + vfsStoragePath );
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "StoragePubSubClientTopic:StoreNextReceivedPubSubBuffer: StoreAsFile() Failed" );
            }
        }
        else
        {
            if ( vfs.EncodeAsFile( *m_msgReceiveBuffer, 0, vfsStoragePath, true, m_config.encodeCodecFamily, m_config.encodeCodecName ) )
            {
                m_encodeSizeRatio = (CORE::Float32) ( m_msgReceiveBuffer->GetDataSize() / ( 1.0f * vfs.GetFileSize( vfsStoragePath ) ) );
                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:StoreNextReceivedPubSubBuffer: Successfully encoded and stored pub-sub mesage block at: \"" + vfsStoragePath + "\" with a encoded size ratio of " + CORE::ToString( m_encodeSizeRatio ) );
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "StoragePubSubClientTopic:StoreNextReceivedPubSubBuffer: EncodeAsFile() Failed" );
            }
        }
    }

    return true;
}

/*-------------------------------------------------------------------------*/

void
CStoragePubSubClientTopic::OnUnableToFullFillStorageToPubSubRequest( const StorageToPubSubRequest& failedRequest )
{GUCEF_TRACE;

    
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::ProcessNextStorageToPubSubRequest( void )
{GUCEF_TRACE;

    StorageToPubSubRequestDeque::iterator i = m_storageToPubSubRequests.begin();
    if ( i != m_storageToPubSubRequests.end() )
    {
        StorageToPubSubRequest& queuedRequest = (*i);

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:ProcessNextStorageToPubSubRequest: Request for messages in range " + 
            CORE::ToString( queuedRequest.startDt ) + " to " + CORE::ToString( queuedRequest.endDt ) );

        if ( queuedRequest.vfsPubSubMsgContainersToPush.empty() )
        {
            if ( !GetPathsToPubSubStorageFiles( queuedRequest.startDt                      ,
                                                queuedRequest.endDt                        ,
                                                queuedRequest.vfsPubSubMsgContainersToPush ) )
            {
                GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:ProcessNextStorageToPubSubRequest: Did not obtain any storage paths for time range " +
                    queuedRequest.startDt.ToIso8601DateTimeString( true, true ) + " to " + queuedRequest.endDt.ToIso8601DateTimeString( true, true ) );
                
                OnUnableToFullFillStorageToPubSubRequest( queuedRequest );
                m_storageToPubSubRequests.pop_back();
                return false;    
            }
        }

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:ProcessNextStorageToPubSubRequest: Available data in the request range spans " + 
            CORE::ToString( queuedRequest.vfsPubSubMsgContainersToPush.size() ) + " containers" );
    
        size_t containersProcessed = 0;
        CORE::CString::StringSet::iterator n = queuedRequest.vfsPubSubMsgContainersToPush.begin();
        while ( n != queuedRequest.vfsPubSubMsgContainersToPush.end() )
        {
            bool needContainerSubsetOnly = false;
            bool containerStartIsInRange = true;
            bool containerEndIsInRange = true;

            CORE::CDateTime containerFileFirstMsgDt;
            CORE::CDateTime containerFileLastMsgDt;
            if ( GetStartAndEndFromContainerFilename( (*n), containerFileFirstMsgDt, containerFileLastMsgDt ) )
            {
                containerStartIsInRange = containerFileFirstMsgDt.IsWithinRange( queuedRequest.startDt, queuedRequest.endDt );
                containerEndIsInRange = containerFileLastMsgDt.IsWithinRange( queuedRequest.startDt, queuedRequest.endDt );                 
                needContainerSubsetOnly = !( containerStartIsInRange && containerEndIsInRange );

                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:ProcessNextStorageToPubSubRequest: Parsed file path container start and end DateTimes. Start=" +
                    CORE::ToString( containerFileFirstMsgDt ) + ", End=" + CORE::ToString( containerFileLastMsgDt ) + ". containerStartIsInRange=" + CORE::ToString( containerStartIsInRange ) +
                    ", containerEndIsInRange=" + CORE::ToString( containerEndIsInRange ) + ", needContainerSubsetOnly=" + CORE::ToString( needContainerSubsetOnly ) );
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:ProcessNextStorageToPubSubRequest: Failed to parse start and/or end DateTime from file path: " + (*n) );
            }
            
            if ( needContainerSubsetOnly )
            {
                if ( GUCEF_NULL == m_msgReceiveBuffer )
                    m_msgReceiveBuffer = GetSerializedMsgBuffers().GetNextWriterBuffer( containerStartIsInRange ? containerFileFirstMsgDt : queuedRequest.startDt, true, GUCEF_MT_INFINITE_LOCK_TIMEOUT );

                if ( GUCEF_NULL != m_msgReceiveBuffer )
                {
                    if ( LoadStorageFile( (*n), *m_msgReceiveBuffer ) )
                    {
                        CORE::UInt32 bytesRead = 0;
                        COMCORE::CPubSubMsgContainerBinarySerializer::TMsgOffsetIndex originalOffsetIndex;
                        COMCORE::CPubSubMsgContainerBinarySerializer::DeserializeFooter( originalOffsetIndex, *m_msgReceiveBuffer, bytesRead );

                        // Since we loaded the entire container we need to now efficiently make sure only the subset gets processed
                        // The way we can do that is by editing the footer in the buffer to logically eliminate entries we do not need
                        // This will make it appear as if only the needed entries are in the container to the reader when reading the footer
                        CORE::UInt32 startIndexOffset = 0;
                        CORE::UInt32 endIndexOffset = 0;
                        bool isCorrupted = false;                        
                        COMCORE::CPubSubMsgContainerBinarySerializer::TBasicPubSubMsgVector msgs;                        
                        if ( COMCORE::CPubSubMsgContainerBinarySerializer::Deserialize( msgs, true, originalOffsetIndex, *m_msgReceiveBuffer, isCorrupted ) )
                        {
                            // Check to see how many we need to trim from the start

                            if ( !containerStartIsInRange )
                            {
                                COMCORE::CPubSubMsgContainerBinarySerializer::TBasicPubSubMsgVector::iterator m = msgs.begin();
                                while ( m != msgs.end() )    
                                {
                                    if ( (*m).GetMsgDateTime() >= queuedRequest.startDt )
                                        break;                         
                                    ++m; ++startIndexOffset;
                                }
                            }
                            if ( !containerEndIsInRange )
                            {
                                COMCORE::CPubSubMsgContainerBinarySerializer::TBasicPubSubMsgVector::reverse_iterator m = msgs.rbegin();
                                while ( m != msgs.rend() )    
                                {
                                    if ( (*m).GetMsgDateTime() <= queuedRequest.endDt )
                                        break;                         
                                    ++m; ++endIndexOffset;
                                }
                            }

                            CORE::UInt32 o2=0;
                            std::size_t newIndexSize = originalOffsetIndex.size() - ( (std::size_t)startIndexOffset + (std::size_t)endIndexOffset );
                            endIndexOffset = (CORE::UInt32) originalOffsetIndex.size() - endIndexOffset;
                            COMCORE::CPubSubMsgContainerBinarySerializer::TMsgOffsetIndex newOffsetIndex( newIndexSize );                            
                            for ( CORE::UInt32 o=startIndexOffset; o<endIndexOffset; ++o )
                            {
                                newOffsetIndex[ o2 ] = originalOffsetIndex[ o ];
                                ++o2;
                            }

                            // Now we overwrite the footer in the in-memory container to only have the subset of messages we care about referenced
                            CORE::UInt32 bytesWritten = 0;
                            if ( COMCORE::CPubSubMsgContainerBinarySerializer::SerializeFooter( newOffsetIndex, m_msgReceiveBuffer->GetDataSize()-1, *m_msgReceiveBuffer, bytesWritten ) )
                            {
                                // We are done with this container
                                ++containersProcessed;
                                m_msgReceiveBuffer = GUCEF_NULL;
                            }
                        }
                    }
                }
                else
                {
                    // No write buffer available, we need to wait before processing more requests
                    return false;
                }
            }
            else
            {
                if ( GUCEF_NULL == m_msgReceiveBuffer )
                    m_msgReceiveBuffer = GetSerializedMsgBuffers().GetNextWriterBuffer( containerFileFirstMsgDt, true, GUCEF_MT_INFINITE_LOCK_TIMEOUT );
                
                if ( GUCEF_NULL != m_msgReceiveBuffer )
                {
                    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:ProcessNextStorageToPubSubRequest: Loading the entire container as-is to serve (part of) the request" );

                    if ( LoadStorageFile( (*n), *m_msgReceiveBuffer ) )
                    {
                        // Since we loaded the entire container and we dont need a subset we are done
                        ++containersProcessed;
                        m_msgReceiveBuffer = GUCEF_NULL;
                    }
                }
                else
                {
                    // No write buffer available, we need to wait before processing more requests
                    return false;
                }
            }
            ++n;
        }

        if ( containersProcessed != queuedRequest.vfsPubSubMsgContainersToPush.size() )
        {
            OnUnableToFullFillStorageToPubSubRequest( queuedRequest );
        }
        
        m_storageToPubSubRequests.pop_front();
        GetSerializedMsgBuffers().SignalEndOfWriting();
    }

    return true;
}

/*-------------------------------------------------------------------------*/

void
CStoragePubSubClientTopic::OnStoredPubSubMsgTransmissionFailure( const CORE::CDateTime& firstMsgDt )
{GUCEF_TRACE;

    
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::TransmitNextPubSubMsgBuffer( void )
{GUCEF_TRACE;    

    CORE::CDateTime firstMsgDt;
    m_msgReceiveBuffer = m_buffers.GetNextReaderBuffer( firstMsgDt, true, 25 );
    if ( GUCEF_NULL == m_msgReceiveBuffer )
        return true; // nothing to do
        
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic(" + CORE::PointerToString( this ) +
        "):TransmitNextPubSubMsgBuffer: New buffer is available of " + CORE::ToString( m_msgReceiveBuffer->GetDataSize() ) + " bytes" );         

    CORE::UInt32 bytesRead = 0;
    COMCORE::CPubSubMsgContainerBinarySerializer::TMsgOffsetIndex originalOffsetIndex;
    if ( !COMCORE::CPubSubMsgContainerBinarySerializer::DeserializeFooter( originalOffsetIndex, *m_msgReceiveBuffer, bytesRead ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic(" + CORE::PointerToString( this ) +
            "):TransmitNextPubSubMsgBuffer: Failed to read container footer" );
        OnStoredPubSubMsgTransmissionFailure( firstMsgDt );
        return false;
    }

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic(" + CORE::PointerToString( this ) +
        "):TransmitNextPubSubMsgBuffer: Per footer the buffer contains " + CORE::ToString( originalOffsetIndex.size() ) + " messages to publish" );

    // We now link logical message objects to the data in the buffer
    CORE::UInt32 startIndexOffset = 0;
    CORE::UInt32 endIndexOffset = 0;
    bool isCorrupted = false;                        
    COMCORE::CPubSubMsgContainerBinarySerializer::TBasicPubSubMsgVector msgs;                        
    if ( !COMCORE::CPubSubMsgContainerBinarySerializer::Deserialize( msgs, true, originalOffsetIndex, *m_msgReceiveBuffer, isCorrupted ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic(" + CORE::PointerToString( this ) +
            "):TransmitNextPubSubMsgBuffer: Failed to deserialize messages from container. According to the footer the container had " + 
            CORE::ToString( originalOffsetIndex.size() ) + " entries. isCorrupted=" + CORE::BoolToString( isCorrupted ) );
        OnStoredPubSubMsgTransmissionFailure( firstMsgDt );
        return false;
    }

    // We now have the messages in a format that allows interpretation by the pub-sub backend
    // We can now proceed with publishing all the messages to the relevant topics
    CORE::UInt32 topicsToPublishOn = 0;
    CORE::UInt32 topicsPublishedOn = 0;
    bool publishSuccess = true;
    //TopicVector::iterator i = m_topics.begin();
    //while ( i != m_topics.end() )
    //{
    //    TopicLink& topicLink = (*i);
    //    COMCORE::CPubSubClientTopic* topic = topicLink.topic;

    //    if ( GUCEF_NULL != topic )
    //    {
    //        if ( topic->IsPublishingSupported() )
    //        {
    //            ++topicsToPublishOn;
    //            if ( topic->Publish( topicLink.currentPublishActionIds, msgs, true ) )
    //            {
    //                ++topicsPublishedOn;
    //            }
    //            else
    //            {
    //                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic(" + CORE::PointerToString( this ) +
    //                    "):TransmitNextPubSubMsgBuffer: Failed to publish messages to topic" );
    //                OnStoredPubSubMsgTransmissionFailure( firstMsgDt );
    //                publishSuccess = false;
    //            }
    //        }
    //    }
    //    ++i;
    //}

    //GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic(" + CORE::PointerToString( this ) +
    //    "):TransmitNextPubSubMsgBuffer: Successfully published messages to " + CORE::ToString( topicsPublishedOn ) + " topics, " + 
    //    CORE::ToString( topicsToPublishOn ) + " topics available for publishing" );

    return publishSuccess;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace STORAGE */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
