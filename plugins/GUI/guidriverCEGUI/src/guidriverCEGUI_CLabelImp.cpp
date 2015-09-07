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
//#include "guceMyGUIOgre_CLabelImp.h"
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
//CLabelImp::CLabelImp( void )
//    : CWidgetImp< GUCEF::GUI::CLabel >() ,
//      m_label( NULL )
//{GUCEF_TRACE;
//
//}
//
///*-------------------------------------------------------------------------*/
//
//CLabelImp::~CLabelImp()
//{GUCEF_TRACE;
//
//    m_label = NULL;
//}
//
///*-------------------------------------------------------------------------*/
//    
//bool
//CLabelImp::SetText( const CString& newText )
//{GUCEF_TRACE;
//
//    if ( NULL != m_label )
//    {
//        m_label->setText( newText.C_String() );
//        return true;
//    }
//    return false;
//}
//
///*-------------------------------------------------------------------------*/
//    
//bool
//CLabelImp::GetText( CString& text ) const
//{GUCEF_TRACE;
//
//    if ( NULL != m_label )
//    {
//        text = m_label->getText().c_str();
//        return true;
//    }
//    return false;
//}
//
///*-------------------------------------------------------------------------*/
//
//void
//CLabelImp::Hook( CEGUI::Window* label )
//{GUCEF_TRACE;
//
//    m_label = label;
//    CWidgetImp< GUCEF::GUI::CLabel >::Hook( label );
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
