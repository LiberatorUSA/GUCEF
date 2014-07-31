/*
 *  inputdriverXWINMSG: GUCEF input driver module for X11
 *  Copyright (C) 2002 - 2012.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "inputdriverNANDROID.h"

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID )

#include <string.h>
#include <stdlib.h>
#include <android/input.h>
#include <android/native_activity.h>
#include <android_native_app_glue.h>

#include "gucefINPUT_keyboard.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

static const TVersion version = { 1, 0, 0, 0 };
#define DRIVER_NAME       "NANDROID\0"
#define DRIVER_COPYRIGHT  "Copyright (C) Dinand Vanvelzen. 2002 - 2012. License: LGPLv3.\0"
#define MAX_CONTEXTS      1

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

struct SContextData
{
    const TInputCallbacks* callbacks;
    char isInUse;
};
typedef struct SContextData TContextData;

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

static struct android_app* androidApp = 0;
static TContextData contexts[ MAX_CONTEXTS ];
static UInt32 keyModStates = 0;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

int
GetFreeContextSlot( void )
{
    int i;
    for ( i=0; i<MAX_CONTEXTS; ++i )
    {
        if ( 0 < contexts[ i ].isInUse )
        {
            return i;
        }
    }
    return -1;
}

/*--------------------------------------------------------------------------*/

void
InitContextSlots( void )
{
    memset( contexts, 0, MAX_CONTEXTS * sizeof( TContextData ) );
}

/*--------------------------------------------------------------------------*/

const char*
GetArgListItem( const int argc    ,
                const char** argv ,
                const char* key   )
{
    int i;
    for ( i=0; i<argc-1; ++i )
    {
        if ( strcmp( argv[ i ], key ) == 0 )
        {
            return argv[ i+1 ];
        }
        ++i;
    }
    return 0;
}

/*--------------------------------------------------------------------------*/

UInt32
ParseArgListItemUInt32( const int argc    ,
                        const char** argv ,
                        const char* key   )
{
    const char* value = GetArgListItem( argc, argv, key );
    if ( NULL != value )
    {
        UInt32 result = 0UL;
        sscanf( value, "%d", &result );
        return result;
    }
    return 0;
}

/*--------------------------------------------------------------------------*/

void*
ParseArgListItemPointer( const int argc    ,
                         const char** argv ,
                         const char* key   )
{
    const char* value = GetArgListItem( argc, argv, key );
    if ( NULL != value )
    {
        void* result = NULL;
        sscanf( value, "%p", &result );
        return result;
    }
    return 0;
}

/*--------------------------------------------------------------------------*/

