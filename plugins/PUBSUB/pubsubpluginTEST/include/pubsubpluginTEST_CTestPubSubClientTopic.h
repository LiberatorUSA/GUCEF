/*
 *  pubsubpluginTEST: Generic GUCEF PUBSUB plugin for adding integration/system tests
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

#ifndef PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTTOPIC_H
#define PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTTOPIC_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <deque>

#ifndef GUCEF_CORE_CTIMER_H
#include "CTimer.h"
#define GUCEF_CORE_CTIMER_H
#endif /* GUCEF_CORE_CTIMER_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFERSWAP_H
#include "gucefCORE_CDynamicBufferSwap.h"
#define GUCEF_CORE_CDYNAMICBUFFERSWAP_H
#endif /* GUCEF_CORE_CDYNAMICBUFFERSWAP_H ? */

#ifndef GUCEF_COMCORE_CHOSTADDRESS_H
#include "CHostAddress.h"
#define GUCEF_COMCORE_CHOSTADDRESS_H
#endif /* GUCEF_COMCORE_CHOSTADDRESS_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBBOOKMARK_H
#include "gucefPUBSUB_CPubSubBookmark.h"
#define GUCEF_PUBSUB_CPUBSUBBOOKMARK_H
#endif /* GUCEF_PUBSUB_CPUBSUBBOOKMARK_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBCLIENTTOPIC_H
#include "gucefPUBSUB_CPubSubClientTopic.h"
#define GUCEF_PUBSUB_CPUBSUBCLIENTTOPIC_H
#endif /* GUCEF_PUBSUB_CPUBSUBCLIENTTOPIC_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBMSGBINARYSERIALIZER_H
#include "gucefPUBSUB_CPubSubMsgBinarySerializer.h"
#define GUCEF_PUBSUB_CPUBSUBMSGBINARYSERIALIZER_H
#endif /* GUCEF_PUBSUB_CPUBSUBMSGBINARYSERIALIZER_H ? */

#ifndef PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTTOPICCONFIG_H
#include "pubsubpluginTEST_CTestPubSubClientTopicConfig.h"
#define PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTTOPICCONFIG_H
#endif /* PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTTOPICCONFIG_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUBPLUGIN {
namespace TEST {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CTestPubSubClient;

class PUBSUBPLUGIN_TEST_PLUGIN_PRIVATE_CPP CTestPubSubClientTopic : public PUBSUB::CPubSubClientTopic
{
    public:

    typedef enum CTestPubSubClientTopicConfig::EChannelMode  TChannelMode;
    typedef std::vector< CORE::UInt32 >                         UInt32Vector;

    #pragma pack(push, 1)  // No structure packing
    struct SStorageBookmarkInfo
    {
        UInt8 bookmarkFormatVersion;
        UInt8 doneWithFile;
        CORE::UInt32 msgIndex;
        CORE::UInt32 offsetInFile;
        CORE::CString vfsFilePath;
    };
    typedef struct SStorageBookmarkInfo TStorageBookmarkInfo;
    #pragma pack(pop)

    CTestPubSubClientTopic( CTestPubSubClient* client );

    virtual ~CTestPubSubClientTopic() GUCEF_VIRTUAL_OVERRIDE;

