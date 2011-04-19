/*
 *  gucefLOADER: GUCEF module which loads up the GUCEF platform
 *  Copyright (C) 2002 - 2011.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
 */

#ifndef GUCEF_LOADER_H
#define GUCEF_LOADER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_LOADER_MACROS_H
#include "gucefLOADER_macros.h"
#define GUCEF_LOADER_MACROS_H
#endif /* GUCEF_LOADER_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_LOADER_PUBLIC_C void*
LoadModules( const char* groupName      ,
             unsigned long mayorVersion ,
             unsigned long minorVersion ,
             long patchVersion          ,
             long releaseVersion        ,
             char** argv                ,
             int argc                   );
             
/*-------------------------------------------------------------------------*/

GUCEF_LOADER_PUBLIC_C void
UnloadModules( void* moduleData );

/*-------------------------------------------------------------------------*/

GUCEF_LOADER_PUBLIC_C void*
LoadGucefPlatform( unsigned long mayorVersion   ,
                   unsigned long minorVersion   ,
                   char** argv                  ,
                   int argc                     );


/*-------------------------------------------------------------------------*/

GUCEF_LOADER_PUBLIC_C void*
LoadGucefPlatformEx( unsigned long mayorVersion   ,
                     unsigned long minorVersion   ,
                     unsigned long patchVersion   ,
                     unsigned long releaseVersion ,
                     char** argv                  ,
                     int argc                     );
                   
/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_LOADER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 12-02-2005 :
        - Initial implementation

-----------------------------------------------------------------------------*/
