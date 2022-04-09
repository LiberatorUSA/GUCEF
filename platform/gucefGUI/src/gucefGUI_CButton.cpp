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

#include "gucefGUI_CButton.h"

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

const CORE::CEvent CButton::ButtonDownEvent = "GUCEF::GUI::CButton::ButtonDownEvent";
const CORE::CEvent CButton::ButtonUpEvent = "GUCEF::GUI::CButton::ButtonUpEvent";
const CORE::CEvent CButton::ButtonClickedEvent = "GUCEF::GUI::CButton::ButtonClickedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/
    
void
CButton::RegisterEvents( void )
{GUCEF_TRACE;

    ButtonDownEvent.Initialize();
    ButtonUpEvent.Initialize();
    ButtonClickedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CButton::CButton( void )
    : CWidget( "CButton" )
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CButton::~CButton()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
bool
CButton::SetButtonText( const CString& newText )
{
    return false;
}

/*-------------------------------------------------------------------------*/
    
bool
CButton::GetButtonText( CString& text ) const
{
    return false;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
