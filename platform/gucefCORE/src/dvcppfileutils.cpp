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

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#include "dvstrutils.h"         /* My own string utils */

#ifndef GUCEF_CORE_DVFILEUTILS_H
#include "dvfileutils.h"        /* all kinds of file utils */
#define GUCEF_CORE_DVFILEUTILS_H
#endif /* GUCEF_CORE_DVFILEUTILS_H ? */

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
  #include <windows.h>		/* WIN32 API */
  #include <winioctl.h>
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

    std::wstring wFilepath = ToWString( path );

    ::WIN32_FILE_ATTRIBUTE_DATA data;
    if ( 0 != ::GetFileAttributesExW( wFilepath.c_str(), GetFileExInfoStandard, &data ) )
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

    std::wstring wFilepath = ToWString( path );

    WIN32_FILE_ATTRIBUTE_DATA data;
    if ( 0 != GetFileAttributesExW( wFilepath.c_str(), GetFileExInfoStandard, &data ) )
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
SetFileMetaData( const CString& filePath           ,
                 const CResourceMetaData& metaData )
{GUCEF_TRACE;

    if ( FileExists( filePath ) )
    {
        #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

        bool totalSuccess = true;
        std::wstring wFilepath = ToWString( filePath );

        // Get the current file attributes
        // we only want to change the flags we have and keep the rest as-is
        CResourceMetaData originalMetaData;
        if ( !GetFileMetaData( filePath, originalMetaData ) )
            return false;

        WIN32_FILE_ATTRIBUTE_DATA data;
        memset( &data, 0, sizeof data );

        // Convert boolean flags to file attribute flags
        if ( metaData.hasIsHidden ) { metaData.isHidden ? data.dwFileAttributes |= FILE_ATTRIBUTE_HIDDEN : data.dwFileAttributes &= ~FILE_ATTRIBUTE_HIDDEN; }
        else { originalMetaData.isHidden ? data.dwFileAttributes |= FILE_ATTRIBUTE_HIDDEN : data.dwFileAttributes &= ~FILE_ATTRIBUTE_HIDDEN; }

        if ( metaData.hasIsReadOnly ) { metaData.isReadOnly ? data.dwFileAttributes |= FILE_ATTRIBUTE_READONLY : data.dwFileAttributes &= ~FILE_ATTRIBUTE_READONLY; }
        else { originalMetaData.isReadOnly ? data.dwFileAttributes |= FILE_ATTRIBUTE_READONLY : data.dwFileAttributes &= ~FILE_ATTRIBUTE_READONLY; }

        if ( metaData.hasIsSystemResource ) { metaData.isSystemResource ? data.dwFileAttributes |= FILE_ATTRIBUTE_SYSTEM : data.dwFileAttributes &= ~FILE_ATTRIBUTE_SYSTEM; }
        else { originalMetaData.isSystemResource ? data.dwFileAttributes |= FILE_ATTRIBUTE_SYSTEM : data.dwFileAttributes &= ~FILE_ATTRIBUTE_SYSTEM; }

        if ( metaData.hasIsArchive ) { metaData.isArchive ? data.dwFileAttributes |= FILE_ATTRIBUTE_ARCHIVE : data.dwFileAttributes &= ~FILE_ATTRIBUTE_ARCHIVE; }
        else { originalMetaData.isArchive ? data.dwFileAttributes |= FILE_ATTRIBUTE_ARCHIVE : data.dwFileAttributes &= ~FILE_ATTRIBUTE_ARCHIVE; }

        if ( metaData.hasIsCompressed ) { metaData.isCompressed ? data.dwFileAttributes |= FILE_ATTRIBUTE_COMPRESSED : data.dwFileAttributes &= ~FILE_ATTRIBUTE_COMPRESSED; }
        else { originalMetaData.isCompressed ? data.dwFileAttributes |= FILE_ATTRIBUTE_COMPRESSED : data.dwFileAttributes &= ~FILE_ATTRIBUTE_COMPRESSED; }

        if ( metaData.hasIsEncrypted ) { metaData.isEncrypted ? data.dwFileAttributes |= FILE_ATTRIBUTE_ENCRYPTED : data.dwFileAttributes &= ~FILE_ATTRIBUTE_ENCRYPTED; }
        else { originalMetaData.isEncrypted ? data.dwFileAttributes |= FILE_ATTRIBUTE_ENCRYPTED : data.dwFileAttributes &= ~FILE_ATTRIBUTE_ENCRYPTED; }

        if ( metaData.hasIsTemporary ) { metaData.isTemporary ? data.dwFileAttributes |= FILE_ATTRIBUTE_TEMPORARY : data.dwFileAttributes &= ~FILE_ATTRIBUTE_TEMPORARY; }
        else { originalMetaData.isTemporary ? data.dwFileAttributes |= FILE_ATTRIBUTE_TEMPORARY : data.dwFileAttributes &= ~FILE_ATTRIBUTE_TEMPORARY; }

        //if ( metaData.hasIsOffline ) { metaData.isOffline ? data.dwFileAttributes |= FILE_ATTRIBUTE_OFFLINE : data.dwFileAttributes &= ~FILE_ATTRIBUTE_OFFLINE; }
        //else
             { originalMetaData.hasIsOffline ? data.dwFileAttributes |= FILE_ATTRIBUTE_OFFLINE : data.dwFileAttributes &= ~FILE_ATTRIBUTE_OFFLINE; }

        // Set the new file attributes
        BOOL result = ::SetFileAttributesW( wFilepath.c_str(), data.dwFileAttributes );
        if ( 0 == result )
        {
            GUCEF_DEBUG_LOG( LOGLEVEL_NORMAL, "SetFileMetaData: SetFileAttributesW failed with error code: " + ToString( (UInt32) ::GetLastError() ) );
            totalSuccess = false;
        }

        // Convert date time to windows file time as needed
        metaData.hasCreationDateTime ? data.ftCreationTime = metaData.creationDateTime.ToWindowsFiletime() : data.ftCreationTime = originalMetaData.creationDateTime.ToWindowsFiletime();
        metaData.hasModifiedDateTime ? data.ftLastWriteTime = metaData.modifiedDateTime.ToWindowsFiletime() : data.ftLastWriteTime = originalMetaData.modifiedDateTime.ToWindowsFiletime();
        metaData.hasLastAccessedDateTime ? data.ftLastAccessTime = metaData.lastAccessedDateTime.ToWindowsFiletime() : data.ftLastAccessTime = originalMetaData.lastAccessedDateTime.ToWindowsFiletime();

        // Open the file for writing attributes without needing a handle for data read/write operations
        // we dont want to touch the file contents
        HANDLE hFile = ::CreateFileW( wFilepath.c_str(), FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
        if ( INVALID_HANDLE_VALUE != hFile )
        {
            if ( 0 == ::SetFileTime( hFile, &data.ftCreationTime, &data.ftLastAccessTime, &data.ftLastWriteTime ) )
            {
                GUCEF_DEBUG_LOG( LOGLEVEL_NORMAL, "SetFileMetaData: SetFileTime failed with error code: " + ToString( (UInt32) ::GetLastError() ) );
                totalSuccess = false;
            }
            ::CloseHandle( hFile );
            hFile = INVALID_HANDLE_VALUE;
        }
        else
        {
            GUCEF_DEBUG_LOG( LOGLEVEL_NORMAL, "SetFileMetaData: CreateFileW for FILE_WRITE_ATTRIBUTES failed with error code: " + ToString( (UInt32) ::GetLastError() ) );
            totalSuccess = false;
        }

        return totalSuccess;

        #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

        /*
         *  Unsupported platform  - @TODO
         */
        return false;

        #else

        /*
         *  Unsupported platform
         */
        return false;

        #endif
    }

    // Cannot set meta-data on a non-existing file
    return false;
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

        std::wstring wFilepath = ToWString( filePath );

        WIN32_FILE_ATTRIBUTE_DATA data;
        if ( 0 != ::GetFileAttributesExW( wFilepath.c_str(), GetFileExInfoStandard, &data ) )
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

            data.dwFileAttributes & FILE_ATTRIBUTE_READONLY ? metaData.isReadOnly = true : metaData.isReadOnly = false;
            metaData.hasIsReadOnly = true;
            data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN ? metaData.isHidden = true : metaData.isHidden = false;
            metaData.hasIsHidden = true;
            data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM || data.dwFileAttributes & FILE_ATTRIBUTE_DEVICE ? metaData.isSystemResource = true : metaData.isSystemResource = false;
            metaData.hasIsSystemResource = true;
            data.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE ? metaData.isArchive = true : metaData.isArchive = false;
            metaData.hasIsArchive = true;
            data.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED ? metaData.isCompressed = true : metaData.isCompressed = false;
            metaData.hasIsCompressed = true;
            data.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED ? metaData.isEncrypted = true : metaData.isEncrypted = false;
            metaData.hasIsEncrypted = true;
            data.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY ? metaData.isTemporary = true : metaData.isTemporary = false;
            metaData.hasIsTemporary = true;
            data.dwFileAttributes & FILE_ATTRIBUTE_OFFLINE ? metaData.isOffline = true : metaData.isOffline = false;
            metaData.hasIsOffline = true;
        }
        else
        {
            // Failed to get file attributes.
            GUCEF_DEBUG_LOG( LOGLEVEL_NORMAL, "GetFileMetaData: GetFileAttributesExW failed with error code: " + ToString( (UInt32) ::GetLastError() ) );
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

    ::chmod( actualNewPath.C_String(), originalPermissions.st_mode );
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

    if ( !filename.IsNULLOrEmpty() )
    {
        #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

        std::wstring wFilename = ToWString( filename );

        WIN32_FIND_DATAW FileInfo;
        HANDLE hFind = GUCEF_NULL;
        hFind = FindFirstFileW( wFilename.c_str(), &FileInfo );
        if ( hFind != INVALID_HANDLE_VALUE )
        {
            FindClose( hFind );

            /* make sure we found a file not a directory */
            return !( FileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY );
        }
        return 0;

        #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

        struct stat buf;
        return stat( filename.C_String(), &buf ) == 0;

        #else

        FILE *fptr = fopen( filename.C_String(), "rb" );
        fclose( fptr );
        return fptr > 0;

        #endif
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
DirExists( const CString& path )
{GUCEF_TRACE;

    if ( GUCEF_NULL != path )
    {
        #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

        std::wstring wPath = ToWString( path );

        WIN32_FIND_DATAW FileInfo;
        HANDLE hFind = GUCEF_NULL;
        hFind = FindFirstFileW( wPath.c_str(), &FileInfo );
        if ( hFind != INVALID_HANDLE_VALUE )
        {
            FindClose( hFind );

            /* make sure we found a directory not a file */
            return ( FileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY );
        }
        return 0;

        #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

        struct stat buf;
        if ( stat( path.C_String(), &buf ) == 0 )
            if ( buf.st_mode & S_IFDIR != 0 )
                return 1;
        return 0;

        #else

        FILE *fptr = fopen( filename.C_String(), "rb" );
        fclose( fptr );
        return fptr > 0;

        #endif
    }
    return 0;
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

    if ( !filename.IsNULLOrEmpty() )
    {
        #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

        std::wstring wFilename = ToWString( filename );

        UInt64 lfilesize = 0;
        WIN32_FIND_DATAW FileInfo;
        HANDLE hFind;
        hFind = FindFirstFileW( wFilename.c_str(), &FileInfo );
        if ( hFind == INVALID_HANDLE_VALUE )
        {
            lfilesize = 0;
        }
        else
        {
            lfilesize = FileInfo.nFileSizeLow;
        }
        FindClose( hFind );
        return lfilesize;

        #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

        struct stat buf;
        int result;
        result = stat( filename.C_String(), &buf );
        if ( result == 0 )
            return buf.st_size;
        return 0;

        #else

        UInt64 filesize = 0;
        FILE *fptr = fopen( filename.C_String(), "rb" );
        fseek( fptr, 0, SEEK_END );
        filesize = ftell( fptr );
        fclose( fptr );
        return filesize;

        #endif
    }

    return 0;
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

    if ( 0 == result && 0 == listSizeInWChars )
        return false;

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

/*-------------------------------------------------------------------------*/

bool
GetAllFileSystemStorageVolumes( CString::StringSet& volumeIds )
{GUCEF_TRACE;

    volumeIds.clear();

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    WCHAR volNameBuffer[ MAX_PATH ];
    memset( volNameBuffer, 0, sizeof( volNameBuffer ) );

    HANDLE volumeFindHandle = ::FindFirstVolumeW( volNameBuffer, MAX_PATH );
    if ( INVALID_HANDLE_VALUE == volumeFindHandle )
    {
        DWORD errorCode = ::GetLastError();
        if ( ERROR_NO_MORE_FILES == errorCode )
            return true;

        GUCEF_DEBUG_LOG( LOGLEVEL_NORMAL, "GetAllFileSystemStorageVolumes: FindFirstVolumeW failed with error code: " + ToString( (UInt32) errorCode ) );
        return false;
    }

    bool findMoreVolumes = true;
    do
    {
        volumeIds.insert( ToString( volNameBuffer ) );

        if ( 0 == ::FindNextVolumeW( volumeFindHandle, volNameBuffer, MAX_PATH ) )
        {
            if ( ERROR_NO_MORE_FILES == ::GetLastError() )
                findMoreVolumes = false;
        }
    }
    while ( findMoreVolumes );
    ::FindVolumeClose( volumeFindHandle );

    return true;

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    return false;

    #else

    return false;

    #endif
}

/*-------------------------------------------------------------------------*/

bool
GetAllFileSystemPathNamesForVolume( const CString& volumeId       ,
                                    CString::StringSet& pathNames )
{GUCEF_TRACE;

    pathNames.clear();

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    std::wstring volumeIdWide = ToWString( volumeId );

    // first get the size of the buffer we need
    DWORD requiredBufferLength = 0;
    BOOL success = ::GetVolumePathNamesForVolumeNameW( volumeIdWide.c_str()  ,
                                                       NULL                  ,
                                                       0                     ,
                                                       &requiredBufferLength );
    if ( success == TRUE )
    {
        // there is nothing to fetch
        return true;
    }
    DWORD errorCode = ::GetLastError();
    if ( ERROR_MORE_DATA != errorCode )
    {
        GUCEF_DEBUG_LOG( LOGLEVEL_NORMAL, "GetAllFileSystemPathNamesForVolume: GetVolumePathNamesForVolumeNameW failed with error code: " + ToString( (UInt32) errorCode ) );
        return false;
    }

    UInt32 requiredBufferBytes = requiredBufferLength * sizeof( WCHAR );
    CORE::CDynamicBuffer volumePathsBuffer( requiredBufferBytes, true );
    volumePathsBuffer.SetBytes( 0 );
    volumePathsBuffer.SetDataSize( requiredBufferBytes );

    // Get the volume paths
    success = ::GetVolumePathNamesForVolumeNameW( volumeIdWide.c_str()                 ,
                                                  volumePathsBuffer.AsTypePtr<WCHAR>() ,
                                                  volumePathsBuffer.GetBufferSize()    ,
                                                  &requiredBufferLength                );
    if ( success == TRUE )
    {
        WCHAR nullTerm = 0;
        Int32 lastOffset = 0;
        Int32 offset = volumePathsBuffer.Find( &nullTerm, sizeof(nullTerm), 0 );
        while ( offset > 0 )
        {
            const wchar_t* pathNamePtr = volumePathsBuffer.AsConstTypePtr< wchar_t >( (UInt32) lastOffset );
            if ( GUCEF_NULL != pathNamePtr && *pathNamePtr != 0 )
                pathNames.insert( ToString( pathNamePtr ) );

            lastOffset = offset;
            offset = volumePathsBuffer.Find( &nullTerm, sizeof(nullTerm), (UInt32) lastOffset+1 );
        }

        return true;
    }
    else
    {
        DWORD errorCode = ::GetLastError();
        GUCEF_DEBUG_LOG( LOGLEVEL_NORMAL, "GetAllFileSystemPathNamesForVolume: GetVolumePathNamesForVolumeNameW failed with error code: " + ToString( (UInt32) errorCode ) );
        return false;
    }

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    return false;

    #else

    return false;

    #endif
}

/*-------------------------------------------------------------------------*/

bool
GetAllFileSystemMountPointsForVolume( const CString& volumeId         ,
                                      CString::StringSet& mountPoints )
{GUCEF_TRACE;

    mountPoints.clear();

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    std::wstring volumeIdWide = ToWString( volumeId );

    const DWORD MOUNT_POINT_BUFFER_SIZE = 1024;
    WCHAR volMountBuffer[ MOUNT_POINT_BUFFER_SIZE ];
    memset( volMountBuffer, 0, sizeof( volMountBuffer ) );

    // Get the Volume mount points as the windows equivelant of symlink based mounts
    // Volume mount points are alternative locations in the file system where volumes are mounted,
    // providing a way to access volumes indirectly or integrate them into the directory structure of another volume.

    HANDLE mountFindHandle = ::FindFirstVolumeMountPointW( volumeIdWide.c_str(), volMountBuffer, MOUNT_POINT_BUFFER_SIZE );
    if ( INVALID_HANDLE_VALUE == mountFindHandle )
    {
        DWORD errorCode = ::GetLastError();
        if ( ERROR_NO_MORE_FILES == errorCode )
            return true;

        GUCEF_DEBUG_LOG( LOGLEVEL_NORMAL, "GetAllFileSystemMountPointsForVolume: FindFirstVolumeMountPointW failed with error code: " + ToString( (UInt32) errorCode ) );
        return false;
    }

    bool findMoreMounts = true;
    do
    {
        mountPoints.insert( ToString( volMountBuffer ) );

        if ( 0 == ::FindNextVolumeMountPointW( mountFindHandle, volMountBuffer, MAX_PATH ) )
        {
            if ( ERROR_NO_MORE_FILES == ::GetLastError() )
                findMoreMounts = false;
        }
    }
    while ( findMoreMounts );
    ::FindVolumeMountPointClose( mountFindHandle );

    return true;

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    return false;

    #else

    return false;

    #endif
}

/*-------------------------------------------------------------------------*/

// Structure used to store dir iteration data which is O/S dependent
class CFileSystemIterator::CFileSystemIteratorOsData
{
 	public:

 	bool isActive;            // Flag indicating if the iterator is active

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

	intptr_t find_handle;        // Unique handle identifying the file or set of files that resulted from a findfirst with the filter provided
	struct _wfinddata64_t find;  // struct that stores entry data

	CFileSystemIteratorOsData( void )
        : isActive( false )  // we start with an inactive iterator. A call to FindFirst will activate it potentially
        , find_handle( 0 )
	{
        memset( &find, 0, sizeof find );
	}

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    DIR* dir;                 // Directory stream
    struct dirent* entry;     // Pointer needed for functions to iterating directory entries. Stores entry name which is used to get stat
    struct stat statinfo;     // Struct needed for determining info about an entry with stat().

	CFileSystemIteratorOsData( void )
        : isActive( false )
        , dir( GUCEF_NULL )
        , entry( GUCEF_NULL )
        , statinfo()
	{
        memset( &statinfo, 0, sizeof statinfo );
	}

    #else

    // -> empty struct because we don't support other OS's atm
    #error Unsupported OS

    #endif
};

/*-------------------------------------------------------------------------*/

CFileSystemIterator::CFileSystemIterator( void )
    : m_osData( GUCEF_NEW CFileSystemIteratorOsData() )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CFileSystemIterator::~CFileSystemIterator()
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_osData )
    {
        FindClose();
        GUCEF_DELETE m_osData;
        m_osData = GUCEF_NULL;
    }
}

/*-------------------------------------------------------------------------*/

bool
CFileSystemIterator::FindFirst( const CString& path )
{GUCEF_TRACE;

    if ( path.IsNULLOrEmpty() )
        return false;
    if ( GUCEF_NULL == m_osData || m_osData->isActive )
        return false;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    /*
     *	In Win32 we use _findfirst ect. because even though the posix
     *	functions are supported on windows NT they are not supported on
     *	Win98 and WinME and these functions are. No support for Win95 or
     *	older though.
     */

    if ( -1 == path.HasChar( '*', false ) )
    {
       CString filterPath = CombinePath( path, "*.*" );
       std::wstring wFilterPath = ToWString( filterPath );
       m_osData->find_handle = _wfindfirst64( wFilterPath.c_str(), &m_osData->find );
    }
    else
    {
        std::wstring wPath = ToWString( path );
        m_osData->find_handle = _wfindfirst64( wPath.c_str(), &m_osData->find );
    }

    // Check if findfirst was successful
    if ( m_osData->find_handle == -1 )
    {
        // There was an error
        _findclose( m_osData->find_handle );
        m_osData->find_handle = 0;

        return false;
    }

    /*
     *	Successfully obtained first entry
     */
    m_osData->isActive = true;
    return true;

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    /*
     *	In Linux we use POSIX functions because these are independant of
     *	the Linux distribution. It may also provide use with support for
     *	other Unix based systems.
     */

    /*
     *	Attempt to open the directory
     */
    m_osData->dir = opendir( path.C_String() );
    if ( GUCEF_NULL == m_osData->dir )
    {
        // Could not open directory
	    return false;
    }

    /*
     *	change working dir to be able to read file information
     */
    chdir( path.C_String() );

    /*
     *	Read first entry
     */
    m_osData->entry = readdir( m_osData->dir );
    while( m_osData->entry )
    {
        /*
         *	Get info on the entry.
         *	We only want regular files and directory entry's. We ignore the
         *	rest.
         */
        stat( m_osData->entry->d_name, &m_osData->statinfo );
        if ( S_ISREG( m_osData->statinfo.st_mode ) || S_ISDIR( m_osData->statinfo.st_mode ) )
        {
        	/*
             *	We found either a regular file or a directory
             *	entry which is now our current entry.
             */
            return m_osData;
        }

        /*
         *	This entry is not what we want,.. skip to the next entry
         */
        m_osData->entry = readdir( m_osData->dir );
    }

    /*
     *	there was an error reading the entry data or no entry was found
     *	on the path specified that was a regular file or directory.
     */
    if ( NULL != m_osData->dir )
    {
        closedir( m_osData->dir );
    }
    free( m_osData );
    return NULL;

    #else

    /*
     *	Unsupported O/S build
     */

    return false;

    #endif

}

/*-------------------------------------------------------------------------*/

bool
CFileSystemIterator::FindNext( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL == m_osData || !m_osData->isActive )
        return false;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    return !_wfindnext64( m_osData->find_handle, &m_osData->find );

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    /*
     *	Read next entry
     */
    m_osData->entry = readdir( m_osData->dir );
    while ( GUCEF_NULL != m_osData->entry )
    {
        /*
         *	Get info on the entry.
         *	We only want regular files and directory entry's. We ignore the
         *	rest.
         */
        stat( m_osData->entry->d_name, &m_osData->statinfo );
        if ( S_ISREG( m_osData->statinfo.st_mode ) || S_ISDIR( m_osData->statinfo.st_mode ) )
        {
        	/*
             *	We found either a regular file or a directory
             *	entry which is now our current entry.
             */
            return true;
        }

        /*
         *	This entry is not what we want,.. skip to the next entry
         */
        m_osData->entry = readdir( m_osData->dir );
    }

    /*
     *	Could not find any other entry's that where either a regular file
     *	or a directory.
     */

    #else

        /*
         *	Unsupported O/S build
         */
    #endif

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CFileSystemIterator::FindClose( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL == m_osData || !m_osData->isActive )
        return false;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    ::_findclose( m_osData->find_handle );
    m_osData->find_handle = GUCEF_NULL;

    #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

    if ( GUCEF_NULL != m_osData->dir )
    {
        ::closedir( m_osData->dir );
        m_osData->dir = GUCEF_NULL;
    }

    #else

    /*
     *	Unsupported O/S build
     */

    #endif

    m_osData->isActive = false;
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CFileSystemIterator::IsADirectory( void ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_osData && m_osData->isActive )
    {
        #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

        return ( m_osData->find.attrib & _A_SUBDIR );

        #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

        return S_ISDIR( m_osData->statinfo.st_mode ) > 0;

        #else

        /*
         *	Unsupported O/S build
         */

        #endif
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CFileSystemIterator::IsAFile( void ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_osData && m_osData->isActive )
    {
        #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

        return !( m_osData->find.attrib & _A_SUBDIR );

        #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

        return S_ISREG( m_osData->statinfo.st_mode ) > 0;

        #else

        /*
         *	Unsupported O/S build
         */

        #endif
    }

    return false;
}

/*-------------------------------------------------------------------------*/

CString
CFileSystemIterator::GetResourceName( void ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_osData && m_osData->isActive )
    {
        #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

        return m_osData->find.name;

        #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

        return m_osData->entry->d_name;

        #else

        /*
         *	Unsupported O/S build
         */

        #endif
    }
    return CString::Empty;
}

/*-------------------------------------------------------------------------*/

bool
CFileSystemIterator::TryReadMetaData( CResourceMetaData& metaData )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_osData && m_osData->isActive )
    {
        #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

        metaData.resourceSizeInBytes = m_osData->find.size;
        metaData.hasResourceSizeInBytes = true;
        metaData.creationDateTime = CDateTime( m_osData->find.time_create, true );
        metaData.hasCreationDateTime = true;
        metaData.modifiedDateTime = CDateTime( m_osData->find.time_access, true );
        metaData.hasModifiedDateTime = true;
        metaData.lastAccessedDateTime = CDateTime( m_osData->find.time_write, true );
        metaData.hasLastAccessedDateTime = true;

        m_osData->find.attrib & FILE_ATTRIBUTE_READONLY ? metaData.isReadOnly = true : metaData.isReadOnly = false;
        metaData.hasIsReadOnly = true;
        m_osData->find.attrib & FILE_ATTRIBUTE_HIDDEN ? metaData.isHidden = true : metaData.isHidden = false;
        metaData.hasIsHidden = true;
        m_osData->find.attrib & FILE_ATTRIBUTE_SYSTEM || m_osData->find.attrib & FILE_ATTRIBUTE_DEVICE ? metaData.isSystemResource = true : metaData.isSystemResource = false;
        metaData.hasIsSystemResource = true;
        m_osData->find.attrib & FILE_ATTRIBUTE_ARCHIVE ? metaData.isArchive = true : metaData.isArchive = false;
        metaData.hasIsArchive = true;
        m_osData->find.attrib & FILE_ATTRIBUTE_COMPRESSED ? metaData.isCompressed = true : metaData.isCompressed = false;
        metaData.hasIsCompressed = true;
        m_osData->find.attrib & FILE_ATTRIBUTE_ENCRYPTED ? metaData.isEncrypted = true : metaData.isEncrypted = false;
        metaData.hasIsEncrypted = true;
        m_osData->find.attrib & FILE_ATTRIBUTE_TEMPORARY ? metaData.isTemporary = true : metaData.isTemporary = false;
        metaData.hasIsTemporary = true;
        m_osData->find.attrib & FILE_ATTRIBUTE_OFFLINE ? metaData.isOffline = true : metaData.isOffline = false;
        metaData.hasIsOffline = true;

        return true;

        #elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )

        metaData.creationDateTime = CDateTime( m_osData->statinfo.st_ctime, true );
        metaData.hasCreationDateTime = true;
        metaData.modifiedDateTime = CDateTime( m_osData->statinfo.st_mtim, true );
        metaData.hasModifiedDateTime = true;
        metaData.lastAccessedDateTime = CDateTime( m_osData->statinfo.st_atim, true );
        metaData.hasLastAccessedDateTime = true;
        metaData.resourceSizeInBytes = (UInt64) m_osData->statinfo.st_size;
        metaData.hasResourceSizeInBytes = true;

        return true;

        #else

        /*
         *	Unsupported O/S build
         */

        #endif
    }

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
