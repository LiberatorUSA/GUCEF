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

#include <string.h>
#include <stdio.h>

#ifndef GUCEF_MT_DVMTOSWRAP_H
#include "gucefMT_dvmtoswrap.h"
#define GUCEF_MT_DVMTOSWRAP_H
#endif /* GUCEF_MT_DVMTOSWRAP_H ? */

#ifndef GUCEF_CORE_DVSTRUTILS_H
#include "dvstrutils.h"         /* C string utils */
#define GUCEF_CORE_DVSTRUTILS_H
#endif /* GUCEF_CORE_DVSTRUTILS_H ? */

#ifndef GUCEF_CORE_DVCPPOSWRAP_H
#include "DVCPPOSWRAP.h"
#define GUCEF_CORE_DVCPPOSWRAP_H
#endif /* GUCEF_CORE_DVCPPOSWRAP_H ? */

#include "dvcppstringutils.h"   /* function prototypes of the functions implemented here */

#ifndef GUCEF_CORE_NO_MD5_SUPPORT
  #ifndef GUCEF_CORE_DVMD5UTILS_H
  #include "dvmd5utils.h"
  #define GUCEF_CORE_DVMD5UTILS_H
  #endif /* GUCEF_CORE_DVMD5UTILS_H ? */
#endif /* GUCEF_CORE_NO_MD5_SUPPORT ? */

#if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>		/* WIN32 API */
  #undef min
  #undef max
  #define MAX_DIR_LENGTH MAX_PATH
  #define GUCEF_DIRSEPCHAROPPOSITE '/'
  #define GUCEF_DIRSEPCHAR '\\'
#elif ( ( GUCEF_PLATFORM == GUCEF_PLATFORM_LINUX ) || ( GUCEF_PLATFORM == GUCEF_PLATFORM_ANDROID ) )
  #include <unistd.h>             /* POSIX utilities */
  #include <limits.h>             /* Linux OS limits */
  #define MAX_DIR_LENGTH PATH_MAX
  #define GUCEF_DIRSEPCHAROPPOSITE '\\'
  #define GUCEF_DIRSEPCHAR '/'
#else
  #error Unsupported OS
#endif

#ifndef GUCEF_CORE_DVFILEUTILS_H
#include "dvfileutils.h"        /* all kinds of file utils */
#define GUCEF_CORE_DVFILEUTILS_H
#endif /* GUCEF_CORE_DVFILEUTILS_H ? */

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */

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

CString
TempDir( void )
{GUCEF_TRACE;

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )
    char buffer[ MAX_PATH+1 ];
    if ( 0 != ::GetTempPathA( MAX_PATH+1, buffer ) )
        return buffer;
    #endif
    return CString::Empty;
}

/*-------------------------------------------------------------------------*/

CString
ModuleDir( void )
{GUCEF_TRACE;

    char buffer[ MAX_DIR_LENGTH ];
    Module_Path( buffer, MAX_DIR_LENGTH );
    Strip_Filename( buffer );
    return buffer;
}

/*-------------------------------------------------------------------------*/

CString
CurrentWorkingDir( void )
{GUCEF_TRACE;

    char buffer[ MAX_DIR_LENGTH ];
    Get_Current_Dir( buffer, MAX_DIR_LENGTH );
    return buffer;
}

/*-------------------------------------------------------------------------*/

static inline void
DivisionRemainder( Int32 dividend   ,
                   Int32 divisor    ,
                   Int32& remainder ,
                   Int32& quotient  )
{GUCEF_TRACE;

    //remainder = dividend % divisor;
    //quotient = (dividend / divisor);

    quotient = dividend / divisor;
    remainder = dividend - divisor * quotient;
}

/*-------------------------------------------------------------------------*/

