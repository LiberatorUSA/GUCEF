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

#ifndef PUBSUBPLUGIN_UDP_CUDPPUBSUBCLIENTCONFIG_H
#define PUBSUBPLUGIN_UDP_CUDPPUBSUBCLIENTCONFIG_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_COMCORE_CPUBSUBCLIENTCONFIG_H
#include "gucefCOMCORE_CPubSubClientConfig.h"
#define GUCEF_COMCORE_CPUBSUBCLIENTCONFIG_H
#endif /* GUCEF_COMCORE_CPUBSUBCLIENTCONFIG_H ? */

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
class PUBSUBPLUGIN_UDP_PLUGIN_PRIVATE_CPP CUdpPubSubClientConfig : public COMCORE::CPubSubClientConfig
{
    public:

    bool simulateReceiveAckFeatureViaLookupId;
    
    CUdpPubSubClientConfig( void );
    
    CUdpPubSubClientConfig( const COMCORE::CPubSubClientConfig& genericConfig );

    virtual ~CUdpPubSubClientConfig() GUCEF_VIRTUAL_OVERRIDE;

    CUdpPubSubClientConfig& operator=( const COMCORE::CPubSubClientConfig& src );

    CUdpPubSubClientConfig& operator=( const CUdpPubSubClientConfig& src );

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

#endif /* PUBSUBPLUGIN_UDP_CMSMQPUBSUBCLIENTCONFIG_H ? */
