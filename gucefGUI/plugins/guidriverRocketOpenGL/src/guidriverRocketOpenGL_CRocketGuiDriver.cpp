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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <Rocket/Core.h>

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_GUIDRIVERROCKET_CGUICONTEXT_H
#include "guidriverRocket_CGUIContext.h"
#define GUCEF_GUIDRIVERROCKET_CGUICONTEXT_H
#endif /* GUCEF_GUIDRIVERROCKET_CGUICONTEXT_H ? */

#include "guidriverRocketOpenGL_CRocketGuiDriver.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUIDRIVERROCKETGL {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CRocketGuiDriver::CRocketGuiDriver( void )
    : GUI::CGUIDriver()              ,
      m_rocketRenderer()             ,
      m_systemInterface()            ,
      m_isRocketInitialized( false )
{GUCEF_TRACE;

	Rocket::Core::SetRenderInterface( &m_rocketRenderer );
    Rocket::Core::SetSystemInterface( &m_systemInterface );
}

/*-------------------------------------------------------------------------*/

CRocketGuiDriver::~CRocketGuiDriver()
{GUCEF_TRACE;

    Rocket::Core::SetRenderInterface( NULL );
    Rocket::Core::SetSystemInterface( NULL );
    Rocket::Core::Shutdown();
}

/*-------------------------------------------------------------------------*/
    
GUI::TGuiContextPtr
CRocketGuiDriver::CreateGUIContext( GUI::TWindowContextPtr windowContext )
{GUCEF_TRACE;

    // Lazy initialize Rocket as needed
    if ( !m_isRocketInitialized )
    {
        m_isRocketInitialized = Rocket::Core::Initialise();
        if ( !m_isRocketInitialized )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Failed to initialize Rocket" );
            return GUI::TGuiContextPtr(); 
        }
        else
        {
            GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "Initialized Rocket" );
        }
    }

    Rocket::Core::Context* context = NULL;
    if ( NULL != windowContext )
    {
        // Create the main Rocket context using the parameters of the windowContext
        context = Rocket::Core::CreateContext( CORE::PointerToString( windowContext.GetPointer() ).C_String()                  ,
                                               Rocket::Core::Vector2i( windowContext->GetWidth(), windowContext->GetHeight() ) );
    }
    else
    {
        // Create the main Rocket context using defaults, it is expected to be altered later
        GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "Initialized Rocket context with defaults since no window context was provided" );
        context = Rocket::Core::CreateContext( "default"                      ,
                                               Rocket::Core::Vector2i( 2, 2 ) );
    }

    if ( NULL == context )
    {
        // Unable to create a Rocket context
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Failed to create Rocket GUI context" );
        Rocket::Core::Shutdown();
        return GUI::TGuiContextPtr();
    }
    
    return new GUIDRIVERROCKET::CGUIContext( this );
}

/*-------------------------------------------------------------------------*/
    
GUI::CGUIDriver::TGUIContextSet
CRocketGuiDriver::GetContextList( void )
{GUCEF_TRACE;

    return GUI::CGUIDriver::TGUIContextSet();
}

/*-------------------------------------------------------------------------*/
    
GUI::UInt32
CRocketGuiDriver::GetContextCount( void )
{GUCEF_TRACE;

    return 0;
}

/*-------------------------------------------------------------------------*/
    
GUI::CString
CRocketGuiDriver::GetDriverName( void )
{GUCEF_TRACE;
    
    return "RocketOpenGL";
}

/*-------------------------------------------------------------------------*/
    
GUI::CGUIDriver::TStringSet
CRocketGuiDriver::GetAvailableFormTypes( void )
{GUCEF_TRACE;

    return GUI::CGUIDriver::TStringSet();
}

/*-------------------------------------------------------------------------*/

GUI::CGUIDriver::TStringSet
CRocketGuiDriver::GetAvailableWidgetTypes( void )
{GUCEF_TRACE;

    return GUI::CGUIDriver::TStringSet();
}

/*-------------------------------------------------------------------------*/
    
const GUI::CString&
CRocketGuiDriver::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static GUI::CString classTypeName = "GUCEF::GUIDRIVERROCKETGL::CRocketGuiDriver";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/
    
GUI::CString
CRocketGuiDriver::GetDriverProperty( const GUI::CString& propertyName ) const
{GUCEF_TRACE;

    return GUI::CString();
}

/*-------------------------------------------------------------------------*/

bool
CRocketGuiDriver::SaveConfig( CORE::CDataNode& tree )
{GUCEF_TRACE;
    
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CRocketGuiDriver::LoadConfig( const CORE::CDataNode& treeroot )
{GUCEF_TRACE;

    return true;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUIDRIVERROCKETGL */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
