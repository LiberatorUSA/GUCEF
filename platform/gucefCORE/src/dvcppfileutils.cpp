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

#include <malloc.h>             /* memory management */
#include <stdio.h>              /* standard I/O utils */
#include <string.h>             /* standard string utils */
#include <assert.h>
#include <limits.h>

#ifndef GUCEF_CORE_CONFIG_H
#include "gucefCORE_config.h"     /* build defines */
#define GUCEF_CORE_CONFIG_H
#endif /* GUCEF_CORE_CONFIG_H ? */

#include "dvstrutils.h"         /* My own string utils */

#ifndef GUCEF_CORE_DVFILEUTILS_H
#include "dvfileutils.h"        /* all kinds of file utils */
#define GUCEF_CORE_DVFILEUTILS_H
#endif /* GUCEF_CORE_DVFILEUTILS_H ? */

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
  #include <windows.h>		/* WIN32 API */
  #undef min
  #undef max
  /* #include <dir.h>: obsolete *//* needed for MAXFILE define */
  #include <io.h>                 /* Dir itteration: findfirst ect. */
  #include <direct.h>             /* dir tools */
  #include "Shlwapi.h"
  #define MAX_DIR_LENGTH MAX_PATH
#elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
  #include <dirent.h>             /* needed for dirent strcture */
  #include <unistd.h>             /* POSIX utilities */
  #include <limits.h>             /* Linux OS limits */
  #include <sys/stat.h>           /* needed for stat function */
  #include <errno.h>
  #include <fcntl.h>
  #define MAX_DIR_LENGTH PATH_MAX
#else
  #error Unsupported OS
#endif
#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID )
  #include <android/log.h>
#endif

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#include "dvcppfileutils.h"

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

CDateTime
GetFileModificationTime( const CString& path )
{GUCEF_TRACE;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    ::WIN32_FILE_ATTRIBUTE_DATA data;
    if ( 0 != ::GetFileAttributesEx( path.C_String(), GetFileExInfoStandard, &data ) )
    {
        return CDateTime( data.ftLastWriteTime );
    }
    return -1;

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    struct stat buf;

    /* Get File Statistics for stat.c. */
    if( stat( path.C_String(), &buf ) == 0 )
    {
        /* get the date/time last modified */
        return CDateTime( buf.st_mtime, true );
    }
    return -1;

    #else

    /*
     *  Unsupported platform
     */
    return CDateTime();

    #endif
}


/*-------------------------------------------------------------------------*/

CDateTime
GetFileCreationTime( const CString& path )
{GUCEF_TRACE;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    WIN32_FILE_ATTRIBUTE_DATA data;
    if ( 0 != GetFileAttributesEx( path.C_String(), GetFileExInfoStandard, &data ) )
    {
        return CDateTime( data.ftCreationTime );
    }
    return -1;

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    struct stat buf;

    /* Get File Statistics for stat.c. */
    if( stat( path.C_String(), &buf ) == 0 )
    {
        /* get the date/time the file was created */
        return CDateTime( buf.st_ctime, true );
    }
    return -1;

    #else

    /*
     *  Unsupported platform
     */
    return CDateTime();

    #endif
}

/*-------------------------------------------------------------------------*/

