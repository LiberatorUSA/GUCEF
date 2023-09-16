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
#include <errno.h>

#ifndef GUCEF_CORE_DVFILEUTILS_H
#include "dvfileutils.h"       /* cross-platform file utils */
#define GUCEF_CORE_DVFILEUTILS_H
#endif /* GUCEF_CORE_DVFILEUTILS_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h" 
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_DVCPPFILEUTILS_H
#include "dvcppfileutils.h"
#define GUCEF_CORE_DVCPPFILEUTILS_H
#endif /* GUCEF_CORE_DVCPPFILEUTILS_H ? */ 

#include "CFileAccess.h"      /* definition of the class implemented here */

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

CFileAccess::CFileAccess( void )
    : CIOAccess()    ,
      m_filename()   ,
      m_mode()       ,
      m_file( NULL )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CFileAccess::CFileAccess( const CString& file           ,
                          const char* mode /* = "rb" */ )
        : CIOAccess(),
          m_filename( file ) ,
          m_mode( mode )     ,
          m_file( NULL )     
{GUCEF_TRACE;

    Open( file, mode );
}

/*-------------------------------------------------------------------------*/

CFileAccess::~CFileAccess()
{GUCEF_TRACE;

    Close();    
}

/*-------------------------------------------------------------------------*/

CString
CFileAccess::GetErrorString( int errorCode )
{GUCEF_TRACE;

    switch ( errorCode )
    {
        case EACCES:
            return "EACCES: Search permission is denied on a component of the path prefix, or the file exists and the permissions specified by mode are denied, or the file does not exist and write permission is denied for the parent directory of the file to be created.";
        case EINTR:
            return "EINTR: A signal was caught during fopen()";
        case EISDIR:
            return "EISDIR: The named file is a directory and mode requires write access";
        case ELOOP:
            return "ELOOP: A loop exists in symbolic links encountered during resolution of the path argument";
        case EMFILE:
            return "EMFILE: {OPEN_MAX} file descriptors are currently open in the calling process";
        case ENAMETOOLONG:
            return "ENAMETOOLONG: The length of the filename argument exceeds {PATH_MAX} or a pathname component is longer than {NAME_MAX}";
        case ENFILE:
            return "ENFILE: The maximum allowable number of files is currently open in the system";
        case ENOENT:
            return "ENOENT: A component of filename does not name an existing file or filename is an empty string";
        case ENOSPC:
            return "ENOSPC: The directory or file system that would contain the new file cannot be expanded, the file does not exist, and the file was to be created";
        case ENOTDIR:
            return "ENOTDIR: A component of the path prefix is not a directory";
        case ENXIO:
            return "ENXIO: The named file is a character special or block special file, and the device associated with this special file does not exist";
        case EOVERFLOW:
            return "EOVERFLOW: The named file is a regular file and the size of the file cannot be represented correctly in an object of type off_t";
        case EROFS:
            return "EROFS: The named file resides on a read-only file system and mode requires write access";
        default:
            return Int32ToString( errorCode );
    }
}

/*-------------------------------------------------------------------------*/

void 
CFileAccess::Open( void )
{GUCEF_TRACE;
        Close();
        
        m_file = fopen( m_filename.C_String() ,
                       m_mode.C_String()    );                      
}

/*-------------------------------------------------------------------------*/

bool
CFileAccess::Open( const CString& file ,
                   const char* mode    )
{
    Close();
    
    m_filename = file;
    m_mode = mode;
    
    _readable = ( strchr( mode, 'r' ) != NULL ) || ( strchr( mode, 'a' ) != NULL );
    _writeable = ( strchr( mode, 'w' ) != NULL ) || ( strchr( mode, 'a' ) != NULL );

    if ( _writeable )
    {
        CString path = StripFilename( file );
        if ( !CreateDirs( path ) )
        {
            GUCEF_DEBUG_LOG( LOGLEVEL_NORMAL, "FileAccess:Open: Failed to recursively create directories" );
            return false;
        }
    }
    if ( _readable )
    {
        _size = Filesize( file.C_String() );
    }
    
    errno = 0;
    m_file = fopen( file.C_String() ,
                    mode            );

    if ( m_file == NULL && 0 != errno )
    {
        GUCEF_DEBUG_LOG( LOGLEVEL_NORMAL, "FileAccess:Open: Failed to open file \"" + file + "\". mode=" + CString(mode) + " error=" + GetErrorString( errno ) );
    }
    
    return m_file != NULL;
}

