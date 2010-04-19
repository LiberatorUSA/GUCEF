/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
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
 
#ifndef DLLMAIN_H
#define DLLMAIN_H

/*
 *      API for a VFS plugin module
 */
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

#ifndef GUCEF_CORE_IOACCESS_H
#include "ioaccess.h"           /* media independant recource I/O */
#define GUCEF_CORE_IOACCESS_H
#endif /* GUCEF_CORE_IOACCESS_H ? */

#ifndef GUCEF_CORE_ESTRUCTS_H
#include "EStructs.h"           /* gucefCORE structures, needed for TVersion */
#define GUCEF_CORE_ESTRUCTS_H
#endif /* GUCEF_CORE_ESTRUCTS_H ? */

#ifndef GUCEF_VFS_MACROS_H
#include "gucefVFS_macros.h"    /* gucefVFS macros, used here for the export and callspec macros */
#define GUCEF_VFS_MACROS_H
#endif /* GUCEF_VFS_MACROS_H ? */ 
 
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

EXPORT_C UInt32 PLUGIN_CALLSPEC_PREFIX
VFSPLUG_Init( void** plugdata ) PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/

EXPORT_C UInt32 PLUGIN_CALLSPEC_PREFIX
VFSPLUG_Shutdown( void** plugdata ) PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/

EXPORT_C UInt32 PLUGIN_CALLSPEC_PREFIX
VFSPLUG_Pack_Open( void** plugdata     ,
                   TIOAccess* packfile ,
                   void** packdata     ) PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/

EXPORT_C UInt32 PLUGIN_CALLSPEC_PREFIX
VFSPLUG_Pack_Close( void** plugdata     ,
                    TIOAccess* packfile ,
                    void** packdata     ) PLUGIN_CALLSPEC_SUFFIX;
                  
/*---------------------------------------------------------------------------*/

EXPORT_C TIOAccess* PLUGIN_CALLSPEC_PREFIX
VFSPLUG_Load( void** plugdata      ,
              TIOAccess* packfile  ,
              void** packdata      , 
              const char* filename ,                            
              UInt32* errorcode    ) PLUGIN_CALLSPEC_SUFFIX;
              
/*---------------------------------------------------------------------------*/              
              
EXPORT_C TIOAccess* PLUGIN_CALLSPEC_PREFIX
VFSPLUG_Load_Writeable( void** plugdata      ,
                        TIOAccess* packfile  ,
                        void** packdata      , 
                        const char* filename ,                                               
                        UInt32* errorcode    ) PLUGIN_CALLSPEC_SUFFIX;
                        
/*---------------------------------------------------------------------------*/
                  
EXPORT_C void PLUGIN_CALLSPEC_PREFIX
VFSPLUG_Unload( void** plugdata      ,
                TIOAccess* packfile  ,                 
                void** packdata      ,
                TIOAccess* file      ,
                const char* filename ,                                
                UInt32* errorcode    ) PLUGIN_CALLSPEC_SUFFIX;
                
/*---------------------------------------------------------------------------*/

EXPORT_C const char** PLUGIN_CALLSPEC_PREFIX
VFSPLUG_Filelist( void** plugdata     ,
                  TIOAccess* packfile ,
                  void** packdata     ,
                  UInt32* errorcode   ) PLUGIN_CALLSPEC_SUFFIX;
                  
/*---------------------------------------------------------------------------*/                  
      
EXPORT_C void PLUGIN_CALLSPEC_PREFIX           
VFSPLUG_Free_Filelist( void** plugdata     ,  
                       TIOAccess* packfile , 
                       void** packdata     , 
                       char** filelist     , 
                       UInt32* errorcode   ) PLUGIN_CALLSPEC_SUFFIX;                  
                  
/*---------------------------------------------------------------------------*/

EXPORT_C const char* PLUGIN_CALLSPEC_PREFIX
VFSPLUG_Name( void** plugdata ) PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/

EXPORT_C const char* PLUGIN_CALLSPEC_PREFIX
VFSPLUG_Copyright_EULA( void** plugdata ) PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/

EXPORT_C const TVersion* PLUGIN_CALLSPEC_PREFIX
VFSPLUG_Version( void** plugdata ) PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/                 

#ifdef __cplusplus
   }
#endif /* __cplusplus */

/*--------------------------------------------------------------------------*/

#endif /* DLLMAIN_H ? */

/*--------------------------------------------------------------------------//
//                                                                          //
//      Info & Changes                                                      //
//                                                                          //
//--------------------------------------------------------------------------//

- 27-03-2005 :
        - Created this set of functions.

----------------------------------------------------------------------------*/
