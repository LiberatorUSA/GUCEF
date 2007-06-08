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
#elif GUCEF_LINUX_BUILD
  #include <unistd.h>             /* POSIX utilities */
  #include <limits.h>             /* Linux OS limits */
  #define MAX_DIR_LENGTH PATH_MAX
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
RelativePath( const CString& relpath )
{TRACE;        
        if ( relpath.Length() )
        {
                char buffer[ MAX_DIR_LENGTH ];
                Relative_Path( relpath.C_String() ,
                               buffer             ,
                               MAX_DIR_LENGTH     );                               
           
                CString abspath( buffer );   
                return abspath;                               
        }                
        return relpath;
}

/*-------------------------------------------------------------------------*/

void
AppendToPath( CString& path           ,
              const CString& addition )
{TRACE;
        char buffer[ MAX_DIR_LENGTH ];
        
        strncpy( buffer          , 
                 path.C_String() ,
                 path.Length()+1 );       
        Append_To_Path( buffer              ,
                        addition.C_String() );                        
        path = buffer;                                
}

/*-------------------------------------------------------------------------*/

CString
VersionToString( const TVersion* version )
{TRACE;
        char verstr[ 25 ];
        sprintf( verstr, "%d.%d.%d.%d", version->mayor, version->minor, version->patch, version->release );
        return verstr;        
}

/*-------------------------------------------------------------------------*/

bool
StringToBool( const CString& str )
{TRACE;
        UInt32 boolint = String_To_Boolint( str.C_String() );
        return boolint > 0;
}

/*-------------------------------------------------------------------------*/

Int32
StringToInt32( const CString& str )
{TRACE;

    Int32 value;
    sscanf( str.C_String(), "%d", &value );
    return value;        
}

/*-------------------------------------------------------------------------*/

CString
Int32ToString( const Int32 value )
{TRACE;

    char intBuffer[ 10 ];
    sprintf( intBuffer, "%d", value );
    return CString( intBuffer );
}

/*-------------------------------------------------------------------------*/

UInt32
StringToUInt32( const CString& str )
{TRACE;

    UInt32 value;
    sscanf( str.C_String(), "%u", &value );
    return value;
}

/*-------------------------------------------------------------------------*/

CString
UInt32ToString( const UInt32 value )
{TRACE;

    char intBuffer[ 10 ];
    sprintf( intBuffer, "%u", value );
    return CString( intBuffer );
}

/*-------------------------------------------------------------------------*/

UInt16
StringToUInt16( const CString& str )
{TRACE;

    UInt16 value;
    sscanf( str.C_String(), "%u", &value );
    return value;
}

/*-------------------------------------------------------------------------*/

CString
UInt16ToString( const UInt16 value )
{TRACE;

    char intBuffer[ 10 ];
    sprintf( intBuffer, "%u", value );
    return CString( intBuffer );
}

/*-------------------------------------------------------------------------*/

CString
PointerToString( const void* value )
{GUCEF_TRACE;

    char addrBuffer[ 10 ];
    sprintf( addrBuffer, "%p", value );
    return CString( addrBuffer );    
}

/*-------------------------------------------------------------------------*/

CString
StringToMD5String( const CString& str )
{TRACE;
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
{TRACE;
        char floatChars[ 16 ];
        sprintf( floatChars, "%f", value );
        
        return floatChars; 
}

/*-------------------------------------------------------------------------*/

CString
DoubleToString( const double value )
{TRACE;
        char doubleChars[ 16 ];
        sprintf( doubleChars, "%f", value );
        
        return doubleChars; 
}

/*-------------------------------------------------------------------------*/

CString
LastSubDir( const CString& path )
{TRACE;

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
StripFilename( const CString& pathPlusFilename )
{TRACE;

    char* buffer = new char[ pathPlusFilename.Length()+1 ];
    UInt32 length = _Strip_Filename( buffer, pathPlusFilename.C_String() );
    CString resultStr( buffer, length );
    delete []buffer;
    return resultStr;
}

/*-------------------------------------------------------------------------*/

GUCEFCORE_EXPORT_CPP CString
ExtractFilename( const CString& pathPlusFilename )
{TRACE;

    return Extract_Filename( pathPlusFilename.C_String() );
}

/*-------------------------------------------------------------------------*/

bool
FileExists( const CString& filename )
{TRACE;

    return 0 != File_Exists( filename.C_String() );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
