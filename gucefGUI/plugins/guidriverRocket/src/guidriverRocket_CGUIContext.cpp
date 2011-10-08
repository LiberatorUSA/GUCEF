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

#include <assert.h>

#ifndef GUCE_MYGUIOGRE_CGUIDRIVER_H
#include "guceMyGUIOgre_CGUIDriver.h"
#define GUCE_MYGUIOGRE_CGUIDRIVER_H
#endif /* GUCE_MYGUIOGRE_CGUIDRIVER_H ? */

#include "guceMyGUIOgre_CGUIContext.h"

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

CGUIContext::CGUIContext( CGUIDriver& myGuiDriver )
    : GUCEF::GUI::CIGUIContext() ,
      m_driver( &myGuiDriver )   ,
      m_widgetSet()              ,
      m_formSet()
{GUCE_TRACE;

    assert( NULL != m_driver );
}

/*-------------------------------------------------------------------------*/

CGUIContext::~CGUIContext()
{GUCE_TRACE;

}

/*-------------------------------------------------------------------------*/

GUCEF::GUI::CWidget*
CGUIContext::CreateWidget( const CString& widgetName )
{GUCE_TRACE;

    GUCEF::GUI::CWidget* widget = m_driver->CreateWidget( widgetName );
    if ( NULL != widget )
    {
        m_widgetSet.insert( widget );
        return widget;
    }
    return NULL;
}

/*-------------------------------------------------------------------------*/
    
void
CGUIContext::DestroyWidget( GUCEF::GUI::CWidget* widget )
{GUCE_TRACE;

    m_widgetSet.erase( widget );
    m_driver->DestroyWidget( widget );
}

/*-------------------------------------------------------------------------*/
    
GUCEF::GUI::CForm*
CGUIContext::CreateForm( const CString& formName )
{GUCE_TRACE;

    GUCEF::GUI::CForm* form = m_driver->CreateForm( formName );
    if ( NULL != form )
    {
        m_formSet.insert( form );
        return form;
    }
    return NULL;
}

/*-------------------------------------------------------------------------*/

void
CGUIContext::DestroyForm( GUCEF::GUI::CForm* form )
{GUCE_TRACE;

    m_formSet.erase( form );
    m_driver->DestroyForm( form ); 
}

/*-------------------------------------------------------------------------*/

CGUIContext::TStringSet
CGUIContext::GetAvailableFormTypes( void )
{GUCE_TRACE;

    return m_driver->GetAvailableFormTypes();
}

/*-------------------------------------------------------------------------*/
    
CGUIContext::TStringSet
CGUIContext::GetAvailableWidgetTypes( void )
{GUCE_TRACE;

    return m_driver->GetAvailableWidgetTypes();
}

/*-------------------------------------------------------------------------*/
    
GUCEF::GUI::CFormBackend*
CGUIContext::CreateFormBackend( void )
{GUCE_TRACE;

    return m_driver->CreateFormBackend();
}

/*-------------------------------------------------------------------------*/
    
void
CGUIContext::DestroyFormBackend( GUCEF::GUI::CFormBackend* formBackend )
{GUCE_TRACE;

    m_driver->DestroyFormBackend( formBackend );
}

/*-------------------------------------------------------------------------*/
    
GUCEF::GUI::CGUIDriver*
CGUIContext::GetDriver( void )
{GUCE_TRACE;

    return m_driver;
}

/*-------------------------------------------------------------------------*/

CGUIContext::TWidgetSet
CGUIContext::GetOwnedWidgets( void )
{GUCE_TRACE;
    
    return m_widgetSet;
}

/*-------------------------------------------------------------------------*/
    
CGUIContext::TFormSet
CGUIContext::GetOwnedForms( void )
{GUCE_TRACE;

    return m_formSet;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MYGUIOGRE */
}; /* namespace GUCE */

/*-------------------------------------------------------------------------*/
