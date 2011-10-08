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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef __Exception_H_
#include "OgreException.h"
#define __Exception_H_
#endif /* __Exception_H_ ? */

#ifndef __ROOT__
#include "OgreRoot.h"
#define __ROOT__
#endif /* __ROOT__ ? */

#ifndef __MYGUI_XML_DOCUMENT_H__
#include "MyGUI_XmlDocument.h"
#define __MYGUI_XML_DOCUMENT_H__
#endif /* __MYGUI_XML_DOCUMENT_H__ */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_CLOGMANAGER_H
#include "CLogManager.h"
#define GUCEF_CORE_CLOGMANAGER_H
#endif /* GUCEF_CORE_CLOGMANAGER_H ? */

#ifndef GUCE_CORE_CIOACCESSARCHIVEFACTORY_H
#include "CIOAccessArchiveFactory.h"
#define GUCE_CORE_CIOACCESSARCHIVEFACTORY_H
#endif /* GUCE_CORE_CIOACCESSARCHIVEFACTORY_H ? */

#ifndef GUCE_MYGUIOGRE_CIOACCESSTOMYGUIDATASTREAMADAPTER_H
#include "guceMyGUIOgre_CIOAccessToMyGUIDataStreamAdapter.h"
#define GUCE_MYGUIOGRE_CIOACCESSTOMYGUIDATASTREAMADAPTER_H
#endif /* GUCE_MYGUIOGRE_CIOACCESSTOMYGUIDATASTREAMADAPTER_H ? */

#ifndef GUCE_MYGUIOGRE_CBUTTONIMP_H
#include "guceMyGUIOgre_CButtonImp.h"
#define GUCE_MYGUIOGRE_CBUTTONIMP_H
#endif /* GUCE_MYGUIOGRE_CBUTTONIMP_H ? */

#ifndef GUCE_MYGUIOGRE_CEDITBOXIMP_H
#include "guceMyGUIOgre_CEditboxImp.h"
#define GUCE_MYGUIOGRE_CEDITBOXIMP_H
#endif /* GUCE_MYGUIOGRE_CEDITBOXIMP_H ? */

#ifndef GUCE_MYGUIOGRE_CLISTBOXIMP_H
#include "guceMyGUIOgre_CListboxImp.h"
#define GUCE_MYGUIOGRE_CLISTBOXIMP_H
#endif /* GUCE_MYGUIOGRE_CLISTBOXIMP_H ? */

#ifndef GUCE_MYGUIOGRE_CWINDOWIMP_H
#include "guceMyGUIOgre_CWindowImp.h"
#define GUCE_MYGUIOGRE_CWINDOWIMP_H
#endif /* GUCE_MYGUIOGRE_CWINDOWIMP_H ? */

#ifndef GUCE_MYGUIOGRE_CIMAGEFRAMEIMP_H
#include "guceMyGUIOgre_CImageFrameImp.h"
#define GUCE_MYGUIOGRE_CIMAGEFRAMEIMP_H
#endif /* GUCE_MYGUIOGRE_CIMAGEFRAMEIMP_H ? */

#ifndef GUCE_MYGUIOGRE_CHECKBOXIMP_H
#include "guceMyGUIOgre_CCheckboxImp.h"
#define GUCE_MYGUIOGRE_CHECKBOXIMP_H
#endif /* GUCE_MYGUIOGRE_CHECKBOXIMP_H ? */

#ifndef GUCE_MYGUIOGRE_CCOMBOBOXIMP_H
#include "guceMyGUIOgre_CComboboxImp.h"
#define GUCE_MYGUIOGRE_CCOMBOBOXIMP_H
#endif /* GUCE_MYGUIOGRE_CCOMBOBOXIMP_H ? */

#ifndef GUCE_MYGUIOGRE_CTABCONTROLIMP_H
#include "guceMyGUIOgre_CTabControlImp.h"
#define GUCE_MYGUIOGRE_CTABCONTROLIMP_H
#endif /* GUCE_MYGUIOGRE_CTABCONTROLIMP_H ? */

