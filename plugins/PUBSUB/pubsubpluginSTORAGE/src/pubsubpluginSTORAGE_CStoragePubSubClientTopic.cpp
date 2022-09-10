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

#ifndef GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#include "CDynamicBufferAccess.h"
#define GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#endif /* GUCEF_CORE_CDYNAMICBUFFERACCESS_H ? */

#ifndef GUCEF_PUBSUB_CBASICPUBSUBMSG_H
#include "gucefPUBSUB_CBasicPubSubMsg.h"
#define GUCEF_PUBSUB_CBASICPUBSUBMSG_H
#endif /* GUCEF_PUBSUB_CBASICPUBSUBMSG_H ? */

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
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

#define GUCEF_DEFAULT_DEFAULT_NR_OF_SWAP_BUFFERS                    2
#define GUCEF_DEFAULT_VFS_CONTAINER_FILE_EXTENSION                  ".pubsubmsgs"

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CStoragePubSubClientTopic::StorageToPubSubRequest::StorageToPubSubRequest( void )
    : startDt()
    , endDt()
    , vfsPubSubMsgContainersToPush()
    , okIfZeroContainersAreFound( true )
    , isPersistentRequest( false )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CStoragePubSubClientTopic::StorageToPubSubRequest::StorageToPubSubRequest( const CORE::CDateTime& startDt         , 
                                                                           const CORE::CDateTime& endDt           , 
                                                                           bool okIfZeroContainersAreFoundDefault ,
                                                                           bool isPersistentRequestDefault        )
    : startDt( startDt )
    , endDt( endDt )
    , vfsPubSubMsgContainersToPush()
    , okIfZeroContainersAreFound( okIfZeroContainersAreFoundDefault ) 
    , isPersistentRequest( isPersistentRequestDefault )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CStoragePubSubClientTopic::StorageToPubSubRequest::StorageToPubSubRequest( const StorageToPubSubRequest& src )
    : startDt( src.startDt )
    , endDt( src.endDt )
    , vfsPubSubMsgContainersToPush( src.vfsPubSubMsgContainersToPush )
    , okIfZeroContainersAreFound( src.okIfZeroContainersAreFound )
    , isPersistentRequest( src.isPersistentRequest )
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

