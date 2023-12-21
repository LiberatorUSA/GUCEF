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

#ifndef GUCEF_CORE_DVFILEUTILS_H
#include "dvfileutils.h"
#define GUCEF_CORE_DVFILEUTILS_H
#endif /* GUCEF_CORE_DVFILEUTILS_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUIDRIVEROGRE_COGREPLUGINADAPTER_H
#include "guidriverOgre_COgrePluginAdapter.h"
#define GUIDRIVEROGRE_COGREPLUGINADAPTER_H
#endif /* GUIDRIVEROGRE_COGREPLUGINADAPTER_H ? */

#include "guidriverOgre_COgrePluginManager.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUIDRIVEROGRE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

COgrePluginManager::COgrePluginManager( void )
    : CORE::CPluginManager()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

COgrePluginManager::~COgrePluginManager()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CORE::TPluginPtr
COgrePluginManager::RegisterPlugin( void* modulePtr                         ,
                                    CORE::TPluginMetaDataPtr pluginMetaData )
{GUCEF_TRACE;

    TCOgrePluginAdapterPtr plugin( new COgrePluginAdapter() );
    if ( plugin->Link( modulePtr      ,
                       pluginMetaData ) )
    {
        return plugin;
    }

    return CORE::TPluginPtr();
}

/*-------------------------------------------------------------------------*/

bool
COgrePluginManager::UnregisterPlugin( CORE::TPluginPtr plugin )
{GUCEF_TRACE;

    TCOgrePluginAdapterPtr ogrePlugin = plugin.StaticCast< COgrePluginAdapter >();
    if ( !ogrePlugin.IsNULL() )
    {
        return ogrePlugin->Unlink();
    }
    return false;
}

/*-------------------------------------------------------------------------*/

CORE::CString
COgrePluginManager::GetPluginType( void ) const
{GUCEF_TRACE;

    return "OgrePlugin";
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace GUIDRIVEROGRE */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
