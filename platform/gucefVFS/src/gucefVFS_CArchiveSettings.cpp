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

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"     
#define DVCPPSTRINGUTILS_H
#endif /* DVCPPSTRINGUTILS_H ? */

#ifndef CDATANODE_H
#include "CDataNode.h"          /* node for data storage */
#define CDATANODE_H
#endif /* CDATANODE_H ? */

#include "gucefVFS_CArchiveSettings.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace VFS {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CString CArchiveSettings::ArchiveSpecificSettingsPrefix = "ArchiveSpecificSetting.";

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

CArchiveSettings::CArchiveSettings( void )
    : CORE::CIConfigurable()
    , m_archiveName()
    , m_archivePath()
    , m_actualArchivePath()
    , m_archiveType()
    , m_mountPath()
    , m_autoMountSubArchives( false )
    , m_autoMountSubArchivesIsRecursive( false )
    , m_writeableRequested( false )
    , m_readableRequested( true )
    , m_directoryWatchingAbilityRequested( false )
    , m_archiveSpecificSettings()
{GUCEF_TRACE;

    m_archiveSpecificSettings.SetAllowDuplicates( true );
    m_archiveSpecificSettings.SetAllowMultipleValues( true );
    m_archiveSpecificSettings.SetConfigKeyNamespace( ArchiveSpecificSettingsPrefix );
}

/*-------------------------------------------------------------------------*/

CArchiveSettings::CArchiveSettings( const CArchiveSettings& src )
    : CORE::CIConfigurable( src )
    , m_archiveName( src.m_archiveName )
    , m_archivePath( src.m_archivePath )
    , m_actualArchivePath( src.m_actualArchivePath )
    , m_archiveType( src.m_archiveType )
    , m_mountPath( src.m_mountPath )
    , m_autoMountSubArchives( src.m_autoMountSubArchives )
    , m_autoMountSubArchivesIsRecursive( src.m_autoMountSubArchivesIsRecursive )
    , m_writeableRequested( src.m_writeableRequested )
    , m_readableRequested( src.m_readableRequested )
    , m_directoryWatchingAbilityRequested( src.m_directoryWatchingAbilityRequested )
    , m_archiveSpecificSettings( src.m_archiveSpecificSettings )
{GUCEF_TRACE;

    m_archiveSpecificSettings.SetAllowDuplicates( true );
    m_archiveSpecificSettings.SetAllowMultipleValues( true );
    m_archiveSpecificSettings.SetConfigKeyNamespace( ArchiveSpecificSettingsPrefix );
}

/*-------------------------------------------------------------------------*/

CArchiveSettings::~CArchiveSettings()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CArchiveSettings& 
CArchiveSettings::operator=( const CArchiveSettings& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        m_archiveName = src.m_archiveName;
        m_archivePath = src.m_archivePath;
        m_actualArchivePath = src.m_actualArchivePath;
        m_archiveType = src.m_archiveType;
        m_mountPath = src.m_mountPath;
        m_autoMountSubArchives = src.m_autoMountSubArchives;
        m_autoMountSubArchivesIsRecursive = src.m_autoMountSubArchivesIsRecursive;
        m_writeableRequested = src.m_writeableRequested;
        m_readableRequested = src.m_readableRequested;
        m_directoryWatchingAbilityRequested = src.m_directoryWatchingAbilityRequested;
        m_archiveSpecificSettings = src.m_archiveSpecificSettings;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/
 
bool 
CArchiveSettings::SaveConfig( CORE::CDataNode& tree ) const
{GUCEF_TRACE;

    CORE::CDataNode settingsNode( "ArchiveSettings" );
    settingsNode.SetAttribute( "Path", m_archivePath );
    settingsNode.SetAttribute( "ActualArchivePath", m_actualArchivePath );
    settingsNode.SetAttribute( "ArchiveName", m_archiveName );
    settingsNode.SetAttribute( "ArchiveType", m_archiveType );
    settingsNode.SetAttribute( "MountPath", m_mountPath );
    settingsNode.SetAttribute( "MountArchives", m_autoMountSubArchives );
    settingsNode.SetAttribute( "MountArchivesIsRecursive", m_autoMountSubArchivesIsRecursive );
    settingsNode.SetAttribute( "Writeable", m_writeableRequested );
    settingsNode.SetAttribute( "Readable", m_readableRequested );
    settingsNode.SetAttribute( "DirectoryWatchingAbility", m_directoryWatchingAbilityRequested );    
    m_archiveSpecificSettings.SaveConfig( settingsNode );
    tree.AddChild( settingsNode );
    return true;
}

/*-------------------------------------------------------------------------*/
                                                                  
bool 
CArchiveSettings::LoadConfig( const CORE::CDataNode& treeroot )
{GUCEF_TRACE;

    const CORE::CDataNode* settingsNode = treeroot.Find( "ArchiveSettings" );
    
    // Check for the legacy naming
    if ( GUCEF_NULL == settingsNode )
        settingsNode = treeroot.Find( "VfsRoot" );
    
    if ( GUCEF_NULL != settingsNode )
    {
        m_actualArchivePath = CORE::RelativePath( settingsNode->GetAttributeValueOrChildValueByName( "ActualArchivePath" ).AsString( m_actualArchivePath, true ) );
        m_archivePath = settingsNode->GetAttributeValueOrChildValueByName( "Path" ).AsString( m_archivePath, true );
        if ( m_archivePath.IsNULLOrEmpty() )
            m_archivePath = m_actualArchivePath;
        if ( m_actualArchivePath.IsNULLOrEmpty() )
            m_actualArchivePath = m_archivePath;
        
        m_archiveName = settingsNode->GetAttributeValueOrChildValueByName( "ArchiveName" ).AsString( m_archiveName, true );
        if ( m_archiveName.IsNULLOrEmpty() )
            m_archiveName = m_archivePath;
        
        m_archiveType = settingsNode->GetAttributeValueOrChildValueByName( "ArchiveType" ).AsString( m_archiveType, true );
        m_mountPath = settingsNode->GetAttributeValueOrChildValueByName( "MountPath" ).AsString( m_mountPath, true );
        m_autoMountSubArchives = settingsNode->GetAttributeValueOrChildValueByName( "MountArchives" ).AsBool( m_autoMountSubArchives, true );
        m_autoMountSubArchivesIsRecursive = settingsNode->GetAttributeValueOrChildValueByName( "MountArchivesIsRecursive" ).AsBool( m_autoMountSubArchivesIsRecursive, true );
        m_writeableRequested = settingsNode->GetAttributeValueOrChildValueByName( "Writeable" ).AsBool( m_writeableRequested, true );
        m_readableRequested = settingsNode->GetAttributeValueOrChildValueByName( "Readable" ).AsBool( m_readableRequested, true );
        m_directoryWatchingAbilityRequested = settingsNode->GetAttributeValueOrChildValueByName( "DirectoryWatchingAbility" ).AsBool( m_directoryWatchingAbilityRequested, true );

        m_archiveSpecificSettings.LoadConfig( *settingsNode );

        GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "ArchiveSettings: Configuration successfully loaded for archive \"" + m_archiveName + "\"" );
        return true;
    }
    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "ArchiveSettings: Unable to load configuration for archive" );
    return false;
}

