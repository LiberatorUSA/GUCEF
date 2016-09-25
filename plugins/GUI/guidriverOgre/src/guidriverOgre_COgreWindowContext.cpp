/*
 *  guidriverWin32GL: module implementing GL based window management for Win32
 *  Copyright (C) 2002 - 2011.  Dinand Vanvelzen
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

#ifndef _Ogre_H__
#include "Ogre.h"
#define _Ogre_H__
#endif /* _Ogre_H__ ? */ 

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_CGUCEFAPPLICATION_H
#include "CGUCEFApplication.h"
#define GUCEF_CORE_CGUCEFAPPLICATION_H
#endif /* GUCEF_CORE_CGUCEFAPPLICATION_H ? */

#ifndef GUCEF_CORE_CPULSEGENERATOR_H
#include "gucefCORE_CPulseGenerator.h"
#define GUCEF_CORE_CPULSEGENERATOR_H
#endif /* GUCEF_CORE_CPULSEGENERATOR_H ? */

#ifndef GUCEF_GUI_CWINDOWCONTEXT_H
#include "gucefGUI_CWindowContext.h"
#define GUCEF_GUI_CWINDOWCONTEXT_H
#endif /* GUCEF_GUI_CWINDOWCONTEXT_H ? */

#include "guidriverOgre_COgreWindowContext.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUIDRIVEROGRE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

COgreWindowContext::COgreWindowContext( void )
    : CWindowContext()         ,
      m_guiContext()           ,
      m_id()                   ,
      m_name()                 ,
      m_osWindow( 0 )          ,
      m_renderWindow( 0 )
{GUCEF_TRACE;

    m_osWindow = CORE::COSWindow::Create();
    SubscribeTo( m_osWindow );
}

/*-------------------------------------------------------------------------*/

COgreWindowContext::~COgreWindowContext()
{GUCEF_TRACE;

    Shutdown();
    CORE::COSWindow::Destroy( m_osWindow );
    m_osWindow = 0;
}

/*-------------------------------------------------------------------------*/

void
COgreWindowContext::SetGuiContext( GUI::TGuiContextPtr& context )
{GUCEF_TRACE;

    m_guiContext = context;
}

/*-------------------------------------------------------------------------*/

GUI::TGuiContextPtr
COgreWindowContext::GetGuiContext( void )
{GUCEF_TRACE;

    return m_guiContext;
}

/*-------------------------------------------------------------------------*/

GUI::UInt32
COgreWindowContext::GetID( void ) const
{GUCEF_TRACE;

    return m_id;
}

/*-------------------------------------------------------------------------*/

bool
COgreWindowContext::IsActive( void ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

GUI::CString
COgreWindowContext::GetName( void ) const
{GUCEF_TRACE;

    return m_name;
}

/*-------------------------------------------------------------------------*/

void
COgreWindowContext::Shutdown( void )
{GUCEF_TRACE;

	CORE::CPulseGenerator& pulseGenerator = CORE::CCoreGlobal::Instance()->GetPulseGenerator();
    pulseGenerator.RequestStopOfPeriodicUpdates( this );
    UnsubscribeFrom( &pulseGenerator );

    if ( NULL != m_osWindow )
    {
        m_osWindow->WindowDestroy();
    }
}

/*-------------------------------------------------------------------------*/

GUI::UInt32
COgreWindowContext::GetWidth( void ) const
{GUCEF_TRACE;

    int dummy;
    int width;
    if ( m_osWindow->GetClientArea( dummy, dummy, width, dummy ) )
    {
        return (GUI::UInt32) width;
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

GUI::UInt32
COgreWindowContext::GetHeight( void ) const
{GUCEF_TRACE;
    
    int dummy;
    int height;
    if ( m_osWindow->GetClientArea( dummy, dummy, dummy, height ) )
    {
        return (GUI::UInt32) height;
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

GUI::CString
COgreWindowContext::GetProperty( const GUI::CString& propertyName ) const
{GUCEF_TRACE;

    return m_osWindow->GetProperty( propertyName );
}

/*-------------------------------------------------------------------------*/

bool
COgreWindowContext::Initialize( const GUI::CString& title                ,
                                const GUI::CVideoSettings& videoSettings )
{GUCEF_TRACE;

    // Do not initialize twice
    Shutdown();

    // First create a regular Win32 window
    if ( m_osWindow->WindowCreate( title                                       ,
                                   0                                           ,
                                   0                                           ,
                                   videoSettings.GetResolutionWidthInPixels()  ,
                                   videoSettings.GetResolutionHeightInPixels() ) )
    {
        // Display the new window
        m_osWindow->Show();
        m_osWindow->SendToForegound();
        m_osWindow->GrabFocus();

	    // Now proceed with setting up the Ogre specifics
        
        // We grab the O/S window identifier 'the handle' 
        // This is passed to Ogre to tie things together
        CORE::Int64 windowRef = 0; 
        CORE::CString windowIntStr = m_osWindow->GetProperty( "WINDOWINT" );
        if ( !windowIntStr.IsNULLOrEmpty() )
        {
            windowRef = CORE::StringToInt64( windowIntStr );
        }
        Ogre::NameValuePairList options;         
        options[ "externalWindowHandle" ] = Ogre::StringConverter::toString( (size_t) windowRef ); 

        Ogre::Root* ogreRoot = Ogre::Root::getSingletonPtr();
        m_renderWindow = ogreRoot->createRenderWindow( title, 
                                                       videoSettings.GetResolutionWidthInPixels(), 
                                                       videoSettings.GetResolutionHeightInPixels(), 
                                                       videoSettings.GetFullscreenState(),
                                                       &options ); 

        // Grab the main app pulse generator and set the update interval for the context to the desired refresh rate
        CORE::CPulseGenerator& pulseGenerator = CORE::CCoreGlobal::Instance()->GetPulseGenerator(); 
        pulseGenerator.RequestPeriodicPulses( this, 1000 / videoSettings.GetFrequency() );
        SubscribeTo( &pulseGenerator );

        GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "OgreWindowContext: Succesfully created Ogre rendering context" );
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

const CORE::CString&
COgreWindowContext::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CORE::CString classTypeName = "GUCEF::GUIDRIVEROGRE::COgreWindowContext";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

void
COgreWindowContext::OnNotify( CORE::CNotifier* notifier   ,
                              const CORE::CEvent& eventID ,
                              CORE::CICloneable* evenData )
{GUCEF_TRACE;

    if ( ( eventID == CORE::CPulseGenerator::PulseEvent ) ||
         ( eventID == CORE::COSWindow::WindowPaintEvent ) )
    {
        // Notify that we are going to redraw the window
        NotifyObservers( WindowContextRedrawEvent );        
    }
    else
    if ( eventID == CORE::COSWindow::WindowResizeEvent )
    {
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "OgreWindowContext: Window resize event received" );
        m_renderWindow->windowMovedOrResized();
        NotifyObservers( WindowContextSizeEvent );
    }
    else
    if ( eventID == CORE::COSWindow::WindowActivationEvent )
    {
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "OgreWindowContext: Window activation event received" );
        NotifyObservers( WindowContextActivateEvent );
    }
    else
    if ( ( eventID == CORE::COSWindow::WindowCloseEvent )   ||
         ( eventID == CORE::COSWindow::WindowDestroyEvent ) )
    {
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "OgreWindowContext: Window close/destroy event received" );
        m_renderWindow->destroy(); // cleanup and call DestroyWindow 
        Shutdown();
    }
}


/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace GUIDRIVEROGRE */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
