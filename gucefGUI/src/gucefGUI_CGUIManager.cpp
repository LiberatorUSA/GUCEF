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

CGUIManager::CGUIManager( void )
    : m_widgetFactory()            ,
      m_formFactory()              ,
      m_formBackendFactory( NULL )
{GUCEF_TRACE;

    
}

/*-------------------------------------------------------------------------*/

CGUIManager::~CGUIManager()
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
