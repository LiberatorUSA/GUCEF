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

#include "gucefCOMCORE_CPubSubClientFeatures.h"

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

CPubSubClientFeatures::CPubSubClientFeatures( void )
    : supportsPublishing( false )
    , supportsSubscribing( false )    
    , supportsAutoReconnect( false )
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
    , supportsBookmarkingConcept( false )
    , supportsAutoBookmarking( false )
    , supportsMsgIdBasedBookmark( false )
    , supportsMsgIndexBasedBookmark( false )
    , supportsMsgDateTimeBasedBookmark( false )
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
    tree.SetAttribute( "supportsBookmarkingConcept", supportsBookmarkingConcept );
    tree.SetAttribute( "supportsAutoBookmarking", supportsAutoBookmarking );
    tree.SetAttribute( "supportsMsgIdBasedBookmark", supportsMsgIdBasedBookmark );
    tree.SetAttribute( "supportsMsgIndexBasedBookmark", supportsMsgIndexBasedBookmark );
    tree.SetAttribute( "supportsMsgDateTimeBasedBookmark", supportsMsgDateTimeBasedBookmark );
    tree.SetAttribute( "supportsMetrics", supportsMetrics );

    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubClientFeatures::LoadConfig( const CORE::CDataNode& cfg )
{GUCEF_TRACE;

    supportsPublishing = CORE::StringToBool( CORE::ResolveVars( cfg.GetAttributeValueOrChildValueByName( "supportsPublishing" ) ), supportsPublishing );
    supportsSubscribing = CORE::StringToBool( CORE::ResolveVars( cfg.GetAttributeValueOrChildValueByName( "supportsSubscribing" ) ), supportsSubscribing );
    supportsAutoReconnect = CORE::StringToBool( CORE::ResolveVars( cfg.GetAttributeValueOrChildValueByName( "supportsAutoReconnect" ) ), supportsAutoReconnect );
    supportsBinaryPayloads = CORE::StringToBool( CORE::ResolveVars( cfg.GetAttributeValueOrChildValueByName( "supportsBinaryPayloads" ) ), supportsBinaryPayloads );
    supportsMultiHostSharding = CORE::StringToBool( CORE::ResolveVars( cfg.GetAttributeValueOrChildValueByName( "supportsMultiHostSharding" ) ), supportsMultiHostSharding );
    supportsPerMsgIds = CORE::StringToBool( CORE::ResolveVars( cfg.GetAttributeValueOrChildValueByName( "supportsPerMsgIds" ) ), supportsPerMsgIds );
    supportsMsgIndex = cfg.GetAttributeValueOrChildValueByName( "supportsMsgIndex" ).AsBool( supportsMsgIndex, true );
    supportsPrimaryPayloadPerMsg = CORE::StringToBool( CORE::ResolveVars( cfg.GetAttributeValueOrChildValueByName( "supportsPrimaryPayloadPerMsg" ) ), supportsPrimaryPayloadPerMsg );    
    supportsAbsentPrimaryPayloadPerMsg = CORE::StringToBool( CORE::ResolveVars( cfg.GetAttributeValueOrChildValueByName( "supportsAbsentPrimaryPayloadPerMsg" ) ), supportsAbsentPrimaryPayloadPerMsg );    
    supportsKeyValueSetPerMsg = CORE::StringToBool( CORE::ResolveVars( cfg.GetAttributeValueOrChildValueByName( "supportsKeyValueSetPerMsg" ) ), supportsKeyValueSetPerMsg );
    supportsDuplicateKeysPerMsg = CORE::StringToBool( CORE::ResolveVars( cfg.GetAttributeValueOrChildValueByName( "supportsDuplicateKeysPerMsg" ) ), supportsDuplicateKeysPerMsg );
    supportsMetaDataKeyValueSetPerMsg = CORE::StringToBool( CORE::ResolveVars( cfg.GetAttributeValueOrChildValueByName( "supportsMetaDataKeyValueSetPerMsg" ) ), supportsMetaDataKeyValueSetPerMsg );
    supportsSubscriberMsgReceivedAck = cfg.GetAttributeValueOrChildValueByName( "supportsSubscriberMsgReceivedAck" ).AsBool( supportsSubscriberMsgReceivedAck, true );
    supportsAutoMsgReceivedAck = cfg.GetAttributeValueOrChildValueByName( "supportsAutoMsgReceivedAck" ).AsBool( supportsAutoMsgReceivedAck, true );
    supportsAbsentMsgReceivedAck = cfg.GetAttributeValueOrChildValueByName( "supportsAbsentMsgReceivedAck" ).AsBool( supportsAbsentMsgReceivedAck, true );
    supportsBookmarkingConcept = cfg.GetAttributeValueOrChildValueByName( "supportsBookmarkingConcept" ).AsBool( supportsBookmarkingConcept, true );
    supportsAutoBookmarking = cfg.GetAttributeValueOrChildValueByName( "supportsAutoBookmarking" ).AsBool( supportsAutoBookmarking, true );
    supportsMsgIdBasedBookmark = cfg.GetAttributeValueOrChildValueByName( "supportsMsgIdBasedBookmark" ).AsBool( supportsMsgIdBasedBookmark, true );
    supportsMsgIndexBasedBookmark = cfg.GetAttributeValueOrChildValueByName( "supportsMsgIndexBasedBookmark" ).AsBool( supportsMsgIndexBasedBookmark, true );
    supportsMsgDateTimeBasedBookmark = cfg.GetAttributeValueOrChildValueByName( "supportsMsgDateTimeBasedBookmark" ).AsBool( supportsMsgDateTimeBasedBookmark, true );
    supportsMetrics = cfg.GetAttributeValueOrChildValueByName( "supportsMetrics" ).AsBool( supportsMetrics, true );

    return true;
}

/*-------------------------------------------------------------------------*/

const CString& 
CPubSubClientFeatures::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CString classTypeName = "GUCEF::COMCORE::CPubSubClientFeatures";
    return classTypeName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
