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
 *      API for a DSTORE codec plugin module
 */
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 
 
#ifndef IOACCESS_H
#include "ioaccess.h"                   /* media independant recource I/O */
#define IOACCESS_H
#endif /* IOACCESS_H ? */

#ifndef DSTORECODECPLUGINSTRUCTS_H
#include "DStoreCodecPluginStructs.h"   /* structs specific for this type of plugin */
#define DSTORECODECPLUGINSTRUCTS_H
#endif /* DSTORECODECPLUGINSTRUCTS_H ? */

#ifndef ESTRUCTS_H
#include "EStructs.h"                   /* gucefCORE structures, needed for TVersion */
#define ESTRUCTS_H
#endif /* ESTRUCTS_H ? */

#ifndef GUCEFCORE_MACROS_H
#include "gucefCORE_macros.h"           /* gucefCORE macros, used here for the export and callspec macros */
#define GUCEFCORE_MACROS_H
#endif /* GUCEFCORE_MACROS_H ? */ 
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef DSTOREPLUG_BUILD_MODULE
  #define DSTOREPLUG_EXPORT_C GUCEFCORE_EXPORT
#endif 
 
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

DSTOREPLUG_EXPORT_C UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
DSTOREPLUG_Init( void** plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/

DSTOREPLUG_EXPORT_C void GUCEF_PLUGIN_CALLSPEC_PREFIX
DSTOREPLUG_Shutdown( void** plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/

DSTOREPLUG_EXPORT_C UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX 
DSTOREPLUG_Dest_File_Open( void** plugdata      ,
                           void** filedata      ,
                           TIOAccess* outFile   ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
                           
/*---------------------------------------------------------------------------*/
                           
DSTOREPLUG_EXPORT_C void GUCEF_PLUGIN_CALLSPEC_PREFIX 
DSTOREPLUG_Dest_File_Close( void** plugdata , 
                            void** filedata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
                            
/*---------------------------------------------------------------------------*/
                            
DSTOREPLUG_EXPORT_C void GUCEF_PLUGIN_CALLSPEC_PREFIX 
DSTOREPLUG_Begin_Node_Store( void** plugdata      , 
                             void** filedata      , 
                             const char* nodename , 
                             UInt32 attscount     ,
                             UInt32 haschildren   ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/
                             
DSTOREPLUG_EXPORT_C void GUCEF_PLUGIN_CALLSPEC_PREFIX 
DSTOREPLUG_End_Node_Store( void** plugdata      , 
                           void** filedata      , 
                           const char* nodename ,
                           UInt32 attscount     ,
                           UInt32 haschildren   ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
                           
/*---------------------------------------------------------------------------*/
                           
DSTOREPLUG_EXPORT_C void GUCEF_PLUGIN_CALLSPEC_PREFIX 
DSTOREPLUG_Store_Node_Att( void** plugdata      , 
                           void** filedata      , 
                           const char* nodename ,
                           UInt32 attscount     , 
                           UInt32 attindex      , 
                           const char* attname  , 
                           const char* attvalue ,
                           UInt32 haschildren   ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
                           
/*---------------------------------------------------------------------------*/
                           
DSTOREPLUG_EXPORT_C void GUCEF_PLUGIN_CALLSPEC_PREFIX 
DSTOREPLUG_Begin_Node_Children( void** plugdata      , 
                                void** filedata      , 
                                const char* nodename ) GUCEF_PLUGIN_CALLSPEC_SUFFIX; 
                                
/*---------------------------------------------------------------------------*/
                                
DSTOREPLUG_EXPORT_C void GUCEF_PLUGIN_CALLSPEC_PREFIX 
DSTOREPLUG_End_Node_Children( void** plugdata      , 
                              void** filedata      , 
                              const char* nodename ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
                                  
/*---------------------------------------------------------------------------*/                                  
                                  
DSTOREPLUG_EXPORT_C UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX 
DSTOREPLUG_Src_File_Open( void** plugdata      , 
                          void** filedata      ,
                          TIOAccess* file      ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
                          
/*---------------------------------------------------------------------------*/
                          
DSTOREPLUG_EXPORT_C void GUCEF_PLUGIN_CALLSPEC_PREFIX 
DSTOREPLUG_Src_File_Close( void** plugdata , 
                           void** filedata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
                           
/*---------------------------------------------------------------------------*/
                           
DSTOREPLUG_EXPORT_C void GUCEF_PLUGIN_CALLSPEC_PREFIX 
DSTOREPLUG_Set_Read_Handlers( void** plugdata                , 
                              void** filedata                , 
                              const TReadHandlers* rhandlers , 
                              void* privdata                 ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
                              
/*---------------------------------------------------------------------------*/
                             
DSTOREPLUG_EXPORT_C UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX 
DSTOREPLUG_Start_Reading( void** plugdata , 
                          void** filedata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
                  
/*---------------------------------------------------------------------------*/

DSTOREPLUG_EXPORT_C const char* GUCEF_PLUGIN_CALLSPEC_PREFIX
DSTOREPLUG_Type( const void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
                  
/*---------------------------------------------------------------------------*/

DSTOREPLUG_EXPORT_C const char* GUCEF_PLUGIN_CALLSPEC_PREFIX
DSTOREPLUG_Name( const void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/

DSTOREPLUG_EXPORT_C const char* GUCEF_PLUGIN_CALLSPEC_PREFIX
DSTOREPLUG_Copyright( const void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/

DSTOREPLUG_EXPORT_C const TVersion* GUCEF_PLUGIN_CALLSPEC_PREFIX
DSTOREPLUG_Version( const void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;

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

- 05-04-2005 :
        - Initial version of this DSTORE codec plugin module for gucefCORE

----------------------------------------------------------------------------*/