#ifndef GUCE_MYGUIOGRE_CTABCONTENTPANEIMP_H
#include "guceMyGUIOgre_CTabContentPaneImp.h"
#define GUCE_MYGUIOGRE_CTABCONTENTPANEIMP_H
#endif /* GUCE_MYGUIOGRE_CTABCONTENTPANEIMP_H ? */

#ifndef GUCE_MYGUIOGRE_CGRIDVIEWIMP_H
#include "guceMyGUIOgre_CGridViewImp.h"
#define GUCE_MYGUIOGRE_CGRIDVIEWIMP_H
#endif /* GUCE_MYGUIOGRE_CGRIDVIEWIMP_H ? */

#ifndef GUCE_MYGUIOGRE_CLABELIMP_H
#include "guceMyGUIOgre_CLabelImp.h"
#define GUCE_MYGUIOGRE_CLABELIMP_H
#endif /* GUCE_MYGUIOGRE_CLABELIMP_H ? */

#ifndef GUCE_MYGUIOGRE_CSPINNERIMP_H
#include "guceMyGUIOgre_CSpinnerImp.h"
#define GUCE_MYGUIOGRE_CSPINNERIMP_H
#endif /* GUCE_MYGUIOGRE_CSPINNERIMP_H ? */

#ifndef GUCE_MYGUIOGRE_CRENDERCONTEXTIMP_H
#include "guceMyGUIOgre_CRenderContextImp.h"
#define GUCE_MYGUIOGRE_CRENDERCONTEXTIMP_H
#endif /* GUCE_MYGUIOGRE_CRENDERCONTEXTIMP_H ? */

#include "guceMyGUIOgre_CFormBackendImp.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCE {
namespace MYGUIOGRE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CFormBackendImp::CFormBackendImp( void )
    : CFormBackend()        ,
      m_widgetMap()         ,
      m_rootWindow( NULL )  ,
      m_widgetNamePrefix()  ,
      m_resourceGroupName() ,
      m_dummyArchive( NULL )
{GUCE_TRACE;

    m_resourceGroupName = "GUCE::MYGUIOGRE::CFormBackendImp::" + GUCEF::CORE::PointerToString( this );
    m_widgetNamePrefix = m_resourceGroupName + '/';    
    
    // provide hacky access to data we get in the CFormBackend API
    Ogre::Root::getSingletonPtr()->addResourceLocation( m_resourceGroupName      , 
                                                        "GUCEF::CORE::CIOAccess" ,
                                                        m_resourceGroupName      );
    m_dummyArchive = (*CORE::CIOAccessArchiveFactory::Instance()->GetArchiveMap().find( m_resourceGroupName )).second;
}

/*-------------------------------------------------------------------------*/
    
CFormBackendImp::~CFormBackendImp()
{GUCE_TRACE;

    Ogre::Root::getSingletonPtr()->removeResourceLocation( m_resourceGroupName, m_resourceGroupName );
    m_dummyArchive = NULL;
}

/*-------------------------------------------------------------------------*/

