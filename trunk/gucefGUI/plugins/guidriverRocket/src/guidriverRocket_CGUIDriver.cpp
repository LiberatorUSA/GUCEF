/*
 *  guceCEGUIOgre: glue module for the CEGUI+Ogre GUI backend
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "MyGUI.h"
#include "MyGUI.OgrePlatform.h"

#ifndef GUCEF_CORE_CLOGMANAGER_H
#include "CLogManager.h"
#define GUCEF_CORE_CLOGMANAGER_H
#endif /* GUCEF_CORE_CLOGMANAGER_H ? */

#ifndef GUCE_CORE_CGUCEAPPLICATION_H
#include "CGUCEApplication.h"
#define GUCE_CORE_CGUCEAPPLICATION_H
#endif /* GUCE_CORE_CGUCEAPPLICATION_H ? */

#ifndef GUCEF_GUI_CGUIMANAGER_H
#include "gucefGUI_CGUIManager.h"
#define GUCEF_GUI_CGUIMANAGER_H
#endif /* GUCEF_GUI_CGUIMANAGER_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCE_GUI_CGUIMANAGER_H
#include "CGUIManager.h"
#define GUCE_GUI_CGUIMANAGER_H
#endif /* GUCE_GUI_CGUIMANAGER_H ? */

#ifndef GUCE_CORE_CWINDOWMANAGER_H
#include "CWindowManager.h"
#define GUCE_CORE_CWINDOWMANAGER_H
#endif /* GUCE_CORE_CWINDOWMANAGER_H ? */

#ifndef GUCE_MYGUIOGRE_CFORMBACKENDIMP_H
#include "guceMyGUIOgre_CFormBackendImp.h"
#define GUCE_MYGUIOGRE_CFORMBACKENDIMP_H
#endif /* GUCE_MYGUIOGRE_CFORMBACKENDIMP_H ? */

#ifndef GUCE_MYGUIOGRE_CMYGUIINPUTADAPTER_H
#include "guceMyGUIOgre_CMyGUIInputAdapter.h"
#define GUCE_MYGUIOGRE_CMYGUIINPUTADAPTER_H
#endif /* GUCE_MYGUIOGRE_CMYGUIINPUTADAPTER_H ? */

#ifndef GUCE_MYGUIOGRE_CGUICONTEXT_H
#include "guceMyGUIOgre_CGUIContext.h"
#define GUCE_MYGUIOGRE_CGUICONTEXT_H
#endif /* GUCE_MYGUIOGRE_CGUICONTEXT_H ? */

#ifndef GUCE_MYGUIOGRE_CBUTTONIMP_H
#include "guceMyGUIOgre_CButtonImp.h"
#define GUCE_MYGUIOGRE_CBUTTONIMP_H
#endif /* GUCE_MYGUIOGRE_CBUTTONIMP_H ? */

#ifndef GUCE_MYGUIOGRE_CWINDOWIMP_H
#include "guceMyGUIOgre_CWindowImp.h"
#define GUCE_MYGUIOGRE_CWINDOWIMP_H
#endif /* GUCE_MYGUIOGRE_CWINDOWIMP_H ? */

#include "guceMyGUIOgre_CGUIDriver.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCE {
namespace MYGUIOGRE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

typedef GUCEF::CORE::CTFactory< GUCEF::GUI::CFormBackend, CFormBackendImp > TFormBackendFactory;

typedef GUCEF::CORE::CTFactory< GUCEF::GUI::CWidget, TBasicWidgetImp >    TWidgetFactory;
typedef GUCEF::CORE::CTFactory< GUCEF::GUI::CWidget, CWindowImp >         TWindowFactory;
typedef GUCEF::CORE::CTFactory< GUCEF::GUI::CWidget, CButtonImp >         TButtonFactory;
//typedef GUCEF::CORE::CTFactory< GUCEF::GUI::CWidget, CPushButtonImp >     TPushButtonFactory;
//typedef GUCEF::CORE::CTFactory< GUCEF::GUI::CWidget, CEditboxImp >        TEditboxFactory;
//typedef GUCEF::CORE::CTFactory< GUCEF::GUI::CWidget, CListboxImp >        TListboxFactory;
//typedef GUCEF::CORE::CTFactory< GUCEF::GUI::CWidget, CImageFrameImp >     TImageFrameFactory;
//typedef GUCEF::CORE::CTFactory< GUCEF::GUI::CWidget, CCheckboxImp >       TCheckboxFactory;
//typedef GUCEF::CORE::CTFactory< GUCEF::GUI::CWidget, CLabelImp >          TLabelFactory;
//typedef GUCEF::CORE::CTFactory< GUCEF::GUI::CWidget, CComboboxImp >       TComboboxFactory;
//typedef GUCEF::CORE::CTFactory< GUCEF::GUI::CWidget, CTabControlImp >     TTabControlFactory;
//typedef GUCEF::CORE::CTFactory< GUCEF::GUI::CWidget, CTabContentPaneImp > TTabContentPaneFactory;
//typedef GUCEF::CORE::CTFactory< GUCEF::GUI::CWidget, CGridViewImp >       TGridViewFactory;
//typedef GUCEF::CORE::CTFactory< GUCEF::GUI::CWidget, CSpinnerImp >        TSpinnerFactory;
//typedef GUCEF::CORE::CTFactory< GUCEF::GUI::CWidget, CRenderContextImp >  TRenderContextFactory;
//
//typedef GUCEF::CORE::CTFactory< GUCEF::GUI::CForm, CFileOpenDialogImp >   TFileOpenDialogFactory;
//typedef GUCEF::CORE::CTFactory< GUCEF::GUI::CForm, CFileSaveDialogImp >   TFileSaveDialogFactory;

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

