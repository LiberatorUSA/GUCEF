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
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#define DOS_EOF_CHAR (char) 26

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
    
    _size = Filesize( file.C_String() );
    m_file = fopen( file.C_String() ,
                    mode            );

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

CString 
CFileAccess::ReadLine( void )
{GUCEF_TRACE;
    
    if ( NULL != m_file )
    {
        CString resultStr;
        while ( feof( m_file ) == 0 )
        {
            int currentCharInt = fgetc( m_file );
            if ( EOF != currentCharInt )
            {
                char currentChar = (char) currentCharInt;
                if ( currentChar == '\0'        ||
                     currentChar == '\n'        || 
                     currentChar == '\r'        ||
                     currentChar == DOS_EOF_CHAR )
                {
                    break;
                }

                resultStr += currentChar;
            }
            else
            {
                return resultStr;
            }
        }
        return resultStr;
    }     
        
    return CString();
}

/*-------------------------------------------------------------------------*/

CString 
CFileAccess::ReadString( void )
{GUCEF_TRACE;

    if ( NULL != m_file )
    {
        CString resultStr;
        while ( feof( m_file ) != 0 )
        {
            int currentCharInt = fgetc( m_file );
            if ( EOF != currentCharInt )
            {
                char currentChar = (char) currentCharInt;
                if ( currentChar == '\0'          ||
                     currentChar == '\n'          || 
                     currentChar == '\r'          ||
                     currentChar ==  DOS_EOF_CHAR ||
                     currentChar == '\t'          ||
                     currentChar == ' '            )
                {
                    break;
                }

                resultStr += currentChar;
            }
            else
            {
                return resultStr;
            }
        }
        return resultStr;
    }     
        
    return CString();
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

UInt32 
CFileAccess::Tell( void ) const
{GUCEF_TRACE;

    if ( NULL != m_file )
    {
        return ftell( m_file );
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

Int32 
CFileAccess::Seek( Int32 offset ,
                   Int32 origin )
{GUCEF_TRACE;

    if ( NULL != m_file )
    {
        return fseek( m_file ,
                      offset ,
                      origin );
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

UInt32 
CFileAccess::Setpos( UInt32 position )
{GUCEF_TRACE;

    if ( NULL != m_file )
    {
        return fseek( m_file    ,
                      position  ,
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

    if ( NULL != m_file )
    {
        return (UInt32) fwrite( srcdata  , 
                                esize    , 
                                elements , 
                                m_file    );
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

UInt32
CFileAccess::Write( CIOAccess& sourceData )
{GUCEF_TRACE;
    
    return CIOAccess::Write( sourceData );
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

UInt32 
CFileAccess::GetSize( void ) const
{GUCEF_TRACE;
        
    if ( NULL != m_file )
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

    if ( NULL != m_file )
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
            return new CFileAccess( m_filename, "rb" );
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
            Move_File( filename.C_String(), m_filename.C_String() );
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
