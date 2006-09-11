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

/*
 *      API for a Input driver plugin module
 */
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

#include <stdlib.h>

#include <DirectInput9.h>

#include "DLLMainInputDriverDI9.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

const TVersion version = { 1, 0, 0, 0 };
#define DRIVER_NAME       "DirectInput9\0"
#define DRIVER_COPYRIGHT  "Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.\0"

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

struct SDI8Data
{
        /* system data */
        HWND hWnd;
        HINSTANCE hInstance;
        
        /*8 Element Array, TRUE if Button [n] is up */
        BOOL* bMouseUp;

        /* Mouse Position */
        POINT* pMousePos;

        /* Cursor HotSpot */
        POINT* pHotSpot;

        /* DI Device Objects */
        LPDIRECTINPUTDEVICE8 pDIJoy;
        LPDIRECTINPUTDEVICE8 pDIKeybrd;
        LPDIRECTINPUTDEVICE8 pDIMouse;

        /* Device State Buffers */
        DIJOYSTATE2 js;
        DIMOUSESTATE2 MouseState;
        UByte buffer[ 256 ];
        DWORD dwElements;
};
typedef struct SDI8Data TDI8Data;
         
/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

HRESULT 
InitDI8( TDI8Data* data )
{	
        /* Create The DI Object */
        if( FAILED( DirectInput8Create( data->hInstance     ,  
                                        DIRECTINPUT_VERSION , 
                                        IID_IDirectInput8   , 
                                        (void**)&data->pDI  , 
                                        NULL                ) ) )
        {                                         
                return E_FAIL;
        }                
	
	/* Set up keyboard input */
	if( FAILED( data->pDI->CreateDevice( GUID_SysKeyboard , 
	                                     &data->pDIKeybrd , 
	                                     NULL             ) ) )
        {	                                     
		return E_FAIL;
        }
        
	if( FAILED( data->pDIKeybrd->SetDataFormat( &c_dfDIKeyboard ) ) )
	{
	        return E_FAIL;
        }	        

        if( FAILED( data->pDIKeybrd->SetCooperativeLevel( data->hWnd                            , 
                                                          DISCL_FOREGROUND | DISCL_NONEXCLUSIVE ) ) )
        {                                                          
                return E_FAIL;
        }                

        if ( data->pDIKeybrd ) 
        {
                data->pDIKeybrd->Acquire(); 
        }                

        /* Set up mouse input */

        if( FAILED( data->pDI->CreateDevice( GUID_SysMouse   , 
                                             &data->pDIMouse , 
                                             NULL            ) ) )
        {                                             
                return E_FAIL;
        }                

        if( FAILED( data->pDIMouse->SetDataFormat( &c_dfDIMouse2 ) ) )
        {
                return E_FAIL;
        }

        if( FAILED( data->pDIMouse->SetCooperativeLevel( hWnd                               , 
                                                         DISCL_FOREGROUND | DISCL_EXCLUSIVE ) ) )
        {                                                         
                return E_FAIL;
        }                

        data->hMouse = CreateEvent( 0, 0, 0, 0 );

        if ( data->hMouse == NULL )
        {
                DIShutdown();
                return 0;
        }

        hr = data->pDIMouse->SetEventNotification( data->hMouse );

        if ( FAILED( hr ) )
        {
                DIShutdown();
                return 0;
        }

        //initialize input buffer
        DIPROPDWORD dipdw; 

        dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
        dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        dipdw.diph.dwObj        = 0;
        dipdw.diph.dwHow        = DIPH_DEVICE;
        dipdw.dwData            = 16;

        hr = data->pDIMouse->SetProperty( DIPROP_BUFFERSIZE , 
                                          &dipdw.diph       );

        if ( FAILED( hr ) )
        {
                DIShutdown();
                return 0;
        }

        if ( data->pDIMouse ) 
        {
                data->pDIMouse->Acquire();
        }                

	/*Set up Joystick input (Only the first joy) */

        if( FAILED( data->pDI->EnumDevices( DI8DEVCLASS_GAMECTRL  , 
                                            EnumJoysticksCallback , 
                                            NULL                  , 
                                            DIEDFL_ATTACHEDONLY   ) ) )
        {
                return E_FAIL;
        }                

        if( FAILED( data->pDIJoy->SetDataFormat( &c_dfDIJoystick2 ) ) )
        {
                return E_FAIL;
        }                

        if( FAILED( data->pDIJoy->SetCooperativeLevel( data->hWnd                         ,  
                                                       DISCL_EXCLUSIVE | DISCL_FOREGROUND ) ) )
        {                                                       
                return E_FAIL;
        }                

        data->DIDevCaps.dwSize = sizeof( DIDEVCAPS );
        if ( FAILED( data->pDIJoy->GetCapabilities( &data->DIDevCaps ) ) )
        {
                return E_FAIL;
        }                

        if ( FAILED( data->pDIJoy->EnumObjects( EnumAxesCallback   , 
                                                (VOID*) data->hWnd , 
                                                DIDFT_AXIS         ) ) )
        {                                                
                return E_FAIL;
        }                

        if( data->pDIJoy ) 
        {
                m_pDIJoy->Acquire();
        }
        
	return S_OK;
}

/*---------------------------------------------------------------------------*/

