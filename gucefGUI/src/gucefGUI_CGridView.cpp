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

#include "gucefGUI_CGridView.h"

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

const CORE::CEvent CGridView::GridItemClickedEvent = "GUCEF::GUI::CGridView::GridItemClickedEvent";
const CORE::CEvent CGridView::GridItemAddedEvent = "GUCEF::GUI::CGridView::GridItemAddedEvent";
const CORE::CEvent CGridView::GridItemDeletedEvent = "GUCEF::GUI::CGridView::GridItemDeletedEvent";
const CORE::CEvent CGridView::GridRowAddedEvent = "GUCEF::GUI::CGridView::GridRowAddedEvent";
const CORE::CEvent CGridView::GridRowDeletedEvent = "GUCEF::GUI::CGridView::GridRowDeletedEvent";
const CORE::CEvent CGridView::GridColumnAddedEvent = "GUCEF::GUI::CGridView::GridColumnAddedEvent";
const CORE::CEvent CGridView::GridColumnDeletedEvent = "GUCEF::GUI::CGridView::GridColumnDeletedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CGridView::RegisterEvents( void )
{GUCEF_TRACE;

    GridItemClickedEvent.Initialize();
    GridItemAddedEvent.Initialize();
    GridItemDeletedEvent.Initialize();
    GridRowAddedEvent.Initialize();
    GridRowDeletedEvent.Initialize();
    GridColumnAddedEvent.Initialize();
    GridColumnDeletedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CGridView::CGridView( void )
    : CWidget()
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/
    
CGridView::~CGridView()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CGridView::SetText( const UInt32 columnIndex ,
                    const UInt32 rowIndex    ,
                    const CString& text      )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CString
CGridView::GetText( const UInt32 columnIndex ,
                    const UInt32 rowIndex    ) const
{GUCEF_TRACE;

    return NULL;
}

/*-------------------------------------------------------------------------*/

void
CGridView::AppendText( const UInt32 columnIndex ,
                       const CString& text      )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CGridView::AppendText( const CString& text )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CGridView::Clear( void )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
const CString&
CGridView::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CString classTypeName = "GUCEF::GUI::CGridView";
    return classTypeName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
