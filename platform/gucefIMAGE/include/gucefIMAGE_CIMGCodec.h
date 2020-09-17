/*
 *  gucefIMAGE: GUCEF module providing image utilities
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
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
    
    typedef CORE::CTSharedPtr< CORE::CICodec, MT::CMutex > CCodecPtr;
    
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