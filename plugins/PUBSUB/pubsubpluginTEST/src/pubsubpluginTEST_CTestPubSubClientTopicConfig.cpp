/*
 *  pubsubpluginTEST: Generic GUCEF PUBSUB plugin for adding integration/system tests
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

#include "pubsubpluginTEST_CTestPubSubClientTopicConfig.h"

#ifndef GUCEF_CORE_METRICSMACROS_H
#include "gucefCORE_MetricsMacros.h"
#define GUCEF_CORE_METRICSMACROS_H
#endif /* GUCEF_CORE_METRICSMACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUBPLUGIN {
namespace TEST {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CTestPubSubClientTopicConfig::CTestPubSubClientTopicConfig( void )
    : STORAGE::CStoragePubSubClientTopicConfig()
    , defaultIsHealthyStatus( true )
    , toggleHealthyStatus( false )
    , healthyStatusToggleIntervalInMs( 5000 )
    , healthyStatusToggleIntervalJitter( 100 )
{GUCEF_TRACE;
    
}

/*-------------------------------------------------------------------------*/

CTestPubSubClientTopicConfig::CTestPubSubClientTopicConfig( const CTestPubSubClientTopicConfig& src )
    : STORAGE::CStoragePubSubClientTopicConfig( src )
    , defaultIsHealthyStatus( src.defaultIsHealthyStatus )
    , toggleHealthyStatus( src.toggleHealthyStatus )
    , healthyStatusToggleIntervalInMs( src.healthyStatusToggleIntervalInMs )
    , healthyStatusToggleIntervalJitter( src.healthyStatusToggleIntervalJitter )
{GUCEF_TRACE;
    
    customConfig = src.customConfig;  
}

/*-------------------------------------------------------------------------*/

CTestPubSubClientTopicConfig::CTestPubSubClientTopicConfig( const PUBSUB::CPubSubClientTopicConfig& genericConfig )
    : STORAGE::CStoragePubSubClientTopicConfig( genericConfig )
    , defaultIsHealthyStatus( true )
    , toggleHealthyStatus( false )
    , healthyStatusToggleIntervalInMs( 5000 )
    , healthyStatusToggleIntervalJitter( 100 )
{GUCEF_TRACE;
    
    LoadCustomConfig( genericConfig.customConfig );  
}

/*-------------------------------------------------------------------------*/

CTestPubSubClientTopicConfig::~CTestPubSubClientTopicConfig()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CTestPubSubClientTopicConfig::LoadCustomConfig( const CORE::CDataNode& config )
{GUCEF_TRACE;
    
    bool success = STORAGE::CStoragePubSubClientTopicConfig::LoadCustomConfig( config );
    
    defaultIsHealthyStatus = config.GetAttributeValueOrChildValueByName( "defaultIsHealthyStatus" ).AsBool( defaultIsHealthyStatus, true );
    toggleHealthyStatus  = config.GetAttributeValueOrChildValueByName( "toggleHealthyStatus" ).AsBool( toggleHealthyStatus, true );
    healthyStatusToggleIntervalInMs = config.GetAttributeValueOrChildValueByName( "healthyStatusToggleIntervalInMs" ).AsUInt32( healthyStatusToggleIntervalInMs, true );
    healthyStatusToggleIntervalJitter = config.GetAttributeValueOrChildValueByName( "healthyStatusToggleIntervalJitter" ).AsUInt32( healthyStatusToggleIntervalJitter, true );

    return success;
}

/*-------------------------------------------------------------------------*/

bool
CTestPubSubClientTopicConfig::SaveCustomConfig( CORE::CDataNode& config ) const
{GUCEF_TRACE;

    bool success = STORAGE::CStoragePubSubClientTopicConfig::SaveCustomConfig( config );

    success = config.SetAttribute( "defaultIsHealthyStatus", defaultIsHealthyStatus ) && success;            
    success = config.SetAttribute( "toggleHealthyStatus", toggleHealthyStatus ) && success;            
    success = config.SetAttribute( "healthyStatusToggleIntervalInMs", healthyStatusToggleIntervalInMs ) && success;            
    success = config.SetAttribute( "healthyStatusToggleIntervalJitter", healthyStatusToggleIntervalJitter ) && success;            
        
    return success;
}

/*-------------------------------------------------------------------------*/

CTestPubSubClientTopicConfig& 
CTestPubSubClientTopicConfig::operator=( const PUBSUB::CPubSubClientTopicConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        STORAGE::CStoragePubSubClientTopicConfig::operator=( src );
        LoadCustomConfig( src.customConfig );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CTestPubSubClientTopicConfig& 
CTestPubSubClientTopicConfig::operator=( const CTestPubSubClientTopicConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        STORAGE::CStoragePubSubClientTopicConfig::operator=( src );

        defaultIsHealthyStatus = src.defaultIsHealthyStatus;
        toggleHealthyStatus = src.toggleHealthyStatus;
        healthyStatusToggleIntervalInMs = src.healthyStatusToggleIntervalInMs;
        healthyStatusToggleIntervalJitter = src.healthyStatusToggleIntervalJitter;

        customConfig = src.customConfig;
    }
    return *this;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace TEST */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
