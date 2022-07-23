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

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#include "gucefPUBSUB_CPubSubFlowRouterConfig.h"

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

CPubSubFlowRouterConfig::CPubSubFlowRouterConfig( void )
    : CORE::CIConfigurable()
    , ackStyle( AckStyle::AllOrNothing )
    , routes()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubFlowRouterConfig::CPubSubFlowRouterConfig( const CPubSubFlowRouterConfig& src )
    : CORE::CIConfigurable( src )
    , ackStyle( src.ackStyle )
    , routes( src.routes )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubFlowRouterConfig::~CPubSubFlowRouterConfig()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubFlowRouterConfig& 
CPubSubFlowRouterConfig::operator=( const CPubSubFlowRouterConfig& src )
{
    if ( this != &src )
    {
        CORE::CIConfigurable::operator=( src );
        ackStyle = src.ackStyle;
        routes = src.routes;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

void
CPubSubFlowRouterConfig::Clear( void )
{GUCEF_TRACE;

    ackStyle = AckStyle::AllOrNothing;
    routes.clear();
}

/*-------------------------------------------------------------------------*/

CORE::CString
CPubSubFlowRouterConfig::AckStyleToString( AckStyle ackStyle )
{GUCEF_TRACE;

    switch ( ackStyle )
    {
        case AllOrNothing:      return "AllOrNothing";
        case FireAndForget:     return "FireAndForget";
        case AnyOneForAll:      return "AnyOneForAll";
        default:                return CORE::Int32ToString( ackStyle );
    }
}

/*-------------------------------------------------------------------------*/

CPubSubFlowRouterConfig::AckStyle 
CPubSubFlowRouterConfig::StringToAckStyle( const CORE::CString& ackStyleStr )
{GUCEF_TRACE;

    CORE::CString ackStyleStrLc = ackStyleStr.Lowercase();
    if ( "allornothing" == ackStyleStrLc ) return AllOrNothing;
    if ( "fireandforget" == ackStyleStrLc ) return FireAndForget;
    if ( "anyoneforall" == ackStyleStrLc ) return AnyOneForAll;

    // If we get here treat it as an int
    return (AckStyle) CORE::StringToInt32( ackStyleStrLc, AllOrNothing );
}

/*-------------------------------------------------------------------------*/

bool
CPubSubFlowRouterConfig::SaveConfig( CORE::CDataNode& cfg ) const
{GUCEF_TRACE;

    bool totalSuccess = true;
    totalSuccess = cfg.SetAttribute( "ackStyle", AckStyleToString( ackStyle ) ) && totalSuccess;

    CORE::CDataNode* routesParentNode = cfg.FindOrAddChild( "routes" );
    if ( GUCEF_NULL != routesParentNode )
    {
        routesParentNode->SetNodeType( GUCEF_DATATYPE_ARRAY );
        PubSubFlowRouteConfigVector::const_iterator i = routes.begin();
        while ( i != routes.end() )
        {
            const CPubSubFlowRouteConfig& route = (*i);
            CORE::CDataNode* routeNode = routesParentNode->AddChild( "PubSubFlowRouteConfig" );
            if ( GUCEF_NULL != routeNode )
            {
                if ( !route.SaveConfig( *routeNode ) )
                    totalSuccess = false;    
            }
            else
            {
                totalSuccess = false;
            }
            ++i;
        }
    }
    else
    {
        totalSuccess = false;
    }

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubFlowRouterConfig::LoadConfig( const CORE::CDataNode& cfg )
{GUCEF_TRACE;

    ackStyle = StringToAckStyle( cfg.GetAttributeValueOrChildValueByName( "ackStyle" ).AsString( AckStyleToString( ackStyle ), true ) );

    const CORE::CDataNode* routesParentNode = cfg.Find( "routes" );
    if ( GUCEF_NULL != routesParentNode )
    {
        CORE::CDataNode::const_iterator i = routesParentNode->ConstBegin();
        while ( i != routesParentNode->ConstEnd() )
        {
            const CORE::CDataNode* routeNode = (*i);
            
            CPubSubFlowRouteConfig route;
            if ( route.LoadConfig( *routeNode ) )
            {
                routes.push_back( route );
            }
            ++i;
        }
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "PubSubFlowRouterConfig(" + CORE::PointerToString( this ) +
            "):LoadConfig: Missing 'routes' config is malformed" );
        return false;
    }

    return true;
}

/*-------------------------------------------------------------------------*/

const CString& 
CPubSubFlowRouterConfig::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CString classTypeName = "GUCEF::PUBSUB::CPubSubFlowRouterConfig";
    return classTypeName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PUBSUB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
