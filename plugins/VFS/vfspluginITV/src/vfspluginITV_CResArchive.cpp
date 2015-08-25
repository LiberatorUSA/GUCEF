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
    IndexVector::const_iterator i = m_index.index.begin();
    while ( i != m_index.index.end() )
    {
        CORE::CString resourceId = CORE::Int32ToString( (*i).resourceNr );
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

    CORE::UInt32 resourceId = CORE::StringToUInt32( filePath );
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

    CORE::UInt32 resourceId = CORE::StringToUInt32( filePath );
    IndexVector::const_iterator i = m_index.index.begin();
    while ( i != m_index.index.end() )
    {
        if ( (*i).resourceNr == resourceId )
        {
            // @todo: determine size from reading record
            return (*i).offset;
        }
        ++i;
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

CORE::CIOAccess*
CResArchive::LoadFile( const VFS::CString& file      ,
                       const VFS::UInt32 memLoadSize ) const
{GUCEF_TRACE;


    // We will not load anything in memory for now (ignore memLoadSize)
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
        if ( 1 != file.Read( &m_index.recordType, 2, 1 ) ) { file.Close(); return false; }

        while ( !file.Eof() )
        {
            IdxRecord idxRecord;
            if ( 1 != file.Read( &idxRecord.resourceNr, 4, 1 ) ) { file.Close(); return false; }
            if ( 1 != file.Read( &idxRecord.offset, 4, 1 ) ) { file.Close(); return false; }

            m_index.index.push_back( idxRecord );
        }

        file.Close();
    }

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "CResArchive:LoadIndex: Successfully loaded " + CORE::UInt32ToString( m_index.index.size() ) + " entries"  );
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

    CORE::CString fileExt = CORE::ExtractFileExtention( archiveName );
    if ( fileExt.IsNULLOrEmpty() )
    {
        m_idxPath = m_resPath = CORE::CombinePath( archivePath, archiveName );
        m_idxPath += ".IDX";
        m_resPath += ".RES";
    }
    else
    if ( fileExt.Equals( "RES", false ) )
    {
        m_resPath = CORE::CombinePath( archivePath, archiveName );
        CORE::CString idxFilename = archiveName.CutChars( 4, false, 0 );
        idxFilename += ".IDX";
        m_idxPath = CORE::CombinePath( archivePath, idxFilename );
    }
    else
    if ( fileExt.Equals( "IDX", false ) )
    {
        m_idxPath = CORE::CombinePath( archivePath, archiveName );
        CORE::CString resFilename = archiveName.CutChars( 4, false, 0 );
        resFilename += ".RES";
        m_resPath = CORE::CombinePath( archivePath, resFilename );
    }

    m_archiveName = archiveName;
    m_archivePath = archivePath;

    return LoadIndex();
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
