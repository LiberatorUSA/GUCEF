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

#include <string.h>            /* needed for memcpy() */

#include "CMFileAccess.h"      /* definition of the class implemented here */

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

UInt32 GUCEF_CALLSPEC_PREFIX 
mfa_open( struct SIOAccess* access ) GUCEF_CALLSPEC_SUFFIX
{
        ( (CMFileAccess*) access->privdata )->Open();
        return 0;
}

/*-------------------------------------------------------------------------*/

UInt32 GUCEF_CALLSPEC_PREFIX 
mfa_opened( struct SIOAccess* access ) GUCEF_CALLSPEC_SUFFIX
{
        return ( (CMFileAccess*) access->privdata )->Opened();
}

/*-------------------------------------------------------------------------*/

UInt32 GUCEF_CALLSPEC_PREFIX
mfa_close( struct SIOAccess* access ) GUCEF_CALLSPEC_SUFFIX
{
        ( (CMFileAccess*) access->privdata )->Close();
        return 0;
}

/*-------------------------------------------------------------------------*/

UInt32 GUCEF_CALLSPEC_PREFIX 
mfa_readl( struct SIOAccess* access , 
           char **dest              ) GUCEF_CALLSPEC_SUFFIX
{
        CString str = ( (CMFileAccess*) access->privdata )->ReadLine();
        *dest = new char[ str.Length()+1 ];
        memcpy( *dest, str.C_String(), str.Length() );        
        return str.Length();
}

/*-------------------------------------------------------------------------*/

UInt32 GUCEF_CALLSPEC_PREFIX 
mfa_reads( struct SIOAccess* access , 
           char **dest              ) GUCEF_CALLSPEC_SUFFIX
{
        CString str = ( (CMFileAccess*) access->privdata )->ReadString();
        *dest = new char[ str.Length()+1 ];
        memcpy( *dest, str.C_String(), str.Length() );
        return str.Length();
}

/*-------------------------------------------------------------------------*/
           
UInt32 GUCEF_CALLSPEC_PREFIX
mfa_read( struct SIOAccess* access , 
          void *dest               , 
          UInt32 esize             , 
          UInt32 elements          ) GUCEF_CALLSPEC_SUFFIX
{
        return ( (CMFileAccess*) access->privdata )->Read( dest     ,
                                                           esize    ,
                                                           elements );
}

/*-------------------------------------------------------------------------*/
          
UInt32 GUCEF_CALLSPEC_PREFIX
mfa_tell( struct SIOAccess* access ) GUCEF_CALLSPEC_SUFFIX
{
        return ( (CMFileAccess*) access->privdata )->Tell();
}

/*-------------------------------------------------------------------------*/

Int32 GUCEF_CALLSPEC_PREFIX
mfa_seek( struct SIOAccess* access , 
          Int32 offset             ,
          Int32 origin             ) GUCEF_CALLSPEC_SUFFIX
{
        return ( (CMFileAccess*) access->privdata )->Seek( offset ,
                                                           origin );
}

/*-------------------------------------------------------------------------*/
          
UInt32 GUCEF_CALLSPEC_PREFIX 
mfa_setpos( struct SIOAccess* access , 
            UInt32 pos               ) GUCEF_CALLSPEC_SUFFIX
{
        return ( (CMFileAccess*) access->privdata )->Setpos( pos );
}

/*-------------------------------------------------------------------------*/
            
Int32 GUCEF_CALLSPEC_PREFIX 
mfa_getc( struct SIOAccess* access ) GUCEF_CALLSPEC_SUFFIX
{
        return ( (CMFileAccess*) access->privdata )->GetChar();
}

/*-------------------------------------------------------------------------*/

Int32 GUCEF_CALLSPEC_PREFIX
mfa_eof( struct SIOAccess* access ) GUCEF_CALLSPEC_SUFFIX
{
        return ( (CMFileAccess*) access->privdata )->Eof();
}

/*-------------------------------------------------------------------------*/

void GUCEF_CALLSPEC_PREFIX 
mfa_free( void* mem ) GUCEF_CALLSPEC_SUFFIX
{
        delete []((char*)mem);
}

