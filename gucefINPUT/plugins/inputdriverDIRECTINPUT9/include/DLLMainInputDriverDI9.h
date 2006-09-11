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
 
#ifndef DLLMAININPUTDRIVERDI8_H
#define DLLMAININPUTDRIVERDI8_H

/*
 *      API for a Input driver plugin module
 */
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

#ifndef ESTRUCTS_H
#include "EStructs.h"           /* gucefCORE structures, needed for TVersion */
#define ESTRUCTS_H
#endif /* ESTRUCTS_H ? */

#ifndef GUCEFINPUT_MACROS_H
#include "gucefINPUT_macros.h"    /* gucefINPUT macros, used here for the export and callspec macros */
#define GUCEFINPUT_MACROS_H
#endif /* GUCEFINPUT_MACROS_H ? */ 
 
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
INPUTDRIVERPLUG_Init( void** plugdata ) PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/

EXPORT_C UInt32 PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_Shutdown( void* plugdata ) PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/

EXPORT_C const char* PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_Name( void* plugdata ) PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/

EXPORT_C const char* PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_Copyright( void* plugdata ) PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/

EXPORT_C const TVersion* PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_Version( void* plugdata ) PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/

EXPORT_C UInt32 PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_Update( void* plugdata    , 
                        void* contextdata ) PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/

EXPORT_C UInt32 PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_CreateContext( void* plugdata     , 
                               void** contextdata ) PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/

EXPORT_C UInt32 PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_DestroyContext( void* plugdata    , 
                                void* contextdata ) PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/

EXPORT_C UInt32 PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_GetMousePos( void* plugdata    , 
                             void* contextdata , 
                             Float32* xpos     , 
                             Float32* ypos     ) PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/

EXPORT_C UInt32 PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_GetKeyBoardKeyStates( void* plugdata    , 
                                      void* contextdata ) PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/

EXPORT_C UInt32 PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_GetMouseButtonPressedState( void* plugdata           , 
                                            void* contextdata        , 
                                            const UInt32 buttonindex ) PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/

EXPORT_C UInt32 PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_GetKeyboardKeyPressedState( void* plugdata        , 
                                            void* contextdata     , 
                                            const UInt32 keyindex ) PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/

EXPORT_C UInt32 PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_GetDeviceBoolState( void* plugdata          , 
                                    void* contextdata       , 
                                    const UInt32 deviceid   , 
                                    const UInt32 stateindex ) PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/

EXPORT_C UInt32 PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_GetDeviceVarState( void* plugdata          , 
                                   void* contextdata       , 
                                   const UInt32 deviceid   , 
                                   const UInt32 stateindex ) PLUGIN_CALLSPEC_SUFFIX;

/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
   }
#endif /* __cplusplus */

/*--------------------------------------------------------------------------*/

#endif /* DLLMAININPUTDRIVERDI8_H ? */

/*--------------------------------------------------------------------------//
//                                                                          //
//      Info & Changes                                                      //
//                                                                          //
//--------------------------------------------------------------------------//

- 27-03-2005 :
        - Created this set of functions.

----------------------------------------------------------------------------*/
