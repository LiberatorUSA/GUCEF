/*
 *  gucefCOM: GUCEF module providing communication 
 *  implementations for standardized protocols.
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
 */

#ifndef GUCEF_COM_CIHTTPSERVERRESOURCE_H
#define GUCEF_COM_CIHTTPSERVERRESOURCE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <vector>

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_CORE_CTSHAREDPTR_H
#include "CTSharedPtr.h"
#define GUCEF_CORE_CTSHAREDPTR_H
#endif /* GUCEF_CORE_CTSHAREDPTR_H ? */

#ifndef GUCEF_COM_MACROS_H
#include "gucefCOM_macros.h"
#define GUCEF_COM_MACROS_H
#endif /* GUCEF_COM_MACROS_H ? */

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

class GUCEF_COM_EXPORT_CPP CIHTTPServerResource
{
    public:
    
    typedef std::vector< CString > TStringVector;
    typedef CORE::CTSharedPtr< CIHTTPServerResource > THTTPServerResourcePtr;

    /**
     *  Result of a CreateResource operation.
     */
    typedef enum TCreateState
    {
        CREATESTATE_CREATED                   , /**< The resource has been created and persisted as desired by the backend */
        CREATESTATE_CONFLICTING               , /**< Unable to create due to a conflicted data state (think of conflicting unique ids inside the data etc with those in a collection) */
        CREATESTATE_DESERIALIZATIONFAILED     , /**< Failed to deserialize the resource with given representation. */
        CREATESTATE_UNSUPPORTEDREPRESENTATION , /**< Resource representation is not supported for deserialization. */
        CREATESTATE_FAILED                      /**< Failed to create the resource */
    };

    /**
     *  Result of a Deserialize operation.
     */
    typedef enum TDeserializeState
    {
        DESERIALIZESTATE_SUCCEEDED      , /**< deserialization succeeded without any problems */
        DESERIALIZESTATE_CORRUPTEDINPUT , /**< Unable to deserialize because the input data is corrupt */
        DESERIALIZESTATE_UNABLETOUPDATE   /**< Unable to deserialize because after reading the input data successfully updating the resource failed. */
    };
    
    public:
    
    CIHTTPServerResource( void );
    
    CIHTTPServerResource( const CIHTTPServerResource& src );
    
    virtual ~CIHTTPServerResource();
    
    CIHTTPServerResource& operator=( const CIHTTPServerResource& src );

    /**
     *  Identifier of the resource, relative to the context of the handler.
     */
    virtual void SetURL( const CString& url ) const = 0;

    /**
     *  Identifier of the resource, relative to the context of the handler.
     */
    virtual const CString& GetURL( void ) const = 0;

    /**
     *  Encoding of the resource (i.e. gzip compression). Matches the HTTP:Content-Encoding field.
     *  Typical example would be zip/gzip compressed content
     */
    virtual const CString& GetEncoding( void ) const = 0;

    /**
     *  Serializes the resource into a stream according to the representation.
     */
    virtual bool Serialize( CORE::CDynamicBuffer& outputBuffer ,
                            const CString& representation      ) = 0;
                    
    /**
     *   Returns a list of all representations the resource can be serialized to.
     */
    virtual const TStringVector& GetSupportedSerializationRepresentations() = 0;

    /**
     *  Returns the representation that best matches a given list of representions for serialization.
     *  Returns "" in case no match can be found.
     */
    virtual CString GetBestMatchedSerializationRepresentation( const TStringVector& representations ) = 0;

    /**
     *  Deserialize the resource from the given stream with given resource representation.
     */
    virtual TDeserializeState Deserialize( const CORE::CDynamicBuffer& inputBuffer ,
                                           const CString& representation           ) = 0;

    /**
     *   Returns a list of all representations the resource can be deserialized from.
     */
    virtual const TStringVector& GetSupportedDeserializationRepresentations() = 0;

    /**
     *  Returns the representation that best matches a given list of representions for deserialization.
     *  In common situations, this may be the same as the GetBestMatchedSerializationRepresentation(..) operation.
     *  Returns "" in case no match can be found.
     */
    virtual CString GetBestSupportedDeserializationRepresentation( const TStringVector& representations ) = 0;

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
                                         TStringVector& supportedRepresentationsOutput ) = 0;
    
    /**
     *  Deletes this resource
     *
     *  @return Returns true if the resource could be deleted.
     */
    virtual bool DeleteResource() = 0;

    /**
     *  The version of the resource (HTTP: Etag)
     */
    virtual const CString& GetResourceVersion( void ) = 0;
    
    virtual const CString& GetLastModifiedTime( void ) = 0;

    /**
     *  The cacheability of the resource.
     */
    virtual const CString& GetCacheability( void ) = 0;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COM_CIHTTPSERVERRESOURCE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 03-03-2007 :
        - Dinand: Added this section

---------------------------------------------------------------------------*/
    