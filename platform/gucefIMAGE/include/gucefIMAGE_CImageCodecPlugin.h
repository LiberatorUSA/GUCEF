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

#ifndef GUCEF_IMAGE_CIMAGECODECPLUGIN_H
#define GUCEF_IMAGE_CIMAGECODECPLUGIN_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CSTDCODECPLUGIN_H
#include "CStdCodecPlugin.h"
#define GUCEF_CORE_CSTDCODECPLUGIN_H
#endif /* GUCEF_CORE_CSTDCODECPLUGIN_H ? */

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

/*-------------------------------------------------------------------------*/

/**
 *  Class wrapping the interface towards Image codec plugins
 *  Image codec plugins provide an additional interface for more efficient
 *  image based data access.
 */
class GUCEF_IMAGE_EXPORT_CPP CImageCodecPlugin : public CORE::CStdCodecPlugin
{
    public:

    static const CString ImageCodecFamilyName;

    CImageCodecPlugin( void );

    virtual ~CImageCodecPlugin();

    virtual bool Link( void* modulePtr                         ,
                       CORE::TPluginMetaDataPtr pluginMetaData );

    virtual bool Unlink( void );

    virtual bool Encode( const CImage& inputImage       ,
                         CORE::CIOAccess& encodedOutput ,
                         const CString& typeName        );

    virtual bool Decode( CORE::CIOAccess& encodedInput ,
                         CImage& outputImage           ,
                         const CString& typeName       );

    virtual bool Encode( CORE::CIOAccess& source ,
                         CORE::CIOAccess& dest   ,
                         const CString& typeName );

    virtual bool Decode( CORE::CIOAccess& source ,
                         CORE::CIOAccess& dest   ,
                         const CString& typeName );

    virtual bool DecodeMetaData( CORE::CIOAccess& encodedInput ,
                                 CORE::CValueList& metaData    ,
                                 const CString& typeName       );

    private:

    CImageCodecPlugin( const CImageCodecPlugin& src );            /**< not implemented */
    CImageCodecPlugin& operator=( const CImageCodecPlugin& src ); /**< not implemented */

    private:

    CORE::TDefaultFuncPtr m_icFuncPointers[ 4 ];
};

/*-------------------------------------------------------------------------*/

typedef CORE::CTSharedPtr< CImageCodecPlugin, MT::CMutex > TImageCodecPluginPtr;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace IMAGE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_IMAGE_CIMAGECODECPLUGIN_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 27-11-2004 :
        - Dinand: Initial implementation

-----------------------------------------------------------------------------*/
