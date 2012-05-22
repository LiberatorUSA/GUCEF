/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
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
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
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

#include "gucefCORE_CPluginMetaData.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

CPluginMetaData::CPluginMetaData( void )
    : CIPluginMetaData()      ,
      CIConfigurable()        ,
      m_description()         ,
      m_copyright()           ,
      m_version()             ,
      m_loaderLogicTypeName() ,
      m_pluginType()          ,
      m_moduleFilename()      ,
      m_modulePath()          ,
      m_params()
{GUCEF_TRACE;

    m_version.major = -1;
    m_version.minor = -1;
    m_version.patch = -1;
    m_version.release = -1;

    m_params.SetAllowMultipleValues( true );
}

/*-------------------------------------------------------------------------*/

CPluginMetaData::CPluginMetaData( const CPluginMetaData& src )
    : CIPluginMetaData()                                 ,
      CIConfigurable()                                   ,
      m_description( src.m_description )                 ,
      m_copyright( src.m_copyright )                     ,
      m_version( src.m_version )                         ,
      m_loaderLogicTypeName( src.m_loaderLogicTypeName ) ,
      m_pluginType( src.m_pluginType )                   ,
      m_moduleFilename( src.m_moduleFilename )           ,
      m_modulePath( src.m_modulePath )                   ,
      m_params( src.m_params )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPluginMetaData::CPluginMetaData( const CIPluginMetaData& src )
    : CIPluginMetaData()                                    ,
      CIConfigurable()                                      ,
      m_description( src.GetDescription() )                 ,
      m_copyright( src.GetCopyright() )                     ,
      m_version( src.GetVersion() )                         ,
      m_loaderLogicTypeName( src.GetLoaderLogicTypeName() ) ,
      m_pluginType( src.GetPluginType() )                   ,
      m_moduleFilename( src.GetModuleFilename() )           ,
      m_modulePath( src.GetFullModulePath() )               ,
      m_params()
{GUCEF_TRACE;

    src.GetParams( m_params );
    m_params.SetAllowMultipleValues( true );
}

/*-------------------------------------------------------------------------*/

