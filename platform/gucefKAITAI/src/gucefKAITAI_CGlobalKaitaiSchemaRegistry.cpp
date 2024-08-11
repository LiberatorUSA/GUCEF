/*
 *  gucefKAITAI: Platform module supporting the Kaitai binary description format
 *
 *  Copyright (C) 1998 - 2023.  Dinand Vanvelzen
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

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_VFS_CVFSGLOBAL_H
#include "gucefVFS_CVfsGlobal.h"
#define GUCEF_VFS_CVFSGLOBAL_H
#endif /* GUCEF_VFS_CVFSGLOBAL_H ? */

#ifndef GUCEF_VFS_CVFS_H
#include "gucefVFS_CVFS.h"
#define GUCEF_VFS_CVFS_H
#endif /* GUCEF_VFS_CVFS_H ? */

#include "gucefKAITAI_CGlobalKaitaiSchemaRegistry.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace KAITAI {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CString CGlobalKaitaiSchemaRegistry::ClassTypeName = "GUCEF::KAITAI::GlobalKaitaiSchemaRegistry";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CGlobalKaitaiSchemaRegistry::CGlobalKaitaiSchemaRegistry( void )
    : CKaitaiSchemaRegistry()
    , CORE::CGloballyConfigurable( false )
    , m_vfsInitIsComplete( false )
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CGlobalKaitaiSchemaRegistry::~CGlobalKaitaiSchemaRegistry()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void 
CGlobalKaitaiSchemaRegistry::RegisterEventHandlers( void )
{GUCEF_TRACE;

    VFS::CVFS& vfs = VFS::CVfsGlobal::Instance()->GetVfs();
    TEventCallback callback( this, &CGlobalKaitaiSchemaRegistry::OnVfsInitializationCompleted );
    AsObserver().SubscribeTo( &vfs                                       ,
                              VFS::CVFS::VfsInitializationCompletedEvent ,
                              callback                                   );
    m_vfsInitIsComplete = VFS::CVfsGlobal::Instance()->GetVfs().IsInitialized();
}

/*-------------------------------------------------------------------------*/

CGlobalKaitaiSchemaRegistry::SchemaSource::SchemaSource( void )
    : location()
    , recursive( false )
    , family()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CGlobalKaitaiSchemaRegistry::OnVfsInitializationCompleted( CORE::CNotifier* notifier    ,
                                                           const CORE::CEvent& eventId  ,
                                                           CORE::CICloneable* eventData )
{GUCEF_TRACE;

    bool vfsInitWasComplete = m_vfsInitIsComplete;
    m_vfsInitIsComplete = true;

    if ( m_vfsInitIsComplete != vfsInitWasComplete )
        LoadSchemasFromSources();
}

/*-------------------------------------------------------------------------*/

bool
CGlobalKaitaiSchemaRegistry::LoadSchemasFromSources( void )
{GUCEF_TRACE;

    bool totalSuccess = true;
    SchemaSourceVector::const_iterator i = m_vfsSchemaSources.begin();
    while ( i != m_vfsSchemaSources.end() )
    {
        const SchemaSource& schemaSource = (*i);
        totalSuccess = LoadSchemasUsingVfs( schemaSource.location, schemaSource.family, schemaSource.recursive ) && totalSuccess; 
        ++i;
    }
    i = m_uriSchemaSources.begin();
    while ( i != m_uriSchemaSources.end() )
    {
        const SchemaSource& schemaSource = (*i);
        totalSuccess = LoadSchemas( schemaSource.location, schemaSource.family, schemaSource.recursive ) && totalSuccess; 
        ++i;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CGlobalKaitaiSchemaRegistry::SaveConfig( CORE::CDataNode& cfg ) const
{GUCEF_TRACE;

    // default is that we wish to save nothing which is always successful
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CGlobalKaitaiSchemaRegistry::LoadConfig( const CORE::CDataNode& cfg )
{GUCEF_TRACE;

    const CORE::CDataNode* globalKaitaiSchemaRegistryNode = cfg.Find( "GlobalKaitaiSchemaRegistry" );
    if ( GUCEF_NULL == globalKaitaiSchemaRegistryNode )
        return true; // these global settings are optional

    const CORE::CDataNode* schemaSourcesNode = globalKaitaiSchemaRegistryNode->Find( "schemaSources" );
    if ( GUCEF_NULL != schemaSourcesNode )
    {
        CORE::CDataNode::TConstDataNodeSet vfsSchemaSourceSet = schemaSourcesNode->FindChildrenOfType( "vfsSchemaSource" );
        CORE::CDataNode::TConstDataNodeSet::iterator i = vfsSchemaSourceSet.begin();
        while ( i != vfsSchemaSourceSet.end() )
        {
            const CORE::CDataNode* schemaSourceNode = (*i);
            
            SchemaSource source;
            source.location = schemaSourceNode->GetAttributeValueOrChildValueByName( "location" ).AsString( source.location, true );
            source.recursive = schemaSourceNode->GetAttributeValueOrChildValueByName( "recursive" ).AsBool( source.recursive, true );
            source.family = schemaSourceNode->GetAttributeValueOrChildValueByName( "family" ).AsString( source.family, true );

            m_vfsSchemaSources.push_back( source );
            ++i;
        }
        CORE::CDataNode::TConstDataNodeSet uriSchemaSourceSet = schemaSourcesNode->FindChildrenOfType( "uriSchemaSource" );
        i = uriSchemaSourceSet.begin();
        while ( i != uriSchemaSourceSet.end() )
        {
            const CORE::CDataNode* schemaSourceNode = (*i);
            
            SchemaSource source;
            source.location = schemaSourceNode->GetAttributeValueOrChildValueByName( "location" ).AsString( source.location, true );
            source.recursive = schemaSourceNode->GetAttributeValueOrChildValueByName( "recursive" ).AsBool( source.recursive, true );
            source.family = schemaSourceNode->GetAttributeValueOrChildValueByName( "family" ).AsString( source.family, true );

            m_uriSchemaSources.push_back( source );
            ++i;
        }
    }
    
    if ( m_vfsInitIsComplete )
        LoadSchemasFromSources();

    return true;
}

/*-------------------------------------------------------------------------*/

const CORE::CString& 
CGlobalKaitaiSchemaRegistry::GetClassTypeName( void ) const
{GUCEF_TRACE;

    return ClassTypeName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace KAITAI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/