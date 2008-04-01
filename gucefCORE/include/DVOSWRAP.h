/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
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
 *              The handle to the primary window is needed for propper operation
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

GUCEF_CORE_EXPORT_C UInt32
GetPhysicalCPUCount( void );

/*--------------------------------------------------------------------------*/

GUCEF_CORE_EXPORT_C UInt32
GetLogicalCPUCount( void );

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

