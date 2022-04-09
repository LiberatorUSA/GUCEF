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

#include "gucefGUI_CPushButton.h"

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

const CORE::CEvent CPushButton::ButtonDownEvent = "GUCEF::GUI::CPushButton::ButtonDownEvent";
const CORE::CEvent CPushButton::ButtonUpEvent = "GUCEF::GUI::CPushButton::ButtonUpEvent";
const CORE::CEvent CPushButton::ButtonClickedEvent = "GUCEF::GUI::CPushButton::ButtonClickedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/
    
void
CPushButton::RegisterEvents( void )
{GUCEF_TRACE;

    ButtonDownEvent.Initialize();
    ButtonUpEvent.Initialize();
    ButtonClickedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CPushButton::CPushButton( void )
    : CWidget( "PushButton" )
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CPushButton::~CPushButton()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
bool
CPushButton::SetButtonText( const CString& newText )
{
    return false;
}

/*-------------------------------------------------------------------------*/
    
bool
CPushButton::GetButtonText( CString& text ) const
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
