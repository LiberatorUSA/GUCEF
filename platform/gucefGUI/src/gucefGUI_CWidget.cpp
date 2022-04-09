/*
 *  gucefGUI: GUCEF module providing a uniform interface towards GUI backends
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

#include "gucefGUI_CWidget.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUI {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CEvent CWidget::PropertiesLoadedEvent = "GUCEF::GUI::CWidget::PropertiesLoadedEvent";
const CORE::CEvent CWidget::PropertiesSavedEvent = "GUCEF::GUI::CWidget::PropertiesSavedEvent";
const CORE::CEvent CWidget::PropertyChangedEvent = "GUCEF::GUI::CWidget::PropertyChangedEvent";
const CORE::CEvent CWidget::MouseMoveEvent = "GUCEF::GUI::CWidget::MouseMoveEvent";
const CORE::CEvent CWidget::MouseDownEvent = "GUCEF::GUI::CWidget::MouseDownEvent";
const CORE::CEvent CWidget::MouseUpEvent = "GUCEF::GUI::CWidget::MouseUpEvent";
const CORE::CEvent CWidget::MouseClickEvent = "GUCEF::GUI::CWidget::MouseClickEvent";
const CORE::CEvent CWidget::MouseDoubleClickEvent = "GUCEF::GUI::CWidget::MouseDoubleClickEvent";
const CORE::CEvent CWidget::MouseTripleClickEvent = "GUCEF::GUI::CWidget::MouseTripleClickEvent";
const CORE::CEvent CWidget::MouseFocusReceivedEvent = "GUCEF::GUI::CWidget::MouseFocusReceivedEvent";
const CORE::CEvent CWidget::MouseFocusLostEvent = "GUCEF::GUI::CWidget::MouseFocusLostEvent";
const CORE::CEvent CWidget::KeyDownEvent = "GUCEF::GUI::CWidget::KeyDownEvent";
const CORE::CEvent CWidget::KeyUpEvent = "GUCEF::GUI::CWidget::KeyUpEvent";
const CORE::CEvent CWidget::KeyboardFocusReceivedEvent = "GUCEF::GUI::CWidget::KeyboardFocusReceivedEvent";
const CORE::CEvent CWidget::KeyboardFocusLostEvent = "GUCEF::GUI::CWidget::KeyboardFocusLostEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CWidget::RegisterEvents( void )
{GUCEF_TRACE;

    PropertiesLoadedEvent.Initialize();
    PropertiesSavedEvent.Initialize();
    PropertyChangedEvent.Initialize();
    MouseMoveEvent.Initialize();
    MouseDownEvent.Initialize();
    MouseUpEvent.Initialize();
    MouseClickEvent.Initialize();
    MouseDoubleClickEvent.Initialize();
    MouseTripleClickEvent.Initialize();
    MouseFocusReceivedEvent.Initialize();
    MouseFocusLostEvent.Initialize();    
    KeyDownEvent.Initialize();
    KeyUpEvent.Initialize();
    KeyboardFocusReceivedEvent.Initialize();
    KeyboardFocusLostEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CWidget::CWidget( void )
    : CObservingNotifier()          ,
      m_impPtr( NULL )              ,
      m_widgetTypeName( "CWidget" )
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CWidget::CWidget( const CString& widgetTypeName )
    : CObservingNotifier()               ,
      m_impPtr( NULL )                   ,
      m_widgetTypeName( widgetTypeName )
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CWidget::~CWidget()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CWidget*
CWidget::GetChildWidget( const CString& widgetName )
{GUCEF_TRACE;

    return NULL;
}

/*-------------------------------------------------------------------------*/

CString
CWidget::GetName( void ) const
{GUCEF_TRACE;
    
    return CString(); 
}

/*-------------------------------------------------------------------------*/

bool
CWidget::ShowModal( void )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CWidget::SetVisibility( const bool isVisible )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/
    
bool
CWidget::IsVisible( void ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/
    
bool
CWidget::SetEnabled( const bool isEnabled )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/
     
bool
CWidget::IsEnabled( void ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/
    
bool
CWidget::SetPosition( const CUVector& newPos )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/
    
bool
CWidget::GetPosition( CUVector& pos )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/
    
bool
CWidget::SetSize( const CUVector& newSize )
{GUCEF_TRACE;
    
    return false;
}

/*-------------------------------------------------------------------------*/
    
bool
CWidget::GetSize( CUVector& size ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CWidget::SetWidth( const CUDim& newWidth )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/
    
bool
CWidget::GetWidth( CUDim& width )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/
    
bool
CWidget::SetHeight( const CUDim& newHeight )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/
    
bool
CWidget::GetHeight( CUDim& width )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/
    
bool
CWidget::SetParentWidget( CWidget* parentWidget )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/
    
CWidget*
CWidget::GetParentWidget( void )
{GUCEF_TRACE;

    return NULL;
}

/*-------------------------------------------------------------------------*/
    
bool
CWidget::GetChildWidgetSet( TWidgetSet& childSet )
{GUCEF_TRACE;
    
    return false;
}

/*-------------------------------------------------------------------------*/
    
bool
CWidget::SetPropertyValue( const CString& widgetProperty ,
                           const CString& propertyValue  )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CWidget::GetPropertyValue( const CString& widgetProperty ,
                           CString& propertyValue        ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CWidget::SetProperties( const CORE::CValueList& list )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CWidget::GetProperties( CORE::CValueList& list ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/
    
bool
CWidget::HasProperty( const CString& widgetProperty ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CWidget::LoadProperties( CORE::CIOAccess& propertyStorage )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CWidget::SaveProperties( CORE::CIOAccess& propertyStorage ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

void
CWidget::SetImplementationPtr( void* impPtr )
{GUCEF_TRACE;

    m_impPtr = impPtr;
}

/*-------------------------------------------------------------------------*/

void*
CWidget::GetImplementationPtr( void ) const
{GUCEF_TRACE;

    return m_impPtr;
}

/*-------------------------------------------------------------------------*/

const CString&
CWidget::GetType( void ) const
{GUCEF_TRACE;

    return m_widgetTypeName;
}

/*-------------------------------------------------------------------------*/

const CString&
CWidget::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CString typeName = "GUCEF::GUI::CWidget";
    return typeName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
