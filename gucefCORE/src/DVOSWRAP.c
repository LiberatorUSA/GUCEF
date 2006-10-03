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

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"       /* build configuration */
#define GUCEFCORE_MACROS_H
#endif /* GUCEFCORE_MACROS_H ? */

#ifdef GUCEF_MSWIN_BUILD
/* Do not use WIN32_LEAN_AND_MEAN because it will remove timeBeginPeriod() etc. */
#undef  WIN32_LEAN_AND_MEAN
#include <windows.h>                /* Windows API */
#define MAX_DIR_LENGTH MAX_PATH
#endif /* GUCEF_MSWIN_BUILD ? */

#ifdef LINUX_BUILD
  #include <limits.h>                 /* Linux OS limits */
  #include <dlfcn.h>                  /* dynamic linking utilities */
  #define MAX_DIR_LENGTH PATH_MAX
#endif /* LINUX_BUILD ? */

#ifndef GUCEF_CORE_DVSTRUTILS_H
#include "dvstrutils.h"               /* needed for str to int */
#define GUCEF_CORE_DVSTRUTILS_H
#endif /* GUCEF_CORE_DVSTRUTILS_H ? */

#include "dvoswrap.h"               /* O/S Wrapping function prototypes*/

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
namespace GUCEF {
namespace CORE {
#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      needed for GUCEFPrecisionDelay()
 *      Initialized in GUCEFPrecisionTimerInit() 
 */
#ifdef GUCEF_MSWIN_BUILD
static LARGE_INTEGER m_high_perf_timer_freq = { 0 };
#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void*
LoadModuleDynamicly( const char* filename )
{
        #ifdef LINUX_BUILD
        return (void*) dlopen( filename, RTLD_NOW );
        #else
          #ifdef GUCEF_MSWIN_BUILD
          return (void*) LoadLibrary( filename );
          #else
          #error Unsupported target platform
          #endif
        #endif
}

/*--------------------------------------------------------------------------*/

void
UnloadModuleDynamicly( void *sohandle )
{
        if ( !sohandle ) return;
        #ifdef LINUX_BUILD
        dlclose( sohandle );
        #elif defined( GUCEF_MSWIN_BUILD )
        FreeLibrary( (HMODULE)sohandle );
        #else
        #error Unsupported target platform
        #endif
}

/*--------------------------------------------------------------------------*/

void*
GetFunctionAddress( void *sohandle           ,
                    const char* functionname ,
                    UInt32 parambytes        )
{
        /*
         *      Calling Convention      Internal*       MSVC DLL (w/ DEF)       MSVC DLL (dllexport)  	DMC DLL         MinGW DLL       BCC DLL
         *      __stdcall               _Function@n  	Function                _Function@n             _Function@n     Function@n      Function
         *      __cdecl                 _Function       Function                Function                Function        Function        _Function
         */
        void* fptr;
        if ( !sohandle ) return NULL;
        #if defined( LINUX_BUILD )
        return (void*) dlsym( sohandle     ,
                              functionname );
        #elif defined( GUCEF_MSWIN_BUILD )
        
        #pragma warning( push )
        #pragma warning( disable : 4054 ) /* warning C4054: 'type cast' : from function pointer 'FARPROC' to data pointer 'void *' */
        
        /*
         *      First we try a normal load using the given
         *      functionname.
         *
         *      Type: Function         
         */
        fptr = (void*) GetProcAddress( (HMODULE)sohandle ,
                                       functionname      );
        if ( fptr == NULL )
        {
                char buffer[ 1024 ];
                UInt32 len = (UInt32)strlen( functionname );
                
                /*
                 *      Maybe without the param bytes then :(
                 *
                 *      Type: _Function
                 */                
                strncpy( buffer+1, functionname, len+1 );
                *buffer = '_';                
                fptr = (void*) GetProcAddress( (HMODULE)sohandle ,
                                               buffer            );                 
                                        
                /*
                 *      Try adding the param bytes value
                 *      ... So much for naming conventions :/
                 *
                 *      Type: _Function@n
                 */
                if ( fptr == NULL )
                { 
                        sprintf( buffer+len+1, "@%d", parambytes );
                        fptr = (void*) GetProcAddress( (HMODULE)sohandle ,
                                                       buffer            );
                 
                        /*
                         *      Last but not least try..
                         *
                         *      Type: Function@n                 
                         */
                        if ( fptr == NULL )
                        {
                                fptr = (void*) GetProcAddress( (HMODULE)sohandle ,
                                                                buffer+1         );
                        } 
                }                                                                             
                                       
        }
        return fptr;
        #pragma warning( pop )
                                       
        #else
        #error Unsupported target platform
        #endif
}

/*--------------------------------------------------------------------------*/

#ifdef GUCEF_MSWIN_BUILD

static HWND
GetCurrentHWND( void )
{
        const char* hwndstr;
        HWND whandle = NULL;
        
        #ifdef SDL_SUPPORT

        /*
         *      If SDL is supported we will try to get the HWND from SDL
         */                
        SDL_SysWMinfo winfo;                
        SDL_GetWMInfo( &winfo );
        whandle = winfo.window;                                                               
        
        if ( !whandle ) {
        
        #endif

        #pragma warning( disable: 4047 ) // 'HWND' differs in levels of indirection from 'Int32'
                
        /*
         *      Try to get the HWND from the proccess enviorment settings
         */
        hwndstr = GUCEFGetEnv( "HWND" );
        if ( hwndstr )
        {
                whandle = Str_To_Int( hwndstr ); 
        }
        else
        {                         
                /*
                 *      If all previous attemps failed then try to use the previous
                 *      clipboard owner (if there is one).
                 */
                whandle = GetClipboardOwner();
        }
        
        #ifdef SDL_SUPPORT                
        }
        #endif
        
        return whandle;                
}

#endif /* GUCEF_MSWIN_BUILD ? */ 

/*--------------------------------------------------------------------------*/

UInt32
StringToClipboard( const char *str )
{
        if ( str )
        {
                UInt32 strlength = (UInt32) strlen( str );
                
                #ifdef GUCEF_MSWIN_BUILD
                                                                                        
                HWND whandle = GetCurrentHWND();
                UInt32 success = OSWRAP_FALSE;
                HGLOBAL hglbcopy;
                LPTSTR lptstrcopy;

                /*
                 *      Open the clipboard with the current task as the owner
                 */
                if ( !OpenClipboard( whandle ) ) return OSWRAP_FALSE;

                /*
                 *      Try to empty the clipboard so that we can get ownership of the
                 *      clipboard which is needed for placing data on it.
                 */
                EmptyClipboard();
                
                /*
                 *      Allocate global MS windows managed memory for the text
                 */
                hglbcopy = GlobalAlloc( GMEM_MOVEABLE                 , 
                                        (strlength+1) * sizeof(TCHAR) );
                if ( hglbcopy == NULL )
                {
                        CloseClipboard();
                        return OSWRAP_FALSE;
                }
                
                /*
                 *      Now we have to lock the memory we just allocated so that
                 *      windows keeps it's paws off of it. After that we can copy
                 *      our text into the global memory buffer
                 */
                lptstrcopy = GlobalLock( hglbcopy );
                memcpy( lptstrcopy, str, strlength+1 );
                GlobalUnlock( hglbcopy );
                                        
                /*
                 *      We now attempt to set the string in the clipboard.
                 *      This will fail if the handle we used in OpenClipboard() is
                 *      NULL or incorrect.
                 */
                if ( SetClipboardData( CF_TEXT, hglbcopy ) )
                {
                        success = OSWRAP_TRUE;
                }
                else
                {
                        success = OSWRAP_FALSE;
                }
                
                /*
                 *      Close the clipboard so that other tasks have access again
                 */
                CloseClipboard();

                return success;                
        }
        return OSWRAP_FALSE;
        
        #else /* GUCEF_MSWIN_BUILD */

        return OSWRAP_FALSE;
        
        #endif /* OS WRAPPING */                                   
}

/*--------------------------------------------------------------------------*/

/**
 *      Function that attempts to retrieve a string from the O/S clipboard
 *      If this functionality is not implemented for you O/S or if it fails then
 *      OSWRAP_FALSE will be returned. Otherwise OSWRAP_TRUE will be returned.
 *      It is assumed that dest has points to size number of allocated bytes.
 *      The actual number of bytes written into the destination buffer is
 *      written into wbytes. If you wish to retrieve all text on the clipboard
 *      you might want to continue calling this function as long as wbytes
 *      equals size. Do note that any other proccess can empty the clipboard
 *      in between calls.
 */
UInt32
StringFromClipboard( char *dest     ,
                     UInt32 size    ,
                     UInt32 *wbytes )
{
        #ifdef GUCEF_MSWIN_BUILD
        
        HWND whandle = GetCurrentHWND();
        HGLOBAL hglb;
        UInt32 success = OSWRAP_FALSE;
        
        #ifdef SDL_SUPPORT
        
        SDL_SysWMinfo winfo;
        
        #endif /* SDL_SUPPORT */
        
        if ( IsClipboardFormatAvailable( CF_TEXT ) )
        {
                /*
                 *      Open the clipboard with the current task as the owner
                 */
                if ( !OpenClipboard( whandle ) ) return OSWRAP_FALSE;

                /*
                 *      Get a pointer to the data using the global handle we will
                 *      obtain if possible. Then copy the data pointed to by the handle
                 *      into the buffer provided by the user
                 */
                hglb = GetClipboardData( CF_TEXT );
                if ( hglb != NULL )
                {
                        LPTSTR lptstr = GlobalLock( hglb );
                        if ( lptstr != NULL )
                        {                           
                                UInt32 offset = *wbytes;
                                UInt32 dsize = (UInt32) strlen( lptstr );
                                if ( dsize > offset )
                                {                               
                                        dsize -= offset;
                                        if ( dsize > size-1 ) dsize = size-1;
                                        strncpy( dest, lptstr+offset, dsize );
                                        *wbytes += dsize;
                                        
                                        success = OSWRAP_TRUE;
                                }
                                GlobalUnlock( hglb );                               
                        }
                        else 
                        {
                                *wbytes = 0;
                        }        
                }                                       

                /*
                 *      Close the clipboard so that other tasks have access again
                 */                
                CloseClipboard();

                return success;
        }
        return OSWRAP_FALSE;
        
        #else /* GUCEF_MSWIN_BUILD */
        
        return OSWRAP_FALSE;
        
        #endif /* OS WRAPPING */                
}

/*--------------------------------------------------------------------------*/

UInt32
GUCEFSetEnv( const char* key   ,
             const char* value )
{
        #ifdef GUCEF_MSWIN_BUILD
        
        UInt32 retval;        
        char* envstr = malloc( strlen( key ) + strlen( value )+2 );
        sprintf( envstr, "%s=%s", key, value );
        retval = _putenv( envstr );
        free( envstr );
        return retval == 0;
        
        #else
        
        return setenv( key, value ) == 0;
        
        #endif        
}

/*--------------------------------------------------------------------------*/

const char*
GUCEFGetEnv( const char* key )
{
        return getenv( key );        
}

/*--------------------------------------------------------------------------*/        

/**
 *      Returns the application tickcount
 */
UInt32
GUCEFGetTickCount( void )
{          
        return GetTickCount();        
}  

/*--------------------------------------------------------------------------*/

UInt32
GetCurrentTaskID( void )
{
        #ifdef GUCEF_MSWIN_BUILD
        return GetCurrentThreadId();
        #else
        #error unsupported target platform
        #endif
}       

/*--------------------------------------------------------------------------*/

void
ShowErrorMessage( const char* message     ,
                  const char* description )
{
        #ifdef GUCEF_MSWIN_BUILD
        MessageBox( GetCurrentHWND()                    ,
                    description                         ,
                    message                             ,
                    MB_OK | MB_ICONERROR | MB_TASKMODAL );
        #else
        fprintf( stderr, "%s : %s\n", message, description );
        #endif                    
}                  

/*--------------------------------------------------------------------------*/

/**
 *      Verry accurate delay mechanism.
 *      Uses the target O/S facilitys to attempt to delay
 *      for the given number of milliseconds as accuratly as possible.
 *      This function requires you to call GUCEFPrecisionTimerInit() at
 *      application startup and GUCEFPrecisionTimerShutdown() at application
 *      shutdown.
 *
 *      @param milliseconds the number of milliseconds you wish to delay the caller thread 
 */
void
GUCEFPrecisionDelay( UInt32 delay )
{
        #ifdef GUCEF_MSWIN_BUILD
        
        /*
         *      Original code obtained from http://www.geisswerks.com/ryan/FAQS/timing.html
         */
        
        /*
         *      note: Possible problem in some cases:
         *              http://support.microsoft.com/default.aspx?scid=KB;EN-US;Q274323&
         *              Performance counter value may unexpectedly leap forward
         */
        
        /*
         *      note: BE SURE YOU CALL timeBeginPeriod(1) at program startup!!!
         *      note: BE SURE YOU CALL timeEndPeriod(1) at program exit!!!
         *      note: that this code will require linking to winmm.lib !!!        
         */         
         
        Int32 ticks_passed; 
        Int32 ticks_left;
        Int32 i; 
         
        static LARGE_INTEGER m_prev_end_of_frame = { 0 };  

        LARGE_INTEGER t;
        QueryPerformanceCounter(&t);

        if (m_prev_end_of_frame.QuadPart != 0)
        {
                Int32 ticks_to_wait = (Int32)m_high_perf_timer_freq.QuadPart / delay;
                Int32 done = 0;
                do
                {
                        QueryPerformanceCounter(&t);

                        ticks_passed = (Int32)( (__int64)t.QuadPart - (__int64)m_prev_end_of_frame.QuadPart );
                        ticks_left = ticks_to_wait - ticks_passed;

                        if ( t.QuadPart < m_prev_end_of_frame.QuadPart )    /* time wrap */
                        {
                                done = 1;
                        }        
                        if (ticks_passed >= ticks_to_wait)
                        {
                                done = 1;
                        }                                

                        if ( !done )
                        {
                                /*   
                                 *      if > 0.002s left, do Sleep(1), which will actually sleep some 
                                 *      steady amount, probably 1-2 ms,
                                 *      and do so in a nice way (cpu meter drops; laptop battery spared).
                                 *      otherwise, do a few Sleep(0)'s, which just give up the timeslice,
                                 *      but don't really save cpu or battery, but do pass a tiny
                                 *     amount of time.
                                 */
                                if ( ticks_left > (Int32)m_high_perf_timer_freq.QuadPart*2/1000)
                                {
                                        Sleep(1);
                                }                                        
                                else                        
                                {
                                        for ( i=0; i<10; ++i )
                                        {
                                                Sleep(0);  /* causes thread to give up its timeslice */
                                        }                                                
                                }
                        }                                
                }
                while (!done);            
        }

        m_prev_end_of_frame = t;
        
        #else
        #error unsupported target platform
        #endif        
}

/*--------------------------------------------------------------------------*/

/**
 *      Does whatever initialization is required for GUCEFPrecisionDelay() to
 *      perform it's magic.
 *      MUST be called at application startup.
 *      Also see GUCEFPrecisionTimerShutdown()
 */
void
GUCEFPrecisionTimerInit( void )
{
        #ifdef GUCEF_MSWIN_BUILD
        /*
         *      Change Sleep() resolution to 1-2 milliseconds
         *
         *      Note that calling timeBeginPeriod() also affects the granularity of some
         *      other timing calls, such as CreateWaitableTimer() and WaitForSingleObject(); 
         *      however, some functions are still unaffected, such as _ftime().
         */
        timeBeginPeriod( 1 );

        /*
         *      Initialize the timer frequenty structure for use
         */        
        QueryPerformanceFrequency( &m_high_perf_timer_freq );
        #endif         
}

/*--------------------------------------------------------------------------*/

/**
 *      Cleans up whatever needs to be cleaned up after a class to 
 *      GUCEFPrecisionTimerInit().
 *      MUST be called if you called GUCEFPrecisionTimerInit() !!!
 *      call at application shutdown.
 */
void
GUCEFPrecisionTimerShutdown( void )
{
        #ifdef GUCEF_MSWIN_BUILD
        /*
         *      Undo timer resolution change
         */
        timeEndPeriod( 1 );
        #endif 
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
}; /* namespace CORE */
}; /* namespace GUCEF */
#endif /* __cplusplus ? */

/*--------------------------------------------------------------------------*/
