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

#ifndef GUCEF_CORE_CVARIANTBINARYSERIALIZER_H
#include "gucefCORE_CVariantBinarySerializer.h"
#define GUCEF_CORE_CVARIANTBINARYSERIALIZER_H
#endif /* GUCEF_CORE_CVARIANTBINARYSERIALIZER_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#ifndef GUCEF_CORE_CDSTORECODECREGISTRY_H
#include "CDStoreCodecRegistry.h"
#define GUCEF_CORE_CDSTORECODECREGISTRY_H
#endif /* GUCEF_CORE_CDSTORECODECREGISTRY_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#include "CDynamicBufferAccess.h"
#define GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#endif /* GUCEF_CORE_CDYNAMICBUFFERACCESS_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_COMCORE_CPUBSUBCLIENTFEATURES_H
#include "gucefCOMCORE_CPubSubClientFeatures.h"
#define GUCEF_COMCORE_CPUBSUBCLIENTFEATURES_H
#endif /* GUCEF_COMCORE_CPUBSUBCLIENTFEATURES_H ? */

#ifndef GUCEF_COMCORE_CPUBSUBCLIENTTOPIC_H
#include "gucefCOMCORE_CPubSubClientTopic.h"
#define GUCEF_COMCORE_CPUBSUBCLIENTTOPIC_H
#endif /* GUCEF_COMCORE_CPUBSUBCLIENTTOPIC_H ? */

#ifndef GUCEF_COMCORE_CPUBSUBCLIENT_H
#include "gucefCOMCORE_CPubSubClient.h"
#define GUCEF_COMCORE_CPUBSUBCLIENT_H
#endif /* GUCEF_COMCORE_CPUBSUBCLIENT_H ? */

#include "gucefCOMCORE_CPubSubMsgSerializer.h"

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

