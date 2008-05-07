/*
 *  gucefVFS: GUCEF module implementing a Virtual File System
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
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_DVFILEUTILS_H
#include "dvfileutils.h"
#define GUCEF_CORE_DVFILEUTILS_H
#endif /* GUCEF_CORE_DVFILEUTILS_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H 
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#include "CDynamicBufferAccess.h"
#define GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#endif /* GUCEF_CORE_CDYNAMICBUFFERACCESS_H ? */

#ifndef GUCEF_CORE_CFILEACCESS_H
#include "CFileAccess.h"
#define GUCEF_CORE_CFILEACCESS_H
#endif /* GUCEF_CORE_CFILEACCESS_H ? */

#ifndef GUCEF_CORE_DVMD5UTILS_H
#include "dvmd5utils.h"	     
#define GUCEF_CORE_DVMD5UTILS_H
#endif /* GUCEF_CORE_DVMD5UTILS_H ? */

#ifndef GUCEF_VFS_CVFS_H
#include "CVFS.h"
#define GUCEF_VFS_CVFS_H
#endif /* GUCEF_VFS_CVFS_H ? */

#include "gucefVFS_CFileSystemArchive.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace VFS {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CFileSystemArchive::CFileSystemArchive( const CString& archiveName ,
                                        const CString& rootDir     ,
                                        bool writeable             )
    : m_diskCacheList()            ,
      m_rootDir( rootDir )         ,
      m_archiveName( archiveName ) ,
      m_writable( writeable )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CFileSystemArchive::~CFileSystemArchive()
{GUCEF_TRACE;

    m_diskCacheList.clear();
}

/*-------------------------------------------------------------------------*/

