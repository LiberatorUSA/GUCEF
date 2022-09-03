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
    : PUBSUB::CPubSubClientConfig()
    , transmitTestPackets( false )
    , testPacketTransmissionIntervalInMs( 1000 )
    , testPacket()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CUdpPubSubClientConfig::CUdpPubSubClientConfig( const PUBSUB::CPubSubClientConfig& genericConfig )
    : PUBSUB::CPubSubClientConfig( genericConfig )
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
CUdpPubSubClientConfig::SaveCustomConfig( CORE::CDataNode& config ) const
{GUCEF_TRACE;
    
    // @TODO
    return false;
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
CUdpPubSubClientConfig::operator=( const PUBSUB::CPubSubClientConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        PUBSUB::CPubSubClientConfig::operator=( src );
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
        PUBSUB::CPubSubClientConfig::operator=( src );
        transmitTestPackets = src.transmitTestPackets;
        testPacketTransmissionIntervalInMs = src.testPacketTransmissionIntervalInMs;
        testPacket = src.testPacket;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool 
CUdpPubSubClientConfig::SerializeCustomConfigToGenericConfig( void )
{GUCEF_TRACE;

    return SaveCustomConfig( customConfig ); 
}

/*-------------------------------------------------------------------------*/

bool
CUdpPubSubClientConfig::LoadConfig( const CORE::CDataNode& config )
{GUCEF_TRACE;

    if ( PUBSUB::CPubSubClientConfig::LoadConfig( config ) )
    {
        return LoadCustomConfig( customConfig ); 
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CUdpPubSubClientConfig::SaveConfig( CORE::CDataNode& config ) const
{GUCEF_TRACE;

    PUBSUB::CPubSubClientConfig cfgCopy( *this );
    if ( SaveCustomConfig( cfgCopy.customConfig ) )
    {
        // Now that the custom config is re-integrated we can just use the base class to save the config
        return cfgCopy.SaveConfig( config ); 
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CUdpPubSubClientConfig::LoadConfig( const PUBSUB::CPubSubClientConfig& cfg )
{GUCEF_TRACE;

    // For the base class portion there is nothing to parse as its a simple
    // assignment. Its the custom config where the actual parsed loading occurs
    if ( &cfg != this )
    {
        PUBSUB::CPubSubClientConfig::operator=( cfg );
    }
    return LoadCustomConfig( cfg.customConfig ); 
}

/*-------------------------------------------------------------------------*/

bool
CUdpPubSubClientConfig::SaveConfig( PUBSUB::CPubSubClientConfig& cfg ) const
{GUCEF_TRACE;

    // For the base class portion there is nothing to do except a simple
    // assignment. Its the custom config where the actual config serialization occurs
    cfg = *this;
    return SaveCustomConfig( cfg.customConfig );
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
