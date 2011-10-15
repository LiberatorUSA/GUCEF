/*
 *  guidriverRocketOpenGL: module providing a GUI backend using Rocket and OpenGL
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
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

#include <Rocket/Core.h>

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_GUI_CIGUICONTEXT_H
#include "gucefGUI_CIGUIContext.h"
#define GUCEF_GUI_CIGUICONTEXT_H
#endif /* GUCEF_GUI_CIGUICONTEXT_H ? */

#include "guidriverRocketOpenGL_CRocketGuiDriver.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUIDRIVERROCKETGL {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CRocketGuiDriver::CRocketGuiDriver( void )
    : GUI::CGUIDriver()  ,
      m_rocketRenderer()
{GUCEF_TRACE;

	Rocket::Core::SetRenderInterface( &m_rocketRenderer );
    //Rocket::Core::SetSystemInterface( &systemInterface );
    Rocket::Core::Initialise();
}

/*-------------------------------------------------------------------------*/

CRocketGuiDriver::~CRocketGuiDriver()
{GUCEF_TRACE;

    Rocket::Core::SetRenderInterface( NULL );
    Rocket::Core::SetSystemInterface( NULL );
    Rocket::Core::Shutdown();
}

/*-------------------------------------------------------------------------*/
    
GUI::TGuiContextPtr
CRocketGuiDriver::CreateGUIContext()
{GUCEF_TRACE;

    return GUI::TGuiContextPtr();
}

/*-------------------------------------------------------------------------*/
    
GUI::CGUIDriver::TGUIContextSet
CRocketGuiDriver::GetContextList( void )
{GUCEF_TRACE;

    return GUI::CGUIDriver::TGUIContextSet();
}

/*-------------------------------------------------------------------------*/
    
GUI::UInt32
CRocketGuiDriver::GetContextCount( void )
{GUCEF_TRACE;

    return 0;
}

/*-------------------------------------------------------------------------*/
    
GUI::CString
CRocketGuiDriver::GetDriverName( void )
{GUCEF_TRACE;
    
    return "RocketOpenGL";
}

/*-------------------------------------------------------------------------*/
    
GUI::CGUIDriver::TStringSet
CRocketGuiDriver::GetAvailableFormTypes( void )
{GUCEF_TRACE;

    return GUI::CGUIDriver::TStringSet();
}

/*-------------------------------------------------------------------------*/

GUI::CGUIDriver::TStringSet
CRocketGuiDriver::GetAvailableWidgetTypes( void )
{GUCEF_TRACE;

    return GUI::CGUIDriver::TStringSet();
}

/*-------------------------------------------------------------------------*/
    
const GUI::CString&
CRocketGuiDriver::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static GUI::CString classTypeName = "GUCEF::GUIDRIVERROCKETGL::CRocketGuiDriver";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/
    
GUI::CString
CRocketGuiDriver::GetDriverProperty( const GUI::CString& propertyName ) const
{GUCEF_TRACE;

    return GUI::CString();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUIDRIVERROCKETGL */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
