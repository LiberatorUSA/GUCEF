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

#ifndef GUCEF_COM_CDEFAULTHTTPSERVERRESOURCE_H
#define GUCEF_COM_CDEFAULTHTTPSERVERRESOURCE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_COM_CIHTTPSERVERRESOURCE_H
#include "gucefCOM_CIHTTPServerResource.h"
#define GUCEF_COM_CIHTTPSERVERRESOURCE_H
#endif /* GUCEF_COM_CIHTTPSERVERRESOURCE_H ? */

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

class GUCEF_COM_EXPORT_CPP CDefaultHTTPServerResource : public CIHTTPServerResource
{    
    public:
    
    CDefaultHTTPServerResource( void );
    
    CDefaultHTTPServerResource( const CDefaultHTTPServerResource& src );
    
    virtual ~CDefaultHTTPServerResource();
    
    CDefaultHTTPServerResource& operator=( const CDefaultHTTPServerResource& src );

    /**
     *  Identifier of the resource, relative to the context of the handler.
     */
    virtual void SetURL( const CString& url ) GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  Identifier of the resource, relative to the context of the handler.
     */
    virtual const CString& GetURL( void ) const GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  Encoding of the resource (i.e. gzip compression). Matches the HTTP:Content-Encoding field.
     *  Typical example would be zip/gzip compressed content
     */
    virtual const CString& GetEncoding( void ) const GUCEF_VIRTUAL_OVERRIDE;
                    
    /**
     *   Returns a list of all representations the resource can be serialized to.
     */
    virtual const TStringVector& GetSupportedSerializationRepresentations() GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  Returns the representation that best matches a given list of representions for serialization.
     *  Returns "" in case no match can be found.
     */
    virtual CString GetBestMatchedSerializationRepresentation( const TStringVector& representations ) GUCEF_VIRTUAL_OVERRIDE;

    /**
     *   Returns a list of all representations the resource can be deserialized from.
     */
    virtual const TStringVector& GetSupportedDeserializationRepresentations() GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  Returns the representation that best matches a given list of representions for deserialization.
     *  In common situations, this may be the same as the GetBestMatchedSerializationRepresentation(..) operation.
     *  Returns "" in case no match can be found.
     */
    virtual CString GetBestSupportedDeserializationRepresentation( const TStringVector& representations ) GUCEF_VIRTUAL_OVERRIDE;

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
                                         TStringVector& supportedRepresentationsOutput ) GUCEF_VIRTUAL_OVERRIDE;
    
    /**
     *  Deletes this resource
     *
     *  @return Returns true if the resource could be deleted.
     */
    virtual bool DeleteResource() GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  Serializes the resource into a stream according to the representation.
     */
    virtual bool Serialize( CORE::CDynamicBuffer& outputBuffer  ,
                            const CORE::CString& representation ) GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  Deserialize the resource from the given stream with given resource representation.
     */
    virtual TDeserializeState Deserialize( const CORE::CDynamicBuffer& inputBuffer ,
                                           const CString& representation           ) GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  The version of the resource (HTTP: Etag)
     */
    virtual const CString& GetResourceVersion( void ) GUCEF_VIRTUAL_OVERRIDE;
    
    virtual const CString& GetLastModifiedTime( void ) GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  The cacheability of the resource.
     */
    virtual const CString& GetCacheability( void ) GUCEF_VIRTUAL_OVERRIDE;

    protected:

    CString m_cacheabilityProperty;
    CString m_lastModifiedProperty;
    CString m_resourceVersionProperty;
    CString m_encodingProperty;
    CString m_urlSegment;
    TStringVector m_deserializationReps;
    TStringVector m_serializationReps;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COM_CDEFAULTHTTPSERVERRESOURCE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 03-03-2007 :
        - Dinand: Added this section

---------------------------------------------------------------------------*/
    