/*-------------------------------------------------------------------------*/

void 
CArchiveSettings::SetArchiveName( const CORE::CString& name )
{GUCEF_TRACE;

    m_archiveName = name;
}

/*-------------------------------------------------------------------------*/

const CORE::CString& 
CArchiveSettings::GetArchiveName( void ) const
{GUCEF_TRACE;

    return m_archiveName;
}

/*-------------------------------------------------------------------------*/

void 
CArchiveSettings::SetArchiveType( const CORE::CString& type )
{GUCEF_TRACE;

    m_archiveType = type;
}             

/*-------------------------------------------------------------------------*/
    
const CORE::CString& 
CArchiveSettings::GetArchiveType( void ) const
{GUCEF_TRACE;

    return m_archiveType;
}

/*-------------------------------------------------------------------------*/

void 
CArchiveSettings::SetArchivePath( const CORE::CString& path )
{GUCEF_TRACE;
    
    m_archivePath = path;
}

/*-------------------------------------------------------------------------*/
    
const CORE::CString& 
CArchiveSettings::GetArchivePath( void ) const
{GUCEF_TRACE;

    return m_archivePath;
}

/*-------------------------------------------------------------------------*/

void 
CArchiveSettings::SetActualArchivePath( const CORE::CString& path )
{GUCEF_TRACE;
    
    m_actualArchivePath = path;
}

/*-------------------------------------------------------------------------*/
    
const CORE::CString& 
CArchiveSettings::GetActualArchivePath( void ) const
{GUCEF_TRACE;

    return m_actualArchivePath;
}

/*-------------------------------------------------------------------------*/

void 
CArchiveSettings::SetMountPath( const CORE::CString& path )
{GUCEF_TRACE;

    m_mountPath = path;
}

/*-------------------------------------------------------------------------*/
    
const CORE::CString&
CArchiveSettings::GetMountPath( void ) const
{GUCEF_TRACE;

    return m_mountPath;
}

/*-------------------------------------------------------------------------*/

void 
CArchiveSettings::SetAutoMountSubArchives( bool autoMount )
{GUCEF_TRACE;

    m_autoMountSubArchives = autoMount;
}

/*-------------------------------------------------------------------------*/
    
bool 
CArchiveSettings::GetAutoMountSubArchives( void ) const
{GUCEF_TRACE;

    return m_autoMountSubArchives;
}

/*-------------------------------------------------------------------------*/

void 
CArchiveSettings::SetAutoMountSubArchivesIsRecursive( bool recursive )
{GUCEF_TRACE;

    m_autoMountSubArchivesIsRecursive = recursive;
}

/*-------------------------------------------------------------------------*/
    
bool 
CArchiveSettings::GetAutoMountSubArchivesIsRecursive( void ) const
{GUCEF_TRACE;

    return m_autoMountSubArchivesIsRecursive;
}

/*-------------------------------------------------------------------------*/

void 
CArchiveSettings::SetWriteableRequested( bool writeable )
{GUCEF_TRACE;

    m_writeableRequested = writeable;
}
       
/*-------------------------------------------------------------------------*/
    
bool 
CArchiveSettings::GetWriteableRequested( void ) const
{GUCEF_TRACE;

    return m_writeableRequested;
}

/*-------------------------------------------------------------------------*/

void 
CArchiveSettings::SetReadableRequested( bool readable )
{GUCEF_TRACE;

    m_readableRequested = readable;
}

/*-------------------------------------------------------------------------*/
    
bool
CArchiveSettings::GetReadableRequested( void ) const
{GUCEF_TRACE;
    
    return m_readableRequested;
}

/*-------------------------------------------------------------------------*/

const CORE::CString& 
CArchiveSettings::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CORE::CString classTypeName = "GUCEF::VFS::CArchiveSettings";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

const CORE::CValueList& 
CArchiveSettings::GetArchiveSpecificSettings( void ) const
{GUCEF_TRACE;
    
    return m_archiveSpecificSettings;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace VFS */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          