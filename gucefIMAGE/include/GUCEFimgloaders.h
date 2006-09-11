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

#ifndef GUCEFIMGLOADERS_H
#define GUCEFIMGLOADERS_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *      Forward declaration of a storage structure used to link functions
 *      to plugin's.
 */
struct SImgLoaderPlugin;

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
 *      Internally used GUCEF function, do not export.
 *      This function links image loader plugin functions which are not really
 *      in a plugin but rather compiled into the GUCEF module. This is good
 *      for testing new loaders and providing basic loading capability's at all
 *      time.
 */
void
GUCEF_IMGPLUG_Link_Functions( struct SImgLoaderPlugin* linkdata );

/*-------------------------------------------------------------------------*/

/*
 *      End the C++ name mangling protection
 */
#ifdef __cplusplus
};
#endif

/*-------------------------------------------------------------------------*/

#endif /* GUCEFIMGLOADERS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 25-08-2004 :
        - Removed TGA loader.
        - Added GUI loader/saver.
- 13-03-2004 :
        - Created the GUCEF_IMGPLUG_Link_Functions() function to allow some
          default loading capability's to be compiled into the GUCEF library
          which is compatible with the new plugin system.

-----------------------------------------------------------------------------*/