KeyCode
MapAndroidKeyToGucef( int keycode )
{
    switch ( keycode )
    {
        case AKEYCODE_SOFT_LEFT: return KEYCODE_LEFT;
        case AKEYCODE_SOFT_RIGHT: return KEYCODE_RIGHT;
        case AKEYCODE_HOME: return KEYCODE_HOME;
        case AKEYCODE_BACK: return KEYCODE_WEBBACK;
        //	case AKEYCODE_CALL: return KEYCODE_UNASSIGNED;
        //	case AKEYCODE_ENDCALL: return KEYCODE_UNASSIGNED;
        case AKEYCODE_0: return KEYCODE_0;
        case AKEYCODE_1: return KEYCODE_1;
        case AKEYCODE_2: return KEYCODE_2;
        case AKEYCODE_3: return KEYCODE_3;
        case AKEYCODE_4: return KEYCODE_4;
        case AKEYCODE_5: return KEYCODE_5;
        case AKEYCODE_6: return KEYCODE_6;
        case AKEYCODE_7: return KEYCODE_7;
        case AKEYCODE_8: return KEYCODE_8;
        case AKEYCODE_9: return KEYCODE_9;
        case AKEYCODE_STAR: return KEYCODE_MULTIPLY;
        //	case AKEYCODE_POUND: return KEYCODE_UNASSIGNED;
        case AKEYCODE_DPAD_UP: return KEYCODE_UP;
        case AKEYCODE_DPAD_DOWN: return KEYCODE_DOWN;
        case AKEYCODE_DPAD_LEFT: return KEYCODE_LEFT;
        case AKEYCODE_DPAD_RIGHT: return KEYCODE_RIGHT;
        case AKEYCODE_DPAD_CENTER: return KEYCODE_UNASSIGNED;
        case AKEYCODE_VOLUME_UP: return KEYCODE_VOLUMEUP;
        case AKEYCODE_VOLUME_DOWN: return KEYCODE_VOLUMEDOWN;
        case AKEYCODE_POWER: return KEYCODE_POWER;
        case AKEYCODE_CAMERA: return KEYCODE_W;
        //case AKEYCODE_CLEAR: return KEYCODE_UNASSIGNED;
        case AKEYCODE_A: return KEYCODE_A;
        case AKEYCODE_B: return KEYCODE_B;
        case AKEYCODE_C: return KEYCODE_C;
        case AKEYCODE_D: return KEYCODE_D;
        case AKEYCODE_E: return KEYCODE_E;
        case AKEYCODE_F: return KEYCODE_F;
        case AKEYCODE_G: return KEYCODE_G;
        case AKEYCODE_H: return KEYCODE_H;
        case AKEYCODE_I: return KEYCODE_I;
        case AKEYCODE_J: return KEYCODE_J;
        case AKEYCODE_K: return KEYCODE_K;
        case AKEYCODE_L: return KEYCODE_L;
        case AKEYCODE_M: return KEYCODE_M;
        case AKEYCODE_N: return KEYCODE_N;
        case AKEYCODE_O: return KEYCODE_O;
        case AKEYCODE_P: return KEYCODE_P;
        case AKEYCODE_Q: return KEYCODE_Q;
        case AKEYCODE_R: return KEYCODE_R;
        case AKEYCODE_S: return KEYCODE_S;
        case AKEYCODE_T: return KEYCODE_T;
        case AKEYCODE_U: return KEYCODE_U;
        case AKEYCODE_V: return KEYCODE_V;
        case AKEYCODE_W: return KEYCODE_W;
        case AKEYCODE_X: return KEYCODE_X;
        case AKEYCODE_Y: return KEYCODE_Y;
        case AKEYCODE_Z: return KEYCODE_Z;
        case AKEYCODE_COMMA: return KEYCODE_COMMA;
        case AKEYCODE_PERIOD: return KEYCODE_PERIOD;
        case AKEYCODE_ALT_LEFT: return KEYCODE_LMENU;
        case AKEYCODE_ALT_RIGHT: return KEYCODE_RMENU;
        case AKEYCODE_SHIFT_LEFT: return KEYCODE_LSHIFT;
        case AKEYCODE_SHIFT_RIGHT: return KEYCODE_RSHIFT;
        case AKEYCODE_TAB: return KEYCODE_TAB;
        case AKEYCODE_SPACE: return KEYCODE_SPACE;
        case AKEYCODE_SYM: return KEYCODE_UNASSIGNED;
        case AKEYCODE_EXPLORER: return KEYCODE_MYCOMPUTER;
        case AKEYCODE_ENVELOPE: return KEYCODE_MAIL;
        case AKEYCODE_ENTER: return KEYCODE_RETURN;
        case AKEYCODE_DEL: return KEYCODE_DELETE;
        case AKEYCODE_GRAVE: return KEYCODE_GRAVE;
        case AKEYCODE_MINUS: return KEYCODE_MINUS;
        case AKEYCODE_EQUALS: return KEYCODE_EQUALS;
        case AKEYCODE_LEFT_BRACKET: return KEYCODE_LBRACKET;
        case AKEYCODE_RIGHT_BRACKET: return KEYCODE_RBRACKET;
        case AKEYCODE_BACKSLASH: return KEYCODE_BACKSLASH;
        case AKEYCODE_SEMICOLON: return KEYCODE_SEMICOLON;
        case AKEYCODE_APOSTROPHE: return KEYCODE_APOSTROPHE;
        case AKEYCODE_SLASH: return KEYCODE_SLASH;
        case AKEYCODE_AT: return KEYCODE_AT;
        case AKEYCODE_NUM: return KEYCODE_NUMLOCK;
        //	case AKEYCODE_HEADSETHOOK: return KEYCODE_UNASSIGNED;
        //	case AKEYCODE_FOCUS: return KEYCODE_UNASSIGNED;
        case AKEYCODE_PLUS: return KEYCODE_ADD;
        case AKEYCODE_MENU: return KEYCODE_LWIN;
        //	case AKEYCODE_NOTIFICATION: return KEYCODE_UNASSIGNED;
        case AKEYCODE_SEARCH: return KEYCODE_WEBSEARCH;
        case AKEYCODE_MEDIA_PLAY_PAUSE: return KEYCODE_PLAYPAUSE;
        case AKEYCODE_MEDIA_STOP: return KEYCODE_MEDIASTOP;
        case AKEYCODE_MEDIA_NEXT: return KEYCODE_NEXTTRACK;
        case AKEYCODE_MEDIA_PREVIOUS: return KEYCODE_PREVTRACK;
        //	case AKEYCODE_MEDIA_REWIND: return KEYCODE_UNASSIGNED;
        //	case AKEYCODE_MEDIA_FAST_FORWARD: return KEYCODE_UNASSIGNED;
        case AKEYCODE_MUTE: return KEYCODE_MUTE;
        case AKEYCODE_PAGE_UP: return KEYCODE_PGUP;
        case AKEYCODE_PAGE_DOWN: return KEYCODE_PGDOWN;
        //	case AKEYCODE_PICTSYMBOLS: return KEYCODE_UNASSIGNED;
        //	case AKEYCODE_SWITCH_CHARSET: return KEYCODE_UNASSIGNED;
        case AKEYCODE_BUTTON_A: return KEYCODE_A;
        case AKEYCODE_BUTTON_B: return KEYCODE_B;
        case AKEYCODE_BUTTON_C: return KEYCODE_C;
        case AKEYCODE_BUTTON_X: return KEYCODE_X;
        case AKEYCODE_BUTTON_Y: return KEYCODE_Y;
        case AKEYCODE_BUTTON_Z: return KEYCODE_Z;
        //	case AKEYCODE_BUTTON_L1: return KEYCODE_UNASSIGNED;
        //	case AKEYCODE_BUTTON_R1: return KEYCODE_UNASSIGNED;
        //	case AKEYCODE_BUTTON_L2: return KEYCODE_UNASSIGNED;
        //	case AKEYCODE_BUTTON_R2: return KEYCODE_UNASSIGNED;
        //	case AKEYCODE_BUTTON_THUMBL: return KEYCODE_UNASSIGNED;
        //	case AKEYCODE_BUTTON_THUMBR: return KEYCODE_UNASSIGNED;
        //	case AKEYCODE_BUTTON_START: return KEYCODE_UNASSIGNED;
        //	case AKEYCODE_BUTTON_SELECT: return KEYCODE_UNASSIGNED;
        //	case AKEYCODE_BUTTON_MODE: return KEYCODE_UNASSIGNED;
        default: return KEYCODE_UNASSIGNED;
    }
}

