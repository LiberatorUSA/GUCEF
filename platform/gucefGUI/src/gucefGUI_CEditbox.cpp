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

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#include "gucefGUI_CEditbox.h"

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

const CORE::CEvent CEditbox::SelectionChangedEvent = "GUCEF::GUI::CEditbox::SelectionChangedEvent";
const CORE::CEvent CEditbox::CaratPositionChangedEvent = "GUCEF::GUI::CEditbox::CaratPositionChangedEvent";
const CORE::CEvent CEditbox::TextChangedEvent = "GUCEF::GUI::CEditbox::TextChangedEvent";
const CORE::CEvent CEditbox::TextEnteredEvent = "GUCEF::GUI::CEditbox::TextEnteredEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/
    
void
CEditbox::RegisterEvents( void )
{GUCEF_TRACE;

    SelectionChangedEvent.Initialize();
    CaratPositionChangedEvent.Initialize();
    TextChangedEvent.Initialize();
    TextEnteredEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CEditbox::CEditbox( void )
    : CWidget( "Editbox" )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CEditbox::~CEditbox()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
bool
CEditbox::SetText( const CString& text )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/
    
CString
CEditbox::GetText( void )
{GUCEF_TRACE;

    return CString::Empty;
}

/*-------------------------------------------------------------------------*/
    
bool
CEditbox::SetSelectedText( const CString& text )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CEditbox::GetSelectedText( CString& text )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

void
CEditbox::Clear( void )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
