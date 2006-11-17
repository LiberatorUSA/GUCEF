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

#ifndef GUCEF_CORE_DVFILEUTILS_H
#include "dvfileutils.h"       /* cross-platform file utils */
#define GUCEF_CORE_DVFILEUTILS_H
#endif /* GUCEF_CORE_DVFILEUTILS_H ? */

#include "CFileAccess.h"      /* definition of the class implemented here */

#ifndef GUCEF_CORE_UCEF_ESSENTIALS_H
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
fa_open( struct SIOAccess* access ) GUCEF_CALLSPEC_SUFFIX
{TRACE;
        ( (CFileAccess*) access->privdata )->Open();
        return 0;
}

/*-------------------------------------------------------------------------*/

UInt32 GUCEF_CALLSPEC_PREFIX 
fa_opened( struct SIOAccess* access ) GUCEF_CALLSPEC_SUFFIX
{TRACE;
        return ( (CFileAccess*) access->privdata )->Opened();
}

/*-------------------------------------------------------------------------*/

UInt32 GUCEF_CALLSPEC_PREFIX
fa_close( struct SIOAccess* access ) GUCEF_CALLSPEC_SUFFIX
{TRACE;
        ( (CFileAccess*) access->privdata )->Close();
        return 0;
}

/*-------------------------------------------------------------------------*/

UInt32 GUCEF_CALLSPEC_PREFIX 
fa_readl( struct SIOAccess* access , 
          char **dest              ) GUCEF_CALLSPEC_SUFFIX
{TRACE;
        CString str = ( (CFileAccess*) access->privdata )->ReadLine();
        *dest = new char[ str.Length()+1 ];
        memcpy( *dest, str.C_String(), str.Length() );
        return str.Length();
}

/*-------------------------------------------------------------------------*/

UInt32 GUCEF_CALLSPEC_PREFIX 
fa_reads( struct SIOAccess* access , 
          char **dest              ) GUCEF_CALLSPEC_SUFFIX
{TRACE;
        CString str = ( (CFileAccess*) access->privdata )->ReadString();
        *dest = new char[ str.Length()+1 ];
        memcpy( *dest, str.C_String(), str.Length() );        
        return str.Length();
}

/*-------------------------------------------------------------------------*/
           
UInt32 GUCEF_CALLSPEC_PREFIX
fa_read( struct SIOAccess* access , 
         void *dest               , 
         UInt32 esize             , 
         UInt32 elements          ) GUCEF_CALLSPEC_SUFFIX
{TRACE;
        return ( (CFileAccess*) access->privdata )->Read( dest     ,
                                                          esize    ,
                                                          elements );
}

/*-------------------------------------------------------------------------*/
          
UInt32 GUCEF_CALLSPEC_PREFIX
fa_tell( struct SIOAccess* access ) GUCEF_CALLSPEC_SUFFIX
{
        GUCEF_BEGIN;
        GUCEF_END_RET( UInt32, ( (CFileAccess*) access->privdata )->Tell() );
}

/*-------------------------------------------------------------------------*/

Int32 GUCEF_CALLSPEC_PREFIX
fa_seek( struct SIOAccess* access , 
         UInt32 offset            , 
         Int32 origin             ) GUCEF_CALLSPEC_SUFFIX
{
        GUCEF_BEGIN;
        GUCEF_END_RET( Int32, ( (CFileAccess*) access->privdata )->Seek( offset ,
                                                                         origin ) );
}

/*-------------------------------------------------------------------------*/
          
UInt32 GUCEF_CALLSPEC_PREFIX 
fa_setpos( struct SIOAccess* access , 
           UInt32 pos               ) GUCEF_CALLSPEC_SUFFIX
{
        GUCEF_BEGIN;
        GUCEF_END_RET( UInt32, ( (CFileAccess*) access->privdata )->Setpos( pos ) );
}

/*-------------------------------------------------------------------------*/
            
Int32 GUCEF_CALLSPEC_PREFIX 
fa_getc( struct SIOAccess* access ) GUCEF_CALLSPEC_SUFFIX
{
        GUCEF_BEGIN;
        GUCEF_END_RET( Int32, ( (CFileAccess*) access->privdata )->GetChar() );
}

/*-------------------------------------------------------------------------*/

Int32 GUCEF_CALLSPEC_PREFIX
fa_eof( struct SIOAccess* access ) GUCEF_CALLSPEC_SUFFIX
{
        GUCEF_BEGIN;
        GUCEF_END_RET( Int32, ( (CFileAccess*) access->privdata )->Eof() );
}

/*-------------------------------------------------------------------------*/

void GUCEF_CALLSPEC_PREFIX 
fa_free( void* mem ) GUCEF_CALLSPEC_SUFFIX
{TRACE;
        delete []((char*)mem);
}

/*-------------------------------------------------------------------------*/

