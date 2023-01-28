/*
 *  vfspluginZIP: Generic GUCEF VFS plugin for ZIP archives
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
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

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#include "vfspluginZIP_CZipIOAccess.h"

// Include the zzip header last because it performs some funky type
// magic which will screw up types in other headers
#ifndef _ZZIP_ZZIP_H
#include "zzip.h"
#define _ZZIP_ZZIP_H
#endif /* _ZZIP_ZZIP_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace VFSPLUGIN {
namespace ZIP {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CZipIOAccess::CZipIOAccess( ZZIP_FILE* zzipFile     , 
                            size_t uncompressedSize )
    : CORE::CIOAccess()          ,
      m_zzipFile( zzipFile )     ,
      m_size( uncompressedSize )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CZipIOAccess::~CZipIOAccess()
{GUCEF_TRACE;

    Close();
}

/*-------------------------------------------------------------------------*/

void
CZipIOAccess::Open( void )
{GUCEF_TRACE;

    // N/A
}

/*-------------------------------------------------------------------------*/

void
CZipIOAccess::Close( void )
{GUCEF_TRACE;

    zzip_file_close( m_zzipFile );
    m_zzipFile = NULL;
}

/*-------------------------------------------------------------------------*/

bool
CZipIOAccess::Opened( void ) const
{GUCEF_TRACE;

    return NULL != m_zzipFile;
}

/*-------------------------------------------------------------------------*/

VFS::CString
CZipIOAccess::ReadLine( void )
{GUCEF_TRACE;
    
    return VFS::CString();
}

/*-------------------------------------------------------------------------*/

VFS::CString
CZipIOAccess::ReadString( void )
{GUCEF_TRACE;

    return VFS::CString();
}

/*-------------------------------------------------------------------------*/

CORE::UInt32
CZipIOAccess::Read( void *dest            ,
                    CORE::UInt32 esize    ,
                    CORE::UInt32 elements )
{GUCEF_TRACE;

    return (CORE::UInt32) zzip_file_read( m_zzipFile, (char*)dest, esize*elements );
}

/*-------------------------------------------------------------------------*/

CORE::UInt64
CZipIOAccess::Tell( void ) const
{GUCEF_TRACE;

    return (CORE::UInt64) zzip_tell( m_zzipFile );
}

/*-------------------------------------------------------------------------*/

CORE::Int32
CZipIOAccess::Seek( CORE::Int64 offset ,
                    CORE::Int32 origin )
{GUCEF_TRACE;

    return zzip_seek( m_zzipFile, static_cast<zzip_off_t>( offset ), origin );
}

/*-------------------------------------------------------------------------*/

char
CZipIOAccess::GetChar( void )
{GUCEF_TRACE;

    char tmp = 0;
    Read( &tmp, 1, 1 );
    return tmp;
}

/*-------------------------------------------------------------------------*/

bool
CZipIOAccess::Eof( void ) const
{GUCEF_TRACE;

    return (zzip_tell( m_zzipFile ) >= static_cast<zzip_off_t>( m_size ) );
}

/*-------------------------------------------------------------------------*/
    
bool
CZipIOAccess::IsReadable( void ) const
{GUCEF_TRACE;
    
    return true;
}

/*-------------------------------------------------------------------------*/
    
bool
CZipIOAccess::IsWriteable( void ) const
{GUCEF_TRACE;

    return false;
}
    
/*-------------------------------------------------------------------------*/

bool
CZipIOAccess::IsValid( void )
{GUCEF_TRACE;

    return Opened();
}

/*-------------------------------------------------------------------------*/
    
CORE::UInt64
CZipIOAccess::GetSize( void ) const
{GUCEF_TRACE;

    return (CORE::UInt64) m_size;
}

/*-------------------------------------------------------------------------*/
    
void
CZipIOAccess::Flush( void )
{GUCEF_TRACE;

    // N/A
}    

/*-------------------------------------------------------------------------*/

CORE::CICloneable*
CZipIOAccess::Clone( void ) const
{GUCEF_TRACE;

    return NULL;
}
    
/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace ZIP */
}; /* namespace VFSPLUGIN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
