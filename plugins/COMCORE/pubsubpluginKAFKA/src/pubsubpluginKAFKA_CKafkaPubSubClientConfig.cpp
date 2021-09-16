/*
 *  pubsubpluginKAFKA: Generic GUCEF COMCORE plugin for providing pubsub via Kafka
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

#include "pubsubpluginKAFKA_CKafkaPubSubClientConfig.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUBPLUGIN {
namespace KAFKA {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CKafkaPubSubClientConfig::CKafkaPubSubClientConfig( void )
    : COMCORE::CPubSubClientConfig()
    , kafkaProducerGlobalConfigSettings()
    , kafkaConsumerGlobalConfigSettings()
    , kafkaConsumerDefaultTopicConfigSettings()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CKafkaPubSubClientConfig::CKafkaPubSubClientConfig( const COMCORE::CPubSubClientConfig& genericConfig )
    : COMCORE::CPubSubClientConfig( genericConfig )
    , kafkaProducerGlobalConfigSettings()
    , kafkaConsumerGlobalConfigSettings()
    , kafkaConsumerDefaultTopicConfigSettings()
{GUCEF_TRACE;

    LoadCustomConfig( genericConfig.customConfig );  
}

/*-------------------------------------------------------------------------*/

CKafkaPubSubClientConfig::~CKafkaPubSubClientConfig()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClientConfig::LoadCustomConfig( const CORE::CDataNode& config )
{GUCEF_TRACE;
    
    // Load the global config settings that are direct pass-through to RdKafka for producers
    const CORE::CDataNode* producerCfgNode = config.Search( "KafkaProducerGlobalConfig", '/', false, false ); 
    if ( GUCEF_NULL != producerCfgNode )
    {
        CORE::CDataNode::const_iterator i = producerCfgNode->ConstBegin();
        while ( i != producerCfgNode->ConstEnd() )
        {
            const CORE::CString& settingKey = (*i)->GetName();
            CORE::CString settingValue = (*i)->GetValue().AsString();

            if ( !settingKey.IsNULLOrEmpty() && !settingValue.IsNULLOrEmpty() )
            {
                kafkaProducerGlobalConfigSettings[ settingKey ] = settingValue; 
            }
            
            ++i;
        }
    }

    // Load the global config settings that are direct pass-through to RdKafka for consumers
    const CORE::CDataNode* consumerCfgNode = config.Search( "KafkaConsumerGlobalConfig", '/', false, false ); 
    if ( GUCEF_NULL != consumerCfgNode )
    {
        CORE::CDataNode::const_iterator i = consumerCfgNode->ConstBegin();
        while ( i != consumerCfgNode->ConstEnd() )
        {
            const CORE::CString& settingKey = (*i)->GetName();
            CORE::CString settingValue = (*i)->GetValue().AsString();

            if ( !settingKey.IsNULLOrEmpty() && !settingValue.IsNULLOrEmpty() )
            {
                kafkaConsumerGlobalConfigSettings[ settingKey ] = settingValue; 
            }
            
            ++i;
        }
    }

    consumerCfgNode = config.Search( "KafkaConsumerDefaultTopicConfig", '/', false, false ); 
    if ( GUCEF_NULL != consumerCfgNode )
    {
        CORE::CDataNode::const_iterator i = consumerCfgNode->ConstBegin();
        while ( i != consumerCfgNode->ConstEnd() )
        {
            const CORE::CString& settingKey = (*i)->GetName();
            CORE::CString settingValue = (*i)->GetValue().AsString();

            if ( !settingKey.IsNULLOrEmpty() && !settingValue.IsNULLOrEmpty() )
            {
                kafkaConsumerDefaultTopicConfigSettings[ settingKey ] = settingValue; 
            }
            
            ++i;
        }
    }

    return true;
}

/*-------------------------------------------------------------------------*/

CKafkaPubSubClientConfig& 
CKafkaPubSubClientConfig::operator=( const COMCORE::CPubSubClientConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        COMCORE::CPubSubClientConfig::operator=( src );
        LoadCustomConfig( src.customConfig );    
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CKafkaPubSubClientConfig& 
CKafkaPubSubClientConfig::operator=( const CKafkaPubSubClientConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        COMCORE::CPubSubClientConfig::operator=( src );

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
