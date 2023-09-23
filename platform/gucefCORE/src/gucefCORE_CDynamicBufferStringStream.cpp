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

#include "gucefCORE_CDynamicBufferStringStream.h"

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

CDynamicBufferStringStream::CDynamicBufferStringStream( void )
    : CDynamicBuffer()
    , CIOAccessToStlStringStream()
    , CDynamicBufferAccess()
{GUCEF_TRACE;

    CDynamicBufferAccess::LinkTo( static_cast< CDynamicBuffer* >( this ), false );
    CIOAccessToStlStringStream::InitIOStreamAccess( *( static_cast< CDynamicBufferAccess* >( this ) ) );
}

/*-------------------------------------------------------------------------*/

CDynamicBufferStringStream::CDynamicBufferStringStream( const char* externalBuffer ,
                                                        const UInt32 bufferSize    ,
                                                        bool autoenlarge           )
    : CDynamicBuffer( externalBuffer, bufferSize, autoenlarge )
    , CIOAccessToStlStringStream()
    , CDynamicBufferAccess()
{GUCEF_TRACE;

    CDynamicBufferAccess::LinkTo( static_cast< CDynamicBuffer* >( this ), false );
    CIOAccessToStlStringStream::InitIOStreamAccess( *( static_cast< CDynamicBufferAccess* >( this ) ) );
}

/*-------------------------------------------------------------------------*/

CDynamicBufferStringStream::CDynamicBufferStringStream( const CDynamicBuffer &src ,
                                                        bool shrinkToDataSize     )
    : CDynamicBuffer( src, shrinkToDataSize )
    , CIOAccessToStlStringStream()
    , CDynamicBufferAccess()
{GUCEF_TRACE;

    CDynamicBufferAccess::LinkTo( static_cast< CDynamicBuffer* >( this ), false );
    CIOAccessToStlStringStream::InitIOStreamAccess( *( static_cast< CDynamicBufferAccess* >( this ) ) );
}

/*-------------------------------------------------------------------------*/

CDynamicBufferStringStream::CDynamicBufferStringStream( const CDynamicBufferStringStream& src )
    : CDynamicBuffer( src )
    , CIOAccessToStlStringStream()
    , CDynamicBufferAccess()
{GUCEF_TRACE;

    CDynamicBufferAccess::LinkTo( static_cast< CDynamicBuffer* >( this ), false );
    CIOAccessToStlStringStream::InitIOStreamAccess( *( static_cast< CDynamicBufferAccess* >( this ) ) );
}

/*-------------------------------------------------------------------------*/

#ifdef GUCEF_RVALUE_REFERENCES_SUPPORTED

CDynamicBufferStringStream::CDynamicBufferStringStream( CDynamicBufferStringStream&& src ) GUCEF_NOEXCEPT
    : CDynamicBuffer( GUCEF_MOVE( static_cast< CDynamicBuffer&& >( src ) ) )
    , CIOAccessToStlStringStream()
    , CDynamicBufferAccess()
{GUCEF_TRACE;

    CDynamicBufferAccess::LinkTo( static_cast< CDynamicBuffer* >( this ), false );
    CIOAccessToStlStringStream::InitIOStreamAccess( *( static_cast< CDynamicBufferAccess* >( this ) ) );
    CDynamicBufferAccess::Setpos( src.Tell() );
    static_cast< CDynamicBufferAccess& >( src ).Unlink();
}

#endif

/*-------------------------------------------------------------------------*/

CDynamicBufferStringStream::~CDynamicBufferStringStream()
{GUCEF_TRACE;

    CDynamicBufferAccess::Unlink();
}

/*-------------------------------------------------------------------------*/

CICloneable* 
CDynamicBufferStringStream::Clone( void ) const
{GUCEF_TRACE;

    return static_cast< CDynamicBuffer* >( new CDynamicBufferStringStream( *this ) );
}

/*-------------------------------------------------------------------------*/

bool 
CDynamicBufferStringStream::operator==( const char* str ) const
{GUCEF_TRACE;

    return 0 == strcmp( str, AsConstTypePtr< char >() );
}

/*-------------------------------------------------------------------------*/

bool 
CDynamicBufferStringStream::operator!=( const char* str ) const
{GUCEF_TRACE;

    return 0 != strcmp( str, AsConstTypePtr< char >() );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
