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

#include "gucefPUBSUB_CIPubSubBookmarkPersistence.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#define GUCEF_DEFAULT_DEFAULT_AUTO_PERSIST_INTERVAL_IN_MS           1000
#define GUCEF_DEFAULT_DEFAULT_AUTO_PERSIST_MSG_INTERVAL             1000
#define GUCEF_DEFAULT_DEFAULT_MAX_OLD_BOOKMARKS_TO_KEEP             25

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CString CPubSubBookmarkPersistenceConfig::ClassTypeName = "GUCEF::PUBSUB::CPubSubBookmarkPersistenceConfig";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CPubSubBookmarkPersistenceConfig::CPubSubBookmarkPersistenceConfig( void )
    : CORE::CIConfigurable()
    , bookmarkPersistenceType()
    , bookmarkNamespace()
    , autoPersistAlways( false )
    , autoPersistIfClientSideBookmarksNeeded( true )
    , autoPersistIntervalInMs( GUCEF_DEFAULT_DEFAULT_AUTO_PERSIST_INTERVAL_IN_MS )
    , autoPersistMsgInterval( GUCEF_DEFAULT_DEFAULT_AUTO_PERSIST_MSG_INTERVAL )
    , maxNrOfBookmarksToKeep( GUCEF_DEFAULT_DEFAULT_MAX_OLD_BOOKMARKS_TO_KEEP )
    , customConfig()    
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubBookmarkPersistenceConfig::CPubSubBookmarkPersistenceConfig( const CPubSubBookmarkPersistenceConfig& src )
    : CORE::CIConfigurable()
    , bookmarkPersistenceType( src.bookmarkPersistenceType )
    , bookmarkNamespace( src.bookmarkNamespace )
    , autoPersistAlways( src.autoPersistAlways )
    , autoPersistIfClientSideBookmarksNeeded( src.autoPersistIfClientSideBookmarksNeeded )
    , autoPersistIntervalInMs( src.autoPersistIntervalInMs )
    , autoPersistMsgInterval( src.autoPersistMsgInterval )
    , maxNrOfBookmarksToKeep( src.maxNrOfBookmarksToKeep )
    , customConfig( src.customConfig )    
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubBookmarkPersistenceConfig::~CPubSubBookmarkPersistenceConfig()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubBookmarkPersistenceConfig& 
CPubSubBookmarkPersistenceConfig::operator=( const CPubSubBookmarkPersistenceConfig& src )
{GUCEF_TRACE;
    
    if ( this != &src )
    {
        bookmarkPersistenceType = src.bookmarkPersistenceType;
        bookmarkNamespace = src.bookmarkNamespace;
        autoPersistAlways = src.autoPersistAlways;
        autoPersistIfClientSideBookmarksNeeded = src.autoPersistIfClientSideBookmarksNeeded;
        autoPersistIntervalInMs = src.autoPersistIntervalInMs;
        autoPersistMsgInterval = src.autoPersistMsgInterval;
        maxNrOfBookmarksToKeep = src.maxNrOfBookmarksToKeep;

        customConfig = src.customConfig;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubBookmarkPersistenceConfig::SaveConfig( CORE::CDataNode& cfg ) const
{GUCEF_TRACE;
    
    bool totalSuccess = true;

       
    cfg.CopySubTree( customConfig );    


    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool 
CPubSubBookmarkPersistenceConfig::LoadConfig( const CORE::CDataNode& cfg )
{GUCEF_TRACE;

    bookmarkPersistenceType = cfg.GetAttributeValueOrChildValueByName( "bookmarkPersistenceType" ).AsString( bookmarkPersistenceType, true );
    bookmarkNamespace = cfg.GetAttributeValueOrChildValueByName( "bookmarkNamespace" ).AsString( bookmarkNamespace, true );
    autoPersistAlways = cfg.GetAttributeValueOrChildValueByName( "autoPersistAlways" ).AsBool( autoPersistAlways, true );
    autoPersistIfClientSideBookmarksNeeded = cfg.GetAttributeValueOrChildValueByName( "autoPersistIfClientSideBookmarksNeeded" ).AsBool( autoPersistIfClientSideBookmarksNeeded, true );
    autoPersistIntervalInMs = cfg.GetAttributeValueOrChildValueByName( "autoPersistIntervalInMs" ).AsInt32( autoPersistIntervalInMs, true );
    autoPersistMsgInterval = cfg.GetAttributeValueOrChildValueByName( "autoPersistMsgInterval" ).AsInt32( autoPersistMsgInterval, true );
    maxNrOfBookmarksToKeep = cfg.GetAttributeValueOrChildValueByName( "maxNrOfBookmarksToKeep" ).AsInt32( maxNrOfBookmarksToKeep, true );

    const CORE::CDataNode* newCustomConfig = cfg.FindChild( "CustomConfig" );
    if ( GUCEF_NULL != newCustomConfig )
    {
        customConfig.Clear();
        customConfig.Copy( *newCustomConfig );
    }

    return true;
}

/*-------------------------------------------------------------------------*/

const CString& 
CPubSubBookmarkPersistenceConfig::GetClassTypeName( void ) const
{GUCEF_TRACE;

    return ClassTypeName;
}

/*-------------------------------------------------------------------------*/

CIPubSubBookmarkPersistence::CIPubSubBookmarkPersistence( void )
    : CORE::CTSharedPtrCreator< CIPubSubBookmarkPersistence, MT::CMutex >( this )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CIPubSubBookmarkPersistence::CIPubSubBookmarkPersistence( const CIPubSubBookmarkPersistence& src )
    : CORE::CTSharedPtrCreator< CIPubSubBookmarkPersistence, MT::CMutex >( this )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CIPubSubBookmarkPersistence::~CIPubSubBookmarkPersistence()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CIPubSubBookmarkPersistence& 
CIPubSubBookmarkPersistence::operator=( const CIPubSubBookmarkPersistence& other )
{GUCEF_TRACE;
    
    return *this;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PUBSUB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
