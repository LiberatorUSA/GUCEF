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

#ifndef GUCEF_CORE_CIDATANODESERIALIZABLE_H
#define GUCEF_CORE_CIDATANODESERIALIZABLE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h" /* macros that are GUCEF specific and generic macros */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_ETYPES_H
#include "ETypes.h"           /* simple types used */
#define GUCEF_CORE_ETYPES_H
#endif /* GUCEF_CORE_ETYPES_H ? */

#ifndef GUCEF_CORE_CITYPENAMED_H
#include "CITypeNamed.h"
#define GUCEF_CORE_CITYPENAMED_H
#endif /* GUCEF_CORE_CITYPENAMED_H ? */

#ifndef GUCEF_CORE_CDATANODESERIALIZABLESETTINGS_H
#include "gucefCORE_CDataNodeSerializableSettings.h"
#define GUCEF_CORE_CDATANODESERIALIZABLESETTINGS_H
#endif /* GUCEF_CORE_CDATANODESERIALIZABLESETTINGS_H ? */

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

class CDataNode;

/*-------------------------------------------------------------------------*/

/**
 *  Abstract base class for adding a serialization API to decending classes.
 *  Serialize/Deserialze via the DataNode class as a DOM 
 */
class GUCEF_CORE_PUBLIC_CPP CIDataNodeSerializable
{
    public:

    CIDataNodeSerializable( void );

    CIDataNodeSerializable( const CIDataNodeSerializable& src );

    virtual ~CIDataNodeSerializable();

    CIDataNodeSerializable& operator=( const CIDataNodeSerializable& src );

    /**
     *  Attempts to serialize the object to a DOM created out of DataNode objects
     */
    virtual bool Serialize( CDataNode& domRootNode                        ,
                            const CDataNodeSerializableSettings& settings ) const = 0;

    /**
     *  Attempts to serialize the object to a DOM created out of DataNode objects
     *
     *  @param domRootNode Node that acts as root of the DOM data tree from which to deserialize
     *  @return whether deserializing the object data from the given DOM was successfull.
     */
    virtual bool Deserialize( const CDataNode& domRootNode                  ,
                              const CDataNodeSerializableSettings& settings ) = 0;

};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CIDATANODESERIALIZABLE_H ? */
