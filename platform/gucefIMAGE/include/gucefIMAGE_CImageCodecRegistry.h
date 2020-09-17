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

#ifndef GUCEF_IMAGE_CIMAGECODECREGISTRY_H
#define GUCEF_IMAGE_CIMAGECODECREGISTRY_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTONREGISTRY_H
#include "CTONRegistry.h"
#define GUCEF_CORE_CTONREGISTRY_H
#endif /* GUCEF_CORE_CTONREGISTRY_H ? */

#ifndef GUCEF_IMAGE_CIIMAGECODEC_H
#include "gucefIMAGE_CIImageCodec.h"
#define GUCEF_IMAGE_CIIMAGECODEC_H
#endif /* GUCEF_IMAGE_CIIMAGECODEC_H ? */

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

class CImageGlobal;

/*-------------------------------------------------------------------------*/

class GUCEF_IMAGE_EXPORT_CPP CImageCodecRegistry : public CORE::CTONRegistry< CIImageCodec, MT::CMutex >
{
    public:

    typedef CORE::CTONRegistry< CIImageCodec, MT::CMutex > TCodecFamilyRegistry;
    typedef CORE::CTSharedPtr< TCodecFamilyRegistry, MT::CMutex > TImageCodecFamilyRegistryPtr;
    typedef CORE::CTONRegistry< CIImageCodec, MT::CMutex >::TStringList TStringList;
    typedef CORE::CTONRegistry< CIImageCodec, MT::CMutex >::TRegisteredObjPtr TImageCodecPtr;

    private:
    friend class CImageGlobal;

    CImageCodecRegistry( void );

    virtual ~CImageCodecRegistry();

    private:
    
    CImageCodecRegistry( const CImageCodecRegistry& src );              /**< not implemented, don't use */
    CImageCodecRegistry& operator=( const CImageCodecRegistry& src );   /**< not implemented, don't use */
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace IMAGE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_IMAGE_CIMAGECODECREGISTRY_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 20-07-2005 :
        - Dinand: Added this class

---------------------------------------------------------------------------*/
