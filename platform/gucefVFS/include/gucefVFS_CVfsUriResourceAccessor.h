/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
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

#ifndef GUCEF_VFS_CVFSURIRESOURCEACCESSOR_H
#define GUCEF_VFS_CVFSURIRESOURCEACCESSOR_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CURIRESOURCEACCESSOR_H
#include "gucefCORE_CUriResourceAccessor.h"
#define GUCEF_CORE_CURIRESOURCEACCESSOR_H
#endif /* GUCEF_CORE_CURIRESOURCEACCESSOR_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace VFS {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Class for sync blocking CRUD access to a VFS resource abstracted through a URI
 *  This class is threadsafe in the sense that it is stateless
 */
class GUCEF_VFS_PUBLIC_CPP CVfsUriResourceAccessor : public CORE::CUriResourceAccessor
{
    public:

    static const CORE::CString SchemeName;

    virtual bool CreateResource( const CORE::CUri& uri                 ,
                                 const CORE::CDynamicBuffer& srcBuffer ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool CreateResource( const CORE::CUri& uri ,
                                 CORE::CIOAccess& src  ) GUCEF_VIRTUAL_OVERRIDE;
    
    virtual bool GetResource( const CORE::CUri& uri                   ,
                              CORE::CDynamicBuffer& destinationBuffer ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool GetResource( const CORE::CUri& uri        ,
                              CORE::CIOAccess& destination ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool GetPartialResource( const CORE::CUri& uri        ,
                                     UInt64 byteOffset            ,
                                     Int64 bytesToGet             ,
                                     CORE::CIOAccess& destination ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool GetResourceMetaData( const CORE::CUri& uri             ,
                                      CORE::CResourceMetaData& metaData ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool UpdateResource( const CORE::CUri& uri           ,
                                 CORE::CDynamicBuffer& srcBuffer ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool UpdateResource( const CORE::CUri& uri ,
                                 CORE::CIOAccess& src  ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool UpdatePartialResource( const CORE::CUri& uri ,
                                        UInt64 byteOffset     ,
                                        Int64 bytesToWrite    ,
                                        CORE::CIOAccess& src  ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool UpdatePartialResource( const CORE::CUri& uri ,
                                        CORE::CIOAccess& src  ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool GetSupportedOperations( const CORE::CUri& uri               ,
                                         CUriResourceAccessorOperations& ops ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool DeleteResource( const CORE::CUri& uri ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool IsACollectionResource( const CORE::CUri& uri ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool ResolveResourcesInCollection( const CORE::CUri& uri  ,
                                               UriVector& resources   ,
                                               bool recursive         ,
                                               bool addCollectionUris ) GUCEF_VIRTUAL_OVERRIDE;

    CVfsUriResourceAccessor( void ); 
    CVfsUriResourceAccessor( const CVfsUriResourceAccessor& src );
    virtual ~CVfsUriResourceAccessor() GUCEF_VIRTUAL_OVERRIDE; 
    CVfsUriResourceAccessor& operator=( const CVfsUriResourceAccessor& src );
};

/*-------------------------------------------------------------------------*/

typedef CORE::CTSharedPtr< CVfsUriResourceAccessor, MT::CMutex > CVfsUriResourceAccessorPtr;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace VFS */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_VFS_CVFSURIRESOURCEACCESSOR_H ? */