CString
ConvertBytesToHexString( const void* byteBuffer ,
                         UInt32 bufferSize      ,
                         bool addSpaces         ,
                         bool addHexPrefix      )
{GUCEF_TRACE;

    static const char* g_hexDigits = "0123456789ABCDEF";
    const char* charByteBuffer = (const char*) byteBuffer;

    // Each byte is expressed as a 2 char hex value
    UInt32 hexStrSize = bufferSize * 2 + 1;
    if ( addSpaces ) hexStrSize += bufferSize;
    if ( addHexPrefix ) hexStrSize += 2*bufferSize;

    CString hexString;
    char* digits = hexString.Reserve( hexStrSize );

    Int32 digitIndex1 = 0;
    Int32 digitIndex2 = 0;
    UInt32 digitOffset = 0;

    for ( UInt32 i=0; i<bufferSize; ++i )
    {
        DivisionRemainder( charByteBuffer[ i ] ,
                           16                  ,
                           digitIndex1         ,
                           digitIndex2         );

        if ( digitIndex1 < 0 ) digitIndex1 *= -1;
        if ( digitIndex2 < 0 ) digitIndex2 *= -1;

        if ( addSpaces )
        {
            if ( addHexPrefix )
            {
                digitOffset = 5 * i;
                digits[ digitOffset ] = '0';
                digits[ digitOffset + 1 ] = 'x';
                digits[ digitOffset + 2 ] = g_hexDigits[ digitIndex2 ];
                digits[ digitOffset + 3 ] = g_hexDigits[ digitIndex1 ];
                digits[ digitOffset + 4 ] = ' ';
            }
            else
            {
                digitOffset = 3 * i;
                digits[ digitOffset ] = g_hexDigits[ digitIndex2 ];
                digits[ digitOffset + 1 ] = g_hexDigits[ digitIndex1 ];
                digits[ digitOffset + 2 ] = ' ';
            }
        }
        else
        {
            if ( addHexPrefix )
            {
                digitOffset = 4 * i;
                digits[ digitOffset ] = '0';
                digits[ digitOffset + 1 ] = 'x';
                digits[ digitOffset + 2 ] = g_hexDigits[ digitIndex2 ];
                digits[ digitOffset + 3 ] = g_hexDigits[ digitIndex1 ];
            }
            else
            {
                digitOffset = 2 * i;
                digits[ digitOffset ] = g_hexDigits[ digitIndex2 ];
                digits[ digitOffset + 1 ] = g_hexDigits[ digitIndex1 ];
            }
        }
    }
    if ( hexString.Length() >= 1 )
    {
        digits[ hexString.Length()-1 ] = '\0';
    }
    return hexString;
}

/*-------------------------------------------------------------------------*/

CString
ResolveVars( const CString& strWithVars )
{GUCEF_TRACE;

    // First resolve any environment variables which in turn could contain other variables
    CString resultStr = strWithVars;
    Int32 idx = 0;
    do
    {
        idx = resultStr.HasSubstr( "$ENVVAR:", idx, true );
        if ( idx > -1 )
        {
            Int32 endIndex = resultStr.HasChar( '$', idx+8, true );
            
            // Break out if there is no closing char, this is invalid
            if ( endIndex < 0 ) break;

            CString varName = resultStr.SubstrFromRange( idx+8, endIndex );
            CString varValue = ::GUCEF::CORE::GetEnv( varName );
            resultStr = resultStr.ReplaceSubStr( idx, endIndex-idx, varValue );
        }
    }
    while ( idx > -1 );

    resultStr = resultStr.ReplaceSubstr( "$HOSTNAME$", GetHostname() );
    resultStr = resultStr.ReplaceSubstr( "$PID$", UInt32ToString( MT::GetProcessID() ) );    
    resultStr = resultStr.ReplaceSubstr( "$TEMPDIR$", TempDir() );
    resultStr = resultStr.ReplaceSubstr( "$MODULEDIR$", ModuleDir() );
    resultStr = resultStr.ReplaceSubstr( "$CURWORKDIR$", CurrentWorkingDir() );
    
    return resultStr;
}

/*-------------------------------------------------------------------------*/

