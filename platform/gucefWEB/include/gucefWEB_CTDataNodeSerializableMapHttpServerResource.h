/*
 *  gucefWEB: GUCEF module providing Web application functionality
 *  for standardized protocols
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

#ifndef GUCEF_WEB_CTDATANODESERIALIZABLEMAPHTTPSERVERRESOURCE_H
#define GUCEF_WEB_CTDATANODESERIALIZABLEMAPHTTPSERVERRESOURCE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <map>

#ifndef GUCEF_MT_COBJECTSCOPELOCK_H
#include "gucefMT_CObjectScopeLock.h"
#define GUCEF_MT_COBJECTSCOPELOCK_H
#endif /* GUCEF_MT_COBJECTSCOPELOCK_H ? */

#ifndef GUCEF_MT_COBJECTSCOPEREADONLYLOCK_H
#include "gucefMT_CObjectScopeReadOnlyLock.h"
#define GUCEF_MT_COBJECTSCOPEREADONLYLOCK_H
#endif /* GUCEF_MT_COBJECTSCOPEREADONLYLOCK_H ? */

#ifndef GUCEF_MT_CNOLOCK_H
#include "gucefMT_CNoLock.h"
#define GUCEF_MT_CNOLOCK_H
#endif /* GUCEF_MT_CNOLOCK_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "gucefCORE_CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_CORE_CDATANODESERIALIZABLESETTINGS_H
#include "gucefCORE_CDataNodeSerializableSettings.h"
#define GUCEF_CORE_CDATANODESERIALIZABLESETTINGS_H
#endif /* GUCEF_CORE_CDATANODESERIALIZABLESETTINGS_H ? */

#ifndef GUCEF_WEB_CCONFIGURABLEHTTPSERVERRESOURCE_H
#include "gucefWEB_CConfigurableHttpServerResource.h"
#define GUCEF_WEB_CCONFIGURABLEHTTPSERVERRESOURCE_H
#endif /* GUCEF_WEB_CCONFIGURABLEHTTPSERVERRESOURCE_H ? */

#ifndef GUCEF_WEB_CSTDPARAMPARSER_H
#include "gucefWEB_CStdParamParser.h"
#define GUCEF_WEB_CSTDPARAMPARSER_H
#endif /* GUCEF_WEB_CSTDPARAMPARSER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace WEB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

template< typename CollectionKeyType, class SerializableDerivedClass >
class CTDataNodeSerializableMapHttpServerResource : public CCodecBasedHTTPServerResource
{
    public:

    typedef std::map< CollectionKeyType, SerializableDerivedClass > TSerializableCollectionMap;

    CTDataNodeSerializableMapHttpServerResource( const CORE::CString& collectionName                                       ,
                                                 const CORE::CString& keyPropertyName                                      ,
                                                 const CORE::CDataNodeSerializableSettings* serializerOptions = GUCEF_NULL ,
                                                 TSerializableCollectionMap* collection = GUCEF_NULL                       ,
                                                 MT::CILockable* collectionLock = GUCEF_NULL                               ,
                                                 bool sourceObjKeyFromObj = false                                          );

    virtual ~CTDataNodeSerializableMapHttpServerResource();

    void LinkTo( const CORE::CDataNodeSerializableSettings* serializerOptions ,
                 TSerializableCollectionMap* collection                       ,
                 MT::CILockable* collectionLock                               );

    virtual bool Serialize( const CString& resourcePath         ,
                            CORE::CDataNode& output             ,
                            const CORE::CString& representation ,
                            const CORE::CString& query          ) GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  Create a new (contained) resource
     *
     *  @param transactionID The unique identifier of the create operation. If a resource was created earlier using the same transactionId, this operation should not create a new one but return the originally created resource instead.
     *  @param input The deserialized input with a DataNode based representation of the resource to create.
     *  @param representation The representation of the resource to create
     *  @param resourceOutput The created IResource. The representation of this resource shall match the representation of the resource to create.
     *  @param supportedRepresentationsOutput In case the representation is not supported for creation, this returns the list of supported representations.
     *  @return The status of the operation. See TCreateState.
     */
    virtual TCreateState CreateResource( const CString& resourcePath                   ,
                                         const CORE::CString& transactionID            ,
                                         const CORE::CDataNode& input                  ,
                                         const CORE::CString& representation           ,
                                         const CORE::CString& query                    ,
                                         THTTPServerResourcePtr& resourceOutput        ,
                                         TStringVector& supportedRepresentationsOutput ) GUCEF_VIRTUAL_OVERRIDE;

