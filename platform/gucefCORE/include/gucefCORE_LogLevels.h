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

#ifndef GUCEF_CORE_LOGLEVELS_H
#define GUCEF_CORE_LOGLEVELS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"    
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifdef __cplusplus

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"
#define GUCEF_CORE_CDVSTRING_H
#endif /* GUCEF_CORE_CDVSTRING_H ? */

#endif 

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

// A number of optional constants that can be used to define log levels.
// These levels are used by the library itself. The lower the importance of the log level
// the more likely it will be filtered out by most logging backends
extern "C"
{

GUCEF_CORE_PUBLIC_C extern const Int32 LOGLEVEL_CRITICAL;
GUCEF_CORE_PUBLIC_C extern const Int32 LOGLEVEL_VERY_IMPORTANT;
GUCEF_CORE_PUBLIC_C extern const Int32 LOGLEVEL_IMPORTANT;
GUCEF_CORE_PUBLIC_C extern const Int32 LOGLEVEL_NORMAL;
GUCEF_CORE_PUBLIC_C extern const Int32 LOGLEVEL_BELOW_NORMAL;
GUCEF_CORE_PUBLIC_C extern const Int32 LOGLEVEL_EVERYTHING;

}

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus 

/**
 *  Function that will convert the loglevel values above into a string equivalant
 *  and the rest into numbers.
 */
GUCEF_CORE_PUBLIC_CPP CString
LogLevelToString( const Int32 logLevel );

#endif

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_LOGLEVELS_H ? */
