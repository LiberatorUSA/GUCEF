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

#ifndef GUCEF_GUI_CGRIDVIEW_H
#define GUCEF_GUI_CGRIDVIEW_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTCLONEABLEOBJ_H
#include "CTCloneableObj.h"
#define GUCEF_CORE_CTCLONEABLEOBJ_H
#endif /* GUCEF_CORE_CTCLONEABLEOBJ_H ? */

#ifndef GUCEF_GUI_CWIDGET_H
#include "gucefGUI_CWidget.h"
#define GUCEF_GUI_CWIDGET_H
#endif /* GUCEF_GUI_CWIDGET_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUI {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  A table: a header row (Columns) above rows of cells. Rows holds one entry per row with the cells
 *  separated by a tab character. A click selects a row (SelectedRow) and raises GridItemClickedEvent with
 *  the cell; a double click activates the row. Properties: Columns, ColumnWidths (empty: equal), Rows,
 *  SelectedRow, RowHeight, Font, TextColour, HeaderColour, SelectionColour, HoverColour, BackgroundColour,
 *  ScrollPosition. Runtime: HoverIndex.
 */
class GUCEF_GUI_PUBLIC_CPP CGridView : public CWidget
{
    public:

    static const CORE::CEvent GridItemClickedEvent;
    static const CORE::CEvent GridItemAddedEvent;
    static const CORE::CEvent GridItemDeletedEvent;
    static const CORE::CEvent GridRowAddedEvent;
    static const CORE::CEvent GridRowDeletedEvent;
    static const CORE::CEvent GridColumnAddedEvent;
    static const CORE::CEvent GridColumnDeletedEvent;

    struct SGridIndex
    {
        UInt32 column;
        UInt32 row;
    };
    typedef struct SGridIndex TGridIndex;
    typedef CORE::CTCloneableObj< TGridIndex > TCloneableGridIndex;

    typedef TCloneableGridIndex TGridItemClickedEventData;
    typedef TCloneableGridIndex TGridItemAddedEventData;
    typedef TCloneableGridIndex TGridItemDeletedEventData;
    typedef TCloneableGridIndex TGridRowAddedEventData;
    typedef TCloneableGridIndex TGridRowDeletedEventData;
    typedef TCloneableGridIndex TGridColumnAddedEventData;
    typedef TCloneableGridIndex TGridColumnDeletedEventData;

    static void RegisterEvents( void );

    public:

    typedef GUCEF::vector< TGridIndex > TGridIndexVector;
    typedef GUCEF::vector< CString > TStringVector;

    CGridView( void );

    virtual ~CGridView();

    virtual void SetText( const UInt32 columnIndex ,
                          const UInt32 rowIndex    ,
                          const CString& text      );

    virtual CString GetText( const UInt32 columnIndex ,
                             const UInt32 rowIndex    ) const;

    /** Adds a row with the text in the given column */
    virtual void AppendText( const UInt32 columnIndex ,
                             const CString& text      );

    /** Adds a row with the text in the first column */
    virtual void AppendText( const CString& text );

    virtual void Clear( void );

    /** Every cell of the selected row */
    virtual void GetSelectedItems( TGridIndexVector& gridIndexList );

    UInt32 GetRowCount( void ) const;

    UInt32 GetColumnCount( void ) const;

    TStringVector GetRowCells( UInt32 rowIndex ) const;

    /** Widths of the columns in pixels */
    GUCEF::vector< Int32 > GetColumnWidths( void ) const;

    Int32 GetRowHeight( void ) const;

    /** The data row at a position relative to the grid (below the header), -1 when none */
    Int32 GetRowAt( Int32 localY ) const;

    Int32 GetColumnAt( Int32 localX ) const;

    virtual const CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    static CWidget* Create( void );

    protected:

    virtual bool OnDefaultAction( const CORE::CEvent& eventId, CRoutedEventData& eventData ) GUCEF_VIRTUAL_OVERRIDE;

    private:

    CGridView( const CGridView& src );
    CGridView& operator=( const CGridView& src );
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_GUI_CGRIDVIEW_H ? */
