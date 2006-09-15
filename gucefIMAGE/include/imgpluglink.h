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
 *      These types are used internally by GUCEF to link with a image loading
 *      plugin module. This header should not be included in a binary only
 *      distribution of the GUCEF module.
 */

#ifndef IMGPLUGLINK_H
#define IMGPLUGLINK_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_IMAGE_ETYPES_H
#include "gucefIMAGE_ETypes.h"      /* simple types used */
#define GUCEF_IMAGE_ETYPES_H
#endif /* GUCEF_IMAGE_ETYPES_H ? */

#ifndef MACROS_H
#include "macros.h"      /* often used macros */
#define MACROS_H
#endif /* MACROS_H ? */

#ifndef GUCEF_CORE_IOACCESS_H
#include "ioaccess.h"    /* source independant recource loading */
#define GUCEF_CORE_IOACCESS_H
#endif /* GUCEF_CORE_IOACCESS_H ? */

#ifndef IMAGEDATA_H
#include "imagedata.h"  /* image data storage */
#define IMAGEDATA_H
#endif /* IMAGEDATA_H ? */

#ifndef ESTRUCTS_H
#include "EStructs.h"
#define ESTRUCTS_H
#endif /* ESTRUCTS_H ? */  

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
namespace GUCEF {
namespace IMAGE {
#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
#define TIOAccess CORE::TIOAccess
#define TVersion CORE::TVersion 
#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

typedef UInt32 ( GUCEF_CALLSPEC_PREFIX *TIMGPLUGFPRT_Check_Ext_Loadable ) ( const char *filename, UInt32 *hidx ) GUCEF_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_CALLSPEC_PREFIX *TIMGPLUGFPRT_Check_Loadable )     ( TIOAccess *access, UInt32 *hidx ) GUCEF_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_CALLSPEC_PREFIX *TIMGPLUGFPRT_Check_FLoadable )    ( Int32 format, UInt32 *hidx ) GUCEF_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_CALLSPEC_PREFIX *TIMGPLUGFPRT_Check_Ext_Saveable ) ( const char *filename, UInt32 *hidx ) GUCEF_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_CALLSPEC_PREFIX *TIMGPLUGFPRT_Check_Saveable )     ( const TImageData* imgdata, UInt32 *hidx ) GUCEF_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_CALLSPEC_PREFIX *TIMGPLUGFPRT_Check_FSaveable )    ( Int32 format, UInt32 *hidx ) GUCEF_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_CALLSPEC_PREFIX *TIMGPLUGFPRT_Load )               ( UInt32 hidx, TIOAccess *access, TImageData *imgdata, void **plugdata ) GUCEF_CALLSPEC_SUFFIX;
typedef UInt32 ( GUCEF_CALLSPEC_PREFIX *TIMGPLUGFPRT_Save )               ( UInt32 hidx, const char *filename, UInt32 format, UInt32 compression, TImageData *imgdata ) GUCEF_CALLSPEC_SUFFIX;
typedef void   ( GUCEF_CALLSPEC_PREFIX *TIMGPLUGFPRT_Unload )             ( UInt32 hidx, TImageData *imgdata, void *plugdata ) GUCEF_CALLSPEC_SUFFIX;
typedef const char* ( GUCEF_CALLSPEC_PREFIX *TIMGPLUGFPTR_Name )          ( void ) GUCEF_CALLSPEC_SUFFIX;
typedef const char* ( GUCEF_CALLSPEC_PREFIX *TIMGPLUGFPTR_Description )   ( void ) GUCEF_CALLSPEC_SUFFIX;
typedef const char* ( GUCEF_CALLSPEC_PREFIX *TIMGPLUGFPTR_Copyright )     ( void ) GUCEF_CALLSPEC_SUFFIX;
typedef const char** ( GUCEF_CALLSPEC_PREFIX *TIMGPLUGFPTR_Loadable_Ext_List ) ( void ) GUCEF_CALLSPEC_SUFFIX;
typedef const char** ( GUCEF_CALLSPEC_PREFIX *TIMGPLUGFPTR_Saveable_Ext_List ) ( void ) GUCEF_CALLSPEC_SUFFIX;
typedef const TVersion* ( GUCEF_CALLSPEC_PREFIX *TIMGPLUGFPTR_Version )  ( void ) GUCEF_CALLSPEC_SUFFIX;

/*-------------------------------------------------------------------------*/

/**
 *      Internally used storage structure for plugin access.
 */
struct SImgLoaderPlugin
{
        TIMGPLUGFPRT_Check_Ext_Loadable IMGPLUG_Check_If_Loadable_Using_Ext;
        TIMGPLUGFPRT_Check_Loadable     IMGPLUG_Check_If_Loadable;
        TIMGPLUGFPRT_Check_FLoadable    IMGPLUG_Check_If_Format_Loadable;
        TIMGPLUGFPRT_Check_Ext_Saveable IMGPLUG_Check_If_Saveable_Using_Ext;
        TIMGPLUGFPRT_Check_Saveable     IMGPLUG_Check_If_Saveable;        
        TIMGPLUGFPRT_Check_FSaveable    IMGPLUG_Check_If_Format_Saveable;
        TIMGPLUGFPRT_Load               IMGPLUG_Load_Image_Data;
        TIMGPLUGFPRT_Save               IMGPLUG_Save_Image_Data;
        TIMGPLUGFPRT_Unload             IMGPLUG_Unload_Image_Data;
        TIMGPLUGFPTR_Name               IMGPLUG_Name;
        TIMGPLUGFPTR_Description        IMGPLUG_Description;
        TIMGPLUGFPTR_Copyright          IMGPLUG_Copyright;    
        TIMGPLUGFPTR_Version            IMGPLUG_Version; 
        TIMGPLUGFPTR_Loadable_Ext_List  IMGPLUG_Loadable_Ext_List;
        TIMGPLUGFPTR_Saveable_Ext_List  IMGPLUG_Saveable_Ext_List;

        UInt32 loadcount;            /* number of outstanding image loads */
        void *sdl_sohandle;          /* access to the dl module */
};

typedef struct SImgLoaderPlugin TImgLoaderPlugin;

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
#undef TIOAccess
#undef TVersion 
#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/
#ifdef __cplusplus

}; /* namespace IMAGE */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/

#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------*/

#endif /* IMGPLUGLINK_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 13-03-2004 :
        - Created this file so i could access these types from another location
          aswell. Note though that this header is for internal use only. It
          should only be distributed with a GUCEF library binary.

-----------------------------------------------------------------------------*/
 