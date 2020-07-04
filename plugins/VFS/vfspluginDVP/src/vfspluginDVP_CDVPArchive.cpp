/*
 *  vfspluginDVP: Generic GUCEF VFS plugin for "Dinand Vanvelzen Pack" archives
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

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#include "vfspluginDVP_CDVPArchive.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace VFSPLUGIN {
namespace DVP {

using namespace DVPACKSYS;

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

typedef std::vector< VFS::CString > TStringVector;

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const VFS::CString CDVPArchive::DVPArchiveTypeName = "dvp";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CDVPArchive::CDVPArchive( void )
    : CIArchive()       ,
      m_header()        ,
      m_entries( NULL ) ,
      m_archiveName()   ,
      m_archivePath()
{GUCEF_TRACE;

    memset( &m_header, 0, sizeof(DVPACKSYS::TDVP_File_Header) );
}

/*-------------------------------------------------------------------------*/
    
CDVPArchive::~CDVPArchive()
{GUCEF_TRACE;

    UnloadArchive();
}

/*-------------------------------------------------------------------------*/
    
VFS::CIArchive::CVFSHandlePtr
CDVPArchive::GetFile( const VFS::CString& file      ,
                      const char* mode              ,
                      const VFS::UInt32 memLoadSize ,
                      const bool overwrite          )
{GUCEF_TRACE;

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CDVPArchive: request to get file: " +  file );
    
    // We only support read only modes
    if ( *mode != 'r' )
    {
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CDVPArchive: Unable to support requested file access mode for file: " + file );
        return CVFSHandlePtr();
    }
    
    // load the file
    CORE::CIOAccess* fileAccess = LoadFile( file, memLoadSize );
    if ( NULL != fileAccess )
    {
        // create a handle for the file
        VFS::CString filePath = m_archivePath;
        CORE::AppendToPath( filePath, file );

        VFS::CVFSHandle* fileHandle = new VFS::CVFSHandle( fileAccess , 
                                                           file       ,
                                                           filePath   );
                                                           
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CVPArchive: providing access to file: " + file );        
        return CVFSHandlePtr( fileHandle, this );
    }
    
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CVPArchive: Unable to provide access to file: " + file );
    return CVFSHandlePtr();
}


/*-------------------------------------------------------------------------*/

bool 
CDVPArchive::StoreAsFile( const CORE::CString& filepath    ,
                          const CORE::CDynamicBuffer& data ,
                          const CORE::UInt64 offset        ,
                          const bool overwrite             )
{GUCEF_TRACE;

    // Not implemented / supported at this time
    return false;
}

/*-------------------------------------------------------------------------*/

