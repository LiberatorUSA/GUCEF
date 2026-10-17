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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <string.h>
#include <vector>
#include <type_traits>

#include <RmlUi/Core.h>
#include "guidriverRmlUIOpenGL_CRmlUIRenderInterfaceOpenGL.h"

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
#include <windows.h>
#endif

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID )
  #include <GLES/gl.h>
  /* This isn't defined in the GLES headers */
  #ifndef GL_UNSIGNED_INT
  #define GL_UNSIGNED_INT 0x1405
  #endif
  #ifndef GL_RGB8
  #define GL_RGB8 GL_RGB
  #endif
  #ifndef GL_RGBA8
  #define GL_RGBA8 GL_RGBA
  #endif
#else
  #include <GL/gl.h>
#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUIDRIVERRMLUIGL {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#define GL_CLAMP_TO_EDGE 0x812F

CRmlUIRenderInterfaceOpenGL::CRmlUIRenderInterfaceOpenGL()
    : Rml::RenderInterface()
    , m_hasTransform( false )
{
}

/*-------------------------------------------------------------------------*/

CRmlUIRenderInterfaceOpenGL::~CRmlUIRenderInterfaceOpenGL()
{
}

/*-------------------------------------------------------------------------*/

Rml::CompiledGeometryHandle
CRmlUIRenderInterfaceOpenGL::CompileGeometry( Rml::Span< const Rml::Vertex > vertices ,
                                              Rml::Span< const int > indices          )
{
    // RmlUi guarantees the data to stay valid until the geometry is released but we render from a copy,
    // which also makes the geometry independent of how RmlUi manages its memory
    SCompiledGeometry* geometry = new SCompiledGeometry();
    geometry->vertices.assign( vertices.begin(), vertices.begin() + vertices.size() );
    geometry->indices.assign( indices.begin(), indices.begin() + indices.size() );
    return (Rml::CompiledGeometryHandle) geometry;
}

/*-------------------------------------------------------------------------*/

void
CRmlUIRenderInterfaceOpenGL::RenderGeometry( Rml::CompiledGeometryHandle geometryHandle ,
                                             Rml::Vector2f translation                  ,
                                             Rml::TextureHandle texture                 )
{
    const SCompiledGeometry* geometry = reinterpret_cast< const SCompiledGeometry* >( geometryHandle );
    if ( NULL == geometry || geometry->vertices.empty() || geometry->indices.empty() )
        return;

    const Rml::Vertex* vertices = &geometry->vertices[ 0 ];
    const int* indices = &geometry->indices[ 0 ];
    const int num_indices = (int) geometry->indices.size();

    // RmlUi renders with premultiplied alpha
    glEnable( GL_BLEND );
    glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_COLOR_ARRAY );

	glPushMatrix();
	glTranslatef( translation.x, translation.y, 0 );

	glVertexPointer( 2, GL_FLOAT, sizeof( Rml::Vertex ), &vertices[ 0 ].position );
	glColorPointer( 4, GL_UNSIGNED_BYTE, sizeof( Rml::Vertex ), &vertices[ 0 ].colour );

	if ( !texture )
	{
		glDisable( GL_TEXTURE_2D );
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	}
	else
	{
		glEnable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, (GLuint) texture );
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		glTexCoordPointer( 2, GL_FLOAT, sizeof( Rml::Vertex ), &vertices[ 0 ].tex_coord );
	}

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID )

    // GL_UNSIGNED_INT is not supported in GLES so we have to convert to GL_UNSIGNED_SHORT
    std::vector< unsigned short > shortIndices( num_indices );
    for ( int i=0; i<num_indices; ++i )
        shortIndices[ i ] = (unsigned short) indices[ i ];

	glDrawElements( GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, &shortIndices[ 0 ] );

    #else

    glDrawElements( GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, indices );

    #endif

	glPopMatrix();
}

/*-------------------------------------------------------------------------*/

void
CRmlUIRenderInterfaceOpenGL::ReleaseGeometry( Rml::CompiledGeometryHandle geometryHandle )
{
    delete reinterpret_cast< SCompiledGeometry* >( geometryHandle );
}

/*-------------------------------------------------------------------------*/

void
CRmlUIRenderInterfaceOpenGL::EnableScissorRegion( bool enable )
{
	if ( enable )
		glEnable( GL_SCISSOR_TEST );
	else
		glDisable( GL_SCISSOR_TEST );
}

/*-------------------------------------------------------------------------*/

void
CRmlUIRenderInterfaceOpenGL::SetScissorRegion( Rml::Rectanglei region )
{
    // The region is given with the origin at the top left, OpenGL has it at the bottom left of the viewport
    GLint viewport[ 4 ] = { 0, 0, 0, 0 };
    glGetIntegerv( GL_VIEWPORT, viewport );
    glScissor( region.Left(), viewport[ 3 ] - region.Bottom(), region.Width(), region.Height() );
}

/*-------------------------------------------------------------------------*/

void
CRmlUIRenderInterfaceOpenGL::SetTransform( const Rml::Matrix4f* transform )
{
    m_hasTransform = NULL != transform;
    glMatrixMode( GL_MODELVIEW );
    if ( NULL != transform )
    {
        // OpenGL wants its matrices in column major order
        if ( std::is_same< Rml::Matrix4f, Rml::ColumnMajorMatrix4f >::value )
            glLoadMatrixf( transform->data() );
        else
            glLoadMatrixf( transform->Transpose().data() );
    }
    else
    {
        glLoadIdentity();
    }
}

