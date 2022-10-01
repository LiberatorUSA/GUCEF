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

#ifndef PUBSUBPLUGIN_AWSSQS_CAWSSQSPUBSUBCLIENTTOPIC_H
#define PUBSUBPLUGIN_AWSSQS_CAWSSQSPUBSUBCLIENTTOPIC_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTIMER_H
#include "CTimer.h"
#define GUCEF_CORE_CTIMER_H
#endif /* GUCEF_CORE_CTIMER_H ? */

#ifndef GUCEF_COMCORE_CHOSTADDRESS_H
#include "CHostAddress.h"
#define GUCEF_COMCORE_CHOSTADDRESS_H
#endif /* GUCEF_COMCORE_CHOSTADDRESS_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBCLIENTTOPIC_H
#include "gucefPUBSUB_CPubSubClientTopic.h"
#define GUCEF_PUBSUB_CPUBSUBCLIENTTOPIC_H
#endif /* GUCEF_PUBSUB_CPUBSUBCLIENTTOPIC_H ? */

#ifndef PUBSUBPLUGIN_AWSSQS_CAWSSQSPUBSUBCLIENTTOPICCONFIG_H
#include "pubsubpluginAWSSQS_CAwsSqsPubSubClientTopicConfig.h"
#define PUBSUBPLUGIN_AWSSQS_CAWSSQSPUBSUBCLIENTTOPICCONFIG_H
#endif /* PUBSUBPLUGIN_AWSSQS_CAWSSQSPUBSUBCLIENTTOPICCONFIG_H ? */

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
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CAwsSqsPubSubClient;

/**
 *  MS Windows AWSSQS implementation of the conceptual pub-sub "topic"
 */
class PUBSUBPLUGIN_AWSSQS_PLUGIN_PRIVATE_CPP CAwsSqsPubSubClientTopic : public PUBSUB::CPubSubClientTopic ,
                                                                        public CORE::CTSharedObjCreator< CAwsSqsPubSubClientTopic, MT::CMutex >
{
    public:

    CAwsSqsPubSubClientTopic( CAwsSqsPubSubClient* client );

    virtual ~CAwsSqsPubSubClientTopic() GUCEF_VIRTUAL_OVERRIDE;

    virtual PUBSUB::CPubSubClient* GetClient( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool InitializeConnectivity( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Disconnect( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsConnected( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsHealthy( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsPublishingSupported( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsSubscribingSupported( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Subscribe( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool SubscribeStartingAtBookmark( const PUBSUB::CPubSubBookmark& bookmark ) GUCEF_VIRTUAL_OVERRIDE;

    virtual PUBSUB::CPubSubBookmark GetCurrentBookmark( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CORE::CString& GetTopicName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Publish( TPublishActionIdVector& publishActionIds, const PUBSUB::CBasicPubSubMsg::TBasicPubSubMsgVector& msgs, bool notify ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Publish( TPublishActionIdVector& publishActionIds, const PUBSUB::CIPubSubMsg::TIPubSubMsgConstRawPtrVector& msgs, bool notify ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool Publish( CORE::UInt64& publishActionId, const PUBSUB::CIPubSubMsg& msg, bool notify ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool AcknowledgeReceipt( const PUBSUB::CIPubSubMsg& msg ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool AcknowledgeReceipt( const PUBSUB::CPubSubBookmark& bookmark ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool SaveConfig( PUBSUB::CPubSubClientTopicConfig& config ) const;

    virtual bool LoadConfig( const PUBSUB::CPubSubClientTopicConfig& config );

    void UnlinkFromParentClient( void );

    void
    OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                         const CORE::CEvent& eventId  ,
                         CORE::CICloneable* eventData );

    void
    OnPulseCycle( CORE::CNotifier* notifier    ,
                  const CORE::CEvent& eventId  ,
                  CORE::CICloneable* eventData );
    
    virtual const MT::CILockable* AsLockable( void ) const GUCEF_VIRTUAL_OVERRIDE;

    protected:

    virtual bool Lock( CORE::UInt32 lockWaitTimeoutInMs = GUCEF_MT_DEFAULT_LOCK_TIMEOUT_IN_MS ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Unlock( void ) const GUCEF_VIRTUAL_OVERRIDE;

    private:

    void RegisterEventHandlers( void );

    void PrepStorageForReadMsgs( CORE::UInt32 msgCount );

    Aws::String GetSqsQueueUrlForQueueName( const CORE::CString& queueName );

    template< class T >
    bool
    TranslateToSqsMsg( T& sqsMsg, const PUBSUB::CIPubSubMsg* msg, CORE::UInt32& msgByteSize );

    private:

    typedef CORE::CTEventHandlerFunctor< CAwsSqsPubSubClientTopic > TEventCallback;
   

    // Types to implement/hook-up topic interface
    typedef std::vector< PUBSUB::CBasicPubSubMsg > TPubSubMsgsVector;
    typedef std::pair< CORE::CDynamicBuffer, CORE::CDynamicBuffer > TBufferPair;
    typedef std::vector< TBufferPair > TBufferVector;

    CAwsSqsPubSubClient* m_client;
    TPubSubMsgsVector m_pubsubMsgs;
    TMsgsRecievedEventData m_pubsubMsgsRefs;
    TBufferVector m_pubsubMsgAttribs;
    COMCORE::CHostAddress m_redisShardHost;
    CAwsSqsPubSubClientTopicConfig m_config;
    MT::CMutex m_lock;
    Aws::String m_queueUrl;
    PUBSUB::CIPubSubMsg::TIPubSubMsgConstRawPtrVector m_publishBulkMsgRemapStorage;
    CORE::UInt64 m_currentPublishActionId;
    CORE::UInt64 m_currentReceiveActionId;
    TPublishActionIdVector m_publishSuccessActionIds;
    TMsgsPublishedEventData m_publishSuccessActionEventData;
    TPublishActionIdVector m_publishFailureActionIds;
    TMsgsPublishFailureEventData m_publishFailureActionEventData;
};

/*-------------------------------------------------------------------------*/

typedef CAwsSqsPubSubClientTopic::TSharedPtrType    CAwsSqsPubSubClientTopicPtr;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace AWSSQS */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* PUBSUBPLUGIN_AWSSQS_CAWSSQSPUBSUBCLIENTTOPIC_H ? */