CPluginMetaData::~CPluginMetaData()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPluginMetaData&
CPluginMetaData::operator=( const CPluginMetaData& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        m_description = src.m_description;
        m_copyright = src.m_copyright;
        m_version = src.m_version;
        m_loaderLogicTypeName = src.m_loaderLogicTypeName;
        m_pluginType = src.m_pluginType;
        m_moduleFilename = src.m_moduleFilename;
        m_modulePath = src.m_modulePath;
        m_params = src.m_params;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

void
CPluginMetaData::SetDescription( const CString& description )
{GUCEF_TRACE;

    m_description = description;
}

/*-------------------------------------------------------------------------*/

CString
CPluginMetaData::GetDescription( void ) const
{GUCEF_TRACE;

    return m_description;
}

/*-------------------------------------------------------------------------*/
    
void
CPluginMetaData::SetCopyright( const CString& copyright )
{GUCEF_TRACE;

    m_copyright = copyright;
}

/*-------------------------------------------------------------------------*/
    
CString
CPluginMetaData::GetCopyright( void ) const
{GUCEF_TRACE;

    return m_copyright;
}

/*-------------------------------------------------------------------------*/

void
CPluginMetaData::SetVersion( const TVersion& version )
{GUCEF_TRACE;

    m_version = version;
}

/*-------------------------------------------------------------------------*/

TVersion
CPluginMetaData::GetVersion( void ) const
{GUCEF_TRACE;

    return m_version;
}

/*-------------------------------------------------------------------------*/

void
CPluginMetaData::SetLoaderLogicTypeName( const CString& loaderLogicTypeName )
{GUCEF_TRACE;

    m_loaderLogicTypeName = loaderLogicTypeName;
}

/*-------------------------------------------------------------------------*/
    
CString
CPluginMetaData::GetLoaderLogicTypeName( void ) const
{GUCEF_TRACE;

    return m_loaderLogicTypeName;
}

/*-------------------------------------------------------------------------*/

void
CPluginMetaData::SetPluginType( const CString& pluginType )
{GUCEF_TRACE;

    m_pluginType = pluginType;
}

/*-------------------------------------------------------------------------*/
    
CString
CPluginMetaData::GetPluginType( void ) const
{GUCEF_TRACE;

    return m_pluginType;
}

/*-------------------------------------------------------------------------*/

void
CPluginMetaData::SetModuleFilename( const CString& moduleFilename )
{GUCEF_TRACE;

    m_moduleFilename = moduleFilename;
}
    
/*-------------------------------------------------------------------------*/

CString
CPluginMetaData::GetModuleFilename( void ) const
{GUCEF_TRACE;

    return m_moduleFilename;
}

/*-------------------------------------------------------------------------*/

void
CPluginMetaData::SetFullModulePath( const CString& modulePath )
{GUCEF_TRACE;

    m_modulePath = modulePath;
}

/*-------------------------------------------------------------------------*/
    
CString
CPluginMetaData::GetFullModulePath( void ) const
{GUCEF_TRACE;

    return m_modulePath;
}

/*-------------------------------------------------------------------------*/

void
CPluginMetaData::GetParams( CValueList& params ) const
{GUCEF_TRACE;

    params = m_params;
}

/*-------------------------------------------------------------------------*/

void
CPluginMetaData::SetParams( const CValueList& params )
{GUCEF_TRACE;

    m_params = params;
}

/*-------------------------------------------------------------------------*/

bool
CPluginMetaData::SaveConfig( CDataNode& tree )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CPluginMetaData::LoadConfig( const CDataNode& treeroot )
{GUCEF_TRACE;

    CDataNode* node = treeroot.Search( "PluginMetaData", '/', true );
    if ( 0 != node )
    {
        CString value = node->GetAttributeValue( "Description" );
        if ( value.IsNULLOrEmpty() )
        {
            m_description = value;
        }
        value = node->GetAttributeValue( "Copyright" );
        if ( value.IsNULLOrEmpty() )
        {
            m_copyright = value;
        }
        value = node->GetAttributeValue( "Path" );
        if ( value.IsNULLOrEmpty() )
        {
            m_modulePath = value;
        }
        value = node->GetAttributeValue( "Filename" );
        if ( value.IsNULLOrEmpty() )
        {
            m_moduleFilename = value;
        }
        value = node->GetAttributeValue( "Version" );
        if ( value.IsNULLOrEmpty() )
        {
            m_version = StringToVersion( value );
        }
        value = node->GetAttributeValue( "Type" );
        if ( value.IsNULLOrEmpty() )
        {
            m_pluginType = value;
        }
        value = node->GetAttributeValue( "LoaderLogicType" );
        if ( value.IsNULLOrEmpty() )
        {
            m_loaderLogicTypeName = value;
        }
        CDataNode::TDataNodeSet nodeSet = node->FindChildrenOfType( "Param" );
        CDataNode::TDataNodeSet::iterator i = nodeSet.begin();
        while ( i != nodeSet.end() )
        {
            CDataNode* childNode = (*i);
            value = childNode->GetAttributeValue( "Key" );
            if ( value.IsNULLOrEmpty() )
            {
                CString value2 = childNode->GetAttributeValue( "Value" );
                m_params.Set( value, value2 );
            }
            ++i;
        }
        return true;
    }                                       
    return false;
}

/*-------------------------------------------------------------------------*/

void
CPluginMetaData::Clear( void )
{GUCEF_TRACE;

    m_description.Clear();
    m_copyright.Clear();
    m_loaderLogicTypeName.Clear();
    m_pluginType.Clear();
    m_moduleFilename.Clear();
    m_modulePath.Clear();
    m_params.Clear();
    m_version.major = 0;
    m_version.minor = 0;
    m_version.patch = 0;
    m_version.release = 0;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
