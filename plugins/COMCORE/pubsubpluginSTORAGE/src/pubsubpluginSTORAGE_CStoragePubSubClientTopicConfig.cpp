/*
 *  pubsubpluginSTORAGE: Generic GUCEF COMCORE plugin for providing pubsub storage
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

#include "pubsubpluginSTORAGE_CStoragePubSubClientTopicConfig.h"

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
namespace STORAGE {

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

CStoragePubSubClientTopicConfig::CStoragePubSubClientTopicConfig( void )
    : COMCORE::CPubSubClientTopicConfig()
{GUCEF_TRACE;
    
}

/*-------------------------------------------------------------------------*/

CStoragePubSubClientTopicConfig::CStoragePubSubClientTopicConfig( const COMCORE::CPubSubClientTopicConfig& genericConfig )
    : COMCORE::CPubSubClientTopicConfig( genericConfig )
{GUCEF_TRACE;
    
    LoadCustomConfig( genericConfig.customConfig );  
}

/*-------------------------------------------------------------------------*/

CStoragePubSubClientTopicConfig::~CStoragePubSubClientTopicConfig()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CStoragePubSubClientTopicConfig::LoadCustomConfig( const CORE::CDataNode& config )
{GUCEF_TRACE;
    
    bool totalSuccess = true;

    return true;
}

/*-------------------------------------------------------------------------*/

CStoragePubSubClientTopicConfig& 
CStoragePubSubClientTopicConfig::operator=( const COMCORE::CPubSubClientTopicConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        COMCORE::CPubSubClientTopicConfig::operator=( src );


        LoadCustomConfig( src.customConfig );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CStoragePubSubClientTopicConfig& 
CStoragePubSubClientTopicConfig::operator=( const CStoragePubSubClientTopicConfig& src )
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

}; /* namespace STORAGE */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
