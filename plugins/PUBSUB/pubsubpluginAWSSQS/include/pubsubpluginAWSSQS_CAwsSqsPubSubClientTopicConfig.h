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

#ifndef PUBSUBPLUGIN_AWSSQS_CAWSSQSPUBSUBCLIENTTOPICCONFIG_H
#define PUBSUBPLUGIN_AWSSQS_CAWSSQSPUBSUBCLIENTTOPICCONFIG_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_PUBSUB_CPUBSUBCLIENTTOPICCONFIG_H
#include "gucefPUBSUB_CPubSubClientTopicConfig.h"
#define GUCEF_PUBSUB_CPUBSUBCLIENTTOPICCONFIG_H
#endif /* GUCEF_PUBSUB_CPUBSUBCLIENTTOPICCONFIG_H ? */

#ifndef PUBSUBPLUGIN_AWSSQS_MACROS_H
#include "pubsubpluginAWSSQS_macros.h"
#define PUBSUBPLUGIN_AWSSQS_MACROS_H
#endif /* PUBSUBPLUGIN_AWSSQS_MACROS_H ? */

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
 *  Standard pub-sub client config with some custom specifcs added for this
 *  specific pub-sub backend
 */
class PUBSUBPLUGIN_AWSSQS_PLUGIN_PRIVATE_CPP CAwsSqsPubSubClientTopicConfig : public PUBSUB::CPubSubClientTopicConfig ,
                                                                              public CORE::CTSharedObjCreator< CAwsSqsPubSubClientTopicConfig, MT::CMutex >
{
    public:

    
    CAwsSqsPubSubClientTopicConfig( void );
    
    CAwsSqsPubSubClientTopicConfig( const PUBSUB::CPubSubClientTopicConfig& genericConfig );

    CAwsSqsPubSubClientTopicConfig( const CAwsSqsPubSubClientTopicConfig& src );

    virtual ~CAwsSqsPubSubClientTopicConfig() GUCEF_VIRTUAL_OVERRIDE;

    CAwsSqsPubSubClientTopicConfig& operator=( const PUBSUB::CPubSubClientTopicConfig& src );

    CAwsSqsPubSubClientTopicConfig& operator=( const CAwsSqsPubSubClientTopicConfig& src );

    bool LoadCustomConfig( const CORE::CDataNode& config );

    bool LoadConfig( const PUBSUB::CPubSubClientTopicConfig& src );

    virtual CORE::CICloneable* Clone( void ) const GUCEF_VIRTUAL_OVERRIDE;

    bool topicNameIsQueueName;
    bool tryToUseSendMessageBatch;
    bool addPrefixWhenSendingKvPairs;
    CORE::CAsciiString kvPairPrefixToAddOnSend;
    bool addPrefixWhenSendingMetaDataKvPairs;
    CORE::CAsciiString metaDatakvPairPrefixToAddOnSend;
};

/*-------------------------------------------------------------------------*/

typedef CAwsSqsPubSubClientTopicConfig::TSharedPtrType     CAwsSqsPubSubClientTopicConfigPtr;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace AWSSQS */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/

#endif /* PUBSUBPLUGIN_AWSSQS_CAWSSQSPUBSUBCLIENTTOPICCONFIG_H ? */
