/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
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

#ifndef GUCEF_CORE_TSPRINTING_H
#define GUCEF_CORE_TSPRINTING_H

/*
 *	Threadsafe versions of printing verions. SDL mutexes are simply used
 *	to wrap the printing functions. This does require SDL to be initialized
 *	before using these functions. Note that the init and shutdown functions
 *	should not be considdered to be threadsafe. If you call the printing
 *	functions without having done a init then the standard version will be
 *	used.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <stdio.h>	     /* needed for FILE* here */

#ifndef GUCEF_CORE_ETYPES_H
#include "gucefCORE_ETypes.h"	     /* small types used */
#define GUCEF_CORE_ETYPES_H
#endif /* GUCEF_CORE_ETYPES_H ? */

#ifndef GUCEF_CORE_GUCEFCORE_MACROS_H
#include "gucefCORE_macros.h"     /* often used gucef macros */
#define GUCEF_CORE_GUCEFCORE_MACROS_H
#endif /* GUCEF_CORE_GUCEFCORE_MACROS_H ? */

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

/*--------------------------------------------------------------------------*/

/**
 *	Threadsafe version of printf(). This function does not garantee any order
 *	in which the output is generated from multiple threads but it does
 *	prevent output from getting mixed.
 */
int
EXPORT_C tsprintf( const char* __format, ... );

/*-------------------------------------------------------------------------*/

/**
 *	Threadsafe version of fprintf(). This function does not garantee any order
 *	in which the output is generated from multiple threads but it does
 *	prevent output from getting mixed.
 */
int 
EXPORT_C tsfprintf( FILE* __stream, const char* __format, ... );

/*-------------------------------------------------------------------------*/

/**
 *	Function that allows you to set the console output file.
 *	The console output file will be used if tsusecoutfile() was called
 *	with argument true.
 */
void
EXPORT_C tssetcoutfile( const char *cout_file );

/*-------------------------------------------------------------------------*/

/**
 *	Set wheter or not to use a file for console output.
 *	the file specified with tssetcoutfile() will be used.
 */
void
EXPORT_C tsusecoutfile( UInt32 use );

/*-------------------------------------------------------------------------*/

/**
 *      Set wheter tsprintf() should output to console.
 */
void
EXPORT_C tspconsoleout( UInt32 use );

/*-------------------------------------------------------------------------*/

/**
 *      It is recommended to use this fuction for clearing the contents of a
 *      console window. It wraps the appropriote O/S function and uses the
 *      proper file stream used by tspinit(). Under WIN32 the outpur streams get
 *      reopened for a non-console application so you can't clear the console
 *      window using a standard O/S function.
 */
void
EXPORT_C console_clrscr( void );

/*-------------------------------------------------------------------------*/

/**
 *	Initializes the mutex's needed for the threadsafe printing functions to
 *	work. 
 */
void
EXPORT_C tspinit( void );

/*-------------------------------------------------------------------------*/

/**
 *	Cleanup after a tspinit() has been used.
 */
void
EXPORT_C tspshutdown( void );

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

#endif /* GUCEF_CORE_TSPRINTING_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 19-04-2004 :
        - Added console_clrscr() for a portable method of clearing the console
          window. Is is even manditory when making a WIN32 non-console
          application because then the console was application allocated.
          The catch is that tspinit() must have been called in order for
          this to function. By you should call that function anyways when using
          these functions.
- 18-04-2004 :
        - Added tspconsoleout() so that console output can be enabled/disabled
          at run-time aswell instead of just design-time.
- 29-12-2003 :
        - Added the option to route console output to a file of choice.
- 26-10-2003 :
        - Created this file because my debugging fprintf() info was getting
          mucked up because multiple threads where printing info at the same
          time.

---------------------------------------------------------------------------*/

