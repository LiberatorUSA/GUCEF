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

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#ifndef GUCEF_CORE_CX11EVENTDISPATCHER_H
#include "gucefCORE_CX11EventDispatcher.h"
#define GUCEF_CORE_CX11EVENTDISPATCHER_H
#endif /* GUCEF_CORE_CX11EVENTDISPATCHER_H ? */

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

/** glXGetProcAddressARB as the GL_GETPROCADDRESS property hands it out */
static void*
GetGlProcAddress( const char* name )
{
    return (void*) glXGetProcAddressARB( (const GLubyte*) name );
}

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
      m_window()               ,
      m_renderContext( NULL )  ,
      m_isCoreProfile( false ) ,
      m_shown( false )
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

	CORE::PulseGeneratorPtr pulseGenerator = CORE::CCoreGlobal::Instance()->GetPulseGenerator();
    pulseGenerator->RequestStopOfPeriodicUpdates( this );
    UnsubscribeFrom( pulseGenerator.GetPointerAlways() );

    if ( NULL != m_renderContext )
    {
        ::Display* display = CORE::CX11EventDispatcher::Instance()->GetDisplay();
        if ( NULL != display )
        {
            glXMakeCurrent( display, None, NULL );
            glXDestroyContext( display, (GLXContext) m_renderContext );
        }
        m_renderContext = NULL;
    }
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
    if ( propertyName == "GL_GETPROCADDRESS" )
    {
        return CORE::PointerToString( (const void*) &GetGlProcAddress );
    }
    if ( propertyName == "GL_FRAMEBUFFER" )
    {
        return "0";
    }
    if ( propertyName == "GL_API" )
    {
        return "GL";
    }
    if ( propertyName == "GL_VERSION" )
    {
        return m_isCoreProfile ? "3.3" : "1.x";
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
    ::Display* display = CORE::CX11EventDispatcher::Instance()->GetDisplay();
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

    // a double buffered RGBA configuration with a stencil buffer (the GUI renderers clip with it)
    static const int frameBufferAttributes[] = { GLX_X_RENDERABLE, True, GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT, GLX_RENDER_TYPE, GLX_RGBA_BIT,
                                                 GLX_RED_SIZE, 8, GLX_GREEN_SIZE, 8, GLX_BLUE_SIZE, 8, GLX_ALPHA_SIZE, 8,
                                                 GLX_STENCIL_SIZE, 8, GLX_DOUBLEBUFFER, True, None };
    XVisualInfo* vi = NULL;
    GLXContext cx = NULL;
    int configCount = 0;
    GLXFBConfig* configs = glXChooseFBConfig( display, DefaultScreen( display ), frameBufferAttributes, &configCount );
    if ( NULL != configs && configCount > 0 )
    {
        vi = glXGetVisualFromFBConfig( display, configs[ 0 ] );

        // OpenGL 3.3 core, what the renderers of the GUI drivers draw with
        typedef GLXContext ( *TCreateContextAttribs )( ::Display*, GLXFBConfig, GLXContext, Bool, const int* );
        TCreateContextAttribs createContextAttribs = (TCreateContextAttribs) glXGetProcAddressARB( (const GLubyte*) "glXCreateContextAttribsARB" );
        if ( NULL != createContextAttribs && NULL != vi )
        {
            static const int contextAttributes[] = { GLX_CONTEXT_MAJOR_VERSION_ARB, 3, GLX_CONTEXT_MINOR_VERSION_ARB, 3,
                                                     GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB, None };
            cx = createContextAttribs( display, configs[ 0 ], NULL, True, contextAttributes );
        }
        ::XFree( configs );
    }
    m_isCoreProfile = NULL != cx;

    if ( NULL == cx )
    {
        // an older driver: a legacy context on any double buffered RGBA visual
        if ( NULL == vi )
        {
            static int attributes[] = { GLX_RGBA, GLX_DOUBLEBUFFER, None };
            vi = glXChooseVisual( display, DefaultScreen( display ), attributes );
        }
        if ( NULL == vi )
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CXWinGLWindowContext: Could not get visual info for display" );
            Shutdown();
            return false;
        }
        cx = glXCreateContext( display, vi, None, GL_TRUE );
    }
    if ( NULL == cx )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CXWinGLWindowContext: Could not create rendering context" );
        XFree( vi );
        Shutdown();
        return false;
    }
    m_renderContext = cx;

    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, CORE::CString( "CXWinGLWindowContext: Created " ) + ( m_isCoreProfile ? "an OpenGL 3.3 core" : "a legacy OpenGL" ) + " context using GLX" );

    // create an X colormap since probably not using default visual
    ::Colormap cmap = ::XCreateColormap( display, RootWindow( display, vi->screen),
                                vi->visual, AllocNone);
    ::XSetWindowAttributes swa;
    swa.colormap = cmap;
    swa.border_pixel = 0;
    swa.event_mask = ExposureMask | EnterWindowMask | StructureNotifyMask | SubstructureNotifyMask;
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

    glXMakeCurrent( display, glwin, cx );

    // frames are paced by the pulse generator; waiting for the vertical blank is what the settings ask.
    // (A window the compositor does not show never gets its vertical blank under XWayland: the swap would wait forever)
    const int swapInterval = videoSettings.GetVSyncState() ? 1 : 0;
    typedef void ( *TSwapIntervalEXT )( ::Display*, GLXDrawable, int );
    typedef int ( *TSwapInterval )( int );
    TSwapIntervalEXT swapIntervalEXT = (TSwapIntervalEXT) glXGetProcAddressARB( (const GLubyte*) "glXSwapIntervalEXT" );
    TSwapInterval swapIntervalMESA = (TSwapInterval) glXGetProcAddressARB( (const GLubyte*) "glXSwapIntervalMESA" );
    TSwapInterval swapIntervalSGI = (TSwapInterval) glXGetProcAddressARB( (const GLubyte*) "glXSwapIntervalSGI" );
    if ( NULL != swapIntervalEXT )
        swapIntervalEXT( display, glwin, swapInterval );
    else
    if ( NULL != swapIntervalMESA )
        swapIntervalMESA( swapInterval );
    else
    if ( NULL != swapIntervalSGI && swapInterval > 0 )
        swapIntervalSGI( swapInterval );

    // Set up the default GL state.
    glViewport( 0, 0, (GLsizei) videoSettings.GetResolutionWidthInPixels(), (GLsizei) videoSettings.GetResolutionHeightInPixels() );
    glClearColor(0, 0, 0, 1);
    if ( !m_isCoreProfile )
    {
        // the fixed function pipeline draws in window pixels
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        glOrtho( 0, videoSettings.GetResolutionWidthInPixels(), videoSettings.GetResolutionHeightInPixels(), 0, -1, 1 );
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
    }

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
    ::XFlush( display );
    if ( videoSettings.GetFullscreenState() )
        m_window.SetFullscreenStatus( true );
    //m_window.GrabFocus();

    // Grab the main app pulse generator and set the update interval for the context to the desired refresh rate
    CORE::PulseGeneratorPtr pulseGenerator = CORE::CCoreGlobal::Instance()->GetPulseGenerator();
    pulseGenerator->RequestPeriodicPulses( this, 1000 / videoSettings.GetFrequency() );
    SubscribeTo( pulseGenerator.GetPointerAlways(), CORE::CPulseGenerator::PulseEvent );

    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "CXWinGLWindowContext: Succesfully created OpenGL rendering context" );

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CXWinGLWindowContext::SetSystemCursorVisible( bool visible )
{GUCEF_TRACE;

    if ( 0 == m_window.GetWindow() )
        return false;
    m_window.SetCursorVisible( visible );
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CXWinGLWindowContext::ApplyVideoSettings( const GUI::CVideoSettings& videoSettings )
{GUCEF_TRACE;

    if ( 0 == m_window.GetWindow() )
        return false;
    m_window.SetFullscreenStatus( videoSettings.GetFullscreenState() );
    if ( !videoSettings.GetFullscreenState() && videoSettings.GetResolutionWidthInPixels() > 0 && videoSettings.GetResolutionHeightInPixels() > 0 )
        m_window.ClientAreaResize( (int) videoSettings.GetResolutionWidthInPixels(), (int) videoSettings.GetResolutionHeightInPixels() );
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

    if ( eventID == CORE::CX11Window::WindowPaintEvent )
        m_shown = true;

    if ( ( eventID == CORE::CPulseGenerator::PulseEvent && m_shown ) ||
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
        int dummy, width, height;
        if ( m_window.GetClientArea( dummy, dummy, width, height ) )
        {
            glViewport( 0, 0, width, height );
            if ( !m_isCoreProfile )
            {
                glMatrixMode( GL_PROJECTION );
                glLoadIdentity();
                glOrtho( 0, width, height, 0, -1, 1 );
                glMatrixMode( GL_MODELVIEW );
            }
        }
        NotifyObservers( WindowContextSizeEvent );
    }
    else
    if ( eventID == CORE::CX11Window::WindowCloseEvent )
    {
        NotifyObservers( WindowContextCloseRequestEvent );
    }
    else
    if ( eventID == CORE::CX11Window::WindowActivationEvent )
    {
        m_shown = true;
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
