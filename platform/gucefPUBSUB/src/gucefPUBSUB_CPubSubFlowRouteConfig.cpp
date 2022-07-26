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
    , failoverSide()
    , spilloverBufferSide()
    , deadLetterSide()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubFlowRouteConfig::CPubSubFlowRouteConfig( const CPubSubFlowRouteConfig& src )
    : CORE::CIConfigurable( src ) 
    , fromSide( src.fromSide )
    , toSide( src.toSide )
    , failoverSide( src.failoverSide )
    , spilloverBufferSide( src.spilloverBufferSide )
    , deadLetterSide( src.deadLetterSide )
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
        failoverSide = src.failoverSide;
        spilloverBufferSide = src.spilloverBufferSide;
        deadLetterSide = src.deadLetterSide;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubFlowRouteConfig::SaveConfig( CORE::CDataNode& cfg ) const
{GUCEF_TRACE;

    bool totalSuccess = true;
    totalSuccess = cfg.SetAttribute( "fromSide", fromSide ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "toSide", toSide ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "failoverSide", failoverSide ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "spilloverBufferSide", spilloverBufferSide ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "deadLetterSide", deadLetterSide ) && totalSuccess;
    
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubFlowRouteConfig::LoadConfig( const CORE::CDataNode& cfg )
{GUCEF_TRACE;

    fromSide = cfg.GetAttributeValueOrChildValueByName( "fromSide" ).AsString( fromSide, true ); 
    toSide = cfg.GetAttributeValueOrChildValueByName( "toSide" ).AsString( toSide, true ); 
    failoverSide = cfg.GetAttributeValueOrChildValueByName( "failoverSide" ).AsString( failoverSide, true ); 
    spilloverBufferSide = cfg.GetAttributeValueOrChildValueByName( "spilloverBufferSide" ).AsString( spilloverBufferSide, true ); 
    deadLetterSide = cfg.GetAttributeValueOrChildValueByName( "deadLetterSide" ).AsString( deadLetterSide, true ); 

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
