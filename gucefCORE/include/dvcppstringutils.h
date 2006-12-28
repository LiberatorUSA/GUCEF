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
VersionToString( const TVersion* version );

/*-------------------------------------------------------------------------*/

GUCEFCORE_EXPORT_CPP bool
StringToBool( const CString& str );

/*-------------------------------------------------------------------------*/

GUCEFCORE_EXPORT_CPP Int32
StringToInt( const CString& str );

/*-------------------------------------------------------------------------*/

GUCEFCORE_EXPORT_CPP CString
IntToString( const Int32 value );

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

- 21-09-2005 :
        - Added StringToInt()
        - Added IntToString()
        - Added StringToMD5String()
- 24-07-2005 :
        - Added StringToBool()
- 26-04-2005 :
        - Fixed RelativePath() so that strings that do not have any tags don't
          cause a problem. 
- 03-04-2005 :
        - Initial version of this file.
          Meant to hold string class utilities that do not belong in the class
          or in the class header itself.
        - Added RelativePath()  

-----------------------------------------------------------------------------*/
