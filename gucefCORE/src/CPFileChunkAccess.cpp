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

#include <string.h>             /* needed for memcpy() */

#include "CPFileChunkAccess.h"  /* definition of the class implemented here */

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
pfca_open( struct SIOAccess* access ) GUCEF_CALLSPEC_SUFFIX
{
        ( (CPFileChunkAccess*) access->privdata )->Open();
        return 0;
}

/*-------------------------------------------------------------------------*/

UInt32 GUCEF_CALLSPEC_PREFIX 
pfca_opened( struct SIOAccess* access ) GUCEF_CALLSPEC_SUFFIX
{
        return ( (CPFileChunkAccess*) access->privdata )->Opened();
}

/*-------------------------------------------------------------------------*/

UInt32 GUCEF_CALLSPEC_PREFIX
pfca_close( struct SIOAccess* access ) GUCEF_CALLSPEC_SUFFIX
{
        ( (CPFileChunkAccess*) access->privdata )->Close();
        return 0;
}

/*-------------------------------------------------------------------------*/

UInt32 GUCEF_CALLSPEC_PREFIX 
pfca_readl( struct SIOAccess* access , 
           char **dest              ) GUCEF_CALLSPEC_SUFFIX
{
        CString str = ( (CPFileChunkAccess*) access->privdata )->ReadLine();
        *dest = new char[ str.Length()+1 ];
        memcpy( *dest, str.C_String(), str.Length() );
        return str.Length();
}

/*-------------------------------------------------------------------------*/

UInt32 GUCEF_CALLSPEC_PREFIX 
pfca_reads( struct SIOAccess* access , 
           char **dest              ) GUCEF_CALLSPEC_SUFFIX
{
        CString str = ( (CPFileChunkAccess*) access->privdata )->ReadString();
        *dest = new char[ str.Length()+1 ];
        memcpy( *dest, str.C_String(), str.Length() );
        return str.Length();
}

/*-------------------------------------------------------------------------*/
           
UInt32 GUCEF_CALLSPEC_PREFIX
pfca_read( struct SIOAccess* access , 
          void *dest               , 
          UInt32 esize             , 
          UInt32 elements          ) GUCEF_CALLSPEC_SUFFIX
{
        return ( (CPFileChunkAccess*) access->privdata )->Read( dest     ,
                                                                esize    ,
                                                                elements );
}

/*-------------------------------------------------------------------------*/
          
UInt32 GUCEF_CALLSPEC_PREFIX
pfca_tell( struct SIOAccess* access ) GUCEF_CALLSPEC_SUFFIX
{
        return ( (CPFileChunkAccess*) access->privdata )->Tell();
}

/*-------------------------------------------------------------------------*/

Int32 GUCEF_CALLSPEC_PREFIX
pfca_seek( struct SIOAccess* access , 
          UInt32 offset            , 
          Int32 origin             ) GUCEF_CALLSPEC_SUFFIX
{
        return ( (CPFileChunkAccess*) access->privdata )->Seek( offset ,
                                                                origin );
}

/*-------------------------------------------------------------------------*/
          
UInt32 GUCEF_CALLSPEC_PREFIX 
pfca_setpos( struct SIOAccess* access , 
            UInt32 pos               ) GUCEF_CALLSPEC_SUFFIX
{
        return ( (CPFileChunkAccess*) access->privdata )->Setpos( pos );
}

/*-------------------------------------------------------------------------*/
            
Int32 GUCEF_CALLSPEC_PREFIX 
pfca_getc( struct SIOAccess* access ) GUCEF_CALLSPEC_SUFFIX
{
        return ( (CPFileChunkAccess*) access->privdata )->GetChar();
}

/*-------------------------------------------------------------------------*/

Int32 GUCEF_CALLSPEC_PREFIX
pfca_eof( struct SIOAccess* access ) GUCEF_CALLSPEC_SUFFIX
{
        return ( (CPFileChunkAccess*) access->privdata )->Eof();
}

/*-------------------------------------------------------------------------*/

