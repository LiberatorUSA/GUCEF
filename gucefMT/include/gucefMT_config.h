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

#ifndef GUCEF_MT_CONFIG_H
#define GUCEF_MT_CONFIG_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CONFIG_H
#include "gucef_config.h"       /* GUCEF platform config */
#define GUCEF_CONFIG_H
#endif /* GUCEF_CONFIG_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      BUILD DEFINES                                                      //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Do you wish to manually define wheter you are building or using a
 *      dynamicly linked library ?
 *      Manual is not recommended unless the macros do not fully support your
 *      compiler.
 */
#undef GUCEFMT_MANUAL_EXPORT_DEFINE      /* do not change this line */  
/* #define GUCEFMT_MANUAL_EXPORT_DEFINE */

/*
 *      Dynamic or static linking ?
 *      and build the dynamic linked library or use it ?
 */
#ifdef GUCEFMT_MANUAL_EXPORT_DEFINE         /* do not change this line */ 
#undef GUCEFMT_BUILD_DLL                    /* do not change this line */  
#undef GUCEFMT_USE_DLL                      /* do not change this line */   
#endif /* GUCEFMT_MANUAL_EXPORT_DEFINE ? */ /* do not change this line */ 
/* #define GUCEFMT_BUILD_DLL */
/* #define GUCEFMT_USE_DLL */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MT_CONFIG_H */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 26-09-2006 :
       - Dinand: moved platform wide build switches to the GUCEF platform config file
- 25-02-2006 :
       - Dinand: Added GUCEFMT_ prefix to all defines
- 16-10-2004 :
       - Dinand: Initial version of this file.

-----------------------------------------------------------------------------*/

