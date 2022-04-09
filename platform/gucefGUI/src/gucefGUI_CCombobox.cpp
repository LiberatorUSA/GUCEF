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

#include "gucefGUI_CCombobox.h"

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

const CORE::CEvent CCombobox::TextChangedEvent = "GUCEF::GUI::CCombobox::TextChangedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CCombobox::RegisterEvents( void )
{GUCEF_TRACE;

    TextChangedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CCombobox::CCombobox( void )
    : CWidget( "Combobox" )
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CCombobox::~CCombobox()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

const CString&
CCombobox::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CString typeName = "GUCEF::GUI::CCombobox";
    return typeName;
}

/*-------------------------------------------------------------------------*/

bool
CCombobox::SetText( const CString& text )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/
    
CString
CCombobox::GetText( void ) const
{GUCEF_TRACE;

    return NULL;
}

/*-------------------------------------------------------------------------*/

bool
CCombobox::SetListItems( const TStringVector& items )
{GUCEF_TRACE;
    
    return false;
}

/*-------------------------------------------------------------------------*/
    
bool
CCombobox::GetListItems( TStringVector& items ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CCombobox::ClearList( void ) const
{GUCEF_TRACE;

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