/*-------------------------------------------------------------------------*/

// Set to byte packing, or the compiler will expand our struct, which means it won't read correctly from file
#pragma pack(1)
struct TGAHeader
{
	char  idLength;
	char  colourMapType;
	char  dataType;
	short int colourMapOrigin;
	short int colourMapLength;
	char  colourMapDepth;
	short int xOrigin;
	short int yOrigin;
	short int width;
	short int height;
	char  bitsPerPixel;
	char  imageDescriptor;
};
// Restore packing
#pragma pack()

/*-------------------------------------------------------------------------*/

Rml::TextureHandle
CRmlUIRenderInterfaceOpenGL::LoadTexture( Rml::Vector2i& texture_dimensions ,
                                          const Rml::String& source         )
{
	Rml::FileInterface* file_interface = Rml::GetFileInterface();
	Rml::FileHandle file_handle = file_interface->Open( source );
	if ( !file_handle )
	{
		return 0;
	}

	file_interface->Seek( file_handle, 0, SEEK_END );
	size_t buffer_size = file_interface->Tell( file_handle );
	file_interface->Seek( file_handle, 0, SEEK_SET );

    if ( buffer_size < sizeof( TGAHeader ) )
    {
        file_interface->Close( file_handle );
		Rml::Log::Message( Rml::Log::LT_ERROR, "The texture is too small to be a TGA." );
        return 0;
    }

    std::vector< char > buffer( buffer_size );
	file_interface->Read( &buffer[ 0 ], buffer_size, file_handle );
	file_interface->Close( file_handle );

	TGAHeader header;
	memcpy( &header, &buffer[ 0 ], sizeof( TGAHeader ) );

	int color_mode = header.bitsPerPixel / 8;
	size_t image_size = (size_t) header.width * header.height * 4; // We always make 32bit textures

	if ( header.dataType != 2 )
	{
		Rml::Log::Message( Rml::Log::LT_ERROR, "Only 24/32bit uncompressed TGAs are supported." );
		return 0;
	}

	// Ensure we have at least 3 colors
	if ( color_mode < 3 )
	{
		Rml::Log::Message( Rml::Log::LT_ERROR, "Only 24 and 32bit textures are supported" );
		return 0;
	}

    if ( buffer_size < sizeof( TGAHeader ) + (size_t) header.width * header.height * color_mode )
    {
		Rml::Log::Message( Rml::Log::LT_ERROR, "The TGA does not hold all the pixels it states." );
        return 0;
    }

	const char* image_src = &buffer[ 0 ] + sizeof( TGAHeader );
    std::vector< Rml::byte > image_dest( image_size );

	// Targa is BGR, swap to RGB and flip Y axis, RmlUi wants the colors premultiplied by the alpha
	for ( long y = 0; y < header.height; y++ )
	{
		long read_index = y * header.width * color_mode;
		long write_index = ((header.imageDescriptor & 32) != 0) ? y * header.width * 4 : (header.height - y - 1) * header.width * 4;
		for ( long x = 0; x < header.width; x++ )
		{
            unsigned int alpha = color_mode == 4 ? (unsigned char) image_src[ read_index+3 ] : 255;
			image_dest[ write_index ] = (Rml::byte) ( ( (unsigned char) image_src[ read_index+2 ] * alpha ) / 255 );
			image_dest[ write_index+1 ] = (Rml::byte) ( ( (unsigned char) image_src[ read_index+1 ] * alpha ) / 255 );
			image_dest[ write_index+2 ] = (Rml::byte) ( ( (unsigned char) image_src[ read_index ] * alpha ) / 255 );
			image_dest[ write_index+3 ] = (Rml::byte) alpha;

			write_index += 4;
			read_index += color_mode;
		}
	}

	texture_dimensions.x = header.width;
	texture_dimensions.y = header.height;

	return GenerateTexture( Rml::Span< const Rml::byte >( &image_dest[ 0 ], image_dest.size() ), texture_dimensions );
}

/*-------------------------------------------------------------------------*/

Rml::TextureHandle
CRmlUIRenderInterfaceOpenGL::GenerateTexture( Rml::Span< const Rml::byte > source ,
                                              Rml::Vector2i source_dimensions     )
{
    if ( NULL == source.data() || source.size() != (size_t) source_dimensions.x * source_dimensions.y * 4 )
    {
		Rml::Log::Message( Rml::Log::LT_ERROR, "The data to generate a texture from does not match its dimensions" );
        return 0;
    }

	GLuint texture_id = 0;
	glGenTextures( 1, &texture_id );
	if ( texture_id == 0 )
	{
		Rml::Log::Message( Rml::Log::LT_ERROR, "Failed to generate textures" );
		return 0;
	}

	glBindTexture( GL_TEXTURE_2D, texture_id );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, source_dimensions.x, source_dimensions.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, source.data() );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	return (Rml::TextureHandle) texture_id;
}

/*-------------------------------------------------------------------------*/

void
CRmlUIRenderInterfaceOpenGL::ReleaseTexture( Rml::TextureHandle texture )
{
    GLuint texture_id = (GLuint) texture;
	glDeleteTextures( 1, &texture_id );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUIDRIVERRMLUIGL */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
