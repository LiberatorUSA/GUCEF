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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <RmlUi/Core.h>

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "gucefCORE_CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_INPUT_CINPUTGLOBAL_H
#include "gucefINPUT_CInputGlobal.h"
#define GUCEF_INPUT_CINPUTGLOBAL_H
#endif /* GUCEF_INPUT_CINPUTGLOBAL_H ? */

#ifndef GUCEF_INPUT_CINPUTCONTROLLER_H
#include "CInputController.h"
#define GUCEF_INPUT_CINPUTCONTROLLER_H
#endif /* GUCEF_INPUT_CINPUTCONTROLLER_H ? */

#ifndef GUCEF_GUIDRIVERRMLUI_CGUICONTEXT_H
#include "guidriverRmlUI_CGUIContext.h"
#define GUCEF_GUIDRIVERRMLUI_CGUICONTEXT_H
#endif /* GUCEF_GUIDRIVERRMLUI_CGUICONTEXT_H ? */

#include "guidriverRmlUIOpenGL_CRmlUIGuiDriver.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUIDRIVERRMLUIGL {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CRmlUIGuiDriver::CRmlUIGuiDriver( void )
    : GUI::CGUIDriver()              ,
      m_renderer()             ,
      m_systemInterface()            ,
      m_fileInterface()              ,
      m_isRmlUIInitialized( false ) ,
      m_contextSet()                 ,
      m_fontTypes()
{GUCEF_TRACE;

	Rml::SetRenderInterface( &m_renderer );
    Rml::SetSystemInterface( &m_systemInterface );
    Rml::SetFileInterface( &m_fileInterface );
}

/*-------------------------------------------------------------------------*/

CRmlUIGuiDriver::~CRmlUIGuiDriver()
{GUCEF_TRACE;

    // Shutting down also releases the contexts and everything else RmlUi holds, which uses the interfaces
    // that are our members and have to stay valid until then
    if ( m_isRmlUIInitialized )
        Rml::Shutdown();
}

/*-------------------------------------------------------------------------*/

GUI::TGuiContextPtr
CRmlUIGuiDriver::CreateGUIContext( GUI::TWindowContextPtr windowContext )
{GUCEF_TRACE;

    // Lazy initialize RmlUi as needed
    if ( !m_isRmlUIInitialized )
    {
        m_isRmlUIInitialized = Rml::Initialise();
        if ( !m_isRmlUIInitialized )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Failed to initialize RmlUi" );
            return GUI::TGuiContextPtr();
        }
        else
        {
            GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "Initialized RmlUi" );
        }
    }

    Rml::Context* context = NULL;
    if ( NULL != windowContext )
    {
        // Create the main RmlUi context using the parameters of the windowContext
        context = Rml::CreateContext( CORE::PointerToString( windowContext.GetPointer() ).C_String()                  ,
                                      Rml::Vector2i( windowContext->GetWidth(), windowContext->GetHeight() ) );
    }
    else
    {
        // Create the main RmlUi context using defaults, it is expected to be altered later
        GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "Initialized RmlUi context with defaults since no window context was provided" );
        context = Rml::CreateContext( "default"                      ,
                                               Rml::Vector2i( 2, 2 ) );
    }

    if ( NULL == context )
    {
        // Unable to create a RmlUi context
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "Failed to create RmlUi GUI context" );
        return GUI::TGuiContextPtr();
    }

    // Create an input context using the default driver and set it for this GUI context so that we can interact
    // with the GUI based on input events. Note that in order to correctly tie in the input system we have to pass the
    // correct parameters for the O/S
    CORE::CValueList inputContextParams;
    inputContextParams.Set( "WINDOW", windowContext->GetProperty( "WINDOW" ) );
    INPUT::CInputContext* inputContext = INPUT::CInputGlobal::Instance()->GetInputController().CreateContext( inputContextParams );

    // Add a reference to the context in our set
    GUI::TGuiContextPtr guiContextPtr = GUI::TGuiContextPtr( new guidriverRmlUI::CGUIContext( this, context, windowContext, inputContext ) );
    m_contextSet.insert( guiContextPtr );

    return guiContextPtr;
}

/*-------------------------------------------------------------------------*/

GUI::CGUIDriver::TGUIContextSet
CRmlUIGuiDriver::GetContextList( void )
{GUCEF_TRACE;

    return m_contextSet;
}

/*-------------------------------------------------------------------------*/

GUI::UInt32
CRmlUIGuiDriver::GetContextCount( void )
{GUCEF_TRACE;

    return (GUI::UInt32) m_contextSet.size();
}

/*-------------------------------------------------------------------------*/

GUI::CString
CRmlUIGuiDriver::GetDriverName( void )
{GUCEF_TRACE;

    return "RmlUIOpenGL";
}

/*-------------------------------------------------------------------------*/

GUI::CGUIDriver::TStringSet
CRmlUIGuiDriver::GetAvailableFormTypes( void )
{GUCEF_TRACE;

    return GUI::CGUIDriver::TStringSet();
}

/*-------------------------------------------------------------------------*/

GUI::CGUIDriver::TStringSet
CRmlUIGuiDriver::GetAvailableWidgetTypes( void )
{GUCEF_TRACE;

    return GUI::CGUIDriver::TStringSet();
}

/*-------------------------------------------------------------------------*/

GUI::CGUIDriver::TStringSet
CRmlUIGuiDriver::GetAvailableFonts( void )
{GUCEF_TRACE;

    return m_fontTypes;
}

/*-------------------------------------------------------------------------*/

bool
CRmlUIGuiDriver::LoadFontFromAsset( const GUI::CString& assetPath )
{GUCEF_TRACE;

    // Simple pass on the load request to RmlUi, all the callback inerfaces will take
    // care of integrating with GUCEF
    if ( Rml::LoadFontFace( assetPath.C_String() ) )
    {
        // @TODO: fix somehow
        // This is not correct but at present RmlUi cant give you the name of the font back
        m_fontTypes.insert( assetPath );
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

const GUI::CString&
CRmlUIGuiDriver::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static GUI::CString classTypeName = "GUCEF::GUIDRIVERRMLUIGL::CRmlUIGuiDriver";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

GUI::CString
CRmlUIGuiDriver::GetDriverProperty( const GUI::CString& propertyName ) const
{GUCEF_TRACE;

    return GUI::CString();
}

/*-------------------------------------------------------------------------*/

bool
CRmlUIGuiDriver::SaveConfig( CORE::CDataNode& tree ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CRmlUIGuiDriver::LoadConfig( const CORE::CDataNode& treeroot )
{GUCEF_TRACE;

    return true;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUIDRIVERRMLUIGL */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
