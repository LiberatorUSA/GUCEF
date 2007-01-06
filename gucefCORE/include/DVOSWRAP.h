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

#ifndef GUCEF_CORE_DVOSWRAP_H
#define GUCEF_CORE_DVOSWRAP_H

/*
 *      This file contains a set of O/S wrapping functions.
 *      The main idea is that you call these functions instead of the native
 *      O/S functions so that your code can become/stay portable.
 *      macro's are used to direct the function calls to the correct native
 *      O/S functions.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_DVFILEUTILS_H
#include "dvfileutils.h"        /* file & dir I/O */
#define GUCEF_CORE_DVFILEUTILS_H
#endif /* GUCEF_CORE_DVFILEUTILS_H ? */

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
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#define OSWRAP_TRUE     1UL
#define OSWRAP_FALSE    0UL

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Prevent C++ Name mangeling
 */
#ifdef __cplusplus
   extern "C" {
#endif   /* __cplusplus */

/*--------------------------------------------------------------------------*/

GUCEFCORE_EXPORT_C void*
LoadModuleDynamicly( const char* filename );

/*--------------------------------------------------------------------------*/

GUCEFCORE_EXPORT_C void
UnloadModuleDynamicly( void *sohandle );

/*--------------------------------------------------------------------------*/

/**
 *      Attempts to load the function from the module.
 *
 *      For MS Windows platform:
 *      If the function "functionname" isn't found then an attempt will
 *      be made to locate it using function decorations.
 *      Although C DLL's can be exchanged between different compilers in theory
 *      in practice they use different naming conventions. Unlike C++ the C
 *      exports are compatible if the same calling convention is used but the
 *      names tend to get mucked up. 
 *      Tnx for the mess M$ _|_
 */
GUCEFCORE_EXPORT_C void*
GetFunctionAddress( void *sohandle           ,
                    const char* functionname ,
                    UInt32 parambytes        );

/*--------------------------------------------------------------------------*/

/**
 *      Function that attempts to store the given string in the O/S clipboard
 *      If this functionality is not implemented for you O/S or if it fails then
 *      OSWRAP_FALSE will be returned. Otherwise OSWRAP_TRUE will be returned.
 *
 *      For MS windows:
 *              The handle to the primairy window is needed for propper operation
 *              various attempts will be made to obtain it including getting the
 *              "HWND" environment variable. If these attempts fail an attempts will
 *              be made to use the previous clipboard owner HWND. You should set the
 *              "HWND" environment setting after creating your primary window using
 *              the GUCEFSetEnv() function.
 */
GUCEFCORE_EXPORT_C UInt32
StringToClipboard( const char *str );

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
 *
 *      wbytes must be initialized to 0 !!!
 *
 *      For MS windows:
 *              The handle to the primairy window is needed for propper operation
 *              various attempts will be made to obtain it including getting the
 *              "HWND" environment variable. If these attempts fail an attempts will
 *              be made to use the previous clipboard owner HWND. You should set the
 *              "HWND" environment setting after creating your primary window using
 *              the GUCEFSetEnv() function.
 */
GUCEFCORE_EXPORT_C UInt32
StringFromClipboard( char *dest     ,
                     UInt32 size    ,
                     UInt32 *wbytes );
                     
/*--------------------------------------------------------------------------*/

GUCEFCORE_EXPORT_C UInt32
GUCEFSetEnv( const char* key   ,
             const char* value );
        
/*--------------------------------------------------------------------------*/        

GUCEFCORE_EXPORT_C const char*
GUCEFGetEnv( const char* key );

/*--------------------------------------------------------------------------*/

/**
 *      Returns the application tickcount
 */
GUCEFCORE_EXPORT_C UInt32
GUCEFGetTickCount( void );   

/*--------------------------------------------------------------------------*/

/**
 *      Displays an O/S error message/dialog
 */
GUCEFCORE_EXPORT_C void
ShowErrorMessage( const char* message     ,
                  const char* description );                  

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
   }
#endif /* __cplusplus */

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

#endif /* GUCEF_CORE_DVOSWRAP_H ? */

/*--------------------------------------------------------------------------//
//                                                                          //
//      Info & Changes                                                      //
//                                                                          //
//--------------------------------------------------------------------------//

- 29-05-2005 :
        - Debugged StringToClipboard(): it is now operational.
        - Debugged StringFromClipboard(): it is now operational.
- 02-02-2004 :
        - Created this set of functions.

----------------------------------------------------------------------------*/

