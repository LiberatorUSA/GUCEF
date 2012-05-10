///*
// *  guidriverMyGUI: glue module for the MyGUI GUI backend
// *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
// *
// *  This library is free software; you can redistribute it and/or
// *  modify it under the terms of the GNU Lesser General Public
// *  License as published by the Free Software Foundation; either
// *  version 2.1 of the License, or (at your option) any later version.
// *
// *  This library is distributed in the hope that it will be useful,
// *  but WITHOUT ANY WARRANTY; without even the implied warranty of
// *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// *  Lesser General Public License for more details.
// *
// *  You should have received a copy of the GNU Lesser General Public
// *  License along with this library; if not, write to the Free Software
// *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
// */
//
//#ifndef GUCEF_MYGUI_CCHECKBOXIMP_H
//#define GUCEF_MYGUI_CCHECKBOXIMP_H
//
///*-------------------------------------------------------------------------//
////                                                                         //
////      INCLUDES                                                           //
////                                                                         //
////-------------------------------------------------------------------------*/
//
//#ifndef _CEGUICheckbox_h_
//#include "elements/CEGUICheckbox.h"
//#define _CEGUICheckbox_h_
//#endif /* _CEGUICheckbox_h_ ? */
//
//#ifndef GUCEF_MYGUI_CWIDGETIMP_H
//#include "guceMyGUIOgre_CWidgetImp.h"
//#define GUCEF_MYGUI_CWIDGETIMP_H
//#endif /* GUCEF_MYGUI_CWIDGETIMP_H ? */
//
//#ifndef GUCEF_GUI_CCHECKBOX_H
//#include "gucefGUI_CCheckbox.h"
//#define GUCEF_GUI_CCHECKBOX_H
//#endif /* GUCEF_GUI_CCHECKBOX_H ? */
//
///*-------------------------------------------------------------------------//
////                                                                         //
////      NAMESPACE                                                          //
////                                                                         //
////-------------------------------------------------------------------------*/
//
//namespace GUCEF {
//namespace MYGUI {
//
///*-------------------------------------------------------------------------//
////                                                                         //
////      CLASSES                                                            //
////                                                                         //
////-------------------------------------------------------------------------*/
//
//class GUCEF_MYGUI_EXPORT_CPP CCheckboxImp : public CWidgetImp< GUCEF::GUI::CCheckbox >
//{
//    public:
//       
//    CCheckboxImp( void );
//    
//    virtual ~CCheckboxImp();
//    
//    virtual bool SetCheckedState( const bool isChecked );
//    
//    virtual bool GetCheckedState( void ) const;
//    
//    virtual void Hook( CEGUI::Checkbox* checkbox );
//    
//    virtual const CString& GetClassTypeName( void ) const;   
//    
//    private:
//    
//    CCheckboxImp( const CCheckboxImp& src );
//    CCheckboxImp& operator=( const CCheckboxImp& src );
//    
//    bool OnCheckStateChanged( const CEGUI::EventArgs& e );
//    
//    private:
//    
//    CEGUI::Checkbox* m_checkbox;
//};
//
///*-------------------------------------------------------------------------//
////                                                                         //
////      NAMESPACE                                                          //
////                                                                         //
////-------------------------------------------------------------------------*/
//
//}; /* namespace MYGUIOGRE */
//}; /* namespace GUCE */
//
///*-------------------------------------------------------------------------*/
//          
//#endif /* GUCEF_MYGUI_CCHECKBOXIMP_H ? */
//
///*-------------------------------------------------------------------------//
////                                                                         //
////      Info & Changes                                                     //
////                                                                         //
////-------------------------------------------------------------------------//
//
//- 18-08-2007 :
//        - Dinand: Initial implementation
//
//-----------------------------------------------------------------------------*/