/*--------------------------------------------------------------------------*/

void
NotifyAllContextsOfKeyUp( KeyCode keyCode )
{
    int i;
    for ( i=0; i<MAX_CONTEXTS; ++i )
    {
        if ( 0 < contexts[ i ].isInUse )
        {
            contexts[ i ].callbacks->onKeyboardKeyUp( contexts[ i ].callbacks->userData, 0, keyCode, keyModStates );
        }
    }
}

/*--------------------------------------------------------------------------*/

void
NotifyAllContextsOfKeyDown( KeyCode keyCode )
{
    int i;
    for ( i=0; i<MAX_CONTEXTS; ++i )
    {
        if ( 0 < contexts[ i ].isInUse )
        {
            contexts[ i ].callbacks->onKeyboardKeyDown( contexts[ i ].callbacks->userData, 0, keyCode, keyModStates );
        }
    }
}

/*--------------------------------------------------------------------------*/

void
NotifyAllContextsOfMouseMove( int xPos, int yPos, int xDelta, int yDelta )
{
    int i;
    for ( i=0; i<MAX_CONTEXTS; ++i )
    {
        if ( 0 < contexts[ i ].isInUse )
        {
            contexts[ i ].callbacks->onMouseMove( contexts[ i ].callbacks->userData, 0, xPos, yPos, xDelta, yDelta );
        }
    }
}

