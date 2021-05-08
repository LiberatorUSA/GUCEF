/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
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

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#include "gucefCOMCORE_CPubSubClientTopicConfig.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

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
        customConfig = src.customConfig;        
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubClientTopicConfig::SaveConfig( CORE::CDataNode& tree ) const
{GUCEF_TRACE;

    tree.SetAttribute( "isOptional", isOptional );
    tree.SetAttribute( "needSubscribeSupport", needSubscribeSupport );
    tree.SetAttribute( "needPublishSupport", needPublishSupport );
    tree.SetAttribute( "preferDedicatedConnection", preferDedicatedConnection );
    tree.SetAttribute( "topicName", topicName );
    tree.SetAttribute( "consumerGroupName", consumerGroupName );
    tree.SetAttribute( "consumerName", consumerName );    
    tree.CopySubTree( customConfig );    
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubClientTopicConfig::LoadConfig( const CORE::CDataNode& cfg )
{GUCEF_TRACE;

    isOptional = CORE::StringToBool( CORE::ResolveVars( cfg.GetAttributeValueOrChildValueByName( "isOptional" ) ), isOptional );
    needSubscribeSupport = CORE::StringToBool( CORE::ResolveVars( cfg.GetAttributeValueOrChildValueByName( "needSubscribeSupport" ) ), needSubscribeSupport );
    needPublishSupport = CORE::StringToBool( CORE::ResolveVars( cfg.GetAttributeValueOrChildValueByName( "needPublishSupport" ) ), needPublishSupport );
    preferDedicatedConnection = CORE::StringToBool( CORE::ResolveVars( cfg.GetAttributeValueOrChildValueByName( "preferDedicatedConnection" ) ), preferDedicatedConnection );
    topicName = CORE::ResolveVars( cfg.GetAttributeValueOrChildValueByName( "topicName", topicName ) );
    consumerGroupName = CORE::ResolveVars( cfg.GetAttributeValueOrChildValueByName( "consumerGroupName", consumerGroupName ) );
    consumerName = CORE::ResolveVars( cfg.GetAttributeValueOrChildValueByName( "consumerName", consumerName ) );
    
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

    static const CString classTypeName = "GUCEF::COMCORE::CPubSubClientTopicConfig";
    return classTypeName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
