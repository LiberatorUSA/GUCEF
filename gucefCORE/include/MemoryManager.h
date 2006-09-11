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
  * This Memory Manager software provides the following functionality:
  *             1. Seamless interface.
  *             2. Tracking all memory allocations and deallocations.
  *             3. Reporting memory leaks, unallocated memory.
  *             4. Reporting memory bounds violations.
  *             5. Reporting the percentage of allocated memory that is actually being used.
  *             6. Customizable tracking.
  *
  * The code is self documented, thus reading through this header file should tell you
  * everything that you would ever need to know inorder to use the memory manager. 
  */

#ifndef GUCEF_CORE_MEMORYMANAGER_H
#define GUCEF_CORE_MEMORYMANAGER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/
                     
#ifndef GUCEF_CORE_ETYPES_H
#include "gucefCORE_ETypes.h"      /* simple types used */
#define GUCEF_CORE_ETYPES_H
#endif /* GUCEF_CORE_ETYPES_H ? */                     
                     
#ifndef GUCEF_CORE_GUCEFCORE_MACROS_H
#include "gucefCORE_macros.h"      /* gucefCORE module build configuration */
#define GUCEF_CORE_GUCEFCORE_MACROS_H
#endif /* GUCEF_CORE_GUCEFCORE_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Only compile in the memory manager code if one or more modules use
 *      the GUCEF memory manager. The following define check takes care of this.
 */
#ifdef ADD_MEMORY_MANAGER

/*-------------------------------------------------------------------------*/

/*
 *      Prevent C++ name mangling
 */
#ifdef __cplusplus
extern "C" {
#endif

/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/

/*
 *      User API, these functions can be used to set parameters within the Memory 
 *      Manager to control the type and extent of the memory tests that are performed.
 *      Note that it is not necessary to call any of these methods, you will get the default 
 *      Memory Manager automatically.
 */
 
/*-------------------------------------------------------------------------*/

UInt32
MEMMAN_Initialize( void );

/*-------------------------------------------------------------------------*/

void 
MEMMAN_Shutdown( void );

/*-------------------------------------------------------------------------*/ 

/**
 *      MEMMAN_dumpLogReport():
 *      Dump the log report to the file, this is the same method that is automatically called 
 *      upon the programs termination to report all statistical information.
 */ 
EXPORT_C void 
MEMMAN_DumpLogReport( void );

/*-------------------------------------------------------------------------*/

/**
 *      MEMMAN_DumpMemoryAllocations():
 *      Report all allocated memory to the log file.
 */ 
EXPORT_C void 
MEMMAN_DumpMemoryAllocations( void );  

/*-------------------------------------------------------------------------*/

/** 
 *      MEMMAN_SetLogFile():
 *      Allows for the log file to be changed from the default.
 */
EXPORT_C void 
MEMMAN_SetLogFile( const char *file );

/*-------------------------------------------------------------------------*/ 

/**
 *      MEMMAN_SetExhaustiveTesting():
 *      This method allows for exhaustive testing.  It has the same functionality as the following
 *      function calls => setLogAlways( true ); setPaddingSize( 1024 ); 
 */
EXPORT_C void 
MEMMAN_SetExhaustiveTesting( UInt32 test );

/*-------------------------------------------------------------------------*/

/** 
 *      MEMMAN_SetLogAlways():
 *      Sets the flag for exhaustive information logging.  All information is sent to the log file.
 */
EXPORT_C void 
MEMMAN_SetLogAlways( UInt32 log );

/*-------------------------------------------------------------------------*/

/** 
 *      MEMMAN_SetPaddingSize():
 *      Sets the padding size for memory bounds checks.
 *      The default is 4
 */ 
EXPORT_C void 
MEMMAN_SetPaddingSize( UInt32 size );

/*-------------------------------------------------------------------------*/

/**
 *      MEMAN_CleanLogFile():
 *      Cleans out the log file by deleting it.
 */ 
EXPORT_C void 
MEMMAN_CleanLogFile( UInt32 clean );

/*-------------------------------------------------------------------------*/  

/**
 *      MEMMAN_BreakOnAllocation():
 *      Allows you to set a break point on the n-th allocation.
 */ 
EXPORT_C void 
MEMMAN_BreakOnAllocation( int alloccount );

/*-------------------------------------------------------------------------*/

/**
 *      MEMMAN_BreakOnDeallocation():
 *      Sets a flag that will set a break point when the specified memory is deallocated.
 */
EXPORT_C void 
MEMMAN_BreakOnDeallocation( void *address );  

/*-------------------------------------------------------------------------*/

/** 
 *      MEMMAN_BreakOnReallocation():
 *      Sets a flag that will set a break point when the specified memory is reallocated by 
 *      using the realloc() method.
 */
EXPORT_C void 
MEMMAN_BreakOnReallocation( void *address );

/*-------------------------------------------------------------------------*/

EXPORT_C void
MEMMAN_Validate( const void* address ,
                 UInt32 blocksize    ,
                 const char *file    ,
                 int line            );
                 
EXPORT_C void
MEMMAN_ValidateChunk( const void* address ,
                      const void* chunk   ,
                      UInt32 blocksize    ,
                      const char *file    ,
                      int line            );

/*-------------------------------------------------------------------------*/

/*
 *      Memory Manager specific definitions.  Below are the definitions that make up the 
 *      Memory Manager. These functions should not be called by the user directly.
 *      They are called by the redirect versions of new, malloc() ect.
 */
 
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
#define MM_UNKNOWN        0  
#define MM_NEW            1      
#define MM_NEW_ARRAY      2
#define MM_MALLOC         3
#define MM_CALLOC         4
#define MM_REALLOC        5
#define MM_DELETE         6
#define MM_DELETE_ARRAY   7
#define MM_FREE           8
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *      MEMMAN_AllocateMemory():
 *      This is the main memory allocation routine, this is called by all of the other 
 *      memory allocation routines to allocate and track memory.
 */ 
EXPORT_C void* 
MEMMAN_AllocateMemory( const char *file , 
                       int line         , 
                       size_t size      , 
                       char type        , 
                       void *address    ); 

/*-------------------------------------------------------------------------*/

/** 
 *      MEMMAN_DeAllocateMemory():
 *      This is the main memory deallocation routine.  
 *      This method is used by all of the other de-allocation routines 
 *      for de-allocating and tracking memory.
 */
EXPORT_C void 
MEMMAN_DeAllocateMemory( void *address   , 
                         char type       ); 

/*-------------------------------------------------------------------------*/

/** 
 *      MEMMAN_SetOwner():
 *      This method is used by the deallocation methods to record the source file and line 
 *      number that is requesting the allocation.  Note that it is important to create a
 *      seperate method for deallocation since we can not pass the addition parameters to 
 *      the delete methods like we do with the new methods.
 */
EXPORT_C void 
MEMMAN_SetOwner( const char *file , 
                 int line         ); 

/*-------------------------------------------------------------------------*/

/*
 *      End the C++ name mangling protection
 */
#ifdef __cplusplus
};
#endif

/*-------------------------------------------------------------------------*/

#endif /* ADD_MEMORY_MANAGER */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_MEMORYMANAGER_H */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 11-04-2005 :
       - Initial version of this file.

-----------------------------------------------------------------------------*/
