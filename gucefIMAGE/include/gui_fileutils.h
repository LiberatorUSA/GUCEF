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
 *      @file gui_fileutils.h
 *      This file has a number of utilities used to write and read GUI files.
 *      GUI stands for Galaxy Unlimited Image. The GUI format uses little endian
 *      integers.
 */

#ifndef GUI_FILEUTILS_H
#define GUI_FILEUTILS_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <stdio.h>

#ifndef IMAGEDATA_H
#include "Imagedata.h"  /* storage structure used by the GUI file format */
#endif /* IMAGEDATA_H ? */

#ifndef IOACCESS_H
#include "IOAccess.h"   /* source/medium independant recource access */
#endif /* IOACCESS_H ? */

#ifndef GUCEFIMAGE_MACROS_H
#include "gucefIMAGE_macros.h"
#define GUCEFIMAGE_MACROS_H
#endif /* GUCEFIMAGE_MACROS_H ? */

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

/*-------------------------------------------------------------------------*/

/**
 *      Tests wether or not the recource is a GUI file.
 *      Note that this function assumes the current location in the recource is
 *      the start of the file. The location will not have changed after
 *      completing this operation.
 */
EXPORT_C UInt32 
GUIF_Test_If_GUI_File( TIOAccess *access );

/*-------------------------------------------------------------------------*/

/**
 *      Function that reads all the data from the GUI file.
 *      0 is returned if an error occured. This function assumes the current
 *      location in the recource is the start of the GUI file. This allows you
 *      to embed a GUI file in another type of file format. Note that this
 *      function allocates memory in the TImageData structure that should be
 *      freed using GUIF_Free_Image_Data().
 */
EXPORT_C UInt32 
GUIF_Read_Image_Data( TIOAccess *access ,
                      TImageData *dest  );

/*-------------------------------------------------------------------------*/

/**
 *      Save the given imagedata to a GUI file with the name given.
 *      returns 0 in case of failure and 1 on success.
 */
EXPORT_C UInt32 
GUIF_Save_Image_Data( FILE *fptr            ,
                      const TImageData *src );

/*-------------------------------------------------------------------------*/

/**
 *      Deallocates memory that was allocated with GUIF_Read_Image_Data().
 */
EXPORT_C void 
GUIF_Free_Image_Data( TImageData *data );

/*-------------------------------------------------------------------------*/

/**
 *      Simply opens a file and returns the FILE*.
 *      Needed because C file pointers cannot be used across DLL boundries. 
 */
EXPORT_C FILE*
GUIF_Openfile( const char *filename ,
               const char *mode     );

/*-------------------------------------------------------------------------*/

/**
 *      Simply closes a file.
 *      Needed because C file pointers cannot be used across DLL boundries. 
 */
EXPORT_C void 
GUIF_Closefile( FILE *fptr );

/*-------------------------------------------------------------------------*/

/*
 *      End the C++ name mangling protection
 */
#ifdef __cplusplus
};
#endif

/*-------------------------------------------------------------------------*/

#endif /* GUI_FILEUTILS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 05-09-2004 :
        - Added GUIF_Openfile()
        - Added GUIF_Closefile()
        - Modified GUIF_Save_Image_Data() to use a FILE* instead of a filename
          so that we can use it to store the file data embededed into another
          file.
- 19-07-2004 :
        - Implemented these functions.

-----------------------------------------------------------------------------*/

 