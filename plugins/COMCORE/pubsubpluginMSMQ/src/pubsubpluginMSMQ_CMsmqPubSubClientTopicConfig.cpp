/*
 *  pubsubpluginMSMQ: Generic GUCEF COMCORE plugin for providing pubsub via Microsoft MSMQ
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

#include "pubsubpluginMSMQ_CMsmqPubSubClientTopicConfig.h"

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
namespace MSMQ {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CMsmqPubSubClientTopicConfig::CMsmqPubSubClientTopicConfig( void )
    : COMCORE::CPubSubClientTopicConfig()
    , msmqPropIdsToReceive()
    , msmqPropIdsToSend()
    , msmqPairedPropIds()
    , maxMsmqMsgsToReadPerSyncCycle( 100 )
    , topicNameIsMsmqFormatName( false )
    , convertMsmqMsgIdToString( false )
    , ignoreUnmappableMetaDataFieldOnPublish( false )
{GUCEF_TRACE;
    
    PopulateDefaultReceivePropIds();
    PopulateDefaultSendPropIds();
    PopulateDefaultPairedPropIds();
}

/*-------------------------------------------------------------------------*/

CMsmqPubSubClientTopicConfig::CMsmqPubSubClientTopicConfig( const COMCORE::CPubSubClientTopicConfig& genericConfig )
    : COMCORE::CPubSubClientTopicConfig( genericConfig )
    , msmqPropIdsToReceive()
    , msmqPropIdsToSend()
    , msmqPairedPropIds()
    , maxMsmqMsgsToReadPerSyncCycle( 100 )
    , topicNameIsMsmqFormatName( false )
    , convertMsmqMsgIdToString( false )
    , ignoreUnmappableMetaDataFieldOnPublish( false )
{GUCEF_TRACE;
    
    PopulateDefaultReceivePropIds();
    PopulateDefaultSendPropIds();
    PopulateDefaultPairedPropIds();
    LoadCustomConfig( genericConfig.customConfig );  
}

/*-------------------------------------------------------------------------*/

CMsmqPubSubClientTopicConfig::~CMsmqPubSubClientTopicConfig()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClientTopicConfig::LoadCustomConfig( const CORE::CDataNode& config )
{GUCEF_TRACE;
    
    bool totalSuccess = true;

    maxMsmqMsgsToReadPerSyncCycle = config.GetAttributeValueOrChildValueByName( "maxMsmqMsgsToReadPerSyncCycle" ).AsUInt32( maxMsmqMsgsToReadPerSyncCycle, true );
    if ( 0 == maxMsmqMsgsToReadPerSyncCycle )
        maxMsmqMsgsToReadPerSyncCycle = 1;

    topicNameIsMsmqFormatName = config.GetAttributeValueOrChildValueByName( "topicNameIsMsmqFormatName" ).AsBool( topicNameIsMsmqFormatName, true );
    convertMsmqMsgIdToString = config.GetAttributeValueOrChildValueByName( "convertMsmqMsgIdToString" ).AsBool( convertMsmqMsgIdToString, true );
    ignoreUnmappableMetaDataFieldOnPublish = config.GetAttributeValueOrChildValueByName( "ignoreUnmappableMetaDataFieldOnPublish" ).AsBool( ignoreUnmappableMetaDataFieldOnPublish, true );
    
    CORE::CString csvPropIds = config.GetAttributeValueOrChildValueByName( "msmqPropIdsToReceive" ).AsString( PropIdsToCsvString( msmqPropIdsToReceive ), true );
    msmqPropIdsToReceive.clear();
    if ( !CsvStringToPropIds( csvPropIds, msmqPropIdsToReceive ) )
    {
        totalSuccess = false;
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "MsmqPubSubClientTopicConfig: Unable to obtain MSMQ Property IDs from msmqPropIdsToReceive: " + csvPropIds );
    }

    csvPropIds = config.GetAttributeValueOrChildValueByName( "msmqPropIdsToSend" ).AsString( PropIdsToCsvString( msmqPropIdsToSend ), true );
    msmqPropIdsToSend.clear();
    if ( !CsvStringToPropIds( csvPropIds, msmqPropIdsToSend ) )
    {
        totalSuccess = false;
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "MsmqPubSubClientTopicConfig: Unable to obtain MSMQ Property IDs from msmqPropIdsToSend: " + csvPropIds );
    }

    SupplementPropIdsWithPairedPropId( msmqPropIdsToReceive );
    SupplementPropIdsWithPairedPropId( msmqPropIdsToSend );
    return true;
}