CFileAccess::CFileAccess( const CString& file           ,
                          const char* mode /* = "rb" */ )
        : _filename( file ) ,
          m_mode( mode )
{TRACE;
        
        _readable = ( strchr( mode, 'r' ) != NULL ) || ( strchr( mode, 'a' ) != NULL );
        _writeable = ( strchr( mode, 'w' ) != NULL ) || ( strchr( mode, 'a' ) != NULL );
        
        _size = Filesize( file.C_String() );
        _file = fopen( file.C_String() ,
                       mode            ); 
                        
        _access.open = fa_open;
        _access.close = fa_close;                       
        _access.opened = fa_opened;
        _access.readl = fa_readl;
        _access.reads = fa_reads;
        _access.read = fa_read;
        _access.tell = fa_tell;
        _access.seek = fa_seek;
        _access.setpos = fa_setpos;
        _access.getc = fa_getc;
        _access.eof = fa_eof;
        _access.memfree = fa_free;
        _access.privdata = this;                        
}

/*-------------------------------------------------------------------------*/

CFileAccess::~CFileAccess()
{TRACE;
        Close();    
}

/*-------------------------------------------------------------------------*/

void 
CFileAccess::Open( void )
{TRACE;
        Close();
        
        _file = fopen( _filename.C_String() ,
                       m_mode.C_String()    );                      
}

/*-------------------------------------------------------------------------*/

void 
CFileAccess::Close( void )
{TRACE;
        if ( _file )
        { 
                fclose( _file );
                _file = NULL;
        }
}

/*-------------------------------------------------------------------------*/

bool 
CFileAccess::Opened( void ) const
{TRACE;
        return _file != NULL;
}

/*-------------------------------------------------------------------------*/

CString 
CFileAccess::ReadLine( void )
{TRACE;

 /*       char* str;
        UInt32 length = mfreadl( &str    , 
                                 &_mfile );
        if ( length && str )
        {
                CString fstr( str );
                mfree( str );
                return fstr;
        }         */        
        
        CString emptystr;
        return emptystr;
}

/*-------------------------------------------------------------------------*/

CString 
CFileAccess::ReadString( void )
{TRACE;
 /*       char* str;
        UInt32 length = mfreads( &str    , 
                                 &_mfile );
        if ( length && str )
        {
                CString fstr( str );
                mfree( str );
                return fstr;
        }       */          
        
        CString emptystr;
        return emptystr;
}

/*-------------------------------------------------------------------------*/

UInt32 
CFileAccess::Read( void *dest      ,
                    UInt32 esize    ,
                    UInt32 elements )
{TRACE;
        return (UInt32) fread( dest     ,
                               esize    ,
                               elements ,
                               _file    );
}

/*-------------------------------------------------------------------------*/

UInt32 
CFileAccess::Tell( void ) const
{TRACE;
        return ftell( _file );
}

/*-------------------------------------------------------------------------*/

Int32 
CFileAccess::Seek( UInt32 offset ,
                    Int32 origin  )
{TRACE;
        return fseek( _file     ,
                      offset    ,
                      origin    );
}

/*-------------------------------------------------------------------------*/

UInt32 
CFileAccess::Setpos( UInt32 position )
{TRACE;
        return fseek( _file     ,
                      position  ,
                      SEEK_SET  );
}

/*-------------------------------------------------------------------------*/

char 
CFileAccess::GetChar( void )
{TRACE;
        return (char) fgetc( _file );
}

/*-------------------------------------------------------------------------*/

bool 
CFileAccess::Eof( void ) const
{TRACE;
        return feof( _file ) == 1;
}

/*-------------------------------------------------------------------------*/

bool 
CFileAccess::IsReadOnly( void ) const
{TRACE;
        return _readable;
}

/*-------------------------------------------------------------------------*/

bool 
CFileAccess::IsWriteOnly( void ) const
{TRACE;
        return _writeable;
}

/*-------------------------------------------------------------------------*/
        
bool 
CFileAccess::IsReadAndWrite( void ) const
{TRACE;
        return _readable && _writeable;
}

/*-------------------------------------------------------------------------*/
       
UInt32 
CFileAccess::Write( const void* srcdata ,
                    UInt32 esize        ,
                    UInt32 elements     )
{TRACE;
        return (UInt32) fwrite( srcdata  , 
                                esize    , 
                                elements , 
                                _file    );
}

/*-------------------------------------------------------------------------*/

TIOAccess* 
CFileAccess::CStyleAccess( void )
{TRACE;
        return &_access;        
}

/*-------------------------------------------------------------------------*/

/**
 *      Is the access to the resource a valid one or
 *      has something gone wrong ?
 */
bool 
CFileAccess::IsValid( void )
{TRACE;
        return File_Exists( _filename.C_String() ) == 1;
}

/*-------------------------------------------------------------------------*/

Int32 
CFileAccess::GetSize( void ) const
{TRACE;
        if ( _file && _writeable )
        {
                fflush( _file );       
        }
        return Filesize( _filename.C_String() );           
}

/*-------------------------------------------------------------------------*/

CICloneable* 
CFileAccess::Clone( void ) const
{TRACE;
        if ( IsReadOnly() )
        {
                return new CFileAccess( _filename, "rb" );
        }
        
        // Cannot be cloned
        return NULL;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
