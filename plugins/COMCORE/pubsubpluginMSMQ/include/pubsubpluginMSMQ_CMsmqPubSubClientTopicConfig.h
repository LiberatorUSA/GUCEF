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

#ifndef PUBSUBPLUGIN_MSMQ_CMSMQPUBSUBCLIENTTOPICCONFIG_H
#define PUBSUBPLUGIN_MSMQ_CMSMQPUBSUBCLIENTTOPICCONFIG_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_COMCORE_CPUBSUBCLIENTTOPICCONFIG_H
#include "gucefCOMCORE_CPubSubClientTopicConfig.h"
#define GUCEF_COMCORE_CPUBSUBCLIENTTOPICCONFIG_H
#endif /* GUCEF_COMCORE_CPUBSUBCLIENTTOPICCONFIG_H ? */

#ifndef PUBSUBPLUGIN_MSMQ_MACROS_H
#include "pubsubpluginMSMQ_macros.h"
#define PUBSUBPLUGIN_MSMQ_MACROS_H
#endif /* PUBSUBPLUGIN_MSMQ_MACROS_H ? */

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
 *  Standard pub-sub client config with some custom specifcs added for this
 *  specific pub-sub backend
 */
class PUBSUBPLUGIN_MSMQ_PLUGIN_PRIVATE_CPP CMsmqPubSubClientTopicConfig : public COMCORE::CPubSubClientTopicConfig
{
    public:

    
    CMsmqPubSubClientTopicConfig( void );
    
    CMsmqPubSubClientTopicConfig( const COMCORE::CPubSubClientTopicConfig& genericConfig );

    virtual ~CMsmqPubSubClientTopicConfig() GUCEF_VIRTUAL_OVERRIDE;

    CMsmqPubSubClientTopicConfig& operator=( const COMCORE::CPubSubClientTopicConfig& src );

    CMsmqPubSubClientTopicConfig& operator=( const CMsmqPubSubClientTopicConfig& src );

    bool LoadCustomConfig( const CORE::CDataNode& config );
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

#endif /* PUBSUBPLUGIN_MSMQ_CMSMQPUBSUBCLIENTTOPICCONFIG_H ? */
