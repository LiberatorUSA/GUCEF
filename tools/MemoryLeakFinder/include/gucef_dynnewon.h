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

//#ifdef USE_MEMORY_LEAK_CHECKER

#include "gucef_dynnewoff.h"      /* Make sure that the new/delete are not declared to avoid circular definitions. */

#include <stdlib.h>               /* Required for malloc() and free() */
#include <assert.h>

#ifdef __cplusplus

#include <new>
#include <crtdbg.h>

#endif /* __cplusplus ? */

#ifndef GUCEF_CORE_DYNMEMORYMANAGERLOADER_H
#include "DynMemoryManagerLoader.h"
#define GUCEF_CORE_DYNMEMORYMANAGERLOADER_H
#endif /* GUCEF_CORE_DYNMEMORYMANAGERLOADER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef MEMCHECK_NEWDELETE
#ifdef __cplusplus

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
    if ( 0 == LazyLoadMemoryManager() ) { assert( 0 ); return 0; }
    return fp_MEMMAN_AllocateMemory( file, line, size, MM_NEW, NULL ); 
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
    if ( 0 == LazyLoadMemoryManager() ) { assert( 0 ); return 0; }
    return fp_MEMMAN_AllocateMemory( file, line, size, MM_NEW_ARRAY, NULL );
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
    if (!address) return;  // ANSI states that delete will allow NULL pointers.
    if ( 0 == LazyLoadMemoryManager() ) { assert( 0 ); return; }
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
    if (!address) return;  // ANSI states that delete will allow NULL pointers.
    if ( 0 == LazyLoadMemoryManager() ) { assert( 0 ); return; }
    fp_MEMMAN_DeAllocateMemory( address, MM_DELETE_ARRAY );
}

// ***** These two routines should never get called, unless an error occures during the 
// ***** allocation process.  These need to be defined to make Visual C++ 6.0 happy.
// ***** If there was an allocation problem these method would be called automatically by 
// ***** the operating system.  C/C++ Users Journal (Vol. 19 No. 4 -> April 2001 pg. 60)  
// ***** has an excellent explanation of what is going on here.
inline void operator delete( void *address, const char *file, int line )   { if ( 0 == LazyLoadMemoryManager() ) { assert( 0 ); return; } fp_MEMMAN_DumpLogReport(); free( address ); }
inline void operator delete[]( void *address, const char *file, int line ) { if ( 0 == LazyLoadMemoryManager() ) { assert( 0 ); return; } fp_MEMMAN_DumpLogReport(); free( address ); }

/*-------------------------------------------------------------------------*/

#endif /* __cplusplus ? */
#endif /* MEMCHECK_NEWDELETE ? */

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
    return ( 0 == LazyLoadMemoryManager() ? 0 : fp_MEMMAN_DeAllocateMemoryEx( file, line, ptr, MM_FREE  ) );
}

/*-------------------------------------------------------------------------*/

#ifdef MEMCHECK_OLEAPI

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

#endif /* MEMCHECK_OLEAPI ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Macros that redirect the standard allocation and deallocation utilities
 *      to our memory manager.
 */

#ifdef MEMCHECK_NEWDELETE 
#ifdef __cplusplus
#define new              new( __FILE__, __LINE__ )
#define delete           ( if ( 0 == LazyLoadMemoryManager() ) { assert( 0 ); return; } fp_MEMMAN_SetOwner( __FILE__, __LINE__ ), false) ? fp_MEMMAN_SetOwner( "", 0 ) : delete
#endif /* __cplusplus ? */
#endif /* MEMCHECK_NEWDELETE ? */

#define malloc(size)     MEMMAN_malloc( __FILE__, __LINE__, size )
#define calloc(num, sz)  MEMMAN_calloc( __FILE__, __LINE__, num, sz )
#define realloc(ptr, sz) MEMMAN_realloc( __FILE__, __LINE__, ptr, sz )
#define free(ptr)        MEMMAN_free( __FILE__, __LINE__, ptr )

#undef CHECKMEM
#undef CHECKMEMSEG
#define CHECKMEM( addr, size ) ( 0 == LazyLoadMemoryManager() ? 0 : fp_MEMMAN_Validate( addr, size, __FILE__, __LINE__ ) )
#define CHECKMEMSEG( addr, chunk, size ) ( 0 == LazyLoadMemoryManager() ? 0 : fp_MEMMAN_ValidateChunk( addr, chunk, size, __FILE__, __LINE__ ) )

#ifdef MEMCHECK_OLEAPI
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
#endif /* MEMCHECK_OLEAPI ? */

/*-------------------------------------------------------------------------*/

//#else

//#include "gucef_dynnewoff.h" 

//#endif /* USE_MEMORY_LEAK_CHECKER ? */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_DYNNEWON_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 11-04-2005 :
       - Initial version of this file.

-----------------------------------------------------------------------------*/