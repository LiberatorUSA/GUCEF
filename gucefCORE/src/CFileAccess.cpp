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
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CFileAccess::CFileAccess( const CString& file           ,
                          const char* mode /* = "rb" */ )
        : _filename( file ) ,
          m_mode( mode )
{GUCEF_TRACE;
        
        _readable = ( strchr( mode, 'r' ) != NULL ) || ( strchr( mode, 'a' ) != NULL );
        _writeable = ( strchr( mode, 'w' ) != NULL ) || ( strchr( mode, 'a' ) != NULL );
        
        _size = Filesize( file.C_String() );
        _file = fopen( file.C_String() ,
                       mode            );                      
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
        
        _file = fopen( _filename.C_String() ,
                       m_mode.C_String()    );                      
}

/*-------------------------------------------------------------------------*/

void 
CFileAccess::Close( void )
{GUCEF_TRACE;
        if ( _file )
        { 
                fclose( _file );
                _file = NULL;
        }
}

/*-------------------------------------------------------------------------*/

bool 
CFileAccess::Opened( void ) const
{GUCEF_TRACE;
        return _file != NULL;
}

/*-------------------------------------------------------------------------*/

CString 
CFileAccess::ReadLine( void )
{GUCEF_TRACE;

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
{GUCEF_TRACE;
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
{GUCEF_TRACE;
        return (UInt32) fread( dest     ,
                               esize    ,
                               elements ,
                               _file    );
}

/*-------------------------------------------------------------------------*/

UInt32 
CFileAccess::Tell( void ) const
{GUCEF_TRACE;
        return ftell( _file );
}

/*-------------------------------------------------------------------------*/

Int32 
CFileAccess::Seek( Int32 offset ,
                   Int32 origin )
{GUCEF_TRACE;
        return fseek( _file     ,
                      offset    ,
                      origin    );
}

/*-------------------------------------------------------------------------*/

UInt32 
CFileAccess::Setpos( UInt32 position )
{GUCEF_TRACE;
        return fseek( _file     ,
                      position  ,
                      SEEK_SET  );
}

/*-------------------------------------------------------------------------*/

char 
CFileAccess::GetChar( void )
{GUCEF_TRACE;
        return (char) fgetc( _file );
}

/*-------------------------------------------------------------------------*/

bool 
CFileAccess::Eof( void ) const
{GUCEF_TRACE;

    return feof( _file ) != 0;
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
        return (UInt32) fwrite( srcdata  , 
                                esize    , 
                                elements , 
                                _file    );
}

/*-------------------------------------------------------------------------*/

UInt32
CFileAccess::Write( CIOAccess& sourceData )
{GUCEF_TRACE;
    
    return CIOAccess::Write( sourceData );
}

/*-------------------------------------------------------------------------*/

bool 
CFileAccess::IsValid( void )
{GUCEF_TRACE;
        return File_Exists( _filename.C_String() ) == 1;
}

/*-------------------------------------------------------------------------*/

UInt32 
CFileAccess::GetSize( void ) const
{GUCEF_TRACE;
        if ( _file && _writeable )
        {
                fflush( _file );       
        }
        return Filesize( _filename.C_String() );           
}

/*-------------------------------------------------------------------------*/

void
CFileAccess::Flush( void )
{GUCEF_TRACE;

    fflush( _file );
}

/*-------------------------------------------------------------------------*/

CICloneable* 
CFileAccess::Clone( void ) const
{GUCEF_TRACE;
        if ( IsReadable() )
        {
                return new CFileAccess( _filename, "rb" );
        }
        
        // Cannot be cloned
        return NULL;
}

/*-------------------------------------------------------------------------*/

const CString&
CFileAccess::GetFilename( void ) const
{GUCEF_TRACE;

    return _filename; 
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
