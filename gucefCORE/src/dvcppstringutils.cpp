/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL, 
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER 
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF 
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT 
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
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
  #define MAX_DIR_LENGTH MAX_PATH
#elif LINUX_BUILD
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
StringToInt( const CString& str )
{TRACE;
        return Str_To_Int( str.C_String() );        
}

/*-------------------------------------------------------------------------*/

CString
IntToString( const Int32 value )
{TRACE;
        CString intstr;
        intstr.SetInt( value );
        return intstr;
}

/*-------------------------------------------------------------------------*/

CString
StringToMD5String( const CString& str )
{TRACE;
        UInt8 md5digest[ 16 ];        
        
        md5fromstring( str.C_String() ,
                       md5digest      );
                       
        char md5string[ 48 ];
        
        md5tostring( md5digest ,
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
