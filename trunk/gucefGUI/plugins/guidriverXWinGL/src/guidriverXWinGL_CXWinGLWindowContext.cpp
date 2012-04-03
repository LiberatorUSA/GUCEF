/*
 *  guidriverXWinGL: module implementing GL based window management for X11
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

#include <GL/glx.h>

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

#include "guidriverXWinGL_CXWinGLWindowContext.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUIDRIVERXWINGL {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CXWinGLWindowContext::CXWinGLWindowContext( void )
    : CWindowContext()         ,
      m_guiContext()           ,
      m_id()                   ,
      m_name()                 ,
      m_window()              // ,
      //m_renderContext( NULL )  ,
      //m_deviceContext( NULL )
{GUCEF_TRACE;

    SubscribeTo( &m_window );
}

/*-------------------------------------------------------------------------*/

CXWinGLWindowContext::~CXWinGLWindowContext()
{GUCEF_TRACE;

    Shutdown();
}

/*-------------------------------------------------------------------------*/

void
CXWinGLWindowContext::SetGuiContext( GUI::TGuiContextPtr& context )
{GUCEF_TRACE;

    m_guiContext = context;
}

/*-------------------------------------------------------------------------*/

GUI::TGuiContextPtr
CXWinGLWindowContext::GetGuiContext( void )
{GUCEF_TRACE;

    return m_guiContext;
}

/*-------------------------------------------------------------------------*/

GUI::UInt32
CXWinGLWindowContext::GetID( void ) const
{GUCEF_TRACE;

    return m_id;
}

/*-------------------------------------------------------------------------*/

