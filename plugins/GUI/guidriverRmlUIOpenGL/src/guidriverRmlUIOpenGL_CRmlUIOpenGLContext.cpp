/*
 *  guidriverRmlUIOpenGL: GUI backend using RmlUi drawing with OpenGL 3.3, OpenGL ES 3 or WebGL 2
 *
 *  Copyright (C) 1998 - 2026.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#if defined( RMLUI_PLATFORM_EMSCRIPTEN ) || defined( __EMSCRIPTEN__ )
    #include <GLES3/gl3.h>
#elif defined( __ANDROID__ )
    #include <GLES3/gl32.h>
#else
    #include "RmlUi_Include_GL3.h"
#endif

#include <RmlUi/Core/Core.h>
#include "RmlUi_Renderer_GL3.h"

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#include "guidriverRmlUIOpenGL_CRmlUIOpenGLContext.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUIDRIVERRMLUIGL {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CRmlUIOpenGLContext::CRmlUIOpenGLContext( GUI::CGUIDriver* driver              ,
                                          GUI::TWindowContextPtr windowContext ,
                                          RenderInterface_GL3* renderer        )
    : guidriverRmlUI::CRmlUIContext( driver, windowContext, renderer )
    , m_renderer( renderer )
    , m_framebuffer( 0 )
    , m_colourBuffer( 0 )
    , m_width( 0 )
    , m_height( 0 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CRmlUIOpenGLContext::~CRmlUIOpenGLContext()
{GUCEF_TRACE;

    // RmlUi lets go of its geometry and textures through the renderer, which goes after it: first the context, then
    // what RmlUi keeps per renderer (it would otherwise use the deleted renderer when it shuts down)
    ReleaseRml();
    Rml::ReleaseRenderManagers();
    ReleaseFramebuffer();
    delete m_renderer;
    m_renderer = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

UInt32
CRmlUIOpenGLContext::GetWindowFramebuffer( void ) const
{GUCEF_TRACE;

    GUI::TWindowContextPtr window = GetWindowContext();
    if ( window.IsNULL() )
        return 0;
    return CORE::StringToUInt32( window->GetProperty( "GL_FRAMEBUFFER" ), 0 );
}

/*-------------------------------------------------------------------------*/

bool
CRmlUIOpenGLContext::PrepareFramebuffer( UInt32 width, UInt32 height )
{GUCEF_TRACE;

    if ( 0 != m_framebuffer && width == m_width && height == m_height )
        return true;
    ReleaseFramebuffer();

    GLuint framebuffer = 0, colour = 0;
    glGenFramebuffers( 1, &framebuffer );
    glBindFramebuffer( GL_FRAMEBUFFER, framebuffer );
    glGenRenderbuffers( 1, &colour );
    glBindRenderbuffer( GL_RENDERBUFFER, colour );
    glRenderbufferStorage( GL_RENDERBUFFER, GL_RGBA8, (GLsizei) width, (GLsizei) height );
    glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colour );
    glBindRenderbuffer( GL_RENDERBUFFER, 0 );
    m_framebuffer = framebuffer;
    m_colourBuffer = colour;

    if ( GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus( GL_FRAMEBUFFER ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "RmlUIOpenGLContext: The framebuffer of the picture is not complete" );
        ReleaseFramebuffer();
        return false;
    }
    m_width = width;
    m_height = height;
    return true;
}

/*-------------------------------------------------------------------------*/

void
CRmlUIOpenGLContext::ReleaseFramebuffer( void )
{GUCEF_TRACE;

    if ( 0 != m_framebuffer )
    {
        GLuint framebuffer = m_framebuffer;
        glDeleteFramebuffers( 1, &framebuffer );
    }
    if ( 0 != m_colourBuffer )
    {
        GLuint colour = m_colourBuffer;
        glDeleteRenderbuffers( 1, &colour );
    }
    m_framebuffer = m_colourBuffer = 0;
    m_width = m_height = 0;
}

/*-------------------------------------------------------------------------*/

