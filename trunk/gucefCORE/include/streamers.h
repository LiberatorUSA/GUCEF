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

#ifndef GUCEF_CORE_STREAMERS_H
#define GUCEF_CORE_STREAMERS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

#ifndef GUCEF_CORE_CTSTREAMER_H
#include "CTStreamer.h"
#define GUCEF_CORE_CTSTREAMER_H
#endif /* GUCEF_CORE_CTSTREAMER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

typedef CTStreamer< char, char >           TStreamerCharChar;
typedef CTStreamer< char, unsigned char >  TStreamerCharUChar;
typedef CTStreamer< char, short >          TStreamerCharShort;
typedef CTStreamer< char, unsigned short > TStreamerCharUShort;
typedef CTStreamer< char, int >            TStreamerCharInt;
typedef CTStreamer< char, unsigned int >   TStreamerCharUInt;
typedef CTStreamer< char, long >           TStreamerCharLong;
typedef CTStreamer< char, unsigned int >   TStreamerCharULong;
typedef CTStreamer< char, float >          TStreamerCharFloat;
typedef CTStreamer< char, double >         TStreamerCharDouble;
typedef CTStreamer< char, Int8 >           TStreamerCharInt8;
typedef CTStreamer< char, UInt8 >          TStreamerCharUInt8;
typedef CTStreamer< char, Int16 >          TStreamerCharInt16;
typedef CTStreamer< char, UInt16 >         TStreamerCharUInt16;
typedef CTStreamer< char, Int32 >          TStreamerCharInt32;
typedef CTStreamer< char, UInt32 >         TStreamerCharUInt32;
typedef CTStreamer< char, Float32 >        TStreamerCharFloat32;
typedef CTStreamer< char, Float64 >        TStreamerCharFloat64;
typedef CTStreamer< char, TVersion >       TStreamerCharVersion;
typedef CTStreamer< char, CString >        TStreamerCharString;

typedef CTStreamer< unsigned char, char >           TStreamerUCharChar;
typedef CTStreamer< unsigned char, unsigned char >  TStreamerUCharUChar;
typedef CTStreamer< unsigned char, short >          TStreamerUCharShort;
typedef CTStreamer< unsigned char, unsigned short > TStreamerUCharUShort;
typedef CTStreamer< unsigned char, int >            TStreamerUCharInt;
typedef CTStreamer< unsigned char, unsigned int >   TStreamerUCharUInt;
typedef CTStreamer< unsigned char, long >           TStreamerUCharLong;
typedef CTStreamer< unsigned char, unsigned int >   TStreamerUCharULong;
typedef CTStreamer< unsigned char, float >          TStreamerUCharFloat;
typedef CTStreamer< unsigned char, double >         TStreamerUCharDouble;
typedef CTStreamer< unsigned char, Int8 >           TStreamerUCharInt8;
typedef CTStreamer< unsigned char, UInt8 >          TStreamerUCharUInt8;
typedef CTStreamer< unsigned char, Int16 >          TStreamerUCharInt16;
typedef CTStreamer< unsigned char, UInt16 >         TStreamerUCharUInt16;
typedef CTStreamer< unsigned char, Int32 >          TStreamerUCharInt32;
typedef CTStreamer< unsigned char, UInt32 >         TStreamerUCharUInt32;
typedef CTStreamer< unsigned char, Float32 >        TStreamerUCharFloat32;
typedef CTStreamer< unsigned char, Float64 >        TStreamerUCharFloat64;
typedef CTStreamer< unsigned char, TVersion >       TStreamerUCharVersion;
typedef CTStreamer< unsigned char, CString >        TStreamerUCharString;

