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

#ifndef GUCEF_IMAGE_CIMAGECODECREGISTRY_H
#define GUCEF_IMAGE_CIMAGECODECREGISTRY_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTGLOBALONREGISTRY_H
#include "gucefCORE_CTGlobalONRegistry.h"
#define GUCEF_CORE_CTGLOBALONREGISTRY_H
#endif /* GUCEF_CORE_CTGLOBALONREGISTRY_H ? */

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

class GUCEF_IMAGE_EXPORT_CPP CImageCodecRegistry : public CORE::CTGlobalONRegistry< CIImageCodec, MT::CMutex >
{
    public:

    typedef CORE::CTONRegistry< CIImageCodec, MT::CMutex >                            TCodecFamilyRegistry;
    typedef CORE::CTSharedPtr< TCodecFamilyRegistry, MT::CMutex >                     TmageCodecFamilyRegistryPtr;
    typedef CORE::CTONRegistry< CIImageCodec, MT::CMutex >::TRegisteredObjPtr         TImageCodecPtr;
    typedef CORE::CTONRegistry< CIImageCodec, MT::CMutex >::TRegisteredObjPtrVector   TImageCodecPtrVector;

    private:
    friend class CImageGlobal;

    CImageCodecRegistry( void );

    virtual ~CImageCodecRegistry() GUCEF_VIRTUAL_OVERRIDE;

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

