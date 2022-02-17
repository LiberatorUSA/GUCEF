/*
 *  pubsubpluginUDP: Generic GUCEF COMCORE plugin for providing pubsub approximation via UDP
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

#include "pubsubpluginUDP_CUdpPubSubClientTopicConfig.h"

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
namespace UDP {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#define GUCEF_DEFAULT_UDP_RECEIVE_BUFFERS                           100
#define GUCEF_DEFAULT_TICKET_REFILLS_ON_BUSY_CYCLE                  10000
#define GUCEF_DEFAULT_UDP_OS_LEVEL_RECEIVE_BUFFER_SIZE              (1024 * 1024 * 10)
#define GUCEF_DEFAULT_UDP_MAX_SOCKET_CYCLES_PER_PULSE               25
#define GUCEF_DEFAULT_UDP_SOURCE_METADATA_KEY_NAME                  "udpSourceAddressIP4"
#define GUCEF_DEFAULT_UDP_MAX_PLAYLOAD_SIZE_IN_BYTES                1500

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CUdpPubSubClientTopicConfig::CUdpPubSubClientTopicConfig( void )
    : COMCORE::CPubSubClientTopicConfig()
    , udpInterface()
    , udpMulticastToJoin()
    , wantsTestPackage( false )
    , ticketRefillOnBusyCycle( GUCEF_DEFAULT_TICKET_REFILLS_ON_BUSY_CYCLE )
    , nrOfUdpReceiveBuffersPerSocket( GUCEF_DEFAULT_UDP_RECEIVE_BUFFERS )
    , udpSocketOsReceiveBufferSize( GUCEF_DEFAULT_UDP_OS_LEVEL_RECEIVE_BUFFER_SIZE )
    , udpSocketUpdateCyclesPerPulse( GUCEF_DEFAULT_UDP_MAX_SOCKET_CYCLES_PER_PULSE )
    , addUdpSourceAddressAsMetaData( false )
    , translateUdpSourceAddressToString( false )
    , udpSourceAddressAsMetaDataKeyName( GUCEF_DEFAULT_UDP_SOURCE_METADATA_KEY_NAME )
    , maxUdpPacketPayloadSizeInBytes( GUCEF_DEFAULT_UDP_MAX_PLAYLOAD_SIZE_IN_BYTES )
    , addTimestampToReceivedPackages( true )
{GUCEF_TRACE;
    
}

/*-------------------------------------------------------------------------*/

CUdpPubSubClientTopicConfig::CUdpPubSubClientTopicConfig( const COMCORE::CPubSubClientTopicConfig& genericConfig )
    : COMCORE::CPubSubClientTopicConfig( genericConfig )
    , udpInterface()
    , udpMulticastToJoin()
    , wantsTestPackage( false )
    , ticketRefillOnBusyCycle( GUCEF_DEFAULT_TICKET_REFILLS_ON_BUSY_CYCLE )
    , nrOfUdpReceiveBuffersPerSocket( GUCEF_DEFAULT_UDP_RECEIVE_BUFFERS )
    , udpSocketOsReceiveBufferSize( GUCEF_DEFAULT_UDP_OS_LEVEL_RECEIVE_BUFFER_SIZE )
    , udpSocketUpdateCyclesPerPulse( GUCEF_DEFAULT_UDP_MAX_SOCKET_CYCLES_PER_PULSE )
    , addUdpSourceAddressAsMetaData( false )
    , translateUdpSourceAddressToString( false )
    , udpSourceAddressAsMetaDataKeyName( GUCEF_DEFAULT_UDP_SOURCE_METADATA_KEY_NAME )
    , maxUdpPacketPayloadSizeInBytes( GUCEF_DEFAULT_UDP_MAX_PLAYLOAD_SIZE_IN_BYTES )
    , addTimestampToReceivedPackages( true )
{GUCEF_TRACE;
    
    LoadCustomConfig( genericConfig.customConfig );  
}

/*-------------------------------------------------------------------------*/

