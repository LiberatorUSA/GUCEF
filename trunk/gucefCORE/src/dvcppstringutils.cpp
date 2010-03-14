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

#ifndef GUCEF_CORE_DVSTRUTILS_H
#include "dvstrutils.h"         /* C string utils */
#define GUCEF_CORE_DVSTRUTILS_H
#endif /* GUCEF_CORE_DVSTRUTILS_H ? */ 

#ifndef GUCEF_CORE_DVMD5UTILS_H
#include "dvmd5utils.h"
#define GUCEF_CORE_DVMD5UTILS_H
#endif /* GUCEF_CORE_DVMD5UTILS_H ? */

#include "dvcppstringutils.h"   /* function prototypes of the functions implemented here */

#ifdef GUCEF_MSWIN_BUILD
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>		/* WIN32 API */
  #undef min
  #undef max
  #define MAX_DIR_LENGTH MAX_PATH
  #define DIRSEPCHAROPPOSITE '/'
  #define DIRSEPCHAR '\\'
#elif GUCEF_LINUX_BUILD
  #include <unistd.h>             /* POSIX utilities */
  #include <limits.h>             /* Linux OS limits */
  #define MAX_DIR_LENGTH PATH_MAX
  #define DIRSEPCHAROPPOSITE '\\'
  #define DIRSEPCHAR '/'  
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
ModuleDir( void )
{
    char buffer[ MAX_DIR_LENGTH ];
    Module_Path( buffer, MAX_DIR_LENGTH );
    Strip_Filename( buffer );
    return buffer;   
}

/*-------------------------------------------------------------------------*/

CString
CurrentWorkingDir( void )
{
    char buffer[ MAX_DIR_LENGTH ];
    Get_Current_Dir( buffer, MAX_DIR_LENGTH );
    return buffer;   
}

/*-------------------------------------------------------------------------*/

CString
RelativePath( const CString& relpath )
{GUCEF_TRACE;        
        
    CString moduleDir = ModuleDir();
    CString resultStr = relpath.ReplaceSubstr( "$MODULEDIR$", moduleDir );

    CString currentWorkingDir = CurrentWorkingDir();
    resultStr = resultStr.ReplaceSubstr( "$CURWORKDIR$", moduleDir );
     
    resultStr = resultStr.ReplaceChar( DIRSEPCHAROPPOSITE, DIRSEPCHAR );
    resultStr = resultStr.CompactRepeatingChar( DIRSEPCHAR ); 
    
    CString upDirSeg = CString( ".." ) + DIRSEPCHAR;
    Int32 upDirIdx = resultStr.HasSubstr( upDirSeg, true );
    while ( upDirIdx > 0 )
    {
        CString prefix( resultStr.C_String(), upDirIdx );
        UInt32 lengthDelta = prefix.Length();
        prefix = StripLastSubDir( prefix );
        lengthDelta = lengthDelta - prefix.Length();
        resultStr = prefix + (resultStr.C_String()+upDirIdx+lengthDelta);
        upDirIdx = resultStr.HasSubstr( upDirSeg, true );
    }

    return resultStr;
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
    
    char pathLastChar = path[ path.Length()-1 ];
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
        path += DIRSEPCHAR + addition;
    }                               
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
    sprintf( verstr, "%d.%d.%d.%d", version.major, version.minor, version.patch, version.release );
    return verstr;        
}

/*-------------------------------------------------------------------------*/

GUCEFCORE_EXPORT_CPP CString
BoolToString( const bool value )
{GUCEF_TRACE;
    
    return Boolint_To_String( value ? 1 : 0 );
}

/*-------------------------------------------------------------------------*/

bool
StringToBool( const CString& str )
{GUCEF_TRACE;
        UInt32 boolint = String_To_Boolint( str.C_String() );
        return boolint > 0;
}

/*-------------------------------------------------------------------------*/

Int64
StringToInt64( const CString& str )
{GUCEF_TRACE;

    Int64 value;
    #ifdef GUCEF_MSWIN_BUILD
    sscanf( str.C_String(), "%I64d", &value );
    #else
    sscanf( str.C_String(), "%d", &value );
    #endif
    
    return value;        
}

/*-------------------------------------------------------------------------*/

CString
Int64ToString( const Int64 value )
{GUCEF_TRACE;

    char intBuffer[ 21 ];
    #ifdef GUCEF_MSWIN_BUILD
    sprintf( intBuffer, "%I64d", value );
    #else
    sprintf( intBuffer, "%d", value );
    #endif
    
    return CString( intBuffer );
}

/*-------------------------------------------------------------------------*/

UInt64
StringToUInt64( const CString& str )
{GUCEF_TRACE;

    UInt64 value;
    #ifdef GUCEF_MSWIN_BUILD
    sscanf( str.C_String(), "%I64u", &value );
    #else
    sscanf( str.C_String(), "%u", &value );
    #endif
    
    return value;        
}

/*-------------------------------------------------------------------------*/

CString
UInt64ToString( const UInt64 value )
{GUCEF_TRACE;

    char intBuffer[ 21 ];
    #ifdef GUCEF_MSWIN_BUILD
    sprintf( intBuffer, "%I64u", value );
    #else
    sprintf( intBuffer, "%u", value );
    #endif
    
    return CString( intBuffer );
}

/*-------------------------------------------------------------------------*/

Int32
StringToInt32( const CString& str )
{GUCEF_TRACE;

    Int32 value;
    sscanf( str.C_String(), "%d", &value );
    return value;        
}

/*-------------------------------------------------------------------------*/

