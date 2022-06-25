/*
 *  gucefPUBSUB: GUCEF module providing pub-sub communication facilities
 *
 *  Copyright (C) 1998 - 2022.  Dinand Vanvelzen
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

#include "gucefPUBSUB_CPubSubClientFeatures.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CPubSubClientFeatures::CPubSubClientFeatures( void )
    : supportsPublishing( false )
    , supportsSubscribing( false )    
    , supportsAutoReconnect( false )
    , supportsSubscriberRateThrottling( false )
    , supportsBinaryPayloads( false )
    , supportsMultiHostSharding( false )
    , supportsPerMsgIds( false )
    , supportsMsgIndex( false )
    , supportsPrimaryPayloadPerMsg( false )
    , supportsAbsentPrimaryPayloadPerMsg( false )
    , supportsKeyValueSetPerMsg( false )
    , supportsDuplicateKeysPerMsg( false )
    , supportsMetaDataKeyValueSetPerMsg( false )
    , supportsMetrics( false )
    , supportsSubscriberMsgReceivedAck( false )
    , supportsAutoMsgReceivedAck( false )
    , supportsAbsentMsgReceivedAck( false )
    , supportsAckUsingLastMsgInBatch( false )
    , supportsAckUsingBookmark( false )
    , supportsBookmarkingConcept( false )
    , supportsSubscribingUsingBookmark( false )
    , supportsServerSideBookmarkPersistance( false )
    , supportsAutoBookmarking( false )
    , supportsMsgIdBasedBookmark( false )
    , supportsMsgIndexBasedBookmark( false )
    , supportsTopicIndexBasedBookmark( false )
    , supportsMsgDateTimeBasedBookmark( false )
    , supportsDerivingBookmarkFromMsg( false )
    , supportsDiscoveryOfAvailableTopics( false )
    , supportsGlobPatternTopicNames( false )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubClientFeatures::~CPubSubClientFeatures()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool 
CPubSubClientFeatures::SaveConfig( CORE::CDataNode& tree ) const
{GUCEF_TRACE;

    tree.SetAttribute( "supportsPublishing", supportsPublishing );
    tree.SetAttribute( "supportsSubscribing", supportsSubscribing );
    tree.SetAttribute( "supportsAutoReconnect", supportsAutoReconnect );
    tree.SetAttribute( "supportsSubscriberRateThrottling", supportsSubscriberRateThrottling );    
    tree.SetAttribute( "supportsBinaryPayloads", supportsBinaryPayloads );
    tree.SetAttribute( "supportsMultiHostSharding", supportsMultiHostSharding );
    tree.SetAttribute( "supportsPerMsgIds", supportsPerMsgIds );
    tree.SetAttribute( "supportsMsgIndex", supportsMsgIndex );
    tree.SetAttribute( "supportsPrimaryPayloadPerMsg", supportsPrimaryPayloadPerMsg );
    tree.SetAttribute( "supportsAbsentPrimaryPayloadPerMsg", supportsAbsentPrimaryPayloadPerMsg );
    tree.SetAttribute( "supportsKeyValueSetPerMsg", supportsKeyValueSetPerMsg );
    tree.SetAttribute( "supportsDuplicateKeysPerMsg", supportsDuplicateKeysPerMsg );
    tree.SetAttribute( "supportsMetaDataKeyValueSetPerMsg", supportsMetaDataKeyValueSetPerMsg );
    tree.SetAttribute( "supportsSubscriberMsgReceivedAck", supportsSubscriberMsgReceivedAck );
    tree.SetAttribute( "supportsAutoMsgReceivedAck", supportsAutoMsgReceivedAck );
    tree.SetAttribute( "supportsAbsentMsgReceivedAck", supportsAbsentMsgReceivedAck );
    tree.SetAttribute( "supportsAckUsingLastMsgInBatch", supportsAckUsingLastMsgInBatch );
    tree.SetAttribute( "supportsAckUsingBookmark", supportsAckUsingBookmark );
    tree.SetAttribute( "supportsBookmarkingConcept", supportsBookmarkingConcept );
    tree.SetAttribute( "supportsSubscribingUsingBookmark", supportsSubscribingUsingBookmark );
    tree.SetAttribute( "supportsServerSideBookmarkPersistance", supportsServerSideBookmarkPersistance );
    tree.SetAttribute( "supportsAutoBookmarking", supportsAutoBookmarking );
    tree.SetAttribute( "supportsMsgIdBasedBookmark", supportsMsgIdBasedBookmark );
    tree.SetAttribute( "supportsMsgIndexBasedBookmark", supportsMsgIndexBasedBookmark );
    tree.SetAttribute( "supportsTopicIndexBasedBookmark", supportsTopicIndexBasedBookmark );    
    tree.SetAttribute( "supportsMsgDateTimeBasedBookmark", supportsMsgDateTimeBasedBookmark );
    tree.SetAttribute( "supportsDerivingBookmarkFromMsg", supportsDerivingBookmarkFromMsg );    
    tree.SetAttribute( "supportsDiscoveryOfAvailableTopics", supportsDiscoveryOfAvailableTopics );    
    tree.SetAttribute( "supportsGlobPatternTopicNames", supportsGlobPatternTopicNames );    
    tree.SetAttribute( "supportsMetrics", supportsMetrics );

    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubClientFeatures::LoadConfig( const CORE::CDataNode& cfg )
{GUCEF_TRACE;

    supportsPublishing = cfg.GetAttributeValueOrChildValueByName( "supportsPublishing" ).AsBool( supportsPublishing, true );
    supportsSubscribing = cfg.GetAttributeValueOrChildValueByName( "supportsSubscribing" ).AsBool( supportsSubscribing, true );
    supportsAutoReconnect = cfg.GetAttributeValueOrChildValueByName( "supportsAutoReconnect" ).AsBool( supportsAutoReconnect, true );
    supportsSubscriberRateThrottling = cfg.GetAttributeValueOrChildValueByName( "supportsSubscriberRateThrottling" ).AsBool( supportsSubscriberRateThrottling, true );
    supportsBinaryPayloads = cfg.GetAttributeValueOrChildValueByName( "supportsBinaryPayloads" ).AsBool( supportsBinaryPayloads, true );
    supportsMultiHostSharding = cfg.GetAttributeValueOrChildValueByName( "supportsMultiHostSharding" ).AsBool( supportsMultiHostSharding, true );
    supportsPerMsgIds = cfg.GetAttributeValueOrChildValueByName( "supportsPerMsgIds" ).AsBool( supportsPerMsgIds, true );
    supportsMsgIndex = cfg.GetAttributeValueOrChildValueByName( "supportsMsgIndex" ).AsBool( supportsMsgIndex, true );
    supportsPrimaryPayloadPerMsg = cfg.GetAttributeValueOrChildValueByName( "supportsPrimaryPayloadPerMsg" ).AsBool( supportsPrimaryPayloadPerMsg, true );    
    supportsAbsentPrimaryPayloadPerMsg = cfg.GetAttributeValueOrChildValueByName( "supportsAbsentPrimaryPayloadPerMsg" ).AsBool( supportsAbsentPrimaryPayloadPerMsg, true );    
    supportsKeyValueSetPerMsg = cfg.GetAttributeValueOrChildValueByName( "supportsKeyValueSetPerMsg" ).AsBool( supportsKeyValueSetPerMsg, true );
    supportsDuplicateKeysPerMsg = cfg.GetAttributeValueOrChildValueByName( "supportsDuplicateKeysPerMsg" ).AsBool( supportsDuplicateKeysPerMsg, true );
    supportsMetaDataKeyValueSetPerMsg = cfg.GetAttributeValueOrChildValueByName( "supportsMetaDataKeyValueSetPerMsg" ).AsBool( supportsMetaDataKeyValueSetPerMsg, true );
    supportsSubscriberMsgReceivedAck = cfg.GetAttributeValueOrChildValueByName( "supportsSubscriberMsgReceivedAck" ).AsBool( supportsSubscriberMsgReceivedAck, true );
    supportsAutoMsgReceivedAck = cfg.GetAttributeValueOrChildValueByName( "supportsAutoMsgReceivedAck" ).AsBool( supportsAutoMsgReceivedAck, true );
    supportsAbsentMsgReceivedAck = cfg.GetAttributeValueOrChildValueByName( "supportsAbsentMsgReceivedAck" ).AsBool( supportsAbsentMsgReceivedAck, true );
    supportsAckUsingLastMsgInBatch = cfg.GetAttributeValueOrChildValueByName( "supportsAckUsingLastMsgInBatch" ).AsBool( supportsAckUsingLastMsgInBatch, true );
    supportsAckUsingBookmark = cfg.GetAttributeValueOrChildValueByName( "supportsAckUsingBookmark" ).AsBool( supportsAckUsingBookmark, true );
    supportsBookmarkingConcept = cfg.GetAttributeValueOrChildValueByName( "supportsBookmarkingConcept" ).AsBool( supportsBookmarkingConcept, true );
    supportsSubscribingUsingBookmark = cfg.GetAttributeValueOrChildValueByName( "supportsSubscribingUsingBookmark" ).AsBool( supportsSubscribingUsingBookmark, true );
    supportsServerSideBookmarkPersistance = cfg.GetAttributeValueOrChildValueByName( "supportsServerSideBookmarkPersistance" ).AsBool( supportsServerSideBookmarkPersistance, true );
    supportsAutoBookmarking = cfg.GetAttributeValueOrChildValueByName( "supportsAutoBookmarking" ).AsBool( supportsAutoBookmarking, true );
    supportsMsgIdBasedBookmark = cfg.GetAttributeValueOrChildValueByName( "supportsMsgIdBasedBookmark" ).AsBool( supportsMsgIdBasedBookmark, true );
    supportsMsgIndexBasedBookmark = cfg.GetAttributeValueOrChildValueByName( "supportsMsgIndexBasedBookmark" ).AsBool( supportsMsgIndexBasedBookmark, true );
    supportsTopicIndexBasedBookmark = cfg.GetAttributeValueOrChildValueByName( "supportsTopicIndexBasedBookmark" ).AsBool( supportsTopicIndexBasedBookmark, true );
    supportsMsgDateTimeBasedBookmark = cfg.GetAttributeValueOrChildValueByName( "supportsMsgDateTimeBasedBookmark" ).AsBool( supportsMsgDateTimeBasedBookmark, true );
    supportsDerivingBookmarkFromMsg = cfg.GetAttributeValueOrChildValueByName( "supportsDerivingBookmarkFromMsg" ).AsBool( supportsDerivingBookmarkFromMsg, true );
    supportsDiscoveryOfAvailableTopics = cfg.GetAttributeValueOrChildValueByName( "supportsDiscoveryOfAvailableTopics" ).AsBool( supportsDiscoveryOfAvailableTopics, true );    
    supportsGlobPatternTopicNames = cfg.GetAttributeValueOrChildValueByName( "supportsGlobPatternTopicNames" ).AsBool( supportsGlobPatternTopicNames, true );    
    supportsMetrics = cfg.GetAttributeValueOrChildValueByName( "supportsMetrics" ).AsBool( supportsMetrics, true );

    return true;
}

/*-------------------------------------------------------------------------*/

const CString& 
CPubSubClientFeatures::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CString classTypeName = "GUCEF::PUBSUB::CPubSubClientFeatures";
    return classTypeName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PUBSUB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