bool
CRmlUIOpenGLContext::RenderPicture( void )
{GUCEF_TRACE;

    UInt32 width = GetLogicalWidth();
    UInt32 height = GetLogicalHeight();
    if ( GUCEF_NULL == m_renderer || 0 == width || 0 == height || !PrepareFramebuffer( width, height ) )
        return false;

    PrepareRml();

    // the picture starts opaque black, like the software renderer's
    glBindFramebuffer( GL_FRAMEBUFFER, m_framebuffer );
    glViewport( 0, 0, (GLsizei) width, (GLsizei) height );
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );

    m_renderer->SetViewport( (int) width, (int) height );
    m_renderer->SetTargetFramebuffer( m_framebuffer );
    m_renderer->BeginFrame();
    RenderRml();
    m_renderer->EndFrame();
    return true;
}

/*-------------------------------------------------------------------------*/

void
CRmlUIOpenGLContext::Present( void )
{GUCEF_TRACE;

    GUI::TWindowContextPtr window = GetWindowContext();
    if ( window.IsNULL() || 0 == m_framebuffer )
        return;
    Int32 windowWidth = (Int32) window->GetWidth();
    Int32 windowHeight = (Int32) window->GetHeight();
    if ( windowWidth <= 0 || windowHeight <= 0 )
        return;

    GLuint target = GetWindowFramebuffer();
    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, target );
    glViewport( 0, 0, windowWidth, windowHeight );
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );

    // the present rectangle has its top at the top of the window, GL counts from the bottom
    GUI::CRect area = GetPresentRect();
    GLenum filter = ( GUI::PRESENTMODE_LINEAR == GetPresentMode() || GUI::PRESENTMODE_STRETCH == GetPresentMode() ) ? GL_LINEAR : GL_NEAREST;
    glBindFramebuffer( GL_READ_FRAMEBUFFER, m_framebuffer );
    glBlitFramebuffer( 0, 0, (GLint) m_width, (GLint) m_height,
                       area.left, windowHeight - area.Bottom(), area.Right(), windowHeight - area.top,
                       GL_COLOR_BUFFER_BIT, filter );
    glBindFramebuffer( GL_FRAMEBUFFER, target );
}

/*-------------------------------------------------------------------------*/

void
CRmlUIOpenGLContext::OnRender( void )
{GUCEF_TRACE;

    if ( RenderPicture() )
        Present();
}

/*-------------------------------------------------------------------------*/

bool
CRmlUIOpenGLContext::CaptureFrame( IMAGE::TPixelMapPtr& pixels )
{GUCEF_TRACE;

    // the picture as it is now, without moving time on
    FlushChanges();
    if ( !RenderPicture() )
        return false;

    if ( pixels.IsNULL() || pixels->GetWidthInPixels() != m_width || pixels->GetHeightInPixels() != m_height ||
         IMAGE::PSF_RGBA != pixels->GetPixelStorageFormat() || GUCEF::DATATYPE_UINT8 != pixels->GetPixelComponentDataType() )
    {
        pixels = IMAGE::TPixelMapPtr( GUCEF_NEW IMAGE::CPixelMap( GUCEF_NULL, m_width, m_height, IMAGE::PSF_RGBA, GUCEF::DATATYPE_UINT8 ) );
    }

    glBindFramebuffer( GL_READ_FRAMEBUFFER, m_framebuffer );
    glPixelStorei( GL_PACK_ALIGNMENT, 1 );
    UInt8* data = static_cast< UInt8* >( pixels->GetDataPtr() );
    size_t rowSize = (size_t) m_width * 4;
    // GL rows go up from the bottom, the pixel map's down from the top
    for ( UInt32 y=0; y<m_height; ++y )
        glReadPixels( 0, (GLint) ( m_height - 1 - y ), (GLsizei) m_width, 1, GL_RGBA, GL_UNSIGNED_BYTE, data + y * rowSize );
    glBindFramebuffer( GL_FRAMEBUFFER, GetWindowFramebuffer() );
    return true;
}

/*-------------------------------------------------------------------------*/

const CORE::CString&
CRmlUIOpenGLContext::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CORE::CString classTypeName = "GUCEF::GUIDRIVERRMLUIGL::CRmlUIOpenGLContext";
    return classTypeName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUIDRIVERRMLUIGL */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
