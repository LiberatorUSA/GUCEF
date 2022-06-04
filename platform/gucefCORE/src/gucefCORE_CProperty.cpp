/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
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

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#include "gucefCORE_CProperty.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CProperty::CProperty( void )
    : CIDataNodeSerializable()
    , name()
    , id()
    , value()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CProperty::CProperty( const CProperty& src )
    : CIDataNodeSerializable( src )
    , name( src.name )
    , id( src.id )
    , value( src.value )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CProperty::~CProperty()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool 
CProperty::operator==( const CProperty& other ) const
{GUCEF_TRACE;

    return id == other.id && name == other.id && value == other.value;
}

/*-------------------------------------------------------------------------*/

bool 
CProperty::operator!=( const CProperty& other ) const
{GUCEF_TRACE;

    return id != other.id || name != other.id || value != other.value;
}

/*-------------------------------------------------------------------------*/

bool
CProperty::Serialize( CDataNode& domRootNode                        ,
                      const CDataNodeSerializableSettings& settings ) const
{GUCEF_TRACE;

    bool totalSuccess = true;

    domRootNode.Clear();
    domRootNode.SetName( "Property" );
    totalSuccess = domRootNode.SetAttribute( "name", name ) && totalSuccess;
    totalSuccess = domRootNode.SetAttribute( "id", id ) && totalSuccess;
    totalSuccess = domRootNode.SetAttribute( "value", value ) && totalSuccess;

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CProperty::Deserialize( const CDataNode& domRootNode                  ,
                        const CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;

    name = domRootNode.GetAttributeValueOrChildValueByName( "name", name );
    id = domRootNode.GetAttributeValueOrChildValueByName( "id", id );
    value = domRootNode.GetAttributeValueOrChildValueByName( "value", value );
    return true;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
