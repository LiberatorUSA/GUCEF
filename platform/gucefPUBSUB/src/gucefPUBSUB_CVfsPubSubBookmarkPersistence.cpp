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
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CVfsPubSubBookmarkPersistence::CVfsPubSubBookmarkPersistence( const CPubSubBookmarkPersistenceConfig& config )
    : CIPubSubBookmarkPersistence()
    , m_config( config )
    , m_serializationBuffer()
    , m_bookmarkFilePostfix()
{GUCEF_TRACE;
}

/*-------------------------------------------------------------------------*/

CVfsPubSubBookmarkPersistence::CVfsPubSubBookmarkPersistence( const CVfsPubSubBookmarkPersistence& src )
    : CIPubSubBookmarkPersistence( src )
    , m_config( src.m_config )
    , m_serializationBuffer( src.m_serializationBuffer )
    , m_bookmarkFilePostfix( src.m_bookmarkFilePostfix )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CVfsPubSubBookmarkPersistence::~CVfsPubSubBookmarkPersistence()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CVfsPubSubBookmarkPersistence& 
CVfsPubSubBookmarkPersistence::operator=( const CVfsPubSubBookmarkPersistence& other )
{GUCEF_TRACE;
    
    CIPubSubBookmarkPersistence::operator=( other );
    return *this;
}

/*-------------------------------------------------------------------------*/

bool 
CVfsPubSubBookmarkPersistence::GetBookmarkPersistenceRootPath( const CPubSubClient& client     ,
                                                               const CPubSubClientTopic& topic ,
                                                               CORE::CString& rootPath         )
{GUCEF_TRACE;
    
    const CORE::CString& clientType = client.GetType();
    const CORE::CString& topicName = topic.GetTopicName();

    rootPath = m_config.persistenceStructure.ReplaceSubstr( "{clientType}", clientType );
    rootPath = rootPath.ReplaceSubstr( "{topicName}", topicName );
    rootPath = CORE::CombinePath( m_config.vfsRootPath, rootPath );

    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CVfsPubSubBookmarkPersistence::GetBookmarkPersistenceFileName( const CPubSubBookmark& bookmark ,
                                                               CORE::CString& bookmarkName     )
{GUCEF_TRACE;
    
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

    CORE::CString rootPath;
    if ( !GetBookmarkPersistenceRootPath( client   ,
                                          topic    ,
                                          rootPath ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VfsPubSubBookmarkPersistence:GetLatestBookmark: failed to generate path" );
        return false;
    }
    
    VFS::CVFS& vfs = VFS::CVfsGlobal::Instance()->GetVfs();

    //vfs.GetList(

    //CORE::CDynamicBuffer bookmarkResource;
    //vfs.LoadFile( bookmarkResource, , "rb" );

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CVfsPubSubBookmarkPersistence::StoreBookmark( const CORE::CString& bookmarkNamespace ,
                                              const CPubSubClient& client            ,
                                              const CPubSubClientTopic& topic        ,
                                              const CPubSubBookmark& bookmark        )
{GUCEF_TRACE;

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
        if ( !GetBookmarkPersistenceRootPath( client   ,
                                              topic    ,
                                              rootPath ) )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VfsPubSubBookmarkPersistence:StoreBookmark: failed to generate root path" );
            return false;
        }    
        CORE::CString fullPath = CORE::CombinePath( rootPath, bmFilename );

        VFS::CVFS& vfs = VFS::CVfsGlobal::Instance()->GetVfs();                
        if ( vfs.StoreAsFile( fullPath, m_serializationBuffer, 0, true ) )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "VfsPubSubBookmarkPersistence:StoreBookmark: Successfully stored bookmark (" + bookmark.ToString() + ") at " + fullPath );
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
                                                   TPubSubBookmarkVector& bookmark        , 
                                                   UInt32 maxNrToLoad                     )
{GUCEF_TRACE;

    CORE::CString rootPath;
    if ( !GetBookmarkPersistenceRootPath( client   ,
                                          topic    ,
                                          rootPath ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "VfsPubSubBookmarkPersistence:GetLatestBookmarks: failed to generate path" );
        return false;
    } 

    return false;
}

/*-------------------------------------------------------------------------*/

const CORE::CString& 
CVfsPubSubBookmarkPersistence::GetType( void ) const
{GUCEF_TRACE;
    
    return BookmarkPersistenceType;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PUBSUB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
