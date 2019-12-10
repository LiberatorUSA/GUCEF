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

#ifndef GUCEF_CORE_CALLSTACK_H
#define GUCEF_CORE_CALLSTACK_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_ETYPES_H
#include "gucefMT_ETypes.h"
#define GUCEF_MT_ETYPES_H
#endif /* GUCEF_MT_ETYPES_H ? */

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
GUCEF_CORE_PUBLIC_C void
GUCEF_UtilityCodeBegin( const char* file ,
                        int line         );

/*-------------------------------------------------------------------------*/

/**
 *      Should be called at the end of a function.
 */
GUCEF_CORE_PUBLIC_C void
GUCEF_UtilityCodeEnd( void );

/*-------------------------------------------------------------------------*/

/**
 *      Prints the current call stack to stdout
 */
GUCEF_CORE_PUBLIC_C void
GUCEF_PrintCallstack( void );

/*-------------------------------------------------------------------------*/

/**
 *      Dumps the current call stack to the file with the given name
 *
 *      @param filename path and name of the output file.
 */
GUCEF_CORE_PUBLIC_C void
GUCEF_DumpCallstack( const char* filename );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_C void
GUCEF_LogStackTo( const char* filename );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_C void
GUCEF_LogStackToStdOut( void );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_C void
GUCEF_SetStackLogging( const UInt32 logStackBool );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_C void
GUCEF_SetStackLoggingInCvsFormat( const UInt32 logAsCvsBool );

/*-------------------------------------------------------------------------*/

typedef void (*TStackPushCallback) ( const char* fileName     ,
                                     const int lineNr         ,
                                     const UInt32 threadId    ,
                                     const UInt32 stackHeight );

GUCEF_CORE_PUBLIC_C void
GUCEF_SetStackPushCallback( TStackPushCallback callback );

/*-------------------------------------------------------------------------*/

typedef void (*TStackPopCallback) ( const char* fileName     ,
                                    const int lineNr         ,
                                    const UInt32 threadId    ,
                                    const UInt32 stackHeight ,
                                    const UInt32 ticksSpent  );

GUCEF_CORE_PUBLIC_C void
GUCEF_SetStackPopCallback( TStackPopCallback callback );

/*-------------------------------------------------------------------------*/

/**
 *  Initializes the call stack utility, should be called before using any of
 *  the functions.
 */
GUCEF_CORE_PUBLIC_C void
GUCEF_InitCallstackUtility( void );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_C void
GUCEF_ShutdowntCallstackUtility( void );

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

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef GUCEF_DEBUG_MODE
  #ifdef GUCEF_DEBUG_CALLSTACK_TRACKING
    #define GUCEF_CALLSTACK_TRACKING
  #endif
#else
  #ifdef GUCEF_RELEASE_CALLSTACK_TRACKING
    #define GUCEF_CALLSTACK_TRACKING
  #endif
#endif

/*-------------------------------------------------------------------------*/

#ifdef GUCEF_CALLSTACK_TRACKING
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
