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

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_GUI_CGUICONTEXT_H
#include "gucefGUI_CGUIContext.h"
#define GUCEF_GUI_CGUICONTEXT_H
#endif /* GUCEF_GUI_CGUICONTEXT_H ? */

#ifndef GUCEF_GUI_UTILS_H
#include "gucefGUI_Utils.h"
#define GUCEF_GUI_UTILS_H
#endif /* GUCEF_GUI_UTILS_H ? */

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
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/


namespace {

CString
JoinCells( const GUCEF::vector< CString >& cells )
{
    CString row;
    for ( UInt32 i=0; i<(UInt32)cells.size(); ++i )
    {
        if ( i > 0 )
            row += '\t';
        row += cells[ i ];
    }
    return row;
}

}

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
    : CWidget( "GridView" )
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CGridView::~CGridView()
{GUCEF_TRACE;


}

/*-------------------------------------------------------------------------*/

CWidget*
CGridView::Create( void )
{GUCEF_TRACE;

    return GUCEF_NEW CGridView();
}

/*-------------------------------------------------------------------------*/

UInt32
CGridView::GetRowCount( void ) const
{GUCEF_TRACE;

    return (UInt32) GetStringListProperty( "Rows" ).size();
}

/*-------------------------------------------------------------------------*/

UInt32
CGridView::GetColumnCount( void ) const
{GUCEF_TRACE;

    UInt32 columns = (UInt32) GetStringListProperty( "Columns" ).size();
    TStringVector rows = GetStringListProperty( "Rows" );
    for ( UInt32 i=0; i<(UInt32)rows.size(); ++i )
    {
        UInt32 cells = (UInt32) rows[ i ].ParseElements( '\t', true ).size();
        if ( cells > columns )
            columns = cells;
    }
    return columns;
}

/*-------------------------------------------------------------------------*/

CGridView::TStringVector
CGridView::GetRowCells( UInt32 rowIndex ) const
{GUCEF_TRACE;

    TStringVector rows = GetStringListProperty( "Rows" );
    TStringVector cells;
    if ( rowIndex >= (UInt32) rows.size() )
        return cells;
    CString::StringVector parts = rows[ rowIndex ].ParseElements( '\t', true );
    for ( UInt32 i=0; i<(UInt32)parts.size(); ++i )
        cells.push_back( parts[ i ] );
    return cells;
}

/*-------------------------------------------------------------------------*/

void
CGridView::SetText( const UInt32 columnIndex ,
                    const UInt32 rowIndex    ,
                    const CString& text      )
{GUCEF_TRACE;

    TStringVector rows = GetStringListProperty( "Rows" );
    while ( rows.size() <= rowIndex )
        rows.push_back( CString::Empty );
    TStringVector cells = GetRowCells( rowIndex );
    while ( cells.size() <= columnIndex )
        cells.push_back( CString::Empty );
    cells[ columnIndex ] = text;
    rows[ rowIndex ] = JoinCells( cells );
    SetStringListProperty( "Rows", rows );
    TGridItemAddedEventData eventData;
    eventData.GetData().column = columnIndex;
    eventData.GetData().row = rowIndex;
    NotifyObservers( GridItemAddedEvent, &eventData );
}

/*-------------------------------------------------------------------------*/

CString
CGridView::GetText( const UInt32 columnIndex ,
                   const UInt32 rowIndex    ) const
{GUCEF_TRACE;

    TStringVector cells = GetRowCells( rowIndex );
    return columnIndex < (UInt32) cells.size() ? cells[ columnIndex ] : CString::Empty;
}

/*-------------------------------------------------------------------------*/

void
CGridView::AppendText( const UInt32 columnIndex ,
                       const CString& text      )
{GUCEF_TRACE;

    UInt32 row = GetRowCount();
    SetText( columnIndex, row, text );
    TGridRowAddedEventData eventData;
    eventData.GetData().column = columnIndex;
    eventData.GetData().row = row;
    NotifyObservers( GridRowAddedEvent, &eventData );
}

/*-------------------------------------------------------------------------*/

void
CGridView::AppendText( const CString& text )
{GUCEF_TRACE;

    AppendText( 0, text );
}

/*-------------------------------------------------------------------------*/

void
CGridView::Clear( void )
{GUCEF_TRACE;

    SetIntProperty( "SelectedRow", -1 );
    SetIntProperty( "ScrollPosition", 0 );
    SetProperty( "Rows", CString::Empty );
}

/*-------------------------------------------------------------------------*/

