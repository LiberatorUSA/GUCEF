/*
 *  guidriverMyGUIOpenGL: glue module for the MyGUI GUI backend using OpenGL
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

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "CIConfigurable.h"
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

#ifndef GUCEF_MYGUIGL_CGUICONTEXTGL_H
#include "guidriverMyGUIOpenGL_CGUIContextGL.h"
#define GUCEF_MYGUIGL_CGUICONTEXTGL_H
#endif /* GUCEF_MYGUIGL_CGUICONTEXTGL_H ? */

#include "guidriverMyGUIOpenGL_CGUIDriverGL.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace MYGUIGL {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CGUIDriverGL::CGUIDriverGL( void )
    : GUI::CGUIDriver()           ,
      m_contextSet()              ,
      m_fontTypes()               ,
      m_myGUI()                   ,
      m_renderManager()           ,
      m_imageLoader()             ,
      m_logAdapter()              ,
      m_dataManager()             ,
      m_myGuiInitialized( false )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CGUIDriverGL::~CGUIDriverGL()
{GUCEF_TRACE;

    m_myGUI.shutdown();
}

/*-------------------------------------------------------------------------*/

GUI::TGuiContextPtr
CGUIDriverGL::CreateGUIContext( GUI::TWindowContextPtr windowContext )
{GUCEF_TRACE;

    // Lazy initialize if needed
    if ( !m_myGuiInitialized )
    {
        m_renderManager.initialise( &m_imageLoader );
        m_myGUI.initialise();        

        m_myGuiInitialized = true;
    }
    
    // Create an input context using the default driver and set it for this GUI context so that we can interact
    // with the GUI based on input events. Note that in order to correctly tie in the input system we have to pass the
    // correct parameters for the O/S
    CORE::CValueList inputContextParams;
    inputContextParams.Set( "WINDOW", windowContext->GetProperty( "WINDOW" ) );
    INPUT::CInputContext* inputContext = INPUT::CInputGlobal::Instance()->GetInputController().CreateContext( inputContextParams );

    // Add a reference to the context in our set
    GUI::TGuiContextPtr guiContextPtr = new CGUIContextGL( *this            ,
                                                           &m_renderManager ,
                                                           windowContext    ,
                                                           inputContext     );
    m_contextSet.insert( guiContextPtr );

    return guiContextPtr;
}

/*-------------------------------------------------------------------------*/

GUI::CGUIDriver::TGUIContextSet
CGUIDriverGL::GetContextList( void )
{GUCEF_TRACE;

    return m_contextSet;
}

/*-------------------------------------------------------------------------*/

GUI::UInt32
CGUIDriverGL::GetContextCount( void )
{GUCEF_TRACE;

    return m_contextSet.size();
}

/*-------------------------------------------------------------------------*/

GUI::CString
CGUIDriverGL::GetDriverName( void )
{GUCEF_TRACE;

    return "MyGUIOpenGL";
}

/*-------------------------------------------------------------------------*/

GUI::CGUIDriver::TStringSet
CGUIDriverGL::GetAvailableFormTypes( void )
{GUCEF_TRACE;

    return GUI::CGUIDriver::TStringSet();
}

/*-------------------------------------------------------------------------*/

GUI::CGUIDriver::TStringSet
CGUIDriverGL::GetAvailableWidgetTypes( void )
{GUCEF_TRACE;

    return GUI::CGUIDriver::TStringSet();
}

/*-------------------------------------------------------------------------*/

GUI::CGUIDriver::TStringSet
CGUIDriverGL::GetAvailableFonts( void )
{GUCEF_TRACE;

    return m_fontTypes;
}

/*-------------------------------------------------------------------------*/

bool
CGUIDriverGL::LoadFontFromAsset( const GUI::CString& assetPath )
{GUCEF_TRACE;

    //// Simple pass on the load request to Rocket, all the callback inerfaces will take
    //// care of integrating with GUCEF
    //if ( Rocket::Core::FontDatabase::LoadFontFace( assetPath.C_String() ) )
    //{
    //    // @TODO: fix somehow
    //    // This is not correct but at present Rocket cant give you the name of the font back
    //    m_fontTypes.insert( assetPath );
    //    return true;
    //}
    return false;
}

/*-------------------------------------------------------------------------*/

const GUI::CString&
CGUIDriverGL::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static GUI::CString classTypeName = "GUCEF::MYGUIGL::CGUIDriverGL";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

GUI::CString
CGUIDriverGL::GetDriverProperty( const GUI::CString& propertyName ) const
{GUCEF_TRACE;

    return GUI::CString();
}

/*-------------------------------------------------------------------------*/

bool
CGUIDriverGL::SaveConfig( CORE::CDataNode& tree )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CGUIDriverGL::LoadConfig( const CORE::CDataNode& treeroot )
{GUCEF_TRACE;

    return true;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MYGUIGL */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
