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
    CORE::CDynamicBuffer inputBuffer( inputImage.GetTotalPixelStorageSize() +  );
    
    // We start with the header
    Int8 enumTemp;
    enumTemp = inputImage.GetPixelComponentDataType();
    inputBuffer.CopyFrom( 1, &enumTemp );
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
//           /*
//struct SImageData
//{
//        Int8 dtype;         /* data type of each value of idata */
//        UInt32 vsize;       /* size of each data value */
//        UInt8 channels;     /* number of channels per pixel */
//        UInt8 has_alpha;    /* wheter the image has an alpha channel */
//        UInt32 width;       /* Width of image. */
//        UInt32 height;      /* Height of image. */
//        UInt8 bbp;          /* BPP, Bits Per Pixel */
//        UInt32 stride;      /* Number of bytes in each horizontal scan line. used for flipping vertically */
//        UInt32 dsize;       /* Allocated pixel data space used by image. */
//        Int8 format;        /* format in which the pixel components are stored */
//        UInt32 frames;      /* total number of images stored */
//        UInt8 mipmaps;      /* number of mipmaps per frame */
//        Int8 icf;           /* image compression format */
//        char compressionType[ 64 ];
//};
//
//typedef struct SImageData TImageData;

}

/*-------------------------------------------------------------------------*/

bool
CIMGCodec::Decode( const CORE::CDynamicBuffer& encodedInput ,
                   CImage& outputImage                      )
{TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CIMGCodec::Decode( const CORE::CIOAccess& encodedInput ,
                   CImage& outputImage                 )
{TRACE;

    // First we copy the recource into a buffer
    CORE::CDynamicBuffer inputBuffer( encodedInput );

    // Now we use the buffer version to do the actual decoding
    return Decode( inputBuffer  ,
                   outputImage  );
}

/*-------------------------------------------------------------------------*/