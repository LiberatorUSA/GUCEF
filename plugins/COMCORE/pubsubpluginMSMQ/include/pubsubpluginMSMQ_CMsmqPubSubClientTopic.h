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

#ifndef PUBSUBPLUGIN_MSMQ_CMSMQPUBSUBCLIENTTOPIC_H
#define PUBSUBPLUGIN_MSMQ_CMSMQPUBSUBCLIENTTOPIC_H

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

#ifndef PUBSUBPLUGIN_MSMQ_CMSMQPUBSUBCLIENTTOPICCONFIG_H
#include "pubsubpluginMSMQ_CMsmqPubSubClientTopicConfig.h"
#define PUBSUBPLUGIN_MSMQ_CMSMQPUBSUBCLIENTTOPICCONFIG_H
#endif /* PUBSUBPLUGIN_MSMQ_CMSMQPUBSUBCLIENTTOPICCONFIG_H ? */

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
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CMsmqPubSubClient;

/**
 *  MS Windows MSMQ implementation of the conceptual pub-sub "topic"
 */
class PUBSUBPLUGIN_MSMQ_PLUGIN_PRIVATE_CPP CMsmqPubSubClientTopic : public COMCORE::CPubSubClientTopic
{
    public:

    CMsmqPubSubClientTopic( CMsmqPubSubClient* client );

    virtual ~CMsmqPubSubClientTopic() GUCEF_VIRTUAL_OVERRIDE;

    virtual bool InitializeConnectivity( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Disconnect( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsConnected( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsPublishingSupported( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsSubscribingSupported( void ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Subscribe( void ) GUCEF_VIRTUAL_OVERRIDE;
    
    virtual bool SubscribeStartingAtMsgId( const CORE::CVariant& msgIdBookmark ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool SubscribeStartingAtMsgDateTime( const CORE::CDateTime& msgDtBookmark ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CORE::CString& GetTopicName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Publish( const COMCORE::CIPubSubMsg& msg ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool SaveConfig( COMCORE::CPubSubClientTopicConfig& config ) const;

    virtual bool LoadConfig( const COMCORE::CPubSubClientTopicConfig& config );

    void
    OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                         const CORE::CEvent& eventId  ,
                         CORE::CICloneable* eventData );
    
    virtual const MT::CILockable* AsLockable( void ) const GUCEF_VIRTUAL_OVERRIDE;

    protected:

    virtual bool Lock( CORE::UInt32 lockWaitTimeoutInMs = GUCEF_MT_DEFAULT_LOCK_TIMEOUT_IN_MS ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Unlock( void ) const GUCEF_VIRTUAL_OVERRIDE;

    private:

    void RegisterEventHandlers( void );

    void PrepStorageForReadMsgs( CORE::UInt32 msgCount );
    void PrepMsmqMsgStorage( void );

    void PrepMsmqVariantStorageForProperty( PROPID propertyId, MQPROPVARIANT& msmqVariant );

    static VARTYPE GetMsmqVariantTypeForMsmqProperty( PROPID propertyId );

    bool SetupToSubscribe( COMCORE::CPubSubClientTopicConfig& config );

    void
    OnSyncReadTimerCycle( CORE::CNotifier* notifier    ,
                          const CORE::CEvent& eventId  ,
                          CORE::CICloneable* eventData );

    void OnMsmqMsgReceived( const MQMSGPROPS& msg, CORE::UInt32 msgCycleIndex, bool linkIfPossible );

    CORE::Int64 GetCurrentNrOfMessagesInQueue( void ) const;

    static bool MsmqPropertyToVariant( MQPROPVARIANT& msmqSourceVariant, CORE::CVariant& targetVariant, bool linkIfPossible, CORE::UInt32 lengthIfApplicable = 0 );
    
    private:

    typedef CORE::CTEventHandlerFunctor< CMsmqPubSubClientTopic > TEventCallback;

    // Types to implement/hook-up topic interface
    typedef std::vector< COMCORE::CBasicPubSubMsg > TPubSubMsgsVector;
    typedef std::pair< CORE::CDynamicBuffer, CORE::CDynamicBuffer > TBufferPair;
    typedef std::vector< TBufferPair > TBufferVector;
    
    // Types used as caches to prevent ongoing memory allocations
    typedef std::vector< MQPROPVARIANT >    MQPROPVARIANTVector;
    typedef std::vector< MSGPROPID >        MSGPROPIDVector;
    typedef std::vector< PROPVARIANT >      PROPVARIANTVector;

    struct SMsmqMsg
    {
        MQMSGPROPS msgprops;
        MSGPROPIDVector aMsgPropId;
        PROPVARIANTVector aMsgPropVar;
    };
    typedef struct SMsmqMsg TMsmqMsg;
    typedef std::vector< TMsmqMsg >         MsmqMsgVector;

    CMsmqPubSubClient* m_client;
    TPubSubMsgsVector m_pubsubMsgs;
    TMsgsRecievedEventData m_pubsubMsgsRefs;
    TBufferVector m_pubsubMsgAttribs;
    MsmqMsgVector m_msmqReceiveMsgs;
    MQPROPVARIANTVector m_msgSendProps;
    CMsmqPubSubClientTopicConfig m_config;
    CORE::CTimer m_syncReadTimer;
    CORE::Int64 m_msmqMsgsQueued;
    MT::CMutex m_lock;
    QUEUEHANDLE m_receiveQueueHandle;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MSMQ */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/

#endif /* PUBSUBPLUGIN_MSMQ_CMSMQPUBSUBCLIENTTOPIC_H ? */
