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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windowsx.h>
#include <windows.h>
#include <winuser.h>

#include "DLLMainInputDriverMSWINMSG.h"

#ifndef GUCEFINPUT_MACROS_H
#include "gucefINPUT_macros.h"    /* gucefINPUT macros, used here for the export and callspec macros */
#define GUCEFINPUT_MACROS_H
#endif /* GUCEFINPUT_MACROS_H ? */ 

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
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

static const TVersion version = { 1, 0, 0, 0 };
#define DRIVER_NAME       "MSWINMSG\0"
#define DRIVER_COPYRIGHT  "Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.\0"

#define MAX_NR_OF_CONTEXTS      32
#define KEYBOARD_DEVICEID       0
#define MOUSE_DEVICEID          1

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

struct SContextData
{
        HWND hWnd;
        WNDPROC PrevWindowProcedurePtr;
        TInputCallbacks callbacks;
        BYTE inUse;
        BYTE keyStateBuffer[ 256 ];
        UInt32 keyModState;
        UInt32 mouseXPos;
        UInt32 mouseYPos;
        UInt32 mouseWheelPos;
        BYTE doGrab;           /* grab the cursor and confine it to the window ? */
         
};
typedef struct SContextData TContextData;

/*---------------------------------------------------------------------------*/
        
struct SDriverData
{
        TContextData contextList[ MAX_NR_OF_CONTEXTS ];
        UInt32 contextCount;
};
typedef struct SDriverData TDriverData;

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

static TDriverData* driverData = NULL;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

UInt32
ParseArgListItemUInt32( const char*** args ,
                        const char* key    )
{       
        UInt32 i=0;
        while ( args[ i ] )
        {
                if ( strcmp( args[ i ][ 0 ], key ) == 0 )
                {
                        UInt32 value = 0UL;
                        sscanf( args[ i ][ 1 ], "%d", &value );
                        return value;
                }
                ++i;
        }
        return 0;
}

/*---------------------------------------------------------------------------*/

void
UpdateKeyModState( TContextData* data )
{         
        GetKeyState( VK_SHIFT ) & 0x80 ? SETBITXON( data->keyModState, 4 ) : SETBITXOFF( data->keyModState, 4 );
        GetKeyState( VK_MENU ) & 0x80 ? SETBITXON( data->keyModState, 0 ) : SETBITXOFF( data->keyModState, 0 );
        GetKeyState( VK_CONTROL ) & 0x80 ? SETBITXON( data->keyModState, 2 ) : SETBITXOFF( data->keyModState, 2 );     
}

/*---------------------------------------------------------------------------*/

TContextData*
GetFreeContext( void )
{
        if ( driverData->contextCount < MAX_NR_OF_CONTEXTS )
        {
                UInt32 i;
                for ( i=0; i<MAX_NR_OF_CONTEXTS; ++i )
                {
                        if ( 0 == driverData->contextList[ i ].inUse )
                        {
                                return &driverData->contextList[ i ];
                        }
                }
                return NULL;
        }
        return NULL;
}

/*---------------------------------------------------------------------------*/

TContextData*
LookupDataForHWND( HWND whnd )
{
        UInt32 i, checked = 0;
        for ( i=0; i<MAX_NR_OF_CONTEXTS; ++i )
        {
                if ( 1 == driverData->contextList[ i ].inUse )
                {
                        ++checked;
                        if ( driverData->contextList[ i ].hWnd == whnd )
                        {
                                return &driverData->contextList[ i ];
                        }
                }
                
                if ( checked == driverData->contextCount )
                {
                        return NULL;
                }                        
        }
        return NULL;        
}

/*---------------------------------------------------------------------------*/

BOOL
ClientRectToScreenRect( HWND hwnd, RECT* rect )
{
        POINT screenPos;
        screenPos.x = rect->left;
        screenPos.y = rect->top;
        if ( ClientToScreen( hwnd, &screenPos ) )
        {
	        rect->left = screenPos.x;
	        rect->top = screenPos.y;
	        
                screenPos.x = rect->right;
	        screenPos.y = rect->bottom;
	        
	        if ( ClientToScreen( hwnd, &screenPos ) )
	        {
	                rect->right = screenPos.x;
	                rect->bottom = screenPos.y;
	                
	                return 1;
	        }	        		                
        }
        return 0;
}

