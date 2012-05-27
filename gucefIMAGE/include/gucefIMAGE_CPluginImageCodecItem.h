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

#ifndef GUCEF_IMAGE_CPLUGINIMAGECODECITEM_H
#define GUCEF_IMAGE_CPLUGINIMAGECODECITEM_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_IMAGE_CIIMAGECODEC_H
#include "gucefIMAGE_CIImageCodec.h"
#define GUCEF_IMAGE_CIIMAGECODEC_H
#endif /* GUCEF_IMAGE_CIIMAGECODEC_H ? */

#ifndef GUCEF_IMAGE_CIMAGECODECPLUGIN_H
#include "gucefIMAGE_CImageCodecPlugin.h"
#define GUCEF_IMAGE_CIMAGECODECPLUGIN_H
#endif /* GUCEF_IMAGE_CIMAGECODECPLUGIN_H ? */

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
 *  Implements the IImageCodec interface by redirecting the calls to
 *  a GucefImageCodecPlugin plugin module
 */
class CPluginImageCodecItem : public CIImageCodec
{
    public:

    /**
     *
     */
    CPluginImageCodecItem( TImageCodecPluginPtr pluginPtr ,
                           const CString& codecTypeName   );

    CPluginImageCodecItem( const CPluginImageCodecItem& src );

    virtual ~CPluginImageCodecItem();

    virtual bool Encode( const CImage& inputImage       ,
                         CORE::CIOAccess& encodedOutput );

    virtual bool Decode( CORE::CIOAccess& encodedInput ,
                         CImage& outputImage           );

    virtual bool Encode( CORE::CIOAccess& source ,
                         CORE::CIOAccess& dest   );

    virtual bool Decode( CORE::CIOAccess& source ,
                         CORE::CIOAccess& dest   );

    virtual CORE::CString GetFamilyName( void ) const;

    virtual CORE::CString GetType( void ) const;

    virtual const CString& GetClassTypeName( void ) const;

    virtual CORE::CICloneable* Clone( void ) const;

    private:

    CPluginImageCodecItem( void );
    CPluginImageCodecItem& operator=( const CPluginImageCodecItem& src );

    private:

    TImageCodecPluginPtr m_pluginPtr;
    CString m_codecTypeName;
};

/*-------------------------------------------------------------------------*/

typedef CORE::CTSharedPtr< CPluginImageCodecItem > TPluginImageCodecItemPtr;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace IMAGE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_IMAGE_CPLUGINIMAGECODECITEM_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 20-07-2005 :
        - Dinand: Added this class

---------------------------------------------------------------------------*/
