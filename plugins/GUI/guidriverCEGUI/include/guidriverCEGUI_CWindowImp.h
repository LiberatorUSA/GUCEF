/*
 *  guidriverCEGUI: glue module for the CEGUI GUI backend
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
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

#ifndef GUCEF_GUIDRIVERCEGUI_CWINDOWIMP_H
#define GUCEF_GUIDRIVERCEGUI_CWINDOWIMP_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef _CEGUIFrameWindow_h_
#include "CEGUI/widgets/FrameWindow.h"
#define _CEGUIFrameWindow_h_
#endif /* _CEGUIFrameWindow_h_ ? */

#ifndef GUCEF_GUIDRIVERCEGUI_CWIDGETIMP_H
#include "guidriverCEGUI_CWidgetImp.h"
#define GUCEF_GUIDRIVERCEGUI_CWIDGETIMP_H
#endif /* GUCEF_GUIDRIVERCEGUI_CWIDGETIMP_H ? */

#ifndef GUCEF_GUI_CWINDOW_H
#include "gucefGUI_CWindow.h"
#define GUCEF_GUI_CWINDOW_H
#endif /* GUCEF_GUI_CWINDOW_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUIDRIVERCEGUI {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_GUIDRIVERCEGUI_EXPORT_CPP CWindowImp : public CWidgetImp< GUCEF::GUI::CWindow >
{
    public:
    
    CWindowImp( void );
    
    virtual ~CWindowImp();
    
    virtual bool ShowModal( void );
    
    virtual bool Hide( void );
    
    virtual bool SetTitleBarText( const CString& title );
    
    virtual bool GetTitleBarText( CString& title ) const;
    
    virtual bool SetTitleBarVisible( const bool isVisible );
    
    virtual bool GetTitleBarVisible( bool& isVisible );
    
    void Hook( CEGUI::FrameWindow* window );
    
    private:

    CWindowImp( const CWindowImp& src );
    CWindowImp& operator=( const CWindowImp& src );
    
    private:
    
    CEGUI::FrameWindow* m_window;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUIDRIVERCEGUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_GUIDRIVERCEGUI_CWINDOWIMP_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-08-2007 :
        - Dinand: Initial implementation

---------------------------------------------------------------------------*/
