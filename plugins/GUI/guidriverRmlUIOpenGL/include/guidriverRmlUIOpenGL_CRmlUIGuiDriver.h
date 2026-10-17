/*
 *  guidriverRmlUIOpenGL: module providing a GUI backend using RmlUi and OpenGL
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

#ifndef GUCEF_GUIDRIVERRMLUIGL_CRMLUIGUIDRIVER_H
#define GUCEF_GUIDRIVERRMLUIGL_CRMLUIGUIDRIVER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_GUI_CGUIDRIVER_H
#include "gucefGUI_CGUIDriver.h"
#define GUCEF_GUI_CGUIDRIVER_H
#endif /* GUCEF_GUI_CGUIDRIVER_H ? */

#ifndef GUCEF_GUIDRIVERRMLUI_CRMLUISYSTEMINTERFACE_H
#include "guidriverRmlUI_CRmlUISystemInterface.h"
#define GUCEF_GUIDRIVERRMLUI_CRMLUISYSTEMINTERFACE_H
#endif /* GUCEF_GUIDRIVERRMLUI_CRMLUISYSTEMINTERFACE_H ? */

#ifndef GUCEF_GUIDRIVERRMLUI_CRMLUIVFSINTERFACE_H
#include "guidriverRmlUI_CRmlUIVFSInterface.h"
#define GUCEF_GUIDRIVERRMLUI_CRMLUIVFSINTERFACE_H
#endif /* GUCEF_GUIDRIVERRMLUI_CRMLUIVFSINTERFACE_H ? */

#ifndef GUCEF_GUIDRIVERRMLUIGL_CRMLUIRENDERINTERFACEOPENGL_H
#include "guidriverRmlUIOpenGL_CRmlUIRenderInterfaceOpenGL.h"
#define GUCEF_GUIDRIVERRMLUIGL_CRMLUIRENDERINTERFACEOPENGL_H
#endif /* GUCEF_GUIDRIVERRMLUIGL_CRMLUIRENDERINTERFACEOPENGL_H ? */

#ifndef GUCEF_GUIDRIVERRMLUIGL_MACROS_H
#include "guidriverRmlUIOpenGL_macros.h"
#define GUCEF_GUIDRIVERRMLUIGL_MACROS_H
#endif /* GUCEF_GUIDRIVERRMLUIGL_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUIDRIVERRMLUIGL {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUIDRIVERRMLUIGL_PUBLIC_CPP CRmlUIGuiDriver : public GUI::CGUIDriver
{    
    public:

    CRmlUIGuiDriver( void );

    virtual ~CRmlUIGuiDriver();
    
    virtual GUI::TGuiContextPtr CreateGUIContext( GUI::TWindowContextPtr windowContext );
    
    virtual TGUIContextSet GetContextList( void );
    
    virtual GUI::UInt32 GetContextCount( void );
    
    virtual GUI::CString GetDriverName( void );
    
    virtual TStringSet GetAvailableFormTypes( void );
    
    virtual TStringSet GetAvailableWidgetTypes( void );

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
    private:
    
    CRmlUIGuiDriver( const CRmlUIGuiDriver& src );

    private:

    CRmlUIRenderInterfaceOpenGL m_renderer;
    guidriverRmlUI::CRmlUISystemInterface m_systemInterface;
    guidriverRmlUI::CRmlUIVFSInterface m_fileInterface;
    bool m_isRmlUIInitialized;
    GUI::CGUIDriver::TGUIContextSet m_contextSet;
    TStringSet m_fontTypes;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUIDRIVERRMLUIGL */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_GUIDRIVERRMLUIGL_CRMLUIGUIDRIVER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-08-2010 :
        - Dinand: Initial implementation

---------------------------------------------------------------------------*/