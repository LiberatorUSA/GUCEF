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

#include "gucefCOMCORE_CMessageSerializer.h"

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

CMessageSerializerOptions::CMessageSerializerOptions( void )
    : msgIdIncluded( true )
    , msgIndexIncluded( true )
    , msgDateTimeIncluded( true )
    , msgDateTimeAsMsSinceUnixEpochInUtc( true )
    , msgPrimaryPayloadIncluded( true )
    , msgKeyValuePairsIncluded( true )
    , msgMetaDataKeyValuePairsIncluded( true )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CMessageSerializerOptions::~CMessageSerializerOptions()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CMessageSerializerOptions::SaveConfig( CORE::CDataNode& config ) const
{GUCEF_TRACE;

    config.SetAttribute( "msgDateTimeIncluded", msgDateTimeIncluded );
    config.SetAttribute( "msgDateTimeAsMsSinceUnixEpochInUtc", msgDateTimeAsMsSinceUnixEpochInUtc );
    config.SetAttribute( "msgIdIncluded", msgIdIncluded );
    config.SetAttribute( "msgIndexIncluded", msgIndexIncluded );
    config.SetAttribute( "msgPrimaryPayloadIncluded", msgPrimaryPayloadIncluded );
    config.SetAttribute( "msgKeyValuePairsIncluded", msgKeyValuePairsIncluded );
    config.SetAttribute( "msgMetaDataKeyValuePairsIncluded", msgMetaDataKeyValuePairsIncluded );
    
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CMessageSerializerOptions::LoadConfig( const CORE::CDataNode& config )
{GUCEF_TRACE;

    msgDateTimeIncluded = config.GetAttributeValueOrChildValueByName( "msgDateTimeIncluded" ).AsBool( msgDateTimeIncluded, true );
    msgDateTimeAsMsSinceUnixEpochInUtc = config.GetAttributeValueOrChildValueByName( "msgDateTimeAsMsSinceUnixEpochInUtc" ).AsBool( msgDateTimeAsMsSinceUnixEpochInUtc, true );
    msgIdIncluded = config.GetAttributeValueOrChildValueByName( "msgIdIncluded" ).AsBool( msgIdIncluded, true );
    msgIndexIncluded = config.GetAttributeValueOrChildValueByName( "msgIndexIncluded" ).AsBool( msgIndexIncluded, true );
    msgPrimaryPayloadIncluded = config.GetAttributeValueOrChildValueByName( "msgPrimaryPayloadIncluded" ).AsBool( msgPrimaryPayloadIncluded, true );
    msgKeyValuePairsIncluded = config.GetAttributeValueOrChildValueByName( "msgKeyValuePairsIncluded" ).AsBool( msgKeyValuePairsIncluded, true );
    msgMetaDataKeyValuePairsIncluded = config.GetAttributeValueOrChildValueByName( "msgMetaDataKeyValuePairsIncluded" ).AsBool( msgMetaDataKeyValuePairsIncluded, true );
    return true;
}

/*-------------------------------------------------------------------------*/

const CString& 
CMessageSerializerOptions::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CString classTypeName = "GUCEF::CORE::CMessageSerializerOptions";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

UInt32 
CMessageSerializerOptions::ToOptionsBitMask( void ) const
{GUCEF_TRACE;

    UInt32 bitMask = 0;
    msgDateTimeIncluded ? GUCEF_SETBITXON( bitMask, 1 ) : GUCEF_SETBITXOFF( bitMask, 1 );
    msgDateTimeAsMsSinceUnixEpochInUtc ? GUCEF_SETBITXON( bitMask, 2 ) : GUCEF_SETBITXOFF( bitMask, 2 );
    msgIdIncluded ? GUCEF_SETBITXON( bitMask, 3 ) : GUCEF_SETBITXOFF( bitMask, 3 );
    msgIndexIncluded ? GUCEF_SETBITXON( bitMask, 4 ) : GUCEF_SETBITXOFF( bitMask, 4 );
    msgPrimaryPayloadIncluded ? GUCEF_SETBITXON( bitMask, 5 ) : GUCEF_SETBITXOFF( bitMask, 5 );
    msgKeyValuePairsIncluded ? GUCEF_SETBITXON( bitMask, 6 ) : GUCEF_SETBITXOFF( bitMask, 6 );
    msgMetaDataKeyValuePairsIncluded ? GUCEF_SETBITXON( bitMask, 7 ) : GUCEF_SETBITXOFF( bitMask, 7 );
    return bitMask;
}

/*-------------------------------------------------------------------------*/

void 
CMessageSerializerOptions::FromOptionsBitMask( UInt32 bitMask )
{GUCEF_TRACE;

    msgDateTimeIncluded = ( 0 != GUCEF_GETBITX( bitMask, 1 ) );
    msgDateTimeAsMsSinceUnixEpochInUtc = ( 0 != GUCEF_GETBITX( bitMask, 2 ) );
    msgIdIncluded = ( 0 != GUCEF_GETBITX( bitMask, 3 ) );
    msgIndexIncluded = ( 0 != GUCEF_GETBITX( bitMask, 4 ) );
    msgPrimaryPayloadIncluded = ( 0 != GUCEF_GETBITX( bitMask, 5 ) );
    msgKeyValuePairsIncluded = ( 0 != GUCEF_GETBITX( bitMask, 6 ) );
    msgMetaDataKeyValuePairsIncluded = ( 0 != GUCEF_GETBITX( bitMask, 7 ) );
}

/*-------------------------------------------------------------------------*/

bool
CMessageSerializer::SerializeKvPairs( const CIMessage::TKeyValuePairs& kvPairs ,
                                      CORE::CDataNode& output                  )
{GUCEF_TRACE;

    bool success = true;
    CIMessage::TKeyValuePairs::const_iterator i = kvPairs.begin();
    while ( i != kvPairs.end() )
    {
        success = output.AddChildWithValue( (*i).first.AsString(), (*i).second ) && success;
        ++i;
    }
    return success;
}

/*-------------------------------------------------------------------------*/

bool
CMessageSerializer::Serialize( const CMessageSerializerOptions& options ,
                               const CIMessage& msg                     ,
                               CORE::CDataNode& output                  )
{GUCEF_TRACE;

    bool success = true;
    try
    {
        output.Clear();
        output.SetNodeType( GUCEF_DATATYPE_OBJECT );
        output.SetName( "Message" );

        if ( options.msgDateTimeIncluded )
        {
            if ( options.msgDateTimeAsMsSinceUnixEpochInUtc )
            {
                // Write the DateTime of the msg as milliseconds since Unix Epoch in UTC
                // In most applications Unix Epoch is not UTC and leap seconds are not taken into account
                // However since the source is of higher fidelity we can retain accuracy as long as the same method is
                // used to deserialize as well. There is a problem with any DateTime which predates Unix epoch
                // If you have to deal with those use string formatting instead
                UInt64 msSinceUnixEpochInUtc = msg.GetMsgDateTime().ToUnixEpochBasedTicksInMillisecs();
                success = output.SetAttribute( "msgDateTimeAsMsSinceUnixEpochInUtc", msSinceUnixEpochInUtc ) && success;
            }
            else
            {
                // Write the DateTime of the msg which as a ISO 8601 spec string
                // Writing it in string form actually takes up less space vs writing all the binary components at full fidelity
                // Using Unix epoch uses less space but we are limited in the time range since historical dates would be a problem 
                CORE::CString dtStr = msg.GetMsgDateTime().ToIso8601DateTimeString( false, true );
                success = output.SetAttribute( "msgDateTimeAsIso8601", dtStr ) && success;
            }
        }

        if ( options.msgIdIncluded )
        {
            success = output.SetAttribute( "msgId", msg.GetMsgId() ) && success;
        }

        if ( options.msgIndexIncluded )
        {
            success = output.SetAttribute( "msgIndex", msg.GetMsgIndex() ) && success;
        }

        if ( options.msgPrimaryPayloadIncluded )
        {
            success = output.AddChildWithValue( "primaryPayload", msg.GetPrimaryPayload() ) && success;
        }

        if ( options.msgKeyValuePairsIncluded )
        {
            const CIMessage::TKeyValuePairs& kvPairs = msg.GetKeyValuePairs();            
            CORE::CDataNode* kvPairsNode = output.AddChild( "keyValuePairs", GUCEF_DATATYPE_ARRAY );
            success = SerializeKvPairs( kvPairs, *kvPairsNode ) && success;
        }

        if ( options.msgMetaDataKeyValuePairsIncluded )
        {
            const CIMessage::TKeyValuePairs& kvPairs = msg.GetKeyValuePairs();            
            CORE::CDataNode* kvPairsNode = output.AddChild( "metaDataKeyValuePairs", GUCEF_DATATYPE_ARRAY );
            success = SerializeKvPairs( kvPairs, *kvPairsNode ) && success;
        }

    }   
    catch ( const std::exception& e )    
    {
        success = false;
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_NORMAL, CString( "MessageSerializer:Serialize: caught exception: " ) + e.what()  );        
    }
    return success;
}

