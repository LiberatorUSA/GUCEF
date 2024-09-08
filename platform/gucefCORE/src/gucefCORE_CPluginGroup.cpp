/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
 *  Copyright (C) 2002 - 2011.  Dinand Vanvelzen
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

#include "gucefCORE_CPluginGroup.h"  /* definition of the class implemented here */

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */ 

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/
    
CPluginGroup::CPluginGroup( void )
    : CTSharedObjCreator< CPluginGroup, MT::CMutex >( this )
    , m_plugins()        
    , m_pluginMetaData()
    , m_priority( 0 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPluginGroup::CPluginGroup( const CPluginGroup& src )
    : CTSharedObjCreator< CPluginGroup, MT::CMutex >( this )
    , m_plugins( src.m_plugins )                
    , m_pluginMetaData( src.m_pluginMetaData )
    , m_priority( src.m_priority )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPluginGroup::~CPluginGroup()
{GUCEF_TRACE;

    m_plugins.clear();
    m_pluginMetaData.clear();
    m_priority = 0;
}

/*-------------------------------------------------------------------------*/

CPluginGroup&
CPluginGroup::operator=( const CPluginGroup& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        m_plugins = src.m_plugins;
        m_pluginMetaData = src.m_pluginMetaData;
        m_priority = src.m_priority;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CPluginGroup::TPluginSet&
CPluginGroup::GetPlugins( void )
{GUCEF_TRACE;

    return m_plugins;
}

/*-------------------------------------------------------------------------*/

const CPluginGroup::TPluginSet&
CPluginGroup::GetPlugins( void ) const
{GUCEF_TRACE;

    return m_plugins;
}

/*-------------------------------------------------------------------------*/

CPluginGroup::TPluginMetaDataSet&
CPluginGroup::GetPluginMetaData( void )
{GUCEF_TRACE;

    return m_pluginMetaData;
}

/*-------------------------------------------------------------------------*/

const CPluginGroup::TPluginMetaDataSet&
CPluginGroup::GetPluginMetaData( void ) const
{GUCEF_TRACE;

    return m_pluginMetaData;
}

/*-------------------------------------------------------------------------*/

bool
CPluginGroup::HasPluginWithFileName( const CString& moduleFileName ) const
{GUCEF_TRACE;

    TPluginSet::const_iterator i = m_plugins.begin();
    while ( i != m_plugins.end() )
    {
        TConstPluginMetaDataPtr metadata = (*i)->GetMetaData();
        if ( metadata )
        {
            if ( moduleFileName == metadata->GetModuleFilename() )
            {
                return true;
            }
        }
        ++i;
    }

    TPluginMetaDataSet::const_iterator n = m_pluginMetaData.begin();
    while ( n != m_pluginMetaData.end() )
    {
        if ( moduleFileName == (*n)->GetModuleFilename() )
        {
            return true;
        }
        ++n;
    }

    return false;
}

/*-------------------------------------------------------------------------*/

TPluginPtr 
CPluginGroup::FindPluginWithModuleName( const CString& moduleName ) const
{GUCEF_TRACE;

    TPluginSet::const_iterator i = m_plugins.begin();
    while ( i != m_plugins.end() )
    {
        TPluginMetaDataPtr metadata = (*i)->GetMetaData();
        if ( !metadata.IsNULL() )
        {
            if ( moduleName == metadata->GetModuleFilename() )
            {
                return (*i);
            }
        }
        ++i;
    }
    return TPluginPtr();
}

/*-------------------------------------------------------------------------*/

TPluginMetaDataPtr 
CPluginGroup::FindPluginMetaDataWithModuleName( const CString& moduleName ) const
{GUCEF_TRACE;

    TPluginMetaDataSet::const_iterator i = m_pluginMetaData.begin();
    while ( i != m_pluginMetaData.end() )
    {
        TPluginMetaDataPtr metadata = (*i);
        if ( !metadata.IsNULL() )
        {
            if ( moduleName == metadata->GetModuleFilename() )
            {
                return (*i);
            }
        }
        ++i;
    }
    return TPluginMetaDataPtr();
}

/*-------------------------------------------------------------------------*/

void 
CPluginGroup::SetGroupPriority( const UInt32 priority )
{GUCEF_TRACE;

    m_priority = priority;
}

/*-------------------------------------------------------------------------*/

UInt32 
CPluginGroup::GetGroupPriority( void ) const
{GUCEF_TRACE;

    return m_priority;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
