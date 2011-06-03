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

#include "zzip.h"

#ifndef GUCEF_CORE_DVMD5UTILS_H
#include "dvmd5utils.h"
#define GUCEF_CORE_DVMD5UTILS_H
#endif /* GUCEF_CORE_DVMD5UTILS_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#include "CDynamicBufferAccess.h"
#define GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#endif /* GUCEF_CORE_CDYNAMICBUFFERACCESS_H ? */

#ifndef GUCEF_CORE_CSUBFILEACCESS_H
#include "gucefCORE_CSubFileAccess.h"
#define GUCEF_CORE_CSUBFILEACCESS_H
#endif /* GUCEF_CORE_CSUBFILEACCESS_H ? */

#ifndef GUCEF_CORE_CLOGMANAGER_H
#include "CLogManager.h"
#define GUCEF_CORE_CLOGMANAGER_H
#endif /* GUCEF_CORE_CLOGMANAGER_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#include "vfspluginZIP_CZipIOAccess.h"

#include "vfspluginZIP_CZIPArchive.h"

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

/// Utility method to format out zzip errors
VFS::CString
getZzipErrorDescription( zzip_error_t zzipError )
{GUCEF_TRACE;

    VFS::CString errorMsg;
    switch ( zzipError )
    {
    case ZZIP_NO_ERROR:
        break;
    case ZZIP_OUTOFMEM:
        errorMsg = "Out of memory.";
        break;
    case ZZIP_DIR_OPEN:
    case ZZIP_DIR_STAT:
    case ZZIP_DIR_SEEK:
    case ZZIP_DIR_READ:
        errorMsg = "Unable to read zip file.";
        break;
    case ZZIP_UNSUPP_COMPR:
        errorMsg = "Unsupported compression format.";
        break;
    case ZZIP_CORRUPTED:
        errorMsg = "Corrupted archive.";
        break;
    default:
        errorMsg = "Unknown error.";
        break;
    };

    return errorMsg;
}

/*-------------------------------------------------------------------------*/

CZIPArchive::CZIPArchive( void )
    : CIArchive()       ,
      m_archiveName()   ,
      m_archivePath()   ,
      m_zipRoot( NULL ) ,
      m_fileList()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CZIPArchive::~CZIPArchive()
{GUCEF_TRACE;

    UnloadArchive();
}

/*-------------------------------------------------------------------------*/

VFS::CIArchive::CVFSHandlePtr
CZIPArchive::GetFile( const VFS::CString& file      ,
                      const char* mode              ,
                      const VFS::UInt32 memLoadSize ,
                      const bool overwrite          )
{GUCEF_TRACE;

    // We only support read only modes
    if ( *mode != 'r' ) return CVFSHandlePtr();

    CORE::CIOAccess* fileAccess = LoadFile( file, memLoadSize );
    if ( NULL != fileAccess )
    {
        // Create path to file
        CORE::CString filePath = m_archivePath;
        CORE::AppendToPath( filePath, file );

        // Construct & return handle
        return VFS::CVFS::CVFSHandlePtr( new VFS::CVFSHandle( fileAccess, file, filePath ), this );
    }
    return VFS::CVFS::CVFSHandlePtr();
}

/*-------------------------------------------------------------------------*/

void
CZIPArchive::GetList( TStringSet& outputList       ,
                      const VFS::CString& location ,
                      bool recursive               ,
                      bool includePathInFilename   ,
                      const VFS::CString& filter   ,
                      bool addFiles                ,
                      bool addDirs                 ) const
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CZIPArchive::FileExists( const VFS::CString& filePath ) const
{GUCEF_TRACE;

    ZZIP_STAT zstat;
    int res = zzip_dir_stat( m_zipRoot, filePath.C_String(), &zstat, ZZIP_CASEINSENSITIVE);

    return (res == ZZIP_NO_ERROR);
}

/*-------------------------------------------------------------------------*/

