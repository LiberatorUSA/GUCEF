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
//#ifndef GUCEF_MYGUI_CVFSINFOPROVIDER_H
//#define GUCEF_MYGUI_CVFSINFOPROVIDER_H
//
///*-------------------------------------------------------------------------//
////                                                                         //
////      INCLUDES                                                           //
////                                                                         //
////-------------------------------------------------------------------------*/
//
//#ifndef GUCEF_GUI_CIFILESYSTEMINFOPROVIDER_H
//#include "gucefGUI_CIFileSystemInfoProvider.h"
//#define GUCEF_GUI_CIFILESYSTEMINFOPROVIDER_H
//#endif /* GUCEF_GUI_CIFILESYSTEMINFOPROVIDER_H ? */
//
//#ifndef GUCEF_MYGUI_MACROS_H
//#include "guceMyGUIOgre_macros.h"
//#define GUCEF_MYGUI_MACROS_H
//#endif /* GUCEF_MYGUI_MACROS_H ? */
//
///*-------------------------------------------------------------------------//
////                                                                         //
////      CLASSES                                                            //
////                                                                         //
////-------------------------------------------------------------------------*/
//
///*
// *      Forward declarations of classes used here 
// */
//
//namespace GUCEF { namespace VFS { class CVFS;
//                                }
//                }
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
///**
// *  Allows GUI widgets to obtain file system information.
// *  Specialization for obtaining the info from the gucefVFS system
// */
//class GUCEF_MYGUI_EXPORT_CPP CVFSInfoProvider : public GUCEF::GUI::CIFileSystemInfoProvider
//{
//    public:
//    
//    CVFSInfoProvider( void );
//    
//    virtual ~CVFSInfoProvider();
//    
//    bool GetDirectoryList( const CString& path   ,
//                           const CString& filter ,
//                           TStringSet& list      );
//
//    bool GetFileList( const CString& path   ,
//                      const CString& filter ,
//                      TStringSet& list      );
//
//    bool GetArchiveList( const CString& path   ,
//                         const CString& filter ,
//                         TStringSet& list      );
//                               
//    private:
//    
//    CVFSInfoProvider( const CVFSInfoProvider& src );
//    CVFSInfoProvider& operator=( const CVFSInfoProvider& src );
//    
//    private:
//    
//    GUCEF::VFS::CVFS* m_vfs;
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
//#endif /* GUCEF_MYGUI_CVFSINFOPROVIDER_H ? */
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
