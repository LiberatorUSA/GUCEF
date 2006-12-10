/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2006.  All rights reserved.
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
 
#ifndef IMGPLUGINDEVIL_H
#define IMGPLUGINDEVIL_H

/*
 *      API for an image load/save plugin module
 */
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

#ifndef GUCEF_CORE_ESTRUCTS_H
#include "EStructs.h"           /* gucefIMAGE structures, needed for TVersion */
#define GUCEF_CORE_ESTRUCTS_H
#endif /* GUCEF_CORE_ESTRUCTS_H ? */

#ifndef GUCEF_CORE_IOACCESS_H
#include "ioaccess.h"           /* C API for media abstraction  */
#define GUCEF_CORE_IOACCESS_H
#endif /* GUCEF_CORE_IOACCESS_H ? */

#ifndef GUCEF_IMAGE_IMAGEDATA_H
#include "imagedata.h"          /* plugin API structures */
#define GUCEF_IMAGE_IMAGEDATA_H
#endif /* GUCEF_IMAGE_IMAGEDATA_H ? */

#ifndef GUCEF_IMAGE_MACROS_H
#include "gucefIMAGE_macros.h"  /* gucefIMAGE macros, used here for the export and callspec macros */
#define GUCEF_IMAGE_MACROS_H
#endif /* GUCEF_IMAGE_MACROS_H ? */ 
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
namespace GUCEF {
namespace IMAGE {
#endif /* __cplusplus */

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Prevent C++ name mangling
 */
#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/

EXPORT_C UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
IMAGEPLUGIN_Init( void** plugdata    , 
                  const char*** args ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/

EXPORT_C UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
IMAGEPLUGIN_Shutdown( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/

EXPORT_C const char* GUCEF_PLUGIN_CALLSPEC_PREFIX
IMAGEPLUGIN_Name( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/

EXPORT_C const char* GUCEF_PLUGIN_CALLSPEC_PREFIX
IMAGEPLUGIN_Copyright( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/

EXPORT_C const TVersion* GUCEF_PLUGIN_CALLSPEC_PREFIX
IMAGEPLUGIN_Version( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/

EXPORT_C UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
IMAGEPLUGIN_LoadImage( void* plugdata           ,
                       const char* imageType    ,
                       TIOAccess* sourceData    ,  
                       TImage** outputImageData ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/

EXPORT_C UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
IMAGEPLUGIN_DeleteLoadedImage( void* plugdata    ,
                               TImage* imageData ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/

EXPORT_C const char* GUCEF_PLUGIN_CALLSPEC_PREFIX
IMAGEPLUGIN_DetectImageType( void* plugdata        ,
                             TIOAccess* sourceData ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/

EXPORT_C UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
IMAGEPLUGIN_SaveImage( void* plugdata         , 
                       TImage* inputImageData ,
                       TIOAccess* outputMedia ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/

EXPORT_C UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
IMAGEPLUGIN_GetFormatList( void* plugdata                 , 
                           const char*** supportedFormats ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
   }
#endif /* __cplusplus */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
}; /* namespace INPUT */
}; /* namespace GUCEF */
#endif /* __cplusplus */

/*-------------------------------------------------------------------------*/

#endif /* IMGPLUGINDEVIL_H ? */

/*--------------------------------------------------------------------------//
//                                                                          //
//      Info & Changes                                                      //
//                                                                          //
//--------------------------------------------------------------------------//

- 02-11-2006 :
        - Created this set of functions.

----------------------------------------------------------------------------*/