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

#ifndef GUCEF_CORE_DYNMEMORYMANAGERLOADER_H
#define GUCEF_CORE_DYNMEMORYMANAGERLOADER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "gucef_dynnewoff.h"      /* Make sure that the new/delete are not declared to avoid circular definitions. */

#include <windows.h>
#include <malloc.h>

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Posible allocation/deallocation types.
 *
 *      Declared as characters to minimize memory footprint,
 *      char = 1 byte
 *      enum types = int = 32 bits = 8 bytes on standard machines
 */
#undef MM_UNKNOWN
#undef MM_NEW
#undef MM_NEW_ARRAY
#undef MM_MALLOC
#undef MM_CALLOC
#undef MM_REALLOC
#undef MM_DELETE
#undef MM_DELETE_ARRAY
#undef MM_FREE
#undef MM_OLE_ALLOC
#undef MM_OLE_FREE
#define MM_UNKNOWN        0
#define MM_NEW            1
#define MM_NEW_ARRAY      2
#define MM_MALLOC         3
#define MM_CALLOC         4
#define MM_REALLOC        5
#define MM_DELETE         6
#define MM_DELETE_ARRAY   7
#define MM_FREE           8
#define MM_OLE_ALLOC      9
#define MM_OLE_FREE       10

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      User API, these functions can be used to set parameters within the Memory
 *      Manager to control the type and extent of the memory tests that are performed.
 *      Note that it is not necessary to call any of these methods, you will get the default
 *      Memory Manager automatically.
 */

typedef unsigned __int32 ( *TFP_MEMMAN_Initialize )( void );
typedef void ( *TFP_MEMMAN_Shutdown )( void );
typedef void ( *TFP_MEMMAN_DumpLogReport )( void );
typedef void ( *TFP_MEMMAN_DumpMemoryAllocations )( void );
typedef void ( *TFP_MEMMAN_SetLogFile )( const char *file );
typedef void ( *TFP_MEMMAN_SetExhaustiveTesting )( unsigned __int32 test );
typedef void ( *TFP_MEMMAN_SetLogAlways )( unsigned __int32 log );
typedef void ( *TFP_MEMMAN_SetPaddingSize )( unsigned __int32 clean );
typedef void ( *TFP_MEMMAN_CleanLogFile )( unsigned __int32 test );
typedef void ( *TFP_MEMMAN_BreakOnAllocation )( int alloccount );
typedef void ( *TFP_MEMMAN_BreakOnDeallocation )( void *address );
typedef void ( *TFP_MEMMAN_BreakOnReallocation )( void *address );
typedef void ( *TFP_MEMMAN_Validate )( const void* address, unsigned __int32 blocksize, const char *file, int line );
typedef void ( *TFP_MEMMAN_ValidateChunk )( const void* address, const void* chunk, unsigned __int32 blocksize, const char *file, int line );

/*-------------------------------------------------------------------------*/

/*
 *  Memory tracking functions which are invoked by the memory allocation overrides
 */

typedef void* ( *TFP_MEMMAN_AllocateMemory )( const char *file, int line, size_t size, char type, void *address );
typedef void ( *TFP_MEMMAN_DeAllocateMemory )( void *address, char type );
typedef void ( *TFP_MEMMAN_DeAllocateMemoryEx )( const char *file, int line, void *address, char type );
typedef void ( *TFP_MEMMAN_SetOwner )( const char *file, int line );


/*-------------------------------------------------------------------------*/

#ifdef MEMCHECK_OLEAPI

/*
 *  OLE Memory tracking functions which are invoked by the OLE memory allocation overrides
 */

typedef wchar_t* ( *TFP_MEMMAN_SysAllocString )( const char *file, int line, wchar_t* wcharStr );
typedef wchar_t* ( *TFP_MEMMAN_SysAllocStringByteLen )( const char *file, int line, const char* str, unsigned int bufferSize );
typedef wchar_t* ( *TFP_MEMMAN_SysAllocStringLen )( const char *file, int line, const wchar_t* str, unsigned int charsToCopy );
typedef void ( *TFP_MEMMAN_SysFreeString )( const char *file, int line, wchar_t* bstrString );
typedef int ( *TFP_MEMMAN_SysReAllocString )( const char *file, int line, wchar_t** pbstr, const wchar_t* psz );
typedef int ( *TFP_MEMMAN_SysReAllocStringLen )( const char *file, int line, wchar_t** pbstr, const wchar_t* psz, unsigned int len );

