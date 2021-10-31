/*
 *  gucef common header: provides header based platform wide facilities
 *
 *  Copyright (C) 1998 - 2020.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 *      These #defines and utilities are the core of the memory manager.
 *      This overrides standard memory allocation and de-allocation routines 
 *      and replaces them with the memory manager's versions.
 *      This allows for memory tracking and statistics to be generated.
 */
 
#ifndef GUCEF_DYNNEWON_H 
#define GUCEF_DYNNEWON_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CONFIG_H
#include "gucef_config.h"            /* GUCEF configuration */
#define GUCEF_CONFIG_H
#endif /* GUCEF_CONFIG_H ? */

#if defined( GUCEF_USE_MEMORY_LEAK_CHECKER ) && defined( GUCEF_USE_PLATFORM_MEMORY_LEAK_CHECKER ) && !defined( GUCEF_DYNNEWON_DISABLED )

#ifndef GUCEF_PLATFORM_H
#include "gucef_platform.h"          /* GUCEF platform compilation targets */
#define GUCEF_PLATFORM_H
#endif /* GUCEF_PLATFORM_H ? */

#ifndef GUCEF_BASICHELPERS_H
#include "gucef_basichelpers.h"  /* GUCEF platform convenience basic helper macros */
#define GUCEF_BASICHELPERS_H
#endif /* GUCEF_BASICHELPERS_H ? */

#include <stdlib.h>               /* Required for malloc() and free() */
#include <assert.h>

#ifdef __cplusplus
  #include <new>
#endif /* __cplusplus ? */

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
  #define _CRTDBG_MAP_ALLOC
  #include <crtdbg.h>             /* only included here to prevent later inclusion from clobbering our macros */
#endif

#ifndef GUCEF_MEMORYMANAGERLOADER_H
#include "gucef_MemoryManagerLoader.h"       /* header that includes the header-only functionality to load the external memory manager */
#define GUCEF_MEMORYMANAGERLOADER_H
#endif /* GUCEF_MEMORYMANAGERLOADER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus

#if ( GUCEF_COMPILER == GUCEF_COMPILER_MSVC )
  #pragma warning( push )
  #pragma warning( disable : 4595 )  /* warning C4595: 'operator delete': non-member operator new or delete functions may not be declared inline */
#endif

/*-------------------------------------------------------------------------*/

// ***** Global overloaded new/delete operators

// ***** These two routines should never get called, however they are provided here for 
// ***** clarity and to through.
//inline void* operator new( size_t size )    { return malloc( size ); }
//inline void* operator new[]( size_t size )  { return malloc( size ); }

/**
 * operator new():
 *  Here is the overloaded new operator, responsible for allocating and tracking the requested
 *  memory.
 * 
 *  Return Type: void* -> A pointer to the requested memory.
 *  Arguments: 
 *  	size_t size	     : The size of memory requested in BYTES
 *  	const char *file : The file responsible for requesting the allocation.
 *  	int line	       : The line number within the file requesting the allocation.
 */
inline void* operator new( size_t size, const char *file, int line ) 
{
    if ( 0 == LazyLoadMemoryManager() ) 
        { GUCEF_ASSERT_ALWAYS; return 0; }
    return fp_MEMMAN_AllocateMemory( file, line, size, MM_NEW, GUCEF_NULL ); 
}

/*-------------------------------------------------------------------------*/

/**
 * operator new[]():
 *  Here is the overloaded new[] operator, responsible for allocating and tracking the 
 *  requested memory.
 * 
 *  Return Type: void* -> A pointer to the requested memory.
 *  Arguments: 
 *  	size_t size	     : The size of memory requested in BYTES
 *  	const char *file : The file responsible for requesting the allocation.
 *  	int line	       : The line number within the file requesting the allocation.
 */
inline void* operator new[]( size_t size, const char *file, int line )
{
    if ( 0 == LazyLoadMemoryManager() ) 
        { GUCEF_ASSERT_ALWAYS; return 0; }
    return fp_MEMMAN_AllocateMemory( file, line, size, MM_NEW_ARRAY, GUCEF_NULL );
}

/*-------------------------------------------------------------------------*/

/**
 * operator delete():
 *  This routine is responsible for de-allocating the requested memory.
 * 
 *  Return Type: void 
 *  Arguments: 
 *  	void *address	: A pointer to the memory to be de-allocated.
 */
