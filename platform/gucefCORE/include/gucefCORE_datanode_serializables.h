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
 
#ifndef GUCEF_CORE_DATANODE_SERIALIZABLES_H
#define GUCEF_CORE_DATANODE_SERIALIZABLES_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTDATANODESERIALIZABLEOBJ_H
#include "gucefCORE_CTDataNodeSerializableObj.h"
#define GUCEF_CORE_CTDATANODESERIALIZABLEOBJ_H
#endif /* GUCEF_CORE_CTDATANODESERIALIZABLEOBJ_H ? */

#ifndef GUCEF_CORE_CSTRING_H
#include "gucefCORE_CString.h"
#define GUCEF_CORE_CSTRING_H
#endif /* GUCEF_CORE_CSTRING_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF { 
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

typedef CTDataNodeSerializableObj< char >                      TDataNodeSerializableChar;
typedef CTDataNodeSerializableObj< unsigned char >             TDataNodeSerializableUChar;
typedef CTDataNodeSerializableObj< short >                     TDataNodeSerializableShort;
typedef CTDataNodeSerializableObj< unsigned short >            TDataNodeSerializableUShort;
typedef CTDataNodeSerializableObj< int >                       TDataNodeSerializableInt;
typedef CTDataNodeSerializableObj< unsigned int >              TDataNodeSerializableUInt;
typedef CTDataNodeSerializableObj< long >                      TDataNodeSerializableLong;
typedef CTDataNodeSerializableObj< unsigned int >              TDataNodeSerializableULong;
typedef CTDataNodeSerializableObj< float >                     TDataNodeSerializableFloat;
typedef CTDataNodeSerializableObj< double >                    TDataNodeSerializableDouble;
typedef CTDataNodeSerializableObj< bool >                      TDataNodeSerializableBool;
typedef CTDataNodeSerializableObj< Float32 >                   TDataNodeSerializableFloat32;
typedef CTDataNodeSerializableObj< Float64 >                   TDataNodeSerializableFloat64;
typedef CTDataNodeSerializableObj< UInt64 >                    TDataNodeSerializableUInt64;
typedef CTDataNodeSerializableObj< Int64 >                     TDataNodeSerializableInt64;
typedef CTDataNodeSerializableObj< UInt32 >                    TDataNodeSerializableUInt32;
typedef CTDataNodeSerializableObj< Int32 >                     TDataNodeSerializableInt32;
typedef CTDataNodeSerializableObj< UInt16 >                    TDataNodeSerializableUInt16;
typedef CTDataNodeSerializableObj< Int16 >                     TDataNodeSerializableInt16;
typedef CTDataNodeSerializableObj< UInt8 >                     TDataNodeSerializableUInt8;
typedef CTDataNodeSerializableObj< Int8 >                      TDataNodeSerializableInt8;
typedef CTDataNodeSerializableObj< CString >                   TTDataNodeSerializableString;
typedef CTDataNodeSerializableObj< CAsciiString >              TTDataNodeSerializableAsciiString;
typedef CTDataNodeSerializableObj< CUtf8String >               TTDataNodeSerializableUtf8String;
                                                                                                   
/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_DATANODE_SERIALIZABLES_H ? */
