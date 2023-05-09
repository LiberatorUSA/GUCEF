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

#include "pubsubpluginKAFKA_CKafkaPubSubClientTopicConfig.h"

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

CKafkaPubSubClientTopicConfig::CKafkaPubSubClientTopicConfig( void )
    : PUBSUB::CPubSubClientTopicConfig()
    , CORE::CTSharedObjCreator< CKafkaPubSubClientTopicConfig, MT::CMutex >( this )
    , kafkaProducerTopicConfigSettings()
    , kafkaConsumerTopicConfigSettings()
    , consumerModeStartOffset( "stored" )
    , useKafkaMsgHeadersForConsumerFiltering( false )
    , kafkaMsgHeaderUsedForFiltering()
    , kafkaMsgValuesUsedForFiltering()
    , addProducerHostnameAsKafkaMsgHeader( false )
    , prefixToAddForMetaDataKvPairs()
    , prefixToAddForKvPairs()
    , stripPrefixForMetaDataKvPairs( true )
    , stripPrefixForKvPairs( true )
    , maxKafkaErrorsToBeHealthy( 1 )
{GUCEF_TRACE;

    consumerGroupName = "0";
}

/*-------------------------------------------------------------------------*/

CKafkaPubSubClientTopicConfig::CKafkaPubSubClientTopicConfig( const PUBSUB::CPubSubClientTopicConfig& genericConfig )
    : PUBSUB::CPubSubClientTopicConfig( genericConfig )
    , CORE::CTSharedObjCreator< CKafkaPubSubClientTopicConfig, MT::CMutex >( this )
    , kafkaProducerTopicConfigSettings()
    , kafkaConsumerTopicConfigSettings()
    , consumerModeStartOffset( "stored" )
    , useKafkaMsgHeadersForConsumerFiltering( false )
    , kafkaMsgHeaderUsedForFiltering()
    , kafkaMsgValuesUsedForFiltering()
    , addProducerHostnameAsKafkaMsgHeader( false )
    , prefixToAddForMetaDataKvPairs()
    , prefixToAddForKvPairs()
    , stripPrefixForMetaDataKvPairs( true )
    , stripPrefixForKvPairs( true )
    , maxKafkaErrorsToBeHealthy( 1 )
{GUCEF_TRACE;

    consumerGroupName = "0";
    LoadCustomConfig( genericConfig.customConfig );  
}

/*-------------------------------------------------------------------------*/

CKafkaPubSubClientTopicConfig::~CKafkaPubSubClientTopicConfig()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClientTopicConfig::LoadCustomConfig( const CORE::CDataNode& config )
{GUCEF_TRACE;
    
    // Load the topic config settings that are direct pass-through to RdKafka for producers
    const CORE::CDataNode* producerCfgNode = config.Search( "KafkaProducerTopicConfig", '/', false, false ); 
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
                    kafkaProducerTopicConfigSettings[ att->first ] = att->second; 
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
                kafkaProducerTopicConfigSettings[ settingKey ] = settingValue; 
            }
            
            ++i;
        }
    }

    // Load the topic config settings that are direct pass-through to RdKafka for consumers
    const CORE::CDataNode* consumerCfgNode = config.Search( "KafkaConsumerTopicConfig", '/', false, false ); 
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
                    kafkaConsumerTopicConfigSettings[ att->first ] = att->second; 
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
                kafkaConsumerTopicConfigSettings[ settingKey ] = settingValue; 
            }
            
            ++i;
        }
    }

    consumerModeStartOffset = config.GetAttributeValueOrChildValueByName( "consumerModeStartOffset" ).AsString( consumerModeStartOffset, true ); 
    useKafkaMsgHeadersForConsumerFiltering = config.GetAttributeValueOrChildValueByName( "useKafkaMsgHeadersForConsumerFiltering" ).AsBool( useKafkaMsgHeadersForConsumerFiltering, true ); 
    kafkaMsgHeaderUsedForFiltering = config.GetAttributeValueOrChildValueByName( "kafkaMsgHeaderUsedForFiltering" ).AsString( kafkaMsgHeaderUsedForFiltering, true );
    kafkaMsgValuesUsedForFiltering = ToStringVector( config.GetAttributeValueOrChildValuesByName( "kafkaMsgValuesUsedForFiltering" ) );
    addProducerHostnameAsKafkaMsgHeader = config.GetAttributeValueOrChildValueByName( "addProducerHostnameAsKafkaMsgHeader" ).AsBool( addProducerHostnameAsKafkaMsgHeader, true ); 
    prefixToAddForMetaDataKvPairs = config.GetAttributeValueOrChildValueByName( "prefixToAddForMetaDataKvPairs" ).AsString( prefixToAddForMetaDataKvPairs, true );
    prefixToAddForKvPairs = config.GetAttributeValueOrChildValueByName( "prefixToAddForKvPairs" ).AsString( prefixToAddForKvPairs, true );
    stripPrefixForMetaDataKvPairs = config.GetAttributeValueOrChildValueByName( "stripPrefixForMetaDataKvPairs" ).AsBool( stripPrefixForMetaDataKvPairs, true ); 
    stripPrefixForKvPairs = config.GetAttributeValueOrChildValueByName( "stripPrefixForKvPairs" ).AsBool( stripPrefixForKvPairs, true ); 
    maxKafkaErrorsToBeHealthy = config.GetAttributeValueOrChildValueByName( "maxKafkaErrorsToBeHealthy" ).AsInt32( maxKafkaErrorsToBeHealthy, true ); 
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CKafkaPubSubClientTopicConfig::LoadConfig( const PUBSUB::CPubSubClientTopicConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        PUBSUB::CPubSubClientTopicConfig::operator=( src );
        return LoadCustomConfig( src.customConfig );    
    }
    return false;
}

/*-------------------------------------------------------------------------*/

CKafkaPubSubClientTopicConfig& 
CKafkaPubSubClientTopicConfig::operator=( const PUBSUB::CPubSubClientTopicConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        PUBSUB::CPubSubClientTopicConfig::operator=( src );
        LoadCustomConfig( src.customConfig );    
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CKafkaPubSubClientTopicConfig& 
CKafkaPubSubClientTopicConfig::operator=( const CKafkaPubSubClientTopicConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        PUBSUB::CPubSubClientTopicConfig::operator=( src );

        kafkaProducerTopicConfigSettings = src.kafkaProducerTopicConfigSettings;
        kafkaConsumerTopicConfigSettings = src.kafkaConsumerTopicConfigSettings;
        consumerModeStartOffset = src.consumerModeStartOffset;
        useKafkaMsgHeadersForConsumerFiltering = src.useKafkaMsgHeadersForConsumerFiltering;
        kafkaMsgHeaderUsedForFiltering = src.kafkaMsgHeaderUsedForFiltering;
        kafkaMsgValuesUsedForFiltering = src.kafkaMsgValuesUsedForFiltering;
        addProducerHostnameAsKafkaMsgHeader = src.addProducerHostnameAsKafkaMsgHeader;
        prefixToAddForMetaDataKvPairs = src.prefixToAddForMetaDataKvPairs;
        prefixToAddForKvPairs = src.prefixToAddForKvPairs;
        stripPrefixForMetaDataKvPairs = src.stripPrefixForMetaDataKvPairs;
        stripPrefixForKvPairs = src.stripPrefixForKvPairs;
        maxKafkaErrorsToBeHealthy = src.maxKafkaErrorsToBeHealthy;
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