CString
RelativePath( const CString& relpath , 
              bool resolveVars       )
{GUCEF_TRACE;

    CString resultStr;

    if ( resolveVars )
    {
        // First resolve any environment variables which in turn could contain other variables
        resultStr = ResolveVars( relpath );
    }
    else
    {
        resultStr = relpath;
    }

    // Now the up dir segments,...
    // First make sure we use the same seperator char in both string across
    // the entire strings uniformly. This simplyfies the algoritm
    resultStr = resultStr.ReplaceChar( GUCEF_DIRSEPCHAROPPOSITE, GUCEF_DIRSEPCHAR );

    // Now make sure there are no multiple repeating seperators
    // Not that unlikely if the string was human generated
    resultStr = resultStr.CompactRepeatingChar( GUCEF_DIRSEPCHAR );

    // Now keep chopping away up dir segments until we run out of segments
    // that meet our criterea.
    CString upDirSeg = CString( ".." ) + GUCEF_DIRSEPCHAR;
    Int32 upDirIdx = resultStr.HasSubstr( upDirSeg, false );
    while ( upDirIdx > 0 )
    {
        // Divide the path into 2 segments at the location of the updir segment
        CString prefix( resultStr.C_String(), upDirIdx );

        // get the dir segment before the updir segment and check if it is
        // another updir segment
        CString lastSubdir = LastSubDir( prefix );
        if ( lastSubdir != upDirSeg )
        {
            // The segment before the updir segment is a regular dir
            // This means we can strip that dir and thus resolve this updir segment
            CString newPath = StripLastSubDir( prefix );
            AppendToPath( newPath, resultStr.C_String()+upDirIdx+3 );
            resultStr = newPath;
            upDirIdx = resultStr.HasSubstr( upDirSeg, false );
        }
        else
        {
            // Skip this updir segment and go to the next one
            upDirIdx = resultStr.HasSubstr( upDirSeg, upDirIdx, false );
        }
    }

    return resultStr;
}

/*---------------------------------------------------------------------------*/

CString
GetRelativePathToOtherPathRoot( const CString& fromPath ,
                                const CString& toPath   ,
                                bool resolveVars        )
{GUCEF_TRACE;

    typedef std::vector< CString >  TStringVector;

    // First resolve any variables in the paths,.. normalize
    CString absFromPath = RelativePath( fromPath, resolveVars );
    CString absToPath = RelativePath( toPath, resolveVars );

    #if GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN

    // For MS Win we need to take into account that paths are
    // the same even if the case doesn't match. Paths are
    // case-insensitive. We will make sure this doesnt affect
    // us by checking the equality in a case-insensitive manner.
    // This principle also applies to drive letters.

    // Now just check for the overlap in paths and based on the overlap
    // we will make a relative path
    Int32 pathEquality = (Int32) absFromPath.FindMaxSubstrEquality( absToPath, 0, true, false );

    #else

    // Now just check for the overlap in paths and based on the overlap
    // we will make a relative path
    Int32 pathEquality = (Int32) absFromPath.FindMaxSubstrEquality( absToPath, 0, true, true );

    #endif

    CString toPathRemainder = absToPath.ReplaceChar( '\\', '/' );
    CString fromPathRemainder = absFromPath.ReplaceChar( '\\', '/' );
    pathEquality = toPathRemainder.HasChar( '/', pathEquality, false );
    if ( pathEquality >= 0 )
    {
        toPathRemainder = toPathRemainder.CutChars( pathEquality+1, true );
        fromPathRemainder = fromPathRemainder.CutChars( pathEquality+1, true );
        TStringVector upDirElements = fromPathRemainder.ParseElements( '/', false );

        CString relativePath;
        for ( UInt32 i=0; i<upDirElements.size(); ++i )
        {
            relativePath += "../";
        }
        AppendToPath( relativePath, toPathRemainder );
        return RelativePath( relativePath, resolveVars );
    }
    return toPathRemainder;
}

/*-------------------------------------------------------------------------*/

