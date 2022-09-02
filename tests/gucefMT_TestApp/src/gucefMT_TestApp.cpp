/*
 *  gucefMT_TestApp: GUCEF test application for the MT module
 *  Copyright (C) 2002 - 2022.  Dinand Vanvelzen
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

#ifndef GUCEF_MT_H
#include "gucefMT.h"
#define GUCEF_MT_H
#endif /* GUCEF_MT_H ? */

#ifndef GUCEF_MT_TESTAPP_TESTREADERWRITERLOCK_H
#include "TestReaderWriterLock.h"
#define GUCEF_MT_TESTAPP_TESTREADERWRITERLOCK_H
#endif /* GUCEF_MT_TESTAPP_TESTREADERWRITERLOCK_H ? */

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
        PerformReaderWriterLockTests();

        return 1;                                                                            
    }
    catch ( ... )
    {
        return -1;                                                      
    }
    return 1;                                                                                                                              
}
GUCEF_OSMAIN_END

/*-------------------------------------------------------------------------*/
