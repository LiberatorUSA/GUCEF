///*
// *  guidriverCEGUI: glue module for the CEGUI GUI backend
// *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
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
//#ifndef GUCEF_CORE_CTRACER_H
//#include "CTracer.h"
//#define GUCEF_CORE_CTRACER_H
//#endif /* GUCEF_CORE_CTRACER_H ? */
//
//#include "guceMyGUIOgre_CCheckboxImp.h"
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
//CCheckboxImp::CCheckboxImp( void )
//    : CWidgetImp< GUCEF::GUI::CCheckbox >() ,
//      m_checkbox( NULL )
//{GUCEF_TRACE;
//
//}
//
///*-------------------------------------------------------------------------*/
//
//CCheckboxImp::~CCheckboxImp()
//{GUCEF_TRACE;
//
//    m_checkbox = NULL;
//}
//
///*-------------------------------------------------------------------------*/
//    
//bool
//CCheckboxImp::SetCheckedState( const bool isChecked )
//{GUCEF_TRACE;
//
//    if ( NULL != m_checkbox )
//    {
//        m_checkbox->setSelected( isChecked );
//        return true;
//    }
//    return false;
//}
//
///*-------------------------------------------------------------------------*/
//    
//bool
//CCheckboxImp::GetCheckedState( void ) const
//{GUCEF_TRACE;
//
//    if ( NULL != m_checkbox )
//    {
//        return m_checkbox->isSelected();
//    }
//    return true;
//}
//
///*-------------------------------------------------------------------------*/
//
//bool
//CCheckboxImp::OnCheckStateChanged( const CEGUI::EventArgs& e )
//{GUCEF_TRACE;
//
//    if ( m_checkbox->isSelected() )
//    {
//        NotifyObservers( BoxCheckedEvent );
//    }
//    else
//    {
//        NotifyObservers( BoxUncheckedEvent );
//    }
//    return true;
//}
//
///*-------------------------------------------------------------------------*/
//
//void
//CCheckboxImp::Hook( CEGUI::Checkbox* checkbox )
//{GUCEF_TRACE;
//
//    m_checkbox = checkbox;
//    CWidgetImp< GUCEF::GUI::CCheckbox >::Hook( checkbox );
//
//    m_checkbox->subscribeEvent( CEGUI::Checkbox::EventCheckStateChanged                        ,
//                                CEGUI::Event::Subscriber( &CCheckboxImp::OnCheckStateChanged , 
//                                                          this                               ) );
//}
//
///*-------------------------------------------------------------------------*/
//
//const CString&
//CCheckboxImp::GetClassTypeName( void ) const
//{GUCEF_TRACE;
//
//    static const CString classTypeName = "GUCE::CEGUIOGRE::CCheckboxImp";
//    return classTypeName;
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