void
ShutdownDI8( TDI8Data* data )
{
        SAFE_DELETE( data->pVB);

        if ( data->pDI )
        {
                if ( data->pDIKeybrd )
                {
                        data->pDIKeybrd->Unacquire();
                        data->pDIKeybrd->Release();
                        data->pDIKeybrd = NULL;
                }

                if ( data->pDIMouse )
                {
                        data->pDIMouse->Unacquire();
                        data->pDIMouse->Release();
                        data->pDIMouse = NULL;
                }
        
                if ( data->pDIJoy )
                {
                        data->pDIJoy->Unacquire();
                        data->pDIJoy->Release();
                        data->pDIJoy = NULL;
                }
                data->pDI->Release();
                data->pDI = NULL;
        }	
}

/*---------------------------------------------------------------------------*/

HRESULT
ProcessKeyboardDI8( TDI8Data* data )
{
        /* Check for keyboard input and fill buffer */
        hr = data->pDIKeybrd->GetDeviceState( sizeof( buffer ) ,
                                             (LPVOID) &buffer  ); 
        if ( FAILED( hr ) ) 
        { 
                data->pDIKeybrd->Acquire();                
                return E_FAIL; 
        }

        return S_OK;       
}

/*---------------------------------------------------------------------------*/

UInt32 PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_Init( void** plugdata ) PLUGIN_CALLSPEC_SUFFIX
{
        UInt32 i;
        TDI8Data* data = (TDI8Data*) malloc( sizeof( TDI8Data ) );
        *plugdata = data;
        
        data->pDI = NULL;
        data->pDIKeybrd	= NULL;
        data->pDIMouse = NULL;
        data->pDIJoy = NULL;
        data->pMousePos	= (POINT*) malloc( sizeof( POINT ) );
        data->pMousePos->x = 0l;
        data->pMousePos->y = 0l;
        data->pHotSpot = (POINT*) malloc( sizeof( POINT ) );
        data->pHotSpot->x = 0l;
        data->pHotSpot->y = 0l;
        data->pVB = NULL;
        data->pCursorTex = NULL;
        data->dwElements = 16;
        data->bMouseUp = (BOOL*) malloc( 8 * sizeof( BOOL ) );
        
        for( i=0; i<8; i++ )
        {
                data->bMouseUp[ i ] = TRUE;
        }
        
        if ( FAILED( InitDI8( data ) )
        {
                free( data );
                *plugdata = NULL;
                return 0;        
        }
        return 1;
}

/*---------------------------------------------------------------------------*/

UInt32 PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_Shutdown( void* plugdata ) PLUGIN_CALLSPEC_SUFFIX
{
        TDI8Data* data = (TDI8Data*) plugdata;

        ShutdownDI8( data );
        
        SAFE_DELETE( data->pMousePos );
        SAFE_DELETE( data->pHotSpot );
        SAFE_DELETE( data->pCursorTex );
        SAFE_DELETE_ARRAY( data->bMouseUp );
        
        free( data );                        
}

/*---------------------------------------------------------------------------*/

const char* PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_Name( void* plugdata ) PLUGIN_CALLSPEC_SUFFIX
{
        return DRIVER_NAME;
}

/*---------------------------------------------------------------------------*/

const char* PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_Copyright( void* plugdata ) PLUGIN_CALLSPEC_SUFFIX
{
        return DRIVER_COPYRIGHT;
}

/*---------------------------------------------------------------------------*/

const TVersion* PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_Version( void* plugdata ) PLUGIN_CALLSPEC_SUFFIX
{
        return &version;
}

/*---------------------------------------------------------------------------*/

UInt32 PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_Update( void* plugdata    , 
                        void* contextdata ) PLUGIN_CALLSPEC_SUFFIX
{
}

/*---------------------------------------------------------------------------*/

UInt32 PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_CreateContext( void* plugdata     , 
                               void** contextdata ) PLUGIN_CALLSPEC_SUFFIX
{
}

/*---------------------------------------------------------------------------*/

UInt32 PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_DestroyContext( void* plugdata    , 
                                void* contextdata ) PLUGIN_CALLSPEC_SUFFIX
{
}

/*---------------------------------------------------------------------------*/

UInt32 PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_GetMousePos( void* plugdata    , 
                             void* contextdata , 
                             Float32* xpos     , 
                             Float32* ypos     ) PLUGIN_CALLSPEC_SUFFIX
{
}

/*---------------------------------------------------------------------------*/

UByte* PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_GetKeyBoardKeyStates( void* plugdata    , 
                                      void* contextdata ) PLUGIN_CALLSPEC_SUFFIX
{
        return &( (TDI8Data*) plugdata )->buffer;
}

/*---------------------------------------------------------------------------*/

UInt32 PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_GetMouseButtonPressedState( void* plugdata           , 
                                            void* contextdata        , 
                                            const UInt32 buttonindex ) PLUGIN_CALLSPEC_SUFFIX
{
}

/*---------------------------------------------------------------------------*/

UInt32 PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_GetKeyboardKeyPressedState( void* plugdata        , 
                                            void* contextdata     , 
                                            const UInt32 keyindex ) PLUGIN_CALLSPEC_SUFFIX
{
}

/*---------------------------------------------------------------------------*/

UInt32 PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_GetDeviceBoolState( void* plugdata          , 
                                    void* contextdata       , 
                                    const UInt32 deviceid   , 
                                    const UInt32 stateindex ) PLUGIN_CALLSPEC_SUFFIX
{
}

/*---------------------------------------------------------------------------*/

UInt32 PLUGIN_CALLSPEC_PREFIX
INPUTDRIVERPLUG_GetDeviceVarState( void* plugdata          , 
                                   void* contextdata       , 
                                   const UInt32 deviceid   , 
                                   const UInt32 stateindex ) PLUGIN_CALLSPEC_SUFFIX
{
}

/*---------------------------------------------------------------------------*/
