/*
 *  dstoreJSONPARSER: GUCEF plugin for data tree management
 *  Copyright (C) 2002 - 2011.  Dinand Vanvelzen
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
 
#ifndef DLLMAINDSTOREPLUGINJSONPARSER_H
#define DLLMAINDSTOREPLUGINJSONPARSER_H

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

#ifdef DSTOREPLUGINJSONPARSER_BUILD_MODULE
  #define DSTOREPLUG_EXPORT_C GUCEF_EXPORT
#else
  #define DSTOREPLUG_EXPORT_C
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
                             Int32 nodeType       ,
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
                           int atttype          ,
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

#endif /* DLLMAINDSTOREPLUGINJSONPARSER_H ? */

/*--------------------------------------------------------------------------//
//                                                                          //
//      Info & Changes                                                      //
//                                                                          //
//--------------------------------------------------------------------------//

- 05-04-2005 :
        - Initial version of this DSTORE codec plugin module for gucefCORE

----------------------------------------------------------------------------*/