CORE::CString
CStoragePubSubClientTopic::GenerateDefaultVfsStorageContainerFileExt( void )
{GUCEF_TRACE;

    return ".v" + CORE::ToString( PUBSUB::CPubSubMsgContainerBinarySerializer::CurrentFormatVersion ) + GUCEF_DEFAULT_VFS_CONTAINER_FILE_EXTENSION;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::StorageToPubSubRequest::SaveConfig( CORE::CDataNode & tree ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::StorageToPubSubRequest::LoadConfig( const CORE::CDataNode & treeroot )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

CStoragePubSubClientTopic::StorageBufferMetaData::StorageBufferMetaData( void )
    : msgOffsetIndex()
    , actionIds()
    , relatedStorageFile()
    , linkedRequest( GUCEF_NULL )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CStoragePubSubClientTopic::CStoragePubSubClientTopic( CStoragePubSubClient* client )
    : PUBSUB::CPubSubClientTopic()
    , m_client( client )
    , m_pubsubMsgs()
    , m_pubsubMsgsRefs()
    , m_config()
    , m_syncVfsOpsTimer( GUCEF_NULL )
    , m_reconnectTimer( GUCEF_NULL )
    , m_bufferContentTimeWindowCheckTimer( GUCEF_NULL )
    , m_lock()
    , m_currentPublishActionId( 1 )
    , m_currentReceiveActionId( 1 )
    , m_publishSuccessActionIds()
    , m_publishSuccessActionEventData()
    , m_publishFailureActionIds()
    , m_publishFailureActionEventData()
    , m_metrics()
    , m_metricFriendlyTopicName()
    , m_isHealthy( true )
    , m_subscriptionIsAtEndOfData( false )
    , m_currentReadBuffer( GUCEF_NULL )
    , m_currentWriteBuffer( GUCEF_NULL )
    , m_currentBookmarkInfo()
    , m_currentBookmark()
    , m_pubsubBookmarkPersistence()
    , m_vfsFilePostfix( GenerateDefaultVfsStorageContainerFileExt() )
    , m_vfsRootPath()
    , m_lastPersistedMsgId()
    , m_lastPersistedMsgDt()
    , m_encodeSizeRatio( -1 )
    , m_stage0StorageToPubSubRequests()
    , m_stage1StorageToPubSubRequests()
    , m_stage2StorageToPubSubRequests()
    , m_buffers( GUCEF_DEFAULT_DEFAULT_NR_OF_SWAP_BUFFERS )
    , m_lastWriteBlockCompletion()
    , m_storageBufferMetaData()
    , m_vfsOpsThread()
    , m_msgsLoadedFromStorage( 0 )
    , m_msgBytesLoadedFromStorage( 0 )
    , m_msgsWrittenToStorage( 0 )
    , m_msgBytesWrittenToStorage( 0 )
    , m_storageCorruptionDetections( 0 )
    , m_storageDeserializationFailures( 0 )    
{GUCEF_TRACE;

    m_publishSuccessActionEventData.LinkTo( &m_publishSuccessActionIds );
    m_publishFailureActionEventData.LinkTo( &m_publishFailureActionIds );

    m_syncVfsOpsTimer = new CORE::CTimer( m_client->GetConfig().pulseGenerator, 25 );

    if ( m_client->GetConfig().desiredFeatures.supportsAutoReconnect )
    {
        m_reconnectTimer = new CORE::CTimer( m_client->GetConfig().pulseGenerator, m_client->GetConfig().reconnectDelayInMs );
    }

    if ( m_client->GetConfig().desiredFeatures.supportsPublishing )
    {
        m_bufferContentTimeWindowCheckTimer = new CORE::CTimer( m_client->GetConfig().pulseGenerator, 1000 );
    }

    m_pubsubBookmarkPersistence = m_client->GetBookmarkPersistence();
    
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

    delete m_bufferContentTimeWindowCheckTimer;
    m_bufferContentTimeWindowCheckTimer = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

PUBSUB::CPubSubClient*
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

    if ( GUCEF_NULL != m_bufferContentTimeWindowCheckTimer )
    {
        TEventCallback callback( this, &CStoragePubSubClientTopic::OnBufferContentTimeWindowCheckCycle );
        SubscribeTo( m_bufferContentTimeWindowCheckTimer ,
                     CORE::CTimer::TimerUpdateEvent      ,
                     callback                            );
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
                                    const PUBSUB::CIPubSubMsg& msg  ,
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
                                    const PUBSUB::CBasicPubSubMsg::TBasicPubSubMsgVector& msgs ,
                                    bool notify                                                 )
{GUCEF_TRACE;

    CORE::UInt64 msgCount = msgs.size();
    TIPubSubMsgConstRawPtrVector msgsPtrs( msgCount );

    for ( CORE::UInt64 i=0; i<msgCount; ++i )
    {
        msgsPtrs[ i ] = &msgs[ i ];
    }

    return PublishViaMsgPtrs< const PUBSUB::CIPubSubMsg* >( publishActionIds, msgsPtrs, notify );
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::Publish( TPublishActionIdVector& publishActionIds ,
                                    const TIPubSubMsgConstRawPtrVector& msgs ,
                                    bool notify                              )
{GUCEF_TRACE;

    return PublishViaMsgPtrs< const PUBSUB::CIPubSubMsg* >( publishActionIds, msgs, notify );
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::Publish( TPublishActionIdVector& publishActionIds ,
                                    const TIPubSubMsgRawPtrVector& msgs      ,
                                    bool notify                              )
{GUCEF_TRACE;

    return PublishViaMsgPtrs< PUBSUB::CIPubSubMsg* >( publishActionIds, msgs, notify );
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::Publish( TPublishActionIdVector& publishActionIds ,
                                    const TIPubSubMsgSPtrVector& msgs        ,
                                    bool notify                              )
{GUCEF_TRACE;

    return PublishViaMsgPtrs< PUBSUB::CIPubSubMsg::TNoLockSharedPtr >( publishActionIds, msgs, notify );
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

    if ( !m_config.needPublishSupport )
    {
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic(" + CORE::PointerToString( this ) +
            "):PublishViaMsgPtrs: Attempt to publish messages on a topic that is not configured for publishing" );
        return false;
    }

    try
    {
        MT::CScopeMutex lock( m_lock );

        TPublishActionIdVector::iterator publishActionIdIttr = publishActionIds.begin();
        typename std::vector< T >::const_iterator i = msgs.begin();
        bool work = true;
        while ( work )
        {        
            bool firstBlock = m_lastWriteBlockCompletion == CORE::CDateTime::Empty;
            if ( firstBlock )
            {
                m_lastWriteBlockCompletion = CORE::CDateTime::NowUTCDateTime();
            }

            StorageBufferMetaData* bufferMetaData = GUCEF_NULL;
            if ( GUCEF_NULL == m_currentWriteBuffer )
            {
                m_currentWriteBuffer = m_buffers.GetNextWriterBuffer( CORE::CDateTime::NowUTCDateTime(), m_config.performVfsOpsASync, GUCEF_MT_INFINITE_LOCK_TIMEOUT );
                if ( GUCEF_NULL == m_currentWriteBuffer )
                {
                    if ( m_config.performVfsOpsASync )
                    {
                        // We should not get here with both the blocking & long timeout
                        // the async buffer reads should be allowing us to proceed in a reasonable timeframe
                        GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "StoragePubSubClientTopic(" + CORE::PointerToString( this ) +
                            "):PublishViaMsgPtrs: Failed to obtain new message serialization buffer" );
                    }
                    else
                    {
                        // Try to persist next ready to read buffer as part of the call chain, consequence of not doing async
                        if ( StoreNextReceivedPubSubBuffer() )
                        {
                            // Try again to get a write buffer, this should typically work based on the prior operation
                            m_currentWriteBuffer = m_buffers.GetNextWriterBuffer( CORE::CDateTime::NowUTCDateTime(), m_config.performVfsOpsASync, GUCEF_MT_INFINITE_LOCK_TIMEOUT );                            
                        }
                        if ( GUCEF_NULL == m_currentWriteBuffer )
                        {                        
                            lock.EarlyUnlock();
                            NotifyObservers( LocalPublishQueueFullEvent );
                            return false;
                        }
                    }
                }
                m_currentWriteBuffer->SetDataSize( 0 );

                CORE::UInt32 newBytesWritten = 0;
                if ( !PUBSUB::CPubSubMsgContainerBinarySerializer::SerializeHeader( m_config.pubsubBinarySerializerOptions, 0, *m_currentWriteBuffer, newBytesWritten ) )
                {
                    // We carry on best effort but this is really bad
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "StoragePubSubClientTopic(" + CORE::PointerToString( this ) +
                        "):PublishViaMsgPtrs: Failed to write container header at start of new pub-sub msg container" );
                }

                // We are (re)using a new buffer, as such make sure we reset the meta-data that may be pre-existing
                bufferMetaData = &( m_storageBufferMetaData[ m_currentWriteBuffer ] );
                bufferMetaData->msgOffsetIndex.clear();
                bufferMetaData->actionIds.clear();

                // Start the timer on content for this buffer
                m_bufferContentTimeWindowCheckTimer->SetEnabled( true );
            }
            else
            {
                // fetch the meta-data for this buffer
                bufferMetaData = &( m_storageBufferMetaData[ m_currentWriteBuffer ] );
            }
            
            CORE::UInt32 bufferOffset = m_currentWriteBuffer->GetDataSize();
        
            while ( i != msgs.end() )
            {
                // see if we have a pre-existing action id (retry)
                CORE::UInt64 publishActionId = 0;
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
                {
                    publishActionIds.push_back( publishActionId );
                    publishActionIdIttr = publishActionIds.end();
                }
                else
                    (*publishActionIdIttr) = publishActionId;

                // Serialize the message
                CORE::UInt32 ticks = CORE::GUCEFGetTickCount();
                CORE::UInt32 msgBytesWritten = 0;
                if ( PUBSUB::CPubSubMsgBinarySerializer::Serialize( m_config.pubsubBinarySerializerOptions, *(*i), bufferOffset, *m_currentWriteBuffer, msgBytesWritten ) && 0 != msgBytesWritten )
                {
                    #ifdef GUCEF_DEBUG_MODE
                    // Extra sanity check
                    if ( !bufferMetaData->msgOffsetIndex.empty() )
                    {
                        if ( bufferOffset < bufferMetaData->msgOffsetIndex[ bufferMetaData->msgOffsetIndex.size()-1 ] )
                        {
                            GUCEF_ERROR_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "StoragePubSubClientTopic(" + CORE::PointerToString( this ) +
                                "):PublishViaMsgPtrs: Non-Ascending msg offset detected. publishActionId=" + CORE::ToString( publishActionId ) );
                        }
                    }
                    #endif

                    // add to our meta-data tracking
                    bufferMetaData->msgOffsetIndex.push_back( bufferOffset );
                    bufferMetaData->actionIds.push_back( publishActionId );

                    ticks = CORE::GUCEFGetTickCount() - ticks;
                    bufferOffset += msgBytesWritten;

                    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "StoragePubSubClientTopic(" + CORE::PointerToString( this ) +
                        "):PublishViaMsgPtrs: Serialized a message with serialized size " + CORE::ToString( msgBytesWritten ) +
                        ". This took " + CORE::ToString( ticks ) + "ms. publishActionId=" + CORE::ToString( publishActionId ) );

                    // Check to see if we have gathered enough data or enough time has passed to consider the current container complete
                    if ( m_currentWriteBuffer->GetDataSize() >= m_config.desiredMinimalSerializedBlockSize ||                                                                  // <- container byte size limit criterea
                        ( !firstBlock && m_lastWriteBlockCompletion.GetTimeDifferenceInMillisecondsToNow() >= m_config.desiredMaxTimeToWaitToGrowSerializedBlockSizeInMs ) ||  // <- container max timespan limit criterea 
                        ( m_config.maxTotalMsgsInFlight > 0 && bufferMetaData->actionIds.size() >= (size_t) m_config.maxTotalMsgsInFlight ) )                                  // <- respect max in-flight limit and dont block
                    {
                        // The current container is now considered to have enough content.
                        // Let's wrap things up...
                        FinalizeWriteBuffer( bufferMetaData, bufferOffset );
                        ++i; break;
                    }
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
            
            if ( i == msgs.end() )
                work = false;
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

void
CStoragePubSubClientTopic::FinalizeWriteBuffer( StorageBufferMetaData* bufferMetaData, CORE::UInt32 bufferOffset )
{GUCEF_TRACE;

    CORE::UInt32 newBytesWritten = 0;
    if ( !PUBSUB::CPubSubMsgContainerBinarySerializer::SerializeFooter( bufferMetaData->msgOffsetIndex, bufferOffset, *m_currentWriteBuffer, newBytesWritten ) )
    {
        // We carry on best effort but this is really bad
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "StoragePubSubClientTopic(" + CORE::PointerToString( this ) +
            "):FinalizeWriteBuffer: Failed to write container footer at end of current pub-sub msg container" );
    }

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic(" + CORE::PointerToString( this ) +
        "):FinalizeWriteBuffer: Completed a serialized msg data block of size " + CORE::ToString( bufferOffset ) );

    // prevent further write action on this buffer and cause a new write buffer to be requested
    m_currentWriteBuffer = GUCEF_NULL;

    // Now that the async threaded work is done signal that we are finished writing
    // this releases the write buffer for reading as a read buffer
    m_buffers.SignalEndOfWriting();

    m_lastWriteBlockCompletion = CORE::CDateTime::NowUTCDateTime();
    m_bufferContentTimeWindowCheckTimer->SetEnabled( false );

    if ( !m_config.performVfsOpsASync )
    {
        // Try to persist next ready to read buffer as part of the call chain, consequence of not doing async
        StoreNextReceivedPubSubBuffer();
    }
}

/*-------------------------------------------------------------------------*/

void
CStoragePubSubClientTopic::FinalizeWriteBuffer( void )
{GUCEF_TRACE;

    CORE::UInt32 bufferOffset = m_currentWriteBuffer->GetDataSize();
    StorageBufferMetaData* bufferMetaData = &( m_storageBufferMetaData[ m_currentWriteBuffer ] );
    FinalizeWriteBuffer( bufferMetaData, bufferOffset );
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::SetupToSubscribe( PUBSUB::CPubSubClientTopicConfig& config )
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
CStoragePubSubClientTopic::SaveConfig( PUBSUB::CPubSubClientTopicConfig& config ) const
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

CORE::CString
CStoragePubSubClientTopic::ResolveVfsRootPath( void ) const
{GUCEF_TRACE;

    CORE::CString vfsRootPath = m_config.vfsStorageRootPath.ReplaceSubstr( "{pubsubIdPrefix}", m_client->GetConfig().pubsubIdPrefix );    
    vfsRootPath = vfsRootPath.ReplaceSubstr( "{topicName}", m_config.topicName );
    vfsRootPath = vfsRootPath.ReplaceSubstr( "{metricsFriendlyTopicName}", m_metricFriendlyTopicName );
    return vfsRootPath;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::LoadConfig( const PUBSUB::CPubSubClientTopicConfig& config )
{GUCEF_TRACE;

    // Sanity check the config
    if ( config.needPublishSupport && config.needSubscribeSupport )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic(" + CORE::PointerToString( this ) + "):LoadConfig: This backend does not support publishing and subscribing on the same topic. Both are currently selected" );
        return false;
    }
    if ( !config.needPublishSupport && !config.needSubscribeSupport )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic(" + CORE::PointerToString( this ) + "):LoadConfig: Neither publishing nor subscribing is selected for the topic, this is invalid" );
        return false;
    }
    
    MT::CScopeMutex lock( m_lock );

    m_config = config;
    m_metricFriendlyTopicName = GenerateMetricsFriendlyTopicName( m_config.topicName );    
    m_vfsRootPath = ResolveVfsRootPath();    

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::Disconnect( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    return StopVfsOps();
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::BeginVfsOps( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    if ( !m_config.performVfsOpsASync )
    {
        // If we do not perform VFS Ops Async then we use less system resources but we do run the risk
        // of filling up our write queues etc and causing performance issues in the pipeline
        // This is a performance tradeoff specific to your use-case
        if ( GUCEF_NULL != m_syncVfsOpsTimer )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic(" + CORE::PointerToString( this ) + "):BeginVfsOps: Will use timer for sync VFS ops" );
            return m_syncVfsOpsTimer->SetEnabled( true );
        }
        return false;
    }
    else
    {
        // Do we want a dedicated thread per topic or will a more general threadpool with intermixed work suffice
        // This is a performance tradeoff specific to your use-case
        //      Do you have extra threading capacity available on the node?
        //      Is it important we write to VFS as fast as possible?
        //      Etc.
        if ( m_config.performVfsOpsInDedicatedThread )
        {
            if ( m_vfsOpsThread.IsNULL() )
            {
                m_vfsOpsThread = CStoragePubSubClientTopicVfsTaskPtr( new CStoragePubSubClientTopicVfsTask( this ) );
            }
            if ( !m_vfsOpsThread.IsNULL() )
            {
                CORE::ThreadPoolPtr threadPool = m_client->GetThreadPool();
                if ( threadPool.IsNULL() )
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "StoragePubSubClientTopic(" + CORE::PointerToString( this ) + "):BeginVfsOps: Failed to obtain dedicated thread pool for dedicated VFS thread for async operations. Falling back to global thread pool" );
                    threadPool = CORE::CCoreGlobal::Instance()->GetTaskManager().GetThreadPool();
                    if ( threadPool.IsNULL() )
                    {
                        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "StoragePubSubClientTopic(" + CORE::PointerToString( this ) + "):BeginVfsOps: Failed to obtain global thread pool for dedicated VFS thread for async operations" );
                        return false;
                    }
                }
                if ( threadPool->StartTask( m_vfsOpsThread ) )
                {
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic(" + CORE::PointerToString( this ) + "):BeginVfsOps: Started dedicated per-topic thread for async VFS ops" );
                }
                else
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "StoragePubSubClientTopic(" + CORE::PointerToString( this ) + "):BeginVfsOps: Failed to start dedicated VFS thread for async operations" );
                    return false;
                }
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "StoragePubSubClientTopic(" + CORE::PointerToString( this ) + "):BeginVfsOps: Failed to create dedicated VFS task (thread) for async operations" );
                return false;
            }
        }
        else
        {
            // Seems we want async but not go as far as spinning up a dedicated thread per topic to handle VFS operations
            // we will rely on defining 'work' tasks as work arrives
        }
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::StopVfsOps( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    bool success = true;

    if ( GUCEF_NULL != m_syncVfsOpsTimer )
    {
        success = m_syncVfsOpsTimer->SetEnabled( false ) && success;
        m_buffers.SignalEndOfWriting();
    }

    if ( !m_vfsOpsThread.IsNULL() )
        success = m_vfsOpsThread->RequestTaskToStop( false ) && success;

    return success;
}

/*-------------------------------------------------------------------------*/

void
CStoragePubSubClientTopic::OnVfsFileCreated( CORE::CNotifier* notifier    ,
                                             const CORE::CEvent& eventid  ,
                                             CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    CORE::CDirectoryWatcherEvents::TFileCreatedEventData* fileCreatedEventData = static_cast< CORE::CDirectoryWatcherEvents::TFileCreatedEventData* >( eventdata );
    if ( GUCEF_NULL != fileCreatedEventData )
    {        
        CORE::CDateTime containerFileFirstMsgDt;
        CORE::CDateTime containerFileLastMsgDt;
        CORE::CDateTime containerFileCaptureDt;
        if ( GetTimestampsFromContainerFilename( *fileCreatedEventData, containerFileFirstMsgDt, containerFileLastMsgDt, containerFileCaptureDt ) )
        {
            // Now that we have the relevant timestamps we check these against the ranges of the persistent requests
            // this is on a first come first serve basis if we find a match

            StorageToPubSubRequestDeque::iterator i = m_stage0StorageToPubSubRequests.begin();
            while ( i != m_stage0StorageToPubSubRequests.end() )
            {
                // Check to see if the file is in range

                bool firstMsgIsInRange = containerFileFirstMsgDt.IsWithinRange( (*i).startDt, (*i).endDt );
                bool lastMsgIsInRange = containerFileLastMsgDt.IsWithinRange( (*i).startDt, (*i).endDt );
                bool isPartialMatch = ( firstMsgIsInRange && !lastMsgIsInRange ) || ( !firstMsgIsInRange && lastMsgIsInRange );

                if ( firstMsgIsInRange || lastMsgIsInRange )
                {
                    StorageToPubSubRequest newAutoRequest( (*i) );
                    newAutoRequest.isPersistentRequest = false;
                    newAutoRequest.vfsPubSubMsgContainersToPush.insert( *fileCreatedEventData );

                    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:OnVfsFileCreated: auto-added file " + *fileCreatedEventData + 
                        " as request based on persistent storage request" );
                    
                    m_stage2StorageToPubSubRequests.push_back( newAutoRequest );
                }

                if ( !isPartialMatch )
                    break;

                ++i;
            }
        }
        else
        {
            // could be some other file that is not a container file
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CStoragePubSubClientTopic::OnVfsArchiveMounted( CORE::CNotifier* notifier    ,
                                                const CORE::CEvent& eventid  ,
                                                CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    // new archives can be dynamically mounted
    // as such we need to be on the lookout for such events and if we have any 
    // persistent storage requests set up a new dir watch for new files IF
    // the archive's mount location matches our vfs root

    MT::CScopeMutex lock( m_lock );

    VFS::CVFS::TArchiveMountedEventData* archiveMountedEventData = static_cast< VFS::CVFS::TArchiveMountedEventData* >( eventdata );
    if ( GUCEF_NULL != archiveMountedEventData )
    {
        if ( !m_stage0StorageToPubSubRequests.empty() )
        {
            if ( m_vfsRootPath.HasSubstr( *archiveMountedEventData, true ) )
            {
                // redo our subscription since the relevant archive landscape changed
                SubscribeToVfsTopicPathEvents();
            }
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CStoragePubSubClientTopic::OnNotify( CORE::CNotifier* notifier    ,
                                     const CORE::CEvent& eventid  ,
                                     CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    VFS::CVFS& vfs = VFS::CVfsGlobal::Instance()->GetVfs();
    if ( &vfs == notifier )
    {
        if ( eventid == CORE::CDirectoryWatcherEvents::FileCreatedEvent )
        {
            OnVfsFileCreated( notifier, eventid, eventdata );
        }
        else
        if ( eventid == VFS::CVFS::ArchiveMountedEvent )
        {
            OnVfsArchiveMounted( notifier, eventid, eventdata );
        }
    }
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::SubscribeToVfsTopicPathEvents( void )
{GUCEF_TRACE;

    VFS::CVFS& vfs = VFS::CVfsGlobal::Instance()->GetVfs();

    CORE::CIDirectoryWatcher::CDirWatchOptions dirWatchOptions( false );
    dirWatchOptions.watchForFileCreation = true;

    SubscribeTo( &vfs );

    return vfs.AddDirToWatch( m_vfsRootPath, dirWatchOptions );
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::AddStorageToPubSubRequest( const StorageToPubSubRequest& request )
{GUCEF_TRACE;

    if ( m_config.needPublishSupport || !m_config.needSubscribeSupport )
    {
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic(" + CORE::PointerToString( this ) +
            "):PublishViaMsgPtrs: Attempt to source messages from storage on a topic that is not configured for subscriptions" );
        return false;
    }

    MT::CScopeMutex lock( m_lock );
    
    m_stage1StorageToPubSubRequests.push_back( request );

    if ( request.isPersistentRequest )
    {
        bool hadPersistentRequests = m_stage0StorageToPubSubRequests.empty();

        // persistent requests get 'refills' from new files being dropped in monitored
        // request root paths
        m_stage0StorageToPubSubRequests.push_back( request );
        
        if ( !hadPersistentRequests )
        {
            SubscribeToVfsTopicPathEvents();
        }
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::Subscribe( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    if ( m_config.needSubscribeSupport && m_config.autoPushAfterStartupIfStorageToPubSub )
    {
        AddStorageToPubSubRequest( StorageToPubSubRequest( m_config.oldestStoragePubSubMsgFileToLoad, m_config.youngestStoragePubSubMsgFileToLoad, true, m_config.autoPushAfterStartupIsPersistent ) );
    }

    bool success = BeginVfsOps();
    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:Subscribe: 'subscribe to topic' for Topic Name: " + m_config.topicName );
    return success;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::SubscribeStartingAtMsgId( const CORE::CVariant& msgIdBookmark )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    if ( m_client->GetConfig().desiredFeatures.supportsMsgIdBasedBookmark )
    {
        // Scan storage from earliest container onward for the given message id
        // this is a very expensive operation

        // @TODO Currently not supported.
    }

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::SubscribeStartingAtMsgDateTime( const CORE::CDateTime& msgDtBookmark )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    if ( m_config.needSubscribeSupport && ( m_config.autoPushAfterStartupIfStorageToPubSub ) )
    {
        if ( !AddStorageToPubSubRequest( StorageToPubSubRequest( msgDtBookmark, m_config.youngestStoragePubSubMsgFileToLoad, true, false ) ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:SubscribeStartingAtMsgDateTime: Failed to add publish request. DT=" + msgDtBookmark.ToIso8601DateTimeString( true, true ) + ", Topic Name: " + m_config.topicName );
            return false;
        }
    }

    bool success = BeginVfsOps();
    if ( success )
    {
        GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:SubscribeStartingAtMsgDateTime: DT=" + msgDtBookmark.ToIso8601DateTimeString( true, true ) + ", Topic Name: " + m_config.topicName );
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:SubscribeStartingAtMsgDateTime: Failed to start VFS Ops. DT=" + msgDtBookmark.ToIso8601DateTimeString( true, true ) + ", Topic Name: " + m_config.topicName );
    }
    return success;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::SubscribeStartingAtBookmarkInfo( const TStorageBookmarkInfo& bookmarkInfo )
{GUCEF_TRACE;

    if ( 0 != bookmarkInfo.doneWithFile )
    {
        // Since we were done with the file referenced we want the next available file

        CORE::CDateTime firstMsgDt;
        CORE::CDateTime lastMsgDt;
        CORE::CDateTime containerDt;
        if ( GetTimestampsFromContainerFilename( bookmarkInfo.vfsFilePath, firstMsgDt, lastMsgDt, containerDt ) )
        {
            // We now know the datetime of the last message
            // We use this as the starting point
            return SubscribeStartingAtMsgDateTime( lastMsgDt );
        }
    }
    else
    {
        // we were not done yet with the referenced file
        // we need to reload it and resume operations
                          // @TODO
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::SubscribeStartingAtBookmark( const PUBSUB::CPubSubBookmark& bookmark )
{GUCEF_TRACE;

    switch ( bookmark.GetBookmarkType() )
    {
        case PUBSUB::CPubSubBookmark::BOOKMARK_TYPE_MSG_DATETIME:
        {
            CORE::CDateTime msgDtBookmark = bookmark.GetBookmarkData().AsDateTime();
            if ( CORE::CDateTime::Empty != msgDtBookmark )
                return SubscribeStartingAtMsgDateTime( msgDtBookmark );
            return false;
        }
        case PUBSUB::CPubSubBookmark::BOOKMARK_TYPE_TOPIC_INDEX:
        {
            TStorageBookmarkInfo bookmarkInfo;
            if ( SyncBookmarkToBookmarkInfo( bookmark, bookmarkInfo ) )
                return SubscribeStartingAtBookmarkInfo( bookmarkInfo );
            return false;
        }

        default:
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:SubscribeStartingAtBookmark: Unsupported bookmark type given. Type=" + bookmark.GetBookmarkTypeName() );
            return false;
        }
    }
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::SyncBookmarkInfoToBookmark( const TStorageBookmarkInfo& info  ,
                                                       PUBSUB::CPubSubBookmark& bookmark )
{GUCEF_TRACE;

    CORE::CVariant& data = bookmark.GetBookmarkData();
    if ( data.IsBlob() )
    {
        CORE::CDynamicBuffer buffer;
        buffer.LinkTo( data );
        CORE::CDynamicBufferAccess bufferAccess( buffer );

        bool totalSuccess = true;
        totalSuccess = bufferAccess.WriteValue( info.bookmarkFormatVersion ) && totalSuccess;
        totalSuccess = bufferAccess.WriteValue( info.doneWithFile ) && totalSuccess;
        totalSuccess = bufferAccess.WriteValue( info.msgIndex ) && totalSuccess;
        totalSuccess = bufferAccess.WriteValue( info.offsetInFile ) && totalSuccess;
        totalSuccess = bufferAccess.WriteByteSizePrefixedString( info.vfsFilePath ) && totalSuccess;
        return totalSuccess;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::SyncBookmarkToBookmarkInfo( const PUBSUB::CPubSubBookmark& bookmark ,
                                                       TStorageBookmarkInfo& info              )
{GUCEF_TRACE;

    const CORE::CVariant& data = bookmark.GetBookmarkData();
    if ( data.IsBlob() )
    {
        CORE::CDynamicBuffer buffer;
        buffer.LinkTo( data );
        CORE::CDynamicBufferAccess bufferAccess( buffer );

        bool totalSuccess = true;
        totalSuccess = bufferAccess.ReadValue( info.bookmarkFormatVersion ) && totalSuccess;
        totalSuccess = bufferAccess.ReadValue( info.doneWithFile ) && totalSuccess;
        totalSuccess = bufferAccess.ReadValue( info.msgIndex ) && totalSuccess;
        totalSuccess = bufferAccess.ReadValue( info.offsetInFile ) && totalSuccess;
        totalSuccess = bufferAccess.ReadByteSizePrefixedString( info.vfsFilePath ) && totalSuccess;
        return totalSuccess;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

PUBSUB::CPubSubBookmark
CStoragePubSubClientTopic::GetCurrentBookmark( void )
{GUCEF_TRACE;

    // provides a bookmark for the latest read batch of data
    
    MT::CScopeMutex lock( m_lock );

    if ( SyncBookmarkInfoToBookmark( m_currentBookmarkInfo, m_currentBookmark ) )
        return m_currentBookmark;

    return PUBSUB::CPubSubBookmark( PUBSUB::CPubSubBookmark::BOOKMARK_TYPE_NOT_AVAILABLE );
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::GetStorageBufferMetaDataPtrForReceiveActionId( CORE::UInt64 receiveActionId           , 
                                                                          const StorageBufferMetaData** metaData ) const
{GUCEF_TRACE;

    TStorageBufferMetaDataMap::const_iterator i = m_storageBufferMetaData.begin();
    while ( i != m_storageBufferMetaData.end() )
    {
        const StorageBufferMetaData& metaDataEntry = (*i).second;
        
        // Logically since we control 'receiveActionId' right here in the backend
        // we know that we always just increment in a batch for all messages in a container
        // as such we can just check the first and last as a range to find the right container
        
        if ( !metaDataEntry.actionIds.empty() )
        {
            CORE::UInt64 containerReceiveActionIdMin = *metaDataEntry.actionIds.begin();
            CORE::UInt64 containerReceiveActionIdMax = *metaDataEntry.actionIds.rbegin();

            if ( receiveActionId >= containerReceiveActionIdMin &&
                 receiveActionId <= containerReceiveActionIdMax  )
            {
                // We found the container this 'receiveActionId' is linked with

                *metaData = &metaDataEntry;
                return true;
            }        
        }     
        ++i;
    }

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::GetBookmarkForReceiveActionId( CORE::UInt64 receiveActionId      , 
                                                          PUBSUB::CPubSubBookmark& bookmark ) const
{GUCEF_TRACE;

    TStorageBookmarkInfo bmInfo;
    const StorageBufferMetaData* metaData = GUCEF_NULL;
    if ( GetBookmarkInfoForReceiveActionId( receiveActionId, &metaData, bmInfo ) )
    {
        return SyncBookmarkInfoToBookmark( bmInfo, bookmark );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::GetBookmarkInfoForReceiveActionId( CORE::UInt64 receiveActionId           , 
                                                              const StorageBufferMetaData** metaData ,
                                                              TStorageBookmarkInfo& bookmarkInfo     ) const
{GUCEF_TRACE;

    const StorageBufferMetaData* mData = GUCEF_NULL;
    if ( GetStorageBufferMetaDataPtrForReceiveActionId( receiveActionId, &mData ) )
    {
        if ( GUCEF_NULL != metaData )
            *metaData = mData;
        
        CORE::UInt64 containerReceiveActionIdMin = *mData->actionIds.cbegin();
        CORE::UInt64 containerReceiveActionIdMax = *mData->actionIds.crbegin();

        // Now we need to find the actual entry
        // for this we again use our knowledge of the 'receiveActionId' to try and forgo
        // having to seach and compare all IDs. Since they should be ordered
        // we should be able to use the min as an offset
                
        TStorageBookmarkInfo bmInfo;
        bool msgOffsetFound = false;

        // most of the time an exact offset will be an exact match
        // we should only have gaps in case of errors
        CORE::UInt64 actionIdSearchOffset = receiveActionId - containerReceiveActionIdMin;
        if ( receiveActionId == mData->actionIds[ actionIdSearchOffset ] )
        {
            bmInfo.msgIndex = (CORE::UInt32) actionIdSearchOffset;
            bmInfo.offsetInFile = mData->msgOffsetIndex[ actionIdSearchOffset ];
            msgOffsetFound = true;
        }
        else
        {
            // we will have to search for the matching action id a bit
            // its still likely to be around the offset we calculated so lets start there
            // and then fan out

            CORE::UInt64 maxSearch = (CORE::UInt64) mData->actionIds.size(); 
            for ( CORE::UInt64 n=1; n<maxSearch; ++n )
            {
                CORE::Int64 downIndex = ( (CORE::Int64) actionIdSearchOffset ) - n;
                CORE::UInt64 upIndex = actionIdSearchOffset + n;

                if ( downIndex >= 0 )
                {
                    if ( receiveActionId == mData->actionIds[ downIndex ] )
                    {
                        bmInfo.msgIndex = (CORE::UInt32) downIndex;
                        bmInfo.offsetInFile = mData->msgOffsetIndex[ downIndex ];
                        msgOffsetFound = true;
                    }
                }
                if ( upIndex < maxSearch )
                {
                    if ( receiveActionId == mData->actionIds[ upIndex ] )
                    {
                        bmInfo.msgIndex = (CORE::UInt32) upIndex;
                        bmInfo.offsetInFile = mData->msgOffsetIndex[ upIndex ];
                        msgOffsetFound = true;
                    }
                }

                if ( downIndex < 0 && upIndex >= maxSearch )
                    break;
            }
        }

        if ( msgOffsetFound )
        {
            bmInfo.bookmarkFormatVersion = 1; 
            bmInfo.doneWithFile = containerReceiveActionIdMax == receiveActionId;
            bmInfo.vfsFilePath = mData->relatedStorageFile;                    
            return true;
        }

        // No other container is going to have this 
        return false;
    }

    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CStoragePubSubClientTopic::DeriveBookmarkFromMsg( const PUBSUB::CIPubSubMsg& msg    , 
                                                  PUBSUB::CPubSubBookmark& bookmark ) const
{GUCEF_TRACE;

    // since the storage backend is more of a transcribing passthrough it doesnt know what on the message means what. 
    // As such it cannot blindly support this using message attributes. The only exception to that is the receiveActionId
    // which is under this backend's runtime control

    if ( m_config.needSubscribeSupport )
    {
        CORE::UInt64 receiveActionId = msg.GetReceiveActionId();
        if ( receiveActionId > 0 )
        {
            return GetBookmarkForReceiveActionId( receiveActionId, bookmark );
        }
        else
        {
            GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:DeriveBookmarkFromMsg: this feature is only supported for messages sourced from this backend at runtime as it needs the 'receiveActionId'" );
            return false;
        }
    }

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::AcknowledgeReceipt( const PUBSUB::CIPubSubMsg& msg )
{GUCEF_TRACE;

    // since the storage backend is more of a transcribing passthrough it doesnt know what on the message means what. 
    // As such it cannot blindly support this using message attributes. The only exception to that is the receiveActionId
    // which is under this backend's runtime control

    if ( m_config.needSubscribeSupport )
    {
        CORE::UInt64 receiveActionId = msg.GetReceiveActionId();
        if ( receiveActionId > 0 )
        {
            MT::CScopeMutex lock( m_lock );

            TStorageBookmarkInfo bmInfo;
            const StorageBufferMetaData* metaData = GUCEF_NULL;
            if ( GetBookmarkInfoForReceiveActionId( receiveActionId, &metaData, bmInfo ) )
            {
                return AcknowledgeReceiptImpl( bmInfo, metaData );
            }
        }
        else
        {
            GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:DeriveBookmarkFromMsg: this feature is only supported for messages sourced from this backend at runtime as it needs the 'receiveActionId'" );
            return false;
        }
    }

    // since the storage backend is more of a transcribing passthrough it doesnt know what on the message means what. As such it cannot support this
    // treat as advisory fyi
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::AcknowledgeReceipt( const PUBSUB::CPubSubBookmark& bookmark )
{GUCEF_TRACE;

    TStorageBookmarkInfo bm;
    if ( SyncBookmarkToBookmarkInfo( bookmark, bm ) )
    {
        MT::CScopeMutex lock( m_lock );
        return AcknowledgeReceiptImpl( bm, GUCEF_NULL );
    }
    
    GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:AcknowledgeReceipt: Unable to interpret generic bookmark" );
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::AcknowledgeReceiptImpl( const TStorageBookmarkInfo& bookmark  , 
                                                   const StorageBufferMetaData* metaData )
{GUCEF_TRACE;

    const CStoragePubSubClientConfig& clientConfig = m_client->GetConfig();

    //
    //
    //if ( !m_pubsubBookmarkPersistence.IsNULL() )
    //{
    //    if ( m_pubsubBookmarkPersistence->StoreBookmark( CORE::CString::Empty, *m_client, *this, bookmark ) )
    //    {
    //        
    //    }
    //}

    // see if we can free up the read buffer and thus the meta data and thus read the next container if any exists

    if ( 0 != bookmark.doneWithFile )
    {
        // we are not done with this container file yet
    }


    return false;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::IsConnected( void ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    // @TODO: Add a VFS based access test
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::IsHealthy( void ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    bool newIsHealthyState = true;
    
    if ( m_config.maxStorageCorruptionDetectionsToBeHealthy >= 0 )
    {
        // Current and last metrics cycle error count counts against the max
        if ( m_storageCorruptionDetections + m_metrics.storageCorruptionDetections > (CORE::UInt32) m_config.maxStorageCorruptionDetectionsToBeHealthy )
            newIsHealthyState = false;
    }

    if ( newIsHealthyState && m_config.maxStorageDeserializationFailuresToBeHealthy >= 0 )
    {
        // Current and last metrics cycle error count counts against the max
        if ( m_storageDeserializationFailures + m_metrics.storageDeserializationFailures > (CORE::UInt32) m_config.maxStorageDeserializationFailuresToBeHealthy )
            newIsHealthyState = false;
    }

    if ( newIsHealthyState && GUCEF_NULL != m_reconnectTimer && m_reconnectTimer->GetEnabled() )
    {
        // running a reconnect timer cannot be good
        newIsHealthyState = false;
    }

    if ( newIsHealthyState != m_isHealthy )
    {
        m_isHealthy = newIsHealthyState;

        if ( m_isHealthy )
        {
            GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:IsHealthy: Topic " + m_config.topicName + " is now healthy" );
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:IsHealthy: Topic " + m_config.topicName + " is now unhealthy" );         
        }

        lock.EarlyUnlock();
        THealthStatusChangeEventData eData( newIsHealthyState ); 
        NotifyObservers( HealthStatusChangeEvent, &eData ); 
    }
    
    return newIsHealthyState;
}

/*-------------------------------------------------------------------------*/

void
CStoragePubSubClientTopic::UpdateIsHealthyStatus( bool newStatus )
{GUCEF_TRACE;

    // See if this is a change from the current persisted state
    if ( newStatus != m_isHealthy )
    {
        // Make sure we use the same consistent logic to determine health
        // the caller might only know about one aspect of health
        // The IsHealthy check itself will update the status and perform eventing if needed
        newStatus = IsHealthy();
    }    
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::InitializeConnectivity( void )
{GUCEF_TRACE;

    m_buffers.SetNrOfBuffers( m_config.desiredNrOfBuffers );
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
CStoragePubSubClientTopic::OnEndOfASyncVfsWork( void )
{GUCEF_TRACE;

    if ( m_config.needPublishSupport )
    {
        // Finalize the write buffer content, there wont be any additional content at this time
        FinalizeWriteBuffer();

        // Persist next ready to read buffer
        StoreNextReceivedPubSubBuffer();
    }
}

/*-------------------------------------------------------------------------*/

void
CStoragePubSubClientTopic::PerformASyncVfsWork( void )
{GUCEF_TRACE;

    if ( m_config.needPublishSupport )
    {
        StoreNextReceivedPubSubBuffer();
    }
    else
    {
        LocateFilesForStorageToPubSubRequest();
        ProcessNextPubSubRequestRelatedFile();
        TransmitNextPubSubMsgBuffer();
    }
}

/*-------------------------------------------------------------------------*/

void
CStoragePubSubClientTopic::OnSyncVfsOpsTimerCycle( CORE::CNotifier* notifier    ,
                                                   const CORE::CEvent& eventId  ,
                                                   CORE::CICloneable* eventData )
{GUCEF_TRACE;

    if ( m_config.needPublishSupport )
    {
        StoreNextReceivedPubSubBuffer();
    }
    else
    {
        LocateFilesForStorageToPubSubRequest();
        ProcessNextPubSubRequestRelatedFile();
        TransmitNextPubSubMsgBuffer();
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
    , largestBufferSizeInBytes( 0 )
    , msgsLoadedFromStorage( 0 )
    , msgBytesLoadedFromStorage( 0 )
    , msgsWrittenToStorage( 0 )
    , msgBytesWrittenToStorage( 0 )
    , storageCorruptionDetections( 0 )
    , storageDeserializationFailures( 0 )
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
CStoragePubSubClientTopic::OnBufferContentTimeWindowCheckCycle( CORE::CNotifier* notifier    ,
                                                                const CORE::CEvent& eventId  ,
                                                                CORE::CICloneable* eventData )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_currentWriteBuffer )
    {
        if ( m_lastWriteBlockCompletion.GetTimeDifferenceInMillisecondsToNow() >= m_config.desiredMaxTimeToWaitToGrowSerializedBlockSizeInMs )
        {
            FinalizeWriteBuffer();
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CStoragePubSubClientTopic::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                                const CORE::CEvent& eventId  ,
                                                CORE::CICloneable* eventData )
{GUCEF_TRACE;

    const PUBSUB::CPubSubClientConfig& clientConfig = m_client->GetConfig();

    CORE::UInt32 smallest = 0;
    CORE::UInt32 largest = 0;
    m_buffers.GetBufferSizeRange( smallest, largest );

    m_metrics.queuedReadyToReadBuffers = m_buffers.GetBuffersQueuedToRead();
    m_metrics.smallestBufferSizeInBytes = smallest;
    m_metrics.largestBufferSizeInBytes = largest;
    m_metrics.msgsLoadedFromStorage = GetMsgsLoadedFromStorageCounter( true );
    m_metrics.msgBytesLoadedFromStorage = GetMsgBytesLoadedFromStorageCounter( true );
    m_metrics.msgsWrittenToStorage = GetMsgsWrittenToStorageCounter( true );
    m_metrics.msgBytesWrittenToStorage = GetMsgBytesWrittenToStorageCounter( true );
    m_metrics.storageCorruptionDetections = GetStorageCorruptionDetectionCounter( true );
    m_metrics.storageDeserializationFailures = GetStorageDeserializationFailuresCounter( true );

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
    VFS::CVFS::TStringVector index;
    vfs.GetFileList( index, m_vfsRootPath, false, true, fileFilter, true );

    // The index is already alphabetically ordered and since we use the datetime as the part of filename we can leverage that
    // to get the last produced file
    if ( !index.empty() )
    {
        VFS::CVFS::TStringVector::reverse_iterator f = index.rbegin();
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
                                                 PUBSUB::CPubSubBookmark& bookmark )
{GUCEF_TRACE;

    // @TODO: Update to use dedicated bookmark persistance

    CORE::CVariant msgId;
    CORE::CDateTime msgDt;
    if ( GetLastPersistedMsgAttributes( channelId, topicName, msgId, msgDt ) )
    {
        if ( msgId.IsInitialized() )
        {
            bookmark.SetBookmarkData( msgId );
            bookmark.SetBookmarkType( PUBSUB::CPubSubBookmark::BOOKMARK_TYPE_MSG_ID );
        }
        else
        {
            CORE::CVariant dtStrVar = msgDt.ToIso8601DateTimeString( true, true );
            bookmark.SetBookmarkData( dtStrVar );
            bookmark.SetBookmarkType( PUBSUB::CPubSubBookmark::BOOKMARK_TYPE_MSG_DATETIME );
        }
        return true;
    }

    bookmark.SetBookmarkType( PUBSUB::CPubSubBookmark::BOOKMARK_TYPE_NOT_AVAILABLE );
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

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:LoadStorageFile: Decoded and loaded persisted file. CodeFamily:" + m_config.decodeCodecFamily +
            " CodecName: " + m_config.decodeCodecName + ". VFS File: \"" + vfsPath + "\". encode ratio: " + CORE::ToString( m_encodeSizeRatio ) );
    }
    else
    {
        // Not using any encoding, load the file as-is
        if ( !vfs.LoadFile( targetBuffer, vfsPath, "rb" ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:LoadStorageFile: Cannot load last persisted file. VFS File: " + vfsPath );
            return false;
        }

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:LoadStorageFile: Loaded VFS File: " + vfsPath );
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
        PUBSUB::CBasicPubSubMsg msg;
        if ( !PUBSUB::CPubSubMsgContainerBinarySerializer::DeserializeMsgAtIndex( msg, true, lastStorageFileContent, 0, false, isCorrupted ) )
        {
            if ( isCorrupted )
            {
                // Attempt to recover what we can with an index rebuild
                // This could effectively move the "last" message received to the actually non-corrupt persisted message as the new "last"
                GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:GetLastPersistedMsgAttributes: Failed to deserialize the last message, will attempt an index rebuild of the corrupt container" );

                CORE::UInt32 bytesRead = 0;
                PUBSUB::CPubSubMsgContainerBinarySerializer::TMsgOffsetIndex newRecoveredIndex;
                if ( PUBSUB::CPubSubMsgContainerBinarySerializer::IndexRebuildScan( newRecoveredIndex, lastStorageFileContent, bytesRead, true ) )
                {
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:GetLastPersistedMsgAttributes: Successfully performed an index rebuild of the corrupt container, discovered " + CORE::ToString( newRecoveredIndex.size() ) + " messages. Will attempt to add a new footer" );

                    CORE::UInt32 bytesWritten = 0;
                    if ( PUBSUB::CPubSubMsgContainerBinarySerializer::SerializeFooter( newRecoveredIndex, lastStorageFileContent.GetDataSize()-1, lastStorageFileContent, bytesWritten ) )
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
                if ( !PUBSUB::CPubSubMsgContainerBinarySerializer::DeserializeMsgAtIndex( msg, true, lastStorageFileContent, 0, false, isCorrupted ) )
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
CStoragePubSubClientTopic::GetTimestampsFromContainerFilename( const CORE::CString& fullPath     ,
                                                               CORE::CDateTime& firstMsgDt       ,
                                                               CORE::CDateTime& lastMsgDt        ,
                                                               CORE::CDateTime& containerStartDt ) const
{GUCEF_TRACE;

    // first strip the extra stuff from the full path to get the string form timestamps
    CORE::CString segment = CORE::ExtractFilename( fullPath );
    segment = segment.CutChars( m_vfsFilePostfix.Length(), false, 0 );
    CORE::CString firstMsgDtStr = segment.SubstrToChar( '_', true, false );
    CORE::CString lastMsgDtStr = segment.SubstrToChar( '_', firstMsgDtStr.Length()+1, true, false );
    CORE::CString containerStartDtStr = segment.SubstrToChar( '_', false, false  );

    // Try to parse what is left as a valid ISO 8601 DateTime
    if ( firstMsgDt.FromIso8601DateTimeString( firstMsgDtStr ) &&
         lastMsgDt.FromIso8601DateTimeString( lastMsgDtStr ) &&
         containerStartDt.FromIso8601DateTimeString( containerStartDtStr ) )
        return true;

    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:GetTimestampsFromContainerFilename: Failed to parse timestamps from filename " + segment );
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::GetPathsToPubSubStorageFiles( const CORE::CDateTime& startDt  ,
                                                         const CORE::CDateTime& endDt    ,
                                                         CORE::CString::StringSet& files ) const
{GUCEF_TRACE;

    bool totalSuccess = true;
    VFS::CVFS& vfs = VFS::CVfsGlobal::Instance()->GetVfs();

    CORE::CString fileFilter = '*' + m_vfsFilePostfix;
    VFS::CVFS::TStringVector index;
    if ( vfs.GetFileList( index, m_vfsRootPath, false, true, fileFilter, true ) )
    {
        VFS::CVFS::TStringVector::iterator i = index.begin();
        while ( i != index.end() )
        {
            CORE::CDateTime containerFileFirstMsgDt;
            CORE::CDateTime containerFileLastMsgDt;
            CORE::CDateTime containerFileCaptureDt;
            if ( GetTimestampsFromContainerFilename( (*i), containerFileFirstMsgDt, containerFileLastMsgDt, containerFileCaptureDt ) )
            {
                // Check the container first messgage dt against the our time range
                // It is assumed here that the containers have messages chronologically ordered
                if ( containerFileFirstMsgDt.IsWithinRange( startDt, endDt ) || containerFileLastMsgDt.IsWithinRange( startDt, endDt ) )
                {
                    files.insert( (*i) );
                }
            }
            else
            {
                totalSuccess = false;
            }
            ++i;
        }
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:GetPathsToPubSubStorageFiles: VFS failure obtaining container list" );
        totalSuccess = false;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

void
CStoragePubSubClientTopic::AddPublishActionIdsToNotify( const TPublishActionIdVector& publishActionIds, bool success )
{GUCEF_TRACE;

    size_t idCount = publishActionIds.size();
    if ( success )
    {
        for ( size_t i=0; i<idCount; ++i )
        {
            m_publishSuccessActionIds.push_back( publishActionIds[ i ] );
        }
    }
    else
    {
        for ( size_t i=0; i<idCount; ++i )
        {
            m_publishFailureActionIds.push_back( publishActionIds[ i ] );
        }
    }
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::StoreNextReceivedPubSubBuffer( void )
{GUCEF_TRACE;

    CORE::CDynamicBufferSwap& buffers = GetSerializedMsgBuffers();

    CORE::CDateTime msgBatchDt;
    if ( GUCEF_NULL == m_currentReadBuffer )
        m_currentReadBuffer = buffers.GetNextReaderBuffer( msgBatchDt, false, 25 );
    else
        msgBatchDt = buffers.GetCurrenReaderBufferAssociatedDt();

    if ( GUCEF_NULL != m_currentReadBuffer )
    {
        if ( msgBatchDt == CORE::CDateTime::Empty )
            msgBatchDt = CORE::CDateTime::NowUTCDateTime();

        // Get the timestamp of the last message in the buffer.
        bool isCorrupted = false;
        bool isMsgDtSupported = false;
        CORE::CDateTime firstMsgDt;
        CORE::CDateTime lastMsgDt;
        if ( !PUBSUB::CPubSubMsgContainerBinarySerializer::DeserializeFirstAndLastMsgDateTime( firstMsgDt, lastMsgDt, *m_currentReadBuffer, isMsgDtSupported, isCorrupted ) )
        {
            if ( !isMsgDtSupported )
            {
                firstMsgDt = msgBatchDt;
                lastMsgDt = CORE::CDateTime::NowUTCDateTime();
            }
        }


        StorageBufferMetaData* bufferMetaData = &( m_storageBufferMetaData[ m_currentReadBuffer ] );

        VFS::CVFS& vfs = VFS::CVfsGlobal::Instance()->GetVfs();

        if ( m_config.encodeCodecFamily.IsNULLOrEmpty() || m_config.encodeCodecName.IsNULLOrEmpty() )
        {
            // Check if the file does not already exists which is highly unlikely working with buffers and a millisecond batch callout
            CORE::CString vfsFilename;
            CORE::CString vfsStoragePath;
            UInt32 loopIndex = 0;
            do 
            {
                msgBatchDt.SetMilliseconds( msgBatchDt.GetMilliseconds() + loopIndex );
                vfsFilename = firstMsgDt.ToIso8601DateTimeString( false, true ) + '_' + lastMsgDt.ToIso8601DateTimeString( false, true )  + '_' + msgBatchDt.ToIso8601DateTimeString( false, true ) + m_vfsFilePostfix;
                vfsStoragePath = CORE::CombinePath( m_vfsRootPath, vfsFilename );
                ++loopIndex;
            }
            while ( vfs.FileExists( vfsStoragePath ) );
            
            if ( vfs.StoreAsFile( vfsStoragePath, *m_currentReadBuffer, 0, true ) )
            {
                m_msgsWrittenToStorage += (UInt32) bufferMetaData->msgOffsetIndex.size();
                m_msgBytesWrittenToStorage += m_currentReadBuffer->GetDataSize();
                AddPublishActionIdsToNotify( bufferMetaData->actionIds, true );

                buffers.SignalEndOfReading();
                m_currentReadBuffer = GUCEF_NULL;

                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:StoreNextReceivedPubSubBuffer: Successfully stored pub-sub mesage block at: " + vfsStoragePath );
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "StoragePubSubClientTopic:StoreNextReceivedPubSubBuffer: StoreAsFile() Failed" );
                AddPublishActionIdsToNotify( bufferMetaData->actionIds, false );
            }
        }
        else
        {
            // Check if the file does not already exists which is highly unlikely working with buffers and a millisecond batch callout
            CORE::CString vfsFilename;
            CORE::CString vfsStoragePath;
            CORE::CString vfsEncodedStoragePath;
            UInt32 loopIndex = 0;
            do 
            {
                msgBatchDt.SetMilliseconds( msgBatchDt.GetMilliseconds() + loopIndex );
                vfsFilename = firstMsgDt.ToIso8601DateTimeString( false, true ) + '_' + lastMsgDt.ToIso8601DateTimeString( false, true )  + '_' + msgBatchDt.ToIso8601DateTimeString( false, true ) + m_vfsFilePostfix;
                vfsStoragePath = CORE::CombinePath( m_vfsRootPath, vfsFilename );
                vfsEncodedStoragePath = vfsStoragePath + '.' + m_config.encodeCodecName;  
                ++loopIndex;
            }
            while ( vfs.FileExists( vfsEncodedStoragePath ) );

            if ( vfs.EncodeAsFile( *m_currentReadBuffer, 0, vfsStoragePath, true, m_config.encodeCodecFamily, m_config.encodeCodecName ) )
            {
                m_msgsWrittenToStorage += (UInt32) bufferMetaData->msgOffsetIndex.size();
                m_msgBytesWrittenToStorage += m_currentReadBuffer->GetDataSize();
                m_encodeSizeRatio = (CORE::Float32) ( m_currentReadBuffer->GetDataSize() / ( 1.0f * vfs.GetFileSize( vfsStoragePath ) ) );
                AddPublishActionIdsToNotify( bufferMetaData->actionIds, true );

                buffers.SignalEndOfReading();
                m_currentReadBuffer = GUCEF_NULL;

                GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:StoreNextReceivedPubSubBuffer: Successfully encoded and stored pub-sub mesage block at: \"" + vfsStoragePath + "\" with a encoded size ratio of " + CORE::ToString( m_encodeSizeRatio ) );
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_CRITICAL, "StoragePubSubClientTopic:StoreNextReceivedPubSubBuffer: EncodeAsFile() Failed" );
                AddPublishActionIdsToNotify( bufferMetaData->actionIds, false );
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
CStoragePubSubClientTopic::LocateFilesForStorageToPubSubRequest( void )
{GUCEF_TRACE;

    bool totalSuccess = true;
    while ( !m_stage1StorageToPubSubRequests.empty() )
    {
        StorageToPubSubRequest& queuedRequest = (*m_stage1StorageToPubSubRequests.begin());

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:LocateFilesForStorageToPubSubRequest: Request for messages in range " +
            CORE::ToString( queuedRequest.startDt ) + " to " + CORE::ToString( queuedRequest.endDt ) );

        if ( GetPathsToPubSubStorageFiles( queuedRequest.startDt                      ,
                                           queuedRequest.endDt                        ,
                                           queuedRequest.vfsPubSubMsgContainersToPush ) && !queuedRequest.vfsPubSubMsgContainersToPush.empty() )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:LocateFilesForStorageToPubSubRequest: Available data in the request range spans " +
                CORE::ToString( queuedRequest.vfsPubSubMsgContainersToPush.size() ) + " containers" );

            m_stage2StorageToPubSubRequests.push_front( queuedRequest );
            m_stage1StorageToPubSubRequests.pop_back();
        }
        else
        {
            if ( !queuedRequest.okIfZeroContainersAreFound )
            {
                GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:LocateFilesForStorageToPubSubRequest: Did not obtain any storage paths for time range " +
                    queuedRequest.startDt.ToIso8601DateTimeString( true, true ) + " to " + queuedRequest.endDt.ToIso8601DateTimeString( true, true ) );

                OnUnableToFullFillStorageToPubSubRequest( queuedRequest );
                totalSuccess = false;
            }

            // Are end-of-data (EOD) event messages desired?
            // A request may never find anything so we have to check here for EOD as well instead of just when having processed a set of files
            if ( m_client->GetConfig().desiredFeatures.supportsSubscriptionEndOfDataEvent )
            {
                if ( m_config.treatEveryFullfilledRequestAsEODEvent )
                {
                    m_subscriptionIsAtEndOfData = true;
                    NotifyObservers( SubscriptionEndOfDataEvent );
                }
                else
                {
                    // Only send end of data event if all requests have been fullfilled
                    if ( m_stage1StorageToPubSubRequests.empty() && m_stage2StorageToPubSubRequests.empty() )
                    {
                        m_subscriptionIsAtEndOfData = true;
                        NotifyObservers( SubscriptionEndOfDataEvent );    
                    }
                }
            }

            m_stage1StorageToPubSubRequests.pop_back();
        }
    }

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::ProcessNextPubSubRequestRelatedFile( void )
{GUCEF_TRACE;

    bool endOfData = false;
    bool totalSuccess = true;
    StorageToPubSubRequestDeque::iterator i = m_stage2StorageToPubSubRequests.begin();
    if ( i != m_stage2StorageToPubSubRequests.end() )
    {
        StorageToPubSubRequest& queuedRequest = (*i);

        size_t containersToProcess = queuedRequest.vfsPubSubMsgContainersToPush.size();
        size_t containersProcessed = 0;
        CORE::CString::StringSet::iterator n = queuedRequest.vfsPubSubMsgContainersToPush.begin();
        while ( n != queuedRequest.vfsPubSubMsgContainersToPush.end() )
        {
            bool needContainerSubsetOnly = false;
            bool containerStartIsInRange = true;
            bool containerEndIsInRange = true;

            CORE::CDateTime containerFileFirstMsgDt;
            CORE::CDateTime containerFileLastMsgDt;
            CORE::CDateTime containerCaptureDt;
            if ( GetTimestampsFromContainerFilename( (*n), containerFileFirstMsgDt, containerFileLastMsgDt, containerCaptureDt ) )
            {
                containerStartIsInRange = containerFileFirstMsgDt.IsWithinRange( queuedRequest.startDt, queuedRequest.endDt );
                containerEndIsInRange = containerFileLastMsgDt.IsWithinRange( queuedRequest.startDt, queuedRequest.endDt );
                needContainerSubsetOnly = !( containerStartIsInRange && containerEndIsInRange );

                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:ProcessNextPubSubRequestRelatedFile: Parsed file path container start and end DateTimes. Start=" +
                    CORE::ToString( containerFileFirstMsgDt ) + ", End=" + CORE::ToString( containerFileLastMsgDt ) + ". containerStartIsInRange=" + CORE::ToString( containerStartIsInRange ) +
                    ", containerEndIsInRange=" + CORE::ToString( containerEndIsInRange ) + ", needContainerSubsetOnly=" + CORE::ToString( needContainerSubsetOnly ) );
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:ProcessNextPubSubRequestRelatedFile: Failed to parse start and/or end DateTime from file at path \"" +
                    (*n) + "\". The file will be disregarded as invalid and skipped" );
                queuedRequest.vfsPubSubMsgContainersToPush.erase( (*n) );
                n = queuedRequest.vfsPubSubMsgContainersToPush.begin();
                totalSuccess = false;
                continue;
            }

            if ( GUCEF_NULL == m_currentWriteBuffer )
                m_currentWriteBuffer = GetSerializedMsgBuffers().GetNextWriterBuffer( false, 0 );

            if ( GUCEF_NULL != m_currentWriteBuffer )
            {
                StorageBufferMetaData* bufferMetaData = &( m_storageBufferMetaData[ m_currentWriteBuffer ] );
                bufferMetaData->msgOffsetIndex.clear();
                bufferMetaData->relatedStorageFile = (*n);
                bufferMetaData->linkedRequest = &queuedRequest;
                m_currentWriteBuffer->SetDataSize( 0 );

                if ( needContainerSubsetOnly )
                {
                    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:ProcessNextPubSubRequestRelatedFile: Loading the container and used a subset of its data to serve (part of) the request: " + bufferMetaData->relatedStorageFile );
                    if ( LoadStorageFile( (*n), *m_currentWriteBuffer ) )
                    {
                        // Since we loaded the entire container we need to now efficiently make sure only the subset gets processed
                        // The way we can do that is by editing the footer in the buffer to logically eliminate entries we do not need
                        // This will make it appear as if only the needed entries are in the container to the reader when reading the footer

                        CORE::UInt32 startIndexOffset = 0;
                        CORE::UInt32 endIndexOffset = 0;

                        bool isCorrupted = false;
                        CORE::UInt32 bytesRead = 0;
                        PUBSUB::CPubSubMsgContainerBinarySerializer::TBasicPubSubMsgVector msgs;
                        if ( !PUBSUB::CPubSubMsgContainerBinarySerializer::DeserializeWithRebuild( msgs, true, bufferMetaData->msgOffsetIndex, *m_currentWriteBuffer, isCorrupted, m_config.bestEffortDeserializeIsAllowed ) )
                        {
                            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:ProcessNextPubSubRequestRelatedFile: Failed obtain messages from pubsub msg container at path \"" +
                                (*n) + "\". The file will be disregarded as invalid and skipped" );
                            queuedRequest.vfsPubSubMsgContainersToPush.erase( (*n) );
                            n = queuedRequest.vfsPubSubMsgContainersToPush.begin();
                            totalSuccess = false;
                            continue;
                        }

                        // Check to see if a rebuild was carried out
                        if ( isCorrupted )
                        {
                            // Make sure we log this because messages could be missing
                            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:ProcessNextPubSubRequestRelatedFile: Successfully rebuild index for corrupted pubsub msg container at path \"" +
                                (*n) + "\". Found " + CORE::ToString( bufferMetaData->msgOffsetIndex.size() ) + " recoverable entries in the file" );
                        }

                        // Check to see how many we need to trim from the start
                        if ( !containerStartIsInRange )
                        {
                            PUBSUB::CPubSubMsgContainerBinarySerializer::TBasicPubSubMsgVector::iterator m = msgs.begin();
                            while ( m != msgs.end() )
                            {
                                if ( (*m).GetMsgDateTime() >= queuedRequest.startDt )
                                    break;
                                ++m; ++startIndexOffset;
                            }
                        }
                        if ( !containerEndIsInRange )
                        {
                            PUBSUB::CPubSubMsgContainerBinarySerializer::TBasicPubSubMsgVector::reverse_iterator m = msgs.rbegin();
                            while ( m != msgs.rend() )
                            {
                                if ( (*m).GetMsgDateTime() <= queuedRequest.endDt )
                                    break;
                                ++m; ++endIndexOffset;
                            }
                        }

                        CORE::UInt32 o2=0;
                        std::size_t newIndexSize = bufferMetaData->msgOffsetIndex.size() - ( (std::size_t)startIndexOffset + (std::size_t)endIndexOffset );
                        endIndexOffset = (CORE::UInt32) bufferMetaData->msgOffsetIndex.size() - endIndexOffset;
                        PUBSUB::CPubSubMsgContainerBinarySerializer::TMsgOffsetIndex newOffsetIndex( newIndexSize );
                        for ( CORE::UInt32 o=startIndexOffset; o<endIndexOffset; ++o )
                        {
                            newOffsetIndex[ o2 ] = bufferMetaData->msgOffsetIndex[ o ];
                            ++o2;
                        }

                        // Now we append a new footer in the in-memory container to only have the subset of messages we care about referenced
                        CORE::UInt32 bytesWritten = 0;
                        if ( PUBSUB::CPubSubMsgContainerBinarySerializer::SerializeFooter( newOffsetIndex, m_currentWriteBuffer->GetDataSize()-1, *m_currentWriteBuffer, bytesWritten ) )
                        {
                            // We are done with this container
                            ++containersProcessed;
                            m_currentWriteBuffer = GUCEF_NULL;
                            m_buffers.SignalEndOfWriting();

                            queuedRequest.vfsPubSubMsgContainersToPush.erase( (*n) );
                            n = queuedRequest.vfsPubSubMsgContainersToPush.begin();
                            continue;
                        }
                        else
                        {
                            // @TODO: better error handling
                            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:ProcessNextPubSubRequestRelatedFile: Failed update pubsub msg container footer in write buffer to reflect messages in the requested time range" );
                            queuedRequest.vfsPubSubMsgContainersToPush.erase( (*n) );
                            n = queuedRequest.vfsPubSubMsgContainersToPush.begin();
                            totalSuccess = false;
                            continue;
                        }
                    }
                    else
                    {
                        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:ProcessNextPubSubRequestRelatedFile: Failed load pubsub msg container at path \"" +
                            (*n) + "\". The file will be disregarded as invalid and skipped" );
                        queuedRequest.vfsPubSubMsgContainersToPush.erase( (*n) );
                        n = queuedRequest.vfsPubSubMsgContainersToPush.begin();
                        totalSuccess = false;
                        continue;
                    }
                }
                else
                {
                    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:ProcessNextPubSubRequestRelatedFile: Loading the entire container as-is to serve (part of) the request: " + bufferMetaData->relatedStorageFile );
                    if ( LoadStorageFile( (*n), *m_currentWriteBuffer ) )
                    {
                        // Since we loaded the entire container and we dont need a subset we are done
                        ++containersProcessed;
                        m_currentWriteBuffer = GUCEF_NULL;
                        m_buffers.SignalEndOfWriting();

                        queuedRequest.vfsPubSubMsgContainersToPush.erase( (*n) );
                        n = queuedRequest.vfsPubSubMsgContainersToPush.begin();
                        continue;
                    }
                    else
                    {
                        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:ProcessNextPubSubRequestRelatedFile: Failed load pubsub msg container at path \"" +
                            (*n) + "\". The file will be disregarded as invalid and skipped" );
                        queuedRequest.vfsPubSubMsgContainersToPush.erase( (*n) );
                        n = queuedRequest.vfsPubSubMsgContainersToPush.begin();
                        totalSuccess = false;
                        continue;
                    }
                }
            }
            else
            {
                // No write buffer available, we need to wait before processing more requests
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "StoragePubSubClientTopic:ProcessNextPubSubRequestRelatedFile: No write buffer available" );
                return totalSuccess;
            }
            ++n;
        }

        if ( containersProcessed != containersToProcess )
        {
            OnUnableToFullFillStorageToPubSubRequest( queuedRequest );
        }
       
        // Check to see if we are done with this request
        if ( queuedRequest.vfsPubSubMsgContainersToPush.empty() )
        {
            GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic:ProcessNextPubSubRequestRelatedFile: Completed storage request. Start=" +
                CORE::ToString( queuedRequest.startDt ) + ", End=" + CORE::ToString( queuedRequest.endDt ) +
                " Proccessed " + CORE::ToString( containersProcessed ) + "/" + CORE::ToString( containersToProcess ) + " files successfully" );
                        
            m_stage2StorageToPubSubRequests.pop_front();
            
            // Are end-of-data (EOD) event messages desired?
            if ( m_client->GetConfig().desiredFeatures.supportsSubscriptionEndOfDataEvent )
            {
                if ( m_config.treatEveryFullfilledRequestAsEODEvent )
                {
                    m_subscriptionIsAtEndOfData = endOfData = true;
                    NotifyObservers( SubscriptionEndOfDataEvent );
                }
                else
                {
                    // Only send end of data event if all requests have been fullfilled
                    if ( m_stage1StorageToPubSubRequests.empty() && m_stage2StorageToPubSubRequests.empty() )
                    {
                        m_subscriptionIsAtEndOfData = endOfData = true;
                        NotifyObservers( SubscriptionEndOfDataEvent );    
                    }
                }
            }
        }
    }

    m_subscriptionIsAtEndOfData = endOfData;
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

void
CStoragePubSubClientTopic::OnStoredPubSubMsgTransmissionFailure( const CORE::CDateTime& firstMsgDt )
{GUCEF_TRACE;


}

/*-------------------------------------------------------------------------*/

CORE::UInt32
CStoragePubSubClientTopic::GetMsgsWrittenToStorageCounter( bool resetCounter )
{GUCEF_TRACE;

    if ( resetCounter )
    {
        CORE::UInt32 msgsWrittenToStorage = m_msgsWrittenToStorage;
        m_msgsWrittenToStorage = 0;
        return msgsWrittenToStorage;
    }
    else
        return m_msgsWrittenToStorage;
}

/*-------------------------------------------------------------------------*/

CORE::UInt32
CStoragePubSubClientTopic::GetMsgBytesWrittenToStorageCounter( bool resetCounter )
{GUCEF_TRACE;

    if ( resetCounter )
    {
        CORE::UInt32 msgBytesWrittenToStorage = m_msgBytesWrittenToStorage;
        m_msgBytesWrittenToStorage = 0;
        return msgBytesWrittenToStorage;
    }
    else
        return m_msgBytesWrittenToStorage;
}

/*-------------------------------------------------------------------------*/

CORE::UInt32
CStoragePubSubClientTopic::GetMsgsLoadedFromStorageCounter( bool resetCounter )
{GUCEF_TRACE;

    if ( resetCounter )
    {
        CORE::UInt32 msgsLoadedFromStorage = m_msgsLoadedFromStorage;
        m_msgsLoadedFromStorage = 0;
        return msgsLoadedFromStorage;
    }
    else
        return m_msgsLoadedFromStorage;
}

/*-------------------------------------------------------------------------*/

CORE::UInt32
CStoragePubSubClientTopic::GetMsgBytesLoadedFromStorageCounter( bool resetCounter )
{GUCEF_TRACE;

    if ( resetCounter )
    {
        CORE::UInt32 msgBytesLoadedFromStorage = m_msgBytesLoadedFromStorage;
        m_msgBytesLoadedFromStorage = 0;
        return msgBytesLoadedFromStorage;
    }
    else
        return m_msgBytesLoadedFromStorage;
}

/*-------------------------------------------------------------------------*/

CORE::UInt32
CStoragePubSubClientTopic::GetStorageCorruptionDetectionCounter( bool resetCounter )
{GUCEF_TRACE;

    if ( resetCounter )
    {
        CORE::UInt32 storageCorruptionDetections = m_storageCorruptionDetections;
        m_storageCorruptionDetections = 0;
        return storageCorruptionDetections;
    }
    else
        return m_storageCorruptionDetections;
}

/*-------------------------------------------------------------------------*/

CORE::UInt32
CStoragePubSubClientTopic::GetStorageDeserializationFailuresCounter( bool resetCounter )
{GUCEF_TRACE;

    if ( resetCounter )
    {
        CORE::UInt32 storageDeserializationFailures = m_storageDeserializationFailures;
        m_storageDeserializationFailures = 0;
        return storageDeserializationFailures;
    }
    else
        return m_storageDeserializationFailures;
}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopic::TransmitNextPubSubMsgBuffer( void )
{GUCEF_TRACE;

    CORE::CDateTime firstMsgDt;
    m_currentReadBuffer = m_buffers.GetNextReaderBuffer( firstMsgDt, false, 0 );
    if ( GUCEF_NULL == m_currentReadBuffer )
        return true; // nothing to do

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic(" + CORE::PointerToString( this ) +
        "):TransmitNextPubSubMsgBuffer: New buffer with data is available of " + CORE::ToString( m_currentReadBuffer->GetDataSize() ) + " bytes" );

    // We now link logical message objects to the data in the buffer
    CORE::UInt32 startIndexOffset = 0;
    CORE::UInt32 endIndexOffset = 0;
    bool isCorrupted = false;
    CORE::UInt32 bytesRead = 0;
    PUBSUB::CPubSubMsgContainerBinarySerializer::TBasicPubSubMsgVector msgs;
    PUBSUB::CPubSubMsgContainerBinarySerializer::TMsgOffsetIndex msgOffsetIndex;
    if ( !PUBSUB::CPubSubMsgContainerBinarySerializer::DeserializeWithRebuild( msgs, true, msgOffsetIndex, *m_currentReadBuffer, isCorrupted, m_config.bestEffortDeserializeIsAllowed ) )
    {
        // update metrics
        if ( isCorrupted )
            ++m_storageCorruptionDetections;
        ++m_storageDeserializationFailures;

        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic(" + CORE::PointerToString( this ) +
            "):TransmitNextPubSubMsgBuffer: Failed to deserialize messages from container. According to the footer the container had " +
            CORE::ToString( msgOffsetIndex.size() ) + " entries. isCorrupted=" + CORE::BoolToString( isCorrupted ) );

        m_buffers.SignalEndOfReading();
        m_currentReadBuffer = GUCEF_NULL;

        OnStoredPubSubMsgTransmissionFailure( firstMsgDt );
        return false;
    }
    if ( isCorrupted )
    {
        // update metrics
        ++m_storageDeserializationFailures;
        ++m_storageCorruptionDetections;

        GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "StoragePubSubClientTopic(" + CORE::PointerToString( this ) +
            "):TransmitNextPubSubMsgBuffer: Deserialize messages from container but corruption was detected. According to the index the container has " +
            CORE::ToString( msgOffsetIndex.size() ) + " entries" );
    }

    // update metrics
    m_msgsLoadedFromStorage += static_cast< CORE::UInt32 >( msgs.size() );
    m_msgBytesLoadedFromStorage += m_currentReadBuffer->GetDataSize();

    PUBSUB::CPubSubMsgContainerBinarySerializer::TBasicPubSubMsgVector::iterator i = msgs.begin();
    while ( i != msgs.end() )
    {
        PUBSUB::CBasicPubSubMsg& msg = (*i);

        msg.SetOriginClientTopic( this );

        msg.SetReceiveActionId( m_currentReceiveActionId );
        ++m_currentReceiveActionId;

        m_pubsubMsgsRefs.push_back( TPubSubMsgRef( &msg ) );

        ++i;
    }

    if ( !NotifyObservers( MsgsRecievedEvent, &m_pubsubMsgsRefs ) )
        return true;

    m_pubsubMsgsRefs.clear();
    m_buffers.SignalEndOfReading();
    m_currentReadBuffer = GUCEF_NULL;
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CStoragePubSubClientTopic::IsSubscriptionAtEndOfData( void ) const
{GUCEF_TRACE;

    return m_subscriptionIsAtEndOfData;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace STORAGE */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
