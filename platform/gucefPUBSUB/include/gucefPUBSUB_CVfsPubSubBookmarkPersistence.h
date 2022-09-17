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

#ifndef GUCEF_PUBSUB_CVFSPUBSUBBOOKMARKPERSISTENCE_H
#define GUCEF_PUBSUB_CVFSPUBSUBBOOKMARKPERSISTENCE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_PUBSUB_CIPUBSUBBOOKMARKPERSISTENCE_H
#include "gucefPUBSUB_CIPubSubBookmarkPersistence.h"
#define GUCEF_PUBSUB_CIPUBSUBBOOKMARKPERSISTENCE_H
#endif /* GUCEF_PUBSUB_CIPUBSUBBOOKMARKPERSISTENCE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_PUBSUB_EXPORT_CPP CVfsPubSubBookmarkPersistenceConfig : public CPubSubBookmarkPersistenceConfig
{
    public:

    static const CORE::CString ClassTypeName;

    CORE::CString vfsRootPath;
    CORE::CString persistenceStructure;

    virtual bool SaveConfig( CORE::CDataNode& cfg ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool LoadConfig( const CORE::CDataNode& cfg ) GUCEF_VIRTUAL_OVERRIDE;

    bool SaveCustomConfig( CORE::CDataNode& cfg );
    
    bool LoadCustomConfig( const CORE::CDataNode& cfg );

    virtual const CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;   

    CVfsPubSubBookmarkPersistenceConfig( void );
    CVfsPubSubBookmarkPersistenceConfig( const CPubSubBookmarkPersistenceConfig& cfg );
    CVfsPubSubBookmarkPersistenceConfig( const CVfsPubSubBookmarkPersistenceConfig& src );
};

/*-------------------------------------------------------------------------*/

class GUCEF_PUBSUB_EXPORT_CPP CVfsPubSubBookmarkPersistence : public CIPubSubBookmarkPersistence
{
    public:

    static const CORE::CString BookmarkPersistenceType;

    virtual bool GetLatestBookmark( const CORE::CString& bookmarkNamespace  ,
                                    const CPubSubClient& client             ,
                                    const CPubSubClientTopic& topic         ,
                                    CPubSubBookmark& bookmark               ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool StoreBookmark( const CORE::CString& bookmarkNamespace ,
                                const CPubSubClient& client            ,
                                const CPubSubClientTopic& topic        ,
                                const CPubSubBookmark& bookmark        ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool GetLatestBookmarks( const CORE::CString& bookmarkNamespace ,
                                     const CPubSubClient& client            ,
                                     const CPubSubClientTopic& topic        ,
                                     TPubSubBookmarkVector& bookmark        , 
                                     UInt32 maxNrToLoad                     ) GUCEF_VIRTUAL_OVERRIDE;
                                            
    virtual const CORE::CString& GetType( void ) const GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  Obtains a snapshot-in-time evaluation of the initialization completeness of the 
     *  persistence backend used 
     */
    virtual bool IsInitialized( void ) const GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  Obtains a snapshot-in-time evaluation of the current health of the archives
     *  mounted into the VFS plus the overall VFS health. 
     */
    virtual bool IsHealthy( void ) const GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  Obtains a snapshot-in-time evaluation of the current connectivity of the archives
     *  mounted into the VFS. This mainly comes into play for various forms of networked
     *  storage.
     */
    virtual bool IsConnected( void ) const GUCEF_VIRTUAL_OVERRIDE;
    
    bool GetBookmarkPersistenceRootPath( const CORE::CString& bookmarkNamespace ,
                                         const CPubSubClient& client            ,
                                         const CPubSubClientTopic& topic        ,
                                         CORE::CString& rootPath                );

    bool GetBookmarkPersistenceFileName( const CPubSubBookmark& bookmark ,
                                         CORE::CString& bookmarkName     );

    CVfsPubSubBookmarkPersistence( void );
    CVfsPubSubBookmarkPersistence( const CPubSubBookmarkPersistenceConfig& config );
    CVfsPubSubBookmarkPersistence( const CVfsPubSubBookmarkPersistence& src );
    virtual ~CVfsPubSubBookmarkPersistence();
    CVfsPubSubBookmarkPersistence& operator=( const CVfsPubSubBookmarkPersistence& other );

    private:

    void PerformMaxBookmarkFilesCleanup( const CORE::CString& bookmarkNamespace ,
                                         const CPubSubClient& client            ,
                                         const CPubSubClientTopic& topic        );

    void PerformMaxBookmarkFilesCleanup( const CORE::CString& rootPath );

    CVfsPubSubBookmarkPersistenceConfig m_config;
    CORE::CDynamicBuffer m_serializationBuffer;
    CORE::CString m_bookmarkFilePostfix;
    CORE::CString m_bookmarkFilePostfixFilter;
    MT::CMutex m_lock;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PUBSUB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PUBSUB_CVFSPUBSUBBOOKMARKPERSISTENCE_H ? */
