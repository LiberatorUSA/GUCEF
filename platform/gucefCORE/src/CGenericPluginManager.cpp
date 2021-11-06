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

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"         /* often used gucefCORE macros */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_CGENERICPLUGIN_H
#include "CGenericPlugin.h"
#define GUCEF_CORE_CGENERICPLUGIN_H
#endif /* GUCEF_CORE_CGENERICPLUGIN_H ? */

#ifndef GUCEF_CORE_DVFILEUTILS_H
#include "dvfileutils.h"
#define GUCEF_CORE_DVFILEUTILS_H
#endif /* GUCEF_CORE_DVFILEUTILS_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#include "CGenericPluginManager.h"

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

CGenericPluginManager::CGenericPluginManager( void )
    : CPluginManager()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CGenericPluginManager::~CGenericPluginManager()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

TPluginPtr
CGenericPluginManager::RegisterPlugin( void* modulePtr                   ,
                                       TPluginMetaDataPtr pluginMetaData )
{GUCEF_TRACE;

    CGenericPlugin* plugin = new CGenericPlugin();
    if ( plugin->Link( modulePtr      ,
                       pluginMetaData ) )
    {
        return plugin;
    }

    delete plugin;
    return NULL;
}

/*-------------------------------------------------------------------------*/

void
CGenericPluginManager::UnregisterPlugin( TPluginPtr plugin )
{GUCEF_TRACE;

    if ( !plugin.IsNULL() )
    {
        CGenericPluginPtr ourPlugin = plugin.StaticCast< CGenericPlugin >();
        if ( !ourPlugin.IsNULL() )
        {
            ourPlugin->Unlink();
        }
    }
}

/*-------------------------------------------------------------------------*/

CString
CGenericPluginManager::GetPluginType( void ) const
{GUCEF_TRACE;

    return "GucefGenericPlugin";
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace CORE */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