void
AppendToPath( CString& path           ,
              const CString& addition )
{GUCEF_TRACE;

    if ( addition.Length() == 0 ) return;

    if ( path.Length() == 0 )
    {
        path = addition;
        return;
    }

    Int32 pathLastChar = path[ path.Length()-1 ];
    if ( pathLastChar == '/' || pathLastChar == '\\' )
    {
        if ( addition[ 0 ] == '/' || addition[ 0 ] == '\\' )
        {
            path += (addition.C_String()+1);
            return;
        }
        path += addition;
        return;
    }
    else
    {
        if ( addition[ 0 ] == '/' || addition[ 0 ] == '\\' )
        {
            path += addition;
            return;
        }
        path += GUCEF_DIRSEPCHAR + addition;
    }
}

/*-------------------------------------------------------------------------*/

CString
CombinePath( const CString& path     ,
             const CString& addition )
{
    CString newPath( path );
    AppendToPath( newPath, addition );
    return newPath;
}

/*-------------------------------------------------------------------------*/

const TVersion
StringToVersion( const CString& versionStr )
{GUCEF_TRACE;

    TVersion version = { 0, 0, 0, 0 };
    std::vector< CString > elements = versionStr.ParseElements( '.' );
    if ( elements.size() > 0 )
    {
        version.major = StringToUInt16( elements[ 0 ] );
        if ( elements.size() > 1 )
        {
            version.minor = StringToUInt16( elements[ 1 ] );
            if ( elements.size() > 2 )
            {
                version.patch = StringToUInt16( elements[ 2 ] );
                if ( elements.size() > 3 )
                {
                    version.release = StringToUInt16( elements[ 3 ] );
                }
            }
        }
    }
    return version;
}

/*-------------------------------------------------------------------------*/

CString
VersionToString( const TVersion& version )
{GUCEF_TRACE;

    char verstr[ 25 ];
    snprintf( verstr, 25, "%d.%d.%d.%d", version.major, version.minor, version.patch, version.release );
    return verstr;
}

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP CString
BoolToString( const bool value )
{GUCEF_TRACE;

    return Boolint_To_String( value ? 1 : 0 );
}

/*-------------------------------------------------------------------------*/

bool
StringToBool( const CString& str, bool defaultIfNeeded )
{GUCEF_TRACE;

    UInt32 boolint = String_To_Boolint_WithDefault( str.C_String(), defaultIfNeeded ? 1 : 0 );
    return boolint > 0;
}

/*-------------------------------------------------------------------------*/

Int64
StringToInt64( const CString& str, Int64 defaultIfNeeded )
{GUCEF_TRACE;

    if ( !str.IsNULLOrEmpty() )
    {
        Int64 value;
        #ifdef GUCEF_MSWIN_BUILD
        sscanf( str.C_String(), "%I64d", &value );
        #else
        sscanf( str.C_String(), "%d", &value );
        #endif

        return value;
    }
    return defaultIfNeeded;
}

/*-------------------------------------------------------------------------*/

CString
Int64ToString( const Int64 value )
{GUCEF_TRACE;

    char intBuffer[ 21 ];
    #ifdef GUCEF_MSWIN_BUILD
    snprintf( intBuffer, 21, "%I64d", value );
    #else
    snprintf( intBuffer, 21, "%lld", value );
    #endif

    return CString( intBuffer );
}

/*-------------------------------------------------------------------------*/

UInt64
StringToUInt64( const CString& str, UInt64 defaultIfNeeded )
{GUCEF_TRACE;

    if ( !str.IsNULLOrEmpty() )
    {
        UInt64 value;
        #ifdef GUCEF_MSWIN_BUILD
        sscanf( str.C_String(), "%I64u", &value );
        #else
        sscanf( str.C_String(), "%u", &value );
        #endif

        return value;
    }
    return defaultIfNeeded;
}

/*-------------------------------------------------------------------------*/

