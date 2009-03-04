/*
 *  gucefGUI: GUCEF module providing a uniform interface towards GUI backends
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#include "gucefGUI_CGUIManager.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUI {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

CGUIManager* CGUIManager::g_instance = NULL;

const CORE::CEvent CGUIManager::FormFactoryRegisteredEvent = "GUCEF::GUI::CGUIManager::FormFactoryRegisteredEvent";
const CORE::CEvent CGUIManager::FormFactoryUnregisteredEvent = "GUCEF::GUI::CGUIManager::FormFactoryUnregisteredEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CGUIManager*
CGUIManager::Instance( void )
{GUCEF_TRACE;

    if ( g_instance == NULL )
    {
        g_instance = new CGUIManager();
    }
    return g_instance;
}

/*-------------------------------------------------------------------------*/

void
CGUIManager::Deinstance( void )
{GUCEF_TRACE;

    delete g_instance;
    g_instance = NULL; 
}

/*-------------------------------------------------------------------------*/

void
CGUIManager::RegisterEvents( void )
{GUCEF_TRACE;

    FormFactoryRegisteredEvent.Initialize();
    FormFactoryUnregisteredEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CGUIManager::CGUIManager( void )
    : m_widgetFactory()            ,
      m_formFactory()              ,
      m_formBackendFactory( NULL ) ,
      m_guiDrivers()
{GUCEF_TRACE;

    RegisterEvents();
    
    SubscribeTo( &m_formFactory                                                 ,
                 CORE::CAbstractFactoryBase::ConcreteFactoryRegisteredEvent     ,
                 &TEventCallback( this, &CGUIManager::OnFormFactoryRegistered ) );
    SubscribeTo( &m_formFactory                                                   ,
                 CORE::CAbstractFactoryBase::ConcreteFactoryUnregisteredEvent     ,
                 &TEventCallback( this, &CGUIManager::OnFormFactoryUnregistered ) );
}

/*-------------------------------------------------------------------------*/

CGUIManager::~CGUIManager()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CGUIContext*
CGUIManager::CreateGUIContext( const CString& guiDriver )
{GUCEF_TRACE;

    return NULL;
}
/*-------------------------------------------------------------------------*/

void
CGUIManager::DestroyGUIContext( CGUIContext* context )
{GUCEF_TRACE;

    
}

/*-------------------------------------------------------------------------*/

void
CGUIManager::RegisterGUIDriver( CIGUIDriver* guiDriver )
{GUCEF_TRACE;

    //m_guiDrivers
}

/*-------------------------------------------------------------------------*/
    
void
CGUIManager::UnregisterGUIDriver( CIGUIDriver* guiDriver )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
void
CGUIManager::UnregisterGUIDriverByName( const CString& guiDriverName )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CWidgetFactory&
CGUIManager::GetWidgetFactory( void )
{GUCEF_TRACE;

    return m_widgetFactory;
}

/*-------------------------------------------------------------------------*/
    
CFormFactory&
CGUIManager::GetFormFactory( void )
{GUCEF_TRACE;

    return m_formFactory;
}

/*-------------------------------------------------------------------------*/

void
CGUIManager::SetFormBackendFactory( CFormBackendFactory* backendFactory )
{GUCEF_TRACE;

    m_formBackendFactory = backendFactory;
}

/*-------------------------------------------------------------------------*/

CFormBackendFactory*
CGUIManager::GetFormBackendFactory( void )
{GUCEF_TRACE;

    return m_formBackendFactory;
}

/*-------------------------------------------------------------------------*/

void
CGUIManager::OnFormFactoryRegistered( CORE::CNotifier* notifier    ,
                                      const CORE::CEvent& eventid  ,
                                      CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    NotifyObservers( FormFactoryRegisteredEvent, eventdata );
}

/*-------------------------------------------------------------------------*/

void
CGUIManager::OnFormFactoryUnregistered( CORE::CNotifier* notifier    ,
                                        const CORE::CEvent& eventid  ,
                                        CORE::CICloneable* eventdata )
{GUCEF_TRACE;

    NotifyObservers( FormFactoryUnregisteredEvent, eventdata );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