CUdpPubSubClientTopicConfig::~CUdpPubSubClientTopicConfig()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CUdpPubSubClientTopicConfig::LoadCustomConfig( const CORE::CDataNode& config )
{GUCEF_TRACE;
    
    bool totalSuccess = true;

    // Within the context of UDP the consumer group field can be used to signal a multicast or broadcast group
    if ( !consumerGroupName.IsNULLOrEmpty() )
    {
        COMCORE::CHostAddress networkGroup;
        if ( networkGroup.SetHostnameAndPort( consumerGroupName ) ) // note that port will be ignored       
            udpMulticastToJoin.push_back( networkGroup );
    }
    // Within the context of UDP the consumer name field can be used to signal the local interface from which to bind which for
    // UDP broadcast acts as the subscriber id / group member name
    if ( !consumerName.IsNULLOrEmpty() )
    {
        COMCORE::CHostAddress networkInterface;
        if ( networkInterface.SetHostnameAndPort( consumerName ) ) // note that port will be ignored       
            udpInterface = networkInterface;
    }
    // Now again load the NIC to bind from the custom settings which may override the generic setting above
    udpInterface.SetHostnameAndPort( config.GetAttributeValueOrChildValueByName( "udpInterface" ).AsString( udpInterface.AddressAndPortAsString(), true ) );

    wantsTestPackage = config.GetAttributeValueOrChildValueByName( "wantsTestPackage" ).AsBool( wantsTestPackage, true );
    ticketRefillOnBusyCycle = config.GetAttributeValueOrChildValueByName( "ticketRefillOnBusyCycle" ).AsUInt32( ticketRefillOnBusyCycle, true );
    nrOfUdpReceiveBuffersPerSocket = config.GetAttributeValueOrChildValueByName( "nrOfUdpReceiveBuffersPerSocket" ).AsUInt32( nrOfUdpReceiveBuffersPerSocket, true );
    udpSocketOsReceiveBufferSize = config.GetAttributeValueOrChildValueByName( "udpSocketOsReceiveBufferSize" ).AsUInt32( udpSocketOsReceiveBufferSize, true );             
    udpSocketUpdateCyclesPerPulse = config.GetAttributeValueOrChildValueByName( "udpSocketUpdateCyclesPerPulse" ).AsUInt32( udpSocketUpdateCyclesPerPulse, true );
    addUdpSourceAddressAsMetaData = config.GetAttributeValueOrChildValueByName( "addUdpSourceAddressAsMetaData" ).AsBool( addUdpSourceAddressAsMetaData, true );
    translateUdpSourceAddressToString = config.GetAttributeValueOrChildValueByName( "translateUdpSourceAddressToString" ).AsBool( translateUdpSourceAddressToString, true );
    udpSourceAddressAsMetaDataKeyName = config.GetAttributeValueOrChildValueByName( "udpSourceAddressAsMetaDataKeyName" ).AsString( udpSourceAddressAsMetaDataKeyName, true );
    maxUdpPacketPayloadSizeInBytes = config.GetAttributeValueOrChildValueByName( "maxUdpPacketPayloadSizeInBytes" ).AsUInt16( maxUdpPacketPayloadSizeInBytes, true );
    addTimestampToReceivedPackages = config.GetAttributeValueOrChildValueByName( "addTimestampToReceivedPackages" ).AsBool( addTimestampToReceivedPackages, true );

    return true;
}

/*-------------------------------------------------------------------------*/

CUdpPubSubClientTopicConfig& 
CUdpPubSubClientTopicConfig::operator=( const COMCORE::CPubSubClientTopicConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        COMCORE::CPubSubClientTopicConfig::operator=( src );

        LoadCustomConfig( src.customConfig );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CUdpPubSubClientTopicConfig& 
CUdpPubSubClientTopicConfig::operator=( const CUdpPubSubClientTopicConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        COMCORE::CPubSubClientTopicConfig::operator=( src );

        udpInterface = src.udpInterface;
        udpMulticastToJoin = src.udpMulticastToJoin;
        wantsTestPackage = src.wantsTestPackage;
        ticketRefillOnBusyCycle = src.ticketRefillOnBusyCycle;
        nrOfUdpReceiveBuffersPerSocket = src.nrOfUdpReceiveBuffersPerSocket;
        udpSocketOsReceiveBufferSize = src.udpSocketOsReceiveBufferSize;
        udpSocketUpdateCyclesPerPulse = src.udpSocketUpdateCyclesPerPulse;
        addUdpSourceAddressAsMetaData = src.addUdpSourceAddressAsMetaData;
        translateUdpSourceAddressToString = src.translateUdpSourceAddressToString;
        udpSourceAddressAsMetaDataKeyName = src.udpSourceAddressAsMetaDataKeyName;
        maxUdpPacketPayloadSizeInBytes = src.maxUdpPacketPayloadSizeInBytes;
        addTimestampToReceivedPackages = src.addTimestampToReceivedPackages;
    }
    return *this;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace UDP */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
