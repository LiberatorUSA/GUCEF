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

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#include "gucefPUBSUB_CPubSubFlowRouteConfig.h"

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

CPubSubFlowRouteTopicConfig::CPubSubFlowRouteTopicConfig( void )
    : CORE::CIConfigurable() 
    , fromSideTopicName()
    , toSideTopicName()
    , failoverSideTopicName()
    , spilloverSideTopicName()
    , deadLetterSideTopicName()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubFlowRouteTopicConfig::CPubSubFlowRouteTopicConfig( const CPubSubFlowRouteTopicConfig& src )
    : CORE::CIConfigurable( src ) 
    , fromSideTopicName( src.fromSideTopicName )
    , toSideTopicName( src.toSideTopicName )
    , failoverSideTopicName( src.failoverSideTopicName )
    , spilloverSideTopicName( src.spilloverSideTopicName )
    , deadLetterSideTopicName( src.deadLetterSideTopicName )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubFlowRouteTopicConfig::~CPubSubFlowRouteTopicConfig()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubFlowRouteTopicConfig& 
CPubSubFlowRouteTopicConfig::operator=( const CPubSubFlowRouteTopicConfig& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        CORE::CIConfigurable::operator=( src ) ;
        fromSideTopicName = src.fromSideTopicName;
        toSideTopicName = src.toSideTopicName;
        failoverSideTopicName = src.failoverSideTopicName;
        spilloverSideTopicName = src.spilloverSideTopicName;
        deadLetterSideTopicName = src.deadLetterSideTopicName;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubFlowRouteTopicConfig::SaveConfig( CORE::CDataNode& cfg ) const
{GUCEF_TRACE;

    bool totalSuccess = true;

    totalSuccess = cfg.SetAttribute( "fromSideTopicName", fromSideTopicName ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "toSideTopicName", toSideTopicName ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "failoverSideTopicName", failoverSideTopicName ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "spilloverSideTopicName", spilloverSideTopicName ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "deadLetterSideTopicName", deadLetterSideTopicName ) && totalSuccess;

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubFlowRouteTopicConfig::LoadConfig( const CORE::CDataNode& cfg )
{GUCEF_TRACE;

    bool totalSuccess = true;

    fromSideTopicName = cfg.GetAttributeValueOrChildValueByName( "fromSideTopicName" ).AsString( fromSideTopicName, true ); 
    toSideTopicName = cfg.GetAttributeValueOrChildValueByName( "toSideTopicName" ).AsString( toSideTopicName, true ); 
    failoverSideTopicName = cfg.GetAttributeValueOrChildValueByName( "failoverSideTopicName" ).AsString( failoverSideTopicName, true ); 
    spilloverSideTopicName = cfg.GetAttributeValueOrChildValueByName( "spilloverSideTopicName" ).AsString( spilloverSideTopicName, true ); 
    deadLetterSideTopicName = cfg.GetAttributeValueOrChildValueByName( "deadLetterSideTopicName" ).AsString( deadLetterSideTopicName, true ); 

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

const CString& 
CPubSubFlowRouteTopicConfig::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CORE::CString classTypeName = "GUCEF::PUBSUB::CPubSubFlowRouteTopicConfig";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

CPubSubFlowRouteConfig::CPubSubFlowRouteConfig( void )
    : CORE::CIConfigurable() 
    , fromSideId()
    , toSideId()
    , failoverSideId()
    , spilloverBufferSideId()
    , deadLetterSideId()
    , topicAssociations()
    , toSideTopicsAutoMatchFromSide( false )
    , failoverSideTopicsAutoMatchFromSide( false )
    , spilloverSideTopicsAutoMatchFromSide( false )
    , deadLetterSideTopicsAutoMatchFromSide( false )
    , preferFromTopicThreadForDestination( false )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubFlowRouteConfig::CPubSubFlowRouteConfig( const CPubSubFlowRouteConfig& src )
    : CORE::CIConfigurable( src ) 
    , fromSideId( src.fromSideId )
    , toSideId( src.toSideId )
    , failoverSideId( src.failoverSideId )
    , spilloverBufferSideId( src.spilloverBufferSideId )
    , deadLetterSideId( src.deadLetterSideId )
    , topicAssociations( src.topicAssociations )
    , toSideTopicsAutoMatchFromSide( src.toSideTopicsAutoMatchFromSide )
    , failoverSideTopicsAutoMatchFromSide( src.failoverSideTopicsAutoMatchFromSide )
    , spilloverSideTopicsAutoMatchFromSide( src.spilloverSideTopicsAutoMatchFromSide )
    , deadLetterSideTopicsAutoMatchFromSide( src.deadLetterSideTopicsAutoMatchFromSide )
    , preferFromTopicThreadForDestination( src.preferFromTopicThreadForDestination )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubFlowRouteConfig::~CPubSubFlowRouteConfig()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubFlowRouteConfig& 
CPubSubFlowRouteConfig::operator=( const CPubSubFlowRouteConfig& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        CORE::CIConfigurable::operator=( src ) ;
        fromSideId = src.fromSideId;
        toSideId = src.toSideId;
        failoverSideId = src.failoverSideId;
        spilloverBufferSideId = src.spilloverBufferSideId;
        deadLetterSideId = src.deadLetterSideId;
        topicAssociations = src.topicAssociations;
        toSideTopicsAutoMatchFromSide = src.toSideTopicsAutoMatchFromSide;
        failoverSideTopicsAutoMatchFromSide = src.failoverSideTopicsAutoMatchFromSide;
        spilloverSideTopicsAutoMatchFromSide = src.spilloverSideTopicsAutoMatchFromSide;
        deadLetterSideTopicsAutoMatchFromSide = src.deadLetterSideTopicsAutoMatchFromSide;
        preferFromTopicThreadForDestination = src.preferFromTopicThreadForDestination;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubFlowRouteConfig::SaveConfig( CORE::CDataNode& cfg ) const
{GUCEF_TRACE;

    bool totalSuccess = true;
    totalSuccess = cfg.SetAttribute( "fromSideId", fromSideId ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "toSideId", toSideId ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "failoverSideId", failoverSideId ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "spilloverBufferSideId", spilloverBufferSideId ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "deadLetterSideId", deadLetterSideId ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "toSideTopicsAutoMatchFromSide", toSideTopicsAutoMatchFromSide ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "failoverSideTopicsAutoMatchFromSide", failoverSideTopicsAutoMatchFromSide ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "spilloverSideTopicsAutoMatchFromSide", spilloverSideTopicsAutoMatchFromSide ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "deadLetterSideTopicsAutoMatchFromSide", deadLetterSideTopicsAutoMatchFromSide ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "preferFromTopicThreadForDestination", preferFromTopicThreadForDestination ) && totalSuccess; 

    CORE::CDataNode* topicAssociationsNode = cfg.FindOrAddChild( "topicAssociations" );
    if ( GUCEF_NULL != topicAssociationsNode )
    {
        topicAssociationsNode->DelSubTree();
        topicAssociationsNode->SetNodeType( GUCEF_DATATYPE_ARRAY );
        PubSubFlowRouteTopicConfigVector::const_iterator i = topicAssociations.begin();
        while ( i != topicAssociations.end() )
        {
            const CPubSubFlowRouteTopicConfig& topicConfig = (*i);
            CORE::CDataNode* topicConfigNode = topicAssociationsNode->AddChild();
            totalSuccess = topicConfig.SaveConfig( *topicConfigNode ) && totalSuccess;
            ++i;
        }
    }    

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubFlowRouteConfig::LoadConfig( const CORE::CDataNode& cfg )
{GUCEF_TRACE;

    bool totalSuccess = true;

    fromSideId = cfg.GetAttributeValueOrChildValueByName( "fromSideId" ).AsString( fromSideId, true ); 
    toSideId = cfg.GetAttributeValueOrChildValueByName( "toSideId" ).AsString( toSideId, true ); 
    failoverSideId = cfg.GetAttributeValueOrChildValueByName( "failoverSideId" ).AsString( failoverSideId, true ); 
    spilloverBufferSideId = cfg.GetAttributeValueOrChildValueByName( "spilloverBufferSideId" ).AsString( spilloverBufferSideId, true ); 
    deadLetterSideId = cfg.GetAttributeValueOrChildValueByName( "deadLetterSideId" ).AsString( deadLetterSideId, true ); 
    toSideTopicsAutoMatchFromSide = cfg.GetAttributeValueOrChildValueByName( "toSideTopicsAutoMatchFromSide" ).AsBool( toSideTopicsAutoMatchFromSide, true ); 
    failoverSideTopicsAutoMatchFromSide = cfg.GetAttributeValueOrChildValueByName( "failoverSideTopicsAutoMatchFromSide" ).AsBool( failoverSideTopicsAutoMatchFromSide, true ); 
    spilloverSideTopicsAutoMatchFromSide = cfg.GetAttributeValueOrChildValueByName( "spilloverSideTopicsAutoMatchFromSide" ).AsBool( spilloverSideTopicsAutoMatchFromSide, true ); 
    deadLetterSideTopicsAutoMatchFromSide = cfg.GetAttributeValueOrChildValueByName( "deadLetterSideTopicsAutoMatchFromSide" ).AsBool( deadLetterSideTopicsAutoMatchFromSide, true );
    preferFromTopicThreadForDestination = cfg.GetAttributeValueOrChildValueByName( "preferFromTopicThreadForDestination" ).AsBool( preferFromTopicThreadForDestination, true );

    const CORE::CDataNode* topicAssociationsNode = cfg.FindChild( "topicAssociations" );
    if ( GUCEF_NULL != topicAssociationsNode )
    {
        CORE::CDataNode::const_iterator i = topicAssociationsNode->ConstBegin();
        while ( i != topicAssociationsNode->ConstEnd() )
        {
            CPubSubFlowRouteTopicConfig topicConfig;
            if ( topicConfig.LoadConfig( *(*i) ) )
            {
                topicAssociations.push_back( topicConfig );
            }
            else
            {
                totalSuccess = false;
            }            
            ++i;
        }
    }

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

const CString& 
CPubSubFlowRouteConfig::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CORE::CString classTypeName = "GUCEF::PUBSUB::CPubSubFlowRouteConfig";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubFlowRouteConfig::IsAnyAutoTopicMatchingNeeded( void ) const
{GUCEF_TRACE;

    return toSideTopicsAutoMatchFromSide        ||
           failoverSideTopicsAutoMatchFromSide  ||
           spilloverSideTopicsAutoMatchFromSide ||
           deadLetterSideTopicsAutoMatchFromSide;
}

/*-------------------------------------------------------------------------*/

CPubSubFlowRouteTopicConfig*
CPubSubFlowRouteConfig::FindTopicAssociation( const CORE::CString& fromTopicName )
{GUCEF_TRACE;

    PubSubFlowRouteTopicConfigVector::iterator i = topicAssociations.begin();
    while ( i != topicAssociations.end() )
    {
        CPubSubFlowRouteTopicConfig& topicConfig = (*i);
        if ( topicConfig.fromSideTopicName == fromTopicName )
            return &topicConfig;
        ++i;
    }
    return GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

CPubSubFlowRouteTopicConfig*
CPubSubFlowRouteConfig::FindOrCreateTopicAssociation( const CORE::CString& fromTopicName )
{GUCEF_TRACE;

    CPubSubFlowRouteTopicConfig* topicRouteConfig = FindTopicAssociation( fromTopicName );
    if ( GUCEF_NULL != topicRouteConfig )
        return topicRouteConfig;

    if ( IsAnyAutoTopicMatchingNeeded() )
    {
        // Create the new association

        CPubSubFlowRouteTopicConfig newTopicRouteConfig;
        newTopicRouteConfig.fromSideTopicName = fromTopicName;
        topicAssociations.push_back( newTopicRouteConfig );
        topicRouteConfig = &topicAssociations.back();

        // Init with matching topic names as desired
        // Once initialized we respect the state as-is but at init the other config plays a role in the manner of initialization
        
        if ( toSideTopicsAutoMatchFromSide && !toSideId.IsNULLOrEmpty() && topicRouteConfig->toSideTopicName.IsNULLOrEmpty() )
            topicRouteConfig->toSideTopicName = fromTopicName;    
        if ( failoverSideTopicsAutoMatchFromSide && !failoverSideId.IsNULLOrEmpty() && topicRouteConfig->failoverSideTopicName.IsNULLOrEmpty() )
            topicRouteConfig->failoverSideTopicName = fromTopicName; 
        if ( spilloverSideTopicsAutoMatchFromSide && !spilloverBufferSideId.IsNULLOrEmpty() && topicRouteConfig->spilloverSideTopicName.IsNULLOrEmpty() )
            topicRouteConfig->spilloverSideTopicName = fromTopicName; 
        if ( deadLetterSideTopicsAutoMatchFromSide && !deadLetterSideId.IsNULLOrEmpty() && topicRouteConfig->deadLetterSideTopicName.IsNULLOrEmpty() )
            topicRouteConfig->deadLetterSideTopicName = fromTopicName; 
    }

    return topicRouteConfig;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PUBSUB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