bool
GetFileMetaData( const CString& filePath     ,
                 CResourceMetaData& metaData )
{GUCEF_TRACE;

    metaData.Clear();

    if ( FileExists( filePath ) )
    {
        metaData.resourceExists = true;

        #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

        WIN32_FILE_ATTRIBUTE_DATA data;
        if ( 0 != GetFileAttributesEx( filePath.C_String(), GetFileExInfoStandard, &data ) )
        {
            metaData.creationDateTime = CDateTime( data.ftCreationTime );
            metaData.hasCreationDateTime = true;
            metaData.modifiedDateTime = CDateTime( data.ftLastWriteTime );
            metaData.hasModifiedDateTime = true;
            metaData.lastAccessedDateTime = CDateTime( data.ftLastAccessTime );
            metaData.hasLastAccessedDateTime = true;

            ULARGE_INTEGER ul; // <- we use this struct for byte allignment
            ul.HighPart = data.nFileSizeHigh;
            ul.LowPart = data.nFileSizeLow;
            metaData.resourceSizeInBytes = (UInt64) ul.QuadPart;
            metaData.hasResourceSizeInBytes = true;
        }

        // true no matter what since access rights also factor into it
        // the meta data class has its own flags
        return true;

        #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

        struct stat buf;

        // Get File Statistics for stat.c.
        if( stat( filePath.C_String(), &buf ) == 0 )
        {
            metaData.creationDateTime = CDateTime( buf.st_ctime, true );
            metaData.hasCreationDateTime = true;
            metaData.modifiedDateTime = CDateTime( buf.st_mtim, true );
            metaData.hasModifiedDateTime = true;
            metaData.lastAccessedDateTime = CDateTime( buf.st_atim, true );
            metaData.hasLastAccessedDateTime = true;
            metaData.resourceSizeInBytes = (UInt64) buf.st_size;
            metaData.hasResourceSizeInBytes = true;
        }

        // true no matter what since access rights also factor into it
        // the meta data class has its own flags
        return true;

        #else

        /*
         *  Unsupported platform
         */
        return false;

        #endif
    }
    else
    {
        metaData.resourceExists = false;
        return false;
    }
}

/*-------------------------------------------------------------------------*/

bool
CreateDirs( const CString& path )
{GUCEF_TRACE;

    CString actualPath = RelativePath( path );
    return 0 != Create_Directory( actualPath.C_String() );
}

/*-------------------------------------------------------------------------*/

bool
DeleteFile( const CString& path )
{GUCEF_TRACE;

    CString actualPath = RelativePath( path );
    return 0 != Delete_File( actualPath.C_String() );
}

/*-------------------------------------------------------------------------*/

bool
MoveFile( const CString& oldPath ,
          const CString& newPath ,
          const bool overwrite   )
{GUCEF_TRACE;

    CString actualOldPath = RelativePath( oldPath );
    CString actualNewPath = RelativePath( newPath );

    if ( !CreatePathDirectories( actualNewPath ) )
        return false;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    std::wstring wActualOldPath = ToWString( actualOldPath );
    std::wstring wActualNewPath = ToWString( actualNewPath );

    DWORD flags = MOVEFILE_WRITE_THROUGH | MOVEFILE_COPY_ALLOWED;
    if ( overwrite )
        flags = flags | MOVEFILE_REPLACE_EXISTING;

    BOOL result = ::MoveFileExW( wActualOldPath.c_str(), wActualNewPath.c_str(), flags );
    return result == TRUE;

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    struct stat originalPermissions;
    if ( 0 != ::stat( actualOldPath.C_String(), &originalPermissions ) ) 
        return 0;

    ::chmod( actualOldPath.C_String(), 0777 );

    if ( 0 != ::rename( actualOldPath.C_String(), actualNewPath.C_String() ) )
    {
        return 0;
    }

    ::chmod( dst, originalPermissions.st_mode );
    return 1;

    #else

    // catch all expensive implementation
    if ( CopyFile( dst, src, overwrite ) )
    {
    	return DeleteFile( src );
    }
    return false;

    #endif
}

/*-------------------------------------------------------------------------*/

bool
FileExists( const CString& filename )
{GUCEF_TRACE;

    return 0 != File_Exists( filename.C_String() );
}

/*-------------------------------------------------------------------------*/

bool
DirExists( const CString& path )
{GUCEF_TRACE;

    return 0 != Dir_Exists( path.C_String() );
}

/*-------------------------------------------------------------------------*/

bool
CreatePathDirectories( const CString& path )
{GUCEF_TRACE;

    Int32 delimIndex = path.HasChar( GUCEF_DIRSEPCHAR, false );
    if ( delimIndex < 0 )
        delimIndex = path.HasChar( GUCEF_DIRSEPCHAROPPOSITE, false );
    
    if ( delimIndex >= 0 )
    {
        CString dirs = path.SubstrToIndex( (UInt32) delimIndex, true );
        return CreateDirs( dirs );
    }
    return true;
}

/*-------------------------------------------------------------------------*/

UInt64
FileSize( const CString& filename )
{GUCEF_TRACE;

    return Filesize( filename.C_String() );
}

/*-------------------------------------------------------------------------*/