    virtual PUBSUB::CPubSubClient* GetClient( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool InitializeConnectivity( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Disconnect( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsConnected( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsHealthy( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsPublishingSupported( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsSubscribingSupported( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Subscribe( void ) GUCEF_VIRTUAL_OVERRIDE;
    
    virtual bool SubscribeStartingAtMsgId( const CORE::CVariant& msgIdBookmark );

    virtual bool SubscribeStartingAtMsgDateTime( const CORE::CDateTime& msgDtBookmark );

    virtual bool SubscribeStartingAtBookmark( const PUBSUB::CPubSubBookmark& bookmark ) GUCEF_VIRTUAL_OVERRIDE;

    bool SubscribeStartingAtBookmarkInfo( const TStorageBookmarkInfo& bookmarkInfo );

    virtual PUBSUB::CPubSubBookmark GetCurrentBookmark( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CORE::CString& GetTopicName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    const CORE::CString& GetMetricFriendlyTopicName( void ) const;

    virtual bool Publish( TPublishActionIdVector& publishActionIds, const PUBSUB::CBasicPubSubMsg::TBasicPubSubMsgVector& msgs, bool notify ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Publish( TPublishActionIdVector& publishActionIds, const TIPubSubMsgConstRawPtrVector& msgs, bool notify ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Publish( TPublishActionIdVector& publishActionIds, const TIPubSubMsgRawPtrVector& msgs, bool notify ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Publish( TPublishActionIdVector& publishActionIds, const TIPubSubMsgSPtrVector& msgs, bool notify ) GUCEF_VIRTUAL_OVERRIDE;    
    virtual bool Publish( TPublishActionIdVector& publishActionIds, const TPubSubMsgsRefVector& msgs, bool notify ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Publish( CORE::UInt64& publishActionId, const PUBSUB::CIPubSubMsg& msg, bool notify ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool AcknowledgeReceipt( const PUBSUB::CIPubSubMsg& msg ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool AcknowledgeReceipt( const PUBSUB::CPubSubBookmark& bookmark ) GUCEF_VIRTUAL_OVERRIDE;
    bool AcknowledgeReceiptImpl( const PUBSUB::CPubSubBookmark& bookmark, CORE::UInt64 receiveActionId );

    virtual bool DeriveBookmarkFromMsg( const PUBSUB::CIPubSubMsg& msg, PUBSUB::CPubSubBookmark& bookmark ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool SaveConfig( PUBSUB::CPubSubClientTopicConfig& config ) const;

    virtual bool LoadConfig( const PUBSUB::CPubSubClientTopicConfig& config );

    class TopicMetrics
    {
        public:

        TopicMetrics( void );      

        CORE::UInt32 queuedReadyToReadBuffers;
        CORE::UInt32 smallestBufferSizeInBytes;
        CORE::UInt32 largestBufferSizeInBytes;
        CORE::UInt32 msgsLoadedFromStorage;
        CORE::UInt32 msgBytesLoadedFromStorage;
        CORE::UInt32 msgsWrittenToStorage;
        CORE::UInt32 msgBytesWrittenToStorage;
        CORE::UInt32 storageCorruptionDetections;
        CORE::UInt32 storageDeserializationFailures;
    };

    const TopicMetrics& GetMetrics( void ) const;

    const CTestPubSubClientTopicConfig& GetTopicConfig( void ) const;

    void
    OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                         const CORE::CEvent& eventId  ,
                         CORE::CICloneable* eventData );
    
    virtual const MT::CILockable* AsLockable( void ) const GUCEF_VIRTUAL_OVERRIDE;

    protected:

    virtual bool Lock( CORE::UInt32 lockWaitTimeoutInMs = GUCEF_MT_DEFAULT_LOCK_TIMEOUT_IN_MS ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Unlock( void ) const GUCEF_VIRTUAL_OVERRIDE;

    private:

    typedef CORE::CTEventHandlerFunctor< CTestPubSubClientTopic > TEventCallback;

    // Types to implement/hook-up topic interface
    typedef std::vector< PUBSUB::CBasicPubSubMsg > TPubSubMsgsVector;

    private:

    class StorageToPubSubRequest : public CORE::CIConfigurable
    {
        public:

        CORE::CDateTime startDt;
        CORE::CDateTime endDt;
        CORE::CString::StringSet vfsPubSubMsgContainersToPush;

        StorageToPubSubRequest( void );
        StorageToPubSubRequest( const CORE::CDateTime& startDt, const CORE::CDateTime& endDt );
        StorageToPubSubRequest( const StorageToPubSubRequest& src );

        virtual const CORE::CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;
        virtual bool SaveConfig( CORE::CDataNode& tree ) const GUCEF_VIRTUAL_OVERRIDE;
        virtual bool LoadConfig( const CORE::CDataNode& treeroot ) GUCEF_VIRTUAL_OVERRIDE;
    };
    typedef std::deque< StorageToPubSubRequest > StorageToPubSubRequestDeque;
    
    struct SStorageBufferMetaData
    {
        PUBSUB::CPubSubMsgContainerBinarySerializer::TMsgOffsetIndex msgOffsetIndex;
        TPublishActionIdVector publishActionIds; 
        CORE::CString relatedStorageFile;
    };
    typedef struct SStorageBufferMetaData TStorageBufferMetaData;
    typedef std::map< CORE::CDynamicBuffer*, TStorageBufferMetaData > TStorageBufferMetaDataMap;

    bool SyncBookmarkInfoToBookmark( const TStorageBookmarkInfo& info  , 
                                     PUBSUB::CPubSubBookmark& bookmark );

    bool SyncBookmarkToBookmarkInfo( const PUBSUB::CPubSubBookmark& bookmark ,
                                     TStorageBookmarkInfo& info              );
    
    void RegisterEventHandlers( void );

    bool SetupToSubscribe( PUBSUB::CPubSubClientTopicConfig& config );

    static CORE::CString GenerateMetricsFriendlyTopicName( const CORE::CString& topicName );
    static CORE::CString GenerateDefaultVfsStorageContainerFileExt( void );

    template < typename T >
    bool PublishViaMsgPtrs( TPublishActionIdVector& publishActionIds, const std::vector< T >& msgs, bool notify );

    bool GetPersistedBookmark( CORE::Int32 channelId              , 
                               const CORE::CString& topicName     , 
                               PUBSUB::CPubSubBookmark& bookmark );

    bool GetLastPersistedMsgAttributes( CORE::Int32 channelId          , 
                                        const CORE::CString& topicName , 
                                        CORE::CVariant& msgId          , 
                                        CORE::CDateTime& msgDt         );

    bool GetLastPersistedMsgAttributesWithOffset( CORE::Int32 channelId          , 
                                                  const CORE::CString& topicName , 
                                                  CORE::CVariant& msgId          , 
                                                  CORE::CDateTime& msgDt         ,
                                                  CORE::UInt32 lastFileOffset    ,
                                                  bool& fileExistedButHasIssue   );

    bool
    GetPathsToPubSubStorageFiles( const CORE::CDateTime& startDt  ,
                                  const CORE::CDateTime& endDt    ,
                                  CORE::CString::StringSet& files ) const;

    bool AddStorageToPubSubRequest( const StorageToPubSubRequest& request );

    void OnUnableToFullFillStorageToPubSubRequest( const StorageToPubSubRequest& failedRequest );

    CORE::CString GetPathToLastWrittenPubSubStorageFile( CORE::UInt32 lastOffset ) const;

    bool StoreNextReceivedPubSubBuffer( void );

    bool ProcessNextPubSubRequestRelatedFile( void );

    bool LocateFilesForStorageToPubSubRequest( void );

    bool LoadStorageFile( const CORE::CString& vfsPath       ,
                          CORE::CDynamicBuffer& targetBuffer );

    bool GetTimestampsFromContainerFilename( const CORE::CString& fullPath     ,
                                             CORE::CDateTime& firstMsgDt       ,
                                             CORE::CDateTime& lastMsgDt        ,
                                             CORE::CDateTime& containerStartDt ) const;

    CORE::CDynamicBufferSwap& GetSerializedMsgBuffers( void );
    
    bool TransmitNextPubSubMsgBuffer( void );

    void OnStoredPubSubMsgTransmissionFailure( const CORE::CDateTime& firstMsgDt );

    void
    OnReconnectTimerCycle( CORE::CNotifier* notifier    ,
                           const CORE::CEvent& eventId  ,
                           CORE::CICloneable* eventData );
    
    void
    OnSyncVfsOpsTimerCycle( CORE::CNotifier* notifier    ,
                            const CORE::CEvent& eventId  ,
                            CORE::CICloneable* eventData );

    void
    OnBufferContentTimeWindowCheckCycle( CORE::CNotifier* notifier    ,
                                         const CORE::CEvent& eventId  ,
                                         CORE::CICloneable* eventData );

    friend class CTestPubSubClientTopicVfsTask;

    void PerformASyncVfsWork( void );
    void OnEndOfASyncVfsWork( void );

    bool BeginVfsOps( void );
    bool StopVfsOps( void );

    void FinalizeWriteBuffer( TStorageBufferMetaData* bufferMetaData, CORE::UInt32 bufferOffset );
    void FinalizeWriteBuffer( void );

    void AddPublishActionIdsToNotify( const TPublishActionIdVector& publishActionIds, bool success );

    CORE::CString ResolveVfsRootPath( void ) const;

    void UpdateIsHealthyStatus( bool newStatus );
    
    void
    OnPulseCycle( CORE::CNotifier* notifier    ,
                  const CORE::CEvent& eventId  ,
                  CORE::CICloneable* eventData );

    
    CORE::UInt32 GetMsgsLoadedFromStorageCounter( bool resetCounter );
    CORE::UInt32 GetMsgBytesLoadedFromStorageCounter( bool resetCounter );
    CORE::UInt32 GetMsgsWrittenToStorageCounter( bool resetCounter );
    CORE::UInt32 GetMsgBytesWrittenToStorageCounter( bool resetCounter );
    CORE::UInt32 GetStorageCorruptionDetectionCounter( bool resetCounter );
    CORE::UInt32 GetStorageDeserializationFailuresCounter( bool resetCounter );









    void
    OnHealthyStatusToggleTimerCycle( CORE::CNotifier* notifier    ,
                                     const CORE::CEvent& eventId  ,
                                     CORE::CICloneable* eventData );











    private:

    CTestPubSubClient* m_client;
    TPubSubMsgsVector m_pubsubMsgs;
    TMsgsRecievedEventData m_pubsubMsgsRefs;
    CTestPubSubClientTopicConfig m_config;
    CORE::CTimer* m_syncVfsOpsTimer;    
    CORE::CTimer* m_reconnectTimer;
    CORE::CTimer* m_bufferContentTimeWindowCheckTimer;
    MT::CMutex m_lock;
    CORE::UInt64 m_currentPublishActionId;
    CORE::UInt64 m_currentReceiveActionId;
    TPublishActionIdVector m_publishSuccessActionIds;
    TMsgsPublishedEventData m_publishSuccessActionEventData;
    TPublishActionIdVector m_publishFailureActionIds;
    TMsgsPublishFailureEventData m_publishFailureActionEventData;
    TopicMetrics m_metrics;
    CORE::CString m_metricFriendlyTopicName;
    mutable bool m_isHealthy;

    CORE::CDynamicBuffer* m_currentReadBuffer;
    CORE::CDynamicBuffer* m_currentWriteBuffer;
    TStorageBookmarkInfo m_currentBookmarkInfo;
    PUBSUB::CPubSubBookmark m_currentBookmark;
    CORE::CString m_vfsFilePostfix;
    CORE::CString m_vfsRootPath;
    CORE::CVariant m_lastPersistedMsgId;
    CORE::CDateTime m_lastPersistedMsgDt;
    CORE::Float32 m_encodeSizeRatio;
    StorageToPubSubRequestDeque m_stage1StorageToPubSubRequests;
    StorageToPubSubRequestDeque m_stage2StorageToPubSubRequests;
   
    CORE::CDynamicBufferSwap m_buffers;
    CORE::CDateTime m_lastWriteBlockCompletion;    
    TStorageBufferMetaDataMap m_storageBufferMetaData;

    CORE::UInt32 m_msgsLoadedFromStorage;
    CORE::UInt32 m_msgBytesLoadedFromStorage;
    CORE::UInt32 m_msgsWrittenToStorage;
    CORE::UInt32 m_msgBytesWrittenToStorage;
    CORE::UInt32 m_storageCorruptionDetections;
    CORE::UInt32 m_storageDeserializationFailures;



    mutable bool m_hasFakeHealthIssue;
    mutable CORE::CTimer m_healthyStatusToggleTimer;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace TEST */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/

#endif /* PUBSUBPLUGIN_TEST_CTESTPUBSUBCLIENTTOPIC_H ? */
