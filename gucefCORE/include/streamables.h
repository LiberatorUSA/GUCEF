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

#ifndef GUCEF_CORE_STREAMABLES_H
#define GUCEF_CORE_STREAMABLES_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

#ifndef GUCEF_CORE_CTSTREAMABLEOBJ_H
#include "CTStreamableObj.h"
#define GUCEF_CORE_CTSTREAMABLEOBJ_H
#endif /* GUCEF_CORE_CTSTREAMABLEOBJ_H ? */

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

typedef CTStreamableObj< char >           TStreamableChar;
typedef CTStreamableObj< unsigned char >  TStreamableUChar;
typedef CTStreamableObj< short >          TStreamableShort;
typedef CTStreamableObj< unsigned short > TStreamableUShort;
typedef CTStreamableObj< int >            TStreamableInt;
typedef CTStreamableObj< unsigned int >   TStreamableUInt;
typedef CTStreamableObj< long >           TStreamableLong;
typedef CTStreamableObj< unsigned int >   TStreamableULong;
typedef CTStreamableObj< float >          TStreamableFloat;
typedef CTStreamableObj< double >         TStreamableDouble;
typedef CTStreamableObj< Int8 >           TStreamableInt8;
typedef CTStreamableObj< UInt8 >          TStreamableUInt8;
typedef CTStreamableObj< Int16 >          TStreamableInt16;
typedef CTStreamableObj< UInt16 >         TStreamableUInt16;
typedef CTStreamableObj< Int32 >          TStreamableInt32;
typedef CTStreamableObj< UInt32 >         TStreamableUInt32;
typedef CTStreamableObj< Float32 >        TStreamableFloat32;
typedef CTStreamableObj< Float64 >        TStreamableFloat64;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_STREAMABLES_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 02-03-2007 :
        - Dinand: re-added this header

-----------------------------------------------------------------------------*/
