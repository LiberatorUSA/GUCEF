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

#ifndef GUCEF_CORE_METRICSMACROS_H
#include "gucefCORE_MetricsMacros.h"
#define GUCEF_CORE_METRICSMACROS_H
#endif /* GUCEF_CORE_METRICSMACROS_H ? */

#include "pubsubpluginUDP_CUdpPubSubClientConfig.h"

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
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CUdpPubSubClientConfig::CUdpPubSubClientConfig( void )
    : COMCORE::CPubSubClientConfig()
    , transmitTestPackets( false )
    , testPacketTransmissionIntervalInMs( 1000 )
    , testPacket()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CUdpPubSubClientConfig::CUdpPubSubClientConfig( const COMCORE::CPubSubClientConfig& genericConfig )
    : COMCORE::CPubSubClientConfig( genericConfig )
    , transmitTestPackets( false )
    , testPacketTransmissionIntervalInMs( 1000 )
    , testPacket()
{GUCEF_TRACE;

    LoadCustomConfig( genericConfig.customConfig );  
}

/*-------------------------------------------------------------------------*/

CUdpPubSubClientConfig::~CUdpPubSubClientConfig()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CUdpPubSubClientConfig::LoadCustomConfig( const CORE::CDataNode& config )
{GUCEF_TRACE;
    
    testPacketTransmissionIntervalInMs = config.GetAttributeValueOrChildValueByName( "testPacketTransmissionIntervalInMs" ).AsUInt32( testPacketTransmissionIntervalInMs, true );
    transmitTestPackets = config.GetAttributeValueOrChildValueByName( "transmitTestPackets" ).AsBool( transmitTestPackets, true );
    
    CORE::CString testPacketAsBase64 = config.GetAttributeValueOrChildValueByName( "testPacketAsBase64" ).AsString( CORE::CString::Empty, true );
    if ( !testPacketAsBase64.IsNULLOrEmpty() )
    {
        testPacket.CopyAndDecodeBase64From( testPacketAsBase64, 0 );
    }
    if ( 0 == testPacket.GetDataSize() )
    {
        testPacket.CopyFrom( 4, "TEST" );
    }

    return true;
}

/*-------------------------------------------------------------------------*/

CUdpPubSubClientConfig& 
CUdpPubSubClientConfig::operator=( const COMCORE::CPubSubClientConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        COMCORE::CPubSubClientConfig::operator=( src );
        LoadCustomConfig( src.customConfig );    
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CUdpPubSubClientConfig& 
CUdpPubSubClientConfig::operator=( const CUdpPubSubClientConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        COMCORE::CPubSubClientConfig::operator=( src );
        transmitTestPackets = src.transmitTestPackets;
        testPacketTransmissionIntervalInMs = src.testPacketTransmissionIntervalInMs;
    }
    return *this;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace KAFKA */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
