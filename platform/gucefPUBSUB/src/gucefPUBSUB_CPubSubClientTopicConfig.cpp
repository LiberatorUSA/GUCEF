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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

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
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CPubSubClientTopicConfig::CPubSubClientTopicConfig( void )
    : CORE::CIConfigurable()
    , isOptional( false )
    , needSubscribeSupport( false )
    , needPublishSupport( false )
    , preferDedicatedConnection( false )
    , topicName()
    , consumerGroupName()
    , consumerName()
    , useTopicLevelMaxTotalMsgsInFlight( false )
    , maxTotalMsgsInFlight( -1 )
    , journalConfig()
    , customConfig()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubClientTopicConfig::CPubSubClientTopicConfig( const CPubSubClientTopicConfig& src )
    : CORE::CIConfigurable( src )
    , isOptional( src.isOptional )
    , needSubscribeSupport( src.needSubscribeSupport )
    , needPublishSupport( src.needPublishSupport )
    , preferDedicatedConnection( src.preferDedicatedConnection )
    , topicName( src.topicName )
    , consumerGroupName( src.consumerGroupName )
    , consumerName( src.consumerName )
    , useTopicLevelMaxTotalMsgsInFlight( src.useTopicLevelMaxTotalMsgsInFlight )
    , maxTotalMsgsInFlight( src.maxTotalMsgsInFlight )
    , journalConfig( src.journalConfig )
    , customConfig( src.customConfig )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubClientTopicConfig::~CPubSubClientTopicConfig()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubClientTopicConfig& 
CPubSubClientTopicConfig::operator=( const CPubSubClientTopicConfig& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        CORE::CIConfigurable::operator=( src );
        isOptional = src.isOptional;
        needSubscribeSupport = src.needSubscribeSupport;
        needPublishSupport = src.needPublishSupport;
        preferDedicatedConnection = src.preferDedicatedConnection;
        topicName = src.topicName;
        consumerGroupName = src.consumerGroupName;
        consumerName = src.consumerName;
        useTopicLevelMaxTotalMsgsInFlight = src.useTopicLevelMaxTotalMsgsInFlight;
        maxTotalMsgsInFlight = src.maxTotalMsgsInFlight;
        journalConfig = src.journalConfig;
        customConfig = src.customConfig;        
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubClientTopicConfig::SaveConfig( CORE::CDataNode& cfg ) const
{GUCEF_TRACE;

    bool totalSuccess = true;

    totalSuccess = cfg.SetAttribute( "isOptional", isOptional ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "needSubscribeSupport", needSubscribeSupport ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "needPublishSupport", needPublishSupport ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "preferDedicatedConnection", preferDedicatedConnection ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "topicName", topicName ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "consumerGroupName", consumerGroupName ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "consumerName", consumerName ) && totalSuccess;        
    totalSuccess = cfg.SetAttribute( "useTopicLevelMaxTotalMsgsInFlight", useTopicLevelMaxTotalMsgsInFlight ) && totalSuccess;        
    totalSuccess = cfg.SetAttribute( "maxTotalMsgsInFlight", maxTotalMsgsInFlight ) && totalSuccess;        
    cfg.CopySubTree( customConfig );   
    
    CORE::CDataNode* journalCfg = cfg.FindOrAddChild( "journalConfig" );
    if ( GUCEF_NULL == journalCfg )
       return false;
    totalSuccess = journalConfig.SaveConfig( *journalCfg ) && totalSuccess;

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubClientTopicConfig::LoadConfig( const CORE::CDataNode& cfg )
{GUCEF_TRACE;

    isOptional = cfg.GetAttributeValueOrChildValueByName( "isOptional" ).AsBool( isOptional, true );
    needSubscribeSupport = cfg.GetAttributeValueOrChildValueByName( "needSubscribeSupport" ).AsBool( needSubscribeSupport, true );
    needPublishSupport = cfg.GetAttributeValueOrChildValueByName( "needPublishSupport" ).AsBool( needPublishSupport, true );
    preferDedicatedConnection = cfg.GetAttributeValueOrChildValueByName( "preferDedicatedConnection" ).AsBool( preferDedicatedConnection, true );
    topicName = cfg.GetAttributeValueOrChildValueByName( "topicName" ).AsString( topicName, true );
    consumerGroupName = cfg.GetAttributeValueOrChildValueByName( "consumerGroupName" ).AsString( consumerGroupName, true );
    consumerName = cfg.GetAttributeValueOrChildValueByName( "consumerName" ).AsString( consumerName, true );
    useTopicLevelMaxTotalMsgsInFlight = cfg.GetAttributeValueOrChildValueByName( "useTopicLevelMaxTotalMsgsInFlight" ).AsBool( useTopicLevelMaxTotalMsgsInFlight, true );
    maxTotalMsgsInFlight = cfg.GetAttributeValueOrChildValueByName( "maxTotalMsgsInFlight" ).AsInt64( maxTotalMsgsInFlight, true );
    
    const CORE::CDataNode* journalConfigNode = cfg.FindChild( "journalConfig" );
    if ( GUCEF_NULL != journalConfigNode )
    {
        if ( !journalConfig.LoadConfig( *journalConfigNode ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientTopicConfig:LoadConfig: failed to load journal config section" );
            return false;
        }
    }

    const CORE::CDataNode* newCustomConfig = cfg.FindChild( "CustomConfig" );
    if ( GUCEF_NULL != newCustomConfig )
    {
        customConfig.Clear();
        customConfig.Copy( *newCustomConfig );
    }

    return true;
}

/*-------------------------------------------------------------------------*/

const CString& 
CPubSubClientTopicConfig::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CString classTypeName = "GUCEF::PUBSUB::CPubSubClientTopicConfig";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

CORE::CICloneable* 
CPubSubClientTopicConfig::Clone( void ) const 
{GUCEF_TRACE;

    return GUCEF_NEW CPubSubClientTopicConfig( *this );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PUBSUB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
