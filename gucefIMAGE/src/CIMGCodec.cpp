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

CIMGCodec::CIMGCodec( void )
    : CICodec()
{TRACE;
}

/*-------------------------------------------------------------------------*/
    
CIMGCodec::CIMGCodec( const CIMGCodec& src )
    : CICodec( src )
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
    
    CICodec::operator=( src );
    
    if ( &src != this )
    {        
    }
    
    return *this;
} 

/*-------------------------------------------------------------------------*/

bool
CIMGCodec::Encode( const CImage& inputImage            ,
                   CORE::CDynamicBuffer& encodedOutput )
{TRACE;

    // We need to first combine all the image data into a single buffer
    CORE::CDynamicBuffer inputBuffer( sizeof( TImageInfo ) + inputImage.GetTotalPixelStorageSize() );
   
    // We start with the header
    inputBuffer.CopyFrom( 0, sizeof( TImageInfo ), &inputImage.GetImageInfo() );
    
    // Now we copy the pixel map hierarchy into the buffer
    CPixelMap* pixelMap = NULL;
    for ( UInt32 frameNr=0; frameNr<inputImage.GetFrameCount(); ++frameNr )
    {
        for ( UInt32 mipLvl=0; mipLvl<inputImage.GetMipmapLevels(); ++mipLvl )
        {
            pixelMap = inputImage.GetFrame( frameNr, mipLvl );
            inputBuffer.Append( pixelMap.GetDataPtr(), pixelMap.GetTotalSizeInBytes() );
        }
    }

    // It is time to perform the actual Encode()
    CORE::CICodec::TDynamicBufferList outputList( 1 );    
    bool encodingSuccess = Encode( inputBuffer.GetBufferPtr() ,
                                   inputBuffer.GetDataSize()  ,
                                   outputList                 ,
                                   0                          );

    if ( encodingSuccess )
    {
        // Copy the data into the buffer given to us by the user
        encodedOutput = *outputList.begin();
    }
    
    return encodingSuccess;

}

/*-------------------------------------------------------------------------*/

bool
CIMGCodec::Encode( const CImage& inputImage       ,
                   CORE::CIOAccess& encodedOutput )
{TRACE;

    CORE::CDynamicBuffer outputBuffer;

    if ( Encode( inputImage   ,
                 outputBuffer ) )
    {
        return outputBuffer.GetDataSize() == encodedOutput.Write( outputBuffer.GetBufferPtr() ,
                                                                  1                           ,
                                                                  outputBuffer.GetDataSize()  );
    }
    
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CIMGCodec::Decode( const CORE::CDynamicBuffer& encodedInput ,
                   CImage& outputImage                      )
{TRACE;

    CORE::CICodec::TDynamicBufferList outputList( 1 );
    
    bool decodingSuccess = Decode( encodedInput.GetBufferPtr() ,
                                   encodedInput.GetDataSize()  ,
                                   outputList                  ,
                                   0                           );

    if ( decodingSuccess )
    {
        CORE::CDynamicBuffer& outputBuffer = *outputList.begin();
        
        // Fill our header from the buffer
        TImageInfo imageInfo;
        outputBuffer.CopyTo( 0, sizeof( TImageInfo ), &imageInfo );
        
        // Set the data in the outputImage obj
        outputImage.SetData( imageInfo                                          ,
                             outputBuffer.GetBufferPtr() + sizeof( TImageInfo ) );
    }
    
    return decodingSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CIMGCodec::Decode( CORE::CIOAccess& encodedInput ,
                   CImage& outputImage           )
{TRACE;

    // First we copy the resource into a buffer
    CORE::CDynamicBuffer inputBuffer( encodedInput );

    // Now we use the buffer version to do the actual decoding
    return Decode( inputBuffer  ,
                   outputImage  );
}

/*-------------------------------------------------------------------------*/