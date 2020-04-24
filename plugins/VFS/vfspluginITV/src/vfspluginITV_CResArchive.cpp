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

#ifndef GUCEF_CORE_CFILEACCESS_H
#include "CFileAccess.h"
#define GUCEF_CORE_CFILEACCESS_H
#endif /* GUCEF_CORE_CFILEACCESS_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#include "vfspluginITV_CResArchive.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace VFSPLUGIN {
namespace ITV {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CResArchive::CResArchive( void )
    : CIArchive()       ,
      m_archiveName()   ,
      m_archivePath()   ,
      m_resPath()       ,
      m_idxPath()       ,
      m_index()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CResArchive::~CResArchive()
{GUCEF_TRACE;

    UnloadArchive();
}

/*-------------------------------------------------------------------------*/

VFS::CIArchive::CVFSHandlePtr
CResArchive::GetFile( const VFS::CString& file      ,
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
CResArchive::GetList( TStringSet& outputList       ,
                      const VFS::CString& location ,
                      bool recursive               ,
                      bool includePathInFilename   ,
                      const VFS::CString& filter   ,
                      bool addFiles                ,
                      bool addDirs                 ) const
{GUCEF_TRACE;

    // This format only supports files
    if ( !addFiles ) return;

    // Generate list of all resource Id's
    CORE::CString resourceType = CORE::Int16ToString( m_index.recordType );
    IndexVector::const_iterator i = m_index.index.begin();
    while ( i != m_index.index.end() )
    {
        CORE::CString resourceId = CORE::Int32ToString( (*i).resourceNr ) + ".itv" + resourceType;
        if ( includePathInFilename )
        {
            resourceId = CORE::CombinePath( m_resPath, resourceId );
        }
        outputList.insert( resourceId );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

bool
CResArchive::FileExists( const VFS::CString& filePath ) const
{GUCEF_TRACE;

    // First perform a sanity check on the resource type
    VFS::CString typeIdStr = filePath.SubstrToChar( '.', false );
    typeIdStr = typeIdStr.CutChars( 3, true, 0 );
    CORE::UInt32 typeId = CORE::StringToUInt32( typeIdStr );
    if ( typeId != m_index.recordType )
        return false;

    VFS::CString resourceIdStr = filePath.SubstrToChar( '.', true );
    CORE::UInt32 resourceId = CORE::StringToUInt32( resourceIdStr );
    
    IndexVector::const_iterator i = m_index.index.begin();
    while ( i != m_index.index.end() )
    {
        if ( (*i).resourceNr == resourceId )
        {
            return true;
        }
        ++i;
    }

    return false;
}

/*-------------------------------------------------------------------------*/

VFS::UInt32
CResArchive::GetFileSize( const VFS::CString& filePath ) const
{GUCEF_TRACE;

    // First perform a sanity check on the resource type
    VFS::CString typeIdStr = filePath.SubstrToChar( '.', false );
    typeIdStr = typeIdStr.CutChars( 3, true, 0 );
    CORE::UInt32 typeId = CORE::StringToUInt32( typeIdStr );
    if ( typeId != m_index.recordType )
        return false;

    CORE::Int32 offset = 0;
    CORE::Int32 size = 0;
    CORE::Int32 resourceId = CORE::StringToInt32( filePath.SubstrToChar( '.', true ) );
    if ( GetResourceInfo( resourceId ,
                          offset     ,
                          size       ) )
    {
        return size;
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

time_t
CResArchive::GetFileModificationTime( const VFS::CString& filePath ) const
{
    // not supported by this format, maybe get archive mod time?
    return 0;
}

/*-------------------------------------------------------------------------*/

bool
CResArchive::GetResourceInfo( CORE::Int32 resourceId ,
                              CORE::Int32& offset    ,
                              CORE::Int32& size      ) const
{
    IndexVector::const_iterator i = m_index.index.begin();
    while ( i != m_index.index.end() )
    {
        if ( (*i).resourceNr == resourceId )
        {
            offset = (*i).offset;
            ++i;
            if ( i != m_index.index.end() )
            {
                size = (*i).offset - offset;
            }
            else
            {
                size = CORE::FileSize( m_resPath ) - 2 - offset;
            }
            return true;
        }
        ++i;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

CORE::CIOAccess*
CResArchive::LoadFile( const VFS::CString& file      ,
                       const VFS::UInt32 memLoadSize ) const
{GUCEF_TRACE;

    // First perform a sanity check on the resource type
    VFS::CString typeIdStr = file.SubstrToChar( '.', false );
    typeIdStr = typeIdStr.CutChars( 3, true, 0 );
    CORE::UInt32 typeId = CORE::StringToUInt32( typeIdStr );
    if ( typeId != m_index.recordType )
        return false;
    
    CORE::Int32 offset = 0;
    CORE::Int32 size = 0;
    CORE::Int32 resourceId = CORE::StringToInt32( file.SubstrToChar( '.', true ) );
    if ( !GetResourceInfo( resourceId ,
                           offset     ,
                           size       ) )
    {
        return NULL;
    }
        
    if ( (CORE::UInt32) size <= memLoadSize )
    {
        CORE::CDynamicBufferAccess* memBuffer = new CORE::CDynamicBufferAccess();
        if ( memBuffer->LoadContentFromFile( m_resPath, offset, size ) )
        {
            return memBuffer;
        } 
    }
    else
    {
        CORE::CSubFileAccess* subFile = new CORE::CSubFileAccess();
        if ( subFile->Load( m_resPath, offset, size ) )
        {
            return subFile;
        }
    }

    return NULL;
}

/*-------------------------------------------------------------------------*/

VFS::CString
CResArchive::GetFileHash( const VFS::CString& file ) const
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
CResArchive::GetArchiveName( void ) const
{GUCEF_TRACE;

    return m_archiveName;
}

/*-------------------------------------------------------------------------*/

bool
CResArchive::IsWriteable( void ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CResArchive::LoadIndex( void )
{GUCEF_TRACE;

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "CResArchive:LoadIndex: Starting index load" );

    CORE::CFileAccess file;
    if ( file.Open( m_idxPath, "rb" ) )
    {
        CORE::UInt32 fileSize = file.GetSize();        
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "CResArchive:LoadIndex: Successfully opened index file of " + CORE::UInt32ToString( fileSize ) + " bytes"  );
        
        if ( 1 != file.Read( &m_index.recordType, 2, 1 ) ) 
        { 
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Failed to read index header at position " + CORE::UInt32ToString( file.Tell() ) );
            file.Close(); 
            return false; 
        }

        // Get the nr of index entries plus do a sanity check
        CORE::UInt32 nrOfEntries = fileSize - 2;
        if ( 0 != nrOfEntries % 8 )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "CResArchive:LoadIndex: The number of bytes in the index is not what we expected, the data will probably be corrupt" );
        }
        nrOfEntries /= 8;

        // read all the index entries
        for ( CORE::UInt32 i=0; i<nrOfEntries; ++i )
        {
            IdxRecord idxRecord;
            if ( 1 != file.Read( &idxRecord.resourceNr, 4, 1 ) ) 
            { 
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Failed to read index entry at pos in index " + CORE::UInt32ToString( file.Tell() ) + 
                    " Entry " + CORE::UInt32ToString( i+1 ) + "/" + CORE::UInt32ToString( nrOfEntries ) );

                file.Close(); 
                return false; 
            }
            if ( 1 != file.Read( &idxRecord.offset, 4, 1 ) ) 
            { 
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Failed to read index entry at pos in index " + CORE::UInt32ToString( file.Tell() ) + 
                    " Entry " + CORE::UInt32ToString( i+1 ) + "/" + CORE::UInt32ToString( nrOfEntries ) );

                file.Close(); 
                return false; 
            }

            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Read index entry: id " + CORE::Int32ToString( idxRecord.resourceNr ) + 
                ", offset " + CORE::Int32ToString( idxRecord.offset ) + ". Pos in index " + CORE::UInt32ToString( file.Tell() ) + 
                " Entry " + CORE::UInt32ToString( i+1 ) + "/" + CORE::UInt32ToString( nrOfEntries ) );
            
            m_index.index.push_back( idxRecord );
        }

        file.Close();
    }

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "CResArchive:LoadIndex: Successfully loaded " + CORE::UInt32ToString( m_index.index.size() ) + " entries from index"  );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CResArchive::LoadArchive( const VFS::CString& archiveName ,
                          const VFS::CString& archivePath ,
                          const bool writableRequest      )
{GUCEF_TRACE;

    // We do not support writable archives
    if ( writableRequest ) return false;

    // Since we always need both the index file and the resource file
    // already construct the paths for both

    CORE::CString fileExt = CORE::ExtractFileExtention( archivePath ).Uppercase();
    if ( fileExt.IsNULLOrEmpty() )
    {
        m_idxPath = m_resPath = archivePath;
        m_idxPath += ".IDX";
        m_resPath += ".RES";
    }
    else
    if ( fileExt.Equals( "RES", false ) )
    {
        m_resPath = archivePath;
        m_idxPath = archivePath.CutChars( 4, false, 0 ) + ".IDX";
    }
    else
    if ( fileExt.Equals( "IDX", false ) )
    {
        m_idxPath = archivePath;
        m_resPath = archivePath.CutChars( 4, false, 0 ) + ".RES";
    }

    m_archiveName = archiveName;
    m_archivePath = archivePath;

    return LoadIndex();
}

/*-------------------------------------------------------------------------*/

bool 
CResArchive::LoadArchive( const VFS::CString& archiveName ,
                          CVFSHandlePtr vfsResource       ,
                          const bool writeableRequest     )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CResArchive::UnloadArchive( void )
{GUCEF_TRACE;

    // wipe the index
    m_index.index.clear();
    return true;
}

/*-------------------------------------------------------------------------*/

void
CResArchive::DestroyObject( VFS::CVFSHandle* objectToBeDestroyed )
{GUCEF_TRACE;

    CORE::CIOAccess* ioAccess = objectToBeDestroyed->GetAccess();
    if ( NULL != ioAccess )
        delete ioAccess;
    delete objectToBeDestroyed;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace ITV */
}; /* namespace VFSPLUGIN */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