bool
CFileSystemArchive::LoadArchive( const CString& archiveName ,
                                 const CString& archivePath ,
                                 const bool writableRequest )
{GUCEF_TRACE;

    m_archiveName = archiveName;
    m_rootDir = archivePath;
    m_writable = writableRequest;
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CFileSystemArchive::UnloadArchive( void )
{GUCEF_TRACE;

    return true;
}

/*-------------------------------------------------------------------------*/

CIArchive::CVFSHandlePtr
CFileSystemArchive::GetFile( const CString& file      ,
                             const char* mode         ,
                             const UInt32 memLoadSize ,
                             const bool overwrite     )

{GUCEF_TRACE;

    CString filepath( file.ReplaceChar( DIRSEPCHAROPPOSITE, DIRSEPCHAR ) );
        
    /* 
     *      Try to locate the file using the rootdirs as 
     *      a prefix
     */
    CVFSHandle* fh = LoadFromDisk( filepath    ,
                                   mode        ,
                                   memLoadSize ,
                                   overwrite   );
    
    if ( fh != NULL )
    {
        return CVFSHandlePtr( fh, this );
    }

    /*
     *      If we get here then the file was not found in one 
     *      of the rootdirs. or in any of the packfiles
     */
    return CVFSHandlePtr();                             
}

/*-------------------------------------------------------------------------*/

void 
CFileSystemArchive::GetListFromRoot( const CORE::CString& root  ,
                                     bool recursive             ,
                                     bool includePathInFilename ,
                                     const CString& filter      ,
                                     TStringSet& outputList     ,
                                     bool addFiles              ,
                                     bool addDirs               ) const
{GUCEF_TRACE;

    CORE::CString filename;
    struct CORE::SDI_Data* did = CORE::DI_First_Dir_Entry( root.C_String() );
    if ( did != NULL )
    {                
        /*
         *      Iterate the dir content adding the file entries from
         *      the dir to the list
         */
        do
        {
            if ( CORE::DI_Is_It_A_File( did ) != 0 )
            {                           
                if ( addFiles )
                {
                    filename = CORE::DI_Name( did );     
                    if ( filename != '.' && filename != ".." )
                    {
                        if ( CVFS::FilterValidation( filename ,
                                                     filter   ) )
                        {
                            if ( !includePathInFilename )
                            {
                                outputList.insert( filename );
                            }
                            else
                            {
                                CORE::CString filePath( root );
                                CORE::AppendToPath( filePath, filename );
                                outputList.insert( filePath );                        
                            }
                        }
                    }
                }                                        
            }
            else
            {
                if ( addDirs )
                {
                    CORE::CString dirName = CORE::DI_Name( did );
                    if ( dirName != '.' && dirName != ".." )
                    {
                        outputList.insert( dirName );
                    }
                }
                
                if ( recursive )
                {
                    /*
                     *      Build the path including the sub-dir
                     */
                    CORE::CString subdir( root );
                    CORE::AppendToPath( subdir, CORE::DI_Name( did ) );
                    
                    /*
                     *      Recursively process the sub-dir
                     */
                    GetListFromRoot( subdir                ,
                                     recursive             ,
                                     includePathInFilename ,
                                     filter                ,
                                     outputList            ,
                                     true                  ,
                                     false                 ); 
                }
            }        
        }
        while ( CORE::DI_Next_Dir_Entry( did ) );
        
        CORE::DI_Cleanup( did );
    }                
}

/*-------------------------------------------------------------------------*/
                                  
void
CFileSystemArchive::GetList( TStringSet& outputList     ,
                             const CString& location    , 
                             bool recursive             ,
                             bool includePathInFilename ,
                             const CString& filter      ,
                             bool addFiles              ,
                             bool addDirs               ) const
{GUCEF_TRACE;
    
    /*
     *      Switch dir separator chars if needed
     */
    CString loc( location.ReplaceChar( DIRSEPCHAROPPOSITE, DIRSEPCHAR ) );              
    
    /*
     *      Make the combo path string
     */
    CString rootdir = m_rootDir;
    AppendToPath( rootdir, location );                 

    /*
     *      Process the root
     */
    GetListFromRoot( rootdir               ,
                     recursive             ,
                     includePathInFilename ,
                     filter                ,
                     outputList            ,
                     addFiles              ,
                     addDirs               );
}
 
/*-------------------------------------------------------------------------*/
 
bool
CFileSystemArchive::FileExists( const CString& filePath ) const
{GUCEF_TRACE;

    CString path = m_rootDir;
    CORE::AppendToPath( path, filePath );
    return CORE::File_Exists( path.C_String() ) != 0;
}

/*-------------------------------------------------------------------------*/
    
UInt32
CFileSystemArchive::GetFileSize( const CString& filePath ) const
{GUCEF_TRACE;

    CString path = m_rootDir;
    CORE::AppendToPath( path, filePath );
    if ( CORE::File_Exists( path.C_String() ) != 0 )
    {
        return CORE::Filesize( path.C_String() );
    }
    return 0;
}

/*-------------------------------------------------------------------------*/
    
CString
CFileSystemArchive::GetFileHash( const CString& file ) const
{GUCEF_TRACE;
   
    CString path = m_rootDir;
    CORE::AppendToPath( path, file );
    if ( CORE::File_Exists( path.C_String() ) != 0 )
    {
        CORE::CFileAccess fileAccess( path );
        
        UInt8 md5Hash[ 16 ];
        if ( CORE::md5frommfile( fileAccess.CStyleAccess() , 
                                 md5Hash                   ) == 1 )
        {
            return CORE::MD5ToString( md5Hash );
        }
    }
    
    return CString();
}

/*-------------------------------------------------------------------------*/

const CString&
CFileSystemArchive::GetArchiveName( void ) const
{GUCEF_TRACE;
    
    return m_archiveName;
}

/*-------------------------------------------------------------------------*/

bool
CFileSystemArchive::IsWriteable( void ) const
{GUCEF_TRACE;

    return m_writable;
}

/*-------------------------------------------------------------------------*/

CVFSHandle*
CFileSystemArchive::LoadFromDisk( const CString& file      ,
                                  const char* mode         ,
                                  const UInt32 memLoadSize ,
                                  const bool overwrite     )
{GUCEF_TRACE;

    // Create a file path for this root
    CString filepath = m_rootDir;
    CORE::AppendToPath( filepath, file );
    
    bool needwriteable( ( strchr( mode, 'a' ) != NULL ) || ( strchr( mode, 'w' ) != NULL ) || ( strchr( mode, '+' ) != NULL )); 

    // Check if we can perform read-only access which allows us 
    // to share the resource
    if ( ( strcmp( mode, "rb" ) == 0 ) ||
         ( strcmp( mode, "r" ) == 0 )   )
    {
        // Check our cache for this file
        TFileMemCache::iterator n = m_diskCacheList.find( filepath );
        if ( n != m_diskCacheList.end() )
        {
            // We found the file in our cache, we will link to the existing buffer.
            TDynamicBufferPtr bufferPtr = (*n).second;
            return new CVFSHandle( new CORE::CDynamicBufferAccess( bufferPtr.GetPointer() , 
                                                                   false                  ) ,
                                   file                                                     ,
                                   filepath                                                 ,
                                   bufferPtr                                                );
        }
    }
    
    // Check if the file even exists on disk
    bool exists( CORE::File_Exists( filepath.C_String() ) > 0 );
    if ( exists )
    {                        
        // Attempt to get access to the file
        CORE::CIOAccess* fa = new CORE::CFileAccess( filepath, mode );
        if ( !fa->IsValid() )
        {
            // try a different root
            delete fa;
            return NULL;
        }
        
        if ( ( strcmp( mode, "rb" ) == 0 ) ||
             ( strcmp( mode, "r" ) == 0 )   )
        {
            // check if we can load the file into memory
            UInt32 fsize = CORE::Filesize( filepath.C_String() );
            if ( fsize <= memLoadSize )
            {
                // Create the memory buffer
                TDynamicBufferPtr bufferPtr( new CORE::CDynamicBuffer() );
                CORE::CIOAccess* bufferAccess = new CORE::CDynamicBufferAccess( bufferPtr.GetPointer(), false );
                
                // Copy the file into the buffer
                if ( fsize == bufferAccess->Write( *fa ) )
                {
                    delete fa;
                    fa = NULL;
                    
                    // reset the carat so the user can access the file from the beginning
                    bufferAccess->Setpos( 0 );
                    
                    // Add the file to our buffered files list
                    m_diskCacheList.insert( std::pair< CORE::CString, TDynamicBufferPtr >( filepath, bufferPtr ) );
                    
                    // return the file handle
                    return new CVFSHandle( bufferAccess ,
                                           file         ,
                                           filepath     ,
                                           bufferPtr    );                  
                }
                else
                {
                    // Something went wrong while trying to load the file into the buffer
                    // try a different root
                    fa->Setpos( 0 );
                    delete bufferAccess;
                }
            }
            
            // return the file handle
            return new CVFSHandle( fa                  ,
                                   file                ,
                                   filepath            ,
                                   TDynamicBufferPtr() );                
                              
        }
    }
    
    return NULL;
}

/*-------------------------------------------------------------------------*/

void
CFileSystemArchive::DestroyObject( CVFSHandle* vfshandle )
{GUCEF_TRACE;

    if ( vfshandle != NULL )
    {
        delete vfshandle->GetAccess();
        
        if ( vfshandle->IsLoadedInMemory() )
        {
            TFileMemCache::iterator n = m_diskCacheList.find( vfshandle->GetFilePath() );
            if ( n != m_diskCacheList.end() )
            {
                // We found the file in our cache, we will link to the existing buffer.
                if ( (*n).second.GetReferenceCount() == 1 )
                {
                    // nobody else is using the memory buffer anymore
                    m_diskCacheList.erase( n );
                }
            }            
        }
        
        delete vfshandle;
    }
}

/*-------------------------------------------------------------------------*/

const CString&
CFileSystemArchive::GetRootDir( void ) const
{GUCEF_TRACE;

    return m_rootDir;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace VFS */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
