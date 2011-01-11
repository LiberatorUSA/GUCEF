/*
 *  GUCEF platform wide macros/defines for the main app entry point
 *  Copyright (C) 2002 - 2011.  Dinand Vanvelzen
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
 
 #ifndef GUCEF_OSMAIN_H
 #define GUCEF_OSMAIN_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/
 
#ifndef GUCEF_PLATFORM_H
#include "gucef_platform.h"
#define GUCEF_PLATFORM_H
#endif /* GUCEF_PLATFORM_H ? */
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/ 

/*
 *  Here we will define the GUCEF_OSMAIN macro which defines a entry point
 *  and makes the following variables available and application parameters:
 *      int argc
 *      char** argv
 *
 *  This makes the parsing of application parameters uniform
 */
 
#if GUCEF_PLATFORM == GUCEF_PLATFORM_WIN32

    #include <windows.h>
    #undef min
    #undef max

    #define GUCEF_OSMAIN_BEGIN                \
                                              \
    int __stdcall                             \
    WinMain( HINSTANCE hinstance     ,        \
             HINSTANCE hprevinstance ,        \
             LPSTR lpcmdline         ,        \
             int ncmdshow            )        \
    {                                         \
                                              \
        int argc = 0;                         \
        char** argv = &lpcmdline;             \
        if ( lpcmdline != NULL )              \
        {                                     \
            if ( *lpcmdline != '\0' )         \
            {                                 \
                argc = 1;                     \
            }                                 \
        }
                                              
    #define GUCEF_OSMAIN_END }

#else

    #define GUCEF_OSMAIN_BEGIN                \
                                              \
    int                                       \
    main( int argc , char* argv[] )           \
    {
    
    #define GUCEF_OSMAIN_END }

#endif

/*-------------------------------------------------------------------------*/ 
 
 #endif /* GUCEF_OSMAIN_H ? */
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 02-01-2007 :
       - Initial version of this file.

---------------------------------------------------------------------------*/
