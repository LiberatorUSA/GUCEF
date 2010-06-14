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

#ifndef GUCEF_GUI_CWIDGET_H
#include "gucefGUI_CWidget.h"
#define GUCEF_GUI_CWIDGET_H
#endif /* GUCEF_GUI_CWIDGET_H ? */

#ifndef GUCEF_CORE_CTCLONEABLEOBJ_H
#include "CTCloneableObj.h"
#define GUCEF_CORE_CTCLONEABLEOBJ_H
#endif /* GUCEF_CORE_CTCLONEABLEOBJ_H ? */

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
    
    typedef std::vector< TGridIndex >   TGridIndexVector;
    
    CGridView( void );
    
    virtual ~CGridView();
    
    virtual void SetText( const UInt32 columnIndex ,
                          const UInt32 rowIndex    ,
                          const CString& text      );

    virtual CString GetText( const UInt32 columnIndex ,
                             const UInt32 rowIndex    ) const;

    virtual void AppendText( const UInt32 columnIndex ,
                             const CString& text      );

    virtual void AppendText( const CString& text );    
    
    virtual void Clear( void );
    
    virtual void GetSelectedItems( TGridIndexVector& gridIndexList );
    
    virtual const CString& GetClassTypeName( void ) const;
    
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-08-2007 :
        - Dinand: Initial implementation

-----------------------------------------------------------------------------*/
