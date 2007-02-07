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
    }
    
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
CIMGCodec::Encode( const void* sourceData         ,
                   const UInt32 sourceBuffersSize ,
                   CORE::CIOAccess& dest          )
{TRACE;

    return m_codecPtr->Encode( sourceData        ,
                               sourceBuffersSize ,
                               dest              );
}

/*-------------------------------------------------------------------------*/

bool
CIMGCodec::Decode( const void* sourceData         ,
                   const UInt32 sourceBuffersSize ,
                   CORE::CIOAccess& dest          )
{TRACE;

    return m_codecPtr->Decode( sourceData        ,
                               sourceBuffersSize ,
                               dest              );
}

/*-------------------------------------------------------------------------*/

bool
CIMGCodec::Encode( const CImage& inputImage       ,
                   CORE::CIOAccess& encodedOutput )
{TRACE;
     /*     //@todo @makeme
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
    
    return encodingSuccess;     */
    
    return false;

}

/*-------------------------------------------------------------------------*/

bool
CIMGCodec::Decode( CORE::CIOAccess& encodedInput ,
                   CImage& outputImage           )
{TRACE;
           //@todo @makeme
 /*   CORE::CICodec::TDynamicBufferList outputList( 1 );
    
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
    
    return decodingSuccess;    */
    
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