VFS::UInt32
CZIPArchive::GetFileSize( const VFS::CString& filePath ) const
{GUCEF_TRACE;

    if ( NULL != m_zipRoot )
    {
        // Get uncompressed size too
        ZZIP_STAT zstat;
        zzip_dir_stat( m_zipRoot, filePath.C_String(), &zstat, ZZIP_CASEINSENSITIVE );

        return zstat.st_size;
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

time_t
CZIPArchive::GetFileModificationTime( const VFS::CString& filePath ) const
{
    // @todo: makeme
    return 0;
}

/*-------------------------------------------------------------------------*/

CORE::CIOAccess*
CZIPArchive::LoadFile( const VFS::CString& file      ,
                       const VFS::UInt32 memLoadSize ) const
{GUCEF_TRACE;

    // Check for failed archive
    if ( NULL == m_zipRoot ) return NULL;

    // Get non const access, no worries, it wont be changed
    ZZIP_DIR* zipRoot = const_cast< ZZIP_DIR* >( m_zipRoot );

    // Format not used here (always binary)
    ZZIP_FILE* zipFile = zzip_file_open( zipRoot, file.C_String(), ZZIP_ONLYZIP | ZZIP_CASELESS );
    if ( NULL == zipFile )
    {
        int zerr = zzip_error( zipRoot );
        VFS::CString zzDesc = getZzipErrorDescription( (zzip_error_t)zerr );
        GUCEF_SYSTEM_LOG( 0, m_archiveName + ": - Unable to open file " + file + ", error was '" + zzDesc + "'" );

        // return null pointer
        return NULL;
    }

    // Get uncompressed size too
    ZZIP_STAT zstat;
    zzip_dir_stat( zipRoot, file.C_String(), &zstat, ZZIP_CASEINSENSITIVE );

    CZipIOAccess* fileAccess = new CZipIOAccess( zipFile, zstat.st_size );

    // We will not load anything in memory for now (ignore memLoadSize)
    return fileAccess;
}

/*-------------------------------------------------------------------------*/

VFS::CString
CZIPArchive::GetFileHash( const VFS::CString& file ) const
{GUCEF_TRACE;

    CORE::CIOAccess* fileAccess = LoadFile( file, 102400 );
    if ( NULL != fileAccess )
    {
        VFS::UInt8 digest[ 16 ];
        if ( 0 != CORE::md5fromfile( fileAccess->CStyleAccess() ,
                                     digest                     ) )
        {
            delete fileAccess;

            char md5_str[ 48 ];
            CORE::md5tostring( digest, md5_str );
            VFS::CString md5Str;
            md5Str.Set( md5_str, 48 );
            return md5Str;
        }

        delete fileAccess;
    }
    return VFS::CString();
}

/*-------------------------------------------------------------------------*/

const VFS::CString&
CZIPArchive::GetArchiveName( void ) const
{GUCEF_TRACE;

    return m_archiveName;
}

/*-------------------------------------------------------------------------*/

bool
CZIPArchive::IsWriteable( void ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

void
CZIPArchive::CheckZzipError( int zzipError                 ,
                             const VFS::CString& operation ) const
{GUCEF_TRACE;

    if ( zzipError != ZZIP_NO_ERROR )
    {
        VFS::CString errorMsg = getZzipErrorDescription( static_cast<zzip_error_t>( zzipError ) );
        GUCEF_ERROR_LOG( 0, "Archive " + m_archiveName + ": - error whilst " + operation + ": " + errorMsg );
    }
}

/*-------------------------------------------------------------------------*/

bool
CZIPArchive::LoadArchive( const VFS::CString& archiveName ,
                          const VFS::CString& archivePath ,
                          const bool writableRequest      )
{GUCEF_TRACE;

    // We do not support writable ZIP archives
    if ( writableRequest ) return false;

    if ( NULL == m_zipRoot )
    {
        zzip_error_t zzipError;
        m_zipRoot = zzip_dir_open( archivePath.C_String(), &zzipError );
        CheckZzipError( zzipError, "opening archive" );

        if ( NULL == m_zipRoot )
        {
            return false;
        }

        // Cache names
        ZZIP_DIRENT zzipEntry;
        while ( zzip_dir_read( m_zipRoot, &zzipEntry ) )
        {
            TZipEntry info;

            // Get the entry name
            info.path = zzipEntry.d_name;

            // Get sizes
            info.compressedSize = static_cast<size_t>(zzipEntry.d_csize);
            info.uncompressedSize = static_cast<size_t>(zzipEntry.st_size);

            // Check for folder entries
            VFS::CString filename = CORE::ExtractFilename( info.path );
            if ( filename.Length() == 0 )
            {
                info.compressedSize = 0;
                info.isDir = true;
            }
            else
            {
                info.isDir = false;
            }

            m_fileList.push_back( info );
        }
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CZIPArchive::UnloadArchive( void )
{GUCEF_TRACE;

    if ( NULL != m_zipRoot )
    {
        zzip_dir_close( m_zipRoot );
        m_zipRoot = NULL;

        m_fileList.clear();
    }
    return true;
}

/*-------------------------------------------------------------------------*/

void
CZIPArchive::DestroyObject( VFS::CVFSHandle* objectToBeDestroyed )
{GUCEF_TRACE;

    delete objectToBeDestroyed;
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
