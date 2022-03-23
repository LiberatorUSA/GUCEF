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

#ifndef GUCEF_COMCORE_CPUBSUBMSGBINARYSERIALIZER_H
#include "gucefCOMCORE_CPubSubMsgSerializer.h"
#define GUCEF_COMCORE_CPUBSUBMSGBINARYSERIALIZER_H
#endif /* GUCEF_COMCORE_CPUBSUBMSGBINARYSERIALIZER_H ? */

#include "gucefCOMCORE_CBasicPubSubMsg.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CString CBasicPubSubMsg::ClassTypeName = "GUCEF::COMCORE::CBasicPubSubMsg";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CBasicPubSubMsg::CBasicPubSubMsg( void )
    : COMCORE::CIPubSubMsg()
    , m_msgId()
    , m_msgIndex()
    , m_msgDateTime()
    , m_primaryPayload()
    , m_keyValuePairs()
    , m_metaDataKeyValuePairs()
    , m_msgOriginClientTopic( GUCEF_NULL )
    , m_receiveActionId( 0 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CBasicPubSubMsg::CBasicPubSubMsg( const CBasicPubSubMsg& src )
    : COMCORE::CIPubSubMsg( src )
    , m_msgId( src.m_msgId )
    , m_msgIndex( src.m_msgIndex )
    , m_msgDateTime( src.m_msgDateTime )
    , m_primaryPayload( src.m_primaryPayload )
    , m_keyValuePairs( src.m_keyValuePairs )
    , m_metaDataKeyValuePairs( src.m_metaDataKeyValuePairs )
    , m_msgOriginClientTopic( src.m_msgOriginClientTopic )
    , m_receiveActionId( src.m_receiveActionId )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CBasicPubSubMsg::~CBasicPubSubMsg()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

const CORE::CVariant& 
CBasicPubSubMsg::GetMsgId( void ) const
{GUCEF_TRACE;

    return m_msgId;
}

/*-------------------------------------------------------------------------*/

CORE::CVariant& 
CBasicPubSubMsg::GetMsgId( void )
{GUCEF_TRACE;

    return m_msgId;
}

/*-------------------------------------------------------------------------*/

const CORE::CVariant& 
CBasicPubSubMsg::GetMsgIndex( void ) const
{GUCEF_TRACE;

    return m_msgIndex;
}

/*-------------------------------------------------------------------------*/

CORE::CVariant& 
CBasicPubSubMsg::GetMsgIndex( void )
{GUCEF_TRACE;

    return m_msgIndex;
}

/*-------------------------------------------------------------------------*/

const CORE::CDateTime& 
CBasicPubSubMsg::GetMsgDateTime( void ) const
{GUCEF_TRACE;

    return m_msgDateTime;
}

/*-------------------------------------------------------------------------*/

CORE::CDateTime& 
CBasicPubSubMsg::GetMsgDateTime( void )
{GUCEF_TRACE;

    return m_msgDateTime;
}

/*-------------------------------------------------------------------------*/

const CORE::CVariant& 
CBasicPubSubMsg::GetPrimaryPayload( void ) const
{GUCEF_TRACE;

    return m_primaryPayload;
}

/*-------------------------------------------------------------------------*/

CORE::CVariant& 
CBasicPubSubMsg::GetPrimaryPayload( void )
{GUCEF_TRACE;

    return m_primaryPayload;
}

/*-------------------------------------------------------------------------*/

const CBasicPubSubMsg::TKeyValuePairs& 
CBasicPubSubMsg::GetKeyValuePairs( void ) const
{GUCEF_TRACE;

    return m_keyValuePairs;
}

/*-------------------------------------------------------------------------*/

CBasicPubSubMsg::TKeyValuePairs& 
CBasicPubSubMsg::GetKeyValuePairs( void )
{GUCEF_TRACE;

    return m_keyValuePairs;
}

/*-------------------------------------------------------------------------*/

const CBasicPubSubMsg::TKeyValuePairs& 
CBasicPubSubMsg::GetMetaDataKeyValuePairs( void ) const
{GUCEF_TRACE;

    return m_metaDataKeyValuePairs;
}

/*-------------------------------------------------------------------------*/

CBasicPubSubMsg::TKeyValuePairs& 
CBasicPubSubMsg::GetMetaDataKeyValuePairs( void )
{GUCEF_TRACE;

    return m_metaDataKeyValuePairs;
}

/*-------------------------------------------------------------------------*/

CORE::CICloneable* 
CBasicPubSubMsg::Clone( void ) const
{GUCEF_TRACE;

    return new CBasicPubSubMsg( *this );
}

/*-------------------------------------------------------------------------*/

bool 
CBasicPubSubMsg::Serialize( CORE::CDataNode& domRootNode                        ,
                            const CORE::CDataNodeSerializableSettings& settings ) const
{GUCEF_TRACE;

    return CPubSubMsgSerializer::Serialize( settings, *this, domRootNode );
}

/*-------------------------------------------------------------------------*/

bool 
CBasicPubSubMsg::Deserialize( const CORE::CDataNode& domRootNode                  ,
                              const CORE::CDataNodeSerializableSettings& settings ) 
{GUCEF_TRACE;

    return CPubSubMsgSerializer::Deserialize( settings, *this, domRootNode );
}

/*-------------------------------------------------------------------------*/

const CString& 
CBasicPubSubMsg::GetClassTypeName( void ) const
{GUCEF_TRACE;

    return ClassTypeName;
}

/*-------------------------------------------------------------------------*/

void 
CBasicPubSubMsg::Clear( void )
{GUCEF_TRACE;

    m_msgId.Clear();
    m_msgDateTime.Clear();
    m_primaryPayload.Clear();
    m_keyValuePairs.clear();    
    m_metaDataKeyValuePairs.clear(); 
    m_msgOriginClientTopic = GUCEF_NULL;
    m_receiveActionId = 0;
}

/*-------------------------------------------------------------------------*/

bool
CBasicPubSubMsg::AddLinkedKeyValuePair( const TKeyValuePair& kvPair, TKeyValuePairs& kvPairsStorage )
{GUCEF_TRACE;

    kvPairsStorage.push_back( TKeyValuePair() );
    kvPairsStorage.back().first.LinkTo( kvPair.first );
    kvPairsStorage.back().second.LinkTo( kvPair.second );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CBasicPubSubMsg::AddLinkedKeyValuePairs( const CORE::CValueList& kvPairs, TKeyValuePairs& kvPairsStorage )
{GUCEF_TRACE;

    kvPairsStorage.reserve( m_keyValuePairs.size() + (size_t) kvPairs.GetCount() );

    CORE::CValueList::TValueMap::const_iterator i = kvPairs.GetDataBeginIterator();
    while ( i != kvPairs.GetDataEndIterator() )
    {
        const CORE::CString& fieldName = (*i).first;
        const CORE::CValueList::TVariantVector& fieldValues = (*i).second;
        CORE::CValueList::TVariantVector::const_iterator n = fieldValues.begin();
        while ( n != fieldValues.end() )
        {
            kvPairsStorage.push_back( TKeyValuePair() );
            kvPairsStorage.back().first.LinkTo( fieldName );
            kvPairsStorage.back().second.LinkTo( (*n) );
            ++n;
        }
        ++i;
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CBasicPubSubMsg::AddLinkedKeyValuePair( const CORE::CDynamicBuffer& key, const CORE::CDynamicBuffer& value, TKeyValuePairs& kvPairsStorage )
{GUCEF_TRACE;

    kvPairsStorage.push_back( TKeyValuePair() );
    kvPairsStorage.back().first.LinkTo( key );
    kvPairsStorage.back().second.LinkTo( value );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CBasicPubSubMsg::AddLinkedKeyValuePair( const CORE::CString& key, const CORE::CDynamicBuffer& value, TKeyValuePairs& kvPairsStorage )
{GUCEF_TRACE;

    kvPairsStorage.push_back( TKeyValuePair() );
    kvPairsStorage.back().first.LinkTo( key );
    kvPairsStorage.back().second.LinkTo( value );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CBasicPubSubMsg::AddLinkedKeyValuePair( const CORE::CString& key, const CORE::CString& value, TKeyValuePairs& kvPairsStorage )
{GUCEF_TRACE;

    kvPairsStorage.push_back( TKeyValuePair() );
    kvPairsStorage.back().first.LinkTo( key );
    kvPairsStorage.back().second.LinkTo( value );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CBasicPubSubMsg::AddLinkedKeyValuePair( const CORE::CVariant& key, const CORE::CVariant& value, TKeyValuePairs& kvPairsStorage )
{GUCEF_TRACE;

    kvPairsStorage.push_back( TKeyValuePair() );
    kvPairsStorage.back().first.LinkTo( key );
    kvPairsStorage.back().second.LinkTo( value );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CBasicPubSubMsg::AddKeyValuePair( const CORE::CVariant& key, const CORE::CVariant& value, TKeyValuePairs& kvPairsStorage )
{GUCEF_TRACE;

    kvPairsStorage.push_back( TKeyValuePair( key, value ) );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CBasicPubSubMsg::AddLinkedKeyValuePair( const TKeyValuePair& kvPair )
{GUCEF_TRACE;

    return AddLinkedKeyValuePair( kvPair, m_keyValuePairs );
}

/*-------------------------------------------------------------------------*/

bool
CBasicPubSubMsg::AddLinkedKeyValuePairs( const CORE::CValueList& kvPairs )
{GUCEF_TRACE;

    return AddLinkedKeyValuePairs( kvPairs, m_keyValuePairs );
}

/*-------------------------------------------------------------------------*/

bool
CBasicPubSubMsg::AddLinkedKeyValuePair( const CORE::CDynamicBuffer& key, const CORE::CDynamicBuffer& value )
{GUCEF_TRACE;

    return AddLinkedKeyValuePair( key, value, m_keyValuePairs );
}

/*-------------------------------------------------------------------------*/

bool
CBasicPubSubMsg::AddLinkedKeyValuePair( const CORE::CString& key, const CORE::CDynamicBuffer& value )
{GUCEF_TRACE;

    return AddLinkedKeyValuePair( key, value, m_keyValuePairs );
}

/*-------------------------------------------------------------------------*/

bool
CBasicPubSubMsg::AddLinkedKeyValuePair( const CORE::CString& key, const CORE::CString& value )
{GUCEF_TRACE;

    return AddLinkedKeyValuePair( key, value, m_keyValuePairs );
}

/*-------------------------------------------------------------------------*/

bool
CBasicPubSubMsg::AddLinkedKeyValuePair( const CORE::CVariant& key, const CORE::CVariant& value )
{GUCEF_TRACE;

    return AddLinkedKeyValuePair( key, value, m_keyValuePairs );
}

/*-------------------------------------------------------------------------*/

bool
CBasicPubSubMsg::AddKeyValuePair( const CORE::CVariant& key, const CORE::CVariant& value )
{GUCEF_TRACE;

    return AddKeyValuePair( key, value, m_keyValuePairs );
}

/*-------------------------------------------------------------------------*/

bool
CBasicPubSubMsg::AddLinkedMetaDataKeyValuePair( const TKeyValuePair& kvPair )
{GUCEF_TRACE;

    return AddLinkedKeyValuePair( kvPair, m_metaDataKeyValuePairs );
}

/*-------------------------------------------------------------------------*/

bool
CBasicPubSubMsg::AddLinkedMetaDataKeyValuePairs( const CORE::CValueList& kvPairs )
{GUCEF_TRACE;

    return AddLinkedKeyValuePairs( kvPairs, m_metaDataKeyValuePairs );
}

/*-------------------------------------------------------------------------*/

bool
CBasicPubSubMsg::AddLinkedMetaDataKeyValuePair( const CORE::CDynamicBuffer& key, const CORE::CDynamicBuffer& value )
{GUCEF_TRACE;

    return AddLinkedKeyValuePair( key, value, m_metaDataKeyValuePairs );
}

/*-------------------------------------------------------------------------*/

bool
CBasicPubSubMsg::AddLinkedMetaDataKeyValuePair( const CORE::CString& key, const CORE::CDynamicBuffer& value )
{GUCEF_TRACE;

    return AddLinkedKeyValuePair( key, value, m_metaDataKeyValuePairs );
}

/*-------------------------------------------------------------------------*/

bool
CBasicPubSubMsg::AddLinkedMetaDataKeyValuePair( const CORE::CString& key, const CORE::CString& value )
{GUCEF_TRACE;

    return AddLinkedKeyValuePair( key, value, m_metaDataKeyValuePairs );
}

/*-------------------------------------------------------------------------*/

bool
CBasicPubSubMsg::AddLinkedMetaDataKeyValuePair( const CORE::CVariant& key, const CORE::CVariant& value )
{GUCEF_TRACE;

    return AddLinkedKeyValuePair( key, value, m_metaDataKeyValuePairs );
}

/*-------------------------------------------------------------------------*/

bool
CBasicPubSubMsg::AddMetaDataKeyValuePair( const CORE::CVariant& key, const CORE::CVariant& value )
{GUCEF_TRACE;

    return AddKeyValuePair( key, value, m_metaDataKeyValuePairs );
}

/*-------------------------------------------------------------------------*/

CPubSubClientTopic* 
CBasicPubSubMsg::GetOriginClientTopic( void ) const
{GUCEF_TRACE;

    return m_msgOriginClientTopic;
}

/*-------------------------------------------------------------------------*/

void 
CBasicPubSubMsg::SetOriginClientTopic( CPubSubClientTopic* msgOriginClientTopic )
{GUCEF_TRACE;

    m_msgOriginClientTopic = msgOriginClientTopic;
}

/*-------------------------------------------------------------------------*/

CORE::UInt64 
CBasicPubSubMsg::GetReceiveActionId( void ) const
{GUCEF_TRACE;

    return m_receiveActionId;
}

/*-------------------------------------------------------------------------*/

void 
CBasicPubSubMsg::SetReceiveActionId( CORE::UInt64 receiveActionId )
{GUCEF_TRACE;

    m_receiveActionId = receiveActionId;
}

/*-------------------------------------------------------------------------*/

bool 
CBasicPubSubMsg::SetKeyValuePairsAsEncodedPrimaryPayload( const TKeyValuePairs& kvPairs, const CORE::CString& dstoreCodecToUse )
{GUCEF_TRACE;

    CORE::CDynamicBuffer payload;
    if ( EncodeKeyValuePairsAsBlob( kvPairs, dstoreCodecToUse, payload ) )
    {
        m_primaryPayload = payload;
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CBasicPubSubMsg::MoveKeyValuePairsToEncodedPrimaryPayload( const CORE::CString& dstoreCodecToUse ,
                                                           int payloadVarType                    )
{GUCEF_TRACE;

    CORE::CDynamicBuffer payload;
    if ( EncodeKeyValuePairsAsBlob( m_keyValuePairs, dstoreCodecToUse, payload ) )
    {
        m_primaryPayload.Set( payload.GetBufferPtr(), payload.GetDataSize(), payloadVarType, false );
        m_keyValuePairs.clear();
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CBasicPubSubMsg::EncodeKeyValuePairsAsDataNodes( const TKeyValuePairs& kvPairs, CORE::CDataNode& kvNodes )
{GUCEF_TRACE;

    kvNodes.Clear();
    kvNodes.SetNodeType( GUCEF_DATATYPE_OBJECT );

    bool totalSuccess = true;
    TKeyValuePairs::const_iterator i = kvPairs.begin();
    while ( i != kvPairs.end() )
    {
        const CORE::CVariant& kvKey = (*i).first;
        const CORE::CVariant& kvValue = (*i).second;
        
        if ( GUCEF_NULL == kvNodes.AddChildWithValue( kvKey.AsString(), kvValue ) )
            totalSuccess = false;
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool 
CBasicPubSubMsg::EncodeKeyValuePairsAsBlob( const TKeyValuePairs& kvPairs, const CORE::CString& dstoreCodecToUse, CORE::CDynamicBuffer& buffer )
{GUCEF_TRACE;

    CORE::CDataNode kvNodes;
    if ( EncodeKeyValuePairsAsDataNodes( kvPairs, kvNodes ) )
    {
        CORE::CDStoreCodecRegistry::TDStoreCodecPtr codec;
        if ( CORE::CCoreGlobal::Instance()->GetDStoreCodecRegistry().TryLookup( dstoreCodecToUse, codec, false ) && !codec.IsNULL() )
        {
            CORE::CDynamicBufferAccess bufferIoAccess( buffer );
            if ( codec->StoreDataTree( &kvNodes, &bufferIoAccess ) )
                return true;
        }
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
