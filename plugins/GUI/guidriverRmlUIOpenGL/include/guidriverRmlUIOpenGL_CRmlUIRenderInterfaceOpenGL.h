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

/*
 *  Based on Rocket code, the copyright below applies
 */

/*
 * This source file is part of libRocket, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://www.librocket.com
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#ifndef GUCEF_GUIDRIVERRMLUIGL_CRMLUIRENDERINTERFACEOPENGL_H
#define GUCEF_GUIDRIVERRMLUIGL_CRMLUIRENDERINTERFACEOPENGL_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <vector>

#ifndef RMLUICORERENDERINTERFACE_H
#include "RmlUi/Core/RenderInterface.h"
#define RMLUICORERENDERINTERFACE_H
#endif /* RMLUICORERENDERINTERFACE_H ? */

#ifndef RMLUICOREMATRIX4_H
#include "RmlUi/Core/Matrix4.h"
#define RMLUICOREMATRIX4_H
#endif /* RMLUICOREMATRIX4_H ? */

#ifndef GUCEF_GUIDRIVERRMLUIGL_MACROS_H
#include "guidriverRmlUIOpenGL_macros.h"
#define GUCEF_GUIDRIVERRMLUIGL_MACROS_H
#endif /* GUCEF_GUIDRIVERRMLUIGL_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUIDRIVERRMLUIGL {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/


/**
 *  Render interface of RmlUi which draws using the fixed function pipeline of OpenGL.
 *  What it expects of the application is that the viewport and the projection are set up before the GUI is rendered.
 *  RmlUi uses premultiplied alpha for everything it renders, the blend function is set for that when geometry is drawn.
 */
class GUIDRIVERRMLUIGL_PUBLIC_CPP CRmlUIRenderInterfaceOpenGL : public Rml::RenderInterface
{
    public:

	CRmlUIRenderInterfaceOpenGL( void );

    virtual ~CRmlUIRenderInterfaceOpenGL() GUCEF_VIRTUAL_OVERRIDE;

	/// Called by RmlUi when it wants to compile geometry to be rendered later, which is kept in memory
	virtual Rml::CompiledGeometryHandle CompileGeometry( Rml::Span< const Rml::Vertex > vertices ,
                                                         Rml::Span< const int > indices          ) GUCEF_VIRTUAL_OVERRIDE;

	/// Called by RmlUi when it wants to render compiled geometry
	virtual void RenderGeometry( Rml::CompiledGeometryHandle geometry ,
                                 Rml::Vector2f translation            ,
                                 Rml::TextureHandle texture           ) GUCEF_VIRTUAL_OVERRIDE;

	/// Called by RmlUi when it wants to release compiled geometry
	virtual void ReleaseGeometry( Rml::CompiledGeometryHandle geometry ) GUCEF_VIRTUAL_OVERRIDE;

	/// Called by RmlUi when a texture is required by the library, textures are loaded using the file interface of RmlUi
	virtual Rml::TextureHandle LoadTexture( Rml::Vector2i& texture_dimensions ,
                                            const Rml::String& source         ) GUCEF_VIRTUAL_OVERRIDE;

	/// Called by RmlUi when a texture is required to be built from an internally-generated sequence of pixels.
	virtual Rml::TextureHandle GenerateTexture( Rml::Span< const Rml::byte > source ,
                                                Rml::Vector2i source_dimensions     ) GUCEF_VIRTUAL_OVERRIDE;

	/// Called by RmlUi when a loaded texture is no longer required.
	virtual void ReleaseTexture( Rml::TextureHandle texture ) GUCEF_VIRTUAL_OVERRIDE;

	/// Called by RmlUi when it wants to enable or disable scissoring to clip content.
	virtual void EnableScissorRegion( bool enable ) GUCEF_VIRTUAL_OVERRIDE;

	/// Called by RmlUi when it wants to change the scissor region.
	virtual void SetScissorRegion( Rml::Rectanglei region ) GUCEF_VIRTUAL_OVERRIDE;

	/// Called by RmlUi when it wants the geometry to be transformed, as for elements with a CSS transform
	virtual void SetTransform( const Rml::Matrix4f* transform ) GUCEF_VIRTUAL_OVERRIDE;

    private:

    CRmlUIRenderInterfaceOpenGL( const CRmlUIRenderInterfaceOpenGL& src );
    CRmlUIRenderInterfaceOpenGL& operator=( const CRmlUIRenderInterfaceOpenGL& src );

    struct SCompiledGeometry
    {
        std::vector< Rml::Vertex > vertices;
        std::vector< int > indices;
    };

    private:

    bool m_hasTransform;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUIDRIVERRMLUIGL */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_GUIDRIVERRMLUIGL_CRMLUIRENDERINTERFACEOPENGL_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-08-2010 :
        - Dinand: Initial implementation

---------------------------------------------------------------------------*/
