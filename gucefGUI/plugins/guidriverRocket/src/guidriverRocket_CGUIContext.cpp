/*
 *  guidriverRocket: GUI backend using Rocket
 *  Copyright (C) 2002 - 2011.  Dinand Vanvelzen
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

#ifndef GUCEF_GUIDRIVERROCKET_CFORMBACKENDIMP_H
#include "guidriverRocket_CFormBackendImp.h"
#define GUCEF_GUIDRIVERROCKET_CFORMBACKENDIMP_H
#endif /* GUCEF_GUIDRIVERROCKET_CFORMBACKENDIMP_H ? */

#include "guidriverRocket_CGUIContext.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUIDRIVERROCKET {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CGUIContext::CGUIContext( GUI::CGUIDriver* guiDriver )
    : GUI::CIGUIContext()      ,
      m_guiDriver( guiDriver )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CGUIContext::~CGUIContext()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
GUI::CWidget*
CGUIContext::CreateWidget( const GUI::CString& widgetName )
{GUCEF_TRACE;

    return NULL;
}

/*-------------------------------------------------------------------------*/
    
void
CGUIContext::DestroyWidget( GUI::CWidget* widget )
{GUCEF_TRACE;
}

/*-------------------------------------------------------------------------*/
    
GUI::CForm*
CGUIContext::CreateForm( const GUI::CString& formName )
{GUCEF_TRACE;

    return NULL;
}

/*-------------------------------------------------------------------------*/
    
void
CGUIContext::DestroyForm( GUI::CForm* form )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

GUI::CIGUIContext::TStringSet
CGUIContext::GetAvailableFormTypes( void )
{GUCEF_TRACE;

    return TStringSet();
}

/*-------------------------------------------------------------------------*/
    
GUI::CIGUIContext::TStringSet
CGUIContext::GetAvailableWidgetTypes( void )
{GUCEF_TRACE;

    return TStringSet();
}

/*-------------------------------------------------------------------------*/
    
GUI::CFormBackend*
CGUIContext::CreateFormBackend( void )
{GUCEF_TRACE;

    return new CFormBackendImp();
}

/*-------------------------------------------------------------------------*/
    
void
CGUIContext::DestroyFormBackend( GUI::CFormBackend* formBackend )
{GUCEF_TRACE;

    delete static_cast< CFormBackendImp* >( formBackend );
}

/*-------------------------------------------------------------------------*/
    
GUI::CGUIDriver*
CGUIContext::GetDriver( void )
{GUCEF_TRACE;

    return m_guiDriver;
}

/*-------------------------------------------------------------------------*/
    
GUI::CIGUIContext::TWidgetSet
CGUIContext::GetOwnedWidgets( void )
{GUCEF_TRACE;

    return TWidgetSet();
}

/*-------------------------------------------------------------------------*/
    
GUI::CIGUIContext::TFormSet
CGUIContext::GetOwnedForms( void )
{GUCEF_TRACE;

    return TFormSet();
}
    
/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUIDRIVERROCKET */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