/*-------------------------------------------------------------------------*/

void 
CFileAccess::Close( void )
{GUCEF_TRACE;

    if ( NULL != m_file )
    { 
            fclose( m_file );
            m_file = NULL;
    }
}

/*-------------------------------------------------------------------------*/

bool 
CFileAccess::Opened( void ) const
{GUCEF_TRACE;

    return m_file != NULL;
}

/*-------------------------------------------------------------------------*/

UInt32 
CFileAccess::Read( void *dest      ,
                    UInt32 esize    ,
                    UInt32 elements )
{GUCEF_TRACE;
    
    if ( NULL != m_file )
    {
        return (UInt32) fread( dest     ,
                               esize    ,
                               elements ,
                               m_file   );
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

UInt64 
CFileAccess::Tell( void ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_file )
    {
        return ftell( m_file );
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

Int32 
CFileAccess::Seek( Int64 offset ,
                   Int32 origin )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_file )
    {
        return fseek( m_file ,
                      (long)offset ,
                      origin );
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

UInt32 
CFileAccess::Setpos( UInt64 position )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_file )
    {
        return fseek( m_file    ,
                      (long)position  ,
                      SEEK_SET  );
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

char 
CFileAccess::GetChar( void )
{GUCEF_TRACE;

    if ( NULL != m_file )
    {
        return (char) fgetc( m_file );
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

bool 
CFileAccess::Eof( void ) const
{GUCEF_TRACE;

    if ( NULL != m_file )
    {
        return feof( m_file ) != 0;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CFileAccess::IsReadable( void ) const
{GUCEF_TRACE;

    return _readable;
}

/*-------------------------------------------------------------------------*/

bool 
CFileAccess::IsWriteable( void ) const
{GUCEF_TRACE;

    return _writeable;
}

/*-------------------------------------------------------------------------*/
       
UInt32 
CFileAccess::Write( const void* srcdata ,
                    UInt32 esize        ,
                    UInt32 elements     )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_file )
    {
        return (UInt32) fwrite( srcdata  , 
                                esize    , 
                                elements , 
                                m_file    );
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

UInt64
CFileAccess::Write( CIOAccess& sourceData ,
                    Int64 bytesToWrite    )
{GUCEF_TRACE;
    
    return CIOAccess::Write( sourceData, bytesToWrite );
}

/*-------------------------------------------------------------------------*/

UInt32
CFileAccess::Write( const CString& string )
{GUCEF_TRACE;

    return CIOAccess::Write( string );
}

/*-------------------------------------------------------------------------*/

bool 
CFileAccess::IsValid( void )
{GUCEF_TRACE;
    
    return File_Exists( m_filename.C_String() ) == 1;
}

/*-------------------------------------------------------------------------*/

UInt64
CFileAccess::GetSize( void ) const
{GUCEF_TRACE;
        
    if ( GUCEF_NULL != m_file )
    {        
        if ( m_file && _writeable )
        {
            fflush( m_file );       
        }
        return Filesize( m_filename.C_String() );           
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

void
CFileAccess::Flush( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_file )
    {
        fflush( m_file );
    }
}

/*-------------------------------------------------------------------------*/

CICloneable* 
CFileAccess::Clone( void ) const
{GUCEF_TRACE;

    if ( IsReadable() )
    {
            return GUCEF_NEW CFileAccess( m_filename, "rb" );
    }
    
    // Cannot be cloned
    return NULL;
}

/*-------------------------------------------------------------------------*/

const CString&
CFileAccess::GetFilename( void ) const
{GUCEF_TRACE;

    return m_filename; 
}

/*-------------------------------------------------------------------------*/

bool
CFileAccess::SetFileToUse( const CString& filename  ,
                           const char* mode         ,
                           bool moveIfCurrentlyOpen )
{GUCEF_TRACE;

    if ( NULL != m_file )
    {
        // We already have a file open.
        fclose( m_file );
        m_file = NULL;

        if ( moveIfCurrentlyOpen )
        {
            if ( !MoveFile( filename, m_filename, true ) )
                return false;
        }
    }

    m_file = fopen( filename.C_String(), mode );
    if ( NULL != m_file )
    {
        m_filename = filename;
        m_mode = mode;
        return true;
    }

    m_filename.Clear();
    m_mode.Clear();
    return false;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
