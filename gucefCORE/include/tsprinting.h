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

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"     /* often used gucef macros */
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

/*--------------------------------------------------------------------------*/

/**
 *	Threadsafe version of printf(). This function does not garantee any order
 *	in which the output is generated from multiple threads but it does
 *	prevent output from getting mixed.
 */
GUCEF_CORE_PUBLIC_C int
tsprintf( const char* __format, ... );

/*-------------------------------------------------------------------------*/

/**
 *	Threadsafe version of fprintf(). This function does not garantee any order
 *	in which the output is generated from multiple threads but it does
 *	prevent output from getting mixed.
 */
GUCEF_CORE_PUBLIC_C int
tsfprintf( FILE* __stream, const char* __format, ... );

/*-------------------------------------------------------------------------*/

/**
 *	Function that allows you to set the console output file.
 *	The console output file will be used if tsusecoutfile() was called
 *	with argument true.
 */
GUCEF_CORE_PUBLIC_C void
tssetcoutfile( const char *cout_file );

/*-------------------------------------------------------------------------*/

/**
 *	Set wheter or not to use a file for console output.
 *	the file specified with tssetcoutfile() will be used.
 */
GUCEF_CORE_PUBLIC_C void
tsusecoutfile( UInt32 use );

/*-------------------------------------------------------------------------*/

/**
 *      Set wheter tsprintf() should output to console.
 */
GUCEF_CORE_PUBLIC_C void
tspconsoleout( UInt32 use );

/*-------------------------------------------------------------------------*/

/**
 *      It is recommended to use this fuction for clearing the contents of a
 *      console window. It wraps the appropriote O/S function and uses the
 *      proper file stream used by tspinit(). Under WIN32 the outpur streams get
 *      reopened for a non-console application so you can't clear the console
 *      window using a standard O/S function.
 */
GUCEF_CORE_PUBLIC_C void
console_clrscr( void );

/*-------------------------------------------------------------------------*/

/**
 *	Initializes the mutex's needed for the threadsafe printing functions to
 *	work.
 */
GUCEF_CORE_PUBLIC_C void
tspinit( void );

/*-------------------------------------------------------------------------*/

/**
 *	Cleanup after a tspinit() has been used.
 */
GUCEF_CORE_PUBLIC_C void
tspshutdown( void );

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
   };
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
        - Dinand: Added console_clrscr() for a portable method of clearing the console
          window. Is is even manditory when making a WIN32 non-console
          application because then the console was application allocated.
          The catch is that tspinit() must have been called in order for
          this to function. By you should call that function anyways when using
          these functions.
- 18-04-2004 :
        - Dinand: Added tspconsoleout() so that console output can be enabled/disabled
          at run-time aswell instead of just design-time.
- 29-12-2003 :
        - Dinand: Added the option to route console output to a file of choice.
- 26-10-2003 :
        - Dinand: Created this file because my debugging fprintf() info was getting
          mucked up because multiple threads where printing info at the same
          time.

---------------------------------------------------------------------------*/

