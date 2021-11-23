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

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CUdpPubSubClientTopicConfig::CUdpPubSubClientTopicConfig( void )
    : COMCORE::CPubSubClientTopicConfig()
{GUCEF_TRACE;
    
}

/*-------------------------------------------------------------------------*/

CUdpPubSubClientTopicConfig::CUdpPubSubClientTopicConfig( const COMCORE::CPubSubClientTopicConfig& genericConfig )
    : COMCORE::CPubSubClientTopicConfig( genericConfig )
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