GUCEF::GUI::CWidget*
CFormBackendImp::CreateAndHookWrapperForWindow( MyGUI::Widget* widget )
{GUCE_TRACE;

    GUCEF::GUI::CWidget* widgetWrapper = NULL;
    CString typeName( CString( widget->getTypeName().c_str() ).SubstrToChar( '/', false ) );

    if ( "Widget" == typeName )
    {
        widgetWrapper = new TBasicWidgetImp();
        static_cast< TBasicWidgetImp* >( widgetWrapper )->Hook( widget );
    }
    else    
    if ( "Button" == typeName )
    {
        widgetWrapper = new CButtonImp();
        static_cast< CButtonImp* >( widgetWrapper )->Hook( static_cast< MyGUI::Button* >( widget ) );
    }
    //else
    //if ( "Editbox" == typeName )
    //{
    //    widgetWrapper = new CEditboxImp();
    //    static_cast< CEditboxImp* >( widgetWrapper )->Hook( static_cast< CEGUI::Editbox* >( widget ) );        
    //}
    //else
    //if ( "Listbox" == typeName )
    //{
    //    widgetWrapper = new CListboxImp();
    //    static_cast< CListboxImp* >( widgetWrapper )->Hook( static_cast< CEGUI::Listbox* >( widget ) );        
    //}
    //else
    //if ( "StaticImage" == typeName )
    //{
    //    CString widgetName( CString( window->getName().c_str() ).SubstrToChar( '/', false ) );
    //    
    //    if ( widgetName == "RenderContext" )
    //    {
    //        widgetWrapper = new CRenderContextImp();
    //        static_cast< CRenderContextImp* >( widgetWrapper )->Hook( window );
    //    }
    //    else
    //    {
    //        widgetWrapper = new CImageFrameImp();
    //        static_cast< CImageFrameImp* >( widgetWrapper )->Hook( window );
    //    }
    //}
    //else    
    //if ( "StaticText" == typeName )
    //{
    //    widgetWrapper = new CLabelImp();
    //    static_cast< CLabelImp* >( widgetWrapper )->Hook( window );
    //}    
    //else    
    //if ( "Checkbox" == typeName )
    //{
    //    widgetWrapper = new CCheckboxImp();
    //    static_cast< CCheckboxImp* >( widgetWrapper )->Hook( static_cast< CEGUI::Checkbox* >( window ) );        
    //}
    //else    
    //if ( "Combobox" == typeName )
    //{
    //    widgetWrapper = new CComboboxImp();
    //    static_cast< CComboboxImp* >( widgetWrapper )->Hook( static_cast< CEGUI::Combobox* >( window ) );        
    //}
    //if ( "TabControl" == typeName )
    //{
    //    widgetWrapper = new CTabControlImp();
    //    static_cast< CTabControlImp* >( widgetWrapper )->Hook( static_cast< CEGUI::TabControl* >( window ) );        
    //}        
    //else
    //if ( "TabContentPane" == typeName )
    //{
    //    widgetWrapper = new CTabContentPaneImp();
    //    static_cast< CTabContentPaneImp* >( widgetWrapper )->Hook( window );        
    //}        
    else        
    if ( "Window" == typeName )
    {
        widgetWrapper = new CWindowImp();
        static_cast< CWindowImp* >( widgetWrapper )->Hook( static_cast< MyGUI::Window* >( widget ) );        
    }
    //else        
    //if ( "MultiColumnList" == typeName )
    //{
    //    widgetWrapper = new CGridViewImp();
    //    static_cast< CGridViewImp* >( widgetWrapper )->Hook( static_cast< CEGUI::MultiColumnList* >( window ) );        
    //}
    //else        
    //if ( "Spinner" == typeName )
    //{
    //    widgetWrapper = new CSpinnerImp();
    //    static_cast< CSpinnerImp* >( widgetWrapper )->Hook( static_cast< CEGUI::Spinner* >( window ) );        
    //}        
    //else
    //if ( "DefaultWindow" == typeName )
    //{
    //    widgetWrapper = new TBasicWidgetImp();
    //    static_cast< TBasicWidgetImp* >( widgetWrapper )->Hook( window );
    //}
    else
    {
        GUCEF_ERROR_LOG( GUCEF::CORE::LOGLEVEL_IMPORTANT, "CFormBackendImp(" + GUCEF::CORE::PointerToString( this ) + "): Unable match widget type to wrapper for widget type: " + typeName );
    }    
    
    if ( NULL != widgetWrapper )
    {
        GUCEF_DEBUG_LOG( GUCEF::CORE::LOGLEVEL_NORMAL, "CFormBackendImp(" + GUCEF::CORE::PointerToString( this ) + "): Wrapped and hooked GUI widget of type " + widgetWrapper->GetType() + " for MyGUI widget of type " + typeName );
    }
    return widgetWrapper;
}

/*-------------------------------------------------------------------------*/

void
CFormBackendImp::WrapAndHookChildWindows( MyGUI::Widget* widget )
{GUCE_TRACE;

    GUCEF::GUI::CWidget* wrappedWidget = NULL;
    MyGUI::Widget* childWidget = NULL;
    
    MyGUI::EnumeratorWidgetPtr childWidgets = widget->getEnumerator();
    while ( childWidgets.next() )
    {
        childWidget = childWidgets.current();
        wrappedWidget = CreateAndHookWrapperForWindow( childWidget );
        if ( NULL != widget )
        {
            CString localWidgetName = wrappedWidget->GetName().SubstrToChar( '/', false );
            m_widgetMap[ localWidgetName ] = wrappedWidget;
        }
        
        WrapAndHookChildWindows( childWidget );                     
    }
}

