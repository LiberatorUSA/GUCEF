/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL, 
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER 
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF 
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT 
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
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
#ifndef IMAGEDATA_H
#include "imagedata.h"
#define IMAGEDATA_H
#endif /* IMAGEDATA_H ? */

/*---------------------------------------------------------------------------/

        C++ includes

/---------------------------------------------------------------------------*/
#ifdef __cplusplus

/*
 *  class that represents an image
 */
#ifndef GUCEF_IMAGE_CIMAGE_H
#include "CImage.h"
#define GUCEF_IMAGE_CIMAGE_H
#endif /* GUCEF_IMAGE_CIMAGE_H ? */

/*
 *  class that represents an 2D pixel grid called a pixelmap
 */
#ifndef GUCEF_IMAGE_CPIXELMAP_H
#include "CPixelMap.h"
#define GUCEF_IMAGE_CPIXELMAP_H
#endif /* GUCEF_IMAGE_CPIXELMAP_H ? */

/*
 *  Class that specializes the CICodec to a more convenient interface
 *  serves as an abstract base class for implementations of image codecs
 */
#ifndef GUCEF_IMAGE_CIMGCODEC_H
#include "CIMGCodec.h"
#define GUCEF_IMAGE_CIMGCODEC_H
#endif /* GUCEF_IMAGE_CIMGCODEC_H ? */

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


 
