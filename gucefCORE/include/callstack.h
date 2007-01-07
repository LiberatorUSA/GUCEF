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
 
#ifndef GUCEF_CORE_CALLSTACK_H
#define GUCEF_CORE_CALLSTACK_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

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
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef DEBUG_MODE

#ifdef __cplusplus
   extern "C" {
#endif   /* __cplusplus */

/*-------------------------------------------------------------------------*/

/**
 *      Should be called at the beginning of a function.
 *
 *      @param file filename of the source file from where this function is called
 *      @param line line number in the caller source file
 */
GUCEFCORE_EXPORT_C void 
GUCEF_UtilityCodeBegin( const char* file ,
                        int line         );

/*-------------------------------------------------------------------------*/
         
/**
 *      Should be called at the end of a function.
 */
GUCEFCORE_EXPORT_C void                        
GUCEF_UtilityCodeEnd( void );

/*-------------------------------------------------------------------------*/
           
/**
 *      Prints the current call stack to stdout
 */
GUCEFCORE_EXPORT_C void                      
GUCEF_PrintCallstack( void );

/*-------------------------------------------------------------------------*/ 

/**
 *      Dumps the current call stack to the file with the given name
 *
 *      @param filename path and name of the output file.
 */
GUCEFCORE_EXPORT_C void
GUCEF_DumpCallstack( const char* filename );

/*-------------------------------------------------------------------------*/

GUCEFCORE_EXPORT_C void
GUCEF_LogStackTo( const char* filename );

/*-------------------------------------------------------------------------*/

GUCEFCORE_EXPORT_C void
GUCEF_LogStackToStdOut( void );

/*-------------------------------------------------------------------------*/

GUCEFCORE_EXPORT_C void
GUCEF_SetStackLogging( const UInt32 logStackBool );

/*-------------------------------------------------------------------------*/

/**
 *  Initializes the call stack utility, should be called before using any of 
 *  the functions.
 */  
GUCEFCORE_EXPORT_C void
GUCEF_InitCallstackUtility( void );

/*-------------------------------------------------------------------------*/

GUCEFCORE_EXPORT_C void
GUCEF_ShutdowntCallstackUtility( void );
                                            
/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
   }
#endif /* __cplusplus */

#endif /* DEBUG_MODE ? */
                      
/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
}; /* namespace CORE */
}; /* namespace GUCEF */
#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef DEBUG_MODE
  #define GUCEF_BEGIN { GUCEF_UtilityCodeBegin( __FILE__, __LINE__ ); }
  #define GUCEF_END { GUCEF_UtilityCodeEnd(); }
  #define GUCEF_END_RET( retvaltype, retval ) { retvaltype var( retval ); GUCEF_UtilityCodeEnd(); return var; }
#else
  #define GUCEF_BEGIN
  #define GUCEF_END
  #define GUCEF_END_RET( retvaltype, retval ) return (retval);
#endif    

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CALLSTACK_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 22-04-2005 :
        - Added protection against missing GUCEF_END segments which would result in
          the stack index going out of bounds.
- 16-04-2005 :
        - Initial version to help with debugging

---------------------------------------------------------------------------*/