CString
UInt64ToString( const UInt64 value )
{GUCEF_TRACE;

    char intBuffer[ 21 ];
    #ifdef GUCEF_MSWIN_BUILD
    snprintf( intBuffer, 21, "%I64u", value );
    #else
    snprintf( intBuffer, 21, "%llu", value );
    #endif

    return CString( intBuffer );
}

/*-------------------------------------------------------------------------*/

Int32
StringToInt32( const CString& str, Int32 defaultIfNeeded )
{GUCEF_TRACE;

    if ( !str.IsNULLOrEmpty() )
    {
        Int32 value;
        sscanf( str.C_String(), "%d", &value );
        return value;
    }
    return defaultIfNeeded;
}

/*-------------------------------------------------------------------------*/

CString
Int32ToString( const Int32 value )
{GUCEF_TRACE;

    char intBuffer[ 12 ];
    snprintf( intBuffer, 12, "%d", value );
    return CString( intBuffer );
}

/*-------------------------------------------------------------------------*/

UInt32
StringToUInt32( const CString& str, UInt32 defaultIfNeeded )
{GUCEF_TRACE;

    if ( !str.IsNULLOrEmpty() )
    {
        UInt32 value;
        sscanf( str.C_String(), "%u", &value );
        return value;
    }
    return defaultIfNeeded;
}

/*-------------------------------------------------------------------------*/

CString
UInt32ToString( const UInt32 value )
{GUCEF_TRACE;

    char intBuffer[ 12 ];
    snprintf( intBuffer, 12, "%u", value );
    return CString( intBuffer );
}

/*-------------------------------------------------------------------------*/

UInt16
StringToUInt16( const CString& str, UInt16 defaultIfNeeded )
{GUCEF_TRACE;

    if ( !str.IsNULLOrEmpty() )
    {
        UInt32 value;
        sscanf( str.C_String(), "%u", &value );
        return (UInt16) value;
    }
    return defaultIfNeeded;
}

/*-------------------------------------------------------------------------*/

CString
UInt16ToString( const UInt16 value )
{GUCEF_TRACE;

    char intBuffer[ 12 ];
    snprintf( intBuffer, 12, "%u", (UInt32) value );
    return CString( intBuffer );
}

/*-------------------------------------------------------------------------*/

Int16
StringToInt16( const CString& str, Int16 defaultIfNeeded )
{GUCEF_TRACE;

    if ( !str.IsNULLOrEmpty() )
    {
        Int32 value;
        sscanf( str.C_String(), "%d", &value );
        return (Int16) value;
    }
    return defaultIfNeeded;
}

/*-------------------------------------------------------------------------*/

CString
Int16ToString( const Int16 value )
{GUCEF_TRACE;

    char intBuffer[ 12 ];
    snprintf( intBuffer, 12, "%d", (Int32) value );
    return CString( intBuffer );
}

/*-------------------------------------------------------------------------*/

Int8
StringToInt8( const CString& str, Int8 defaultIfNeeded )
{GUCEF_TRACE;

    if ( !str.IsNULLOrEmpty() )
    {
        Int32 value;
        sscanf( str.C_String(), "%d", &value );
        return (Int8) value;
    }
    return defaultIfNeeded;
}

/*-------------------------------------------------------------------------*/

CString
Int8ToString( const Int8 value )
{GUCEF_TRACE;

    char intBuffer[ 12 ];
    snprintf( intBuffer, 12, "%d", (Int32) value );
    return CString( intBuffer );
}

/*-------------------------------------------------------------------------*/

UInt8
StringToUInt8( const CString& str, UInt8 defaultIfNeeded )
{GUCEF_TRACE;

    if ( !str.IsNULLOrEmpty() )
    {
        UInt32 value;
        sscanf( str.C_String(), "%u", &value );
        return (UInt8) value;
    }
    return defaultIfNeeded;
}

/*-------------------------------------------------------------------------*/

CString
UInt8ToString( const UInt8 value )
{GUCEF_TRACE;

    char intBuffer[ 12 ];
    snprintf( intBuffer, 12, "%u", (UInt32) value );
    return CString( intBuffer );
}

