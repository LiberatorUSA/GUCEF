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

#ifndef GUCEF_CORE_DVCPPOSWRAP_H
#define GUCEF_CORE_DVCPPOSWRAP_H

/*
 *      This file contains a set of O/S wrapping functions.
 *      The main idea is that you call these functions instead of the native
 *      O/S functions so that your code can become/stay portable.
 *      macro's are used to direct the function calls to the correct native
 *      O/S functions.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"        /* basic C versions of functions exposed here */
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H ? */

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"
#define GUCEF_CORE_CDVSTRING_H
#endif /* GUCEF_CORE_CDVSTRING_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP UInt32
SetEnv( const CString& key   ,
        const CString& value );

/*--------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP void
SetEnvOverride( const CString& key   ,
                const CString& value );

/*--------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP void
SetEnvOverrides( const CString& allKeysAndValues );

/*--------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP CString
GetEnv( const CString& key );

/*--------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP bool 
GetExeNameForProcessId( TProcessId* pid  , 
                        CString& exeName );

/*--------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP CString
GetHostname( void );

/*--------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP bool
CommandLineExecute( const CString& command, CString& result, bool waitForExit );

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_DVCPPOSWRAP_H ? */

/*--------------------------------------------------------------------------//
//                                                                          //
//      Info & Changes                                                      //
//                                                                          //
//--------------------------------------------------------------------------//

- 29-05-2005 :
        - Debugged StringToClipboard(): it is now operational.
        - Debugged StringFromClipboard(): it is now operational.
- 02-02-2004 :
        - Created this set of functions.

----------------------------------------------------------------------------*/

