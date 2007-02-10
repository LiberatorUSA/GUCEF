/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2007.  All rights reserved.
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

#ifndef GUCEF_IMAGE_CGUIIMAGECODEC_H
#define GUCEF_IMAGE_CGUIIMAGECODEC_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CICODEC_H
#include "CICodec.h"
#define GUCEF_CORE_CICODEC_H
#endif /* GUCEF_CORE_CICODEC_H ? */

#ifndef GUCEF_IMAGE_MACROS_H
#include "gucefIMAGE_macros.h"       /* module macro's */
#define GUCEF_IMAGE_MACROS_H
#endif /* GUCEF_IMAGE_MACROS_H ? */

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

/**
 *  Dummy codec for the 'gui' format which is the un-encoded format used by
 *  GUCEF. 'gui' stands for Galaxy Unlimited Image. The output of the codec
 *  is identical to the input because the input is already in the desired format.
 */
class EXPORT_CPP CGUIImageCodec : public CORE::CICodec
{
    public:
    
    CGUIImageCodec( void );
    
    CGUIImageCodec( const CGUIImageCodec& src );
    
    CGUIImageCodec& operator=( const CGUIImageCodec& src );
    
    virtual ~CGUIImageCodec();
    
    virtual bool Encode( CORE::CIOAccess& source ,
                         CORE::CIOAccess& dest   );

    virtual bool Decode( CORE::CIOAccess& source ,
                         CORE::CIOAccess& dest   );
                         
    virtual CORE::CString GetFamilyName( void ) const;
    
    virtual CORE::CString GetType( void ) const;
    
    virtual CORE::CICloneable* Clone( void ) const;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace IMAGE */
} /* namespace GUCEF */

/*--------------------------------------------------------------------------*/

#endif /* GUCEF_IMAGE_CIMAGE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 10-02-2007 :
        - Dinand: Added this class to make allow registration of the 'gui'
          codec as a hardcoded supported codec.
        
-----------------------------------------------------------------------------*/