/*-------------------------------------------------------------------------*/

CMFileAccess::CMFileAccess( const void* memblock ,
                            const UInt32 size    )
{
        mfinit( &_mfile  ,
                memblock ,
                size     );
                
        _access.open = mfa_open;
        _access.close = mfa_close;                       
        _access.opened = mfa_opened;
        _access.readl = mfa_readl;
        _access.reads = mfa_reads;
        _access.read = mfa_read;
        _access.tell = mfa_tell;
        _access.seek = mfa_seek;
        _access.setpos = mfa_setpos;
        _access.getc = mfa_getc;
        _access.eof = mfa_eof;
        _access.memfree = mfa_free;
        _access.privdata = this;                        
}

/*-------------------------------------------------------------------------*/

CMFileAccess::CMFileAccess( const CMFileAccess& src )
        : CROIOAccess( src )
{
        _access.privdata = this;
        
        mfinit( &_mfile           ,
                src._mfile.mchunk ,
                src._mfile.size   );        
}

/*-------------------------------------------------------------------------*/

CMFileAccess::~CMFileAccess()
{  
}

/*-------------------------------------------------------------------------*/

void 
CMFileAccess::Open( void )
{
        mfsetpos( &_mfile ,
                  0       ); 
}

/*-------------------------------------------------------------------------*/

void 
CMFileAccess::Close( void )
{
        mfsetpos( &_mfile ,
                  0       ); 
}

/*-------------------------------------------------------------------------*/

bool 
CMFileAccess::Opened( void ) const
{
        return true;
}

/*-------------------------------------------------------------------------*/

CString 
CMFileAccess::ReadLine( void )
{
        char* str;
        UInt32 length = mfreadl( &str    , 
                                 &_mfile );
        if ( length && str )
        {
                CString fstr( str );
                mfree( str );
                return fstr;
        }                 
        
        CString emptystr;
        return emptystr;
}

/*-------------------------------------------------------------------------*/

CString 
CMFileAccess::ReadString( void )
{
        char* str;
        UInt32 length = mfreads( &str    , 
                                 &_mfile );
        if ( length && str )
        {
                CString fstr( str );
                mfree( str );
                return fstr;
        }                 
        
        CString emptystr;
        return emptystr;
}

/*-------------------------------------------------------------------------*/

UInt32 
CMFileAccess::Read( void *dest      ,
                    UInt32 esize    ,
                    UInt32 elements )
{
        return mfread( dest     ,
                       esize    ,
                       elements ,
                       &_mfile  );
}

/*-------------------------------------------------------------------------*/

UInt32 
CMFileAccess::Tell( void ) const
{
        return mftell( &_mfile );
}

/*-------------------------------------------------------------------------*/

Int32 
CMFileAccess::Seek( Int32 offset ,
                    Int32 origin )
{
        return mfseek( &_mfile ,
                       offset  ,
                       origin  );
}

/*-------------------------------------------------------------------------*/

UInt32 
CMFileAccess::Setpos( UInt32 position )
{
        return mfsetpos( &_mfile  , 
                         position ); 
}

/*-------------------------------------------------------------------------*/

UInt32
CMFileAccess::GetSize( void ) const
{
        return _mfile.size;
}

/*-------------------------------------------------------------------------*/

char 
CMFileAccess::GetChar( void )
{
        return mfgetc( &_mfile );
}

/*-------------------------------------------------------------------------*/

bool 
CMFileAccess::Eof( void ) const
{
        return mfeof( &_mfile ) == 1;
}

/*-------------------------------------------------------------------------*/

TIOAccess* 
CMFileAccess::CStyleAccess( void )
{
        return &_access;        
}

/*-------------------------------------------------------------------------*/

/**
 *      Is the access to the resource a valid one or
 *      has something gone wrong ?
 */
bool 
CMFileAccess::IsValid( void )
{
        return true;
}

/*-------------------------------------------------------------------------*/

CICloneable*
CMFileAccess::Clone( void ) const
{
        return new CMFileAccess( *this );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