inline void operator delete( void *address )
{
    if ( GUCEF_NULL == address )  
        return;  // ANSI states that delete will allow NULL pointers.
    if ( 0 == LazyLoadMemoryManager() ) 
        { GUCEF_ASSERT_ALWAYS; return; }
    fp_MEMMAN_DeAllocateMemory( address, MM_DELETE );
}

/*-------------------------------------------------------------------------*/

/**
 * operator delete[]():
 *  This routine is responsible for de-allocating the requested memory.
 * 
 *  Return Type: void 
 *  Arguments: 
 *  	void *address	: A pointer to the memory to be de-allocated.
 */
inline void operator delete[]( void *address )
{
    if ( GUCEF_NULL == address )  
        return;  // ANSI states that delete will allow NULL pointers.
    if ( 0 == LazyLoadMemoryManager() ) 
        { GUCEF_ASSERT_ALWAYS; return; }
    fp_MEMMAN_DeAllocateMemory( address, MM_DELETE_ARRAY );
}

// ***** These two routines should never get called, unless an error occures during the 
// ***** allocation process.  These need to be defined to make Visual C++ 6.0 happy.
// ***** If there was an allocation problem these method would be called automatically by 
// ***** the operating system.  C/C++ Users Journal (Vol. 19 No. 4 -> April 2001 pg. 60)  
// ***** has an excellent explanation of what is going on here.
inline void operator delete( void *address, const char *file, int line )   { if ( 0 == LazyLoadMemoryManager() ) { GUCEF_ASSERT_ALWAYS; return; } fp_MEMMAN_DumpLogReport(); free( address ); }
inline void operator delete[]( void *address, const char *file, int line ) { if ( 0 == LazyLoadMemoryManager() ) { GUCEF_ASSERT_ALWAYS; return; } fp_MEMMAN_DumpLogReport(); free( address ); }

/*-------------------------------------------------------------------------*/

#if ( GUCEF_COMPILER == GUCEF_COMPILER_MSVC )
  #pragma warning( pop )
#endif

/*-------------------------------------------------------------------------*/

#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------*/

inline
void*
MEMMAN_malloc( const char *file, int line, size_t size )
{
    return ( 0 == LazyLoadMemoryManager() ? 0 : fp_MEMMAN_AllocateMemory( file, line, size, MM_MALLOC, NULL ) );    
}

/*-------------------------------------------------------------------------*/

inline
void*
MEMMAN_calloc( const char *file, int line, size_t num, size_t size )
{
    return ( 0 == LazyLoadMemoryManager() ? 0 : fp_MEMMAN_AllocateMemory( file, line, size*num, MM_CALLOC, NULL ) );    
}

/*-------------------------------------------------------------------------*/

inline
void*
MEMMAN_realloc( const char *file, int line, void* ptr, size_t size )
{
    return ( 0 == LazyLoadMemoryManager() ? 0 : ( ptr ? fp_MEMMAN_AllocateMemory( file, line, size, MM_REALLOC, ptr ) : fp_MEMMAN_AllocateMemory( file, line, size, MM_MALLOC, NULL ) ) );
}

/*-------------------------------------------------------------------------*/

inline
void
MEMMAN_free( const char *file, int line, void* ptr )
{
    ( 0 == LazyLoadMemoryManager() ? 0 : fp_MEMMAN_DeAllocateMemoryEx( file, line, ptr, MM_FREE  ) );
}

/*-------------------------------------------------------------------------*/

#ifdef GUCEF_MEMCHECK_OLEAPI

inline
wchar_t* 
MEMMAN_SysAllocString( const char *file, int line, wchar_t* wcharStr )
{    
    wchar_t* ptr = ( 0 == LazyLoadMemoryManager() ? 0 : fp_MEMMAN_SysAllocString( file, line, wcharStr ) );
    //fp_MEMMAN_DumpMemoryAllocations();
    return ptr;
}

/*-------------------------------------------------------------------------*/

inline
wchar_t* 
MEMMAN_SysAllocStringByteLen( const char *file, int line, const char* str, unsigned int bufferSize )
{
    wchar_t* ptr = ( 0 == LazyLoadMemoryManager() ? 0 : fp_MEMMAN_SysAllocStringByteLen( file, line, str, bufferSize ) );
    //fp_MEMMAN_DumpMemoryAllocations();
    return ptr;
}

/*-------------------------------------------------------------------------*/

inline
wchar_t* 
MEMMAN_SysAllocStringLen( const char *file, int line, const wchar_t* str, unsigned int charsToCopy )
{
    wchar_t* ptr = ( 0 == LazyLoadMemoryManager() ? 0 : fp_MEMMAN_SysAllocStringLen( file, line, str, charsToCopy ) );
    //fp_MEMMAN_DumpMemoryAllocations();
    return ptr;
}

