/*
 *  guceMyGUIOgre: glue module for the MyGUI+Ogre GUI backend
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
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

#ifndef GUCEF_GUI_CGUIMANAGER_H
#include "gucefGUI_CGUIManager.h"
#define GUCEF_GUI_CGUIMANAGER_H
#endif /* GUCEF_GUI_CGUIMANAGER_H ? */

#ifndef GUCE_GUI_CGUIMANAGER_H
#include "CGUIManager.h"
#define GUCE_GUI_CGUIMANAGER_H
#endif /* GUCE_GUI_CGUIMANAGER_H ? */

#ifndef GUCE_MYGUIOGRE_H
#include "guceMyGUIOgre.h"
#define GUCE_MYGUIOGRE_H
#endif /* GUCE_MYGUIOGRE_H ? */

#include "guceMyGUIOgre_CModule.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCE {
namespace MYGUIOGRE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

bool
CModule::Load( void )
{GUCE_TRACE;

    // Register the GUI Driver at the GUCEF level
    GUCEF::GUI::CGUIManager& guiManager = *GUCEF::GUI::CGUIManager::Instance();
    guiManager.RegisterGUIDriver( "MyGUIOgre", CGUIDriver::Instance() );    
    return true;
}

/*-------------------------------------------------------------------------*/
    
bool
CModule::Unload( void )
{GUCE_TRACE;
    
    // Unregister the GUI Driver at the GUCEF level
    GUCEF::GUI::CGUIManager& guiManager = *GUCEF::GUI::CGUIManager::Instance();
    guiManager.UnregisterGUIDriverByName( "MyGUIOgre" );    
    CGUIDriver::Deinstance();        
    return true;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MYGUIOGRE */
}; /* namespace GUCE */

/*-------------------------------------------------------------------------*/