typedef CTStreamer< short, char >           TStreamerShortChar;
typedef CTStreamer< short, unsigned char >  TStreamerShortUChar;
typedef CTStreamer< short, short >          TStreamerShortShort;
typedef CTStreamer< short, unsigned short > TStreamerShortUShort;
typedef CTStreamer< short, int >            TStreamerShortInt;
typedef CTStreamer< short, unsigned int >   TStreamerShortUInt;
typedef CTStreamer< short, long >           TStreamerShortLong;
typedef CTStreamer< short, unsigned int >   TStreamerShortULong;
typedef CTStreamer< short, float >          TStreamerShortFloat;
typedef CTStreamer< short, double >         TStreamerShortDouble;
typedef CTStreamer< short, Int8 >           TStreamerShortInt8;
typedef CTStreamer< short, UInt8 >          TStreamerShortUInt8;
typedef CTStreamer< short, Int16 >          TStreamerShortInt16;
typedef CTStreamer< short, UInt16 >         TStreamerShortUInt16;
typedef CTStreamer< short, Int32 >          TStreamerShortInt32;
typedef CTStreamer< short, UInt32 >         TStreamerShortUInt32;
typedef CTStreamer< short, Float32 >        TStreamerShortFloat32;
typedef CTStreamer< short, Float64 >        TStreamerShortFloat64;
typedef CTStreamer< short, TVersion >       TStreamerShortVersion;
typedef CTStreamer< short, CString >        TStreamerShortString;

typedef CTStreamer< UInt16, char >           TStreamerUInt16Char;
typedef CTStreamer< UInt16, unsigned char >  TStreamerUInt16UChar;
typedef CTStreamer< UInt16, short >          TStreamerUInt16Short;
typedef CTStreamer< UInt16, unsigned short > TStreamerUInt16UShort;
typedef CTStreamer< UInt16, int >            TStreamerUInt16Int;
typedef CTStreamer< UInt16, unsigned int >   TStreamerUInt16UInt;
typedef CTStreamer< UInt16, long >           TStreamerUInt16Long;
typedef CTStreamer< UInt16, unsigned int >   TStreamerUInt16ULong;
typedef CTStreamer< UInt16, float >          TStreamerUInt16Float;
typedef CTStreamer< UInt16, double >         TStreamerUInt16Double;
typedef CTStreamer< UInt16, Int8 >           TStreamerUInt16Int8;
typedef CTStreamer< UInt16, UInt8 >          TStreamerUInt16UInt8;
typedef CTStreamer< UInt16, Int16 >          TStreamerUInt16Int16;
typedef CTStreamer< UInt16, UInt16 >         TStreamerUInt16UInt16;
typedef CTStreamer< UInt16, Int32 >          TStreamerUInt16Int32;
typedef CTStreamer< UInt16, UInt32 >         TStreamerUInt16UInt32;
typedef CTStreamer< UInt16, Float32 >        TStreamerUInt16Float32;
typedef CTStreamer< UInt16, Float64 >        TStreamerUInt16Float64;
typedef CTStreamer< UInt16, TVersion >       TStreamerUInt16Version;
typedef CTStreamer< UInt16, CString >        TStreamerUInt16String;

typedef CTStreamer< Int32, char >           TStreamerInt32Char;
typedef CTStreamer< Int32, unsigned char >  TStreamerInt32UChar;
typedef CTStreamer< Int32, short >          TStreamerInt32Short;
typedef CTStreamer< Int32, unsigned short > TStreamerInt32UShort;
typedef CTStreamer< Int32, int >            TStreamerInt32Int;
typedef CTStreamer< Int32, unsigned int >   TStreamerInt32UInt;
typedef CTStreamer< Int32, long >           TStreamerInt32Long;
typedef CTStreamer< Int32, unsigned int >   TStreamerInt32ULong;
typedef CTStreamer< Int32, float >          TStreamerInt32Float;
typedef CTStreamer< Int32, double >         TStreamerInt32Double;
typedef CTStreamer< Int32, Int8 >           TStreamerInt32Int8;
typedef CTStreamer< Int32, UInt8 >          TStreamerInt32UInt8;
typedef CTStreamer< Int32, Int16 >          TStreamerInt32Int16;
typedef CTStreamer< Int32, UInt16 >         TStreamerInt32UInt16;
typedef CTStreamer< Int32, Int32 >          TStreamerInt32Int32;
typedef CTStreamer< Int32, UInt32 >         TStreamerInt32UInt32;
typedef CTStreamer< Int32, Float32 >        TStreamerInt32Float32;
typedef CTStreamer< Int32, Float64 >        TStreamerInt32Float64;
typedef CTStreamer< Int32, TVersion >       TStreamerInt32Version;
typedef CTStreamer< Int32, CString >        TStreamerInt32String;

