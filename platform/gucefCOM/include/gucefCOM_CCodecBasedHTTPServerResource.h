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

#ifndef GUCEF_COM_CCODECBASEDHTTPSERVERRESOURCE_H
#define GUCEF_COM_CCODECBASEDHTTPSERVERRESOURCE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CDSTORECODECREGISTRY_H
#include "CDStoreCodecRegistry.h"
#define GUCEF_CORE_CDSTORECODECREGISTRY_H
#endif /* GUCEF_CORE_CDSTORECODECREGISTRY_H ? */

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

class GUCEF_COM_EXPORT_CPP CCodecBasedHTTPServerResource : public CDefaultHTTPServerResource
{    
    public:
    
    CCodecBasedHTTPServerResource( void );
    
    CCodecBasedHTTPServerResource( const CCodecBasedHTTPServerResource& src );
    
    virtual ~CCodecBasedHTTPServerResource();
    
    CCodecBasedHTTPServerResource& operator=( const CCodecBasedHTTPServerResource& src );

    /**
     *  Serializes the resource into a stream according to the representation.
     */
    virtual bool Serialize( CORE::CDynamicBuffer& outputBuffer  ,
                            const CORE::CString& representation ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Serialize( CORE::CDataNode& output             ,
                            const CORE::CString& representation );

    /**
     *  Deserialize the resource from the given stream with given resource representation.
     */
    virtual TDeserializeState Deserialize( const CORE::CDynamicBuffer& inputBuffer ,
                                           const CString& representation           ) GUCEF_VIRTUAL_OVERRIDE;

    virtual TDeserializeState Deserialize( const CORE::CDataNode& input  ,
                                           const CString& representation );

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
     *  Create a new (contained) resource
     *
     *  @param transactionID The unique identifier of the create operation. If a resource was created earlier using the same transactionId, this operation should not create a new one but return the originally created resource instead.
     *  @param input The deserialized input with a DataNode based representation of the resource to create.
     *  @param representation The representation of the resource to create
     *  @param resourceOutput The created IResource. The representation of this resource shall match the representation of the resource to create.
     *  @param supportedRepresentationsOutput In case the representation is not supported for creation, this returns the list of supported representations.
     *  @return The status of the operation. See TCreateState.
     */
    virtual TCreateState CreateResource( const CString& transactionID                  ,
                                         const CORE::CDataNode& input                  ,
                                         const CString& representation                 ,
                                         THTTPServerResourcePtr& resourceOutput        ,
                                         TStringVector& supportedRepresentationsOutput );

    protected:

    typedef std::map< CORE::CString, CORE::CDStoreCodecRegistry::TDStoreCodecPtr > TStringToCodecMap;

    bool InitCodecLinks( void );

    TStringToCodecMap m_serializeRepToCodecMap;
    TStringToCodecMap m_deserializeRepToCodecMap;
    bool m_allowCreate;
    bool m_allowDeserialize;
    bool m_allowSerialize;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COM_CCODECBASEDHTTPSERVERRESOURCE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 03-03-2007 :
        - Dinand: Added this section

---------------------------------------------------------------------------*/
    