#endif /* MEMCHECK_OLEAPI ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

static TFP_MEMMAN_Initialize fp_MEMMAN_Initialize = 0;
static TFP_MEMMAN_Shutdown fp_MEMMAN_Shutdown = 0;
static TFP_MEMMAN_DumpLogReport fp_MEMMAN_DumpLogReport = 0;
static TFP_MEMMAN_DumpMemoryAllocations fp_MEMMAN_DumpMemoryAllocations = 0;
static TFP_MEMMAN_SetLogFile fp_MEMMAN_SetLogFile = 0;
static TFP_MEMMAN_SetExhaustiveTesting fp_MEMMAN_SetExhaustiveTesting = 0;
static TFP_MEMMAN_SetLogAlways fp_MEMMAN_SetLogAlways = 0;
static TFP_MEMMAN_SetPaddingSize fp_MEMMAN_SetPaddingSize = 0;
static TFP_MEMMAN_CleanLogFile fp_MEMMAN_CleanLogFile = 0;
static TFP_MEMMAN_BreakOnAllocation fp_MEMMAN_BreakOnAllocation = 0;
static TFP_MEMMAN_BreakOnDeallocation fp_MEMMAN_BreakOnDeallocation = 0;
static TFP_MEMMAN_BreakOnReallocation fp_MEMMAN_BreakOnReallocation = 0;
static TFP_MEMMAN_Validate fp_MEMMAN_Validate = 0;
static TFP_MEMMAN_ValidateChunk fp_MEMMAN_ValidateChunk = 0;

/*-------------------------------------------------------------------------*/

static TFP_MEMMAN_AllocateMemory fp_MEMMAN_AllocateMemory = 0;
static TFP_MEMMAN_DeAllocateMemory fp_MEMMAN_DeAllocateMemory = 0;
static TFP_MEMMAN_DeAllocateMemoryEx fp_MEMMAN_DeAllocateMemoryEx = 0;
static TFP_MEMMAN_SetOwner fp_MEMMAN_SetOwner = 0;


/*-------------------------------------------------------------------------*/

#ifdef MEMCHECK_OLEAPI

static TFP_MEMMAN_SysAllocString fp_MEMMAN_SysAllocString = 0;
static TFP_MEMMAN_SysAllocStringByteLen fp_MEMMAN_SysAllocStringByteLen = 0;
static TFP_MEMMAN_SysAllocStringLen fp_MEMMAN_SysAllocStringLen = 0;
static TFP_MEMMAN_SysFreeString fp_MEMMAN_SysFreeString = 0;
static TFP_MEMMAN_SysReAllocString fp_MEMMAN_SysReAllocString = 0;
static TFP_MEMMAN_SysReAllocStringLen fp_MEMMAN_SysReAllocStringLen = 0;

#endif /* MEMCHECK_OLEAPI ? */

/*-------------------------------------------------------------------------*/

static void* g_memoryManagerModulePtr = NULL; 
static void* g_dynLoadMutex = NULL;
static const char* MemoryLeakFinderLib = "MemoryLeakFinder_d.dll";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

