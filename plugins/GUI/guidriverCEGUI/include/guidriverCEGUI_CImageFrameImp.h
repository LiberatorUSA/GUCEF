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
//#ifndef GUCEF_GUIDRIVERCEGUI_CIMAGEFRAMEIMP_H
//#define GUCEF_GUIDRIVERCEGUI_CIMAGEFRAMEIMP_H
//
///*-------------------------------------------------------------------------//
////                                                                         //
////      INCLUDES                                                           //
////                                                                         //
////-------------------------------------------------------------------------*/
//
//#ifndef GUCEF_GUIDRIVERCEGUI_CWIDGETIMP_H
//#include "guceMyGUIOgre_CWidgetImp.h"
//#define GUCEF_GUIDRIVERCEGUI_CWIDGETIMP_H
//#endif /* GUCEF_GUIDRIVERCEGUI_CWIDGETIMP_H ? */
//
//#ifndef GUCEF_GUI_CIMAGEFRAME_H
//#include "gucefGUI_CImageFrame.h"
//#define GUCEF_GUI_CIMAGEFRAME_H
//#endif /* GUCEF_GUI_CIMAGEFRAME_H ? */
//
///*-------------------------------------------------------------------------//
////                                                                         //
////      NAMESPACE                                                          //
////                                                                         //
////-------------------------------------------------------------------------*/
//
//namespace GUIDRIVERCEGUI { class Imageset; };
//namespace GUCEF { namespace GUI { class OgreCEGUITexture; }; };
//
///*-------------------------------------------------------------------------*/
//
//namespace GUCEF {
//namespace GUIDRIVERCEGUI {
//
///*-------------------------------------------------------------------------//
////                                                                         //
////      CLASSES                                                            //
////                                                                         //
////-------------------------------------------------------------------------*/
//
//class GUCEF_GUIDRIVERCEGUI_EXPORT_CPP CImageFrameImp : public CWidgetImp< GUCEF::GUI::CImageFrame >
//{
//    public:
//       
//    CImageFrameImp( void );
//    
//    virtual ~CImageFrameImp();
//    
//    virtual bool SetImage( const GUCEF::IMAGE::CImage& srcImage );
//    
//    virtual void Hook( CEGUI::Window* window );
//    
//    private:
//    
//    CImageFrameImp( const CImageFrameImp& src );
//    CImageFrameImp& operator=( const CImageFrameImp& src );
//    
//    private:
//    
//    CEGUI::Imageset* m_imageSet;
//    GUI::OgreCEGUITexture* m_texture;
//};
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
//          
//#endif /* GUCEF_GUIDRIVERCEGUI_CIMAGEFRAMEIMP_H ? */
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