/*---------------------------------------------------------------------------*/

UInt32
GrabWindowCursorInput( TContextData* data ,
                       const BYTE doGrab  )
{
	if ( data->doGrab == doGrab ) return 0;
	
	if ( doGrab ) 
	{	        			
		RECT screenRect;
		if ( GetClientRect( data->hWnd, &screenRect ) )
		{
		        /*
		         *      Now we have to translate to window coordinates
		         */
		        if ( ClientRectToScreenRect( data->hWnd, &screenRect ) )
		        { 
		                if ( ClipCursor( &screenRect ) )
		                {
		                        /*      
		                         *      successfull cursor grab ,
		                         *      we now place the cursor in the center of the window
		                         */
		                        POINT cursorPos, screenPos;
		                        cursorPos.x = ( screenRect.right - screenRect.left ) / 2;
		                        cursorPos.y = ( screenRect.bottom - screenRect.top ) / 2;
		                        screenPos = cursorPos; 
		                        if ( ClientToScreen( data->hWnd, &screenPos ) )
		                        {
		                                if ( SetCursorPos( screenPos.x, screenPos.y ) )
		                                {
		                                        data->doGrab = 1;
		                                        
		                                        /*
		                                         *      Send an event to the user so he can adjust for the cursor warp
		                                         */
		                                        data->callbacks.onMouseMove( data->callbacks.userData, MOUSE_DEVICEID, cursorPos.x, cursorPos.y, cursorPos.x - data->mouseXPos, cursorPos.y - data->mouseYPos );
                                                        data->mouseXPos = cursorPos.x;
                                                        data->mouseYPos = cursorPos.y;
                                                        
                                                        // temp hack:
                                                        ShowCursor( 0 );
		                                        return 1;
                                                }
                                        }
                                }                                        
		        }
                }
        }
        else
        {
		/*
		 *      Allow the cursor to move anywhere
		 */
		ClipCursor( NULL );
		
		data->doGrab = 0;
				
		//temp hack:
		ShowCursor( 1 );
		return 1;		
        }
        return 0;
}

/*---------------------------------------------------------------------------*/

