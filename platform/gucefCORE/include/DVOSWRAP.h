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

/*--------------------------------------------------------------------------*/

/*
 *      Prevent C++ Name mangeling
 */
#ifdef __cplusplus
   extern "C" {
#endif   /* __cplusplus */

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#define OSWRAP_TRUE     1UL
#define OSWRAP_FALSE    0UL

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

typedef void (*TDefaultFuncPtr) ();

/*--------------------------------------------------------------------------*/

/**
 *  Union meant to provide storage that can be used to store either a
 *  pointer-to-data or pointer-to-function. Conversion between the two types
 *  is not allowed according to ISO C++ and is in fact undefined.
 *  In some implementations a pointer-to-function will require more space then
 *  a pointer-to-data. O/S functions however will often return a void*, which is
 *  a pointer-to-data. The union below allows us to work around this problem.
 */
union anyPointer
{
    void* objPtr;
    TDefaultFuncPtr funcPtr;
};
typedef union anyPointer TAnyPointer;

/*-------------------------------------------------------------------------*/

struct SProcessId;
typedef struct SProcessId TProcessId;

/*-------------------------------------------------------------------------*/

/**
 *  Structure holding various bits of memory usage related information
 *  If for the current O/S a field cannot be determined it will be set to -1
 */
struct SProcessMemoryUsageInfo
{
    #ifdef GUCEF_32BIT
    typedef UInt32 TProcMemStatInt;
    #else
    typedef UInt64 TProcMemStatInt;
    #endif

    TProcMemStatInt workingSetSizeInBytes;
    TProcMemStatInt peakWorkingSetSizeInBytes;
    TProcMemStatInt pageFaultCountInBytes;
    TProcMemStatInt pageFileUsageInBytes;
    TProcMemStatInt peakPageFileUsageInBytes;
};
typedef struct SProcessMemoryUsageInfo TProcessMemoryUsageInfo;

/*-------------------------------------------------------------------------*/

/**
 *  Structure holding various bits of memory usage related information
 *  If for the current O/S a field cannot be determined it will be set to -1
 */
struct SGlobalMemoryUsageInfo
{
    typedef UInt64 TMemStatInt;

    UInt8 memoryLoadPercentage;
    TMemStatInt totalPhysicalMemoryInBytes;
    TMemStatInt availablePhysicalMemoryInBytes;
    TMemStatInt totalPageFileSizeInBytes;
    TMemStatInt availablePageFileSizeInBytes;
    TMemStatInt totalVirtualMemoryInBytes;
    TMemStatInt availableVirtualMemoryInBytes;
    TMemStatInt availExtendedVirtualMemoryInBytes;
};
typedef struct SGlobalMemoryUsageInfo TGlobalMemoryUsageInfo;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_C void*
LoadModuleDynamicly( const char* filename );

/*--------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_C void
UnloadModuleDynamicly( void *sohandle );

/*--------------------------------------------------------------------------*/

/** 
 *  Gets a pointer to an already loaded module. This function does NOT
 *  increment the reference count of the module referenced. If you do not know
 *  whether the modules is already loaded you should use LoadModuleDynamicly()
 *  instead of this function.
 */
GUCEF_CORE_PUBLIC_C void*
GetModulePointer( const char* moduleName );

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
 */
GUCEF_CORE_PUBLIC_C TAnyPointer
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
GUCEF_CORE_PUBLIC_C UInt32
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
 *      equals size. Do note that any other process can empty the clipboard
 *      in between calls.
 *
 *      wbytes must be initialized to 0 !!!
 *
 *      For MS windows:
 *              The handle to the primary window is needed for proper operation
 *              various attempts will be made to obtain it including getting the
 *              "HWND" environment variable. If these attempts fail an attempts will
 *              be made to use the previous clipboard owner HWND. You should set the
 *              "HWND" environment setting after creating your primary window using
 *              the GUCEFSetEnv() function.
 */
GUCEF_CORE_PUBLIC_C UInt32
StringFromClipboard( char *dest     ,
                     UInt32 size    ,
                     UInt32 *wbytes );

/*--------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_C UInt32
GUCEFSetEnv( const char* key   ,
             const char* value );

/*--------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_C const char*
GUCEFGetEnv( const char* key );

/*--------------------------------------------------------------------------*/

/**
 *      Returns the application tickcount in milliseconds
 */
GUCEF_CORE_PUBLIC_C UInt32
GUCEFGetTickCount( void );

/*--------------------------------------------------------------------------*/

/**
 *      Displays an O/S error message/dialog
 */
GUCEF_CORE_PUBLIC_C void
ShowErrorMessage( const char* message     ,
                  const char* description );

/*--------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_C UInt32
GetPhysicalCPUCount( void );

/*--------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_C UInt32
GetLogicalCPUCount( void );

/*--------------------------------------------------------------------------*/

/**
 *  Obtains a list of Process IDs for all the processes running on the system as visible to the
 *  account under which the software is executing
 */
GUCEF_CORE_PUBLIC_C UInt32
GetProcessList( TProcessId** processList , 
                UInt32* processCount     );

/*--------------------------------------------------------------------------*/

/**
 *  To be used with GetProcessList() to iterate the resulting processList
 */
GUCEF_CORE_PUBLIC_C TProcessId*
GetProcessIdAtIndex( TProcessId* processList ,
                     UInt32 index            );

/*--------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_C void
FreeProcessList( TProcessId* processList );

/*--------------------------------------------------------------------------*/

/**
 *  Copies a PID structure.
 *  This allocated memory that needs to be freed with FreeProcessId()
 */
GUCEF_CORE_PUBLIC_C TProcessId*
CopyProcessId( TProcessId* pid );

/*--------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_C void
FreeProcessId( TProcessId* processList );

/*--------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_C UInt32
GetProcessMemmoryUsage( TProcessId* pid                     , 
                        TProcessMemoryUsageInfo* memUseInfo );

/*--------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_C UInt32 
GetExeNameForProcessId( TProcessId* pid        , 
                        char* outNameBuffer    , 
                        UInt32 nameBufferSize  ,
                        UInt32* usedBufferSize );


/*--------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_C UInt32
GetGlobalMemoryUsage( TGlobalMemoryUsageInfo* memUseInfo );

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