/*-------------------------------------------------------------------------*/

bool
CFormBackendImp::LoadLayout( const GUCEF::CORE::CString& layoutStoragePath )
{GUCE_TRACE;

    MyGUI::VectorWidgetPtr rootWidgets;
    MyGUI::LayoutManager* lmgr = MyGUI::LayoutManager::getInstancePtr();
    
    GUCEF_DEBUG_LOG( 0, "CFormBackendImp(" + GUCEF::CORE::PointerToString( this ) + "): Starting layout load for a GUI Form" );
    
    try
    {
        rootWidgets = lmgr->loadLayout( layoutStoragePath.STL_String()  , 
                                        m_widgetNamePrefix.STL_String() , 
                                        nullptr                         );
    }
    catch ( Ogre::Exception& e )
    {
        GUCEF_ERROR_LOG( GUCEF::CORE::LOGLEVEL_IMPORTANT, CString( "CFormBackendImp(" + GUCEF::CORE::PointerToString( this ) + "): Ogre Exception while attempting to load form layout: " ) + e.getFullDescription().c_str() );
        return false;
    }
    
    // Now that we completed loading lets see what we got from MyGUI
    m_rootWindow = NULL;    
    MyGUI::VectorWidgetPtr::iterator i = rootWidgets.begin();
    while ( i != rootWidgets.end() )
    {
        // Begin by providing a wrapper for the root window
        MyGUI::Widget* rootWidget = (*i);
        GUCEF::GUI::CWidget* wrappedRootWidget = CreateAndHookWrapperForWindow( rootWidget );
        if ( NULL != wrappedRootWidget )
        {
            CString localWidgetName = wrappedRootWidget->GetName().SubstrToChar( '/', false );
            m_widgetMap[ localWidgetName ] = wrappedRootWidget;
            if ( NULL == m_rootWindow )
            {
                m_rootWindow = wrappedRootWidget;
            }
            WrapAndHookChildWindows( rootWidget );
        }
        rootWidget->setVisible( false );
        ++i;        
    }
    
    GUCEF_DEBUG_LOG( 0, "CFormBackendImp(" + GUCEF::CORE::PointerToString( this ) + "): Successfully loaded a GUI Form layout" );
    return true;        
}

/*-------------------------------------------------------------------------*/
    
bool
CFormBackendImp::SaveLayout( const GUCEF::CORE::CString& layoutStoragePath )
{GUCE_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CFormBackendImp::LoadLayout( GUCEF::CORE::CIOAccess& layoutStorage )
{GUCE_TRACE;

    return false; 
}

/*-------------------------------------------------------------------------*/
    
bool
CFormBackendImp::SaveLayout( GUCEF::CORE::CIOAccess& layoutStorage )
{GUCE_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/
    
GUCEF::GUI::CWidget*
CFormBackendImp::GetRootWidget( void )
{GUCE_TRACE;

    return m_rootWindow;
}

/*-------------------------------------------------------------------------*/
    
const GUCEF::GUI::CWidget*
CFormBackendImp::GetRootWidget( void ) const
{GUCE_TRACE;

    return m_rootWindow;
}
    
/*-------------------------------------------------------------------------*/

GUCEF::GUI::CWidget*
CFormBackendImp::GetWidget( const CString& widgetName )
{GUCE_TRACE;

    TWidgetMap::iterator i = m_widgetMap.find( widgetName );
    if ( i != m_widgetMap.end() )
    {
        return (*i).second;
    }
    
    return NULL;
}

/*-------------------------------------------------------------------------*/
    
void
CFormBackendImp::GetWidgetVector( const CString& widgetName   ,
                                  TWidgetVector& widgetVector )
{GUCE_TRACE;

    TWidgetMap::iterator i = m_widgetMap.begin();
    while ( i != m_widgetMap.end() )
    {
        widgetVector.push_back( (*i).second );
        ++i;
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MYGUIOGRE */
}; /* namespace GUCE */

/*-------------------------------------------------------------------------*/