static TWidgetFactory widgetFactory;
static TWindowFactory windowFactory;
static TButtonFactory buttonFactory;

CGUIDriver* CGUIDriver::g_instance = NULL;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CGUIDriver::CGUIDriver( void )
    : CIGUIDriver()                ,
      m_initialized( false )       ,
      m_window( NULL )             ,
      m_guiSystem( NULL )          ,
      m_myguiRenderManager( NULL ) ,
      m_inputAdapter( NULL )       ,
      m_guiConfig()                ,
      m_formFactory()              ,
      m_widgetFactory()            ,
      m_contextList()              ,
      m_guiSystemConfigPath()      ,
      m_myguiDataManager()
{GUCE_TRACE;

    SetDriverResourceGroup( GetDriverName() );
    m_widgetFactory.RegisterConcreteFactory( "Widget", &widgetFactory );
    m_widgetFactory.RegisterConcreteFactory( "Window", &windowFactory );
    m_widgetFactory.RegisterConcreteFactory( "Button", &buttonFactory );
    //widgetFactory.RegisterConcreteFactory( "PushButton", new TPushButtonFactory() );
    //widgetFactory.RegisterConcreteFactory( "Editbox", new TEditboxFactory() );
    //widgetFactory.RegisterConcreteFactory( "Listbox", new TListboxFactory() );
    //widgetFactory.RegisterConcreteFactory( "ImageFrame", new TImageFrameFactory() );
    //widgetFactory.RegisterConcreteFactory( "Checkbox", new TCheckboxFactory() );
    //widgetFactory.RegisterConcreteFactory( "Label", new TLabelFactory() );
    //widgetFactory.RegisterConcreteFactory( "Combobox", new TComboboxFactory() );
    //widgetFactory.RegisterConcreteFactory( "TabControl", new TTabControlFactory() );
    //widgetFactory.RegisterConcreteFactory( "TabContentPane", new TTabContentPaneFactory() );
    //widgetFactory.RegisterConcreteFactory( "GridView", new TGridViewFactory() );
    //widgetFactory.RegisterConcreteFactory( "Spinner", new TSpinnerFactory() );
    //widgetFactory.RegisterConcreteFactory( "RenderContext", new TRenderContextFactory() );
    //
    //formFactory.RegisterConcreteFactory( "FileOpenDialog", new TFileOpenDialogFactory );
    //formFactory.RegisterConcreteFactory( "FileSaveDialog", new TFileSaveDialogFactory );
}

/*-------------------------------------------------------------------------*/

CGUIDriver::~CGUIDriver()
{GUCE_TRACE;

    m_widgetFactory.UnregisterConcreteFactory( "Widget" );
    m_widgetFactory.UnregisterConcreteFactory( "Window" );
    m_widgetFactory.UnregisterConcreteFactory( "Button" );
    //widgetFactory.UnregisterConcreteFactory( "PushButton" );
    //widgetFactory.UnregisterConcreteFactory( "Editbox" );
    //widgetFactory.UnregisterConcreteFactory( "Listbox" );
    //widgetFactory.UnregisterConcreteFactory( "ImageFrame" );
    //widgetFactory.UnregisterConcreteFactory( "Checkbox" );
    //widgetFactory.UnregisterConcreteFactory( "Label" );
    //widgetFactory.UnregisterConcreteFactory( "Combobox" );
    //widgetFactory.UnregisterConcreteFactory( "TabControl" );
    //widgetFactory.UnregisterConcreteFactory( "TabContentPane" );
    //widgetFactory.UnregisterConcreteFactory( "GridView" );
    //widgetFactory.UnregisterConcreteFactory( "Spinner" );
    //widgetFactory.UnregisterConcreteFactory( "RenderContext" );

    //formFactory.UnregisterConcreteFactory( "FileOpenDialog" );
    //formFactory.UnregisterConcreteFactory( "FileSaveDialog" );
}
/*-------------------------------------------------------------------------*/

