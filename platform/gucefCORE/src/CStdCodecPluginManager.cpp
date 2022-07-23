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

const CEvent CStdCodecPluginManager::StdCodecRegisteredEvent = "GUCEF::CORE::CStdCodecPluginManager::StdCodecRegisteredEvent";
const CEvent CStdCodecPluginManager::StdCodecUnregisteredEvent = "GUCEF::CORE::CStdCodecPluginManager::StdCodecUnregisteredEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

void 
CStdCodecPluginManager::RegisterEvents( void )
{GUCEF_TRACE;

    StdCodecRegisteredEvent.Initialize();
    StdCodecUnregisteredEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CStdCodecPluginManager::CStdCodecPluginManager( void )
    : CPluginManager()
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CStdCodecPluginManager::~CStdCodecPluginManager()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CString
CStdCodecPluginManager::GetPluginType( void ) const
{GUCEF_TRACE;

    return "GucefStandardCodecPlugin";
}

/*-------------------------------------------------------------------------*/

TPluginPtr
CStdCodecPluginManager::RegisterPlugin( void* modulePtr                   ,
                                        TPluginMetaDataPtr pluginMetaData )
{GUCEF_TRACE;

    CStdCodecPlugin* plugin = new CStdCodecPlugin();
    if ( plugin->Link( modulePtr      ,
                       pluginMetaData ) )
    {
        return TPluginPtr( plugin );
    }

    delete plugin;
    return TPluginPtr();
}

/*-------------------------------------------------------------------------*/

void
CStdCodecPluginManager::UnregisterPlugin( TPluginPtr plugin )
{GUCEF_TRACE;

    if ( !plugin.IsNULL() )
    {
        CStdCodecPluginPtr stdCodecPlugin = plugin.StaticCast< CStdCodecPlugin >();
        if ( !stdCodecPlugin.IsNULL() )
        {
            stdCodecPlugin->Unlink();
        }
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
