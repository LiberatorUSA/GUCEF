/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
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

    private:

    CImageCodecPlugin( const CImageCodecPlugin& src );            /**< not implemented */
    CImageCodecPlugin& operator=( const CImageCodecPlugin& src ); /**< not implemented */

    private:

    CORE::TDefaultFuncPtr m_icFuncPointers[ 3 ];
};

/*-------------------------------------------------------------------------*/

typedef CORE::CTSharedPtr< CImageCodecPlugin > TImageCodecPluginPtr;

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