    void SetResourceKeyPropertyName( const CString& keyPropertyName );

    const CString& GetResourceKeyPropertyName( void ) const;

    protected:

    const CORE::CDataNodeSerializableSettings* m_serializerOptions;
    TSerializableCollectionMap* m_collection;
    MT::CILockable* m_collectionLock;
    CORE::CString m_collectionName;
    CORE::CString m_keyPropertyName;
    bool m_sourceObjKeyFromObj;

    private:

    CTDataNodeSerializableMapHttpServerResource( void );
    CTDataNodeSerializableMapHttpServerResource( const CTDataNodeSerializableMapHttpServerResource& src );
    CTDataNodeSerializableMapHttpServerResource& operator=( const CTDataNodeSerializableMapHttpServerResource& src );
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

template< typename CollectionKeyType, class SerializableDerivedClass >
CTDataNodeSerializableMapHttpServerResource< CollectionKeyType, SerializableDerivedClass >::CTDataNodeSerializableMapHttpServerResource( const CORE::CString& collectionName                          ,
                                                                                                                                         const CORE::CString& keyPropertyName                         ,
                                                                                                                                         const CORE::CDataNodeSerializableSettings* serializerOptions ,
                                                                                                                                         TSerializableCollectionMap* collection                       ,
                                                                                                                                         MT::CILockable* collectionLock                               ,
                                                                                                                                         bool sourceObjKeyFromObj                                     )
    : CCodecBasedHTTPServerResource()
    , m_serializerOptions( serializerOptions )
    , m_collection( collection )
    , m_collectionLock( collectionLock )
    , m_collectionName( collectionName )
    , m_keyPropertyName( keyPropertyName )
    , m_sourceObjKeyFromObj( sourceObjKeyFromObj )
{GUCEF_TRACE;

    m_allowSerialize = true;
    m_allowDeserialize = false;
    m_allowCreate = true;
}

/*-------------------------------------------------------------------------*/

template< typename CollectionKeyType, class SerializableDerivedClass >
CTDataNodeSerializableMapHttpServerResource< CollectionKeyType, SerializableDerivedClass >::~CTDataNodeSerializableMapHttpServerResource()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template< typename CollectionKeyType, class SerializableDerivedClass >
void
CTDataNodeSerializableMapHttpServerResource< CollectionKeyType, SerializableDerivedClass >::LinkTo( const CORE::CDataNodeSerializableSettings* serializerOptions ,
                                                                                                    TSerializableCollectionMap* collection                       ,
                                                                                                    MT::CILockable* collectionLock                               )
{GUCEF_TRACE;

    m_serializerOptions = serializerOptions;
    m_collection = collection;
    m_collectionLock = collectionLock;
}

/*-------------------------------------------------------------------------*/

template< typename CollectionKeyType, class SerializableDerivedClass >
void
CTDataNodeSerializableMapHttpServerResource< CollectionKeyType, SerializableDerivedClass >::SetResourceKeyPropertyName( const CString& keyPropertyName )
{GUCEF_TRACE;

    m_keyPropertyName = keyPropertyName;
}

/*-------------------------------------------------------------------------*/

template< typename CollectionKeyType, class SerializableDerivedClass >
const CString&
CTDataNodeSerializableMapHttpServerResource< CollectionKeyType, SerializableDerivedClass >::GetResourceKeyPropertyName( void ) const
{GUCEF_TRACE;

    return m_keyPropertyName;
}

/*-------------------------------------------------------------------------*/

template< typename CollectionKeyType, class SerializableDerivedClass >
bool
CTDataNodeSerializableMapHttpServerResource< CollectionKeyType, SerializableDerivedClass >::Serialize( const CString& resourcePath         ,
                                                                                                       CORE::CDataNode& output             ,
                                                                                                       const CORE::CString& representation ,
                                                                                                       const CString& params               )
{GUCEF_TRACE;

    if ( GUCEF_NULL == m_collection )
        return false;

    // Make sure we always have some degree of serializer options
    const CORE::CDataNodeSerializableSettings* serializerOptions = m_serializerOptions;
    if ( GUCEF_NULL == serializerOptions )
    {
        static const CORE::CDataNodeSerializableSettings defaultDummy;
        serializerOptions = &defaultDummy;
    }

    bool foundSupportedParams = false;
    CStdParams stdParams( *serializerOptions );
    CORE::CTSharedPtr< const CORE::CDataNodeSerializableSettings, MT::CNoLock > requestSpecificSerializerOptions;
    if ( CStdParamParser::ParseStdParams( params, stdParams, foundSupportedParams ) && foundSupportedParams && stdParams.levelOfDetailWasProvided )
    {
        // We place the clone in a shared ptr so that when we leave this scope the clone is properly disposed of
        requestSpecificSerializerOptions = CORE::CTSharedPtr< CORE::CDataNodeSerializableSettings, MT::CNoLock >( static_cast< CORE::CDataNodeSerializableSettings* >( serializerOptions->Clone() ) );
        const_cast< CORE::CDataNodeSerializableSettings* >( requestSpecificSerializerOptions.GetPointerAlways() )->levelOfDetail = stdParams.levelOfDetail;
        serializerOptions = requestSpecificSerializerOptions.GetPointerAlways();
    }

    bool totalSuccess = true;
    output = CORE::CDataNode( m_collectionName, GUCEF_DATATYPE_ARRAY );

    MT::CObjectScopeReadOnlyLock lock( m_collectionLock );

    if ( VIEW_TYPE_INDEX_ONLY == stdParams.viewType && !m_sourceObjKeyFromObj )
    {
        typename TSerializableCollectionMap::iterator i = m_collection->begin();
        while ( i != m_collection->end() )
        {
            CORE::CDataNode* childNode = output.AddChild( m_keyPropertyName, GUCEF_DATATYPE_STRING );
            if ( GUCEF_NULL != childNode )
            {
                CORE::CVariant keyValueVar( (*i).first );
                childNode->SetValue( keyValueVar );
                childNode->SetNodeType( keyValueVar.GetTypeId() );
            }
            else
                totalSuccess = false;
            ++i;
        }
    }
    else
    {
        typename TSerializableCollectionMap::iterator i = m_collection->begin();
        while ( i != m_collection->end() )
        {
            CORE::CDataNode* childNode = output.AddChild( CORE::ToString( (*i).first ) );
            if ( GUCEF_NULL != childNode )
            {
                if ( (*i).second->Serialize( *childNode, *serializerOptions ) )
                {

                }
                else
                {
                    totalSuccess = false;
                }
            }
            else
            {
                totalSuccess = false;
            }
            ++i;
        }
    }

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

template< typename CollectionKeyType, class SerializableDerivedClass >
CCodecBasedHTTPServerResource::TCreateState
CTDataNodeSerializableMapHttpServerResource< CollectionKeyType, SerializableDerivedClass >::CreateResource( const CString& resourcePath                   ,
                                                                                                            const CString& transactionID                  ,
                                                                                                            const CORE::CDataNode& input                  ,
                                                                                                            const CString& representation                 ,
                                                                                                            const CString& params                         ,
                                                                                                            THTTPServerResourcePtr& resourceOutput        ,
                                                                                                            TStringVector& supportedRepresentationsOutput )
{GUCEF_TRACE;

    if ( m_keyPropertyName.IsNULLOrEmpty() )
    {
        return ECreateState::CREATESTATE_FAILED;
    }

    const CORE::CVariant& keyPropertyValue = input.GetAttributeValueOrChildValueByName( m_keyPropertyName );
    CollectionKeyType entryKey = keyPropertyValue.AsTValue< CollectionKeyType >();
    if ( keyPropertyValue.IsNULLOrEmpty() )
    {
        return ECreateState::CREATESTATE_UNSUPPORTEDREPRESENTATION;
    }

    // Obtain a write lock to the collection
    MT::CObjectScopeLock lock( m_collectionLock );

    if ( GUCEF_NULL == m_collection )
        return TCreateState::CREATESTATE_FAILED;

    typename TSerializableCollectionMap::iterator i = m_collection->find( entryKey );
    if ( i != m_collection->end() )
    {
        return ECreateState::CREATESTATE_CONFLICTING;
    }

    SerializableDerivedClass& newEntry = (*m_collection)[ entryKey ];
    if ( GUCEF_NULL != m_serializerOptions )
    {
        if ( !newEntry->Deserialize( input, *m_serializerOptions ) )
        {
            m_collection->erase( entryKey );
            return ECreateState::CREATESTATE_DESERIALIZATIONFAILED;
        }
    }
    else
    {
        CORE::CDataNodeSerializableSettings defaultSerializableSettings;
        if ( !newEntry->Deserialize( input, defaultSerializableSettings ) )
        {
            m_collection->erase( entryKey );
            return ECreateState::CREATESTATE_DESERIALIZATIONFAILED;
        }
    }

    return ECreateState::CREATESTATE_CREATED;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_WEB_CTDATANODESERIALIZABLEMAPHTTPSERVERRESOURCE_H ? */