void GUCEF_CALLSPEC_PREFIX 
pfca_free( void* mem ) GUCEF_CALLSPEC_SUFFIX
{
        delete []((char*)mem);
}

/*-------------------------------------------------------------------------*/

CPFileChunkAccess::CPFileChunkAccess( CPFileAccess& pafileaccess ,
                                      UInt32 startoffset         ,
                                      UInt32 size                )
        : m_startOffset( startoffset )   ,
          m_size( size )                 ,
          m_pafileaccess( &pafileaccess )
{
        _pafile = palinkchunktocontrol( pafileaccess._pafilecontrol ,
                                        startoffset                 ,
                                        size                        );
                
        _access.open = pfca_open;
        _access.close = pfca_close;                       
        _access.opened = pfca_opened;
        _access.readl = pfca_readl;
        _access.reads = pfca_reads;
        _access.read = pfca_read;
        _access.tell = pfca_tell;
        _access.seek = pfca_seek;
        _access.setpos = pfca_setpos;
        _access.getc = pfca_getc;
        _access.eof = pfca_eof;
        _access.memfree = pfca_free;
        _access.privdata = this;                        
}

/*-------------------------------------------------------------------------*/

CPFileChunkAccess::CPFileChunkAccess( const CPFileChunkAccess& src )
        : m_startOffset( src.m_startOffset )   ,
          m_size( src.m_size )                 ,
          m_pafileaccess( src.m_pafileaccess )
{
        _access = src._access;
        _access.privdata = this;
        
        _pafile = palinkchunktocontrol( m_pafileaccess->_pafilecontrol ,
                                        m_startOffset                  ,
                                        m_size                         );        
}

/*-------------------------------------------------------------------------*/

CPFileChunkAccess::~CPFileChunkAccess()
{
        paunlink( _pafile );      
}

/*-------------------------------------------------------------------------*/

void 
CPFileChunkAccess::Open( void )
{
        pafsetpos( _pafile ,
                   0       ); 
}

/*-------------------------------------------------------------------------*/

void 
CPFileChunkAccess::Close( void )
{
        pafsetpos( _pafile ,
                   0       ); 
}

/*-------------------------------------------------------------------------*/

bool 
CPFileChunkAccess::Opened( void ) const
{
        return _pafile != NULL;
}

/*-------------------------------------------------------------------------*/

CString 
CPFileChunkAccess::ReadLine( void )
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
CPFileChunkAccess::ReadString( void )
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
CPFileChunkAccess::Read( void *dest      ,
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
CPFileChunkAccess::Tell( void ) const
{
        return paftell( _pafile );
}

/*-------------------------------------------------------------------------*/

Int32 
CPFileChunkAccess::Seek( UInt32 offset ,
                         Int32 origin  )
{
        return pafseek( _pafile ,
                        offset  ,
                        origin  );
}

/*-------------------------------------------------------------------------*/

UInt32 
CPFileChunkAccess::Setpos( UInt32 position )
{
        return pafsetpos( _pafile  , 
                          position ); 
}

/*-------------------------------------------------------------------------*/

Int32
CPFileChunkAccess::GetSize( void ) const
{
        return pafsize( _pafile );
}

/*-------------------------------------------------------------------------*/

char 
CPFileChunkAccess::GetChar( void )
{
        return (char) pafgetc( _pafile );
}

/*-------------------------------------------------------------------------*/

bool 
CPFileChunkAccess::Eof( void ) const
{
        return pafeof( _pafile ) == 1;
}

/*-------------------------------------------------------------------------*/

TIOAccess* 
CPFileChunkAccess::CStyleAccess( void )
{
        return &_access;        
}

/*-------------------------------------------------------------------------*/

/**
 *      Is the access to the resource a valid one or
 *      has something gone wrong ?
 */
bool 
CPFileChunkAccess::IsValid( void )
{
        if ( _pafile )
        {       
                return palinked( _pafile ) == 1;
        }
        return false;
}

/*-------------------------------------------------------------------------*/

CORE::CICloneable*
CPFileChunkAccess::Clone( void ) const
{
        return new CPFileChunkAccess( *this );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
