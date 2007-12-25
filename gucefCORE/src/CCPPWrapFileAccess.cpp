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
{
        GUCEF_BEGIN;
        GUCEF_END;                            
}

/*-------------------------------------------------------------------------*/

CCPPWrapFileAccess::~CCPPWrapFileAccess()
{
        GUCEF_BEGIN;
        GUCEF_END;        
}

/*-------------------------------------------------------------------------*/

void 
CCPPWrapFileAccess::Open( void )
{
        GUCEF_BEGIN;
        _access->open( _access );
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

void 
CCPPWrapFileAccess::Close( void )
{
        GUCEF_BEGIN;
        _access->close( _access ); 
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

bool 
CCPPWrapFileAccess::Opened( void ) const
{
        GUCEF_BEGIN;
        GUCEF_END_RET( bool, _access->opened( _access ) == 1 );
}

/*-------------------------------------------------------------------------*/

UInt32 
CCPPWrapFileAccess::GetSize( void ) const
{        
        GUCEF_BEGIN;
        GUCEF_END;
        return 0;  
}

/*-------------------------------------------------------------------------*/

CString 
CCPPWrapFileAccess::ReadLine( void )
{
        GUCEF_BEGIN;
        char* str;
        UInt32 length = _access->readl( _access ,
                                        &str    );
        if ( length && str )
        {
                CString fstr( str );
                _access->memfree( str );
                GUCEF_END;
                return fstr;
        }                 
        
        CString emptystr;
        GUCEF_END;
        return emptystr;
}

/*-------------------------------------------------------------------------*/

CString 
CCPPWrapFileAccess::ReadString( void )
{
        GUCEF_BEGIN;
        char* str;
        UInt32 length = _access->reads( _access ,
                                        &str    );
        if ( length && str )
        {
                CString fstr( str );
                _access->memfree( str );
                GUCEF_END;
                return fstr;
        }                 
        
        CString emptystr;
        GUCEF_END;
        return emptystr;
}

/*-------------------------------------------------------------------------*/

UInt32 
CCPPWrapFileAccess::Read( void *dest      ,
                          UInt32 esize    ,
                          UInt32 elements )
{
        GUCEF_BEGIN;
        GUCEF_END_RET( UInt32, _access->read( _access  , 
                                              dest     ,
                                              esize    ,
                                              elements ) );
}

/*-------------------------------------------------------------------------*/

UInt32 
CCPPWrapFileAccess::Tell( void ) const
{
        GUCEF_BEGIN;
        GUCEF_END_RET( UInt32, _access->tell( _access ) );
}

/*-------------------------------------------------------------------------*/

Int32 
CCPPWrapFileAccess::Seek( Int32 offset ,
                          Int32 origin )
{
        GUCEF_BEGIN;
        GUCEF_END_RET( Int32, _access->seek( _access ,
                                             offset  ,
                                             origin  ) );
}

/*-------------------------------------------------------------------------*/

UInt32 
CCPPWrapFileAccess::Setpos( UInt32 position )
{
        GUCEF_BEGIN;
        GUCEF_END_RET( UInt32, _access->setpos( _access  , 
                                                position ) ); 
}

/*-------------------------------------------------------------------------*/

char 
CCPPWrapFileAccess::GetChar( void )
{
        GUCEF_BEGIN;
        GUCEF_END_RET( char, (char) _access->getc( _access ) );
}

/*-------------------------------------------------------------------------*/

bool 
CCPPWrapFileAccess::Eof( void ) const
{
        GUCEF_BEGIN;
        GUCEF_END_RET( bool, _access->eof( _access ) == 1 );
}

/*-------------------------------------------------------------------------*/

bool 
CCPPWrapFileAccess::IsReadOnly( void ) const
{
        GUCEF_BEGIN;
        GUCEF_END;
        return true;
}

/*-------------------------------------------------------------------------*/

bool 
CCPPWrapFileAccess::IsWriteOnly( void ) const
{
        GUCEF_BEGIN;
        GUCEF_END;
        return false;
}

/*-------------------------------------------------------------------------*/
        
bool 
CCPPWrapFileAccess::IsReadAndWrite( void ) const
{
        GUCEF_BEGIN;
        GUCEF_END;
        return false;
}

/*-------------------------------------------------------------------------*/
       
UInt32 
CCPPWrapFileAccess::Write( const void* srcdata ,
                           UInt32 esize        ,
                           UInt32 elements     )
{
        return 0;
}

/*-------------------------------------------------------------------------*/

TIOAccess* 
CCPPWrapFileAccess::CStyleAccess( void )
{
        GUCEF_BEGIN;
        GUCEF_END;
        return _access;        
}

/*-------------------------------------------------------------------------*/

/**
 *      Is the access to the resource a valid one or
 *      has something gone wrong ?
 */
bool 
CCPPWrapFileAccess::IsValid( void )
{
        GUCEF_BEGIN;
        GUCEF_END;
        return _access != NULL;
}

/*-------------------------------------------------------------------------*/

CICloneable*
CCPPWrapFileAccess::Clone( void ) const
{GUCEF_TRACE;
        return new CCPPWrapFileAccess( _access );
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
