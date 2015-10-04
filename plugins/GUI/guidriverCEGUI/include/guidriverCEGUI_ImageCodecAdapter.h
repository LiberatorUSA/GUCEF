/*
 *  guidriverCEGUI: glue module for the CEGUI GUI backend
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef GUCEF_GUIDRIVERCEGUI_IMAGECODECADAPTER_H
#define GUCEF_GUIDRIVERCEGUI_IMAGECODECADAPTER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef _CEGUIBase_h_
#include "CEGUI/Base.h"
#define _CEGUIBase_h_
#endif /* _CEGUIBase_h_ ? */

#ifndef _CEGUIImageCodec_h_
#include "CEGUI/ImageCodec.h"
#define _CEGUIImageCodec_h_
#endif /* _CEGUIImageCodec_h_ ? */

#ifndef GUCEF_GUIDRIVERCEGUI_MACROS_H
#include "guidriverCEGUI_macros.h" 
#define GUCEF_GUIDRIVERCEGUI_MACROS_H
#endif /* GUCEF_GUIDRIVERCEGUI_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUIDRIVERCEGUI {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_GUIDRIVERCEGUI_EXPORT_CPP ImageCodecAdapter : public ::CEGUI::ImageCodec
{
    public :

    ImageCodecAdapter();

    virtual ~ImageCodecAdapter();

    /*!
      \brief 
      Return the name of the image codec object 
      
      Return the name of the image codec 

      \return a string containing image codec name 
    */
    const CEGUI::String& getIdentifierString() const;

    /*! 
      \brief
      Return the list of image file format supported 

      Return a list of space separated image format supported by this
      codec

      \return 
      list of supported image file format separated with space 
    */
    const CEGUI::String& getSupportedFormat() const;
    
    /*!
      \brief 
      Load an image from a memory buffer 

      \param data the image data 

      \param result the texture to use for storing the image data 
     
      \return result on success or 0 if the load failed 
    */
    virtual CEGUI::Texture* load( const CEGUI::RawDataContainer& data, CEGUI::Texture* result );

    static bool TryConvertPixelFormat( IMAGE::TPixelStorageFormat pixelFormat , 
                                       CORE::UInt32 channelSize               , 
                                       CEGUI::Texture::PixelFormat& outFormat );

    private:

    CEGUI::String m_codecName;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUIDRIVERCEGUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_GUIDRIVERCEGUI_IMAGECODECADAPTER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-08-2010 :
        - Dinand: Initial implementation

---------------------------------------------------------------------------*/