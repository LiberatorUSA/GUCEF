/*
 *  gucefCOM: GUCEF module providing communication implementations 
 *  for standardized protocols
 *
 *  Copyright (C) 1998 - 2020.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef GUCEFCOMDLLINIT_H
#define GUCEFCOMDLLINIT_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_COM_ETYPES_H
#include "gucefCOM_ETypes.h"                      /* platform types */
#define GUCEF_COM_ETYPES_H
#endif /* GUCEF_COM_ETYPES_H ? */

#ifndef GUCEFCOM_MACROS_H
#include "gucefCOM_macros.h"         /* often used gucefCOM macros */
#define GUCEFCOM_MACROS_H
#endif /* GUCEFCOM_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef GUCEF_MSWIN_BUILD
  #include <windows.h>

  #undef DLL_INIT_FUNC
  #define DLL_INIT_FUNC( ns )                          \
                                                       \
        int WINAPI DllMain( HINSTANCE hinstDLL ,       \
                            DWORD fwdreason    ,       \
                            LPVOID lpvReserved )       \
        {                                              \
                return 1;                              \
        }                                              

  #undef DLL_INIT_FUNC_BEG
  #define DLL_INIT_FUNC_BEG( ns )                      \
                                                       \
        int WINAPI DllMain( HINSTANCE hinstDLL ,       \
                            DWORD fwdreason    ,       \
                            LPVOID lpvReserved )       \
        {                                              \
                if ( fwdreason == DLL_PROCESS_ATTACH ) \
                {                                      \
                        if ( !ns::Load() )             \
                        {                              \
                                return 0;              \
                        }                              \
                }                                      \
                return 1;                              \
        }                                                               

  #undef DLL_INIT_FUNC_END
  #define DLL_INIT_FUNC_END( ns )                       \
                                                        \
        int WINAPI DllMain( HINSTANCE hinstDLL ,        \
                            DWORD fwdreason    ,        \
                            LPVOID lpvReserved )        \
        {                                               \
                if ( fwdreason == DLL_PROCESS_DETACH )  \
                {                                       \
                        if ( !ns::Unload() )            \
                        {                               \
                                return 0;               \
                        }                               \
                }                                       \
		return 1;                               \
        }                                                         
                     
  #undef DLL_INIT_FUNC_BEG_END                     
  #define DLL_INIT_FUNC_BEG_END( ns )                          \
                                                               \
        int WINAPI DllMain( HINSTANCE hinstDLL ,               \
                            DWORD fwdreason    ,               \
                            LPVOID lpvReserved )               \
        {                                                      \
                if ( fwdreason == DLL_PROCESS_ATTACH )         \
                {                                              \
                        if ( !ns::Load() )                     \
                        {                                      \
                                return 0;                      \
                        }                                      \
                        if ( fwdreason == DLL_PROCESS_DETACH ) \
                        {                                      \
                                if ( !ns::Unload() )           \
                                {                              \
                                        return 0;              \
                                }                              \
                        }                                      \
			return 1;                                          \
		}                                                      \
		return 1;                                              \
        }                                                      
#else          				
  #ifdef GUCEF_LINUX_BUILD

    #undef DLL_INIT_FUNC
    #define DLL_INIT_FUNC( ns )   \
                                  \ 
          int _init( void )       \
          {                       \
                  return 1;       \
          }                       \
                                  \
          int _fini( void )       \
          {                       \
                  return 1;       \
          }                       
          
    #undef DLL_INIT_FUNC_BEG      
    #define DLL_INIT_FUNC_BEG( ns )   \
                                      \
          int _init( void )           \
          {                           \
                  if ( !ns::Load() )  \
                  {                   \
                          return 0;   \
                  }                   \
                  return 1;           \
          }                           \
                                      \
          int _fini( void )           \
          {                           \
                  return 1;           \
          }                           
                
    #undef DLL_INIT_FUNC_END            
    #define DLL_INIT_FUNC_END( ns )     \
                                        \
          int _init( void )             \
          {                             \
                  return 1;             \
          }                             \
                                        \
          int _fini( void )             \
          {                             \
                  if ( !ns::Unload() )  \
                  {                     \
                          return 0;     \
                  }                     \
                  return 1;             \
          }                             
                  
    #undef DLL_INIT_FUNC_BEG_END              
    #define DLL_INIT_FUNC_BEG_END( ns ) \
                                        \
          int _init( void )             \
          {                             \
                  if ( !ns::Load() )    \
                  {                     \
                          return 0;     \
                  }                     \
                  return 1;             \
          }                             \
                                        \
          int _fini( void )             \
          {                             \
                  if ( !ns::Unload() )  \
                  {                     \
                          return 0;     \
                  }                     \
                  return 1;             \
          }                             
  #else                  
    #error No DLL entry points available for the target platform
  #endif  
#endif

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEFCOMDLLINIT_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 12-03-2005 :
        - Initial implementation

-----------------------------------------------------------------------------*/
