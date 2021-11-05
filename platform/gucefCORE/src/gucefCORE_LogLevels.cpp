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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#include "gucefCORE_LogLevels.h"    

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

extern "C" {

const Int32 LOGLEVEL_CRITICAL = GUCEFCORE_INT32MAX - 1;
const Int32 LOGLEVEL_VERY_IMPORTANT = 250000;
const Int32 LOGLEVEL_IMPORTANT = 100000;
const Int32 LOGLEVEL_NORMAL = 50000;
const Int32 LOGLEVEL_BELOW_NORMAL = 25000;
const Int32 LOGLEVEL_EVERYTHING = 0;

}

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus 

CString
LogLevelToString( const Int32 logLevel )
{GUCEF_TRACE;

    switch ( logLevel )
    {
        case LOGLEVEL_CRITICAL : return "CRITICAL";
        case LOGLEVEL_VERY_IMPORTANT : return "VERY_IMPORTANT";
        case LOGLEVEL_IMPORTANT : return "IMPORTANT";
        case LOGLEVEL_NORMAL : return "NORMAL";
        case LOGLEVEL_BELOW_NORMAL : return "BELOW_NORMAL";
        case LOGLEVEL_EVERYTHING : return "EVERYTHING";
        default : return Int32ToString( logLevel );
    }
}

#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
