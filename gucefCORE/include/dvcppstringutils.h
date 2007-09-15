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

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H 
#define GUCEF_CORE_DVCPPSTRINGUTILS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_ESTRUCTS_H
#include "EStructs.h"           /* generic structs */
#define GUCEF_CORE_ESTRUCTS_H
#endif /* GUCEF_CORE_ESTRUCTS_H ? */

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"          /* string class implementation */
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

/**
 *      Turns the given relative path into an absolute path
 */
GUCEFCORE_EXPORT_CPP CString
RelativePath( const CString& relpath );

/*-------------------------------------------------------------------------*/

GUCEFCORE_EXPORT_CPP void
AppendToPath( CString& path           ,
              const CString& addition );
              
/*-------------------------------------------------------------------------*/
              
GUCEFCORE_EXPORT_CPP CString
StripFilename( const CString& pathPlusFilename );

/*-------------------------------------------------------------------------*/

GUCEFCORE_EXPORT_CPP CString
StripLastSubDir( const CString& completePath );

/*-------------------------------------------------------------------------*/

GUCEFCORE_EXPORT_CPP CString
ExtractFilename( const CString& pathPlusFilename );

/*-------------------------------------------------------------------------*/

GUCEFCORE_EXPORT_CPP bool
FileExists( const CString& filename );

/*-------------------------------------------------------------------------*/

GUCEFCORE_EXPORT_CPP CString
VersionToString( const TVersion* version );

/*-------------------------------------------------------------------------*/

GUCEFCORE_EXPORT_CPP bool
StringToBool( const CString& str );

/*-------------------------------------------------------------------------*/

GUCEFCORE_EXPORT_CPP Int32
StringToInt32( const CString& str );

/*-------------------------------------------------------------------------*/

GUCEFCORE_EXPORT_CPP CString
Int32ToString( const Int32 value );

/*-------------------------------------------------------------------------*/

GUCEFCORE_EXPORT_CPP UInt32
StringToUInt32( const CString& str );

/*-------------------------------------------------------------------------*/

GUCEFCORE_EXPORT_CPP CString
UInt32ToString( const UInt32 value );

/*-------------------------------------------------------------------------*/

GUCEFCORE_EXPORT_CPP UInt16
StringToUInt16( const CString& str );

/*-------------------------------------------------------------------------*/

GUCEFCORE_EXPORT_CPP CString
UInt16ToString( const UInt16 value );

/*-------------------------------------------------------------------------*/

GUCEFCORE_EXPORT_CPP CString
PointerToString( const void* value );

/*-------------------------------------------------------------------------*/

GUCEFCORE_EXPORT_CPP CString
StringToMD5String( const CString& str );

/*-------------------------------------------------------------------------*/

GUCEFCORE_EXPORT_CPP CString
MD5ToString( const UInt8 md5Digest[ 16 ] );

/*-------------------------------------------------------------------------*/

GUCEFCORE_EXPORT_CPP CString
FloatToString( const float value );

/*-------------------------------------------------------------------------*/

GUCEFCORE_EXPORT_CPP CString
DoubleToString( const double value );

/*-------------------------------------------------------------------------*/

GUCEFCORE_EXPORT_CPP CString
LastSubDir( const CString& path );

/*-------------------------------------------------------------------------*/

GUCEFCORE_EXPORT_CPP bool
IsPathValid( const CString& path  );

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 03-03-2007 :
        - Dinand: Added StripFilename()
- 21-09-2005 :
        - Dinand: Added StringToInt()
        - Dinand: Added IntToString()
        - Dinand: Added StringToMD5String()
- 24-07-2005 :
        - Dinand: Added StringToBool()
- 26-04-2005 :
        - Dinand: Fixed RelativePath() so that strings that do not have any tags don't
          cause a problem. 
- 03-04-2005 :
        - Dinand: Initial version of this file.
          Meant to hold string class utilities that do not belong in the class
          or in the class header itself.
        - Dinand: Added RelativePath()  

-----------------------------------------------------------------------------*/
