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

#ifndef GUCEF_CORE_DVMD5UTILS_H
#define GUCEF_CORE_DVMD5UTILS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_IOACCESS_H
#include "ioaccess.h"     /* medium & source independant file handling utils */
#define GUCEF_CORE_IOACCESS_H
#endif /* GUCEF_CORE_IOACCESS_H ? */

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

#ifndef GUCEF_CORE_NO_MD5_SUPPORT

/*-------------------------------------------------------------------------*/

/**
 *	Function that returns an MD5 digest generated from the resource given
 *	Returns 1 on success and 0 on failure.
 */
GUCEF_CORE_PUBLIC_C UInt32
md5fromfile( TIOAccess* access  ,
             UInt8 digest[ 16 ] );

/*--------------------------------------------------------------------------*/

/**
 *	Converts an md5 digest into a string 48 chars long.
 *	Each digest value is converted to 3 characters.
 *	A null terminator is not included.
 */
GUCEF_CORE_PUBLIC_C void
md5tostring( const UInt8 digest[ 16 ] ,
             char md5_str[ 48 ]       );

/*--------------------------------------------------------------------------*/

/**
 *	Converts the given null-terminated string
 *      into an md5 digest
 */
GUCEF_CORE_PUBLIC_C void
md5fromstring( const char* string ,
               UInt8 digest[ 16 ] );

/*--------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_NO_MD5_SUPPORT ? */

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

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_DVMD5UTILS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 21-09-2005 :
        - Added stringtomd5string()
- 25-11-2004 :
        - Got rid of dependancy on MFILE and duplicate impementations for
          different recource sources. Instead TIOAccess is used now.

---------------------------------------------------------------------------*/

