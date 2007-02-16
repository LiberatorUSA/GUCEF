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

#include "CPFileAccess.h"      /* definition of the class implemented here */

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
pfa_open( struct SIOAccess* access ) GUCEF_CALLSPEC_SUFFIX
{
        ( (CPFileAccess*) access->privdata )->Open();
        return 0;
}

/*-------------------------------------------------------------------------*/

UInt32 GUCEF_CALLSPEC_PREFIX 
pfa_opened( struct SIOAccess* access ) GUCEF_CALLSPEC_SUFFIX
{
        return ( (CPFileAccess*) access->privdata )->Opened();
}

/*-------------------------------------------------------------------------*/

UInt32 GUCEF_CALLSPEC_PREFIX
pfa_close( struct SIOAccess* access ) GUCEF_CALLSPEC_SUFFIX
{
        ( (CPFileAccess*) access->privdata )->Close();
        return 0;
}

/*-------------------------------------------------------------------------*/

UInt32 GUCEF_CALLSPEC_PREFIX 
pfa_readl( struct SIOAccess* access , 
           char **dest              ) GUCEF_CALLSPEC_SUFFIX
{
        CString str = ( (CPFileAccess*) access->privdata )->ReadLine();
        *dest = new char[ str.Length()+1 ];
        memcpy( *dest, str.C_String(), str.Length() );        
        return str.Length();
}

/*-------------------------------------------------------------------------*/

UInt32 GUCEF_CALLSPEC_PREFIX 
pfa_reads( struct SIOAccess* access , 
           char **dest              ) GUCEF_CALLSPEC_SUFFIX
{
        CString str = ( (CPFileAccess*) access->privdata )->ReadString();
        *dest = new char[ str.Length()+1 ];
        memcpy( *dest, str.C_String(), str.Length() );
        return str.Length();
}

/*-------------------------------------------------------------------------*/
           
UInt32 GUCEF_CALLSPEC_PREFIX
pfa_read( struct SIOAccess* access , 
          void *dest               , 
          UInt32 esize             , 
          UInt32 elements          ) GUCEF_CALLSPEC_SUFFIX
{
        return ( (CPFileAccess*) access->privdata )->Read( dest     ,
                                                           esize    ,
                                                           elements );
}

/*-------------------------------------------------------------------------*/
          
UInt32 GUCEF_CALLSPEC_PREFIX
pfa_tell( struct SIOAccess* access ) GUCEF_CALLSPEC_SUFFIX
{
        return ( (CPFileAccess*) access->privdata )->Tell();
}

/*-------------------------------------------------------------------------*/

Int32 GUCEF_CALLSPEC_PREFIX
pfa_seek( struct SIOAccess* access , 
          Int32 offset             ,
          Int32 origin             ) GUCEF_CALLSPEC_SUFFIX
{
        return ( (CPFileAccess*) access->privdata )->Seek( offset ,
                                                           origin );
}

/*-------------------------------------------------------------------------*/
          
UInt32 GUCEF_CALLSPEC_PREFIX 
pfa_setpos( struct SIOAccess* access , 
            UInt32 pos               ) GUCEF_CALLSPEC_SUFFIX
{
        return ( (CPFileAccess*) access->privdata )->Setpos( pos );
}

/*-------------------------------------------------------------------------*/
            
Int32 GUCEF_CALLSPEC_PREFIX 
pfa_getc( struct SIOAccess* access ) GUCEF_CALLSPEC_SUFFIX
{
        return ( (CPFileAccess*) access->privdata )->GetChar();
}

/*-------------------------------------------------------------------------*/

Int32 GUCEF_CALLSPEC_PREFIX
pfa_eof( struct SIOAccess* access ) GUCEF_CALLSPEC_SUFFIX
{
        return ( (CPFileAccess*) access->privdata )->Eof();
}

/*-------------------------------------------------------------------------*/

void GUCEF_CALLSPEC_PREFIX 
pfa_free( void* mem ) GUCEF_CALLSPEC_SUFFIX
{
        delete []((char*)mem);
}