/*--------------------------------------------------------------------------*/

static int32_t
OnAndroidInputEvent( struct android_app* app, AInputEvent* event )
{
    int eventType = AInputEvent_getType( event );
    switch ( eventType )
    {
        case AINPUT_EVENT_TYPE_KEY:
        {
            int androidKeyCode = AKeyEvent_getKeyCode( event );
            int androidKeyAction = AKeyEvent_getAction( event );
            KeyCode gucefKeyCode = MapAndroidKeyToGucef( androidKeyCode );
            if ( AKEY_EVENT_ACTION_DOWN == androidKeyAction )
            {
                NotifyAllContextsOfKeyDown( gucefKeyCode );
            }
            else
            if ( AKEY_EVENT_ACTION_UP == androidKeyAction )
            {
                NotifyAllContextsOfKeyUp( gucefKeyCode );
            }
            break;
        }
        case AINPUT_EVENT_TYPE_MOTION:
        {
            static int prevXPos = 0;
            static int prevYPos = 0;

            int xPos = AMotionEvent_getX( event, 0 );
            int yPos = AMotionEvent_getY( event, 0 );

            int xDelta = xPos - prevXPos;
            int yDelta = yPos - prevYPos;

            prevXPos = xPos;
            prevYPos = yPos;

            NotifyAllContextsOfMouseMove( xPos, yPos, xDelta, yDelta );

            break;
        }
    }

    return 0;
}

/*--------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_Init( void** plugdata   ,
                      const int argc    ,
                      const char** argv ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    struct android_app* state = (struct android_app*) ParseArgListItemPointer( argc, argv, "-android_app" );
    if ( NULL == state )
    {
        // init context data
        InitContextSlots();

        // hook up our handler into the android native app code
        state->onInputEvent = OnAndroidInputEvent;

        //
        return 1;
    }
    return 0;
}

/*--------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_Shutdown( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    return 1;
}

/*--------------------------------------------------------------------------*/

const char* GUCEF_PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_Name( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    return DRIVER_NAME;
}

/*--------------------------------------------------------------------------*/

const char* GUCEF_PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_Copyright( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    return DRIVER_COPYRIGHT;
}

/*--------------------------------------------------------------------------*/

const TVersion* GUCEF_PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_Version( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    return &version;
}

/*--------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_Update( void* plugdata    ,
                        void* contextdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    // we are updated automatically
    return 1;
}

/*--------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_CreateContext( void* plugdata                   ,
                               void** contextdata               ,
                               int argc                         ,
                               const char** argv                ,
                               const TInputCallbacks* callbacks ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{


    //GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "Created Native Android input context" );

    return 1;
}

/*--------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_DestroyContext( void* plugdata    ,
                                void* contextdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{

    return 1;
}

/*--------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_GetMousePos( void* plugdata    ,
                             void* contextdata ,
                             UInt32* xpos      ,
                             UInt32* ypos      ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    return 0;
}

/*--------------------------------------------------------------------------*/

UInt8* GUCEF_PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_GetKeyBoardKeyStates( void* plugdata    ,
                                      void* contextdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    return 0;
}

/*--------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_GetMouseButtonPressedState( void* plugdata           ,
                                            void* contextdata        ,
                                            const UInt32 buttonindex ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    return 0;
}

/*--------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_GetKeyboardKeyPressedState( void* plugdata        ,
                                            void* contextdata     ,
                                            const UInt32 keyindex ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    return 0;
}

/*--------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_GetDeviceBoolState( void* plugdata          ,
                                    void* contextdata       ,
                                    const UInt32 deviceid   ,
                                    const UInt32 stateindex ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    /* not supported */
    return 0;
}

/*--------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_GetDeviceVarState( void* plugdata          ,
                                   void* contextdata       ,
                                   const UInt32 deviceid   ,
                                   const UInt32 stateindex ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
    /* not supported */
    return 0;
}

/*--------------------------------------------------------------------------*/

#endif /* GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ? */

/*--------------------------------------------------------------------------*/
