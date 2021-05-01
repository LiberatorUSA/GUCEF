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
 
#ifndef GUCEF_CORE_CLONEABLES_H
#define GUCEF_CORE_CLONEABLES_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_ETYPES_H
#include "gucefCORE_ETypes.h"      /* gucefCORE basic types */
#define GUCEF_CORE_ETYPES_H
#endif /* GUCEF_CORE_ETYPES_H ? */

#ifndef GUCEF_CORE_CTCLONEABLEOBJ_H
#include "CTCloneableObj.h"        /* templated implementation of a cloneable object */
#define GUCEF_CORE_CTCLONEABLEOBJ_H
#endif /* GUCEF_CORE_CTCLONEABLEOBJ_H ? */

#ifndef GUCEF_CORE_CTLINKEDCLONEABLEOBJ_H
#include "CTLinkedCloneableObj.h"        /* templated implementation of a linked cloneable object */
#define GUCEF_CORE_CTLINKEDCLONEABLEOBJ_H
#endif /* GUCEF_CORE_CTLINKEDCLONEABLEOBJ_H ? */

#ifndef GUCEF_CORE_CTCLONEABLEEXPANSION_H
#include "CTCloneableExpansion.h"        /* templated implementation of class expansion to add a cloneable interface */
#define GUCEF_CORE_CTCLONEABLEEXPANSION_H
#endif /* GUCEF_CORE_CTCLONEABLEEXPANSION_H ? */

#ifndef GUCEF_CORE_CSTRING_H
#include "gucefCORE_CString.h"
#define GUCEF_CORE_CSTRING_H
#endif /* GUCEF_CORE_CSTRING_H ? */

#ifndef GUCEF_CORE_CVARIANT_H
#include "gucefCORE_CVariant.h"
#define GUCEF_CORE_CVARIANT_H
#endif /* GUCEF_CORE_CVARIANT_H ? */

#ifndef GUCEF_CORE_CDATETIME_H
#include "gucefCORE_CDateTime.h"
#define GUCEF_CORE_CDATETIME_H
#endif /* GUCEF_CORE_CDATETIME_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_CORE_CLINKEDTRANSFERBUFFER_H
#include "gucefCORE_CLinkedTransferBuffer.h"
#define GUCEF_CORE_CLINKEDTRANSFERBUFFER_H
#endif /* GUCEF_CORE_CLINKEDTRANSFERBUFFER_H ? */

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

typedef CTCloneableObj< CDateTime >      TCloneableDateTime;
typedef CTCloneableObj< CVariant >       TCloneableVariant;
typedef CTCloneableObj< CDynamicBuffer > TCloneableBuffer;
typedef CTCloneableObj< CString >        TCloneableString;
typedef CTCloneableObj< char >           TCloneableChar;
typedef CTCloneableObj< unsigned char >  TCloneableUChar;
typedef CTCloneableObj< short >          TCloneableShort;
typedef CTCloneableObj< unsigned short > TCloneableUShort;
typedef CTCloneableObj< int >            TCloneableInt;
typedef CTCloneableObj< unsigned int >   TCloneableUInt;
typedef CTCloneableObj< long >           TCloneableLong;
typedef CTCloneableObj< unsigned int >   TCloneableULong;
typedef CTCloneableObj< float >          TCloneableFloat;
typedef CTCloneableObj< double >         TCloneableDouble;
typedef CTCloneableObj< bool >           TCloneableBool;
typedef CTCloneableObj< double >         TCloneableDouble;
typedef CTCloneableObj< float >          TCloneableFloat;
typedef CTCloneableObj< UInt64 >         TCloneableUInt64;
typedef CTCloneableObj< Int64 >          TCloneableInt64;
typedef CTCloneableObj< UInt32 >         TCloneableUInt32;
typedef CTCloneableObj< Int32 >          TCloneableInt32;
typedef CTCloneableObj< UInt16 >         TCloneableUInt16;
typedef CTCloneableObj< Int16 >          TCloneableInt16;
typedef CTCloneableObj< UInt8 >          TCloneableUInt8;
typedef CTCloneableObj< Int8 >           TCloneableInt8;

typedef CTLinkedCloneableObj< CDynamicBuffer > TLinkedCloneableBuffer;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CLONEABLES_H ? */