/*-------------------------------------------------------------------------*/

CPFileAccess::CPFileAccess( const CString& file )
        : _filename( file )
{
        _pafilecontrol = pacreate( file.C_String() ,
                                   "rb"            );
        _pafile = palinktocontrol( _pafilecontrol );                                                   
                
        _access.open = pfa_open;
        _access.close = pfa_close;                       
        _access.opened = pfa_opened;
        _access.readl = pfa_readl;
        _access.reads = pfa_reads;
        _access.read = pfa_read;
        _access.tell = pfa_tell;
        _access.seek = pfa_seek;
        _access.setpos = pfa_setpos;
        _access.getc = pfa_getc;
        _access.eof = pfa_eof;
        _access.memfree = pfa_free;
        _access.privdata = this;
}

/*-------------------------------------------------------------------------*/

CPFileAccess::~CPFileAccess()
{
        paunlink( _pafile );
        padestroy( _pafilecontrol );
}

/*-------------------------------------------------------------------------*/

void 
CPFileAccess::Open( void )
{
        pafsetpos( _pafile ,
                   0       ); 
}

/*-------------------------------------------------------------------------*/

void 
CPFileAccess::Close( void )
{
        pafsetpos( _pafile ,
                   0       ); 
}

/*-------------------------------------------------------------------------*/

bool 
CPFileAccess::Opened( void ) const
{
        return ( _pafilecontrol != NULL ) && ( _pafile != NULL );
}

/*-------------------------------------------------------------------------*/

CString 
CPFileAccess::ReadLine( void )
{
        char* str;
        UInt32 length = pafreadl( &str    , 
                                  _pafile );
        if ( length && str )
        {
                CString fstr( str );
                pafree( str );
                return fstr;
        }                 
        
        CString emptystr;
        return emptystr;
}

/*-------------------------------------------------------------------------*/

CString 
CPFileAccess::ReadString( void )
{
        char* str;
        UInt32 length = pafreads( &str    , 
                                  _pafile );
        if ( length && str )
        {
                CString fstr( str );
                pafree( str );
                return fstr;
        }                 
        
        CString emptystr;
        return emptystr;
}

/*-------------------------------------------------------------------------*/

UInt32 
CPFileAccess::Read( void *dest      ,
                    UInt32 esize    ,
                    UInt32 elements )
{
        return pafread( dest     ,
                        esize    ,
                        elements ,
                        _pafile  );
}

/*-------------------------------------------------------------------------*/

UInt32 
CPFileAccess::Tell( void ) const
{
        return paftell( _pafile );
}

/*-------------------------------------------------------------------------*/

Int32 
CPFileAccess::Seek( Int32 offset ,
                    Int32 origin )
{
        return pafseek( _pafile ,
                        offset  ,
                        origin  );
}

/*-------------------------------------------------------------------------*/

UInt32 
CPFileAccess::Setpos( UInt32 position )
{
        return pafsetpos( _pafile  , 
                          position ); 
}

/*-------------------------------------------------------------------------*/

Int32
CPFileAccess::GetSize( void ) const
{
        return pafsize( _pafile );
}

/*-------------------------------------------------------------------------*/

char 
CPFileAccess::GetChar( void )
{
        return (char) pafgetc( _pafile );
}

/*-------------------------------------------------------------------------*/

bool 
CPFileAccess::Eof( void ) const
{
        return pafeof( _pafile ) == 1;
}

/*-------------------------------------------------------------------------*/

TIOAccess* 
CPFileAccess::CStyleAccess( void )
{
        return &_access;        
}

/*-------------------------------------------------------------------------*/

/**
 *      Is the access to the resource a valid one or
 *      has something gone wrong ?
 */
bool 
CPFileAccess::IsValid( void )
{
        if ( _pafile && _pafilecontrol )
        {       
                return palinked( _pafile ) == 1;
        }
        return false;
}

/*-------------------------------------------------------------------------*/

CORE::CICloneable*
CPFileAccess::Clone( void ) const
{
        return new CPFileAccess( _filename );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
