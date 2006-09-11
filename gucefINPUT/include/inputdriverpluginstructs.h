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

#ifndef INPUTDRIVERPLUGINSTRUCTS_H
#define INPUTDRIVERPLUGINSTRUCTS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_INPUT_MACROS_H
#include "gucefINPUT_macros.h"
#define GUCEF_INPUT_MACROS_H
#endif /* GUCEF_INPUT_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
namespace GUCEF {
namespace INPUT {
#endif /* __cplusplus */

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTCALLBACK_OnMouseButtonDown )  ( void* userData, const UInt32 buttonindex ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTCALLBACK_OnMouseButtonUp )    ( void* userData, const UInt32 buttonindex ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTCALLBACK_OnMouseMove )        ( void* userData, const Int32 xPos, const Int32 yPos, const Int32 xDelta, const Int32 yDelta ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTCALLBACK_OnMouseVarChanged )  ( void* userData, const UInt32 varIndex, const Int32 value, const Int32 valueDelta ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTCALLBACK_OnKeyboardKeyDown )  ( void* userData, const UInt32 keyindex, const UInt32 keyModState ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTCALLBACK_OnKeyboardKeyUp )    ( void* userData, const UInt32 keyindex, const UInt32 keyModState ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTCALLBACK_OnDeviceBooleanOn )  ( void* userData, const UInt32 deviceid, const UInt32 stateindex ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTCALLBACK_OnDeviceBooleanOff ) ( void* userData, const UInt32 deviceid, const UInt32 stateindex ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;
typedef void ( GUCEF_PLUGIN_CALLSPEC_PREFIX *TINPUTCALLBACK_OnDeviceVarChanged ) ( void* userData, const UInt32 deviceid, const UInt32 stateindex, const Float32 value ) GUCEF_PLUGIN_CALLSPEC_SUFFIX;

/*-------------------------------------------------------------------------*/

struct SInputCallbacks
{
        TINPUTCALLBACK_OnMouseButtonDown  onMouseButtonDown;
        TINPUTCALLBACK_OnMouseButtonUp    onMouseButtonUp;
        TINPUTCALLBACK_OnMouseMove        onMouseMove;
        TINPUTCALLBACK_OnMouseVarChanged  onMouseVarChanged;
        TINPUTCALLBACK_OnKeyboardKeyDown  onKeyboardKeyDown;
        TINPUTCALLBACK_OnKeyboardKeyUp    onKeyboardKeyUp;
        TINPUTCALLBACK_OnDeviceBooleanOn  onDeviceBooleanOn;
        TINPUTCALLBACK_OnDeviceBooleanOff onDeviceBooleanOff;
        TINPUTCALLBACK_OnDeviceVarChanged onDeviceVarChanged;
        void* userData;
};
typedef struct SInputCallbacks TInputCallbacks;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
}; /* namespace INPUT */
}; /* namespace GUCEF */
#endif /* __cplusplus */

/*-------------------------------------------------------------------------*/
          
#endif /* INPUTDRIVERPLUGINSTRUCTS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 08-10-2005 :
        - Initial implementation

-----------------------------------------------------------------------------*/
