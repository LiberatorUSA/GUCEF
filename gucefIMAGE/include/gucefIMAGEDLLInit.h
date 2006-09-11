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

#ifndef GUCEFIMAGEDLLINIT_H
#define GUCEFIMAGEDLLINIT_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEFIMAGE_MACROS_H
#include "gucefIMAGE_macros.h"         /* often used gucefIMAGE macros */
#define GUCEFIMAGE_MACROS_H
#endif /* GUCEFIMAGE_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef MSWIN_BUILD
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
  #ifdef LINUX_BUILD

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
          
#endif /* GUCEFIMAGEDLLINIT_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 12-02-2005 :
        - Initial implementation

-----------------------------------------------------------------------------*/