/*
 *  guceMyGUIOgre: glue module for the MyGUI+Ogre GUI backend
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

#ifndef GUCE_MYGUIOGRE_CGUIDRIVER_H
#define GUCE_MYGUIOGRE_CGUIDRIVER_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_GUI_CWIDGETFACTORY_H
#include "gucefGUI_CWidgetFactory.h"
#define GUCEF_GUI_CWIDGETFACTORY_H
#endif /* GUCEF_GUI_CWIDGETFACTORY_H ? */

#ifndef GUCEF_GUI_CFORMFACTORY_H
#include "gucefGUI_CFormFactory.h"
#define GUCEF_GUI_CFORMFACTORY_H
#endif /* GUCEF_GUI_CFORMFACTORY_H ? */

#ifndef GUCE_GUI_CIGUIDRIVER_H
#include "guceGUI_CIGUIDriver.h"
#define GUCE_GUI_CIGUIDRIVER_H
#endif /* GUCE_GUI_CIGUIDRIVER_H ? */

#ifndef GUCE_MYGUIOGRE_CDATAMANAGER_H
#include "guceMyGUIOgre_CDataManager.h"
#define GUCE_MYGUIOGRE_CDATAMANAGER_H
#endif /* GUCE_MYGUIOGRE_CDATAMANAGER_H ? */

#ifndef GUCE_MYGUIOGRE_MACROS_H
#include "guceMyGUIOgre_macros.h"     /* often used guceMYGUIOGRE macros */
#define GUCE_MYGUIOGRE_MACROS_H
#endif /* GUCE_MYGUIOGRE_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Forward declarations of classes used here 
 */
namespace Ogre { class RenderWindow; class RenderTexture; class Root; }
namespace MyGUI { class Gui; class OgrePlatform; class OgreRenderManager; }
namespace GUCEF { namespace CORE { class CDataNode; } }

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCE {
namespace MYGUIOGRE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CModule;
class CMyGUIInputAdapter;

/*-------------------------------------------------------------------------*/

/**
 *  Implementation of the GUI driver for the CEGUI&Ogre combo
 */
class GUCE_MYGUIOGRE_EXPORT_CPP CGUIDriver : public GUCE::GUI::CIGUIDriver
{
    public:    
    
    static CGUIDriver* Instance( void );
    
    virtual bool Initialize( CORE::TWindowContextPtr windowContext );
    
    virtual bool Shutdown( void );
    
    virtual GUCEF::GUI::TGuiContextPtr CreateGUIContext();
    
    virtual TGUIContextSet GetContextList( void );
    
    virtual UInt32 GetContextCount( void );
    
    virtual CString GetDriverName( void );
    
    virtual TStringSet GetAvailableFormTypes( void );
    
    virtual TStringSet GetAvailableWidgetTypes( void );
    
    virtual bool LoadConfig( const GUCEF::CORE::CDataNode& rootNode );
    
    virtual bool SaveConfig( GUCEF::CORE::CDataNode& tree );

    GUCEF::GUI::CWidget* CreateWidget( const CString& widgetName );
    
    void DestroyWidget( GUCEF::GUI::CWidget* widget );
    
    GUCEF::GUI::CForm* CreateForm( const CString& formName );
    
    void DestroyForm( GUCEF::GUI::CForm* form );
    
    GUCEF::GUI::CFormBackend* CreateFormBackend( void );
    
    void DestroyFormBackend( GUCEF::GUI::CFormBackend* formBackend );
    
    virtual const CString& GetClassTypeName( void ) const;
    
    virtual CString GetDriverProperty( const CString& propertyName ) const;

    virtual void SetDriverResourceGroup( const CString& resourceGroup );
    
    virtual const CString& GetDriverResourceGroup( void ) const;
    
    private:
    friend class CModule;
    
    static void Deinstance( void );
    
    private:
    
    CGUIDriver( void );
    CGUIDriver( const CGUIDriver& src );        
    virtual ~CGUIDriver();    
    CGUIDriver& operator=( const CGUIDriver& src );
    
    void DestroyGUIContext( GUCEF::GUI::CIGUIContext* context );
    
    private:
    
    static CGUIDriver* g_instance;
    
    bool m_initialized;                                   /**< flag for manager initialization */    
    Ogre::RenderWindow* m_window;                         /**< window displaying our GUI */
    MyGUI::Gui* m_guiSystem;                              /**< main GUI system class */
    MyGUI::OgreRenderManager* m_myguiRenderManager;       /**< Ogre rendering backend for the GUI system */
    CMyGUIInputAdapter* m_inputAdapter;                   /**< binding between the input system and MyGUI */
    GUCEF::CORE::CDataNode m_guiConfig;    
    GUCEF::GUI::CFormFactory m_formFactory;
    GUCEF::GUI::CWidgetFactory m_widgetFactory;
    TGUIContextSet m_contextList;
    CString m_guiSystemConfigPath;
    CDataManager m_myguiDataManager;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace MYGUIOGRE */
} /* namespace GUCE */

/*-------------------------------------------------------------------------*/

#endif /* GUCE_MYGUIOGRE_CGUIDRIVER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 08-04-2007 :
        - Initial implementation

---------------------------------------------------------------------------*/
