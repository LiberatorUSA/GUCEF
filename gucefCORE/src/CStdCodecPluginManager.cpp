/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL, 
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER 
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF 
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT 
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

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
{TRACE;
    
}

/*-------------------------------------------------------------------------*/

CStdCodecPluginManager::~CStdCodecPluginManager()
{TRACE;

    UnloadAll();
}

/*-------------------------------------------------------------------------*/

CStdCodecPluginManager*
CStdCodecPluginManager::Instance( void )
{TRACE;
    
    if ( NULL == m_instance )
    {
        m_instance = new CStdCodecPluginManager();
    }
    return m_instance;
}

/*-------------------------------------------------------------------------*/

void
CStdCodecPluginManager::Deinstance( void )
{TRACE;

    delete m_instance;
    m_instance = NULL;
}

/*-------------------------------------------------------------------------*/

CStdCodecPluginManager::TPluginPtr
CStdCodecPluginManager::LoadPlugin( const CString& pluginPath )
{TRACE;

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
{TRACE;

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