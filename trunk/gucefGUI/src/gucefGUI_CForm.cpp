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

#ifndef GUCEF_GUI_CGUIMANAGER_H
#include "gucefGUI_CGUIManager.h"
#define GUCEF_GUI_CGUIMANAGER_H
#endif /* GUCEF_GUI_CGUIMANAGER_H ? */

#ifndef GUCEF_GUI_CIGUICONTEXT_H
#include "gucefGUI_CIGUIContext.h"
#define GUCEF_GUI_CIGUICONTEXT_H
#endif /* GUCEF_GUI_CIGUICONTEXT_H ? */

#include "gucefGUI_CForm.h"

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

const CORE::CEvent CForm::LayoutLoadedEvent = "GUCEF::GUI::CForm::LayoutLoadedEvent";
const CORE::CEvent CForm::LayoutSavedEvent = "GUCEF::GUI::CForm::LayoutSavedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/
    
void
CForm::RegisterEvents( void )
{GUCEF_TRACE;

    LayoutLoadedEvent.Initialize();
    LayoutSavedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CForm::CForm( void )
    : CORE::CObservingNotifier() ,
      m_backend( NULL )          ,
      m_parentWidget( NULL )     ,
      m_context( NULL )
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CForm::CForm( CIGUIContext* context )
    : CORE::CObservingNotifier() ,
      m_backend( NULL )          ,
      m_parentWidget( NULL )     ,
      m_context( context )
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CForm::~CForm()
{GUCEF_TRACE;

    delete m_backend;
    m_backend = NULL;
}

/*-------------------------------------------------------------------------*/

void
CForm::SetContext( CIGUIContext* context )
{GUCEF_TRACE;

    m_context = context;
}

/*-------------------------------------------------------------------------*/
    
CIGUIContext*
CForm::GetContext( void )
{GUCEF_TRACE;

    return m_context;
}

/*-------------------------------------------------------------------------*/
    
bool
CForm::LoadLayout( CORE::CIOAccess& layoutStorage )
{GUCEF_TRACE;

    if ( NULL != m_context )
    {
        if ( NULL == m_backend )
        {
            m_backend = m_context->CreateFormBackend();
        }
    }
    
    if ( NULL == m_backend )
    {
        OnPreLayoutLoad();
        if ( m_backend->LoadLayout( layoutStorage ) )
        {
            if ( NULL != m_parentWidget )
            {
                SetParent( m_parentWidget );
            }
            OnPostLayoutLoad();
            NotifyObservers( LayoutLoadedEvent );
            return true;
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/
    
bool
CForm::SaveLayout( CORE::CIOAccess& layoutStorage )
{GUCEF_TRACE;

    if ( NULL != m_backend )
    {
        if ( m_backend->SaveLayout( layoutStorage ) )
        {
            NotifyObservers( LayoutSavedEvent );
            return true;
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/
    
const CWidget*
CForm::GetRootWidget( void ) const
{GUCEF_TRACE;

    if ( NULL != m_backend )
    {
        return m_backend->GetRootWidget();
    }
    return NULL;
}

/*-------------------------------------------------------------------------*/
    
CWidget*
CForm::GetRootWidget( void )
{GUCEF_TRACE;

    if ( NULL != m_backend )
    {
        return m_backend->GetRootWidget();
    }
    return NULL;
}

/*-------------------------------------------------------------------------*/

CWidget*
CForm::GetWidget( const CString& widgetName )
{GUCEF_TRACE;

    if ( NULL != m_backend )
    {
        return m_backend->GetWidget( widgetName );
    }
    return NULL;    
}

/*-------------------------------------------------------------------------*/

bool
CForm::SetVisibility( const bool isVisible )
{GUCEF_TRACE;

    CWidget* widget = GetRootWidget();
    if ( NULL != widget )
    {
        return widget->SetVisibility( isVisible );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CForm::IsVisible( void ) const
{GUCEF_TRACE;

    const CWidget* widget = GetRootWidget();
    if ( NULL != widget )
    {
        return widget->IsVisible();
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CForm::Show( void )
{GUCEF_TRACE;

    CWidget* widget = GetRootWidget();
    if ( NULL != widget )
    {
        return widget->SetVisibility( true );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CForm::ShowModal( void )
{GUCEF_TRACE;

    CWidget* widget = GetRootWidget();
    if ( NULL != widget )
    {
        return widget->ShowModal();
    }
    return false;    
}

/*-------------------------------------------------------------------------*/
    
bool
CForm::Hide( void )
{GUCEF_TRACE;

    CWidget* widget = GetRootWidget();
    if ( NULL != widget )
    {
        return widget->SetVisibility( false );
    }
    return false;    
}

/*-------------------------------------------------------------------------*/

bool
CForm::SetParent( CForm* parentForm )
{GUCEF_TRACE;

    CWidget* widget = GetRootWidget();
    if ( NULL != widget )
    {
        if ( NULL != parentForm )
        {
            m_parentWidget = parentForm->GetRootWidget();
            if ( NULL != m_parentWidget )
            {
                return widget->SetParentWidget( m_parentWidget );
            }
        }
        return widget->SetParentWidget( NULL );
    }
    return false;
}

/*-------------------------------------------------------------------------*/
    
bool
CForm::SetParent( CWidget* parentWidget )
{GUCEF_TRACE;

    CWidget* widget = GetRootWidget();
    if ( NULL != widget )
    {
        m_parentWidget = parentWidget;
        return widget->SetParentWidget( m_parentWidget );
    }
    return false;
}

/*-------------------------------------------------------------------------*/
    
CWidget*
CForm::GetParent( void )
{GUCEF_TRACE;

    CWidget* widget = GetRootWidget();
    if ( NULL != widget )
    {
        return widget->GetParentWidget();
    }
    return m_parentWidget;
}

/*-------------------------------------------------------------------------*/

 void
 CForm::OnPreLayoutLoad( void )
 {GUCEF_TRACE;
 
    // no need to do anything
 }
 
 /*-------------------------------------------------------------------------*/
    
 void
 CForm::OnPostLayoutLoad( void )
 {GUCEF_TRACE;
 
    // no need to do anything
 }
 
 /*-------------------------------------------------------------------------*/

const CString&
CForm::GetClassTypeName( void ) const
{
    static CString typeName = "GUCEF::GUI::CForm";
    return typeName;
}

/*-------------------------------------------------------------------------*/

const CString&
CForm::GetType( void ) const
{GUCEF_TRACE;

    static CString typeName = "CForm";
    return typeName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
