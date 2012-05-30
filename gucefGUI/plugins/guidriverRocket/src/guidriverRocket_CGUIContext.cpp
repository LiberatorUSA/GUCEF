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

#ifndef GUCEF_GUI_CFORMEX_H
#include "gucefGUI_CFormEx.h"
#define GUCEF_GUI_CFORMEX_H
#endif /* GUCEF_GUI_CFORMEX_H ? */

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

CGUIContext::CGUIContext( GUI::CGUIDriver* guiDriver           ,
                          Rocket::Core::Context* rocketContext ,
                          GUI::TWindowContextPtr windowContext ,
                          INPUT::CInputContext* inputContext   )
    : GUI::CIGUIContext()              ,
      m_guiDriver( guiDriver )         ,
      m_rocketContext( rocketContext ) ,
      m_windowContext( windowContext ) ,
      m_inputContext( inputContext )   ,
      m_inputAdapter()
{GUCEF_TRACE;

    SubscribeTo( m_windowContext.GetPointer() );
    m_inputAdapter.SetRocketContext( rocketContext );
    m_inputAdapter.StartListningForInputEvents();
    m_inputAdapter.SetInputContext( m_inputContext );
}

/*-------------------------------------------------------------------------*/

CGUIContext::~CGUIContext()
{GUCEF_TRACE;

    m_inputAdapter.StopListningForInputEvents();
    m_inputAdapter.SetRocketContext( NULL );
    m_rocketContext->RemoveReference();
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

    delete widget;
}

/*-------------------------------------------------------------------------*/
    
GUI::CForm*
CGUIContext::CreateForm( const GUI::CString& formName )
{GUCEF_TRACE;

    if ( formName.IsNULLOrEmpty() || formName == "FormEx" )
    {
        GUI::CFormEx* form = new GUI::CFormEx();
        form->SetContext( this );
        return form;
    }
    else
    if ( formName == "Form" )
    {
        GUI::CForm* form = new GUI::CForm();
        form->SetContext( this );
        return form;
    }
    return NULL;
}

/*-------------------------------------------------------------------------*/
    
void
CGUIContext::DestroyForm( GUI::CForm* form )
{GUCEF_TRACE;

    delete form;
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

    return new CFormBackendImp( this );
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

/*-------------------------------------------------------------------------*/

Rocket::Core::Context*
CGUIContext::GetRocketContext( void )
{GUCEF_TRACE;

    return m_rocketContext;
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::OnNotify( CORE::CNotifier* notifier   ,
                       const CORE::CEvent& eventID ,
                       CORE::CICloneable* evenData )
{GUCEF_TRACE;

    if ( eventID == GUI::CWindowContext::WindowContextRedrawEvent )
    {
        m_rocketContext->Update();
	    m_rocketContext->Render();
    }
    else
    if ( eventID == GUI::CWindowContext::WindowContextSizeEvent )
    {        
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "GUIContext: Resizing Rocket GUI context to " + CORE::UInt32ToString( m_windowContext->GetWidth() ) + "x" +
                                                                                                CORE::UInt32ToString( m_windowContext->GetHeight() ) );

        Rocket::Core::Vector2i newSize( m_windowContext->GetWidth(), m_windowContext->GetHeight() );
        m_rocketContext->SetDimensions( newSize );
    }
}

/*-------------------------------------------------------------------------*/

const CORE::CString&
CGUIContext::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CString classTypeName = "GUCEF::GUIDRIVERROCKET::CGUIContext";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

GUI::TWindowContextPtr
CGUIContext::GetWindowContext( void )
{GUCEF_TRACE;

    return m_windowContext;
}

/*-------------------------------------------------------------------------*/

INPUT::CInputContext*
CGUIContext::GetInputContext( void )
{GUCEF_TRACE;

    return m_inputContext;
}
    
/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUIDRIVERROCKET */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
