/*
 *  pubsubpluginKAFKA: Generic GUCEF COMCORE plugin for providing pubsub via Kafka
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_METRICSMACROS_H
#include "gucefCORE_MetricsMacros.h"
#define GUCEF_CORE_METRICSMACROS_H
#endif /* GUCEF_CORE_METRICSMACROS_H ? */

#include "pubsubpluginMSMQ_CMsmqPubSubClientConfig.h"

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
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CMsmqPubSubClientConfig::CMsmqPubSubClientConfig( void )
    : COMCORE::CPubSubClientConfig()
    , simulateReceiveAckFeatureViaLookupId( true )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CMsmqPubSubClientConfig::CMsmqPubSubClientConfig( const COMCORE::CPubSubClientConfig& genericConfig )
    : COMCORE::CPubSubClientConfig( genericConfig )
    , simulateReceiveAckFeatureViaLookupId( true )
{GUCEF_TRACE;

    LoadCustomConfig( genericConfig.customConfig );  
}

/*-------------------------------------------------------------------------*/

CMsmqPubSubClientConfig::~CMsmqPubSubClientConfig()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClientConfig::LoadCustomConfig( const CORE::CDataNode& config )
{GUCEF_TRACE;
    
    simulateReceiveAckFeatureViaLookupId = config.GetAttributeValueOrChildValueByName( "simulateReceiveAckFeatureViaLookupId" ).AsBool( simulateReceiveAckFeatureViaLookupId, true );    

    return true;
}

/*-------------------------------------------------------------------------*/

CMsmqPubSubClientConfig& 
CMsmqPubSubClientConfig::operator=( const COMCORE::CPubSubClientConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        COMCORE::CPubSubClientConfig::operator=( src );
        LoadCustomConfig( src.customConfig );    
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CMsmqPubSubClientConfig& 
CMsmqPubSubClientConfig::operator=( const CMsmqPubSubClientConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        COMCORE::CPubSubClientConfig::operator=( src );
        simulateReceiveAckFeatureViaLookupId = src.simulateReceiveAckFeatureViaLookupId;
    }
    return *this;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace KAFKA */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