bool
CXWinGLWindowContext::IsActive( void ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

GUI::CString
CXWinGLWindowContext::GetName( void ) const
{GUCEF_TRACE;

    return m_name;
}

/*-------------------------------------------------------------------------*/

void
CXWinGLWindowContext::Shutdown( void )
{GUCEF_TRACE;

	CORE::CPulseGenerator& pulseGenerator = CORE::CCoreGlobal::Instance()->GetPulseGenerator();
    pulseGenerator.RequestStopOfPeriodicUpdates( this );
    UnsubscribeFrom( &pulseGenerator );

    if ( 0 != m_window.GetWindow() )
    {
        m_window.WindowDestroy();
    }
}

/*-------------------------------------------------------------------------*/

GUI::UInt32
CXWinGLWindowContext::GetWidth( void ) const
{GUCEF_TRACE;

    int dummy;
    int width;
    if ( m_window.GetClientArea( dummy, dummy, width, dummy ) )
    {
        return (GUI::UInt32) width;
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

GUI::UInt32
CXWinGLWindowContext::GetHeight( void ) const
{GUCEF_TRACE;

    int dummy;
    int height;
    if ( m_window.GetClientArea( dummy, dummy, dummy, height ) )
    {
        return (GUI::UInt32) height;
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

GUI::CString
CXWinGLWindowContext::GetProperty( const GUI::CString& propertyName ) const
{GUCEF_TRACE;

    if ( propertyName == "WINDOW" )
    {
        return CORE::UInt32ToString( m_window.GetWindow() );
    }
    if ( propertyName == "DISPLAY" )
    {
        return CORE::PointerToString( m_window.GetDisplay() );
    }
    return GUI::CString();
}

/*-------------------------------------------------------------------------*/

bool
CXWinGLWindowContext::Initialize( const GUI::CString& title                ,
                                  const GUI::CVideoSettings& videoSettings )
{GUCEF_TRACE;

    // Do not initialize twice
    Shutdown();

    // First get access to the display
    ::Display* display = ::XOpenDisplay( NULL );
    if ( NULL == display )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CXWinGLWindowContext: Could not open display" );
        return false;
    }

    // Now proceed with setting up the OpenGL specifics
    int dummy = 0;
    if ( 0 == glXQueryExtension( display, &dummy, &dummy ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CXWinGLWindowContext: X server has no OpenGL GLX extension" );
        Shutdown();
        return false;
    }

    // find an OpenGL-capable Color Index visual with depth buffer
    static int attributes[] = {GLX_RGBA, GLX_DOUBLEBUFFER, None};
    //static int attributes[] = {GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None};
    XVisualInfo* vi = glXChooseVisual( display, DefaultScreen( display ), attributes );
    if ( NULL == vi )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CXWinGLWindowContext: Could not get visual info for display" );
        Shutdown();
        return false;
    }

    // create an OpenGL rendering context
    GLXContext cx = glXCreateContext( display, vi,  None, GL_TRUE );
    if ( NULL == cx )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CXWinGLWindowContext: Could not create rendering context" );
        XFree( vi );
        Shutdown();
        return false;
    }
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CXWinGLWindowContext: Created GL context using GLX" );

    // create an X colormap since probably not using default visual
    ::Colormap cmap = ::XCreateColormap( display, RootWindow( display, vi->screen),
                                vi->visual, AllocNone);

    ::XSetWindowAttributes swa;
    swa.colormap = cmap;
    swa.border_pixel = 0;
    swa.event_mask = ExposureMask | KeyPressMask | StructureNotifyMask;
    ::Window glwin = ::XCreateWindow(display, RootWindow(display, vi->screen), 0, 0, videoSettings.GetResolutionWidthInPixels(),
                        videoSettings.GetResolutionHeightInPixels(), 0, vi->depth, InputOutput, vi->visual,
                        CWBorderPixel | CWColormap | CWEventMask, &swa);
    if ( 0 == glwin )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CXWinGLWindowContext: Failed to create X11 window" );
        ::XFree( vi );
        Shutdown();
        return false;
    }
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CXWinGLWindowContext: Created X11 Window" );

//    ::XSetStandardProperties(display, glwin, "xogl", "xogl", None, 0,
//                                0, NULL);

    glXMakeCurrent( display, glwin, cx );

    // Set up the default GL state.
    glClearColor(0, 0, 0, 1);
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_COLOR_ARRAY );

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0, 1024, 768, 0, -1, 1 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    // Hook up our window object to re-use its eventing capabilities
    m_window.SetDisplay( display );
    m_window.SetScreen( vi->screen );
    m_window.SetWindow( glwin );
    m_window.SetText( title );

    // cleanup
    ::XFree( vi );

    // Display the new window
    m_window.Show();
    m_window.SendToForegound();
    //m_window.GrabFocus();

    // Grab the main app pulse generator and set the update interval for the context to the desired refresh rate
    CORE::CPulseGenerator& pulseGenerator = CORE::CCoreGlobal::Instance()->GetPulseGenerator();
    pulseGenerator.RequestPeriodicPulses( this, 1000 / videoSettings.GetFrequency() );
    SubscribeTo( &pulseGenerator, CORE::CPulseGenerator::PulseEvent );

    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "CXWinGLWindowContext: Succesfully created OpenGL rendering context" );

    return true;
}

/*-------------------------------------------------------------------------*/

const CORE::CString&
CXWinGLWindowContext::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CORE::CString classTypeName = "GUCEF::GUIDRIVERXWINGL::CXWinGLWindowContext";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

void
CXWinGLWindowContext::OnNotify( CORE::CNotifier* notifier   ,
                                const CORE::CEvent& eventID ,
                                CORE::CICloneable* evenData )
{GUCEF_TRACE;

    if ( ( eventID == CORE::CPulseGenerator::PulseEvent )  ||
         ( eventID == CORE::CX11Window::WindowPaintEvent ) )
    {
        // Clear the screen in preparation for the redraw
        glClear( GL_COLOR_BUFFER_BIT );

        // Notify that we are going to redraw the window
        NotifyObservers( WindowContextRedrawEvent );

        // Swap our front and back buffers
        ::glXSwapBuffers( m_window.GetDisplay(), m_window.GetWindow() );
    }
    else
    if ( eventID == CORE::CX11Window::WindowResizeEvent )
    {
        NotifyObservers( WindowContextSizeEvent );
    }
    else
    if ( eventID == CORE::CX11Window::WindowActivationEvent )
    {
        NotifyObservers( WindowContextActivateEvent );
    }
}


/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace GUIDRIVERXWINGL */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
