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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#include "CDynamicBufferAccess.h"
#define GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#endif /* GUCEF_CORE_CDYNAMICBUFFERACCESS_H ? */

#include "gucefCORE_CUriResourceAccessor.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CUriResourceAccessor::CUriResourceAccessor( void )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CUriResourceAccessor::CUriResourceAccessor( const CUriResourceAccessor& src )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CUriResourceAccessor::~CUriResourceAccessor()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CUriResourceAccessor& 
CUriResourceAccessor::operator=( const CUriResourceAccessor& src )
{GUCEF_TRACE;

    return *this;
}

/*-------------------------------------------------------------------------*/

CUriResourceAccessor::CUriResourceAccessorOperations::CUriResourceAccessorOperations( void )
    : createResource( false )
    , getResource( false )
    , getPartialResource( false )
    , getResourceMetaData( false )
    , getResourceWriteAccess( false )
    , getResourceAppendAccess( false )
    , getResourceReadAccess( false )
    , updateResource( false )
    , updatePartialResource( false )
    , deleteResource( false )
    , collectionDetermnination( false )
    , collectionResolution( false )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CUriResourceAccessor::CUriResourceAccessorOperations::CUriResourceAccessorOperations( const CUriResourceAccessorOperations& src )
    : createResource( src.createResource )
    , getResource( src.getResource )
    , getPartialResource( src.getPartialResource )
    , getResourceMetaData( src.getResourceMetaData )
    , getResourceWriteAccess( src.getResourceWriteAccess )
    , getResourceAppendAccess( src.getResourceAppendAccess )
    , getResourceReadAccess( src.getResourceReadAccess )
    , updateResource( src.updateResource )
    , updatePartialResource( src.updatePartialResource )
    , deleteResource( src.deleteResource )
    , collectionDetermnination( src.collectionDetermnination )
    , collectionResolution( src.collectionResolution )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CUriResourceAccessor::CreateResource( const CUri& uri                 ,
                                      const CDynamicBuffer& srcBuffer )
{GUCEF_TRACE;

    CDynamicBufferAccess access( srcBuffer );
    return CreateResource( uri, access ); 
}

/*-------------------------------------------------------------------------*/

bool
CUriResourceAccessor::GetResource( const CUri& uri                   ,
                                   CDynamicBuffer& destinationBuffer )
{GUCEF_TRACE;

    CDynamicBufferAccess access( destinationBuffer );
    return GetResource( uri, access ); 
}

/*-------------------------------------------------------------------------*/

bool
CUriResourceAccessor::GetPartialResource( const CUri& uri                   ,
                                          UInt64 byteOffset                 ,
                                          Int64 bytesToGet                  ,
                                          CDynamicBuffer& destinationBuffer )
{GUCEF_TRACE;

    CDynamicBufferAccess access( destinationBuffer );
    return GetPartialResource( uri, byteOffset, bytesToGet, access );
}

/*-------------------------------------------------------------------------*/

bool 
CUriResourceAccessor::UpdateResource( const CUri& uri           ,
                                      CDynamicBuffer& srcBuffer )
{GUCEF_TRACE;

    CDynamicBufferAccess access( srcBuffer );
    return UpdateResource( uri, access );
}

/*-------------------------------------------------------------------------*/

bool
CUriResourceAccessor::UpdatePartialResource( const CUri& uri           ,
                                             UInt64 byteOffset         ,
                                             Int64 bytesToWrite        ,
                                             CDynamicBuffer& srcBuffer )
{GUCEF_TRACE;

    CDynamicBufferAccess access( srcBuffer );
    return UpdatePartialResource( uri, byteOffset, bytesToWrite, access );
}

/*-------------------------------------------------------------------------*/

bool 
CUriResourceAccessor::UpdatePartialResource( const CUri& uri           ,
                                             CDynamicBuffer& srcBuffer )
{GUCEF_TRACE;

    CDynamicBufferAccess access( srcBuffer );
    return UpdatePartialResource( uri, access );
}

/*-------------------------------------------------------------------------*/

const char*
CUriResourceAccessor::ResourceAccessModeStr( TURI_RESOURCEACCESS_MODE mode )
{GUCEF_TRACE;

    switch ( mode )
    {
        case URI_RESOURCEACCESS_MODE_READ:
            return "rb";
        case URI_RESOURCEACCESS_MODE_WRITE:
            return "wb";
        case URI_RESOURCEACCESS_MODE_APPEND:
            return "a";
        default:
            return "";
    }
}

/*-------------------------------------------------------------------------*/

bool
CUriResourceAccessor::IsReadOnlyResourceAccessMode( TURI_RESOURCEACCESS_MODE mode )
{GUCEF_TRACE;

    switch ( mode )
    {
        case URI_RESOURCEACCESS_MODE_READ:
            return true;
        
        default:
        case URI_RESOURCEACCESS_MODE_WRITE:
        case URI_RESOURCEACCESS_MODE_APPEND:
            return false;
    }
}

/*-------------------------------------------------------------------------*/

bool
CUriResourceAccessor::IsOverwriteResourceAccessMode( TURI_RESOURCEACCESS_MODE mode )
{GUCEF_TRACE;

    switch ( mode )
    {
        case URI_RESOURCEACCESS_MODE_WRITE:
            return true;
        
        default:
            return false;
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
