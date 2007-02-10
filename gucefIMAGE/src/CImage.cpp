/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "CImage.h"       /* Header for this class */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace IMAGE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_IMPLEMENT_MSGEXCEPTION( CImage, EInvalidIndex );

/*--------------------------------------------------------------------------*/

CImage::CImage( void )
    : m_frameList()
{TRACE;

}

/*--------------------------------------------------------------------------*/

CImage::CImage( const TFrameList& frameList )
    : m_frameList( frameList )
{TRACE;

}

/*--------------------------------------------------------------------------*/
    
CImage::CImage( const TMipMapList& mipmapList )
    : m_frameList()
{TRACE;

    m_frameList.push_back( mipmapList );
}

/*--------------------------------------------------------------------------*/
    
CImage::CImage( const TPixelMapPtr& pixelMapPtr )
    : m_frameList()
{TRACE;

    TMipMapList mipmapList;
    mipmapList.push_back( pixelMapPtr );
    m_frameList.push_back( mipmapList );
}

/*--------------------------------------------------------------------------*/

void
CImage::Assign( const TFrameList& frameList )
{TRACE;

    // First we clean up our toys
    Clear();
    
    // Assign the image data
    m_frameList = frameList;
}

/*--------------------------------------------------------------------------*/
    
void
CImage::Assign( const TMipMapList& mipmapList )
{TRACE;

    // First we clean up our toys
    Clear();
    
    // Assign the image data
    m_frameList.push_back( mipmapList );
}

/*--------------------------------------------------------------------------*/
    
void
CImage::Assign( const TPixelMapPtr& pixelMapPtr )
{TRACE;

    // First we clean up our toys
    Clear();
    
    // Assign the image data
    TMipMapList mipmapList;
    mipmapList.push_back( pixelMapPtr );    
    m_frameList.push_back( mipmapList );
}

/*--------------------------------------------------------------------------*/

void
CImage::AddFrame( TMipMapList& imageFrame )
{TRACE;

    m_frameList.push_back( imageFrame );
}

/*--------------------------------------------------------------------------*/

void
CImage::AddFrame( TPixelMapPtr& imageFrame )
{TRACE;

    TMipMapList mipMapList;
    mipMapList.push_back( imageFrame );
    m_frameList.push_back( mipMapList );
}

/*--------------------------------------------------------------------------*/

void
CImage::SetFrame( TPixelMapPtr& imageFrame          ,
                  const UInt32 frameIndex /* = 0 */ )
{TRACE;

    TMipMapList mipMapList;
    mipMapList.push_back( imageFrame );
    SetFrame( mipMapList, frameIndex );    
}

/*--------------------------------------------------------------------------*/

void
CImage::SetFrame( TMipMapList& imageFrame           ,
                  const UInt32 frameIndex /* = 0 */ )
{TRACE;
    
    if ( frameIndex < m_frameList.size() )
    {
        m_frameList[ frameIndex ] = imageFrame;
    }
    else
    if ( frameIndex == m_frameList.size() )
    {
        m_frameList.push_back( imageFrame );
    }
    else
    {
        GUCEF_EMSGTHROW( EInvalidIndex, "CImage::SetFrame(): Invalid frame index" );
    }
}

/*--------------------------------------------------------------------------*/

CImage::~CImage()
{TRACE;
        Clear();
}

/*-------------------------------------------------------------------------*/

UInt32
CImage::GetFrameCount( void ) const
{TRACE;

    return static_cast< UInt32 >( m_frameList.size() );
}

/*-------------------------------------------------------------------------*/

UInt32
CImage::GetMipmapLevels( const UInt32 frameIndex /* = 0 */ ) const
{TRACE;

    if ( m_frameList.size() > frameIndex )
    {
        return static_cast< UInt32 >( m_frameList[ frameIndex ].size() );
    }
    
    GUCEF_EMSGTHROW( EInvalidIndex, "CImage::GetMipmapLevels(): Invalid frame index" );
}

/*-------------------------------------------------------------------------*/

void
CImage::Clear( void )
{
    // Because we are using shared pointers all we have to do is clear the list
    // this will cause things to be de-allocated as needed
    m_frameList.clear();
}

/*-------------------------------------------------------------------------*/

CImage::TPixelMapPtr
CImage::GetPixelMap( const UInt32 frameIndex /* = 0 */  ,
                     const UInt32 mipMapLevel /* = 0 */ )
{TRACE;

    if ( m_frameList.size() > frameIndex )
    {
        TMipMapList& mipmapList = m_frameList[ frameIndex ];
        if ( mipmapList.size() > mipMapLevel )
        {
            return mipmapList[ mipMapLevel ];
        }
    }
    
    GUCEF_EMSGTHROW( EInvalidIndex, "CImage::GetFrame(): Invalid frame or mipmap index" );    
}

/*-------------------------------------------------------------------------*/

CImage::TMipMapList&
CImage::GetFrame( const UInt32 frameIndex /* = 0 */ )
{TRACE;

    if ( m_frameList.size() > frameIndex )
    {
        return m_frameList[ frameIndex ];
    }
    
    GUCEF_EMSGTHROW( EInvalidIndex, "CImage::GetFrame(): Invalid frame or mipmap index" );    
}

/*-------------------------------------------------------------------------*/

const CImage::TMipMapList&
CImage::GetFrame( const UInt32 frameIndex /* = 0 */ ) const
{TRACE;

    if ( m_frameList.size() > frameIndex )
    {
        return m_frameList[ frameIndex ];
    }
    
    GUCEF_EMSGTHROW( EInvalidIndex, "CImage::GetFrame(): Invalid frame or mipmap index" );    
}

/*-------------------------------------------------------------------------*/

bool
CImage::HasFrames( void ) const
{TRACE;

    return !m_frameList.empty();
}

/*-------------------------------------------------------------------------*/

UInt32
CImage::GetTotalPixelStorageSize( void ) const
{TRACE;

    UInt32 totalBytes = 0;
    TFrameList::const_iterator i = m_frameList.begin();
    while ( i != m_frameList.end() )
    {
        const TMipMapList& mipmapList = (*i);
        TMipMapList::const_iterator n = mipmapList.begin();
        while ( n != mipmapList.end() )
        {
            totalBytes += (*n)->GetTotalSizeInBytes();
            ++n;
        }
        ++i;
    }
    
    return totalBytes;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace IMAGE */
} /* namespace GUCEF */

/*--------------------------------------------------------------------------*/




