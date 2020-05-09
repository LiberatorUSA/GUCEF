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
{GUCEF_TRACE;

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
{GUCEF_TRACE;

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
    settingsNode.SetAttribute( "MountArchives", CORE::BoolToString( m_autoMountSubArchives ), GUCEF_DATATYPE_BOOLEAN_STRING );
    settingsNode.SetAttribute( "MountArchivesIsRecursive", CORE::BoolToString( m_autoMountSubArchivesIsRecursive ), GUCEF_DATATYPE_BOOLEAN_STRING );
    settingsNode.SetAttribute( "Writeable", CORE::BoolToString( m_writeableRequested ), GUCEF_DATATYPE_BOOLEAN_STRING );
    settingsNode.SetAttribute( "Readable", CORE::BoolToString( m_readableRequested ), GUCEF_DATATYPE_BOOLEAN_STRING );
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
        m_actualArchivePath = CORE::RelativePath( settingsNode->GetAttributeValueOrChildValueByName( "ActualArchivePath" ) );
        m_archivePath = settingsNode->GetAttributeValueOrChildValueByName( "Path" );
        if ( m_archivePath.IsNULLOrEmpty() )
            m_archivePath = m_actualArchivePath;
        if ( m_actualArchivePath.IsNULLOrEmpty() )
            m_actualArchivePath = m_archivePath;
        m_archiveName = settingsNode->GetAttributeValueOrChildValueByName( "ArchiveName" );
        if ( m_archiveName.IsNULLOrEmpty() )
            m_archiveName = m_archivePath;
        m_archiveType = settingsNode->GetAttributeValueOrChildValueByName( "ArchiveType" );
        m_mountPath = settingsNode->GetAttributeValueOrChildValueByName( "MountPath" );
        m_autoMountSubArchives = CORE::StringToBool( settingsNode->GetAttributeValueOrChildValueByName( "MountArchives" ), false );
        m_autoMountSubArchivesIsRecursive = CORE::StringToBool( settingsNode->GetAttributeValueOrChildValueByName( "MountArchivesIsRecursive" ), false );
        m_writeableRequested = CORE::StringToBool( settingsNode->GetAttributeValueOrChildValueByName( "Writeable" ), false );
        m_readableRequested = CORE::StringToBool( settingsNode->GetAttributeValueOrChildValueByName( "Readable" ), true );

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

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace VFS */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          