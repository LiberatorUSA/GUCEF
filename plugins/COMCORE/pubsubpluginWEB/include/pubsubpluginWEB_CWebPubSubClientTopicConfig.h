/*
 *  pubsubpluginWEB: Generic GUCEF COMCORE plugin for providing pubsub approximation via the WEB
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

#ifndef PUBSUBPLUGIN_WEB_CWEBPUBSUBCLIENTTOPICCONFIG_H
#define PUBSUBPLUGIN_WEB_CWEBPUBSUBCLIENTTOPICCONFIG_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <vector>

#ifndef GUCEF_COMCORE_CPUBSUBCLIENTTOPICCONFIG_H
#include "gucefCOMCORE_CPubSubClientTopicConfig.h"
#define GUCEF_COMCORE_CPUBSUBCLIENTTOPICCONFIG_H
#endif /* GUCEF_COMCORE_CPUBSUBCLIENTTOPICCONFIG_H ? */

#ifndef PUBSUBPLUGIN_WEB_MACROS_H
#include "pubsubpluginWEB_macros.h"
#define PUBSUBPLUGIN_WEB_MACROS_H
#endif /* PUBSUBPLUGIN_WEB_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUBPLUGIN {
namespace WEB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CWebPubSubClient;

/**
 *  Standard pub-sub client config with some custom specifcs added for this
 *  specific pub-sub backend
 */
class PUBSUBPLUGIN_WEB_PLUGIN_PRIVATE_CPP CWebPubSubClientTopicConfig : public COMCORE::CPubSubClientTopicConfig
{
    public:

    CWebPubSubClientTopicConfig( void );
    
    CWebPubSubClientTopicConfig( const COMCORE::CPubSubClientTopicConfig& genericConfig );

    virtual ~CWebPubSubClientTopicConfig() GUCEF_VIRTUAL_OVERRIDE;

    CWebPubSubClientTopicConfig& operator=( const COMCORE::CPubSubClientTopicConfig& src );

    CWebPubSubClientTopicConfig& operator=( const CWebPubSubClientTopicConfig& src );

    bool LoadCustomConfig( const CORE::CDataNode& config );

    CORE::UInt16 httpServerPort;
    bool exposeBasicHealthEndpoint;
    CORE::CString basicHealthEndpointPath;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace WEB */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/

#endif /* PUBSUBPLUGIN_WEB_CWEBPUBSUBCLIENTTOPICCONFIG_H ? */
