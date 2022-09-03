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
    : PUBSUB::CPubSubClientConfig()
    , kafkaProducerGlobalConfigSettings()
    , kafkaConsumerGlobalConfigSettings()
    , kafkaConsumerDefaultTopicConfigSettings()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CKafkaPubSubClientConfig::CKafkaPubSubClientConfig( const PUBSUB::CPubSubClientConfig& genericConfig )
    : PUBSUB::CPubSubClientConfig( genericConfig )
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
CKafkaPubSubClientConfig::SaveCustomConfig( CORE::CDataNode& config ) const
{GUCEF_TRACE;
    
    // @TODO
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClientConfig::LoadCustomConfig( const CORE::CDataNode& config )
{GUCEF_TRACE;
    
    // Load the global config settings that are direct pass-through to RdKafka for producers
    const CORE::CDataNode* producerCfgNode = config.Search( "KafkaProducerGlobalConfig", '/', false, false ); 
    if ( GUCEF_NULL != producerCfgNode )
    {
        UInt32 attCount = producerCfgNode->GetAttCount();
        for ( UInt32 n=0; n<attCount; ++n )
        {
            const CORE::CDataNode::TKeyValuePair* att = producerCfgNode->GetAttribute( n );
            if ( GUCEF_NULL != att )
            {
                if ( !att->first.IsNULLOrEmpty() && !att->second.IsNULLOrEmpty() )
                {
                    kafkaProducerGlobalConfigSettings[ att->first ] = att->second; 
                }                
            }
        }
        
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
        UInt32 attCount = consumerCfgNode->GetAttCount();
        for ( UInt32 n=0; n<attCount; ++n )
        {
            const CORE::CDataNode::TKeyValuePair* att = consumerCfgNode->GetAttribute( n );
            if ( GUCEF_NULL != att )
            {
                if ( !att->first.IsNULLOrEmpty() && !att->second.IsNULLOrEmpty() )
                {
                    kafkaConsumerGlobalConfigSettings[ att->first ] = att->second; 
                }                
            }
        }

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
        UInt32 attCount = consumerCfgNode->GetAttCount();
        for ( UInt32 n=0; n<attCount; ++n )
        {
            const CORE::CDataNode::TKeyValuePair* att = consumerCfgNode->GetAttribute( n );
            if ( GUCEF_NULL != att )
            {
                if ( !att->first.IsNULLOrEmpty() && !att->second.IsNULLOrEmpty() )
                {
                    kafkaConsumerDefaultTopicConfigSettings[ att->first ] = att->second; 
                }                
            }
        }

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
CKafkaPubSubClientConfig::operator=( const PUBSUB::CPubSubClientConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        PUBSUB::CPubSubClientConfig::operator=( src );
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
        PUBSUB::CPubSubClientConfig::operator=( src );

    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool 
CKafkaPubSubClientConfig::SerializeCustomConfigToGenericConfig( void )
{GUCEF_TRACE;

    return SaveCustomConfig( customConfig ); 
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClientConfig::LoadConfig( const CORE::CDataNode& config )
{GUCEF_TRACE;

    if ( PUBSUB::CPubSubClientConfig::LoadConfig( config ) )
    {
        return LoadCustomConfig( customConfig ); 
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClientConfig::SaveConfig( CORE::CDataNode& config ) const
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
CKafkaPubSubClientConfig::LoadConfig( const PUBSUB::CPubSubClientConfig& cfg )
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
CKafkaPubSubClientConfig::SaveConfig( PUBSUB::CPubSubClientConfig& cfg ) const
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
