/*
 *  gucefINPUT: GUCEF module providing input device interaction
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
