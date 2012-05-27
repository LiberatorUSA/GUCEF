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

/*
 *      This is the main include header for the gucefIMAGE library.
 *      It is part of a framework called GUCEF.
 *      All classes in this library have to do with image manipulation and
 *      loading saving of image data into/from a generic storage structure.
 */

#ifndef GUCEFIMAGE_H
#define GUCEFIMAGE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus

namespace GUCEF
{
    /**
     *  Namespace for all image manipulation functionality within GUCEF
     *  All classes in this namespace have to do with image manipulation and
     *  loading saving of image data into/from a generic storage structure.
     */
    namespace IMAGE
    {
    }
}

#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Library specific macros and generic macros
 */
#ifndef GUCEFIMAGE_MACROS_H
#include "gucefIMAGE_macros.h"
#define GUCEFIMAGE_MACROS_H
#endif /* GUCEFIMAGE_MACROS_H ? */

/*
 *      Image data storage structure
 */
#ifndef GUCEF_IMAGE_IMAGEDATA_H
#include "gucefIMAGE_imagedata.h"
#define GUCEF_IMAGE_IMAGEDATA_H
#endif /* GUCEF_IMAGE_IMAGEDATA_H ? */

/*---------------------------------------------------------------------------/

        C++ includes

/---------------------------------------------------------------------------*/
#ifdef __cplusplus

/*
 *  class that represents an image
 */
#ifndef GUCEF_IMAGE_CIMAGE_H
#include "gucefIMAGE_CImage.h"
#define GUCEF_IMAGE_CIMAGE_H
#endif /* GUCEF_IMAGE_CIMAGE_H ? */

/*
 *  class that represents an 2D pixel grid called a pixelmap
 */
#ifndef GUCEF_IMAGE_CPIXELMAP_H
#include "gucefIMAGE_CPixelMap.h"
#define GUCEF_IMAGE_CPIXELMAP_H
#endif /* GUCEF_IMAGE_CPIXELMAP_H ? */

/*
 *  Class that specializes the CICodec to a more convenient interface
 *  serves as an abstract base class for implementations of image codecs
 */
#ifndef GUCEF_IMAGE_CIMGCODEC_H
#include "gucefIMAGE_CIMGCodec.h"
#define GUCEF_IMAGE_CIMGCODEC_H
#endif /* GUCEF_IMAGE_CIMGCODEC_H ? */

#ifndef GUCEF_IMAGE_CIMAGECODECPLUGINMANAGER_H
#include "gucefIMAGE_CImageCodecPluginManager.h"
#define GUCEF_IMAGE_CIMAGECODECPLUGINMANAGER_H
#endif /* GUCEF_IMAGE_CIMAGECODECPLUGINMANAGER_H ? */

#ifndef GUCEF_IMAGE_CIMAGECODECREGISTRY_H
#include "gucefIMAGE_CImageCodecRegistry.h"
#define GUCEF_IMAGE_CIMAGECODECREGISTRY_H
#endif /* GUCEF_IMAGE_CIMAGECODECREGISTRY_H ? */

#ifndef GUCEF_IMAGE_CIMAGEGLOBAL_H
#include "gucefIMAGE_CImageGlobal.h"
#define GUCEF_IMAGE_CIMAGEGLOBAL_H
#endif /* GUCEF_IMAGE_CIMAGEGLOBAL_H ? */

#endif /* __cplusplus ? */
/*--------------------------------------------------------------------------*/

#endif /* GUCEFIMAGE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 27-11-2004 :
       - Replaced CImageLoaderPlugins with the CIMGCodecManager singleton class.
       - Added the CIMGCodec class.
       - Modified include descriptions to match the new structure.
- 11-03-2004 :
       - Made the initial version of the library header. The classes here
         where originally part of one large GUCEF codebase without segmentation.

-----------------------------------------------------------------------------*/