CString
Int32ToString( const Int32 value )
{GUCEF_TRACE;

    char intBuffer[ 12 ];
    sprintf( intBuffer, "%d", value );
    return CString( intBuffer );
}

/*-------------------------------------------------------------------------*/

UInt32
StringToUInt32( const CString& str )
{GUCEF_TRACE;

    UInt32 value;
    sscanf( str.C_String(), "%u", &value );
    return value;
}

/*-------------------------------------------------------------------------*/

CString
UInt32ToString( const UInt32 value )
{GUCEF_TRACE;

    char intBuffer[ 12 ];
    sprintf( intBuffer, "%u", value );
    return CString( intBuffer );
}

/*-------------------------------------------------------------------------*/

UInt16
StringToUInt16( const CString& str )
{GUCEF_TRACE;

    UInt32 value;
    sscanf( str.C_String(), "%u", &value );
    return (UInt16) value;
}

/*-------------------------------------------------------------------------*/

CString
UInt16ToString( const UInt16 value )
{GUCEF_TRACE;

    char intBuffer[ 12 ];
    sprintf( intBuffer, "%u", value );
    return CString( intBuffer );
}

/*-------------------------------------------------------------------------*/

Int8
StringToInt8( const CString& str )
{GUCEF_TRACE;

    Int32 value;
    sscanf( str.C_String(), "%d", &value );
    return (Int8) value;
}

/*-------------------------------------------------------------------------*/

CString
Int8ToString( const Int8 value )
{GUCEF_TRACE;

    char intBuffer[ 12 ];
    sprintf( intBuffer, "%d", value );
    return CString( intBuffer );
}

/*-------------------------------------------------------------------------*/

UInt8
StringToUInt8( const CString& str )
{GUCEF_TRACE;

    UInt32 value;
    sscanf( str.C_String(), "%u", &value );
    return (UInt8) value;
}

/*-------------------------------------------------------------------------*/

CString
UInt8ToString( const UInt8 value )
{GUCEF_TRACE;

    char intBuffer[ 12 ];
    sprintf( intBuffer, "%u", value );
    return CString( intBuffer );
}

/*-------------------------------------------------------------------------*/

CString
PointerToString( const void* value )
{GUCEF_TRACE;

    char addrBuffer[ 12 ];
    sprintf( addrBuffer, "%p", value );
    return CString( addrBuffer );    
}

/*-------------------------------------------------------------------------*/

CString
StringToMD5String( const CString& str )
{GUCEF_TRACE;
        UInt8 md5digest[ 16 ];        
        
        md5fromstring( str.C_String() ,
                       md5digest      );
        
        return MD5ToString( md5digest );                               
}

/*-------------------------------------------------------------------------*/

GUCEFCORE_EXPORT_CPP CString
MD5ToString( const UInt8 md5Digest[ 16 ] )
{
        char md5string[ 48 ];
        
        md5tostring( md5Digest ,
                     md5string );
                     
        CString md5str;
        md5str.Set( md5string, 48 );
        
        return md5str;
}

/*-------------------------------------------------------------------------*/

CString
FloatToString( const float value )
{GUCEF_TRACE;
        char floatChars[ 16 ];
        sprintf( floatChars, "%f", value );
        
        return floatChars; 
}

/*-------------------------------------------------------------------------*/

CString
DoubleToString( const double value )
{GUCEF_TRACE;
        char doubleChars[ 16 ];
        sprintf( doubleChars, "%f", value );
        
        return doubleChars; 
}

/*-------------------------------------------------------------------------*/

CString
LastSubDir( const CString& path )
{GUCEF_TRACE;

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

/*-------------------------------------------------------------------------*/

CString
StripLastSubDir( const CString& completePath )
{GUCEF_TRACE;

    if ( 0 != completePath.Length() )
    {
        CString path = completePath.ReplaceChar( DIRSEPCHAROPPOSITE, DIRSEPCHAR );        
        if ( path[ path.Length()-1 ] == DIRSEPCHAR )
        {
            path = path.CutChars( 1, false );
        }        
        
        CString remnamt = path.SubstrToChar( DIRSEPCHAR, false );
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

GUCEFCORE_EXPORT_CPP CString
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

bool
WriteStringAsTextFile( const CString& filePath    , 
                       const CString& fileContent )
{GUCEF_TRACE;
    
    FILE* fptr = fopen( filePath.C_String(), "wb" );
    if ( NULL != fptr )
    {
        #ifdef GUCEF_MSWIN_BUILD
        // Turn everything into "\n" in case of a mixed EOL char string
        CString content = fileContent.ReplaceSubstr( "\r\n", "\n" );
        content = fileContent.ReplaceSubstr( "\r", "\n" );
        
        // Now turn "\n" into the windows variant which is "\r\n"
        content = fileContent.ReplaceSubstr( "\n", "\r\n" );
        
        fwrite( content.C_String(), content.Length(), 1, fptr );
        fclose( fptr );
        #else
        fwrite( fileContent.C_String(), fileContent.Length(), 1, fptr );
        fclose( fptr );
        #endif
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
LoadTextFileAsString( const CString& filePath , 
                      CString& fileContent    )
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
                if ( charBuffer[ i ] == 0 )
                {
                    delimter = (int) i;
                }
            }
            
            if ( delimter != bytesRead )
            {
                // This is not a text file
                fclose( fptr );
                return false;
            }
            
            // Append what we can
            fileContent.Append( charBuffer, (UInt32)delimter );
        }
        while ( bytesRead == 1024 );
        
        fclose( fptr );
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
