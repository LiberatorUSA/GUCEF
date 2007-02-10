/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
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

#ifndef GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#include "CDynamicBufferAccess.h"
#define GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#endif /* GUCEF_CORE_CDYNAMICBUFFERACCESS_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_CORE_CIOACCESS_H
#include "CIOAccess.h"
#define GUCEF_CORE_CIOACCESS_H
#endif /* GUCEF_CORE_CIOACCESS_H ? */

#ifndef GUCEF_IMAGE_CIMAGE_H
#include "CImage.h"
#define GUCEF_IMAGE_CIMAGE_H
#endif /* GUCEF_IMAGE_CIMAGE_H ? */

#include "CIMGCodec.h"

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

GUCEF_IMPLEMENT_MSGEXCEPTION( CIMGCodec, EInvalidCodec );

/*-------------------------------------------------------------------------*/

CIMGCodec::CIMGCodec( const CCodecPtr& codecPtr )
    : m_codecPtr( codecPtr )
{TRACE;

    if ( m_codecPtr->GetFamilyName() != "ImageCodec" )
    {
        m_codecPtr = NULL;
        GUCEF_EMSGTHROW( EInvalidCodec, "CIMGCodec::CIMGCodec(): Invalid codec type given" );
    }
}

/*-------------------------------------------------------------------------*/
    
CIMGCodec::CIMGCodec( const CIMGCodec& src )
    : m_codecPtr( src.m_codecPtr )
{TRACE;
}

/*-------------------------------------------------------------------------*/    

CIMGCodec::~CIMGCodec()
{TRACE;
}

/*-------------------------------------------------------------------------*/
    
