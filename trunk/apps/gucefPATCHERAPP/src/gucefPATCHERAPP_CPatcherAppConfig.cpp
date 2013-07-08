/*
 *  gucefPATCHERAPP: Application module for the patcher application
 *  Copyright (C) 2002 - 2013.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
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

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#include "gucefPATCHERAPP_CPatcherAppConfig.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PATCHERAPP {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CPatcherAppConfig::CPatcherAppConfig( void )
    : CORE::CIConfigurable( true )     ,
      m_windowManagerName()            ,
      m_guiBackendName()               ,
      m_initialFormTypeName()          ,
      m_initialFormResourcePath()      ,
      m_fontAssetsToLoad()             ,
      m_logfilePath()                  ,
      m_isFileLoggerEnabled( true )    ,
      m_isConsoleLoggerEnabled( true ) ,
      m_isConsoleWindowEnabled( true )
{GUCEF_TRACE;

    InitToDefaultValues();
}

/*-------------------------------------------------------------------------*/

CPatcherAppConfig::~CPatcherAppConfig()
{GUCEF_TRACE;

    Clear();
}

/*-------------------------------------------------------------------------*/

const CString&
CPatcherAppConfig::GetGuiBackendName( void ) const
{GUCEF_TRACE;

    return m_guiBackendName;
}

/*-------------------------------------------------------------------------*/

const CString&
CPatcherAppConfig::GetWindowManagerName( void ) const
{GUCEF_TRACE;

    return m_windowManagerName;
}

/*-------------------------------------------------------------------------*/

const CString&
CPatcherAppConfig::GetInitialFormTypeName( void ) const
{GUCEF_TRACE;

    return m_initialFormTypeName;
}

/*-------------------------------------------------------------------------*/

const CString&
CPatcherAppConfig::GetInitialFormResourcePath( void ) const
{GUCEF_TRACE;

    return m_initialFormResourcePath;
}

/*-------------------------------------------------------------------------*/

const CString&
CPatcherAppConfig::GetLogFilePath( void ) const
{GUCEF_TRACE;

    return m_logfilePath;
}

/*-------------------------------------------------------------------------*/

bool
CPatcherAppConfig::IsFileLoggerEnabled( void ) const
{GUCEF_TRACE;
    
    return m_isFileLoggerEnabled;
}

/*-------------------------------------------------------------------------*/

bool
CPatcherAppConfig::IsConsoleLoggerEnabled( void ) const
{GUCEF_TRACE;

    return m_isConsoleLoggerEnabled;
}

/*-------------------------------------------------------------------------*/

bool
CPatcherAppConfig::IsConsoleWindowEnabled( void ) const
{GUCEF_TRACE;

    return m_isConsoleWindowEnabled;
}

/*-------------------------------------------------------------------------*/

const CPatcherAppConfig::TStringList&
CPatcherAppConfig::GetFontAssetsToLoad( void ) const
{GUCEF_TRACE;
    
    return m_fontAssetsToLoad;
}

/*-------------------------------------------------------------------------*/

void
CPatcherAppConfig::SetPatchEngineConfigFilePath( const CString& configFilePath )
{GUCEF_TRACE;

    m_patchEngineConfigFilePath = configFilePath;
}

/*-------------------------------------------------------------------------*/

const CString&
CPatcherAppConfig::GetPatchEngineConfigFilePath( void ) const
{GUCEF_TRACE;

    return m_patchEngineConfigFilePath;
}

/*-------------------------------------------------------------------------*/

void
CPatcherAppConfig::SetPatchEngineConfigFileCodec( const CString& configFileCodec )
{GUCEF_TRACE;

    m_patchEngineConfigFileCodec = configFileCodec;
}

/*-------------------------------------------------------------------------*/

const CString&
CPatcherAppConfig::GetPatchEngineConfigFileCodec( void ) const
{GUCEF_TRACE;

    return m_patchEngineConfigFileCodec;
}

/*-------------------------------------------------------------------------*/

