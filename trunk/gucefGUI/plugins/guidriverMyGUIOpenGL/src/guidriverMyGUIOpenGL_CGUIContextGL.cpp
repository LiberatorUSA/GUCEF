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

#ifndef GUCEF_MYGUIGL_CGUIDRIVERGL_H
#include "guidriverMyGUIOpenGL_CGUIDriverGL.h"
#define GUCEF_MYGUIGL_CGUIDRIVERGL_H
#endif /* GUCEF_MYGUIGL_CGUIDRIVERGL_H ? */

#include "guidriverMyGUIOpenGL_CGUIContextGL.h"

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

CGUIContextGL::CGUIContextGL( CGUIDriverGL& guiDriver               ,
                              MyGUI::OpenGLImageLoader* imageLoader ,
                              GUI::TWindowContextPtr windowContext  ,
                              INPUT::CInputContext* inputContext    )
    : MYGUI::CGUIContext( guiDriver    ,
                          inputContext )  ,
      m_myGuiPlatform()                   ,
      m_windowContext( windowContext ) 
{GUCEF_TRACE;

    m_myGuiPlatform.initialise( imageLoader );
}

/*-------------------------------------------------------------------------*/

CGUIContextGL::~CGUIContextGL()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

const CORE::CString&
CGUIContextGL::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CORE::CString classTypeName = "GUCEF::MYGUIGL::CGUIContextGL";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

void
CGUIContextGL::OnNotify( CORE::CNotifier* notifier   ,
                         const CORE::CEvent& eventID ,
                         CORE::CICloneable* evenData )
{GUCEF_TRACE;

    if ( eventID == GUI::CWindowContext::WindowContextRedrawEvent )
    {
        m_myGuiPlatform.getRenderManagerPtr()->drawOneFrame();
    }
    else
    if ( eventID == GUI::CWindowContext::WindowContextSizeEvent )
    {        
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "GUIContext: Resizing MyGUI GUI context to " + CORE::UInt32ToString( m_windowContext->GetWidth() ) + "x" +
                                                                                               CORE::UInt32ToString( m_windowContext->GetHeight() ) );

		m_myGuiPlatform.getRenderManagerPtr()->setViewSize( m_windowContext->GetWidth(), m_windowContext->GetHeight() );
        m_myGuiPlatform.getRenderManagerPtr()->drawOneFrame();
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MYGUI */
}; /* namespace GUCE */

/*-------------------------------------------------------------------------*/
