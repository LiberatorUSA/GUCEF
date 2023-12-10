/*
 *  gucefINPUT: GUCEF module providing input device interaction
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

#ifndef GUCEF_INPUT_CINPUTGLOBAL_H
#include "gucefINPUT_CInputGlobal.h"
#define GUCEF_INPUT_CINPUTGLOBAL_H
#endif /* GUCEF_INPUT_CINPUTGLOBAL_H ? */

#ifndef GUCEF_INPUT_CINPUTCONTROLLER_H
#include "CInputController.h"
#define GUCEF_INPUT_CINPUTCONTROLLER_H
#endif /* GUCEF_INPUT_CINPUTCONTROLLER_H ? */

#ifndef GUCEF_INPUT_CINPUTDRIVERPLUGIN_H
#include "CInputDriverPlugin.h"
#define GUCEF_INPUT_CINPUTDRIVERPLUGIN_H
#endif /* GUCEF_INPUT_CINPUTDRIVERPLUGIN_H ? */

#include "gucefINPUT_CInputDriverPluginManager.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace INPUT {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CInputDriverPluginManager::CInputDriverPluginManager( void )
    : CORE::CPluginManager()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CInputDriverPluginManager::~CInputDriverPluginManager()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CString
CInputDriverPluginManager::GetPluginType( void ) const
{GUCEF_TRACE;

    return "GucefInputDriverPlugin";
}

/*-------------------------------------------------------------------------*/

CORE::TPluginPtr
CInputDriverPluginManager::RegisterPlugin( void* modulePtr                         ,
                                           CORE::TPluginMetaDataPtr pluginMetaData )
{GUCEF_TRACE;

    TInputDriverPluginPtr plugin( new CInputDriverPlugin() );
    if ( plugin->Link( modulePtr      ,
                       pluginMetaData ) )
    {
        CInputGlobal::Instance()->GetInputController().RegisterDriver( plugin );        
        return plugin;
    }
    return TInputDriverPluginPtr(); 
}

/*-------------------------------------------------------------------------*/

bool
CInputDriverPluginManager::UnregisterPlugin( CORE::TPluginPtr plugin )
{GUCEF_TRACE;

    // First unregister from the registry
    TInputDriverPluginPtr pointerToPlugin = plugin.StaticCast< CInputDriverPlugin >();
    CInputGlobal::Instance()->GetInputController().UnregisterDriver( pointerToPlugin->GetName() );

    // Now unlink the plugin
    pointerToPlugin->Unlink();

    return true;
}
    
/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
