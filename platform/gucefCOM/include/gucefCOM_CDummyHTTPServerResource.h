/*
 *  gucefCOM: GUCEF module providing communication implementations 
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

#ifndef GUCEF_COM_CDUMMYHTTPSERVERRESOURCE_H
#define GUCEF_COM_CDUMMYHTTPSERVERRESOURCE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_COM_CDEFAULTHTTPSERVERRESOURCE_H
#include "gucefCOM_CDefaultHTTPServerResource.h"
#define GUCEF_COM_CDEFAULTHTTPSERVERRESOURCE_H
#endif /* GUCEF_COM_CDEFAULTHTTPSERVERRESOURCE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COM {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_COM_PUBLIC_CPP CDummyHTTPServerResource : public CDefaultHTTPServerResource
{    
    public:
    
    CDummyHTTPServerResource( void );
    
    CDummyHTTPServerResource( const CDummyHTTPServerResource& src );
    
    virtual ~CDummyHTTPServerResource();
    
    CDummyHTTPServerResource& operator=( const CDummyHTTPServerResource& src );

    /**
     *  Returns the representation that best matches a given list of representions for serialization.
     *  Returns "" in case no match can be found.
     */
    virtual CString GetBestMatchedSerializationRepresentation( const TStringVector& representations );

    /**
     *  Returns the representation that best matches a given list of representions for deserialization.
     *  In common situations, this may be the same as the GetBestMatchedSerializationRepresentation(..) operation.
     *  Returns "" in case no match can be found.
     */
    virtual CString GetBestSupportedDeserializationRepresentation( const TStringVector& representations );

    /**
     *  Create a new (contained) resource
     *
     *  @param transactionID The unique identifier of the create operation. If a resource was created earlier using the same transactionId, this operation should not create a new one but return the originally created resource instead.
     *  @param inputBuffer The buffer containing the serialized representation of the resource to create.
     *  @param representation The representation of the resource to create
     *  @param resourceOutput The created IResource. The representation of this resource shall match the representation of the resource to create.
     *  @param supportedRepresentationsOutput In case the representation is not supported for creation, this returns the list of supported representations.
     *  @return The status of the operation. See TCreateState.
     */
    virtual TCreateState CreateResource( const CString& transactionID                  ,
                                         const CORE::CDynamicBuffer& inputBuffer       ,
                                         const CString& representation                 ,
                                         THTTPServerResourcePtr& resourceOutput        ,
                                         TStringVector& supportedRepresentationsOutput );
    
    /**
     *  Deletes this resource
     *
     *  @return Returns true if the resource could be deleted.
     */
    virtual bool DeleteResource();

    /**
     *  Serializes the resource into a stream according to the representation.
     */
    virtual bool Serialize( CORE::CDynamicBuffer& outputBuffer  ,
                            const CORE::CString& representation );

    /**
     *  Deserialize the resource from the given stream with given resource representation.
     *  @param isDeltaUpdateOnly Signals whether we are trying to deserialize a full resource in one go or just apply a delta update
     */
    virtual TDeserializeState Deserialize( const CORE::CDynamicBuffer& inputBuffer ,
                                           const CString& representation           ,
                                           bool isDeltaUpdateOnly                  );

};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COM_CDUMMYHTTPSERVERRESOURCE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 03-03-2007 :
        - Dinand: Added this section

---------------------------------------------------------------------------*/
    