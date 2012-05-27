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
 *  Implemented the IImageCodec interface by redirecting the calls to 
 *  an GucefImageCodecPlugin plugin module
 */
class GUCEF_IMAGE_EXPORT_CPP CPluginImageCodecItem : public CIImageCodec
{
    public:
    
    /**
     *  Constructs and links the conversion object
     *
     *  @throw EInvalidCodec thrown if the given codec is not of the "ImageCodec" family
     */
    CPluginImageCodecItem( const CCodecPtr& codecPtr );
    
    CPluginImageCodecItem( const CPluginImageCodecItem& src );
    
    virtual ~CPluginImageCodecItem();
    
    CPluginImageCodecItem& operator=( const CPluginImageCodecItem& src );

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
    
    private:
    
    CPluginImageCodecItem( void );
    
    private:
    
    CCodecPtr m_codecPtr;
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