/*-------------------------------------------------------------------------*/

CString
PointerToString( const void* value )
{GUCEF_TRACE;

    char addrBuffer[ 64 ];
    snprintf( addrBuffer, 64, "%p", value );
    return CString( addrBuffer );
}

/*-------------------------------------------------------------------------*/

void*
StringToPointer( const CString& str, void* defaultIfNeeded )
{GUCEF_TRACE;

    if ( !str.IsNULLOrEmpty() )
    {
        void* ptr = GUCEF_NULL;
        sscanf( str.C_String(), "%p", &ptr );
        return ptr;
    }
    return defaultIfNeeded;
}

/*-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_NO_MD5_SUPPORT

CString
StringToMD5String( const CString& str )
{GUCEF_TRACE;

    UInt8 md5digest[ 16 ];

    md5fromstring( str.C_String() ,
                   md5digest      );

    return MD5ToString( md5digest );
}

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP CString
MD5ToString( const UInt8 md5Digest[ 16 ] )
{GUCEF_TRACE;

    char md5string[ 48 ];

    md5tostring( md5Digest ,
                 md5string );

    CString md5str;
    md5str.Set( md5string, 48 );

    return md5str;
}

#endif /* GUCEF_CORE_NO_MD5_SUPPORT ? */

/*-------------------------------------------------------------------------*/

CString
FloatToString( const float value )
{GUCEF_TRACE;

    char floatChars[ 16 ];
    snprintf( floatChars, 16, "%f", value );

    return floatChars;
}

/*-------------------------------------------------------------------------*/

float
StringToFloat( const CString& str, float defaultIfNeeded )
{GUCEF_TRACE;

    if ( !str.IsNULLOrEmpty() )
    {
        float value;
        sscanf( str.C_String(), "%f", &value );
        return value;
    }
    return defaultIfNeeded;    
}

/*-------------------------------------------------------------------------*/

CString
DoubleToString( const double value )
{GUCEF_TRACE;

    char doubleChars[ 128 ];
    snprintf( doubleChars, 128, "%lf", value );

    return doubleChars;
}

/*-------------------------------------------------------------------------*/

double
StringToDouble( const CString& str, double defaultIfNeeded )
{GUCEF_TRACE;

    if ( !str.IsNULLOrEmpty() )
    {
        double value;
        sscanf( str.C_String(), "%lf", &value );
        return value;
    }
    return defaultIfNeeded;
}

/*-------------------------------------------------------------------------*/

CString::StringVector
StringToStringVector( const CString& str                           , 
                      const CString::StringVector& defaultIfNeeded ,
                      char seperator                               )
{GUCEF_TRACE;

    if ( !str.IsNULLOrEmpty() )
    {
        return str.ParseElements( seperator, true );
    }
    return defaultIfNeeded;
}

/*-------------------------------------------------------------------------*/

CString
LastSubDir( const CString& path )
{GUCEF_TRACE;

    if ( !path.IsNULLOrEmpty() )
    {
        CString lastSubDir;
        UInt32 offset = Last_Subdir( path.C_String() );
        if ( offset > 0 )
        {
            lastSubDir.Set( path.C_String()+offset, path.Length() - offset );
        }
        else
        {
            lastSubDir = path;
        }
        return lastSubDir;
    }
    return path;
}

/*-------------------------------------------------------------------------*/

CString
StripLastSubDir( const CString& completePath )
{GUCEF_TRACE;

    if ( 0 != completePath.Length() )
    {
        CString path = completePath.ReplaceChar( GUCEF_DIRSEPCHAROPPOSITE, GUCEF_DIRSEPCHAR );
        if ( path[ path.Length()-1 ] == GUCEF_DIRSEPCHAR )
        {
            path = path.CutChars( 1, false );
        }

        CString remnamt = path.SubstrToChar( GUCEF_DIRSEPCHAR, false );
        if ( remnamt.Length() > 0 )
        {
            return path.CutChars( remnamt.Length()+1, false );
        }
        return completePath;
    }
    return CString();
}