CGUIDriver* 
CGUIDriver::Instance( void )
{GUCE_TRACE;

    if ( NULL == g_instance )
    {
        g_instance = new CGUIDriver();
    }
    return g_instance;
}

/*-------------------------------------------------------------------------*/

void
CGUIDriver::Deinstance( void )
{GUCE_TRACE;

    delete g_instance;
    g_instance = NULL;
}

/*-------------------------------------------------------------------------*/

bool
CGUIDriver::Shutdown( void )
{GUCE_TRACE;

    if ( m_initialized )
    {   
        GUCEF_SYSTEM_LOG( GUCEF::CORE::LOGLEVEL_NORMAL, "CGUIDriver: Shutting down MyGUI system" );
    }
        
    delete m_inputAdapter;
    m_inputAdapter = NULL;
        
    if ( NULL != m_guiSystem )
    {
        m_guiSystem->shutdown();
    }
    delete m_guiSystem;
    m_guiSystem = NULL;
        
    if ( NULL != m_myguiRenderManager )
    {
        m_myguiRenderManager->shutdown();
    }
    delete m_myguiRenderManager;
    m_myguiRenderManager = NULL;
       
    m_window = NULL;        
            
    if ( m_initialized )
    {
        GUCEF_SYSTEM_LOG( GUCEF::CORE::LOGLEVEL_NORMAL, "CGUIDriver: Shutdown of MyGUI system complete" );
    }
    
    m_initialized = false;
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CGUIDriver::Initialize( CORE::TWindowContextPtr windowContext )
{GUCE_TRACE;

    if ( !m_initialized )
    {
        GUCEF_SYSTEM_LOG( GUCEF::CORE::LOGLEVEL_NORMAL, "CGUIDriver: Initializing GUI system" );
        
        try
        {
            assert( windowContext != NULL );
            m_window = windowContext->GetOgreWindowPtr();                
            assert( m_window != NULL );

            MyGUI::LogManager::registerSection( MYGUI_PLATFORM_LOG_SECTION, MYGUI_PLATFORM_LOG_FILENAME );

            #ifdef GUCE_CORE_DEBUG_MODE
            MyGUI::LogManager::setSTDOutputEnabled( true );
            #endif

            // Set the path used during the initialization phase as the root for all files
            CDataManager::getInstance().SetGuiDataRoot( m_guiSystemConfigPath );

            m_myguiRenderManager = new MyGUI::OgreRenderManager();
            m_myguiRenderManager->initialise( m_window, CORE::CGUCEApplication::Instance()->GetSceneManager() );                       
            
            m_guiSystem = new MyGUI::Gui();
            m_guiSystem->initialise( "core.xml" );
            
            /* feed MyGUI with input events */
            m_inputAdapter = new CMyGUIInputAdapter( m_guiSystem );

            // Clear the path after initialization and use normal full paths
            // from this point onward
            CDataManager::getInstance().SetGuiDataRoot( CString() );

        }
        catch ( Ogre::Exception& e )
        {
            GUCEF_ERROR_LOG( GUCEF::CORE::LOGLEVEL_CRITICAL, "CGUIDriver: Exception while initializing the GUI system: " + e.getFullDescription() );
            Shutdown();
            return false;
        }    
        m_initialized = true;

        // Apply the loaded configuration (if any exists)
        if ( LoadConfig( m_guiConfig ) )
        {
            GUCEF_SYSTEM_LOG( GUCEF::CORE::LOGLEVEL_NORMAL, "CGUIDriver: Successfully initialized the GUI system" );
        }
        else
        {
            GUCEF_SYSTEM_LOG( GUCEF::CORE::LOGLEVEL_NORMAL, "CGUIDriver: Failed to initialize the GUI system" );
            Shutdown();
            return false;
        }
    }
    
    if ( NULL == windowContext->GetGuiContext() )
    {
        windowContext->SetGuiContext( CreateGUIContext() );
    }
    
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CGUIDriver::LoadConfig( const GUCEF::CORE::CDataNode& rootNode )       
{GUCE_TRACE;             

    const GUCEF::CORE::CDataNode* ourRoot = rootNode.FindChild( GetDriverName() );
    if ( NULL != ourRoot )
    {
        const GUCEF::CORE::CDataNode* node = ourRoot->FindChild( "ResourceGroup" );
        if ( NULL != node )
        {
            SetDriverResourceGroup( node->GetAttributeValue( "Name" ) );
        }
        node = ourRoot->FindChild( "GuiSystemConfig" );
        if ( NULL != node )
        {
            m_guiSystemConfigPath = node->GetAttributeValue( "Path" );
        }
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CGUIDriver::SaveConfig( GUCEF::CORE::CDataNode& tree )
{GUCE_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

CString
CGUIDriver::GetDriverProperty( const CString& propertyName ) const
{GUCE_TRACE;

    if ( propertyName == HasGuceInterfaceDriverProperty )
    {
        return "true";
    }
    return CString();
}

/*-------------------------------------------------------------------------*/

GUCEF::GUI::TGuiContextPtr
CGUIDriver::CreateGUIContext()
{GUCE_TRACE;

    GUCEF::GUI::TGuiContextPtr context( new CGUIContext( *this ) );
    m_contextList.insert( context );
    return context;
}

/*-------------------------------------------------------------------------*/
    
void
CGUIDriver::DestroyGUIContext( GUCEF::GUI::CIGUIContext* context )
{GUCE_TRACE;
    
    //m_contextList.erase( context );
    //delete context;
}

/*-------------------------------------------------------------------------*/

CGUIDriver::TGUIContextSet
CGUIDriver::GetContextList( void )
{GUCE_TRACE;

    return m_contextList;
}

/*-------------------------------------------------------------------------*/
    
UInt32
CGUIDriver::GetContextCount( void )
{GUCE_TRACE;

    return (UInt32) m_contextList.size();
}

/*-------------------------------------------------------------------------*/
    
CString
CGUIDriver::GetDriverName( void )
{GUCE_TRACE;

    return "guceMyGUIOgre";
}

/*-------------------------------------------------------------------------*/

GUCEF::GUI::CWidget*
CGUIDriver::CreateWidget( const CString& widgetName )
{GUCE_TRACE;

    return m_widgetFactory.Create( widgetName );
}
    
/*-------------------------------------------------------------------------*/
    
void
CGUIDriver::DestroyWidget( GUCEF::GUI::CWidget* widget )
{GUCE_TRACE;

    m_widgetFactory.Destroy( widget );
}
    
/*-------------------------------------------------------------------------*/
    
GUCEF::GUI::CForm*
CGUIDriver::CreateForm( const CString& formName )
{GUCE_TRACE;

    return m_formFactory.Create( formName );
}
    
/*-------------------------------------------------------------------------*/
    
void
CGUIDriver::DestroyForm( GUCEF::GUI::CForm* form )
{GUCE_TRACE;

    return m_formFactory.Destroy( form );
}
    
/*-------------------------------------------------------------------------*/

CGUIDriver::TStringSet
CGUIDriver::GetAvailableFormTypes( void )
{GUCE_TRACE;

    // Get all the generic forms
    TStringSet formTypes = GUCEF::GUI::CGUIManager::Instance()->GetGenericFormTypes();
    
    // Add/overwrite with the driver specific forms
    m_formFactory.ObtainKeySet( formTypes );
    
    return formTypes;
}
    
/*-------------------------------------------------------------------------*/
    
CGUIDriver::TStringSet
CGUIDriver::GetAvailableWidgetTypes( void )
{GUCE_TRACE;

    // Get all the generic forms
//    TStringSet widgetTypes = GUCEF::GUI::CGUIManager::Instance()->GetGenericWidgetTypes();
    TStringSet widgetTypes;
    
    // Add/overwrite with the driver specific forms
    m_widgetFactory.ObtainKeySet( widgetTypes );
    return widgetTypes;
}
    
/*-------------------------------------------------------------------------*/
    
GUCEF::GUI::CFormBackend*
CGUIDriver::CreateFormBackend( void )
{GUCE_TRACE;

    return new CFormBackendImp();
}
    
/*-------------------------------------------------------------------------*/
    
void
CGUIDriver::DestroyFormBackend( GUCEF::GUI::CFormBackend* formBackend )
{GUCE_TRACE;

    delete formBackend;
}

/*-------------------------------------------------------------------------*/

const CString&
CGUIDriver::GetClassTypeName( void ) const
{GUCE_TRACE;

    static CString classTypeName = "GUCE::MYGUIOGRE::CGUIDriver";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

void
CGUIDriver::SetDriverResourceGroup( const CString& resourceGroup )
{GUCE_TRACE;

    CDataManager::getInstance().SetResourceGroup( resourceGroup );
}

/*-------------------------------------------------------------------------*/
    
const CString&
CGUIDriver::GetDriverResourceGroup( void ) const
{GUCE_TRACE;

    return CDataManager::getInstance().GetResourceGroup();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace MYGUIOGRE */
} /* namespace GUCE */

/*-------------------------------------------------------------------------*/
