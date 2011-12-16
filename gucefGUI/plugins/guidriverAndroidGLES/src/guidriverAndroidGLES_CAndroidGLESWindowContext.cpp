/*
 *  guidriverAndroidGLES: module implementing GLES based window management for Android
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

#include <GLES/gl.h>
#include <android_native_app_glue.h>

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_GUI_CWINDOWCONTEXT_H
#include "gucefGUI_CWindowContext.h"
#define GUCEF_GUI_CWINDOWCONTEXT_H
#endif /* GUCEF_GUI_CWINDOWCONTEXT_H ? */

#include "guidriverAndroidGLES_CAndroidGLESWindowContext.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUIDRIVERANDROIDGLES {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CAndroidGLESWindowContext::CAndroidGLESWindowContext( void )
    : CWindowContext()            ,
      m_display( EGL_NO_DISPLAY ) ,
      m_context( EGL_NO_CONTEXT ) ,
      m_surface( EGL_NO_SURFACE )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CAndroidGLESWindowContext::~CAndroidGLESWindowContext()
{GUCEF_TRACE;

    Shutdown();
}

/*-------------------------------------------------------------------------*/

GUI::UInt32
CAndroidGLESWindowContext::GetWidth( void ) const
{GUCEF_TRACE;

    EGLint width = 0;
    eglQuerySurface( m_display, m_surface, EGL_WIDTH, &width );
    return width;
}

/*-------------------------------------------------------------------------*/

GUI::UInt32
CAndroidGLESWindowContext::GetHeight( void ) const
{GUCEF_TRACE;

    EGLint height = 0;
    eglQuerySurface( m_display, m_surface, EGL_HEIGHT, &height );
    return height;
}

/*-------------------------------------------------------------------------*/

GUI::CString
CAndroidGLESWindowContext::GetProperty( const GUI::CString& propertyName ) const
{GUCEF_TRACE;

    if ( propertyName == "WINDOW" )
    {
        return CORE::PointerToString( m_display );
    }

    return GUI::CString();
}

/*-------------------------------------------------------------------------*/

void
CAndroidGLESWindowContext::SetGuiContext( GUI::TGuiContextPtr& context )
{GUCEF_TRACE;

    m_guiContext = context;
}

/*-------------------------------------------------------------------------*/

GUI::TGuiContextPtr
CAndroidGLESWindowContext::GetGuiContext( void )
{GUCEF_TRACE;

    return m_guiContext;
}

/*-------------------------------------------------------------------------*/

GUI::UInt32
CAndroidGLESWindowContext::GetID( void ) const
{GUCEF_TRACE;

    return m_id;
}

/*-------------------------------------------------------------------------*/

bool
CAndroidGLESWindowContext::IsActive( void ) const
{GUCEF_TRACE;

    if ( EGL_NO_CONTEXT != m_context )
    {
        return m_context == eglGetCurrentContext();
    }
    return false;
}

/*-------------------------------------------------------------------------*/

GUI::CString
CAndroidGLESWindowContext::GetName( void ) const
{GUCEF_TRACE;

    return m_name;
}

/*-------------------------------------------------------------------------*/

void
CAndroidGLESWindowContext::Shutdown( void )
{GUCEF_TRACE;

    if ( m_display != EGL_NO_DISPLAY )
    {
        eglMakeCurrent( m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
        if ( m_context != EGL_NO_CONTEXT )
        {
            eglDestroyContext( m_display, m_context );
        }
        if ( m_surface != EGL_NO_SURFACE )
        {
            eglDestroySurface( m_display, m_surface );
        }
        eglTerminate( m_display );
    }
    m_display = EGL_NO_DISPLAY;
    m_context = EGL_NO_CONTEXT;
    m_surface = EGL_NO_SURFACE;
}

/*-------------------------------------------------------------------------*/

bool
CAndroidGLESWindowContext::Initialize( const GUI::CVideoSettings& videoSettings )
{GUCEF_TRACE;

    // Do not initialize twice
    Shutdown();

    // @TODO: get the app from somewhere
    struct android_app* app = NULL;
    EGLint bitDepth = (EGLint) videoSettings.GetResolutionDepthInBits() / 3;

    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least <bitDepth> bits per color
     * component compatible with on-screen windows
     *  if <bitDepth> is 0 then the smallest available size is used.
     */
    const EGLint attribs[] =
    {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, bitDepth,
            EGL_GREEN_SIZE, bitDepth,
            EGL_RED_SIZE, bitDepth,
            EGL_NONE
    };
    EGLint w, h, format;
    EGLint numConfigs;
    EGLConfig config;

    m_display = eglGetDisplay( EGL_DEFAULT_DISPLAY );

    eglInitialize( m_display, 0, 0 );

    /* Here, the application chooses the configuration it desires. In this
     * sample, we have a very simplified selection process, where we pick
     * the first EGLConfig that matches our criteria */
    eglChooseConfig( m_display, attribs, &config, 1, &numConfigs );

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib( m_display, config, EGL_NATIVE_VISUAL_ID, &format );

    ANativeWindow_setBuffersGeometry( app->window, 0, 0, format );

    m_surface = eglCreateWindowSurface( m_display, config, app->window, NULL );
    m_context = eglCreateContext( m_display, config, NULL, NULL );

    if ( eglMakeCurrent( m_display, m_surface, m_surface, m_context ) == EGL_FALSE )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "eglMakeCurrent failed" );
        return false;
    }

    eglQuerySurface( m_display, m_surface, EGL_WIDTH, &w );
    eglQuerySurface( m_display, m_surface, EGL_HEIGHT, &h );

    //engine->width = w;
    //engine->height = h;

    // Initialize GL state.
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST );
    glEnable( GL_CULL_FACE );
    glShadeModel( GL_SMOOTH );
    glDisable( GL_DEPTH_TEST );

    return true;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace GUIDRIVERANDROIDGLES */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
