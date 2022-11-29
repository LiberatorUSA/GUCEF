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

#ifndef GUCEF_PUBSUB_CPUBSUBCHANNELSETTINGS_H
#define GUCEF_PUBSUB_CPUBSUBCHANNELSETTINGS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBSIDECHANNELSETTINGS_H
#include "gucefPUBSUB_CPubSubSideChannelSettings.h"
#define GUCEF_PUBSUB_CPUBSUBSIDECHANNELSETTINGS_H
#endif /* GUCEF_PUBSUB_CPUBSUBSIDECHANNELSETTINGS_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBFLOWROUTERCONFIG_H
#include "gucefPUBSUB_CPubSubFlowRouterConfig.h"
#define GUCEF_PUBSUB_CPUBSUBFLOWROUTERCONFIG_H
#endif /* GUCEF_PUBSUB_CPUBSUBFLOWROUTERCONFIG_H ? */

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
 *  Class that conveys config for a pub sub channel
 */
class GUCEF_PUBSUB_EXPORT_CPP CPubSubChannelConfig : public CORE::CIConfigurable
{
    public:

    typedef std::map< CORE::CString, CPubSubSideChannelSettings > TStringToPubSubSideChannelSettingsMap;

    CPubSubChannelConfig( void );
    CPubSubChannelConfig( const CPubSubChannelConfig& src );
    virtual ~CPubSubChannelConfig();
    CPubSubChannelConfig& operator=( const CPubSubChannelConfig& src );

    CPubSubSideChannelSettings* GetPubSubSideSettings( const CORE::CString& sideId );

    TStringToPubSubSideChannelSettingsMap pubSubSideChannelSettingsMap;
    CPubSubFlowRouterConfig flowRouterConfig;

    CORE::Int32 channelId;
    CORE::CString channelName;
    CORE::UInt32 ticketRefillOnBusyCycle;
    bool collectMetrics;                                                   
    CORE::UInt32 metricsIntervalInMs;

    virtual bool SaveConfig( CORE::CDataNode& tree ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool LoadConfig( const CORE::CDataNode& tree ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CORE::CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;
};

/*-------------------------------------------------------------------------*/

/**
 *  Class that conveys runtime relevant settings and config for a pub sub channel
 */
class GUCEF_PUBSUB_EXPORT_CPP CPubSubChannelSettings : public CPubSubChannelConfig
{
    public:

    typedef std::map< CORE::CString, CPubSubSideChannelSettings > TStringToPubSubSideChannelSettingsMap;

    CPubSubChannelSettings( void );
    CPubSubChannelSettings( const CPubSubChannelSettings& src );
    virtual ~CPubSubChannelSettings();
    CPubSubChannelSettings& operator=( const CPubSubChannelSettings& src );

    void UpdateDerivedSettings( void );

    CORE::CString metricsPrefix;                                          //< this setting is derived and cached from other settings

    virtual bool SaveConfig( CORE::CDataNode& tree ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool LoadConfig( const CORE::CDataNode& tree ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CORE::CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PUBSUB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PUBSUB_CPUBSUBCHANNELSETTINGS_H ? */

