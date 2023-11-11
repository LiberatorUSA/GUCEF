/*
 *  pubsubpluginAWSSQS: Generic GUCEF COMCORE plugin for providing pubsub via AWS's SQS
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

#include "pubsubpluginAWSSQS_CAwsSqsPubSubClientTopicConfig.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUBPLUGIN {
namespace AWSSQS {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CAwsSqsPubSubClientTopicConfig::CAwsSqsPubSubClientTopicConfig( void )
    : PUBSUB::CPubSubClientTopicConfig()
    , CORE::CTSharedObjCreator< CAwsSqsPubSubClientTopicConfig, MT::CMutex >( this )
    , topicNameIsQueueName( false )
    , tryToUseSendMessageBatch( false )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CAwsSqsPubSubClientTopicConfig::CAwsSqsPubSubClientTopicConfig( const CAwsSqsPubSubClientTopicConfig& src )
    : PUBSUB::CPubSubClientTopicConfig( src )
    , CORE::CTSharedObjCreator< CAwsSqsPubSubClientTopicConfig, MT::CMutex >( this )
    , topicNameIsQueueName( src.topicNameIsQueueName )
    , tryToUseSendMessageBatch( src.tryToUseSendMessageBatch )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CAwsSqsPubSubClientTopicConfig::CAwsSqsPubSubClientTopicConfig( const PUBSUB::CPubSubClientTopicConfig& genericConfig )
    : PUBSUB::CPubSubClientTopicConfig( genericConfig )
    , CORE::CTSharedObjCreator< CAwsSqsPubSubClientTopicConfig, MT::CMutex >( this )
    , topicNameIsQueueName( false )
    , tryToUseSendMessageBatch( false )
{GUCEF_TRACE;

    LoadCustomConfig( genericConfig.customConfig );  
}

/*-------------------------------------------------------------------------*/

CAwsSqsPubSubClientTopicConfig::~CAwsSqsPubSubClientTopicConfig()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CAwsSqsPubSubClientTopicConfig::LoadCustomConfig( const CORE::CDataNode& config )
{GUCEF_TRACE;
        
    topicNameIsQueueName = config.GetAttributeValueOrChildValueByName( "topicNameIsQueueName" ).AsBool( topicNameIsQueueName, true );
    tryToUseSendMessageBatch = config.GetAttributeValueOrChildValueByName( "tryToUseSendMessageBatch" ).AsBool( tryToUseSendMessageBatch, true );
    
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CAwsSqsPubSubClientTopicConfig::LoadConfig( const PUBSUB::CPubSubClientTopicConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        PUBSUB::CPubSubClientTopicConfig::operator=( src );
        return LoadCustomConfig( src.customConfig );    
    }
    return true;
}

/*-------------------------------------------------------------------------*/

CAwsSqsPubSubClientTopicConfig& 
CAwsSqsPubSubClientTopicConfig::operator=( const PUBSUB::CPubSubClientTopicConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        PUBSUB::CPubSubClientTopicConfig::operator=( src );
        LoadCustomConfig( src.customConfig );    
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CAwsSqsPubSubClientTopicConfig& 
CAwsSqsPubSubClientTopicConfig::operator=( const CAwsSqsPubSubClientTopicConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        PUBSUB::CPubSubClientTopicConfig::operator=( src );
        topicNameIsQueueName = src.topicNameIsQueueName;
        tryToUseSendMessageBatch = src.tryToUseSendMessageBatch;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CORE::CICloneable* 
CAwsSqsPubSubClientTopicConfig::Clone( void ) const
{GUCEF_TRACE;

    return GUCEF_NEW CAwsSqsPubSubClientTopicConfig( *this );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace AWSSQS */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
