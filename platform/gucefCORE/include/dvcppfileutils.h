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

#ifndef GUCEF_CORE_DVCPPFILEUTILS_H
#define GUCEF_CORE_DVCPPFILEUTILS_H

/*-------------------------------------------------------------------------*/

/*
 *      The utilities are meant to be cross-platform replacements for
 *      O/S dependant file utilities. Compile time build switches are used
 *      to direct these functions to the O/S native versions of the function.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include  <time.h>

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"     /* often used gucef macros */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_ETYPES_H
#include "gucefCORE_ETypes.h"     /* simple types used */
#define GUCEF_CORE_ETYPES_H
#endif /* GUCEF_CORE_ETYPES_H ? */

#ifndef GUCEF_CORE_CDATETIME_H
#include "gucefCORE_CDateTime.h"
#define GUCEF_CORE_CDATETIME_H
#endif /* GUCEF_CORE_CDATETIME_H ? */

#ifndef GUCEF_CORE_CRESOURCEMETADATA_H
#include "gucefCORE_CResourceMetaData.h"
#define GUCEF_CORE_CRESOURCEMETADATA_H
#endif /* GUCEF_CORE_CRESOURCEMETADATA_H ? */

#undef MoveFile
#undef CopyFile
#undef DeleteFile

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

/**
 *  Returns the modification time of the indicated resource
 */
GUCEF_CORE_PUBLIC_CPP CDateTime
GetFileModificationTime( const CString& path );

/*-------------------------------------------------------------------------*/

/**
 *  Returns the creation time of the indicated resource
 */
GUCEF_CORE_PUBLIC_CPP CDateTime
GetFileCreationTime( const CString& path );

/*-------------------------------------------------------------------------*/

/**
 *  Attempts to obtain various file meta-data from the file system
 */
GUCEF_CORE_PUBLIC_CPP bool
GetFileMetaData( const CString& filePath     ,
                 CResourceMetaData& metaData );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP bool
FileExists( const CString& filename );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP bool
DeleteFile( const CString& filename );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP bool
MoveFile( const CString& oldPath ,
          const CString& newPath ,
          const bool overwrite   );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP bool
DirExists( const CString& path );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP UInt64
FileSize( const CString& filename );

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_DVCPPFILEUTILS_H ? */
