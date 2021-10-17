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

#ifndef GUCEF_COMCORE_CBASICPUBSUBMSG_H
#define GUCEF_COMCORE_CBASICPUBSUBMSG_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <vector>

#ifndef GUCEF_COMCORE_CIPUBSUBMSG_H
#include "gucefCOMCORE_CIPubSubMsg.h"
#define GUCEF_COMCORE_CIPUBSUBMSG_H
#endif /* GUCEF_COMCORE_CIPUBSUBMSG_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Basic implementation of the IPubSubMsg interface
 *  It does not integrate with any backend internal data types and as such may not be
 *  as efficient compared to a dedicated backend specific implementations.
 *  You can however using memory linking whereever possible to avoid data copies
 */
class GUCEF_COMCORE_EXPORT_CPP CBasicPubSubMsg : public CIPubSubMsg
{
    public:

    typedef std::vector< CBasicPubSubMsg >  TBasicPubSubMsgVector;

    CBasicPubSubMsg( void );

    CBasicPubSubMsg( const CBasicPubSubMsg& src );

    virtual ~CBasicPubSubMsg() GUCEF_VIRTUAL_OVERRIDE;
    
    virtual CORE::CVariant& GetMsgId( void ) GUCEF_VIRTUAL_OVERRIDE;    
    virtual const CORE::CVariant& GetMsgId( void ) const GUCEF_VIRTUAL_OVERRIDE;
    
    virtual CORE::CVariant& GetMsgIndex( void ) GUCEF_VIRTUAL_OVERRIDE;    
    virtual const CORE::CVariant& GetMsgIndex( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual CORE::CDateTime& GetMsgDateTime( void ) GUCEF_VIRTUAL_OVERRIDE;    
    virtual const CORE::CDateTime& GetMsgDateTime( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual CORE::CVariant& GetPrimaryPayload( void ) GUCEF_VIRTUAL_OVERRIDE;
    virtual const CORE::CVariant& GetPrimaryPayload( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual TKeyValuePairs& GetKeyValuePairs( void ) GUCEF_VIRTUAL_OVERRIDE;
    virtual const TKeyValuePairs& GetKeyValuePairs( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual TKeyValuePairs& GetMetaDataKeyValuePairs( void ) GUCEF_VIRTUAL_OVERRIDE;
    virtual const TKeyValuePairs& GetMetaDataKeyValuePairs( void ) const GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  For a message that was received by a pub-sub topic this would provide
     *  a link back to the topic where the message originated via a subscription
     *
     *  For a message constructed by the application for publishing this can be left 
     *  as GUCEF_NULL
     */ 
    virtual CPubSubClientTopic* GetOriginClientTopic( void ) const GUCEF_VIRTUAL_OVERRIDE;

    void SetOriginClientTopic( CPubSubClientTopic* msgOriginClientTopic );

    bool AddKeyValuePair( const CORE::CVariant& key, const CORE::CVariant& value );
    bool AddMetaDataKeyValuePair( const CORE::CVariant& key, const CORE::CVariant& value );
    
    bool SetKeyValuePairsAsEncodedPrimaryPayload( const TKeyValuePairs& kvPairs, const CORE::CString& dstoreCodecToUse );
    bool MoveKeyValuePairsToEncodedPrimaryPayload( const CORE::CString& dstoreCodecToUse, int payloadVarType = GUCEF_DATATYPE_BINARY_BLOB );

    /**
     *  Utility member functions
     *  Adds a key-value pair entry to the message but instead of copying the content provided links to it
     *  This is far more efficient since no (potentially large) data blocks need to be copied.
     *  It does however REQUIRE that the passed memory remains valid for the lifecycle of the message
     *  as such this is best used in a transient manner
     */
    bool AddLinkedKeyValuePair( const TKeyValuePair& kvPair );
    bool AddLinkedKeyValuePairs( const CORE::CValueList& kvPairs );
    bool AddLinkedKeyValuePair( const CORE::CDynamicBuffer& key, const CORE::CDynamicBuffer& value );
    bool AddLinkedKeyValuePair( const CORE::CString& key, const CORE::CDynamicBuffer& value );
    bool AddLinkedKeyValuePair( const CORE::CString& key, const CORE::CString& value );
    bool AddLinkedKeyValuePair( const CORE::CVariant& key, const CORE::CVariant& value );

    /**
     *  Utility member functions
     *  Adds a meta-data key-value pair entry associated with the message but instead of copying the content provided links to it
     *  This is far more efficient since no (potentially large) data blocks need to be copied.
     *  It does however REQUIRE that the passed memory remains valid for the lifecycle of the message
     *  as such this is best used in a transient manner
     */
    bool AddLinkedMetaDataKeyValuePair( const TKeyValuePair& kvPair );
    bool AddLinkedMetaDataKeyValuePairs( const CORE::CValueList& kvPairs );
    bool AddLinkedMetaDataKeyValuePair( const CORE::CDynamicBuffer& key, const CORE::CDynamicBuffer& value );
    bool AddLinkedMetaDataKeyValuePair( const CORE::CString& key, const CORE::CDynamicBuffer& value );
    bool AddLinkedMetaDataKeyValuePair( const CORE::CString& key, const CORE::CString& value );
    bool AddLinkedMetaDataKeyValuePair( const CORE::CVariant& key, const CORE::CVariant& value );

    virtual CORE::CICloneable* Clone( void ) const GUCEF_VIRTUAL_OVERRIDE;

    void Clear( void );
    
    private:

    bool AddLinkedKeyValuePair( const TKeyValuePair& kvPair, TKeyValuePairs& kvPairsStorage );
    bool AddLinkedKeyValuePairs( const CORE::CValueList& kvPairs, TKeyValuePairs& kvPairsStorage );
    bool AddLinkedKeyValuePair( const CORE::CDynamicBuffer& key, const CORE::CDynamicBuffer& value, TKeyValuePairs& kvPairsStorage );
    bool AddLinkedKeyValuePair( const CORE::CString& key, const CORE::CDynamicBuffer& value, TKeyValuePairs& kvPairsStorage );
    bool AddLinkedKeyValuePair( const CORE::CString& key, const CORE::CString& value, TKeyValuePairs& kvPairsStorage );
    bool AddLinkedKeyValuePair( const CORE::CVariant& key, const CORE::CVariant& value, TKeyValuePairs& kvPairsStorage );

    bool AddKeyValuePair( const CORE::CVariant& key, const CORE::CVariant& value, TKeyValuePairs& kvPairsStorage );

    static bool EncodeKeyValuePairsAsDataNodes( const TKeyValuePairs& kvPairs, CORE::CDataNode& kvNodes );
    static bool EncodeKeyValuePairsAsBlob( const TKeyValuePairs& kvPairs, const CORE::CString& dstoreCodecToUse, CORE::CDynamicBuffer& buffer );

    protected:

    CORE::CVariant m_msgId;
    CORE::CVariant m_msgIndex;
    CORE::CDateTime m_msgDateTime;
    CORE::CVariant m_primaryPayload;
    TKeyValuePairs m_keyValuePairs;    
    TKeyValuePairs m_metaDataKeyValuePairs;    
    CPubSubClientTopic* m_msgOriginClientTopic;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COMCORE_CBASICPUBSUBMSG_H ? */