static int 
LazyLoadMemoryManager( void )
{
    /* check to make sure we havent already loaded the library */
    if ( 0 != g_memoryManagerModulePtr ) 
        return 1;
    if ( 0 == g_dynLoadMutex )
    {
        g_dynLoadMutex = CreateMutex( NULL, FALSE, NULL );
        if ( 0 == g_dynLoadMutex )
            return 0;
    }
    if ( WAIT_FAILED == WaitForSingleObject( (HANDLE) g_dynLoadMutex, 30000 ) ) 
        return 0;
    if ( 0 != g_memoryManagerModulePtr ) 
        return 1;

    /* check to see if the module is already loaded elsewhere in the process */
    g_memoryManagerModulePtr = (void*) GetModuleHandleA( MemoryLeakFinderLib );
    if ( 0 == g_memoryManagerModulePtr )
    {
        /* load the library */
        g_memoryManagerModulePtr = (void*) LoadLibrary( MemoryLeakFinderLib );
        if ( 0 == g_memoryManagerModulePtr )
        {
            ReleaseMutex( (HANDLE) g_dynLoadMutex );
            return 0;
        }
    }

    /* load the function pointers */
    fp_MEMMAN_Initialize = (TFP_MEMMAN_Initialize) GetProcAddress( (HMODULE) g_memoryManagerModulePtr, "MEMMAN_Initialize" );
    fp_MEMMAN_Shutdown = (TFP_MEMMAN_Shutdown) GetProcAddress( (HMODULE) g_memoryManagerModulePtr, "MEMMAN_Shutdown" );
    fp_MEMMAN_DumpLogReport = (TFP_MEMMAN_DumpLogReport) GetProcAddress( (HMODULE) g_memoryManagerModulePtr, "MEMMAN_DumpLogReport" );
    fp_MEMMAN_DumpMemoryAllocations = (TFP_MEMMAN_DumpMemoryAllocations) GetProcAddress( (HMODULE) g_memoryManagerModulePtr, "MEMMAN_DumpMemoryAllocations" );
    fp_MEMMAN_SetLogFile = (TFP_MEMMAN_SetLogFile) GetProcAddress( (HMODULE) g_memoryManagerModulePtr, "MEMMAN_SetLogFile" );
    fp_MEMMAN_SetExhaustiveTesting = (TFP_MEMMAN_SetExhaustiveTesting) GetProcAddress( (HMODULE) g_memoryManagerModulePtr, "MEMMAN_SetExhaustiveTesting" );
    fp_MEMMAN_SetLogAlways = (TFP_MEMMAN_SetLogAlways) GetProcAddress( (HMODULE) g_memoryManagerModulePtr, "MEMMAN_SetLogAlways" );
    fp_MEMMAN_SetPaddingSize = (TFP_MEMMAN_SetPaddingSize) GetProcAddress( (HMODULE) g_memoryManagerModulePtr, "MEMMAN_SetPaddingSize" );
    fp_MEMMAN_CleanLogFile = (TFP_MEMMAN_CleanLogFile) GetProcAddress( (HMODULE) g_memoryManagerModulePtr, "MEMMAN_CleanLogFile" );
    fp_MEMMAN_BreakOnAllocation = (TFP_MEMMAN_BreakOnAllocation) GetProcAddress( (HMODULE) g_memoryManagerModulePtr, "MEMMAN_BreakOnAllocation" );
    fp_MEMMAN_BreakOnDeallocation = (TFP_MEMMAN_BreakOnDeallocation) GetProcAddress( (HMODULE) g_memoryManagerModulePtr, "MEMMAN_BreakOnDeallocation" );
    fp_MEMMAN_BreakOnReallocation = (TFP_MEMMAN_BreakOnReallocation) GetProcAddress( (HMODULE) g_memoryManagerModulePtr, "MEMMAN_BreakOnReallocation" );
    fp_MEMMAN_Validate = (TFP_MEMMAN_Validate) GetProcAddress( (HMODULE) g_memoryManagerModulePtr, "MEMMAN_Validate" );
    fp_MEMMAN_ValidateChunk = (TFP_MEMMAN_ValidateChunk) GetProcAddress( (HMODULE) g_memoryManagerModulePtr, "MEMMAN_ValidateChunk" );
    fp_MEMMAN_AllocateMemory = (TFP_MEMMAN_AllocateMemory) GetProcAddress( (HMODULE) g_memoryManagerModulePtr, "MEMMAN_AllocateMemory" );
    fp_MEMMAN_DeAllocateMemory = (TFP_MEMMAN_DeAllocateMemory) GetProcAddress( (HMODULE) g_memoryManagerModulePtr, "MEMMAN_DeAllocateMemory" );
    fp_MEMMAN_DeAllocateMemoryEx = (TFP_MEMMAN_DeAllocateMemoryEx) GetProcAddress( (HMODULE) g_memoryManagerModulePtr, "MEMMAN_DeAllocateMemoryEx" );
    fp_MEMMAN_SetOwner = (TFP_MEMMAN_SetOwner) GetProcAddress( (HMODULE) g_memoryManagerModulePtr, "MEMMAN_SetOwner" );

    if ( 0 == fp_MEMMAN_Initialize ||
         0 == fp_MEMMAN_Shutdown ||
         0 == fp_MEMMAN_DumpLogReport ||
         0 == fp_MEMMAN_DumpMemoryAllocations ||
         0 == fp_MEMMAN_SetLogFile ||
         0 == fp_MEMMAN_SetExhaustiveTesting ||
         0 == fp_MEMMAN_SetLogAlways ||
         0 == fp_MEMMAN_SetPaddingSize || 
         0 == fp_MEMMAN_CleanLogFile ||
         0 == fp_MEMMAN_BreakOnAllocation ||
         0 == fp_MEMMAN_BreakOnDeallocation ||
         0 == fp_MEMMAN_BreakOnReallocation ||
         0 == fp_MEMMAN_Validate ||
         0 == fp_MEMMAN_ValidateChunk ||
         0 == fp_MEMMAN_AllocateMemory ||
         0 == fp_MEMMAN_DeAllocateMemory ||
         0 == fp_MEMMAN_SetOwner ) 
    {
        FreeLibrary( (HMODULE) g_memoryManagerModulePtr );
        g_memoryManagerModulePtr = 0;
        return 0;
    }

    #ifdef MEMCHECK_OLEAPI

    fp_MEMMAN_SysAllocString = (TFP_MEMMAN_SysAllocString) GetProcAddress( (HMODULE) g_memoryManagerModulePtr, "MEMMAN_SysAllocString" );
    fp_MEMMAN_SysAllocStringByteLen = (TFP_MEMMAN_SysAllocStringByteLen) GetProcAddress( (HMODULE) g_memoryManagerModulePtr, "MEMMAN_SysAllocStringByteLen" );
    fp_MEMMAN_SysAllocStringLen = (TFP_MEMMAN_SysAllocStringLen) GetProcAddress( (HMODULE) g_memoryManagerModulePtr, "MEMMAN_SysAllocStringLen" );
    fp_MEMMAN_SysFreeString = (TFP_MEMMAN_SysFreeString) GetProcAddress( (HMODULE) g_memoryManagerModulePtr, "MEMMAN_SysFreeString" );
    fp_MEMMAN_SysReAllocString = (TFP_MEMMAN_SysReAllocString) GetProcAddress( (HMODULE) g_memoryManagerModulePtr, "MEMMAN_SysReAllocString" );
    fp_MEMMAN_SysReAllocStringLen = (TFP_MEMMAN_SysReAllocStringLen) GetProcAddress( (HMODULE) g_memoryManagerModulePtr, "MEMMAN_SysReAllocStringLen" );

    if ( 0 == fp_MEMMAN_SysAllocString ||
         0 == fp_MEMMAN_SysAllocStringByteLen ||
         0 == fp_MEMMAN_SysAllocStringLen ||
         0 == fp_MEMMAN_SysFreeString ||
         0 == fp_MEMMAN_SysReAllocString ||
         0 == fp_MEMMAN_SysReAllocStringLen )
    {
        FreeLibrary( (HMODULE) g_memoryManagerModulePtr );
        g_memoryManagerModulePtr = 0;
        return 0;
    }

    #endif /* MEMCHECK_OLEAPI ? */

    if ( fp_MEMMAN_Initialize() )
    {
        ReleaseMutex( (HANDLE) g_dynLoadMutex );
        g_dynLoadMutex = 0;

        return 1;
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_DYNMEMORYMANAGERLOADER_H ? */