CIMGCodec&
CIMGCodec::operator=( const CIMGCodec& src )
{TRACE;

    if ( &src != this )
    {        
        m_codecPtr = src.m_codecPtr;
    }
    
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
CIMGCodec::Encode( CORE::CIOAccess& source ,
                   CORE::CIOAccess& dest   )
{TRACE;

    return m_codecPtr->Encode( source ,
                               dest   );
}

/*-------------------------------------------------------------------------*/

bool
CIMGCodec::Decode( CORE::CIOAccess& source ,
                   CORE::CIOAccess& dest   )
{TRACE;

    return m_codecPtr->Decode( source ,
                               dest   );
}

/*-------------------------------------------------------------------------*/

bool
CIMGCodec::Encode( const CImage& inputImage       ,
                   CORE::CIOAccess& encodedOutput )
{TRACE;

    if ( inputImage.HasFrames() )
    {    
        CORE::CDynamicBuffer inputBuffer( 102400, true );
        CORE::CDynamicBufferAccess bufferAccess( inputBuffer );
        
        // Fill our header
        TImageInfo imageInfo;
        imageInfo.version = GUCEF_IMAGE_TIMAGEINFO_VERSION;
        imageInfo.nrOfFramesInImage = inputImage.GetFrameCount();

        // write header
        bufferAccess.Write( &imageInfo, sizeof( TImageInfo ), 1 );
        
        // Now we add each frame's info + mipmap info
        TImageFrameInfo frameInfo;
        frameInfo.version = GUCEF_IMAGE_TIMAGEFRAMEINFO_VERSION;
        TImageMipMapLevelInfo mipMapInfo;
        mipMapInfo.version = GUCEF_IMAGE_TIMAGEMIPMAPLEVELINFO_VERSION;
        const CImage::TMipMapList* mipMapList = NULL;
        const CImage::TPixelMapPtr* pixelMap = NULL;
        for ( UInt32 frameNr=0; frameNr<imageInfo.nrOfFramesInImage; ++frameNr )
        {   
            mipMapList = &inputImage.GetFrame( frameNr );
            frameInfo.nrOfMipmapLevels = static_cast< UInt32 >( mipMapList->size() );
            bufferAccess.Write( &frameInfo, sizeof( TImageFrameInfo ), 1 );
            
            // Now we add the info for each mipmap level
            for ( UInt32 mipLvl=0; mipLvl<frameInfo.nrOfMipmapLevels; ++mipLvl )
            {
                pixelMap = &(*mipMapList)[ mipLvl ];
                mipMapInfo.frameWidth = (*pixelMap)->GetWidthInPixels();
                mipMapInfo.frameHeight = (*pixelMap)->GetHeightInPixels();
                mipMapInfo.pixelStorageFormat = (*pixelMap)->GetPixelStorageFormat();
                mipMapInfo.pixelComponentDataType = (*pixelMap)->GetPixelComponentDataType();
                mipMapInfo.channelCountPerPixel = (*pixelMap)->GetNumberOfChannelsPerPixel();
                mipMapInfo.channelComponentSize = (*pixelMap)->GetSizeOfPixelComponentInBytes();
                
                bufferAccess.Write( &mipMapInfo, sizeof( TImageMipMapLevelInfo ), 1 );
            }
        }
        
        // Now it is time to add the pixel data itself
        for ( UInt32 frameNr=0; frameNr<imageInfo.nrOfFramesInImage; ++frameNr )
        {
            mipMapList = &inputImage.GetFrame( frameNr );
            for ( UInt32 mipLvl=0; mipLvl<mipMapList->size(); ++mipLvl )
            {
                pixelMap = &(*mipMapList)[ mipLvl ];
                bufferAccess.Write( (*pixelMap)->GetDataPtr(), (*pixelMap)->GetTotalSizeInBytes(), 1 );
            }
        }
        
        // We have now merged all the image object data into a single buffer using the decoded 'ImageCodec' format
        // It is time to perform the actual Encode()   
        return Encode( bufferAccess  ,
                       encodedOutput );
    }

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CIMGCodec::Decode( CORE::CIOAccess& encodedInput ,
                   CImage& outputImage           )
{TRACE;

    CORE::CDynamicBuffer outputBuffer( 102400, true );
    CORE::CDynamicBufferAccess bufferAccess( outputBuffer );
    
    if ( Decode( encodedInput  ,
                 bufferAccess  ) )
    {
        UInt32 bytesWritten = 0;
        bufferAccess.Setpos( 0 );
        
        // Fill our header from the buffer
        TImageInfo imageInfo;
        bytesWritten = bufferAccess.Read( &imageInfo, sizeof( TImageInfo ), 1 );
        if ( bytesWritten !=  sizeof( TImageInfo ) ) return false;
        
        // Check the header version for compatibility
        if ( imageInfo.version != GUCEF_IMAGE_TIMAGEINFO_VERSION ) return false;
        
        // Check if we are finished already
        if ( imageInfo.nrOfFramesInImage == 0 ) return true;
        
        // Process the image frames
        TImageFrame* imageFrameInfo = new TImageFrame[ imageInfo.nrOfFramesInImage ]; 
        for ( UInt32 i=0; i<imageInfo.nrOfFramesInImage; ++i )
        {            
            // Read the frame info
            TImageFrameInfo* currentFrame = &imageFrameInfo[ i ].frameInfo;
            bytesWritten = bufferAccess.Read( currentFrame, sizeof( TImageFrameInfo ), 1 );
            
            // Sanity check 
            if ( ( bytesWritten != sizeof( TImageFrameInfo ) )                   ||
                 ( currentFrame->version != GUCEF_IMAGE_TIMAGEFRAMEINFO_VERSION )  )
            {
                delete []imageFrameInfo;
                return false;
            
                imageFrameInfo[ i ].mipmapLevel = new TImageMipMapLevel[ currentFrame->nrOfMipmapLevels ];
                for ( UInt32 n=0; n<currentFrame->nrOfMipmapLevels; ++n )
                {
                    // Read the frame's mipmap info
                    TImageMipMapLevelInfo* mipMapLevel = &imageFrameInfo[ i ].mipmapLevel[ n ].mipLevelInfo;
                    bytesWritten = bufferAccess.Read( mipMapLevel, sizeof( TImageMipMapLevelInfo ), 1 );
                    
                    // Sanity check
                    if ( ( bytesWritten != sizeof( TImageMipMapLevelInfo ) )                   ||
                         ( mipMapLevel->version != GUCEF_IMAGE_TIMAGEMIPMAPLEVELINFO_VERSION )  )
                    {
                        for ( UInt32 m=0; m<n; ++m ) delete []imageFrameInfo[ m ].mipmapLevel;
                        delete []imageFrameInfo;
                        return false;
                    }
                }
            }
        }

        // Now we can read all the pixel data
        CImage::TFrameList frameList;
        CImage::TMipMapList mipMapList;
        UInt32 bufferOffset = bufferAccess.Tell();        
        char* pixelBuffer = static_cast< char* >( outputBuffer.GetBufferPtr() ) + bufferOffset;
        for ( UInt32 i=0; i<imageInfo.nrOfFramesInImage; ++i )
        {
            TImageFrameInfo* currentFrame = &imageFrameInfo[ i ].frameInfo;
            for ( UInt32 n=0; n<currentFrame->nrOfMipmapLevels; ++n )
            {
                // Create a pixelmap using the information we obtained + out pixelBuffer pointer
                TImageMipMapLevelInfo* mipMapLevel = &imageFrameInfo[ i ].mipmapLevel[ n ].mipLevelInfo;
                CPixelMap* pixelMap = new CPixelMap( pixelBuffer                                             ,
                                                     mipMapLevel->frameWidth                                 ,
                                                     mipMapLevel->frameHeight                                ,
                                                     TPixelStorageFormat( mipMapLevel->pixelStorageFormat )  ,
                                                     TBuildinDataType( mipMapLevel->pixelComponentDataType ) );

                // Add this mipmap level to the frame
                mipMapList.push_back( CImage::TPixelMapPtr( pixelMap ) );
                
                // Jump to the next image component in the buffer
                UInt32 pixelBlockSize = ( mipMapLevel->frameWidth * mipMapLevel->frameHeight ) * ( mipMapLevel->channelComponentSize * mipMapLevel->channelCountPerPixel );
                bufferOffset += pixelBlockSize;
                if ( bufferOffset < outputBuffer.GetDataSize() )
                {
                    pixelBuffer += pixelBlockSize;
                }
                else
                {
                    // oh oh,.. we ran out of pixels in our buffer even though our image info
                    // suggests we should have more pixels
                    for ( UInt32 m=n; m<currentFrame->nrOfMipmapLevels; ++m ) delete []imageFrameInfo[ m ].mipmapLevel;
                    delete []imageFrameInfo;
                    return false;
                }
                
            }
            
            // Add the frame with all it's mipmap levels to the frame list
            frameList.push_back( mipMapList );
            mipMapList.clear();
            
            // Clean up our info storage
            delete []imageFrameInfo[ i ].mipmapLevel;
        }
                
        // Clean up our info storage
        delete []imageFrameInfo;
        
        // Assign the parsed data to the output image object
        outputImage.Assign( frameList );
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

CORE::CString
CIMGCodec::GetFamilyName( void ) const
{TRACE;
    
    return m_codecPtr->GetFamilyName();
}

/*-------------------------------------------------------------------------*/

CORE::CString
CIMGCodec::GetType( void ) const
{TRACE;

    return m_codecPtr->GetType();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace IMAGE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/