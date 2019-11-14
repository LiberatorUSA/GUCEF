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

#ifndef GUCEF_GUIDRIVERCEGUI_CCEGUIDRIVER_H
#define GUCEF_GUIDRIVERCEGUI_CCEGUIDRIVER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_GUI_CWIDGET_H
#include "gucefGUI_CWidget.h"
#define GUCEF_GUI_CWIDGET_H
#endif /* GUCEF_GUI_CWIDGET_H ? */

#ifndef GUCEF_GUI_CFORMEX_H
#include "gucefGUI_CFormEx.h"
#define GUCEF_GUI_CFORMEX_H
#endif /* GUCEF_GUI_CFORMEX_H ? */

#ifndef GUCEF_GUI_CGUIDRIVER_H
#include "gucefGUI_CGUIDriver.h"
#define GUCEF_GUI_CGUIDRIVER_H
#endif /* GUCEF_GUI_CGUIDRIVER_H ? */

#ifndef GUCEF_GUIDRIVERCEGUI_MACROS_H
#include "guidriverCEGUI_macros.h" 
#define GUCEF_GUIDRIVERCEGUI_MACROS_H
#endif /* GUCEF_GUIDRIVERCEGUI_MACROS_H ? */

#ifndef GUCEF_GUIDRIVERMYGUI_CFORMBACKENDIMP_H
#include "guidriverCEGUI_CFormBackendImp.h"
#define GUCEF_GUIDRIVERMYGUI_CFORMBACKENDIMP_H
#endif /* GUCEF_GUIDRIVERMYGUI_CFORMBACKENDIMP_H ? */

#ifndef GUCEF_GUIDRIVERCEGUI_VFSRESOURCEPROVIDER_H
#include "guidriverCEGUI_VFSResourceProvider.h"
#define GUCEF_GUIDRIVERCEGUI_VFSRESOURCEPROVIDER_H
#endif /* GUCEF_GUIDRIVERCEGUI_VFSRESOURCEPROVIDER_H ? */

#ifndef GUCEF_GUIDRIVERCEGUI_IMAGECODECADAPTER_H
#include "guidriverCEGUI_ImageCodecAdapter.h"
#define GUCEF_GUIDRIVERCEGUI_IMAGECODECADAPTER_H
#endif /* GUCEF_GUIDRIVERCEGUI_IMAGECODECADAPTER_H ? */

#ifndef GUCEF_GUIDRIVERCEGUI_XMLPARSERADAPTER_H
#include "guidriverCEGUI_XMLParserAdapter.h"
#define GUCEF_GUIDRIVERCEGUI_XMLPARSERADAPTER_H
#endif /* GUCEF_GUIDRIVERCEGUI_XMLPARSERADAPTER_H ? */

#ifndef GUCEF_GUIDRIVERCEGUI_CLOGADAPTER_H
#include "guidriverCEGUI_CLogAdapter.h"
#define GUCEF_GUIDRIVERCEGUI_CLOGADAPTER_H
#endif /* GUCEF_GUIDRIVERCEGUI_CLOGADAPTER_H ? */


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

class GUCEF_GUIDRIVERCEGUI_EXPORT_CPP CCEGUIDriver : public GUI::CGUIDriver
{
    public:

    CCEGUIDriver( bool useglobalconfig = true );

    virtual ~CCEGUIDriver();
     
    virtual const CString& GetClassTypeName( void ) const;
    
    virtual GUCEF::GUI::CWidget* CreateWidget( const CString& widgetName );
    
    virtual void DestroyWidget( GUI::CWidget* widget );

    virtual GUI::CGUIDriver::TStringSet GetAvailableFormTypes( void );
    
    virtual GUI::CGUIDriver::TStringSet GetAvailableWidgetTypes( void );
    
    virtual GUCEF::GUI::CFormBackend* CreateFormBackend( void );
    
    virtual void DestroyFormBackend( GUI::CFormBackend* formBackend );

    virtual CEGUI::System* GetCEGui( void ) = 0;

    /**
     *      Attempts to store the given tree in the file
     *      given according to the method of the codec metadata
     *
     *      @param tree the data tree you wish to store
     *      @return wheter storing the tree was successfull
     */
    virtual bool SaveConfig( CORE::CDataNode& tree ) const;

    /**
     *      Attempts to load data from the given file to the
     *      root node given. The root data will be replaced
     *      and any children the node may already have will be deleted.
     *
     *      @param treeroot pointer to the node that is to act as root of the data tree
     *      @return whether building the tree from the given file was successfull.
     */
    virtual bool LoadConfig( const CORE::CDataNode& treeroot );

    protected:

    VfsResourceProvider m_vfsResourceProvider;
    CLogAdapter m_logAdapter;
    ImageCodecAdapter* m_imageCodecAdapter;
    XMLParserAdapter m_xmlParserAdapter;
    CORE::CString m_schemeToUse;
    CORE::CString m_defaultFont;
    CORE::CString m_defaultCursorImage;
    CORE::CString m_schemasResourceGroup;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUIDRIVERCEGUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_GUIDRIVERCEGUI_CCEGUIDRIVER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-08-2010 :
        - Dinand: Initial implementation

---------------------------------------------------------------------------*/
