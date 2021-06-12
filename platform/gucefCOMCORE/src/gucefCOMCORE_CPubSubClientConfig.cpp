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

#include "gucefCOMCORE_CPubSubClientConfig.h"

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

CPubSubClientConfig::CPubSubClientConfig( void )
    : CORE::CIConfigurable()
    , pubsubClientType()
    , desiredFeatures()
    , customConfig()
    , pulseGenerator( GUCEF_NULL )
    , reconnectDelayInMs( 0 )
    , remoteAddress()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubClientConfig::CPubSubClientConfig( const CPubSubClientConfig& src )
    : CORE::CIConfigurable()
    , pubsubClientType( src.pubsubClientType )
    , desiredFeatures( src.desiredFeatures )
    , customConfig( src.customConfig )
    , pulseGenerator( src.pulseGenerator )
    , reconnectDelayInMs( src.reconnectDelayInMs )
    , remoteAddress( src.remoteAddress )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubClientConfig::~CPubSubClientConfig()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool 
CPubSubClientConfig::SaveConfig( CORE::CDataNode& tree ) const
{GUCEF_TRACE;
    
    tree.SetAttribute( "pubsubClientType", pubsubClientType );
    tree.SetAttribute( "reconnectDelayInMs", reconnectDelayInMs );
    tree.SetAttribute( "remoteAddress", remoteAddress.HostnameAndPortAsString() );
   
    tree.CopySubTree( customConfig );    

    CORE::CDataNode* desiredFeaturesCfg = tree.FindChild( "DesiredFeatures" );
    if ( GUCEF_NULL == desiredFeaturesCfg )
    {
        desiredFeaturesCfg = tree.AddChild( "DesiredFeatures" );
    }
    desiredFeatures.SaveConfig( *desiredFeaturesCfg );

    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubClientConfig::LoadConfig( const CORE::CDataNode& cfg )
{GUCEF_TRACE;

    pubsubClientType = CORE::ResolveVars( cfg.GetAttributeValueOrChildValueByName( "pubsubClientType", pubsubClientType ) );
    reconnectDelayInMs = CORE::StringToUInt32( CORE::ResolveVars( cfg.GetAttributeValueOrChildValueByName( "reconnectDelayInMs" ) ), reconnectDelayInMs );
    remoteAddress.SetHostnameAndPort( CORE::ResolveVars( cfg.GetAttributeValueOrChildValueByName( "remoteAddress", remoteAddress.HostnameAndPortAsString() ) ) );
    
    const CORE::CDataNode* newCustomConfig = cfg.FindChild( "CustomConfig" );
    if ( GUCEF_NULL != newCustomConfig )
    {
        customConfig.Clear();
        customConfig.Copy( *newCustomConfig );
    }

    const CORE::CDataNode* newDesiredFeatures = cfg.FindChild( "DesiredFeatures" );
    if ( GUCEF_NULL != newDesiredFeatures )
    {
        if ( !desiredFeatures.LoadConfig( *newDesiredFeatures ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientConfig:LoadConfig: failed to load DesiredFeatures section" );
            return false;
        }
    }

    const CORE::CDataNode* topicCollectionNode = cfg.FindChild( "Topics" );
    if ( GUCEF_NULL != topicCollectionNode )
    {
        topics.clear();

        CORE::CDataNode::TConstDataNodeSet psClientTopicConfigs = topicCollectionNode->FindChildrenOfType( "PubSubClientTopicConfig", true );
        if ( !psClientTopicConfigs.empty() )
        {
            CORE::UInt32 n=0;
            topics.resize( psClientTopicConfigs.size() );
        
            CORE::CDataNode::TConstDataNodeSet::iterator i = psClientTopicConfigs.begin();
            while ( i != psClientTopicConfigs.end() )
            {
                COMCORE::CPubSubClientTopicConfig& topicConfig = topics[ n ];            
                if ( !topicConfig.LoadConfig( *(*i) ) )
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubClientConfig:LoadConfig: failed to load PubSubClientTopicConfig section" );
                    return false;
                }
                ++i; ++n;            
            }
        }
    }

    return true;
}

/*-------------------------------------------------------------------------*/

const CString& 
CPubSubClientConfig::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CString classTypeName = "GUCEF::COMCORE::CPubSubClientConfig";
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
