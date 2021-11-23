/*
 *  pubsubpluginUDP: Generic GUCEF COMCORE plugin for providing pubsub approximation via UDP
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

#ifndef PUBSUBPLUGIN_UDP_CUDPPUBSUBCLIENTTOPIC_H
#define PUBSUBPLUGIN_UDP_CUDPPUBSUBCLIENTTOPIC_H

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

#ifndef GUCEF_COMCORE_CPUBSUBCLIENTTOPIC_H
#include "gucefCOMCORE_CPubSubClientTopic.h"
#define GUCEF_COMCORE_CPUBSUBCLIENTTOPIC_H
#endif /* GUCEF_COMCORE_CPUBSUBCLIENTTOPIC_H ? */

#ifndef PUBSUBPLUGIN_UDP_CUDPPUBSUBCLIENTTOPICCONFIG_H
#include "pubsubpluginUDP_CUdpPubSubClientTopicConfig.h"
#define PUBSUBPLUGIN_UDP_CUDPPUBSUBCLIENTTOPICCONFIG_H
#endif /* PUBSUBPLUGIN_UDP_CUDPPUBSUBCLIENTTOPICCONFIG_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUBPLUGIN {
namespace UDP {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CUdpPubSubClient;

class PUBSUBPLUGIN_UDP_PLUGIN_PRIVATE_CPP CUdpPubSubClientTopic : public COMCORE::CPubSubClientTopic
{
    public:

    typedef std::vector< CORE::UInt32 > UInt32Vector;

    CUdpPubSubClientTopic( CUdpPubSubClient* client );

    virtual ~CUdpPubSubClientTopic() GUCEF_VIRTUAL_OVERRIDE;

    virtual COMCORE::CPubSubClient* GetClient( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool InitializeConnectivity( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Disconnect( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsConnected( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsPublishingSupported( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsSubscribingSupported( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Subscribe( void ) GUCEF_VIRTUAL_OVERRIDE;
    
    virtual bool SubscribeStartingAtMsgId( const CORE::CVariant& msgIdBookmark );

    virtual bool SubscribeStartingAtMsgDateTime( const CORE::CDateTime& msgDtBookmark );

    virtual bool SubscribeStartingAtBookmark( const COMCORE::CPubSubBookmark& bookmark ) GUCEF_VIRTUAL_OVERRIDE;

    virtual COMCORE::CPubSubBookmark GetCurrentBookmark( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CORE::CString& GetTopicName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    const CORE::CString& GetMetricFriendlyTopicName( void ) const;

    virtual bool Publish( CORE::UInt64& publishActionId, const COMCORE::CIPubSubMsg& msg, bool notify ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool AcknowledgeReceipt( const COMCORE::CIPubSubMsg& msg ) GUCEF_VIRTUAL_OVERRIDE;
    virtual bool AcknowledgeReceipt( const COMCORE::CPubSubBookmark& bookmark ) GUCEF_VIRTUAL_OVERRIDE;
    bool AcknowledgeReceiptImpl( const COMCORE::CPubSubBookmark& bookmark, CORE::UInt64 receiveActionId );

    virtual bool SaveConfig( COMCORE::CPubSubClientTopicConfig& config ) const;

    virtual bool LoadConfig( const COMCORE::CPubSubClientTopicConfig& config );

    CORE::UInt64 GetMsmqErrorsOnReceiveCounter( bool resetCounter );

    CORE::UInt64 GetMsmqMessagesReceivedCounter( bool resetCounter );

    CORE::UInt64 GetMsmqMessagesPublishedCounter( bool resetCounter );

    CORE::UInt64 GetMsmqErrorsOnPublishCounter( bool resetCounter );

    CORE::UInt64 GetMsmqErrorsOnAckCounter( bool resetCounter );
    
    class TopicMetrics
    {
        public:

        TopicMetrics( void );

        CORE::Int64 msmqMsgsInQueue;

        CORE::UInt64 msmqMessagesPublished;
        CORE::UInt64 msmqErrorsOnPublish;
        CORE::UInt64 msmqErrorsOnAck;

        CORE::UInt64 msmqMessagesReceived;
        CORE::UInt64 msmqErrorsOnReceive;

        UInt32Vector msmqMsgSentToArriveLatencies;
    };

    const TopicMetrics& GetMetrics( void ) const;

    const CUdpPubSubClientTopicConfig& GetTopicConfig( void ) const;

    void
    OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                         const CORE::CEvent& eventId  ,
                         CORE::CICloneable* eventData );
    
    virtual const MT::CILockable* AsLockable( void ) const GUCEF_VIRTUAL_OVERRIDE;

    protected:

    virtual bool Lock( CORE::UInt32 lockWaitTimeoutInMs = GUCEF_MT_DEFAULT_LOCK_TIMEOUT_IN_MS ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Unlock( void ) const GUCEF_VIRTUAL_OVERRIDE;

    private:

    typedef CORE::CTEventHandlerFunctor< CUdpPubSubClientTopic > TEventCallback;

    // Types to implement/hook-up topic interface
    typedef std::vector< COMCORE::CBasicPubSubMsg > TPubSubMsgsVector;    

    private:

    void RegisterEventHandlers( void );

    bool SetupToSubscribe( COMCORE::CPubSubClientTopicConfig& config );

    static CORE::CString GenerateMetricsFriendlyTopicName( const CORE::CString& topicName );

    void
    OnReconnectTimerCycle( CORE::CNotifier* notifier    ,
                           const CORE::CEvent& eventId  ,
                           CORE::CICloneable* eventData );
    
    void
    OnPulseCycle( CORE::CNotifier* notifier    ,
                  const CORE::CEvent& eventId  ,
                  CORE::CICloneable* eventData );
    
    private:

    CUdpPubSubClient* m_client;
    TPubSubMsgsVector m_pubsubMsgs;
    TMsgsRecievedEventData m_pubsubMsgsRefs;
    CUdpPubSubClientTopicConfig m_config;  
    CORE::CTimer* m_reconnectTimer;
    MT::CMutex m_lock;
    CORE::UInt64 m_currentPublishActionId;
    CORE::UInt64 m_currentReceiveActionId;
    TPublishActionIdVector m_publishSuccessActionIds;
    TMsgsPublishedEventData m_publishSuccessActionEventData;
    TPublishActionIdVector m_publishFailureActionIds;
    TMsgsPublishFailureEventData m_publishFailureActionEventData;
    TopicMetrics m_metrics;
    CORE::CString m_metricFriendlyTopicName;

};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace UDP */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/

#endif /* PUBSUBPLUGIN_UDP_CUDPPUBSUBCLIENTTOPIC_H ? */
