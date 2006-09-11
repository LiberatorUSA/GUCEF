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
 *      These #defines and utilities are the core of the memory manager.
 *      This overrides standard memory allocation and de-allocation routines 
 *      and replaces them with the memory manager's versions.
 *      This allows for memory tracking and statistics to be generated.
 */
 
#ifndef GUCEF_NEW_ON_H 
#define GUCEF_NEW_ON_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/
                            
#ifndef GUCEF_CORE_MEMORYMANAGER_H
#include "MemoryManager.h"      /* gucefCORE memory manager */
#define GUCEF_CORE_MEMORYMANAGER_H
#endif /* GUCEF_CORE_MEMORYMANAGER_H ? */

#ifdef ADD_MEMORY_MANAGER

#ifndef GUCEF_CORE_CALLSTACK_H 
#include "callstack.h"
#define GUCEF_CORE_CALLSTACK_H
#endif /* GUCEF_CORE_CALLSTACK_H ? */                         

#include "gucef_new_off.h"      /* Make sure that the new/delete are not declared to avoid circular definitions. */
#include <stdlib.h>             /* Required for malloc() and free() */

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

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
  return MEMMAN_AllocateMemory( file, line, size, MM_NEW, NULL ); 
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
  return MEMMAN_AllocateMemory( file, line, size, MM_NEW_ARRAY, NULL );
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

	MEMMAN_DeAllocateMemory( address, MM_DELETE );
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
	
	MEMMAN_DeAllocateMemory( address, MM_DELETE_ARRAY );
}

// ***** These two routines should never get called, unless an error occures during the 
// ***** allocation process.  These need to be defined to make Visual C++ 6.0 happy.
// ***** If there was an allocation problem these method would be called automatically by 
// ***** the operating system.  C/C++ Users Journal (Vol. 19 No. 4 -> April 2001 pg. 60)  
// ***** has an excellent explanation of what is going on here.
inline void operator delete( void *address, const char *file, int line )   { GUCEF_PrintCallstack(); GUCEF_DumpCallstack( "GUCEFCallstack.txt" ); tsprintf( "MEMMAN: allocation error for address %p @ %s:%d\n", address, file, line ); MEMMAN_DumpLogReport(); free( address ); }
inline void operator delete[]( void *address, const char *file, int line ) { GUCEF_PrintCallstack(); GUCEF_DumpCallstack( "GUCEFCallstack.txt" ); tsprintf( "MEMMAN: allocation error for address %p @ %s:%d\n", address, file, line ); MEMMAN_DumpLogReport(); free( address ); }

/*-------------------------------------------------------------------------*/

#endif /* __cplusplus ? */

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
#define new              new( __FILE__, __LINE__ )
#define delete           (MEMMAN_SetOwner( __FILE__, __LINE__ ), false) ? MEMMAN_SetOwner( "", 0 ) : delete
#endif /* __cplusplus ? */

#define malloc(sz)       MEMMAN_AllocateMemory(   __FILE__, __LINE__, sz,     MM_MALLOC, NULL )
#define calloc(num, sz)  MEMMAN_AllocateMemory(   __FILE__, __LINE__, sz*num, MM_CALLOC, NULL )
#define realloc(ptr, sz) ( ptr ? MEMMAN_AllocateMemory( __FILE__, __LINE__, sz, MM_REALLOC, ptr ) : MEMMAN_AllocateMemory( __FILE__, __LINE__, sz, MM_MALLOC, NULL ) ) 
#define free(sz)         MEMMAN_DeAllocateMemory( sz, MM_FREE  )

#undef CHECKMEM
#undef CHECKMEMSEG
#define CHECKMEM( addr, size ) MEMMAN_Validate( addr, size, __FILE__, __LINE__ )
#define CHECKMEMSEG( addr, chunk, size ) MEMMAN_ValidateChunk( addr, chunk, size, __FILE__, __LINE__ )

/*-------------------------------------------------------------------------*/

#else

#include "gucef_new_off.h" 

#endif /* ADD_MEMORY_MANAGER ? */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_NEW_ON_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 11-04-2005 :
       - Initial version of this file.

-----------------------------------------------------------------------------*/