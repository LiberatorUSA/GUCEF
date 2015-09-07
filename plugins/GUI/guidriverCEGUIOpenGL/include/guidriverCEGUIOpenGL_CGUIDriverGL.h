/*
 *  guidriverRocketOpenGL: module providing a GUI backend using Rocket and OpenGL
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

#ifndef GUCEF_GUIDRIVERCEGUIGL_CGUIDRIVERGL_H
#define GUCEF_GUIDRIVERCEGUIGL_CGUIDRIVERGL_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef __MYGUI_H__
#include "MyGUI.h"
#define __MYGUI_H__
#endif /* __MYGUI_H__ ? */

#ifndef __MYGUI_OPENGL_RENDER_MANAGER_H__
#include "MyGUI_OpenGLRenderManager.h"
#define __MYGUI_OPENGL_RENDER_MANAGER_H__
#endif /* __MYGUI_OPENGL_RENDER_MANAGER_H__ ? */

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_GUIDRIVERCEGUI_CCEGUIDRIVER_H
#include "guidriverCEGUI_CCEGuiDriver.h"
#define GUCEF_GUIDRIVERCEGUI_CCEGUIDRIVER_H
#endif /* GUCEF_GUIDRIVERCEGUI_CCEGUIDRIVER_H ? */

#ifndef GUCEF_MYGUI_CLOGADAPTER_H
#include "guidriverCEGUI_CLogAdapter.h"
#define GUCEF_MYGUI_CLOGADAPTER_H
#endif /* GUCEF_MYGUI_CLOGADAPTER_H ? */

#ifndef GUCEF_GUIDRIVERCEGUIGL_CIMAGELOADER_H
#include "guidriverCEGUIOpenGL_CImageLoader.h"
#define GUCEF_GUIDRIVERCEGUIGL_CIMAGELOADER_H
#endif /* GUCEF_GUIDRIVERCEGUIGL_CIMAGELOADER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUIDRIVERCEGUIGL {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_GUIDRIVERCEGUIGL_EXPORT_CPP CGUIDriverGL : public GUCEF::GUIDRIVERCEGUI::CCEGUIDriver
{
    public:

    CGUIDriverGL( void );

    virtual ~CGUIDriverGL();

    virtual GUI::TGuiContextPtr CreateGUIContext( GUI::TWindowContextPtr windowContext );

    virtual TGUIContextSet GetContextList( void );

    virtual GUI::UInt32 GetContextCount( void );

    virtual GUI::CString GetDriverName( void );

    virtual TStringSet GetAvailableFonts( void );

    virtual bool LoadFontFromAsset( const GUI::CString& assetPath );

    virtual const GUI::CString& GetClassTypeName( void ) const;

    virtual GUI::CString GetDriverProperty( const GUI::CString& propertyName ) const;

    /**
     *      Attempts to store the given tree in the file
     *      given according to the method of the codec metadata
     *
     *      @param tree the data tree you wish to store
     *      @return wheter storing the tree was successfull
     */
    virtual bool SaveConfig( CORE::CDataNode& tree );

    /**
     *      Attempts to load data from the given file to the
     *      root node given. The root data will be replaced
     *      and any children the node may already have will be deleted.
     *
     *      @param treeroot pointer to the node that is to act as root of the data tree
     *      @return whether building the tree from the given file was successfull.
     */
    virtual bool LoadConfig( const CORE::CDataNode& treeroot );

    virtual CEGUI::System* GetCEGui( void );

    private:

    CGUIDriverGL( const CGUIDriverGL& src );

    private:

    GUI::CGUIDriver::TGUIContextSet m_contextSet;
    TStringSet m_fontTypes;
    GUIDRIVERCEGUI::Gui m_myGUI;
    GUIDRIVERCEGUI::OpenGLRenderManager m_renderManager;
    CImageLoader m_imageLoader;
    GUIDRIVERCEGUI::CLogAdapter m_logAdapter;
    bool m_myGuiInitialized;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MYGUIGL */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_GUIDRIVERCEGUIGL_CGUIDRIVERGL_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-08-2010 :
        - Dinand: Initial implementation

---------------------------------------------------------------------------*/