typedef CTStreamer< UInt32, char >           TStreamerUInt32Char;
typedef CTStreamer< UInt32, unsigned char >  TStreamerUInt32UChar;
typedef CTStreamer< UInt32, short >          TStreamerUInt32Short;
typedef CTStreamer< UInt32, unsigned short > TStreamerUInt32UShort;
typedef CTStreamer< UInt32, int >            TStreamerUInt32Int;
typedef CTStreamer< UInt32, unsigned int >   TStreamerUInt32UInt;
typedef CTStreamer< UInt32, long >           TStreamerUInt32Long;
typedef CTStreamer< UInt32, unsigned int >   TStreamerUInt32ULong;
typedef CTStreamer< UInt32, float >          TStreamerUInt32Float;
typedef CTStreamer< UInt32, double >         TStreamerUInt32Double;
typedef CTStreamer< UInt32, Int8 >           TStreamerUInt32Int8;
typedef CTStreamer< UInt32, UInt8 >          TStreamerUInt32UInt8;
typedef CTStreamer< UInt32, Int16 >          TStreamerUInt32Int16;
typedef CTStreamer< UInt32, UInt16 >         TStreamerUInt32UInt16;
typedef CTStreamer< UInt32, Int32 >          TStreamerUInt32Int32;
typedef CTStreamer< UInt32, UInt32 >         TStreamerUInt32UInt32;
typedef CTStreamer< UInt32, Float32 >        TStreamerUInt32Float32;
typedef CTStreamer< UInt32, Float64 >        TStreamerUInt32Float64;
typedef CTStreamer< UInt32, TVersion >       TStreamerUInt32Version;
typedef CTStreamer< UInt32, CString >        TStreamerUInt32String;

typedef CTStreamer< CString, char >           TStreamerStringChar;
typedef CTStreamer< CString, unsigned char >  TStreamerStringUChar;
typedef CTStreamer< CString, short >          TStreamerStringShort;
typedef CTStreamer< CString, unsigned short > TStreamerStringUShort;
typedef CTStreamer< CString, int >            TStreamerStringInt;
typedef CTStreamer< CString, unsigned int >   TStreamerStringUInt;
typedef CTStreamer< CString, long >           TStreamerStringLong;
typedef CTStreamer< CString, unsigned int >   TStreamerStringULong;
typedef CTStreamer< CString, float >          TStreamerStringFloat;
typedef CTStreamer< CString, double >         TStreamerStringDouble;
typedef CTStreamer< CString, Int8 >           TStreamerStringInt8;
typedef CTStreamer< CString, UInt8 >          TStreamerStringUInt8;
typedef CTStreamer< CString, Int16 >          TStreamerStringInt16;
typedef CTStreamer< CString, UInt16 >         TStreamerStringUInt16;
typedef CTStreamer< CString, Int32 >          TStreamerStringInt32;
typedef CTStreamer< CString, UInt32 >         TStreamerStringUInt32;
typedef CTStreamer< CString, Float32 >        TStreamerStringFloat32;
typedef CTStreamer< CString, Float64 >        TStreamerStringFloat64;
typedef CTStreamer< CString, TVersion >       TStreamerStringVersion;
typedef CTStreamer< CString, CString >        TStreamerStringString;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_STREAMERS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 02-03-2007 :
        - Dinand: re-added this header

-----------------------------------------------------------------------------*/
