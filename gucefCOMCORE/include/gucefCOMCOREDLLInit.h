/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
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

#ifndef GUCEFCOMCOREDLLINIT_H
#define GUCEFCOMCOREDLLINIT_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_COMCORE_ETYPES_H
#include "gucefCOMCORE_ETypes.h"                      /* platform types */
#define GUCEF_COMCORE_ETYPES_H
#endif /* GUCEF_COMCORE_ETYPES_H ? */

#ifndef GUCEFCOMCORE_MACROS_H
#include "gucefCOMCORE_macros.h"         /* often used gucefCOMCORE macros */
#define GUCEFCOMCORE_MACROS_H
#endif /* GUCEFCOMCORE_MACROS_H ? */

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
			return 1;                              \
		}                                              \
		return 1;                                      \
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
          
#endif /* GUCEFCOMCOREDLLINIT_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 12-03-2005 :
        - Initial implementation

-----------------------------------------------------------------------------*/