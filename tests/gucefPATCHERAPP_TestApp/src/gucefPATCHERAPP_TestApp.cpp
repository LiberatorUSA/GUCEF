/*
 *  gucefPATCHERAPP_TestApp: Application for testing gucefPATCHERAPP
 *  Copyright (C) 2002 - 2013.  Dinand Vanvelzen
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_LOADER_H
#include "gucefLOADER.h"
#define GUCEF_LOADER_H
#endif /* GUCEF_LOADER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

using namespace GUCEF;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Application entry point
 */
GUCEF_OSMAIN_BEGIN
{
    try
    {        
        #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

        // the test dir path is based on a CMake win32 build
        LOADER::Int32 appReturnCode = LOADER::LoadAndRunGucefPlatformApp( "gucefPATCHERAPP", "$MODULEDIR$/../../../TestData/gucefPATCHERAPP_TestApp/mswin", 0, 0, 0, 0, 0, 0 );

        #endif

        return 1;
    }
    catch ( ... )
    {

    }
    return 1;
}
GUCEF_OSMAIN_END

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 10-12-2006
        - Dinand: Added this comment section

---------------------------------------------------------------------------*/
