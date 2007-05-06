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

#ifndef GUCEF_CORE_NUMERICIDS_H
#define GUCEF_CORE_NUMERICIDS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

#ifndef GUCEF_CORE_ETYPES_H
#include "gucefCORE_ETypes.h"
#define GUCEF_CORE_ETYPES_H
#endif /* GUCEF_CORE_ETYPES_H ? */

#ifndef GUCEF_CORE_CTNUMERICID_H
#include "CTNumericID.h"
#define GUCEF_CORE_CTNUMERICID_H
#endif /* GUCEF_CORE_CTNUMERICID_H ? */

#ifndef GUCEF_CORE_CTNUMERICIDGENERATOR_H
#include "CTNumericIDGenerator.h"
#define GUCEF_CORE_CTNUMERICIDGENERATOR_H
#endif /* GUCEF_CORE_CTNUMERICIDGENERATOR_H ? */

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

typedef CTNumericIDGenerator< UInt8 >    T8BitNumericIDGenerator;
typedef CTNumericIDGenerator< UInt16 >   T16BitNumericIDGenerator;
typedef CTNumericIDGenerator< UInt32 >   T32BitNumericIDGenerator;
typedef CTNumericIDGenerator< UInt64 >   T64BitNumericIDGenerator;

typedef CTNumericID< UInt8 >    T8BitNumericID;
typedef CTNumericID< UInt16 >   T16BitNumericID;
typedef CTNumericID< UInt32 >   T32BitNumericID;
typedef CTNumericID< UInt64 >   T64BitNumericID;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_NUMERICIDS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 02-03-2007 :
        - Dinand: re-added this header

-----------------------------------------------------------------------------*/
