/*
 *  gucefPUBSUB: GUCEF module providing pub-sub communication facilities
 *
 *  Copyright (C) 1998 - 2022.  Dinand Vanvelzen
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

#ifndef GUCEF_PUBSUB_CPUBSUBCLIENT_H
#define GUCEF_PUBSUB_CPUBSUBCLIENT_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

#ifndef GUCEF_CORE_CITYPENAMED_H
#include "CITypeNamed.h"
#define GUCEF_CORE_CITYPENAMED_H
#endif /* GUCEF_CORE_CITYPENAMED_H ? */

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "gucefCORE_CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_COMCORE_CHOSTADDRESS_H
#include "CHostAddress.h"
#define GUCEF_COMCORE_CHOSTADDRESS_H
#endif /* GUCEF_COMCORE_CHOSTADDRESS_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBCLIENTFEATURES_H
#include "gucefPUBSUB_CPubSubClientFeatures.h"    
#define GUCEF_PUBSUB_CPUBSUBCLIENTFEATURES_H
#endif /* GUCEF_PUBSUB_CPUBSUBCLIENTFEATURES_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBCLIENTTOPIC_H
#include "gucefPUBSUB_CPubSubClientTopic.h"    
#define GUCEF_PUBSUB_CPUBSUBCLIENTTOPIC_H
#endif /* GUCEF_PUBSUB_CPUBSUBCLIENTTOPIC_H ? */

#include "gucefPUBSUB_CPubSubClientTopicConfig.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/
 
/**
 *  Base class for all pub-sub client implementations
 */
class GUCEF_PUBSUB_EXPORT_CPP CPubSubClient : public CORE::CObservingNotifier ,
                                              public virtual CORE::CIConfigurable
{
    public:

    static const CORE::CEvent TopicAccessCreatedEvent;                 /**< occurs when topic access is created programatically or via loading of config requesting the same */
    static const CORE::CEvent TopicAccessDestroyedEvent;               /**< occurs when topic access is destroyed programatically */
    static const CORE::CEvent TopicsAccessAutoCreatedEvent;            /**< occurs when topic access is automatically created following discovery. Requires the 'supportsDiscoveryOfAvailableTopics' feature to be supported and enabled */
    static const CORE::CEvent TopicsAccessAutoDestroyedEvent;          /**< occurs when topic access is automatically destroyed following discovery. Requires the 'supportsDiscoveryOfAvailableTopics' feature to be supported and enabled */
    static const CORE::CEvent TopicDiscoveryEvent;                     /**< occurs when available topic changes are detected. Requires the 'supportsDiscoveryOfAvailableTopics' feature to be supported and enabled */
    static const CORE::CEvent HealthStatusChangeEvent;                 /**< event msg sent if the health status changes for the topic */

    typedef std::set< CPubSubClientTopic* >                         PubSubClientTopicSet;
    typedef CORE::TCloneableString                                  TopicAccessCreatedEventData;            /**< name of the topic is passed as event relevant data */
    typedef CORE::TCloneableString                                  TopicAccessDestroyedEventData;          /**< name of the topic is passed as event relevant data */
    typedef CORE::CTCloneableObj< PubSubClientTopicSet >            TopicsAccessAutoCreatedEventData;       /**< access to the topics is passed as event relevant data */
    typedef CORE::CTCloneableObj< PubSubClientTopicSet >            TopicsAccessAutoDestroyedEventData;     /**< access to the topics is passed as event relevant data */
    typedef CORE::CTCloneableExpansion< CORE::CString::StringSet >  TopicDiscoveryEventData;                /**< list of discovered topics */
    typedef CORE::TCloneableBool                                    THealthStatusChangeEventData;           /**< boolean flag indicating the health status */

    CPubSubClient( void );

    CPubSubClient( const CPubSubClient& src );

    virtual ~CPubSubClient();

    CPubSubClient& operator=( const CPubSubClient& src );

    virtual bool GetSupportedFeatures( CPubSubClientFeatures& features ) const = 0;

    virtual CPubSubClientTopic* CreateTopicAccess( const CPubSubClientTopicConfig& topicConfig ) = 0;

    /**
     *  Same as the version that takes an entire config except the expectation here is that the topic      
     *  is already configured via a CPubSubClientConfig but not yet instantiated
     *  This would be the typical case when using global app config defined topics and not programatic topic access
     */ 
    virtual CPubSubClientTopic* CreateTopicAccess( const CString& topicName );

    virtual CPubSubClientTopic* GetTopicAccess( const CString& topicName ) = 0;

    virtual CPubSubClientTopic* GetOrCreateTopicAccess( const CString& topicName );

    virtual bool GetMultiTopicAccess( const CString& topicName          ,
                                      PubSubClientTopicSet& topicAccess );

    virtual bool GetMultiTopicAccess( const CString::StringSet& topicNames ,
                                      PubSubClientTopicSet& topicAccess    );
    
    virtual bool GetOrCreateMultiTopicAccess( const CString& topicName          ,
                                              PubSubClientTopicSet& topicAccess );

    virtual bool CreateMultiTopicAccess( const CPubSubClientTopicConfig& topicConfig ,
                                         PubSubClientTopicSet& topicAccess           );

    /**
     *  Same as the version that takes an entire config except the expectation here is that the topic      
     *  is already configured via a CPubSubClientConfig but not yet instantiated
     *  This would be the typical case when using global app config defined topics and not programatic topic access
     */ 
    virtual bool CreateMultiTopicAccess( const CString& topicName          ,
                                         PubSubClientTopicSet& topicAccess );

    virtual const CPubSubClientTopicConfig* GetTopicConfig( const CString& topicName ) = 0;

    /**
     *  Attempts to get a list of topics available on the backend regardless of whichever topics are known
     *  due to configuration. Thus for some backends this allows one to 'discover' new topics
     */
    virtual bool BeginTopicDiscovery( const CORE::CString::StringSet& globPatternFilters = CORE::CString::EmptyStringSet );
    
    virtual void GetConfiguredTopicNameList( CString::StringSet& topicNameList ) = 0;

    virtual void GetCreatedTopicAccessNameList( CString::StringSet& topicNameList ) = 0;

    virtual void DestroyTopicAccess( const CString& topicName ) = 0;

    virtual bool Connect( void ) = 0;

    virtual bool Disconnect( void ) = 0;

    virtual bool IsConnected( void ) const = 0;

    virtual bool IsHealthy( void ) const = 0;

    /**
     *  Provide ability to get the textual name of the formatter
     */
    virtual const CString& GetType( void ) const = 0;

    virtual const CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    void SetOpaqueUserData( void* opaqueUserData );
    
    void* GetOpaqueUserData( void ) const;

    private:

    static void RegisterEvents( void );
    
    private:

    void* m_opaqueUserData;
};

/*-------------------------------------------------------------------------*/

typedef CORE::CTBasicSharedPtr< CPubSubClient, MT::CMutex > CPubSubClientPtr;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PUBSUB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PUBSUB_CPUBSUBCLIENT_H ? */

