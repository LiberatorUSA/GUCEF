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
    : PUBSUB::CPubSubClientConfig()
    , simulateReceiveAckFeatureViaLookupId( true )
    , gatherMsmqMetricForTotalBytesAllQueues( false )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CMsmqPubSubClientConfig::CMsmqPubSubClientConfig( const PUBSUB::CPubSubClientConfig& genericConfig )
    : PUBSUB::CPubSubClientConfig( genericConfig )
    , simulateReceiveAckFeatureViaLookupId( true )
    , gatherMsmqMetricForTotalBytesAllQueues( false )
{GUCEF_TRACE;

    LoadCustomConfig( genericConfig.customConfig );  
}

/*-------------------------------------------------------------------------*/

CMsmqPubSubClientConfig::~CMsmqPubSubClientConfig()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClientConfig::SaveCustomConfig( CORE::CDataNode& config ) const
{GUCEF_TRACE;
    
    // @TODO
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClientConfig::LoadCustomConfig( const CORE::CDataNode& config )
{GUCEF_TRACE;
    
    simulateReceiveAckFeatureViaLookupId = config.GetAttributeValueOrChildValueByName( "simulateReceiveAckFeatureViaLookupId" ).AsBool( simulateReceiveAckFeatureViaLookupId, true );    
    gatherMsmqMetricForTotalBytesAllQueues = config.GetAttributeValueOrChildValueByName( "gatherMsmqMetricForTotalBytesAllQueues" ).AsBool( gatherMsmqMetricForTotalBytesAllQueues, true );    

    return true;
}

/*-------------------------------------------------------------------------*/

CMsmqPubSubClientConfig& 
CMsmqPubSubClientConfig::operator=( const PUBSUB::CPubSubClientConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        PUBSUB::CPubSubClientConfig::operator=( src );
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
        PUBSUB::CPubSubClientConfig::operator=( src );
        simulateReceiveAckFeatureViaLookupId = src.simulateReceiveAckFeatureViaLookupId;
        gatherMsmqMetricForTotalBytesAllQueues = src.gatherMsmqMetricForTotalBytesAllQueues;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool 
CMsmqPubSubClientConfig::SerializeCustomConfigToGenericConfig( void )
{GUCEF_TRACE;

    return SaveCustomConfig( customConfig ); 
}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClientConfig::LoadConfig( const CORE::CDataNode& config )
{GUCEF_TRACE;

    if ( PUBSUB::CPubSubClientConfig::LoadConfig( config ) )
    {
        return LoadCustomConfig( customConfig ); 
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClientConfig::SaveConfig( CORE::CDataNode& config ) const
{GUCEF_TRACE;

    PUBSUB::CPubSubClientConfig cfgCopy( *this );
    if ( SaveCustomConfig( cfgCopy.customConfig ) )
    {
        // Now that the custom config is re-integrated we can just use the base class to save the config
        return cfgCopy.SaveConfig( config ); 
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClientConfig::LoadConfig( const PUBSUB::CPubSubClientConfig& cfg )
{GUCEF_TRACE;

    // For the base class portion there is nothing to parse as its a simple
    // assignment. Its the custom config where the actual parsed loading occurs
    if ( &cfg != this )
    {
        PUBSUB::CPubSubClientConfig::operator=( cfg );
    }
    return LoadCustomConfig( cfg.customConfig ); 
}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClientConfig::SaveConfig( PUBSUB::CPubSubClientConfig& cfg ) const
{GUCEF_TRACE;

    // For the base class portion there is nothing to do except a simple
    // assignment. Its the custom config where the actual config serialization occurs
    cfg = *this;
    return SaveCustomConfig( cfg.customConfig );
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
