/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
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

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#include "gucefCOMCORE_CINetworkInterface.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CNetworkInterfaceMetrics::CNetworkInterfaceMetrics( void )
    : transmitLinkSpeedBitsPerSec( 0 )
    , hasTransmitLinkSpeedBitsPerSec( false )
    , receiveLinkSpeedBitsPerSec( 0 )
    , hasReceiveLinkSpeedBitsPerSec( false )
    , inboundOctets( 0 )
    , hasInboundOctets( false )
    , inboundUnicastPackets( 0 )
    , hasInboundUnicastPackets( false )
    , inboundNonUnicastPackets( 0 )
    , hasInboundNonUnicastPackets( false )
    , inboundDiscardedPackets( 0 )
    , hasInboundDiscardedPackets( false )
    , inboundErroredPackets( 0 )
    , hasInboundErroredPackets( false )
    , inboundUnknownProtocolPackets( 0 )
    , hasInboundUnknownProtocolPackets( false )
    , inboundUnicastOctets( 0 )
    , hasInboundUnicastOctets( false )
    , inboundMulticastOctets( 0 )
    , hasInboundMulticastOctets( false )
    , inboundBroadcastOctets( 0 )
    , hasInboundBroadcastOctets( false )
    , outboundOctets( 0 )
    , hasOutboundOctets( false )
    , outboundUnicastPackets( 0 )
    , hasOutboundUnicastPackets( false )
    , outboundNonUnicastPackets( 0 )
    , hasOutboundNonUnicastPackets( false )
    , outboundDiscardedPackets( 0 )
    , hasOutboundDiscardedPackets( false )
    , outboundErroredPackets( 0 )
    , hasOutboundErroredPackets( false )
    , outboundUnknownProtocolPackets( 0 )
    , hasOutboundUnknownProtocolPackets( false )
    , outboundUnicastOctets( 0 )
    , hasOutboundUnicastOctets( false )
    , outboundMulticastOctets( 0 )
    , hasOutboundMulticastOctets( false )
    , outboundBroadcastOctets( 0 )
    , hasOutboundBroadcastOctets( false )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CINetworkInterface::CINetworkInterface( void )
    : CICommunicationInterface()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CINetworkInterface::CINetworkInterface( const CINetworkInterface& src )
    : CICommunicationInterface( src )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CINetworkInterface&
CINetworkInterface::operator=( const CINetworkInterface& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CINetworkInterface::~CINetworkInterface()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