CPubSubMsgSerializerOptions::CPubSubMsgSerializerOptions( void )
    : CMessageSerializerOptions()
    , receiveActionIdIncluded( true )
    , originTopicObjPointerIncluded( false )
    , originTopicNameIncluded( false )
    , originClientTypeNameIncluded( false )
    , originClientObjPointerIncluded( false )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubMsgSerializerOptions::~CPubSubMsgSerializerOptions()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CPubSubMsgSerializerOptions::SaveConfig( CORE::CDataNode& config ) const
{GUCEF_TRACE;

    bool success = CMessageSerializerOptions::SaveConfig( config );
    
    config.SetAttribute( "receiveActionIdIncluded", receiveActionIdIncluded );
    config.SetAttribute( "originTopicObjPointerIncluded", originTopicObjPointerIncluded );
    config.SetAttribute( "originTopicNameIncluded", originTopicNameIncluded );
    config.SetAttribute( "originClientTypeNameIncluded", originClientTypeNameIncluded );
    config.SetAttribute( "originClientObjPointerIncluded", originClientObjPointerIncluded );
    
    return success;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubMsgSerializerOptions::LoadConfig( const CORE::CDataNode& config )
{GUCEF_TRACE;

    receiveActionIdIncluded = config.GetAttributeValueOrChildValueByName( "receiveActionIdIncluded" ).AsBool( receiveActionIdIncluded, true );
    originTopicObjPointerIncluded = config.GetAttributeValueOrChildValueByName( "originTopicObjPointerIncluded" ).AsBool( originTopicObjPointerIncluded, true );
    originTopicNameIncluded = config.GetAttributeValueOrChildValueByName( "originTopicNameIncluded" ).AsBool( originTopicNameIncluded, true );
    originClientTypeNameIncluded = config.GetAttributeValueOrChildValueByName( "originClientTypeNameIncluded" ).AsBool( originClientTypeNameIncluded, true );
    originClientObjPointerIncluded = config.GetAttributeValueOrChildValueByName( "originClientObjPointerIncluded" ).AsBool( originClientObjPointerIncluded, true );

    return true;
}

/*-------------------------------------------------------------------------*/

const CString& 
CPubSubMsgSerializerOptions::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CString classTypeName = "GUCEF::CORE::CPubSubMsgSerializerOptions";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

UInt32 
CPubSubMsgSerializerOptions::ToOptionsBitMask( void ) const
{GUCEF_TRACE;

    UInt32 bitMask = CMessageSerializerOptions::ToOptionsBitMask();
    receiveActionIdIncluded ? GUCEF_SETBITXON( bitMask, 20 ) : GUCEF_SETBITXOFF( bitMask, 20 );
    originTopicObjPointerIncluded ? GUCEF_SETBITXON( bitMask, 21 ) : GUCEF_SETBITXOFF( bitMask, 21 );
    originTopicNameIncluded ? GUCEF_SETBITXON( bitMask, 22 ) : GUCEF_SETBITXOFF( bitMask, 22 );
    originClientTypeNameIncluded ? GUCEF_SETBITXON( bitMask, 23 ) : GUCEF_SETBITXOFF( bitMask, 23 );
    originClientObjPointerIncluded ? GUCEF_SETBITXON( bitMask, 24 ) : GUCEF_SETBITXOFF( bitMask, 24 );
    return bitMask;
}

/*-------------------------------------------------------------------------*/

void 
CPubSubMsgSerializerOptions::FromOptionsBitMask( UInt32 bitMask )
{GUCEF_TRACE;

    CMessageSerializerOptions::FromOptionsBitMask( bitMask );

    receiveActionIdIncluded = ( 0 != GUCEF_GETBITX( bitMask, 20 ) );
    originTopicObjPointerIncluded = ( 0 != GUCEF_GETBITX( bitMask, 21 ) );
    originTopicNameIncluded = ( 0 != GUCEF_GETBITX( bitMask, 22 ) );
    originClientTypeNameIncluded = ( 0 != GUCEF_GETBITX( bitMask, 23 ) );
    originClientObjPointerIncluded = ( 0 != GUCEF_GETBITX( bitMask, 24 ) );
}

/*-------------------------------------------------------------------------*/

void 
CPubSubMsgSerializerOptions::FromPubSubClientFeatures( const CPubSubClientFeatures& features )
{GUCEF_TRACE;

    // Set MessageSerializerOptions which has no concept of pubsub specifics
    msgDateTimeIncluded = features.supportsMsgDateTimeBasedBookmark;
    msgDateTimeAsMsSinceUnixEpochInUtc = true;
    msgIdIncluded = features.supportsPerMsgIds;
    msgIndexIncluded = features.supportsMsgIndex;
    msgPrimaryPayloadIncluded = features.supportsPrimaryPayloadPerMsg;
    msgKeyValuePairsIncluded = features.supportsKeyValueSetPerMsg;
    msgMetaDataKeyValuePairsIncluded = features.supportsMetaDataKeyValueSetPerMsg;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubMsgSerializer::Serialize( const CPubSubMsgSerializerOptions& options ,
                                 const CIPubSubMsg& msg                     ,
                                 CORE::CDataNode& output                    )
{GUCEF_TRACE;
    
    bool success = true;
    try
    {
         success = CMessageSerializer::Serialize( options ,
                                                  msg     ,
                                                  output  );
        
        if ( options.receiveActionIdIncluded )
        {
            success = output.SetAttribute( "receiveActionId", msg.GetReceiveActionId() ) && success;
        }
    
        const COMCORE::CPubSubClientTopic* topic = msg.GetOriginClientTopic();
        if ( GUCEF_NULL != topic )
        {
            if ( options.originTopicNameIncluded )
            {
                success = output.SetAttribute( "originTopicName", topic->GetTopicName() ) && success;
            }
            if ( options.originTopicObjPointerIncluded )
            {
                success = output.SetAttribute( "originTopicObj", CORE::ToString( topic ) ) && success;
            }
            
            const CPubSubClient* pubsubClient = topic->GetClient();
            if ( GUCEF_NULL != pubsubClient )
            {
                if ( options.originClientTypeNameIncluded )
                {
                    success = output.SetAttribute( "originClientTypeName", pubsubClient->GetType() ) && success;
                }
                if ( options.originClientObjPointerIncluded )
                {
                    success = output.SetAttribute( "originClientObj", CORE::ToString( pubsubClient ) ) && success;
                }
            }
            else
            {
                if ( options.originClientTypeNameIncluded )
                {
                    success = output.SetAttribute( "originClientTypeName", CString::Empty ) && success;
                }
                if ( options.originClientObjPointerIncluded )
                {
                    success = output.SetAttribute( "originClientObj", CORE::ToString( (void*)GUCEF_NULL ) ) && success;
                }
            }
        }
        else
        {
            if ( options.originTopicNameIncluded )
            {
                success = output.SetAttribute( "originTopicName", CString::Empty ) && success;
            }
            if ( options.originTopicObjPointerIncluded )
            {
                success = output.SetAttribute( "originTopicObj", CORE::ToString( (void*)GUCEF_NULL ) ) && success;
            }
            if ( options.originClientTypeNameIncluded )
            {
                success = output.SetAttribute( "originClientTypeName", CString::Empty ) && success;
            }
            if ( options.originClientObjPointerIncluded )
            {
                success = output.SetAttribute( "originClientObj", CORE::ToString( (void*)GUCEF_NULL ) ) && success;
            }            
        }
    }   
    catch ( const std::exception& e )    
    {
        success = false;
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_NORMAL, CString( "PubSubMsgSerializer:Serialize: caught exception: " ) + e.what()  );        
    }
    return success;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubMsgSerializer::Serialize( const CPubSubMsgSerializerOptions& options ,
                                 const CIPubSubMsg& msg                     , 
                                 CORE::CDynamicBuffer& target               ,
                                 const CORE::CString& dstoreCodecToUse      )
{GUCEF_TRACE;
    
    try
    {
        CORE::CDataNode nodes;
        if ( Serialize( options, msg, nodes ) )
        {
            CORE::CDStoreCodecRegistry::TDStoreCodecPtr codec;
            if ( CORE::CCoreGlobal::Instance()->GetDStoreCodecRegistry().TryLookup( dstoreCodecToUse, codec, false ) && !codec.IsNULL() )
            {
                CORE::CDynamicBufferAccess bufferIoAccess( target );
                if ( codec->StoreDataTree( &nodes, &bufferIoAccess ) )
                    return true;
            }
        }
    }   
    catch ( const std::exception& e )    
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_NORMAL, CString( "PubSubMsgSerializer:Serialize: caught exception: " ) + e.what()  );        
    } 
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubMsgSerializer::Deserialize( const CPubSubMsgSerializerOptions& options ,
                                   CIPubSubMsg& msg                           ,
                                   const CORE::CDataNode& input               )
{GUCEF_TRACE;

    bool success = true;
    try
    {
         success = CMessageSerializer::Deserialize( options ,
                                                    msg     ,
                                                    input   );
        

    }   
    catch ( const std::exception& e )    
    {
        success = false;
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_NORMAL, CString( "PubSubMsgSerializer:Deserialize: caught exception: " ) + e.what()  );        
    }
    return success;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubMsgSerializer::Deserialize( const CPubSubMsgSerializerOptions& options ,
                                   CIPubSubMsg& msg                           ,
                                   const CORE::CDynamicBuffer& inputBuffer    ,
                                   const CORE::CString& dstoreCodecToUse      )
{GUCEF_TRACE;

    try
    {
        CORE::CDStoreCodecRegistry::TDStoreCodecPtr codec;
        if ( CORE::CCoreGlobal::Instance()->GetDStoreCodecRegistry().TryLookup( dstoreCodecToUse, codec, false ) && !codec.IsNULL() )
        {
            CORE::CDataNode nodes;
            CORE::CDynamicBufferAccess bufferIoAccess( inputBuffer );
            if ( codec->BuildDataTree( &nodes, &bufferIoAccess ) )
            {
                if ( Deserialize( options, msg, nodes ) )
                {
                    return true;
                }
            }
        }
    }   
    catch ( const std::exception& e )    
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_NORMAL, CString( "PubSubMsgSerializer:Serialize: caught exception: " ) + e.what()  );        
    } 
    return false;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
