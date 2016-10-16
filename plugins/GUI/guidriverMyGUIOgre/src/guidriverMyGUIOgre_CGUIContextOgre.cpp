/*
 *  guidriverMyGUIOgre: glue module for the MyGUI GUI backend using OpenGL
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

#ifndef GUCEF_MYGUIOGRE_CGUIDRIVEROGRE_H
#include "guidriverMyGUIOgre_CGUIDriverOgre.h"
#define GUCEF_MYGUIOGRE_CGUIDRIVEROGRE_H
#endif /* GUCEF_MYGUIOGRE_CGUIDRIVEROGRE_H ? */

#include "guidriverMyGUIOgre_CGUIContextOgre.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace MYGUIOGRE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CGUIContextOgre::CGUIContextOgre( CGUIDriverOgre& guiDriver               ,
                                  MyGUI::OgreRenderManager* renderManager ,
                                  GUI::TWindowContextPtr windowContext    ,
                                  INPUT::CInputContext* inputContext      )
    : MYGUI::CGUIContext( guiDriver    ,
                          inputContext )  ,
      m_renderManager( renderManager )    ,
      m_windowContext( windowContext ) 
{GUCEF_TRACE;

    SubscribeTo( m_windowContext.GetPointer() );
}

/*-------------------------------------------------------------------------*/

CGUIContextOgre::~CGUIContextOgre()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

const CORE::CString&
CGUIContextOgre::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CORE::CString classTypeName = "GUCEF::MYGUIGL::CGUIContextOgre";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

void
CGUIContextOgre::OnNotify( CORE::CNotifier* notifier   ,
                         const CORE::CEvent& eventID ,
                         CORE::CICloneable* evenData )
{GUCEF_TRACE;

    if ( eventID == GUI::CWindowContext::WindowContextRedrawEvent   ||
         eventID == GUI::CWindowContext::WindowContextActivateEvent  )
    {
        //m_renderManager->drawOneFrame();
    }
    else
    if ( eventID == GUI::CWindowContext::WindowContextSizeEvent )
    {        
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "GUIContext: Resizing MyGUI GUI context to " + CORE::UInt32ToString( m_windowContext->GetWidth() ) + "x" +
                                                                                               CORE::UInt32ToString( m_windowContext->GetHeight() ) );

		//m_renderManager->setViewSize( m_windowContext->GetWidth(), m_windowContext->GetHeight() );
        //m_renderManager->drawOneFrame();
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
