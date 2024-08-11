/*
 *  gucefIMAGE: GUCEF module providing image utilities
 *
 *  Copyright (C) 1998 - 2024.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
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