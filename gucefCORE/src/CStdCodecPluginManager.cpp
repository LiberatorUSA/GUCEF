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

#ifndef GUCEF_CORE_CLOGMANAGER_H
#include "CLogManager.h"
#define GUCEF_CORE_CLOGMANAGER_H
#endif /* GUCEF_CORE_CLOGMANAGER_H ? */

#ifndef GUCEF_CORE_CSTDCODECPLUGIN_H
#include "CStdCodecPlugin.h"
#define GUCEF_CORE_CSTDCODECPLUGIN_H
#endif /* GUCEF_CORE_CSTDCODECPLUGIN_H ? */

#include "CStdCodecPluginManager.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

CStdCodecPluginManager* CStdCodecPluginManager::m_instance = NULL;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CStdCodecPluginManager::CStdCodecPluginManager( void )
    : CPluginManager() ,
      m_plugins()
{GUCEF_TRACE;
    
}

/*-------------------------------------------------------------------------*/

CStdCodecPluginManager::~CStdCodecPluginManager()
{GUCEF_TRACE;

    UnloadAll();
}

/*-------------------------------------------------------------------------*/

CStdCodecPluginManager*
CStdCodecPluginManager::Instance( void )
{GUCEF_TRACE;
    
    if ( NULL == m_instance )
    {
        m_instance = new CStdCodecPluginManager();
        GUCEF_SYSTEM_LOG( 0, "GUCEF::CORE::CStdCodecPluginManager Singleton created" );
    }
    return m_instance;
}

/*-------------------------------------------------------------------------*/

void
CStdCodecPluginManager::Deinstance( void )
{GUCEF_TRACE;

    delete m_instance;
    m_instance = NULL;
    
    GUCEF_SYSTEM_LOG( 0, "GUCEF::CORE::CStdCodecPluginManager Singleton destroyed" );
}

/*-------------------------------------------------------------------------*/

CStdCodecPluginManager::TPluginPtr
CStdCodecPluginManager::LoadPlugin( const CString& pluginPath )
{GUCEF_TRACE;

    CStdCodecPlugin* plugin = new CStdCodecPlugin();
    if ( plugin->Load( pluginPath ) )
    {
        TPluginPtr pluginPtr( plugin );
        m_plugins.push_back( pluginPtr );
        return pluginPtr;
    }

    delete plugin;
    return TPluginPtr(); // return NULL pointer
}

/*-------------------------------------------------------------------------*/

void
CStdCodecPluginManager::UnloadAll( void )
{GUCEF_TRACE;

    while ( !m_plugins.empty() )
    {
        ( m_plugins.back() )->Unload();
        m_plugins.pop_back();
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/