void
CGridView::GetSelectedItems( TGridIndexVector& gridIndexList )
{GUCEF_TRACE;

    Int32 row = GetIntProperty( "SelectedRow" );
    if ( row < 0 || row >= (Int32) GetRowCount() )
        return;
    UInt32 columns = GetColumnCount();
    for ( UInt32 c=0; c<columns; ++c )
    {
        TGridIndex index;
        index.column = c;
        index.row = (UInt32) row;
        gridIndexList.push_back( index );
    }
}

/*-------------------------------------------------------------------------*/

GUCEF::vector< Int32 >
CGridView::GetColumnWidths( void ) const
{GUCEF_TRACE;

    GUCEF::vector< Int32 > widths = GetIntListProperty( "ColumnWidths" );
    UInt32 columns = GetColumnCount();
    if ( widths.size() >= columns && !widths.empty() )
        return widths;
    widths.clear();
    if ( 0 == columns )
        return widths;
    Int32 width = GetRect().width / (Int32) columns;
    for ( UInt32 i=0; i<columns; ++i )
        widths.push_back( width );
    return widths;
}

/*-------------------------------------------------------------------------*/

Int32
CGridView::GetRowHeight( void ) const
{GUCEF_TRACE;

    Int32 rowHeight = GetIntProperty( "RowHeight" );
    if ( rowHeight > 0 )
        return rowHeight;
    return GUCEF_NULL != GetContext() ? GetContext()->GetLineHeight( GetProperty( "Font" ) ) : 16;
}

/*-------------------------------------------------------------------------*/

Int32
CGridView::GetRowAt( Int32 localY ) const
{GUCEF_TRACE;

    Int32 rowHeight = GetRowHeight();
    if ( rowHeight <= 0 || localY < rowHeight )
        return -1;
    Int32 row = GetIntProperty( "ScrollPosition" ) + ( localY - rowHeight ) / rowHeight;
    return row < (Int32) GetRowCount() ? row : -1;
}

/*-------------------------------------------------------------------------*/

Int32
CGridView::GetColumnAt( Int32 localX ) const
{GUCEF_TRACE;

    GUCEF::vector< Int32 > widths = GetColumnWidths();
    Int32 x = 0;
    for ( UInt32 i=0; i<(UInt32)widths.size(); ++i )
    {
        if ( localX >= x && localX < x + widths[ i ] )
            return (Int32) i;
        x += widths[ i ];
    }
    return -1;
}

/*-------------------------------------------------------------------------*/

bool
CGridView::OnDefaultAction( const CORE::CEvent& eventId, CRoutedEventData& eventData )
{GUCEF_TRACE;

    if ( MouseMoveEvent == eventId )
    {
        SetIntProperty( "HoverIndex", GetRowAt( static_cast< CPointerEventData* >( &eventData )->localY ) );
        return true;
    }
    if ( MouseFocusLostEvent == eventId )
    {
        SetIntProperty( "HoverIndex", -1 );
        return true;
    }
    if ( MouseClickEvent == eventId && MOUSEBUTTON_LEFT == static_cast< CPointerEventData* >( &eventData )->button )
    {
        CPointerEventData* pointer = static_cast< CPointerEventData* >( &eventData );
        Int32 row = GetRowAt( pointer->localY );
        Int32 column = GetColumnAt( pointer->localX );
        if ( row < 0 )
            return false;
        TGridItemClickedEventData clicked;
        clicked.GetData().column = column < 0 ? 0 : (UInt32) column;
        clicked.GetData().row = (UInt32) row;
        NotifyObservers( GridItemClickedEvent, &clicked );
        if ( 2 == pointer->clickCount )
            ActivateItem( row, GetStringListProperty( "Rows" )[ row ] );
        else
            ChangeValueByUser( "SelectedRow", CORE::Int32ToString( row ) );
        return true;
    }
    if ( MouseWheelEvent == eventId )
    {
        Int32 visible = GetRowHeight() > 0 ? GetRect().height / GetRowHeight() - 1 : 0;
        Int32 maxScroll = (Int32) GetRowCount() - visible;
        if ( maxScroll < 0 ) maxScroll = 0;
        Int32 scroll = GetIntProperty( "ScrollPosition" ) - static_cast< CWheelEventData* >( &eventData )->delta;
        if ( scroll < 0 ) scroll = 0;
        if ( scroll > maxScroll ) scroll = maxScroll;
        ChangeValueByUser( "ScrollPosition", CORE::Int32ToString( scroll ) );
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

const CString&
CGridView::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CString classTypeName = "GUCEF::GUI::CGridView";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
