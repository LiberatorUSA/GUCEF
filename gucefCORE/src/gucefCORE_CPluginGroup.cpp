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
    : m_plugins()        ,
      m_pluginMetaData()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPluginGroup::CPluginGroup( const CPluginGroup& src )
    : m_plugins( src.m_plugins )                ,
      m_pluginMetaData( src.m_pluginMetaData )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPluginGroup::~CPluginGroup()
{GUCEF_TRACE;

    m_plugins.clear();
    m_pluginMetaData.clear();
}

/*-------------------------------------------------------------------------*/

CPluginGroup&
CPluginGroup::operator=( const CPluginGroup& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        m_plugins = src.m_plugins;
        m_pluginMetaData = src.m_pluginMetaData;
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

CPluginGroup::TPluginMetaDataSet&
CPluginGroup::GetPluginMetaData( void )
{GUCEF_TRACE;

    return m_pluginMetaData;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
