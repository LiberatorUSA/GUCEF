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
class GUCEF_IMAGE_EXPORT_CPP CGUIImageCodec : public CORE::CICodec
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

    virtual const CString& GetClassTypeName( void ) const;
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