/*-------------------------------------------------------------------------*/

CMsmqPubSubClientTopicConfig& 
CMsmqPubSubClientTopicConfig::operator=( const COMCORE::CPubSubClientTopicConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        COMCORE::CPubSubClientTopicConfig::operator=( src );

        msmqPropIdsToReceive.clear();
        msmqPropIdsToSend.clear();
        PopulateDefaultReceivePropIds();
        PopulateDefaultSendPropIds();

        LoadCustomConfig( src.customConfig );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CMsmqPubSubClientTopicConfig& 
CMsmqPubSubClientTopicConfig::operator=( const CMsmqPubSubClientTopicConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        COMCORE::CPubSubClientTopicConfig::operator=( src );
        msmqPairedPropIds = src.msmqPairedPropIds;
        msmqPropIdsToReceive = src.msmqPropIdsToReceive;
        msmqPropIdsToSend = src.msmqPropIdsToSend;
        maxMsmqMsgsToReadPerSyncCycle = src.maxMsmqMsgsToReadPerSyncCycle;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CORE::CString
CMsmqPubSubClientTopicConfig::PropIdsToCsvString( const MSGPROPIDVector& propIds )
{GUCEF_TRACE;

    CORE::CString csvStr;
    MSGPROPIDVector::const_iterator i = propIds.begin();
    while ( i != propIds.end() )
    {
        if ( !csvStr.IsNULLOrEmpty() )
        {
            csvStr += ',' + CORE::ToString( CORE::UInt64( (*i) ) );
        }
        else
        {
            csvStr = CORE::ToString( CORE::UInt64( (*i) ) );
        }
        ++i;
    }
    return csvStr;
}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClientTopicConfig::CsvStringToPropIds( const CORE::CString& csvStr, MSGPROPIDVector& propIds )
{GUCEF_TRACE;

    CORE::CString::StringVector elements = csvStr.ParseElements( ',', false );
    
    bool totalSuccess = true;
    propIds.reserve( elements.size() );
    CORE::CString::StringVector::iterator i = elements.begin();
    while ( i != elements.end() )
    {
        CORE::UInt64 propId = CORE::StringToUInt64( (*i), GUCEF_MT_UINT64MAX );
        if ( propId != GUCEF_MT_UINT64MAX )
            propIds.push_back( (MSGPROPID) propId );
        else
            totalSuccess = false;
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClientTopicConfig::PopulateDefaultReceivePropIds( void )
{GUCEF_TRACE;

    // Maps to our Primary payload variant
    msmqPropIdsToReceive.push_back( PROPID_M_BODY );
    
    // Maps to our PubSubMsg MsgId, 20 unicode chars 
    msmqPropIdsToReceive.push_back( PROPID_M_MSGID );

    // Maps to our PubSubMsg DateTime, uses Unix Epoch based UInt32 in seconds
    msmqPropIdsToReceive.push_back( PROPID_M_SENTTIME );

    // Maps to the topic name which is a MSMQ queue address
    msmqPropIdsToReceive.push_back( PROPID_M_DEST_QUEUE );

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClientTopicConfig::PopulateDefaultSendPropIds( void )
{GUCEF_TRACE;

    // Maps to our Primary payload variant
    msmqPropIdsToSend.push_back( PROPID_M_BODY ); 

    // Maps to our PubSubMsg MsgId, 20 unicode chars 
    // Note only available after send completes and is assigned by MSMQ itself
    msmqPropIdsToSend.push_back( PROPID_M_MSGID );

    // Maps to our PubSubMsg DateTime, uses Unix Epoch based UInt32 in seconds
    msmqPropIdsToSend.push_back( PROPID_M_SENTTIME );

    // Maps to the topic name which is a MSMQ queue address
    msmqPropIdsToSend.push_back( PROPID_M_DEST_QUEUE );

    return true;

}
/*--------------------------------------------------------------------------*/

void
CMsmqPubSubClientTopicConfig::PopulateDefaultPairedPropIds( void )
{GUCEF_TRACE;

    msmqPairedPropIds[ PROPID_M_BODY ].push_back( PROPID_M_BODY_SIZE );
    msmqPairedPropIds[ PROPID_M_BODY ].push_back( PROPID_M_BODY_TYPE );
    msmqPairedPropIds[ PROPID_M_DEST_QUEUE ].push_back( PROPID_M_DEST_QUEUE_LEN );
    msmqPairedPropIds[ PROPID_M_DEST_SYMM_KEY ].push_back( PROPID_M_DEST_SYMM_KEY_LEN );    
    msmqPairedPropIds[ PROPID_M_DEADLETTER_QUEUE ].push_back( PROPID_M_DEADLETTER_QUEUE_LEN );
    msmqPairedPropIds[ PROPID_M_EXTENSION ].push_back( PROPID_M_EXTENSION_LEN );
    msmqPairedPropIds[ PROPID_M_LABEL ].push_back( PROPID_M_LABEL_LEN );
    msmqPairedPropIds[ PROPID_M_PROV_NAME ].push_back( PROPID_M_PROV_NAME_LEN );
    msmqPairedPropIds[ PROPID_M_RESP_FORMAT_NAME ].push_back( PROPID_M_RESP_FORMAT_NAME_LEN );
    msmqPairedPropIds[ PROPID_M_RESP_QUEUE ].push_back( PROPID_M_RESP_QUEUE_LEN );
    msmqPairedPropIds[ PROPID_M_SENDER_CERT ].push_back( PROPID_M_SENDER_CERT_LEN );
    msmqPairedPropIds[ PROPID_M_SENDERID ].push_back( PROPID_M_SENDERID_LEN );
    msmqPairedPropIds[ PROPID_M_SENDERID ].push_back( PROPID_M_SENDERID_TYPE );
    msmqPairedPropIds[ PROPID_M_SIGNATURE ].push_back( PROPID_M_SIGNATURE_LEN );
    msmqPairedPropIds[ PROPID_M_XACT_STATUS_QUEUE ].push_back( PROPID_M_XACT_STATUS_QUEUE_LEN );

    // Introduced in MSMQ 3.0
    msmqPairedPropIds[ PROPID_M_COMPOUND_MESSAGE ].push_back( PROPID_M_COMPOUND_MESSAGE_SIZE );
    msmqPairedPropIds[ PROPID_M_DEST_FORMAT_NAME ].push_back( PROPID_M_DEST_FORMAT_NAME_LEN );
    msmqPairedPropIds[ PROPID_M_SOAP_ENVELOPE ].push_back( PROPID_M_SOAP_ENVELOPE_LEN );    
}

/*--------------------------------------------------------------------------*/

void
CMsmqPubSubClientTopicConfig::ErasePropId( PROPID propId, MSGPROPIDVector& propIds )
{GUCEF_TRACE;

    MSGPROPIDVector::iterator i = propIds.begin();
    while ( i != propIds.end() )
    {
        if ( (*i) == propId )
        {
            propIds.erase( i );
            i = propIds.begin();
        }
        ++i;
    }
}

/*--------------------------------------------------------------------------*/

bool 
CMsmqPubSubClientTopicConfig::SupplementPropIdsWithPairedPropId( MSGPROPIDVector& propIds )
{GUCEF_TRACE;
    
    // To make the config less error prone we will auto-add properties
    // which are MSMQ mandated paired properties
    // We only need to do this at config load so we use the vector to store the values for runtime performance
    // even if it makes this a bit less efficient

    MSGPROPIDVector newPropIds;

    MSGPROPIDVector::iterator i = propIds.begin();
    while ( i != propIds.end() )
    {
        MSGPROPIDMapVector::iterator n = msmqPairedPropIds.find( (*i) );
        if ( n != msmqPairedPropIds.end() )
        {                                  
            // For speed of processing later we should add the paired properties in order
            // first in the flattended list. For example read the length of the string before the
            // string itself to allow proper pre-allocation when needed
            MSGPROPIDVector& pairedProps = (*n).second;
            MSGPROPIDVector::iterator m = pairedProps.begin();
            while ( m != pairedProps.end() ) 
            {
                ErasePropId( (*m), newPropIds );
                newPropIds.push_back( (*m) );
                ++m;
            }
        }
        
        newPropIds.push_back( (*i) );
        ++i;
    }

    propIds = newPropIds;
    return true;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MSMQ */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
