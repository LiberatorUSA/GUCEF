/*
 *  gucefPUBSUB: GUCEF module providing pub-sub communication facilities
 *
 *  Copyright (C) 1998 - 2022.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CSCOPEMUTEX_H
#include "gucefMT_CScopeMutex.h"
#define GUCEF_MT_CSCOPEMUTEX_H
#endif /* GUCEF_MT_CSCOPEMUTEX_H ? */

#ifndef GUCEF_VFS_CVFS_H
#include "gucefVFS_CVFS.h"
#define GUCEF_VFS_CVFS_H
#endif /* GUCEF_VFS_CVFS_H ? */

#ifndef GUCEF_VFS_CVFSGLOBAL_H
#include "gucefVFS_CVfsGlobal.h"
#define GUCEF_VFS_CVFSGLOBAL_H
#endif /* GUCEF_VFS_CVFSGLOBAL_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBCLIENT_H
#include "gucefPUBSUB_CPubSubClient.h"
#define GUCEF_PUBSUB_CPUBSUBCLIENT_H
#endif /* GUCEF_PUBSUB_CPUBSUBCLIENT_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBBOOKMARKBINARYSERIALIZER_H
#include "gucefPUBSUB_CPubSubBookmarkBinarySerializer.h"
#define GUCEF_PUBSUB_CPUBSUBBOOKMARKBINARYSERIALIZER_H
#endif /* GUCEF_PUBSUB_CPUBSUBBOOKMARKBINARYSERIALIZER_H ? */

#include "gucefPUBSUB_CVfsPubSubBookmarkPersistence.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CString CVfsPubSubBookmarkPersistenceConfig::ClassTypeName = "GUCEF::PUBSUB::CVfsPubSubBookmarkPersistenceConfig";
const CString CVfsPubSubBookmarkPersistence::BookmarkPersistenceType = "VFS";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CVfsPubSubBookmarkPersistenceConfig::CVfsPubSubBookmarkPersistenceConfig( void ) 
    : CPubSubBookmarkPersistenceConfig()
    , vfsRootPath()
    , persistenceStructure( "clientType/{clientType}/topicName/{topicName}" )
{GUCEF_TRACE;
 
}

/*-------------------------------------------------------------------------*/

CVfsPubSubBookmarkPersistenceConfig::CVfsPubSubBookmarkPersistenceConfig( const CVfsPubSubBookmarkPersistenceConfig& src ) 
    : CPubSubBookmarkPersistenceConfig( src )
    , vfsRootPath( src.vfsRootPath )
    , persistenceStructure( src.persistenceStructure )
{GUCEF_TRACE;
 
}

/*-------------------------------------------------------------------------*/

CVfsPubSubBookmarkPersistenceConfig::CVfsPubSubBookmarkPersistenceConfig( const CPubSubBookmarkPersistenceConfig& cfg ) 
    : CPubSubBookmarkPersistenceConfig( cfg )
    , vfsRootPath()
    , persistenceStructure( "clientType/{clientType}/topicName/{topicName}" )
{GUCEF_TRACE;
 
    LoadCustomConfig( customConfig );
}

/*-------------------------------------------------------------------------*/

