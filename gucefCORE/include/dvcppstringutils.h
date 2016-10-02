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
 *  Turns the given relative path into an absolute path
 *  You can use this function to implement support for variables in path names
 *  Supported variables are:
 *      - $CURWORKDIR$ -> Is resolved to the current working directory
 *      - $MODULEDIR$  -> Is resolved to the directory this module is located in
 *      - ..           -> If dir segments are available before this one the path is shortened
 */
GUCEF_CORE_PUBLIC_CPP CString
RelativePath( const CString& relpath );

/*-------------------------------------------------------------------------*/

/**
 *  Determines the most compact relative path between the two paths given.
 *  the given paths can have variables in them, these will be resolved via
 *  RelativePath()
 */
GUCEF_CORE_PUBLIC_CPP CString
GetRelativePathToOtherPathRoot( const CString& fromPath ,
                                const CString& toPath   );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP void
AppendToPath( CString& path           ,
              const CString& addition );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP CString
CombinePath( const CString& path     ,
             const CString& addition );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP CString
StripFilename( const CString& pathPlusFilename );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP CString
StripLastSubDir( const CString& completePath );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP CString
ExtractFilename( const CString& pathPlusFilename );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP bool
FileExists( const CString& filename );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP UInt32
FileSize( const CString& filename );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP const TVersion
StringToVersion( const CString& version );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP CString
VersionToString( const TVersion& version );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP CString
BoolToString( const bool value );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP bool
StringToBool( const CString& str );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP UInt64
StringToUInt64( const CString& str );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP CString
UInt64ToString( const UInt64 value );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP Int64
StringToInt64( const CString& str );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP CString
Int64ToString( const Int64 value );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP Int32
StringToInt32( const CString& str );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP CString
Int32ToString( const Int32 value );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP UInt32
StringToUInt32( const CString& str );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP CString
UInt32ToString( const UInt32 value );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP UInt16
StringToUInt16( const CString& str );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP CString
UInt16ToString( const UInt16 value );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP Int16
StringToInt16( const CString& str );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP CString
Int16ToString( const Int16 value );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP Int8
StringToInt8( const CString& str );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP CString
UInt8ToString( const UInt8 value );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP UInt8
StringToUInt8( const CString& str );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP CString
Int8ToString( const Int8 value );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP CString
PointerToString( const void* value );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP void*
StringToPointer( const CString& value );

/*-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_NO_MD5_SUPPORT

GUCEF_CORE_PUBLIC_CPP CString
StringToMD5String( const CString& str );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP CString
MD5ToString( const UInt8 md5Digest[ 16 ] );

#endif /* GUCEF_CORE_NO_MD5_SUPPORT ? */

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP CString
ConvertBytesToHexString( const void* byteBuffer , 
                         UInt32 bufferSize      ,
                         bool addSpaces         ,
                         bool addHexPrefix      );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP CString
FloatToString( const float value );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP CString
DoubleToString( const double value );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP CString
LastSubDir( const CString& path );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP bool
IsPathValid( const CString& path );

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP CString
ExtractFileExtention( const CString& path );

/*-------------------------------------------------------------------------*/

/**
 *  Checks whether the file path points to a file in the exact given dir path
 */
GUCEF_CORE_PUBLIC_CPP bool
IsFileInDir( const CString& dirPath  ,
             const CString& filePath );

/*-------------------------------------------------------------------------*/

/**
 *  Attempts to load the text file into a string
 *
 *  This function can unify any end-of-line characters in the given file
 *  to the given eolString. If no eolString is given then the platform
 *  native eol format is used.
 */
GUCEF_CORE_PUBLIC_CPP bool
LoadTextFileAsString( const CString& filePath      ,
                      CString& fileContent         ,
                      const bool unifyEol = false  ,
                      const char* eolString = NULL );

/*-------------------------------------------------------------------------*/

/**
 *  Attempts to write the string as a text file
 *
 *  This function can unify any end-of-line characters in the given content
 *  string to the given eolString. If no eolString is given then the platform
 *  native eol format is used.
 */
GUCEF_CORE_PUBLIC_CPP bool
WriteStringAsTextFile( const CString& filePath      ,
                       const CString& fileContent   ,
                       const bool unifyEol = false  ,
                       const char* eolString = NULL );

/*-------------------------------------------------------------------------*/

/**
 *  Attempts recursively create all the dirs in the path if they don't exist
 */
GUCEF_CORE_PUBLIC_CPP bool
CreateDirs( const CString& path );

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

- 08-02-2008 :
        - Dinand: Added IsFileInDir()
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
