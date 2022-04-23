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

#ifndef GUCEF_PUBSUB_CIPUBSUBBOOKMARKPERSISTENCE_H
#define GUCEF_PUBSUB_CIPUBSUBBOOKMARKPERSISTENCE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_CORE_CTSHAREDPTR_H
#include "CTSharedPtr.h"
#define GUCEF_CORE_CTSHAREDPTR_H
#endif /* GUCEF_CORE_CTSHAREDPTR_H ? */

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "gucefCORE_CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBBOOKMARK_H
#include "gucefPUBSUB_CPubSubBookmark.h"
#define GUCEF_PUBSUB_CPUBSUBBOOKMARK_H
#endif /* GUCEF_PUBSUB_CPUBSUBBOOKMARK_H ? */

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

class CPubSubClient;
class CPubSubClientTopic;

/*-------------------------------------------------------------------------*/

class GUCEF_PUBSUB_EXPORT_CPP CPubSubBookmarkPersistenceConfig : public CORE::CIConfigurable
{
    public:

    static const CORE::CString ClassTypeName;

    CORE::CString bookmarkPersistenceType;
    CORE::CString bookmarkNamespace;
    CORE::CDataNode customConfig;

    CPubSubBookmarkPersistenceConfig( void );
    CPubSubBookmarkPersistenceConfig( const CPubSubBookmarkPersistenceConfig& src );
    virtual ~CPubSubBookmarkPersistenceConfig();
    CPubSubBookmarkPersistenceConfig& operator=( const CPubSubBookmarkPersistenceConfig& src );

    virtual bool SaveConfig( CORE::CDataNode& cfg ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool LoadConfig( const CORE::CDataNode& cfg ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;   
};

/*-------------------------------------------------------------------------*/

class GUCEF_PUBSUB_EXPORT_CPP CIPubSubBookmarkPersistence : public CORE::CTSharedPtrCreator< CIPubSubBookmarkPersistence, MT::CMutex >
{
    public:

    typedef CPubSubBookmark::TPubSubBookmarkVector  TPubSubBookmarkVector;

    virtual bool GetLatestBookmark( const CORE::CString& bookmarkNamespace ,
                                    const CPubSubClient& client            ,
                                    const CPubSubClientTopic& topic        ,
                                    CPubSubBookmark& bookmark              ) = 0;

    virtual bool StoreBookmark( const CORE::CString& bookmarkNamespace ,
                                const CPubSubClient& client            ,
                                const CPubSubClientTopic& topic        ,
                                const CPubSubBookmark& bookmark        ) = 0;

    virtual bool GetLatestBookmarks( const CORE::CString& bookmarkNamespace ,
                                     const CPubSubClient& client            ,
                                     const CPubSubClientTopic& topic        ,
                                     TPubSubBookmarkVector& bookmark        , 
                                     UInt32 maxNrToLoad                     ) = 0;
    
    virtual const CORE::CString& GetType( void ) const = 0;
    
    CIPubSubBookmarkPersistence( void );
    CIPubSubBookmarkPersistence( const CIPubSubBookmarkPersistence& src );
    virtual ~CIPubSubBookmarkPersistence();
    CIPubSubBookmarkPersistence& operator=( const CIPubSubBookmarkPersistence& other );
};

/*-------------------------------------------------------------------------*/

typedef CIPubSubBookmarkPersistence::TSharedPtrType          TIPubSubBookmarkPersistencePtr;
typedef CIPubSubBookmarkPersistence::TBasicSharedPtrType     TIPubSubBookmarkPersistenceBasicPtr;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PUBSUB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PUBSUB_CIPUBSUBBOOKMARKPERSISTENCE_H ? */