bool
CPatcherAppConfig::SaveConfig( CORE::CDataNode& tree )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPatcherAppConfig::LoadConfig( const CORE::CDataNode& treeroot )
{GUCEF_TRACE;

    CORE::CDataNode::TConstDataNodeSet configNodes( treeroot.FindChildrenOfType( "PatcherAppConfig", true ) );
    CORE::CDataNode::TConstDataNodeSet::iterator i = configNodes.begin();
    if ( i != configNodes.end() )
    {
        const CORE::CDataNode* configNode = (*i);

        CORE::CString value = configNode->GetAttributeValueOrChildValueByName( "LogFilePath" );
        if ( !value.IsNULLOrEmpty() )
        {
            m_logfilePath = value;
        }
        value = configNode->GetAttributeValueOrChildValueByName( "IsFileLoggerEnabled" );
        if ( !value.IsNULLOrEmpty() )
        {
            m_isFileLoggerEnabled = CORE::StringToBool( value );
        }
        value = configNode->GetAttributeValueOrChildValueByName( "IsConsoleLoggerEnabled" );
        if ( !value.IsNULLOrEmpty() )
        {
            m_isConsoleLoggerEnabled = CORE::StringToBool( value );
        }
        value = configNode->GetAttributeValueOrChildValueByName( "IsConsoleWindowEnabled" );
        if ( !value.IsNULLOrEmpty() )
        {
            m_isConsoleWindowEnabled = CORE::StringToBool( value );
        }

        value = configNode->GetAttributeValueOrChildValueByName( "WindowManager" );
        if ( !value.IsNULLOrEmpty() )
        {
            m_windowManagerName = value;
        }

        CORE::CDataNode::TConstDataNodeSet nodeSet( configNode->FindChildrenOfType( "InitialForm", true ) );
        CORE::CDataNode::TConstDataNodeSet::iterator n = nodeSet.begin();
        if ( n != nodeSet.end() )
        {
            const CORE::CDataNode* formInfoNode = (*n);

            m_guiBackendName = formInfoNode->GetAttributeValueOrChildValueByName( "GuiBackend" );
            if ( !m_guiBackendName.IsNULLOrEmpty() )
            {
                m_initialFormTypeName = formInfoNode->GetChildValueByName( "FormTypeName" );
                m_initialFormResourcePath = formInfoNode->GetChildValueByName( "FormResource" );
            }
        }

        nodeSet = configNode->FindChildrenOfType( "FontsToLoadFromAssets", true );
        n = nodeSet.begin();
        while ( n != nodeSet.end() )
        {
            const CORE::CDataNode* fontInfoNode = (*n);
            CString fontAsset = fontInfoNode->GetAttributeValueOrChildValueByName( "FontAsset" );
            if ( !fontAsset.IsNULLOrEmpty() )
            {
                m_fontAssetsToLoad.push_back( fontAsset );
            }
            ++n;
        }

    }
    return true;
}

/*-------------------------------------------------------------------------*/

void
CPatcherAppConfig::Clear( void )
{GUCEF_TRACE;

    m_windowManagerName.Clear();
    m_guiBackendName.Clear();
    m_initialFormTypeName.Clear();
    m_initialFormResourcePath.Clear();
}

/*-------------------------------------------------------------------------*/

void
CPatcherAppConfig::InitToDefaultValues( void )
{GUCEF_TRACE;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    // Window/Gui settings
    m_windowManagerName = "Win32GL";
    m_guiBackendName = "MyGUIOpenGL";
    m_initialFormTypeName = "FormEx";
    m_initialFormResourcePath = "mainmenu.layout";

    // Logging settings
    m_logfilePath = GUCEF::CORE::RelativePath( "$CURWORKDIR$" );
    CORE::AppendToPath( m_logfilePath, "gucefPATCHERAPP_Log.txt" );
    m_isFileLoggerEnabled = true;
    m_isConsoleLoggerEnabled = true;

    // Misc settings
    m_isConsoleWindowEnabled = true;
    
    #elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX )
    
    // Window/Gui settings
    m_windowManagerName = "XWinGL";
    m_guiBackendName = "MyGUIOpenGL";
    m_initialFormTypeName = "FormEx";
    m_initialFormResourcePath = "mainmenu.layout";
    
    // Logging settings
    m_logfilePath = GUCEF::CORE::RelativePath( "$CURWORKDIR$" );
    CORE::AppendToPath( m_logfilePath, "gucefPATCHERAPP_Log.txt" );
    m_isFileLoggerEnabled = true;
    m_isConsoleLoggerEnabled = true;

    // Misc settings
    m_isConsoleWindowEnabled = true;
            
    #elif ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID )

    // Window/Gui settings
    m_windowManagerName = "AndroidGLES";
    m_guiBackendName = "MyGUIOpenGL";
    m_initialFormTypeName = "FormEx";
    m_initialFormResourcePath = "mainmenu.layout";

    // Logging settings
    m_logfilePath = GUCEF::CORE::RelativePath( "$CURWORKDIR$" );
    CORE::AppendToPath( m_logfilePath, "gucefPATCHERAPP_Log.txt" );
    m_isFileLoggerEnabled = true;
    m_isConsoleLoggerEnabled = false;

    // Misc settings
    m_isConsoleWindowEnabled = false;
        
    #endif
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PATCHERAPP */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