/*-------------------------------------------------------------------------*/

CString
StripFilename( const CString& pathPlusFilename )
{GUCEF_TRACE;

    char* buffer = new char[ pathPlusFilename.Length()+1 ];
    UInt32 length = _Strip_Filename( buffer, pathPlusFilename.C_String() );
    CString resultStr( buffer, length );
    delete []buffer;
    return resultStr;
}

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP CString
ExtractFilename( const CString& pathPlusFilename )
{GUCEF_TRACE;

    return Extract_Filename( pathPlusFilename.C_String() );
}

/*-------------------------------------------------------------------------*/

bool
FileExists( const CString& filename )
{GUCEF_TRACE;

    return 0 != File_Exists( filename.C_String() );
} 

/*-------------------------------------------------------------------------*/

bool
DirExists( const CString& path )
{GUCEF_TRACE;

    return 0 != Dir_Exists( path.C_String() );
}

/*-------------------------------------------------------------------------*/

UInt32
FileSize( const CString& filename )
{GUCEF_TRACE;

    return Filesize( filename.C_String() );
}

/*-------------------------------------------------------------------------*/

bool
IsPathValid( const CString& path  )
{GUCEF_TRACE;

    return 0 != Is_Path_Valid( path.C_String() );
}

/*-------------------------------------------------------------------------*/

CString
ExtractFileExtention( const CString& path )
{GUCEF_TRACE;

    return Extract_File_Ext( path.C_String() );
}

/*-------------------------------------------------------------------------*/

