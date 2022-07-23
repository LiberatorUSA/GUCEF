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

#ifndef GUCEF_PUBSUB_CPUBSUBFLOWROUTER_H
#define GUCEF_PUBSUB_CPUBSUBFLOWROUTER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CREADWRITELOCK_H
#include "gucefMT_CReadWriteLock.h"
#define GUCEF_MT_CREADWRITELOCK_H
#endif /* GUCEF_MT_CREADWRITELOCK_H ? */

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "gucefCORE_CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBCLIENTSIDE_H
#include "gucefPUBSUB_CPubSubClientSide.h"
#define GUCEF_PUBSUB_CPUBSUBCLIENTSIDE_H
#endif /* GUCEF_PUBSUB_CPUBSUBCLIENTSIDE_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBFLOWROUTERCONFIG_H
#include "gucefPUBSUB_CPubSubFlowRouterConfig.h"
#define GUCEF_PUBSUB_CPUBSUBFLOWROUTERCONFIG_H
#endif /* GUCEF_PUBSUB_CPUBSUBFLOWROUTERCONFIG_H ? */

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

/**
 *  Class which aims to provide a config driven mapping between pubsub sides
 *  The mapping entails various information flows and behaviour between said sides
 */
class GUCEF_PUBSUB_EXPORT_CPP CPubSubFlowRouter 
{
    public:

    CPubSubFlowRouter( void );

    virtual ~CPubSubFlowRouter();

    bool SaveConfig( CPubSubFlowRouterConfig& cfg ) const;

    bool LoadConfig( const CPubSubFlowRouterConfig& src );

    void ClearRoutes( void );
    
    bool BuildRoutes( const CPubSubFlowRouterConfig& config ,
                      TPubSubClientSidePtrVector& sides     );

    bool IsTrackingInFlightPublishedMsgsForAcksNeeded( CPubSubClientSide* sideWeAskFor ) const;

    bool PublishMsgs( CPubSubClientSide* fromSide                          ,
                      const CPubSubClientTopic::TPubSubMsgsRefVector& msgs ,
                      bool isDeadLetter                                    );
    
    bool AcknowledgeReceiptForSide( CPubSubClientSide* msgReceiverSide ,
                                    CIPubSubMsg::TNoLockSharedPtr& msg );
    
    private:

    typedef std::vector< CPubSubClientSidePtr >   TPubSubClientSideVector;

    struct SRouteInfo
    {
        TPubSubClientSidePtrVector primaryRoutes;
        TPubSubClientSidePtrVector failoverRoutes;
        TPubSubClientSidePtrVector spilloverBufferRoutes;
        TPubSubClientSidePtrVector deadLetterRoutes;
    };
    typedef struct SRouteInfo TRouteInfo;

    typedef std::map< CPubSubClientSide*, TRouteInfo >        TSidePtrToRouteInfoMap;
    typedef std::map< CORE::CString, CORE::CString >          TStringMap;

    bool NormalizeConfig( const CPubSubFlowRouterConfig& originalConfig ,
                          TPubSubClientSidePtrVector& sides             ,
                          CPubSubFlowRouterConfig& normalizedConfig     );

    bool IsTrackingInFlightPublishedMsgsForAcksNeeded( const TPubSubClientSideVector& sides ) const;

    CPubSubFlowRouterConfig m_config;
    TSidePtrToRouteInfoMap m_routeMap;
    MT::CReadWriteLock m_lock;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PUBSUB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PUBSUB_CPUBSUBFLOWROUTER_H ? */

