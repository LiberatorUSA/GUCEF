///*
// *  guidriverCEGUI: glue module for the CEGUI GUI backend
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
///*-------------------------------------------------------------------------//
////                                                                         //
////      INCLUDES                                                           //
////                                                                         //
////-------------------------------------------------------------------------*/
//
//#include "guceMyGUIOgre_CTabControlImp.h"
//
///*-------------------------------------------------------------------------//
////                                                                         //
////      NAMESPACE                                                          //
////                                                                         //
////-------------------------------------------------------------------------*/
//
//namespace GUCEF {
//namespace GUIDRIVERCEGUI {
//
///*-------------------------------------------------------------------------//
////                                                                         //
////      UTILITIES                                                          //
////                                                                         //
////-------------------------------------------------------------------------*/
//
//CTabControlImp::CTabControlImp( void )
//    : CWidgetImp< GUCEF::GUI::CTabControl >() ,
//      m_tabControl( NULL )
//{GUCEF_TRACE;
//
//}
//    
///*-------------------------------------------------------------------------*/
//    
//CTabControlImp::~CTabControlImp()
//{GUCEF_TRACE;
//
//}
//
///*-------------------------------------------------------------------------*/
//    
//GUCEF::GUI::CTabContentPane*
//CTabControlImp::GetTabContentPane( const UInt32 tabIndex )
//{GUCEF_TRACE;
//
//    return NULL;
//}
//
///*-------------------------------------------------------------------------*/
//
//bool
//CTabControlImp::SetActiveTab( const UInt32 tabIndex )
//{GUCEF_TRACE;
//
//    try
//    {
//        m_tabControl->setSelectedTabAtIndex( tabIndex );
//    }
//    catch ( CEGUI::InvalidRequestException& )
//    {
//        return false;
//    }
//    return true;
//}
//
///*-------------------------------------------------------------------------*/
//
//Int32
//CTabControlImp::GetActiveTab( void ) const
//{GUCEF_TRACE;
//
//    return (Int32) m_tabControl->getSelectedTabIndex();   
//}
//
///*-------------------------------------------------------------------------*/
//    
//const CString&
//CTabControlImp::GetClassTypeName( void ) const
//{GUCEF_TRACE;
//
//    static const CString classTypeName = "GUCE::CEGUIOGRE::CTabControlImp";
//    return classTypeName;
//}
//
///*-------------------------------------------------------------------------*/
//
//void
//CTabControlImp::Hook( CEGUI::TabControl* tabControl )
//{GUCEF_TRACE;
//
//    m_tabControl = tabControl;
//    CWidgetImp< GUCEF::GUI::CTabControl >::Hook( tabControl );
//}
//    
///*-------------------------------------------------------------------------//
////                                                                         //
////      NAMESPACE                                                          //
////                                                                         //
////-------------------------------------------------------------------------*/
//
//}; /* namespace GUIDRIVERCEGUI */
//}; /* namespace GUCEF */
//
///*-------------------------------------------------------------------------*/