/*-------------------------------------------------------------------------*/

bool
CMessageSerializer::Serialize( const CMessageSerializerOptions& options ,
                               const CIMessage& msg                     ,
                               CORE::CDynamicBuffer& outputBuffer       ,
                               const CORE::CString& dstoreCodecToUse    )
{GUCEF_TRACE;
    
    try
    {
        CORE::CDataNode nodes;
        if ( Serialize( options, msg, nodes ) )
        {
            CORE::CDStoreCodecRegistry::TDStoreCodecPtr codec;
            if ( CORE::CCoreGlobal::Instance()->GetDStoreCodecRegistry().TryLookup( dstoreCodecToUse, codec, false ) && !codec.IsNULL() )
            {
                CORE::CDynamicBufferAccess bufferIoAccess( outputBuffer );
                if ( codec->StoreDataTree( &nodes, &bufferIoAccess ) )
                    return true;
            }
        }
    }   
    catch ( const std::exception& e )    
    {
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_NORMAL, CString( "MessageSerializer:Serialize: caught exception: " ) + e.what()  );        
    } 
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CMessageSerializer::Deserialize( const CMessageSerializerOptions& options ,
                                 CIMessage& msg                           ,
                                 const CORE::CDataNode& input             )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CMessageSerializer::Deserialize( const CMessageSerializerOptions& options ,
                                 CIMessage& msg                           ,
                                 const CORE::CDynamicBuffer& inputBuffer  ,
                                 const CORE::CString& dstoreCodecToUse    )
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
        GUCEF_EXCEPTION_LOG( CORE::LOGLEVEL_NORMAL, CString( "MessageSerializer:Serialize: caught exception: " ) + e.what()  );        
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
