/*
 *  guidriverMyGUI: glue module for the MyGUI GUI backend
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

#ifndef GUCEF_MYGUI_CFORMBACKENDIMP_H
#include "guceMyGUI_CFormBackendImp.h"
#define GUCEF_MYGUI_CFORMBACKENDIMP_H
#endif /* GUCEF_MYGUI_CFORMBACKENDIMP_H ? */

#include "guidriverMyGUI_CGuiDriver.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace MYGUI {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

CGUIDriver::CGUIDriver( void )
    : GUI::CGUIDriver() ,
      m_widgets()       ,
      m_forms()         ,
{GUCEF_TRACE;   
}

/*-------------------------------------------------------------------------*/

CGUIDriver::~CGUIDriver()
{GUCEF_TRACE;
}

/*-------------------------------------------------------------------------*/
  
CGUIDriver::TStringSet
CGUIDriver::GetAvailableFormTypes( void )
{GUCEF_TRACE;
}

/*-------------------------------------------------------------------------*/
    
TStringSet
CGUIDriver::GetAvailableWidgetTypes( void )
{GUCEF_TRACE;
}

/*-------------------------------------------------------------------------*/
    
const GUI::CString&
CGUIDriver::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static GUI::CString classTypeName = "GUCEF::MYGUI::CGUIDriver";
}

/*-------------------------------------------------------------------------*/
    
GUCEF::GUI::CWidget*
CGUIDriver::CreateWidget( const GUI::CString& widgetName )
{GUCEF_TRACE;

    CButtonImp();
}

/*-------------------------------------------------------------------------*/
    
void
CGUIDriver::DestroyWidget( GUI::CWidget* widget )
{GUCEF_TRACE;
}

/*-------------------------------------------------------------------------*/
    
GUCEF::GUI::CForm*
CGUIDriver::CreateForm( const CString& formName )
{GUCEF_TRACE;
}
    
/*-------------------------------------------------------------------------*/

void
CGUIDriver::DestroyForm( GUI::CForm* form )
{GUCEF_TRACE;
}

/*-------------------------------------------------------------------------*/
    
GUCEF::GUI::CFormBackend*
CGUIDriver::CreateFormBackend( void )
{GUCEF_TRACE;

    return new CFormBackendImp();
}

/*-------------------------------------------------------------------------*/
    
void
CGUIDriver::DestroyFormBackend( GUI::CFormBackend* formBackend )
{GUCEF_TRACE;

    delete static_cast< CFormBackendImp* >( formBackend );
}

/*-------------------------------------------------------------------------*/

CGUIDriver::TWidgetSet
CGUIDriver::GetOwnedWidgets( void )
{GUCEF_TRACE;

    return m_widgets;
}

/*-------------------------------------------------------------------------*/
    
CGUIDriver::TFormSet
CGUIDriver::GetOwnedForms( void )
{GUCEF_TRACE;

    return m_forms;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MYGUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
