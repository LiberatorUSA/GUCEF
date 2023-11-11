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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "pubsubpluginWEB_CWebPubSubClientTopicConfig.h"

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
namespace WEB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#define REST_API_DEFAULT_SERVER_PORT            50321                

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CWebPubSubClientTopicConfig::CWebPubSubClientTopicConfig( void )
    : PUBSUB::CPubSubClientTopicConfig()
    , CORE::CTSharedObjCreator< CWebPubSubClientTopicConfig, MT::CMutex >( this )
    , httpServerPort( REST_API_DEFAULT_SERVER_PORT )
    , exposeBasicHealthEndpoint( true )
    , basicHealthEndpointPath( "/health/basic" )
    , supportHttpServerBasedRestEndpoints( true )
    , maxPublishedMsgCountToRetainForRest( 100 )
{GUCEF_TRACE;
    
}

/*-------------------------------------------------------------------------*/

CWebPubSubClientTopicConfig::CWebPubSubClientTopicConfig( const PUBSUB::CPubSubClientTopicConfig& genericConfig )
    : PUBSUB::CPubSubClientTopicConfig( genericConfig )
    , CORE::CTSharedObjCreator< CWebPubSubClientTopicConfig, MT::CMutex >( this )
    , httpServerPort( REST_API_DEFAULT_SERVER_PORT )
    , exposeBasicHealthEndpoint( true )
    , basicHealthEndpointPath( "/health/basic" )
    , supportHttpServerBasedRestEndpoints( true )
    , maxPublishedMsgCountToRetainForRest( 100 )
{GUCEF_TRACE;
    
    LoadCustomConfig( genericConfig.customConfig );  
}

/*-------------------------------------------------------------------------*/

CWebPubSubClientTopicConfig::CWebPubSubClientTopicConfig( const CWebPubSubClientTopicConfig& src )
    : PUBSUB::CPubSubClientTopicConfig( src )
    , CORE::CTSharedObjCreator< CWebPubSubClientTopicConfig, MT::CMutex >( this )
    , httpServerPort( src.httpServerPort )
    , exposeBasicHealthEndpoint( src.exposeBasicHealthEndpoint )
    , basicHealthEndpointPath( src.basicHealthEndpointPath )
    , supportHttpServerBasedRestEndpoints( src.supportHttpServerBasedRestEndpoints )
    , maxPublishedMsgCountToRetainForRest( src.maxPublishedMsgCountToRetainForRest )
{GUCEF_TRACE;
      
}

/*-------------------------------------------------------------------------*/

CWebPubSubClientTopicConfig::~CWebPubSubClientTopicConfig()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CWebPubSubClientTopicConfig::LoadCustomConfig( const CORE::CDataNode& config )
{GUCEF_TRACE;
    
    httpServerPort = config.GetAttributeValueOrChildValueByName( "httpServerPort" ).AsUInt16( httpServerPort, true );
    exposeBasicHealthEndpoint = config.GetAttributeValueOrChildValueByName( "exposeBasicHealthEndpoint" ).AsBool( exposeBasicHealthEndpoint, true );
    basicHealthEndpointPath = config.GetAttributeValueOrChildValueByName( "basicHealthEndpointPath" ).AsString( basicHealthEndpointPath, true );
    supportHttpServerBasedRestEndpoints = config.GetAttributeValueOrChildValueByName( "supportHttpServerBasedRestEndpoints" ).AsBool( supportHttpServerBasedRestEndpoints, true );
    maxPublishedMsgCountToRetainForRest = config.GetAttributeValueOrChildValueByName( "maxPublishedMsgCountToRetainForRest" ).AsInt32( maxPublishedMsgCountToRetainForRest, true );

    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CWebPubSubClientTopicConfig::LoadConfig( const PUBSUB::CPubSubClientTopicConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        PUBSUB::CPubSubClientTopicConfig::operator=( src );
        return LoadCustomConfig( src.customConfig );
    }
    return true;
}

/*-------------------------------------------------------------------------*/

CWebPubSubClientTopicConfig& 
CWebPubSubClientTopicConfig::operator=( const PUBSUB::CPubSubClientTopicConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        PUBSUB::CPubSubClientTopicConfig::operator=( src );

        LoadCustomConfig( src.customConfig );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CWebPubSubClientTopicConfig& 
CWebPubSubClientTopicConfig::operator=( const CWebPubSubClientTopicConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        PUBSUB::CPubSubClientTopicConfig::operator=( src );
        httpServerPort = src.httpServerPort;
        exposeBasicHealthEndpoint = src.exposeBasicHealthEndpoint;
        basicHealthEndpointPath = src.basicHealthEndpointPath;
        supportHttpServerBasedRestEndpoints = src.supportHttpServerBasedRestEndpoints;
        maxPublishedMsgCountToRetainForRest = src.maxPublishedMsgCountToRetainForRest;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CORE::CICloneable* 
CWebPubSubClientTopicConfig::Clone( void ) const
{GUCEF_TRACE;

    return GUCEF_NEW CWebPubSubClientTopicConfig( *this );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace WEB */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