/*-------------------------------------------------------------------------*/

inline
void 
MEMMAN_SysFreeString( const char *file, int line, wchar_t* bstrString )
{
    ( 0 == LazyLoadMemoryManager() ? 0 : fp_MEMMAN_SysFreeString( file, line, bstrString ) );
    //fp_MEMMAN_DumpMemoryAllocations();
}

/*-------------------------------------------------------------------------*/

inline
int 
MEMMAN_SysReAllocString( const char* file, int line, wchar_t** pbstr, const wchar_t* psz )
{
    int result = ( 0 == LazyLoadMemoryManager() ? 0 : fp_MEMMAN_SysReAllocString( file, line, pbstr, psz ) );
    //fp_MEMMAN_DumpMemoryAllocations();
    return result;
}

/*-------------------------------------------------------------------------*/

inline
int 
MEMMAN_SysReAllocStringLen( const char* file, int line, wchar_t** pbstr, const wchar_t* psz, unsigned int len )
{
    int result = ( 0 == LazyLoadMemoryManager() ? 0 : fp_MEMMAN_SysReAllocStringLen( file, line, pbstr, psz, len ) );
    //fp_MEMMAN_DumpMemoryAllocations();
    return result;
}

#endif /* GUCEF_MEMCHECK_OLEAPI ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Macros that redirect the standard allocation and deallocation utilities
 *      to our memory manager.
 */

#ifdef __cplusplus

#undef new
#undef delete

#define new              new( __FILE__, __LINE__ )
/* #define delete           MEMMAN_SetOwner( __FILE__, __LINE__ ) ? delete : delete <- fix me */

#endif /* __cplusplus ? */

#undef malloc
#undef calloc
#undef realloc
#undef free

#define malloc(size)     MEMMAN_malloc( __FILE__, __LINE__, size )
#define calloc(num, sz)  MEMMAN_calloc( __FILE__, __LINE__, num, sz )
#define realloc(ptr, sz) MEMMAN_realloc( __FILE__, __LINE__, ptr, sz )
#define free(ptr)        MEMMAN_free( __FILE__, __LINE__, ptr )

#undef GUCEF_CHECKMEM
#undef GUCEF_CHECKMEMSEG
#define GUCEF_CHECKMEM( addr, size ) ( 0 == LazyLoadMemoryManager() ? 0 : fp_MEMMAN_Validate( addr, size, __FILE__, __LINE__ ) )
#define GUCEF_CHECKMEMSEG( addr, chunk, size ) ( 0 == LazyLoadMemoryManager() ? 0 : fp_MEMMAN_ValidateChunk( addr, chunk, size, __FILE__, __LINE__ ) )

#ifdef GUCEF_MEMCHECK_OLEAPI

#undef SysAllocString
#undef SysAllocStringByteLen
#undef SysAllocStringLen
#undef SysFreeString
#undef SysReAllocString
#undef SysReAllocStringLen
#define SysAllocString( wcharStr )              MEMMAN_SysAllocString( __FILE__, __LINE__, wcharStr )
#define SysAllocStringByteLen( psz, len )       MEMMAN_SysAllocStringByteLen( __FILE__, __LINE__, psz, len )
#define SysAllocStringLen( strIn, ui )          MEMMAN_SysAllocStringLen( __FILE__, __LINE__, strIn, ui )
#define SysFreeString( bstrString )             MEMMAN_SysFreeString( __FILE__, __LINE__, bstrString )
#define SysReAllocString( pbstr, psz )          MEMMAN_SysReAllocString( __FILE__, __LINE__, pbstr, psz )
#define SysReAllocStringLen( pbstr, psz, len )  MEMMAN_SysReAllocStringLen( __FILE__, __LINE__, pbstr, psz, len )

#endif /* GUCEF_MEMCHECK_OLEAPI ? */

/*-------------------------------------------------------------------------*/

#else  /* GUCEF_USE_MEMORY_LEAK_CHECKER && GUCEF_USE_PLATFORM_MEMORY_LEAK_CHECKER? */

#ifndef GUCEF_DYNNEWOFF_H 
#include "gucef_dynnewoff.h"
#define GUCEF_DYNNEWOFF_H
#endif /* GUCEF_DYNNEWOFF_H ? */

#endif /* GUCEF_USE_MEMORY_LEAK_CHECKER && GUCEF_USE_PLATFORM_MEMORY_LEAK_CHECKER? */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_DYNNEWON_H ? */