LRESULT
InputDriverProcessMSWINMSG( HWND whnd     , 
                            UINT message  , 
                            WPARAM wparam , 
                            LPARAM lparam )
{
        TContextData* data = LookupDataForHWND( whnd );
        if ( NULL == data ) return 0;
        
        switch ( message )
        {
                case WM_CHAR :
                {
                        /* 
                         *      the key down & action has been translated into a char
                         *      In ASCII mode we should only use this
                         */
                        break;
                }
                case WM_KEYUP :
                {
                        /* 
                         *      In non-ASCII mode we should only use this and ignore WM_CHAR
                         */
                        switch ( wparam )
                        {
                                case VK_SHIFT :
                                {
                                        SETBITXOFF( data->keyModState, 4 );
                                        SETBITXOFF( data->keyModState, 5 );
                                        break;                                        
                                }
                                case VK_MENU :
                                {
                                        SETBITXOFF( data->keyModState, 0 );
                                        SETBITXOFF( data->keyModState, 1 );
                                        break;                                        
                                } 
                                case VK_CONTROL :
                                {
                                        SETBITXOFF( data->keyModState, 2 );
                                        SETBITXOFF( data->keyModState, 3 );
                                        break;                                        
                                }                                                               
                                default :
                                {
                                        break;
                                }
                        }                        
                        data->keyStateBuffer[ wparam ] = 0;                          /* , data->keyModState <- modifiers */
                        data->callbacks.onKeyboardKeyUp( data->callbacks.userData, KEYBOARD_DEVICEID, (KeyCode)wparam );
                        break;
                }
                case WM_KEYDOWN :
                {
                        switch ( wparam )
                        {
                                case VK_CAPITAL :
                                {
                                        /* the capital or CAPS key is a toggle key and should be handled as such */
                                        GETBITX( data->keyModState, 6 ) ? SETBITXOFF( data->keyModState, 6 ) : SETBITXON( data->keyModState, 6 );
                                        break;
                                }
                                case VK_SHIFT :
                                {
                                        SETBITXON( data->keyModState, 4 );
                                        SETBITXON( data->keyModState, 5 );
                                        break;                                        
                                }
                                case VK_MENU :
                                {
                                        SETBITXON( data->keyModState, 0 );
                                        SETBITXON( data->keyModState, 1 );
                                        break;                                        
                                } 
                                case VK_CONTROL :
                                {
                                        SETBITXON( data->keyModState, 2 );
                                        SETBITXON( data->keyModState, 3 );
                                        break;                                        
                                }                                                               
                                default :
                                {
                                        break;
                                }
                        }                        
                        data->keyStateBuffer[ wparam ] = 1;                           /* , data->keyModState <- modifiers */
                        data->callbacks.onKeyboardKeyDown( data->callbacks.userData, KEYBOARD_DEVICEID, (KeyCode)wparam ); 
                        break;               
                }
                case WM_MOUSEMOVE :
                {
                        UInt32 xPos = GET_X_LPARAM( lparam );
                        UInt32 yPos = GET_Y_LPARAM( lparam );
                        data->callbacks.onMouseMove( data->callbacks.userData, MOUSE_DEVICEID, xPos, yPos, xPos - data->mouseXPos, yPos - data->mouseYPos );
                        data->mouseXPos = xPos;
                        data->mouseYPos = yPos;  
                        break;
                }
                case 0x020A : // WM_MOUSEWHEEL :
                {
                        UInt32 zDelta = (short)HIWORD(wparam);
                        
                        UInt32 xPos = GET_X_LPARAM( lparam );
                        UInt32 yPos = GET_Y_LPARAM( lparam );
                        data->callbacks.onMouseMove( data->callbacks.userData, MOUSE_DEVICEID, xPos, yPos, xPos - data->mouseXPos, yPos - data->mouseYPos );
                        data->mouseXPos = xPos;
                        data->mouseYPos = yPos;  

                        //zDelta = GET_WHEEL_DELTA_WPARAM( wParam ); // macro in MSDN but unknown                        
                        data->callbacks.onMouseVarChanged( data->callbacks.userData, MOUSE_DEVICEID, 0, data->mouseWheelPos, zDelta );
                        data->mouseWheelPos += zDelta;
                        break;
                }
                case WM_LBUTTONDOWN :
                {
                        UInt32 xPos = GET_X_LPARAM( lparam );
                        UInt32 yPos = GET_Y_LPARAM( lparam );
                        data->callbacks.onMouseMove( data->callbacks.userData, MOUSE_DEVICEID, xPos, yPos, xPos - data->mouseXPos, yPos - data->mouseYPos );
                        data->mouseXPos = xPos;
                        data->mouseYPos = yPos;  
                        
                        data->callbacks.onMouseButtonDown( data->callbacks.userData, MOUSE_DEVICEID, 0 );
                        break;
                }
                case WM_LBUTTONUP :
                {
                        WORD xPos = GET_X_LPARAM( lparam );
                        WORD yPos = GET_Y_LPARAM( lparam );
                        data->callbacks.onMouseMove( data->callbacks.userData, MOUSE_DEVICEID, xPos, yPos, xPos - data->mouseXPos, yPos - data->mouseYPos );
                        data->mouseXPos = xPos;
                        data->mouseYPos = yPos;  
                        
                        data->callbacks.onMouseButtonUp( data->callbacks.userData, MOUSE_DEVICEID, 0 );
                        break;
                }                
                case WM_MBUTTONDOWN :
                {
                        WORD xPos = GET_X_LPARAM( lparam );
                        WORD yPos = GET_Y_LPARAM( lparam );
                        data->callbacks.onMouseMove( data->callbacks.userData, MOUSE_DEVICEID, xPos, yPos, xPos - data->mouseXPos, yPos - data->mouseYPos );
                        data->mouseXPos = xPos;
                        data->mouseYPos = yPos;  
                        
                        data->callbacks.onMouseButtonDown( data->callbacks.userData, MOUSE_DEVICEID, 1 );
                        break;
                }
                case WM_MBUTTONUP :
                {
                        WORD xPos = GET_X_LPARAM( lparam );
                        WORD yPos = GET_Y_LPARAM( lparam );
                        data->callbacks.onMouseMove( data->callbacks.userData, MOUSE_DEVICEID, xPos, yPos, xPos - data->mouseXPos, yPos - data->mouseYPos );
                        data->mouseXPos = xPos;
                        data->mouseYPos = yPos; 
                        
                        data->callbacks.onMouseButtonUp( data->callbacks.userData, MOUSE_DEVICEID, 1 );
                        break;
                }                                 
                case WM_RBUTTONDOWN :
                {
                        WORD xPos = GET_X_LPARAM( lparam );
                        WORD yPos = GET_Y_LPARAM( lparam );
                        data->callbacks.onMouseMove( data->callbacks.userData, MOUSE_DEVICEID, xPos, yPos, xPos - data->mouseXPos, yPos - data->mouseYPos );
                        data->mouseXPos = xPos;
                        data->mouseYPos = yPos;  
                        
                        data->callbacks.onMouseButtonDown( data->callbacks.userData, MOUSE_DEVICEID, 2 );
                        break;
                }
                case WM_RBUTTONUP :
                {
                        WORD xPos = GET_X_LPARAM( lparam );
                        WORD yPos = GET_Y_LPARAM( lparam );
                        data->callbacks.onMouseMove( data->callbacks.userData, MOUSE_DEVICEID, xPos, yPos, xPos - data->mouseXPos, yPos - data->mouseYPos );
                        data->mouseXPos = xPos;
                        data->mouseYPos = yPos; 
                        
                        data->callbacks.onMouseButtonUp( data->callbacks.userData, MOUSE_DEVICEID, 2 );
                        break;   
                }                
                default : 
                {
                        break;
                }                
        }        
        
        return CallWindowProc( data->PrevWindowProcedurePtr ,
                               whnd                         ,
                               message                      ,
                               wparam                       ,
                               lparam                       );
}                   

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_Init( void** plugdata    ,
                      const char*** args ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
        if ( NULL != driverData )
        {
                /* we should not get here, init called more then once ? */
                return 1;
        }
        *plugdata = driverData = ( TDriverData* ) malloc( sizeof( TDriverData ) );
        
        if ( NULL == driverData )
        {
                /* allocation failed */
                return 0;
        }
        
        memset( driverData, 0, sizeof( TDriverData ) );
        return 1;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_Shutdown( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
        free( driverData );
        driverData = NULL;
        return 1;                        
}

/*---------------------------------------------------------------------------*/

const char* GUCEF_PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_Name( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
        return DRIVER_NAME;
}

/*---------------------------------------------------------------------------*/

const char* GUCEF_PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_Copyright( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
        return DRIVER_COPYRIGHT;
}

/*---------------------------------------------------------------------------*/

const TVersion* GUCEF_PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_Version( void* plugdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
        return &version;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_Update( void* plugdata    , 
                        void* contextdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
        if ( plugdata && contextdata )
        {
                TContextData* data = (TContextData*) contextdata;
                
                //OpenInputDesktop(
                
                POINT cursorPos;
                if ( GetCursorPos( &cursorPos ) )
                {                        
                        if ( ScreenToClient( data->hWnd, &cursorPos ) )
                        {
                                if ( ( cursorPos.x != data->mouseXPos ) ||
                                     ( cursorPos.y != data->mouseYPos )  )
                                {                             
                                        data->callbacks.onMouseMove( data->callbacks.userData, MOUSE_DEVICEID, cursorPos.x, cursorPos.y, cursorPos.x - data->mouseXPos, cursorPos.y - data->mouseYPos );
                                        data->mouseXPos = cursorPos.x;
                                        data->mouseYPos = cursorPos.y;                        
                                }
                        }                                                                
                }
                
        }
        return 1;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_CreateContext( void* plugdata                   ,
                               void** contextdata               ,
                               const char*** args               ,
                               const TInputCallbacks* callbacks ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
        TContextData* data;
        *contextdata = data = GetFreeContext();
        
        /* make sure we didnt hit the context maximum */
        if ( NULL == data ) return 0;

        data->callbacks = *callbacks; /* copy our callbacks */
        data->inUse = 1;              /* we are using this context */
        data->doGrab = 0;             /* default win32 is no input cursor grabbing */
        
        /* 
         *      get the window handle, this is always passed for MSWIN input drivers 
         */
        #pragma warning( disable: 4312 )
        data->hWnd = (HWND) ParseArgListItemUInt32( args     ,
                                                    "WINDOW" );
                
        /*
         *      Retrieve a pointer to the current window message handler
         *      we will have to restore this link if the context is destroyed !!!
         */
        data->PrevWindowProcedurePtr = (WNDPROC) GetWindowLongPtr( data->hWnd  ,
                                                                   GWL_WNDPROC );

        /*
         *      use per instance window sub-classing 
         *      we redirect windows messages to our function which will process 
         *      and then forward the messages
         */
        #pragma warning( disable: 4244 ) // 'argument' : conversion from 'LONG_PTR' to 'LONG', possible loss of data
        SetWindowLong( data->hWnd                            ,
                       GWL_WNDPROC                           ,
                       (LONG_PTR) InputDriverProcessMSWINMSG );

        /*
         *      Confine the mouse to the window and hide the windows cursor
         */
        GrabWindowCursorInput( data, 1 ); 
        
        callbacks->onKeyboardAttached( callbacks->userData, KEYBOARD_DEVICEID );
        callbacks->onMouseAttached( callbacks->userData, MOUSE_DEVICEID );
                               
        return 1;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_DestroyContext( void* plugdata    , 
                                void* contextdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
        TContextData* data = (TContextData*) contextdata;

        /*
         *      Restore the link to the old window handler
         */        
        #pragma warning( disable: 4244 ) // 'argument' : conversion from 'LONG_PTR' to 'LONG', possible loss of data
        SetWindowLong( data->hWnd                              ,
                       GWL_WNDPROC                             ,
                       (LONG_PTR) data->PrevWindowProcedurePtr );
                                        
        /* make sure we release our grab on the cursor */
        GrabWindowCursorInput( data, 0 );

        data->callbacks.onKeyboardDetached( data->callbacks.userData, KEYBOARD_DEVICEID );
        data->callbacks.onMouseDetached( data->callbacks.userData, MOUSE_DEVICEID );
        
        return 1;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_GetMousePos( void* plugdata    , 
                             void* contextdata , 
                             UInt32* xpos      , 
                             UInt32* ypos      ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
        TContextData* data = (TContextData*) contextdata;
        return 1;
}

/*---------------------------------------------------------------------------*/

UInt8* GUCEF_PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_GetKeyBoardKeyStates( void* plugdata    , 
                                      void* contextdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
        TContextData* data = (TContextData*) contextdata;
        return data->keyStateBuffer;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_GetMouseButtonPressedState( void* plugdata           , 
                                            void* contextdata        , 
                                            const UInt32 buttonindex ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
        TContextData* data = (TContextData*) contextdata;                         
        return 0;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_GetKeyboardKeyPressedState( void* plugdata        , 
                                            void* contextdata     , 
                                            const UInt32 keyindex ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
        TContextData* data = (TContextData*) contextdata;
        if ( keyindex < 256 )
        {
                return data->keyStateBuffer[ keyindex ];                
        }
        return 0;                
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_GetDeviceBoolState( void* plugdata          , 
                                    void* contextdata       , 
                                    const UInt32 deviceid   , 
                                    const UInt32 stateindex ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
        /* not supported */
        return 0;
}

/*---------------------------------------------------------------------------*/

UInt32 GUCEF_PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_GetDeviceVarState( void* plugdata          , 
                                   void* contextdata       , 
                                   const UInt32 deviceid   , 
                                   const UInt32 stateindex ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{
        /* not supported */
        return 0;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
}; /* namespace INPUT */
}; /* namespace GUCEF */
#endif /* __cplusplus */

/*---------------------------------------------------------------------------*/
