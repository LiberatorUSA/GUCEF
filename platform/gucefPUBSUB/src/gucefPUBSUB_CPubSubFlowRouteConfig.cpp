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

CPubSubFlowRouteConfig::CPubSubFlowRouteConfig( void )
    : CORE::CIConfigurable() 
    , fromSide()
    , toSide()
    , routeType( RouteType::Disabled )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubFlowRouteConfig::CPubSubFlowRouteConfig( const CPubSubFlowRouteConfig& src )
    : CORE::CIConfigurable( src ) 
    , fromSide( src.fromSide )
    , toSide( src.toSide )
    , routeType( src.routeType )
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
        fromSide = src.fromSide;
        toSide = src.toSide;
        routeType = src.routeType;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CORE::CString
CPubSubFlowRouteConfig::RouteTypeToString( RouteType routeType )
{GUCEF_TRACE;

    switch ( routeType )
    {
        case Disabled:          return "Disabled";
        case Primary:           return "Primary";
        case Failover:          return "Failover";
        case SpilloverBuffer:   return "SpilloverBuffer";
        case DeadLetter:        return "DeadLetter";
        default:                return CString::Empty;
    }
}

/*-------------------------------------------------------------------------*/

CPubSubFlowRouteConfig::RouteType 
CPubSubFlowRouteConfig::StringToRouteType( const CORE::CString& routeTypeStr )
{GUCEF_TRACE;

    CORE::CString routeTypeStrLc = routeTypeStr.Lowercase();
    if ( "disabled" == routeTypeStrLc ) return Disabled;
    if ( "primary" == routeTypeStrLc ) return Primary;
    if ( "failover" == routeTypeStrLc ) return Failover;
    if ( "spilloverbuffer" == routeTypeStrLc ) return SpilloverBuffer;
    if ( "deadletter" == routeTypeStrLc ) return DeadLetter;

    // If we get here treat it as an int
    return (RouteType) CORE::StringToInt32( routeTypeStrLc, Disabled );
}

/*-------------------------------------------------------------------------*/

bool
CPubSubFlowRouteConfig::SaveConfig( CORE::CDataNode& cfg ) const
{GUCEF_TRACE;

    bool totalSuccess = true;
    totalSuccess = cfg.SetAttribute( "fromSide", fromSide ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "toSide", toSide ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "routeType", RouteTypeToString( routeType ) ) && totalSuccess;
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubFlowRouteConfig::LoadConfig( const CORE::CDataNode& cfg )
{GUCEF_TRACE;

    fromSide = cfg.GetAttributeValueOrChildValueByName( "fromSide" ).AsString( fromSide, true ); 
    toSide = cfg.GetAttributeValueOrChildValueByName( "toSide" ).AsString( toSide, true ); 
    routeType = StringToRouteType( cfg.GetAttributeValueOrChildValueByName( "routeType" ).AsString( RouteTypeToString( routeType ), true ) );
    return true;
}

/*-------------------------------------------------------------------------*/

const CString& 
CPubSubFlowRouteConfig::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CORE::CString classTypeName = "GUCEF::PUBSUB::CPubSubFlowRouteConfig";
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
