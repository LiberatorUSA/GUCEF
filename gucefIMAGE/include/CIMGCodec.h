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

#ifndef GUCEF_IMAGE_CIMGCODEC_H
#define GUCEF_IMAGE_CIMGCODEC_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CICODEC_H
#include "CICodec.h"
#define GUCEF_CORE_CICODEC_H
#endif /* GUCEF_CORE_CICODEC_H ? */

#ifndef GUCEFIMAGE_MACROS_H
#include "gucefIMAGE_macros.h"       /* module macro's */
#define GUCEFIMAGE_MACROS_H
#endif /* GUCEFIMAGE_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace IMAGE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CImage;
class CORE::CIOAccess;

/*-------------------------------------------------------------------------*/

/**
 *  Utility conversion class for image codec's
 *  It adds some utility member functions that make it easier to use 
 *  the interface when dealing with images
 *
 *  Note that decoded image data is expected to be a blob of the header
 *  info of type TImageInfo followed by the pixel data. The pixel data is
 *  defined as ordered in the following hierarchy:
 *      - frame
 *          - mipmap levels
 *  Each pixel segment is ordered with the bottom left pixel as the first 
 *  pixel in the segment stored as a series of horizontal scan lines.
 */
class GUCEF_IMAGE_EXPORT_CPP CIMGCodec
{
    public:
    
    typedef CORE::CTSharedPtr< CORE::CICodec > CCodecPtr;
    
    /**
     *  Constructs and links the conversion object
     *
     *  @throw EInvalidCodec thrown if the given codec is not of the "ImageCodec" family
     */
    CIMGCodec( const CCodecPtr& codecPtr );
    
    CIMGCodec( const CIMGCodec& src );
    
    ~CIMGCodec();
    
    CIMGCodec& operator=( const CIMGCodec& src );

    bool Encode( const CImage& inputImage       ,
                 CORE::CIOAccess& encodedOutput );
                     
    bool Decode( CORE::CIOAccess& encodedInput ,
                 CImage& outputImage           );
                 
    bool Encode( CORE::CIOAccess& source ,
                 CORE::CIOAccess& dest   );

    bool Decode( CORE::CIOAccess& source ,
                 CORE::CIOAccess& dest   );
                 
    CORE::CString GetFamilyName( void ) const;
    
    CORE::CString GetType( void ) const;
    
    GUCEF_DEFINE_MSGEXCEPTION( GUCEF_IMAGE_EXPORT_CPP, EInvalidCodec );
    
    private:
    
    CIMGCodec( void );
    
    private:
    
    CCodecPtr m_codecPtr;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace IMAGE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_IMAGE_CIMGCODEC_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 20-07-2005 :
        - Dinand: Added this class

-----------------------------------------------------------------------------*/