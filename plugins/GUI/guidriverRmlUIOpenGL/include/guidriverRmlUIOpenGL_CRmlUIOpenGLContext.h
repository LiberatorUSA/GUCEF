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

#ifndef GUCEF_GUIDRIVERRMLUIGL_CRMLUIOPENGLCONTEXT_H
#define GUCEF_GUIDRIVERRMLUIGL_CRMLUIOPENGLCONTEXT_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_GUIDRIVERRMLUI_CRMLUICONTEXT_H
#include "guidriverRmlUI_CRmlUIContext.h"
#define GUCEF_GUIDRIVERRMLUI_CRMLUICONTEXT_H
#endif /* GUCEF_GUIDRIVERRMLUI_CRMLUICONTEXT_H ? */

#ifndef GUCEF_GUIDRIVERRMLUIGL_MACROS_H
#include "guidriverRmlUIOpenGL_macros.h"
#define GUCEF_GUIDRIVERRMLUIGL_MACROS_H
#endif /* GUCEF_GUIDRIVERRMLUIGL_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

class RenderInterface_GL3;

namespace GUCEF {
namespace GUIDRIVERRMLUIGL {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  An RmlUi context drawn with OpenGL in a window's GL context.
 *
 *  A frame is drawn at the logical resolution into a framebuffer of its own, then shown in the window
 *  at the present rectangle with the present mode (nearest or linear scaling, black around it). The
 *  window's picture is framebuffer 0, or the one its GL_FRAMEBUFFER property names (offscreen windows).
 *  CaptureFrame() reads the picture at the logical resolution back.
 */
class GUIDRIVERRMLUIGL_PUBLIC_CPP CRmlUIOpenGLContext : public guidriverRmlUI::CRmlUIContext
{
    public:

    /** Takes the renderer, which must have been made in the window's GL context */
    CRmlUIOpenGLContext( GUI::CGUIDriver* driver              ,
                         GUI::TWindowContextPtr windowContext ,
                         RenderInterface_GL3* renderer        );

    virtual ~CRmlUIOpenGLContext();

    virtual bool CaptureFrame( IMAGE::TPixelMapPtr& pixels ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CORE::CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    protected:

    virtual void OnRender( void ) GUCEF_VIRTUAL_OVERRIDE;

    private:

    /** Draws the documents into the framebuffer of the logical picture */
    bool RenderPicture( void );

    /** Shows the logical picture in the window */
    void Present( void );

    bool PrepareFramebuffer( UInt32 width, UInt32 height );

    void ReleaseFramebuffer( void );

    UInt32 GetWindowFramebuffer( void ) const;

    CRmlUIOpenGLContext( const CRmlUIOpenGLContext& src );
    CRmlUIOpenGLContext& operator=( const CRmlUIOpenGLContext& src );

    private:

    RenderInterface_GL3* m_renderer;
    UInt32 m_framebuffer;
    UInt32 m_colourBuffer;
    UInt32 m_width;
    UInt32 m_height;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUIDRIVERRMLUIGL */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_GUIDRIVERRMLUIGL_CRMLUIOPENGLCONTEXT_H ? */
