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

#include "CCPPWrapFileAccess.h" /* definition of the class implemented here */

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CCPPWrapFileAccess::CCPPWrapFileAccess( TIOAccess *access )
        : _access( access )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CCPPWrapFileAccess::~CCPPWrapFileAccess()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CCPPWrapFileAccess::Open( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL != _access )
        _access->open( _access );
}

/*-------------------------------------------------------------------------*/

void
CCPPWrapFileAccess::Close( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL != _access )
        _access->close( _access );
}

/*-------------------------------------------------------------------------*/

bool
CCPPWrapFileAccess::Opened( void ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL != _access )
        return _access->opened( _access ) == 1;
    return false;
}

/*-------------------------------------------------------------------------*/

UInt64
CCPPWrapFileAccess::GetSize( void ) const
{GUCEF_TRACE;

    return 0;
}

/*-------------------------------------------------------------------------*/

CString
CCPPWrapFileAccess::ReadLine( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL != _access )
    {
        char* str = GUCEF_NULL;
        UInt32 length = _access->readl( _access, &str );
        if ( length > 0 && str )
        {
            CString fstr( str, length );
            _access->memfree( str );
            str = GUCEF_NULL;

            return fstr;
        }
    }
    return CString::Empty;
}

/*-------------------------------------------------------------------------*/

CString
CCPPWrapFileAccess::ReadString( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL != _access )
    {
        char* str = GUCEF_NULL;
        UInt32 length = _access->reads( _access, &str );
        if ( length > 0 && str )
        {
            CString fstr( str, length );
            _access->memfree( str );
            str = GUCEF_NULL;

            return fstr;
        }
    }
    return CString::Empty;
}

/*-------------------------------------------------------------------------*/

UInt32
CCPPWrapFileAccess::Read( void *dest      ,
                          UInt32 esize    ,
                          UInt32 elements )
{GUCEF_TRACE;

    if ( GUCEF_NULL != _access )
        return _access->read( _access  ,
                              dest     ,
                              esize    ,
                              elements );
    return 0;
}

/*-------------------------------------------------------------------------*/

UInt64
CCPPWrapFileAccess::Tell( void ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL != _access )
        return _access->tell( _access );
    return 0;
}

/*-------------------------------------------------------------------------*/

Int32
CCPPWrapFileAccess::Seek( Int64 offset ,
                          Int32 origin )
{GUCEF_TRACE;

    if ( GUCEF_NULL != _access )
        return _access->seek( _access ,
                              offset  ,
                              origin  );
    return 1;
}

/*-------------------------------------------------------------------------*/

UInt32
CCPPWrapFileAccess::Setpos( UInt64 position )
{GUCEF_TRACE;

    if ( GUCEF_NULL != _access )
        return _access->setpos( _access, position );
    return 1;
}

/*-------------------------------------------------------------------------*/

char
CCPPWrapFileAccess::GetChar( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL != _access )
        return ((TIOAccessfunction_getc)_access->getc)( _access );
    return 0;
}

/*-------------------------------------------------------------------------*/

bool
CCPPWrapFileAccess::Eof( void ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL != _access )
        return _access->eof( _access ) == 1;
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CCPPWrapFileAccess::IsReadable( void ) const
{GUCEF_TRACE;

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CCPPWrapFileAccess::IsWriteable( void ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

UInt32
CCPPWrapFileAccess::Write( const void* srcdata ,
                           UInt32 esize        ,
                           UInt32 elements     )
{GUCEF_TRACE;

    return 0;
}

/*-------------------------------------------------------------------------*/

TIOAccess*
CCPPWrapFileAccess::CStyleAccess( void )
{GUCEF_TRACE;

    return _access;
}

/*-------------------------------------------------------------------------*/

bool
CCPPWrapFileAccess::IsValid( void )
{GUCEF_TRACE;

    return _access != GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

CICloneable*
CCPPWrapFileAccess::Clone( void ) const
{GUCEF_TRACE;

    return GUCEF_NEW CCPPWrapFileAccess( _access );
}

/*-------------------------------------------------------------------------*/

void
CCPPWrapFileAccess::Flush( void )
{GUCEF_TRACE;

    // no-op, not supported on the C-Style interface at this time
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
