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

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CWebPubSubClientTopicConfig::CWebPubSubClientTopicConfig( void )
    : COMCORE::CPubSubClientTopicConfig()
    , httpServerPort( 10001 )
    , exposeBasicHealthEndpoint( true )
    , basicHealthEndpointPath( "/health/basic" )
{GUCEF_TRACE;
    
}

/*-------------------------------------------------------------------------*/

CWebPubSubClientTopicConfig::CWebPubSubClientTopicConfig( const COMCORE::CPubSubClientTopicConfig& genericConfig )
    : COMCORE::CPubSubClientTopicConfig( genericConfig )
    , httpServerPort( 10001 )
    , exposeBasicHealthEndpoint( true )
    , basicHealthEndpointPath( "/health/basic" )
{GUCEF_TRACE;
    
    LoadCustomConfig( genericConfig.customConfig );  
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

    return true;
}

/*-------------------------------------------------------------------------*/

CWebPubSubClientTopicConfig& 
CWebPubSubClientTopicConfig::operator=( const COMCORE::CPubSubClientTopicConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        COMCORE::CPubSubClientTopicConfig::operator=( src );

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
        COMCORE::CPubSubClientTopicConfig::operator=( src );
        httpServerPort = src.httpServerPort;
        exposeBasicHealthEndpoint = src.exposeBasicHealthEndpoint;
        basicHealthEndpointPath = src.basicHealthEndpointPath;
    }
    return *this;
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