bool
IsFileInDir( const CString& dirPath  ,
             const CString& filePath )
{GUCEF_TRACE;

    // must contain the dir path as a starting string
    if ( 0 == filePath.HasSubstr( dirPath, true ) )
    {
        // now we check for deeper nesting
        CString pathRemainder = filePath.CutChars( dirPath.Length(), true );
        CString filename = ExtractFilename( pathRemainder );
        UInt32 remnantSize = pathRemainder.Length() - filename.Length();
        if ( remnantSize == 1 )
        {
            // if the remainder is just a prefixed dir seperator then we don't count
            // the file as being in the dir, otherwise it isnt
            char testChar = *(pathRemainder.C_String());
            return testChar == '\\' || testChar == '/';
        }
        else
        if ( remnantSize == 0 )
        {
            return true;
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PRIVATE_CPP void
UnifyStringEol( const char* eolString ,
                CString& textStr      )
{GUCEF_TRACE;

    // If no desired eol format is given then use the platform
    // native format
    const char* eolStrToUse = eolString;
    if ( NULL == eolStrToUse || *eolStrToUse == '\0' )
    {
        eolStrToUse = GUCEF_EOL;
    }

    // Turn everything into "\n" in case of a mixed EOL char string
    textStr = textStr.ReplaceSubstr( "\r\n", "\n" ).ReplaceSubstr( "\r", "\n" );
    // Now we unified the EOL segments into \n.

    if ( 0 != strcmp( eolStrToUse, "\n" ) )
    {
        // Convert into whatever is given as the desired format
        textStr = textStr.ReplaceSubstr( "\n", eolStrToUse );
    }
}

/*-------------------------------------------------------------------------*/

bool
WriteStringAsTextFile( const CString& filePath    ,
                       const CString& fileContent ,
                       const bool unifyEol        ,
                       const char* eolString      )
{GUCEF_TRACE;

    if ( filePath.IsNULLOrEmpty() ) return false;

    FILE* fptr = fopen( filePath.C_String(), "wb" );
    if ( NULL != fptr )
    {
        if ( unifyEol )
        {
            CString unifiedContent = fileContent;
            UnifyStringEol( eolString, unifiedContent );
            fwrite( unifiedContent.C_String(), unifiedContent.Length(), 1, fptr );
        }
        else
        {
            fwrite( fileContent.C_String(), fileContent.Length(), 1, fptr );
        }
        fclose( fptr );
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
LoadTextFileAsString( const CString& filePath ,
                      CString& fileContent    ,
                      const bool unifyEol     ,
                      const char* eolString   )
{GUCEF_TRACE;

    fileContent.Clear();

    FILE* fptr = fopen( filePath.C_String(), "rb" );
    if ( NULL != fptr )
    {
        char charBuffer[ 1024 ];

        size_t bytesRead = 0;
        do
        {
            // read a block
            bytesRead = fread( charBuffer, 1, 1024, fptr );

            // Look for a null terminator
            size_t delimter = bytesRead;
            for ( size_t i=0; i<bytesRead; ++i )
            {
                if ( 0 == charBuffer[ i ] )
                {
                    delimter = (int) i;
                }
            }

            if ( delimter+1 < bytesRead )
            {
                // This is not a text file
                // The only valid location for a zero terminator (if any) 
                // is at the end of the file
                fclose( fptr );
                return false;
            }

            // Append what we can
            fileContent.Append( charBuffer, (UInt32)delimter );
        }
        while ( bytesRead == 1024 );

        fclose( fptr );
        
        if ( unifyEol )
        {
            UnifyStringEol( eolString, fileContent );
        }
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CreateDirs( const CString& path )
{GUCEF_TRACE;

    CString actualPath = RelativePath( path );
    return 0 != Create_Directory( actualPath.C_String() );
}

/*-------------------------------------------------------------------------*/

bool
DeleteFile( const CString& path )
{GUCEF_TRACE;

    CString actualPath = RelativePath( path );
    return 0 != Delete_File( actualPath.C_String() );
}

/*-------------------------------------------------------------------------*/

bool
IsAbsolutePath( const CString& path )
{GUCEF_TRACE;
    
    // Check for MS Windows drive letter segment or MS Windows UNC path
    if ( 1 == path.HasSubstr( ":\\", true ) || 0 == path.HasSubstr( "\\", true ) )
        return true;

    // Check for Linux Root designator
    if ( !path.IsNULLOrEmpty() && path[ 0 ] == '/' )
        return true;

    // Check for 
    if ( 1 == path.HasSubstr( ":\\", true ) )
        return true;

    return false;
}

/*-------------------------------------------------------------------------*/

bool 
Utf16toUtf8( const std::wstring& wstr ,
             std::string& str         )
{GUCEF_TRACE;

    if ( wstr.empty() )
    {
        str = std::string();
        return true;
    }

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    size_t charsNeeded = ::WideCharToMultiByte( CP_ACP, 0, wstr.c_str(), (int)wstr.size()+1, 0, 0, 0, 0 );
    if ( charsNeeded == 0 )
        return false; // Failed converting UTF-16 string to UTF-8

    str.resize( charsNeeded, '\0' );
    int charsConverted = ::WideCharToMultiByte( CP_ACP, 0, wstr.c_str(), (int)wstr.size()+1, (LPSTR)str.c_str(), (int)charsNeeded, 0, 0 );
    if ( charsConverted == 0 )
        return false; // Failed converting UTF-16 string to UTF-8

    return true;

    #else

    return false;

    #endif
}

/*-------------------------------------------------------------------------*/

bool 
Utf8toUtf16( const std::string& str ,
             std::wstring& wstr     )
{GUCEF_TRACE;

    if ( str.empty() )
    {
        wstr = std::wstring();
        return true;
    }

    #if ( GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN )

    size_t charsNeeded = ::MultiByteToWideChar( CP_UTF8, 0, str.data(), (int)str.size(), NULL, 0 );
    if ( charsNeeded == 0 )
        return false; // Failed converting UTF-8 string to UTF-16

    wstr.resize( charsNeeded );
    int charsConverted = ::MultiByteToWideChar( CP_UTF8, 0, str.data(), (int)str.size(), (LPWSTR)wstr.c_str(), (int)charsNeeded );
    if ( charsConverted == 0 )
        return false; // Failed converting UTF-8 string to UTF-16

    return true;

    #else

    return false;

    #endif
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