void
CDVPArchive::GetList( TStringSet& outputList       ,
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
CDVPArchive::FileExists( const VFS::CString& filePath ) const
{GUCEF_TRACE;

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CDVPArchive: request to check if file exists: " +  filePath );
    
    return -1 < FindFileEntry( filePath );
}

/*-------------------------------------------------------------------------*/

VFS::UInt32
CDVPArchive::GetFileSize( const VFS::CString& filePath ) const
{GUCEF_TRACE;

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CDVPArchive: request for file size for file: " +  filePath );
    
    VFS::Int32 fileEntryIndex = FindFileEntry( filePath );
    if ( -1 < fileEntryIndex )
    {
        return m_entries[ fileEntryIndex ].size;
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

CORE::CIOAccess*
CDVPArchive::LoadFile( const VFS::CString& filePath  ,
                       const VFS::UInt32 memLoadSize ) const
{GUCEF_TRACE;
    
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CDVPArchive: Attempting to load file: " +  filePath );
    
    VFS::Int32 fileEntryIndex = FindFileEntry( filePath );
    if ( -1 < fileEntryIndex )
    {
        if ( memLoadSize < m_entries[ fileEntryIndex ].size )
        {
            CORE::CSubFileAccess* subFileAccess = new CORE::CSubFileAccess();
            if ( !subFileAccess->Load( filePath                           , 
                                       m_entries[ fileEntryIndex ].offset , 
                                       m_entries[ fileEntryIndex ].size   ) )
            {
                delete subFileAccess;
                
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CDVPArchive: Failed to link file subsegment as a subfile for archive file: " +  m_archivePath );
                return NULL;
            }
            return subFileAccess;
        }
        else
        {
            CORE::CDynamicBuffer* buffer = new CORE::CDynamicBuffer();
            if ( !buffer->LoadContentFromFile( m_archivePath                      , 
                                               m_entries[ fileEntryIndex ].offset , 
                                               m_entries[ fileEntryIndex ].size   ) )
            {
                delete buffer;
                
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CDVPArchive: Failed to load file into memory from archive file: " +  m_archivePath );
                return NULL;
            }
            
            return new CORE::CDynamicBufferAccess( buffer, true );
        }

    }
    return NULL;    
}

/*-------------------------------------------------------------------------*/
    
VFS::CString
CDVPArchive::GetFileHash( const VFS::CString& file ) const
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

VFS::Int32
CDVPArchive::FindDirEntry( const VFS::CString& path ) const
{GUCEF_TRACE;

    if ( NULL != m_entries || path.IsNULLOrEmpty() )
    {
        TStringVector pathElements = path.ParseElements( '/', false );
        VFS::UInt32 entryCount = m_header.num_dirs + m_header.num_files;
        
        VFS::CString currentPath;
        for ( VFS::UInt32 n=0; n<entryCount; ++n )
        {
            // Check if the current entry is a dir
            if ( 0 == m_entries[ n ].size )
            {
                // Check if this is an up-dir element                
                if ( 0 == strcmp( m_entries[ n ].name, ".." ) )
                {
                    currentPath = CORE::StripLastSubDir( currentPath );
                }
                else
                {
                    CORE::AppendToPath( currentPath, m_entries[ n ].name );
                    currentPath = currentPath.ReplaceChar( '\\', '/' );
                }
                
                if ( path == currentPath )
                {
                    return (VFS::Int32) n;
                }
            }
        }
    }
    return -1;
}

/*-------------------------------------------------------------------------*/

VFS::Int32
CDVPArchive::FindFileEntry( const VFS::CString& filePath ) const
{GUCEF_TRACE;

    VFS::CString filename = CORE::ExtractFilename( filePath );
    VFS::CString path = CORE::StripFilename( filePath );
    
    VFS::Int32 dirEntryIndex = -2;
    if ( path.IsNULLOrEmpty() )
    {
        // If we have no path start looking at the first element
        dirEntryIndex = -1;
    }
    else
    {
        dirEntryIndex = FindDirEntry( path ) - 1;
    }

    if ( -2 < dirEntryIndex )
    {
        // Look in this directory for the given file
        VFS::Int32 entryCount = (VFS::Int32) m_header.num_dirs + m_header.num_files;
        for ( VFS::Int32 i=dirEntryIndex+1; i<entryCount; ++i )
        {
            // Check if the current entry is a dir
            if ( 0 != m_entries[ i ].size )
            {
                // We encountered the next dir element, thus the file is not in this path
                break;
            }
            
            // Check if the name matches what we are looking for
            if ( filename == m_entries[ i ].name )
            {
                return i;
            }
        }
    }
    return -1;
}

/*-------------------------------------------------------------------------*/

time_t
CDVPArchive::GetFileModificationTime( const VFS::CString& filePath ) const
{GUCEF_TRACE;

    VFS::Int32 fileEntryIndex = FindFileEntry( filePath );
    if ( -1 < fileEntryIndex )
    {
        return (time_t) m_entries[ fileEntryIndex ].timestamp;
    }
    return 0;
}

/*-------------------------------------------------------------------------*/
    
const VFS::CString&
CDVPArchive::GetArchiveName( void ) const
{GUCEF_TRACE;

    return m_archiveName;
}

/*-------------------------------------------------------------------------*/

bool
CDVPArchive::IsWriteable( void ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/
    
bool
CDVPArchive::LoadArchive( const VFS::CArchiveSettings& settings )
{GUCEF_TRACE;
    
    // We do not support writable DVP archives
    if ( settings.GetWriteableRequested() ) 
        return false;
    
    UnloadArchive();

    FILE* fptr = DVP_Open_File( settings.GetActualArchivePath().C_String(), "rb" );
    if ( NULL == fptr ) return false;

    _DVP_Read_Header( fptr, &m_header );

    /* 
     *      Read all index entries
     */       
    VFS::UInt32 offset = m_header.index_offset;
    VFS::UInt32 count = m_header.num_dirs + m_header.num_files;
    m_entries = new TDVP_Index_Entry[ count ]; 
    for ( UInt32 i=0; i<count; ++i )
    {
        _DVP_Read_Index_Entry( offset          ,
                               &m_entries[ i ] ,
                               fptr            );       
    }
    DVP_Close_File( fptr );
        
    m_archiveName = settings.GetArchiveName();
    m_archivePath = settings.GetActualArchivePath();
    
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CDVPArchive::LoadArchive( const VFS::CString& archiveName ,
                          CVFSHandlePtr vfsResource       ,
                          const bool writeableRequest     )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CDVPArchive::UnloadArchive( void )
{GUCEF_TRACE;

    memset( &m_header, 0, sizeof( TDVP_File_Header ) );
    
    delete m_entries;
    m_entries = NULL;
    
    m_archivePath = NULL;
    m_archiveName = NULL;
    
    return true;
}

/*-------------------------------------------------------------------------*/

const VFS::CString& 
CDVPArchive::GetType( void ) const
{GUCEF_TRACE;

    return DVPArchiveTypeName;
}

/*-------------------------------------------------------------------------*/

void
CDVPArchive::DestroyObject( VFS::CVFSHandle* objectToBeDestroyed )
{GUCEF_TRACE;

    delete objectToBeDestroyed;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace DVP */
}; /* namespace VFSPLUGIN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
