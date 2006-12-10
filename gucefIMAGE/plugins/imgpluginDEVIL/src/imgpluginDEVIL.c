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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef __IL_H__
#include "il.h"                 /* main API include for the DevIL library */
#define __IL_H__
#endif /* __IL_H__ ? */

#include "imgpluginDEVIL.h"     /* function prototypes for this plugin */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

static TIOAccess* currentRecource = NULL;

static const char* supportedTypes[] = {

    #ifdef IL_TGA
    "tga", "vda", "icb", "vst",
    #endif  
    #ifdef IL_JPG
    "jpg", "jpe", "jpeg",
    #endif
    #ifdef IL_PNG
    "png",
    #endif
    #ifndef IL_BMP
    "bmp", "dib",
    #endif
    #ifdef IL_GIF
    "gif",
    #endif
    #ifdef IL_CUT
    "cut",
    #endif
    #ifdef IL_HDR
    "hdr",
    #endif
    #ifdef IL_ICO
    "ico",
    #endif
    #ifdef IL_JNG
    "jng",
    #endif
    #ifdef IL_LIF
    "lif",
    #endif
    #ifdef IL_MDL
    "mdl",
    #endif    
    #ifdef IL_MNG
    "mng", "jng",
    #endif 
    #ifdef IL_PCD
    "pcd",
    #endif
    #ifdef IL_PCX
    "pcx",
    #endif
    #ifdef IL_PIC
    "pic",
    #endif
    #ifdef IL_PIX
    "pix",
    #endif
    #ifdef IL_PNM
    "pbm", "pgm", "pnm", "ppm",
    #endif
    #ifdef IL_PSD
    "psd", "pdd",
    #endif
    #ifdef IL_PSP
    "psp",
    #endif    
    #ifdef IL_PXR
    "pxr",
    #endif
    #ifdef IL_SGI
    "sgi", "bw", "rgb", "rgba",
    #endif
    #ifdef IL_TIF
    "tif", "tiff",
    #endif
    #ifdef IL_WAL
    "wal",
    #endif 
    #ifdef IL_XPM
    "xpm",
    #endif                   
    NULL
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

ILvoid ILAPIENTRY
ilfCloseRProc( ILHANDLE handle )
{
    if ( NULL != currentRecource )
    {
  //      currentRecource->close();
    }
}

/*---------------------------------------------------------------------------*/

ILboolean ILAPIENTRY 
ilfEofProc( ILHANDLE handle )
{
    if ( NULL != currentRecource )
    {
    //    currentRecource->close();
    }
}

/*---------------------------------------------------------------------------*/

ILint ILAPIENTRY
ilfGetcProc( ILHANDLE handle )
{
}

/*---------------------------------------------------------------------------*/

ILHANDLE ILAPIENTRY 
ilfOpenRProc( const ILstring ilstring )
{
}

/*---------------------------------------------------------------------------*/

ILint ILAPIENTRY
ilfReadProc( void* data, 
             ILuint a, 
             ILuint  b    , 
             ILHANDLE handle )
{
}

/*---------------------------------------------------------------------------*/

ILint ILAPIENTRY
ilfSeekRProc( ILHANDLE handle , 
              ILint a, 
              ILint  b         )
{
}

/*---------------------------------------------------------------------------*/

ILint ILAPIENTRY
ilfTellRProc( ILHANDLE handle )
{
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
IMAGEPLUGIN_Init( void** plugdata    , 
                  const char*** args ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{

    ilSetRead( ilfOpenRProc  , 
               ilfCloseRProc , 
               ilfEofProc    , 
               ilfGetcProc   , 
               ilfReadProc   , 
               ilfSeekRProc  , 
               ilfTellRProc  );
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
IMAGEPLUGIN_Shutdown( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
}

/*---------------------------------------------------------------------------*/

const char* GUCEF_PLUGIN_CALLSPEC_PREFIX
IMAGEPLUGIN_Name( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    return "imgpluginDEVIL";
}

/*---------------------------------------------------------------------------*/

const char* GUCEF_PLUGIN_CALLSPEC_PREFIX
IMAGEPLUGIN_Copyright( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    return "Copyright (C) Dinand Vanvelzen. 2002 - 2006.  All rights reserved.\0";
}

/*---------------------------------------------------------------------------*/

const TVersion* GUCEF_PLUGIN_CALLSPEC_PREFIX
IMAGEPLUGIN_Version( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    TVersion version;
    
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
IMAGEPLUGIN_LoadImage( void* plugdata           ,
                       TIOAccess* sourceData    ,  
                       TImage** outputImageData ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
IMAGEPLUGIN_DeleteLoadedImage( void* plugdata    ,
                               TImage* imageData ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
IMAGEPLUGIN_SaveImage( void* plugdata         , 
                       TImage* inputImageData ,
                       TIOAccess* outputMedia ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
IMAGEPLUGIN_GetFormatList( void* plugdata                 , 
                           const char*** supportedFormats ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    *supportedFormats = supportedTypes;
    return 1;
}

/*---------------------------------------------------------------------------*/