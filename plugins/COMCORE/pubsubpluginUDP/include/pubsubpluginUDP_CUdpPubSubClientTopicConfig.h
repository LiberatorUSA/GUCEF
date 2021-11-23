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

#ifndef PUBSUBPLUGIN_UDP_CUDPPUBSUBCLIENTTOPICCONFIG_H
#define PUBSUBPLUGIN_UDP_CUDPPUBSUBCLIENTTOPICCONFIG_H

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

#ifndef PUBSUBPLUGIN_UDP_MACROS_H
#include "pubsubpluginUDP_macros.h"
#define PUBSUBPLUGIN_UDP_MACROS_H
#endif /* PUBSUBPLUGIN_UDP_MACROS_H ? */

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

/**
 *  Standard pub-sub client config with some custom specifcs added for this
 *  specific pub-sub backend
 */
class PUBSUBPLUGIN_UDP_PLUGIN_PRIVATE_CPP CUdpPubSubClientTopicConfig : public COMCORE::CPubSubClientTopicConfig
{
    public:

    CUdpPubSubClientTopicConfig( void );
    
    CUdpPubSubClientTopicConfig( const COMCORE::CPubSubClientTopicConfig& genericConfig );

    virtual ~CUdpPubSubClientTopicConfig() GUCEF_VIRTUAL_OVERRIDE;

    CUdpPubSubClientTopicConfig& operator=( const COMCORE::CPubSubClientTopicConfig& src );

    CUdpPubSubClientTopicConfig& operator=( const CUdpPubSubClientTopicConfig& src );

    bool LoadCustomConfig( const CORE::CDataNode& config );

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

#endif /* PUBSUBPLUGIN_UDP_CUDPPUBSUBCLIENTTOPICCONFIG_H ? */
