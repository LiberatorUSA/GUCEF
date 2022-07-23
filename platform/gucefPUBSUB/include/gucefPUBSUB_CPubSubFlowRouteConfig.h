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

#ifndef GUCEF_PUBSUB_CPUBSUBFLOWROUTECONFIG_H
#define GUCEF_PUBSUB_CPUBSUBFLOWROUTECONFIG_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "gucefCORE_CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_PUBSUB_MACROS_H
#include "gucefPUBSUB_macros.h"
#define GUCEF_PUBSUB_MACROS_H
#endif /* GUCEF_PUBSUB_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_PUBSUB_EXPORT_CPP CPubSubFlowRouteConfig : public CORE::CIConfigurable
{
    public:

    typedef std::vector< CPubSubFlowRouteConfig >   PubSubFlowRouteConfigVector;

    enum RouteType : Int32
    {
        Disabled        = 0 ,
        Primary         = 1 , /**< regular flow under normal conditions */
        Failover        = 2 , /**< if the primary flow fails traffic would be rerouted here as a equivelant */ 
        SpilloverBuffer = 3 , /**< if the primary is unhealthy or is a slow consumer the spill over acts as buffer for the primary route */
        DeadLetter      = 4   /**< unable to publish on configured channels and remedies exhausted messages go here */
    };

    CORE::CString fromSide;
    CORE::CString toSide;
    RouteType routeType;

    static CORE::CString RouteTypeToString( RouteType routeType );
    static RouteType StringToRouteType( const CORE::CString& routeTypeStr );

    CPubSubFlowRouteConfig( void );

    CPubSubFlowRouteConfig( const CPubSubFlowRouteConfig& src );

    virtual ~CPubSubFlowRouteConfig();

    CPubSubFlowRouteConfig& operator=( const CPubSubFlowRouteConfig& src );

    virtual bool SaveConfig( CORE::CDataNode& cfg ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool LoadConfig( const CORE::CDataNode& cfg ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;

};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PUBSUB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PUBSUB_CPUBSUBFLOWROUTECONFIG_H ? */

