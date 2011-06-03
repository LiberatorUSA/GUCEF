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

#include "gucefGUI_CListBox.h"

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

const CORE::CEvent CListbox::SelectionChangedEvent = "GUCEF::GUI::CListbox::SelectionChangedEvent";
const CORE::CEvent CListbox::ItemAddedEvent = "GUCEF::GUI::CListbox::ItemAddedEvent";
const CORE::CEvent CListbox::ItemRemovedEvent = "GUCEF::GUI::CListbox::ItemRemovedEvent";
const CORE::CEvent CListbox::ItemSelectedEvent = "GUCEF::GUI::CListbox::ItemSelectedEvent";
const CORE::CEvent CListbox::ItemDeselectedEvent = "GUCEF::GUI::CListbox::ItemDeselectedEvent";
const CORE::CEvent CListbox::ItemClickedEvent = "GUCEF::GUI::CListbox::ItemClickedEvent";
const CORE::CEvent CListbox::ItemDoubleClickedEvent = "GUCEF::GUI::CListbox::ItemDoubleClickedEvent";
const CORE::CEvent CListbox::ItemTripleClickedEvent = "GUCEF::GUI::CListbox::ItemTripleClickedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CListbox::RegisterEvents( void )
{GUCEF_TRACE;

    SelectionChangedEvent.Initialize();
    ItemAddedEvent.Initialize();
    ItemRemovedEvent.Initialize();
    ItemSelectedEvent.Initialize();
    ItemDeselectedEvent.Initialize();
    ItemClickedEvent.Initialize();
    ItemDoubleClickedEvent.Initialize();
    ItemTripleClickedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CListbox::CListbox( void )
    : CWidget( "Listbox" )
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CListbox::~CListbox()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

const CString&
CListbox::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CString typeName = "GUCEF::GUI::CListbox";
    return typeName;
}

/*-------------------------------------------------------------------------*/

bool
CListbox::SetAllowMultiSelect( bool allowMultiselect )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CListbox::GetAllowMultiSelect( bool allowMultiselect )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

UInt32
CListbox::GetLineCount( void )
{GUCEF_TRACE;

    return 0;
}

/*-------------------------------------------------------------------------*/

bool
CListbox::Clear( void )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CListbox::GetLineAtIndex( const UInt32 lineIndex ,
                          CString& lineText      ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CListbox::InsertLineAtIndex( const UInt32 lineIndex ,
                             const CString& text    )
{GUCEF_TRACE;


    return false;
}

/*-------------------------------------------------------------------------*/

bool
CListbox::AppendLine( const CString& text )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CListbox::GetText( CString& text ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CListbox::SetLineSelection( const UInt32 lineIndex ,
                            const bool isSelected  )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CListbox::GetLineSelection( const UInt32 lineIndex ,
                            bool& isSelected       ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CListbox::GetFirstSelectedLine( CString& text ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CListbox::GetLastSelectedLine( CString& text ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CListbox::GetSelectedLines( TStringVector& selectedLines ) const
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
