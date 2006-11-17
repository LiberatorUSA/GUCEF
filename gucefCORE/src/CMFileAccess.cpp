/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL, 
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER 
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF 
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT 
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
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
          UInt32 offset            , 
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
CMFileAccess::Seek( UInt32 offset ,
                    Int32 origin  )
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

Int32
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