bool 
CVfsPubSubBookmarkPersistenceConfig::SaveConfig( CORE::CDataNode& cfg ) const
{GUCEF_TRACE;
    
    bool totalSuccess = true;

       
    cfg.CopySubTree( customConfig );    


    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool 
CVfsPubSubBookmarkPersistenceConfig::LoadConfig( const CORE::CDataNode& cfg )
{GUCEF_TRACE;

    if ( CPubSubBookmarkPersistenceConfig::LoadConfig( cfg ) )
    {
        return LoadCustomConfig( customConfig );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CVfsPubSubBookmarkPersistenceConfig::LoadCustomConfig( const CORE::CDataNode& cfg )
{GUCEF_TRACE;

    vfsRootPath = cfg.GetAttributeValueOrChildValueByName( "vfsRootPath" ).AsString( vfsRootPath, true );
    persistenceStructure = cfg.GetAttributeValueOrChildValueByName( "persistenceStructure" ).AsString( persistenceStructure, true );

    return true;
}

/*-------------------------------------------------------------------------*/

const CString& 
CVfsPubSubBookmarkPersistenceConfig::GetClassTypeName( void ) const
{GUCEF_TRACE;

    return ClassTypeName;
}

/*-------------------------------------------------------------------------*/

CVfsPubSubBookmarkPersistence::CVfsPubSubBookmarkPersistence( void )
    : CIPubSubBookmarkPersistence()
    , m_config()
    , m_serializationBuffer()
    , m_bookmarkFilePostfix( ".v" + CORE::ToString( CPubSubBookmarkBinarySerializer::CurrentFormatVersion ) + ".pubsubbookmark" )
    , m_bookmarkFilePostfixFilter()
    , m_lock()
{GUCEF_TRACE;

    m_bookmarkFilePostfixFilter = '*' + m_bookmarkFilePostfix; 
}

/*-------------------------------------------------------------------------*/

CVfsPubSubBookmarkPersistence::CVfsPubSubBookmarkPersistence( const CPubSubBookmarkPersistenceConfig& config )
    : CIPubSubBookmarkPersistence()
    , m_config( config )
    , m_serializationBuffer()
    , m_bookmarkFilePostfix( ".v" + CORE::ToString( CPubSubBookmarkBinarySerializer::CurrentFormatVersion ) + ".pubsubbookmark" )
    , m_bookmarkFilePostfixFilter()
    , m_lock()
{GUCEF_TRACE;

    m_bookmarkFilePostfixFilter = '*' + m_bookmarkFilePostfix;
}

/*-------------------------------------------------------------------------*/

CVfsPubSubBookmarkPersistence::CVfsPubSubBookmarkPersistence( const CVfsPubSubBookmarkPersistence& src )
    : CIPubSubBookmarkPersistence( src )
    , m_config( src.m_config )
    , m_serializationBuffer( src.m_serializationBuffer )
    , m_bookmarkFilePostfix( src.m_bookmarkFilePostfix )
    , m_bookmarkFilePostfixFilter( src.m_bookmarkFilePostfixFilter )
    , m_lock()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CVfsPubSubBookmarkPersistence::~CVfsPubSubBookmarkPersistence()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CVfsPubSubBookmarkPersistence& 
CVfsPubSubBookmarkPersistence::operator=( const CVfsPubSubBookmarkPersistence& src )
{GUCEF_TRACE;
    
    if ( this != &src )
    {
        MT::CScopeMutex lock( m_lock );
        CIPubSubBookmarkPersistence::operator=( src );
        m_config = src.m_config;
        m_bookmarkFilePostfix = src.m_bookmarkFilePostfix;
        m_bookmarkFilePostfixFilter = src.m_bookmarkFilePostfixFilter;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool 
CVfsPubSubBookmarkPersistence::GetBookmarkPersistenceRootPath( const CORE::CString& bookmarkNamespace ,
                                                               const CPubSubClient& client            ,
                                                               const CPubSubClientTopic& topic        ,
                                                               CORE::CString& rootPath                )
{GUCEF_TRACE;
    
    MT::CScopeMutex lock( m_lock );

    const CORE::CString& clientType = client.GetType();
    const CORE::CString& topicName = topic.GetTopicName();

    rootPath = m_config.persistenceStructure.ReplaceSubstr( "{bookmarkNamespace}", bookmarkNamespace );
    rootPath = rootPath.ReplaceSubstr( "{clientType}", clientType );
    rootPath = rootPath.ReplaceSubstr( "{topicName}", topicName );
    rootPath = CORE::CombinePath( m_config.vfsRootPath, rootPath );

    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CVfsPubSubBookmarkPersistence::GetBookmarkPersistenceFileName( const CPubSubBookmark& bookmark ,
                                                               CORE::CString& bookmarkName     )
{GUCEF_TRACE;
    
    MT::CScopeMutex lock( m_lock );
    bookmarkName = bookmark.GetBookmarkDateTime().ToIso8601DateTimeString( false, true ) + m_bookmarkFilePostfix;
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CVfsPubSubBookmarkPersistence::GetLatestBookmark( const CORE::CString& bookmarkNamespace ,
                                                  const CPubSubClient& client            ,
                                                  const CPubSubClientTopic& topic        ,
                                                  CPubSubBookmark& bookmark              )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    CORE::CString rootPath;
    if ( !GetBookmarkPersistenceRootPath( bookmarkNamespace ,
                                          client            ,
                                          topic             ,
                                          rootPath          ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VfsPubSubBookmarkPersistence:GetLatestBookmark: failed to generate path" );
        return false;
    }
    
    VFS::CVFS& vfs = VFS::CVfsGlobal::Instance()->GetVfs();

    CORE::CString::StringVector bmFilePaths;
    vfs.GetFileList( bmFilePaths, rootPath, false, true, m_bookmarkFilePostfixFilter, 25 );

    CORE::CString::StringVector::iterator i = bmFilePaths.begin();
    while ( i != bmFilePaths.end() )
    {
        CORE::CDynamicBuffer bookmarkResource;
        if ( vfs.LoadFile( bookmarkResource, (*i), "rb" ) )
        {
            if ( CPubSubBookmarkBinarySerializer::Deserialize( bookmark, false, bookmarkResource ) )
            {
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "VfsPubSubBookmarkPersistence:GetLatestBookmark: Successfully obtained latest bookmark (" + bookmark.ToString() + ") from " + (*i) );
                return true;
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VfsPubSubBookmarkPersistence:GetLatestBookmark: Failed to deserialize bookmark from data loaded from " + (*i) );
            }
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VfsPubSubBookmarkPersistence:GetLatestBookmark: Failed to load data from " + (*i) );
        }
        ++i;
    }

    if ( bmFilePaths.empty() )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "VfsPubSubBookmarkPersistence:GetLatestBookmark: No bookmarks found using VFS root path: " + rootPath );
    }

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CVfsPubSubBookmarkPersistence::StoreBookmark( const CORE::CString& bookmarkNamespace ,
                                              const CPubSubClient& client            ,
                                              const CPubSubClientTopic& topic        ,
                                              const CPubSubBookmark& bookmark        )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    CORE::CString bmFilename;
    bool serializeOk = false;
    const CORE::CDateTime& bmCreationDt = bookmark.GetBookmarkDateTime();
    if ( bmCreationDt != CORE::CDateTime::Empty )
    {
        serializeOk = CPubSubBookmarkBinarySerializer::Serialize( bookmark, m_serializationBuffer );
        serializeOk = GetBookmarkPersistenceFileName( bookmark, bmFilename ) && serializeOk;  
    }
    else
    {
        CPubSubBookmark nowBookmark( bookmark, true, CORE::CDateTime::NowUTCDateTime() );
        serializeOk = CPubSubBookmarkBinarySerializer::Serialize( nowBookmark, m_serializationBuffer );
        serializeOk = GetBookmarkPersistenceFileName( nowBookmark, bmFilename ) && serializeOk;
    }
    
    if ( serializeOk )
    {
        CORE::CString rootPath;
        if ( !GetBookmarkPersistenceRootPath( bookmarkNamespace ,
                                              client            ,
                                              topic             ,
                                              rootPath          ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VfsPubSubBookmarkPersistence:StoreBookmark: failed to generate root path" );
            return false;
        }    
        CORE::CString fullPath = CORE::CombinePath( rootPath, bmFilename );

        VFS::CVFS& vfs = VFS::CVfsGlobal::Instance()->GetVfs();                
        if ( vfs.StoreAsFile( fullPath, m_serializationBuffer, 0, true ) )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "VfsPubSubBookmarkPersistence:StoreBookmark: Successfully stored bookmark (" + bookmark.ToString() + ") at " + fullPath );

            // Also clean up older files as we go
            PerformMaxBookmarkFilesCleanup( rootPath );

            return true;
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VfsPubSubBookmarkPersistence:StoreBookmark: failed to store bookmark (" + bookmark.ToString() + ") at " + fullPath );
            return false;
        }
    }
    else
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VfsPubSubBookmarkPersistence:StoreBookmark: failed to serialize bookmark: " + bookmark.ToString() );
        return false;
    } 
}

/*-------------------------------------------------------------------------*/

bool
CVfsPubSubBookmarkPersistence::GetLatestBookmarks( const CORE::CString& bookmarkNamespace ,
                                                   const CPubSubClient& client            ,
                                                   const CPubSubClientTopic& topic        ,
                                                   TPubSubBookmarkVector& bookmarks       , 
                                                   UInt32 maxNrToLoad                     )
{GUCEF_TRACE;

    CORE::CString rootPath;
    if ( !GetBookmarkPersistenceRootPath( bookmarkNamespace ,
                                          client            ,
                                          topic             ,
                                          rootPath          ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VfsPubSubBookmarkPersistence:GetLatestBookmarks: failed to generate path" );
        return false;
    } 

    MT::CScopeMutex lock( m_lock );
    
    // Since bookmark files have the datetime in the same we can just get a list of files from the VFS
    // they would be automatically alphabetically ordered based on age due to the naming
    CString::StringVector files;
    VFS::CVFS& vfs = VFS::CVfsGlobal::Instance()->GetVfs();
    vfs.GetFileList( files, rootPath, false, true, m_bookmarkFilePostfixFilter, maxNrToLoad );

    CORE::CString::StringVector::iterator i = files.begin();
    while ( i != files.end() )
    {
        CORE::CDynamicBuffer bookmarkResource;
        if ( vfs.LoadFile( bookmarkResource, (*i), "rb" ) )
        {
            CPubSubBookmark bookmark;
            if ( CPubSubBookmarkBinarySerializer::Deserialize( bookmark, false, bookmarkResource ) )
            {
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "VfsPubSubBookmarkPersistence:GetLatestBookmarks: Successfully obtained bookmark (" + bookmark.ToString() + ") from " + (*i) );
                bookmarks.push_back( bookmark );
            }
            else
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VfsPubSubBookmarkPersistence:GetLatestBookmarks: Failed to deserialize bookmark from data loaded from " + (*i) );
                
                // We are looking for a list of 'latest', no gaps allowed in the ordering.
                // As such we abort if we fail to obtain a bookmark
                return false;
            }
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VfsPubSubBookmarkPersistence:GetLatestBookmarks: Failed to load data from " + (*i) );

            // We are looking for a list of 'latest', no gaps allowed in the ordering.
            // As such we abort if we fail to obtain a bookmark
            return false;
        }
        ++i;
    }

    return true;
}

/*-------------------------------------------------------------------------*/

const CORE::CString& 
CVfsPubSubBookmarkPersistence::GetType( void ) const
{GUCEF_TRACE;
    
    return BookmarkPersistenceType;
}

/*-------------------------------------------------------------------------*/

void
CVfsPubSubBookmarkPersistence::PerformMaxBookmarkFilesCleanup( const CORE::CString& rootPath )
{GUCEF_TRACE;

    // Since bookmark files have the datetime in the same we can just get a list of files from the VFS
    // they would be automatically alphabetically ordered based on age due to the naming
    CString::StringVector files;
    VFS::CVFS& vfs = VFS::CVfsGlobal::Instance()->GetVfs();
    vfs.GetFileList( files, rootPath, false, true, m_bookmarkFilePostfixFilter, GUCEF_UINT32MAX );

    if ( files.size() > (size_t) m_config.maxNrOfBookmarksToKeep )
    {    
        CString::StringVector failedFiles;
        CString::StringVector deletedFiles;
        deletedFiles.reserve( files.size() - (size_t) m_config.maxNrOfBookmarksToKeep );

        while ( files.size() > (size_t) m_config.maxNrOfBookmarksToKeep )
        {
            const CString& fileToDelete = (*files.begin());
            if ( vfs.DeleteFile( fileToDelete, true ) )
            {
                deletedFiles.push_back( fileToDelete );
            }
            else
            {
                failedFiles.push_back( fileToDelete );
            }
            files.erase( files.begin() );
        }

        CString::StringVector::iterator i = failedFiles.begin();
        while ( i != failedFiles.end() )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "VfsPubSubBookmarkPersistence: Failed to delete older bookmark file: " + (*i) );
            ++i;
        }

        i = deletedFiles.begin();
        while ( i != deletedFiles.end() )
        {
            GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "VfsPubSubBookmarkPersistence: Deleted old bookmark file: " + (*i) );
            ++i;
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CVfsPubSubBookmarkPersistence::PerformMaxBookmarkFilesCleanup( const CORE::CString& bookmarkNamespace ,
                                                               const CPubSubClient& client            ,
                                                               const CPubSubClientTopic& topic        )
{GUCEF_TRACE;

    // Is bookmark file max enabled? 
    if ( m_config.maxNrOfBookmarksToKeep < 1 )
        return;

    CORE::CString rootPath;
    if ( !GetBookmarkPersistenceRootPath( bookmarkNamespace ,
                                          client            ,
                                          topic             ,
                                          rootPath          ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VfsPubSubBookmarkPersistence:PerformMaxBookmarkFilesCleanup: failed to generate path" );
        return;
    } 

    PerformMaxBookmarkFilesCleanup( rootPath );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PUBSUB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