bool
GetFileSystemStorageVolumeInformationByDirPath( TStorageVolumeInformation& info, const CString& path )
{GUCEF_TRACE;

    memset( &info, 0, sizeof( info ) );
    if ( path.IsNULLOrEmpty() )
        return false;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    CString dirPath = RelativePath( path );
    if ( dirPath.Length() != dirPath.HasChar( '\\', false )+1 )
    {
        // A trailing backslash is required for the below Win32 API
        dirPath += '\\';
    }
    
    std::wstring wActualPath = ToWString( dirPath );

    ULARGE_INTEGER freeBytesAvailableToCaller;
    ULARGE_INTEGER totalNumberOfBytes;
    ULARGE_INTEGER totalNumberOfFreeBytes;
    BOOL result = ::GetDiskFreeSpaceExW( wActualPath.c_str()         , 
                                         &freeBytesAvailableToCaller , 
                                         &totalNumberOfBytes         , 
                                         &totalNumberOfFreeBytes     );
    if ( TRUE == result )
    {
        info.freeBytesAvailableToCaller = freeBytesAvailableToCaller.QuadPart;
        info.totalNumberOfBytes = totalNumberOfBytes.QuadPart;
        info.totalNumberOfFreeBytes = totalNumberOfFreeBytes.QuadPart;
        return true;
    }
    return false;

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    return false;

    #else

    return false;

    #endif
}

/*-------------------------------------------------------------------------*/

bool
GetFileSystemStorageVolumeInformationByVolumeId( TStorageVolumeInformation& info, const CString& volumeId )
{GUCEF_TRACE;

    memset( &info, 0, sizeof( info ) );
    if ( volumeId.IsNULLOrEmpty() )
        return false;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    std::wstring wVolumeId = ToWString( volumeId );

    DWORD listSizeInWChars = 0;
    BOOL result = ::GetVolumePathNamesForVolumeNameW( wVolumeId.c_str() ,
                                                      NULL              ,
                                                      0                 ,
                                                      &listSizeInWChars );

    // Get a buffer that receives the list of drive letters and mounted folder paths
    CORE::CDynamicBuffer buffer( (UInt32) listSizeInWChars * sizeof(wchar_t), true );
    result = ::GetVolumePathNamesForVolumeNameW( wVolumeId.c_str()              ,
                                                 (LPWCH) buffer.GetBufferPtr()  ,
                                                 (DWORD) buffer.GetBufferSize() ,
                                                 &listSizeInWChars              );
    if ( TRUE != result )
        return false;
    buffer.SetDataSize( (UInt32) ( listSizeInWChars * sizeof(wchar_t) ) );

    // The list is an array of null-terminated strings terminated by an additional NULL character
    // We only need 1 for the volume info function
    std::wstring wVolumeRootPath = buffer.AsConstTypePtr< wchar_t >();    
    return GetFileSystemStorageVolumeInformationByDirPath( info, ToString( wVolumeRootPath ) );
    
    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    return false;

    #else

    return false;

    #endif
}

/*-------------------------------------------------------------------------*/

bool
GetFileSystemStorageVolumeIdByDirPath( CString& volumeId, const CString& path )
{GUCEF_TRACE;

    volumeId.Clear();

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    CString dirPath = RelativePath( path );
    if ( dirPath.Length() != dirPath.HasChar( '\\', false )+1 )
    {
        // A trailing backslash is required for the below Win32 API
        dirPath += '\\';
    }

    std::wstring wActualPath = ToWString( dirPath );
    if ( TRUE != ::PathIsRootW( wActualPath.c_str() ) )
    {
        // the win32 function below only works with roots
        if ( TRUE != ::PathStripToRootW( (wchar_t*) wActualPath.c_str() ) )
        {
            // failed to determine root
            return false;
        }
    }

    wchar_t volumeNameBuffer[ MAX_PATH+1 ];
    BOOL result = ::GetVolumeNameForVolumeMountPointW( wActualPath.c_str()     , 
                                                       volumeNameBuffer        ,  
                                                       MAX_PATH                );
    if ( TRUE == result )
    {
        volumeId = volumeNameBuffer;
        return true;
    }
    return false;

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    return false;

    #else

    return false;

    #endif
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
