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

#include "gucefCORE_CPropertyDefinition.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CString CPropertyDefinition::ClassTypeName = "GUCEF::CORE::CPropertyDefinition";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CPropertyDefinition::CPropertyDefinition( void )
    : CIDataNodeSerializable()
    , name()
    , id()
    , value()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPropertyDefinition::CPropertyDefinition( const CPropertyDefinition& src )
    : CIDataNodeSerializable( src )
    , name( src.name )
    , id( src.id )
    , value( src.value )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPropertyDefinition::~CPropertyDefinition()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool 
CPropertyDefinition::AreConstraintsSatisfiedBy( const CProperty& propertyToConstrain ) const
{GUCEF_TRACE;

    return id.AreConstraintsSatisfiedBy( propertyToConstrain.id ) &&
           name.AreConstraintsSatisfiedBy( propertyToConstrain.name ) &&
           value.AreConstraintsSatisfiedBy( propertyToConstrain.value );
}

/*-------------------------------------------------------------------------*/

CPropertyDefinition& 
CPropertyDefinition::operator=( const CPropertyDefinition& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        CIDataNodeSerializable::operator=( src );
        id = src.id;
        name = src.name;
        value = src.value;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool 
CPropertyDefinition::operator==( const CPropertyDefinition& other ) const
{GUCEF_TRACE;

    return id == other.id &&
           name == other.name &&
           value == other.value;
}

/*-------------------------------------------------------------------------*/

bool 
CPropertyDefinition::operator!=( const CPropertyDefinition& other ) const
{GUCEF_TRACE;

    return id != other.id ||
           name != other.name ||
           value != other.value;
}

/*-------------------------------------------------------------------------*/

bool
CPropertyDefinition::Serialize( CDataNode& domRootNode                        ,
                                const CDataNodeSerializableSettings& settings ) const
{GUCEF_TRACE;

    bool totalSuccess = true;

    domRootNode.Clear();
    domRootNode.SetName( "PropertyDefinition" );

    CDataNode* idDefinitionNode = domRootNode.AddChild( "id", GUCEF_DATATYPE_OBJECT );
    if ( GUCEF_NULL != idDefinitionNode )
    {
        totalSuccess = id.Serialize( *idDefinitionNode, settings ) && totalSuccess;
    }
    CDataNode* nameDefinitionNode = domRootNode.AddChild( "name", GUCEF_DATATYPE_OBJECT );
    if ( GUCEF_NULL != nameDefinitionNode )
    {
        totalSuccess = name.Serialize( *nameDefinitionNode, settings ) && totalSuccess;
    }
    CDataNode* valueDefinitionNode = domRootNode.AddChild( "value", GUCEF_DATATYPE_OBJECT );
    if ( GUCEF_NULL != valueDefinitionNode )
    {
        totalSuccess = value.Serialize( *valueDefinitionNode, settings ) && totalSuccess;
    }

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CPropertyDefinition::Deserialize( const CDataNode& domRootNode                  ,
                                  const CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;

    bool totalSuccess = true;
    
    const CDataNode* idDefinitionNode = domRootNode.FindChild( "id" );
    if ( GUCEF_NULL != idDefinitionNode )
    {
        totalSuccess = id.Deserialize( *idDefinitionNode, settings ) && totalSuccess;
    }
    const CDataNode* nameDefinitionNode = domRootNode.FindChild( "name" );
    if ( GUCEF_NULL != nameDefinitionNode )
    {
        totalSuccess = name.Deserialize( *nameDefinitionNode, settings ) && totalSuccess;
    }
    const CDataNode* valueDefinitionNode = domRootNode.FindChild( "value" );
    if ( GUCEF_NULL != valueDefinitionNode )
    {
        totalSuccess = value.Deserialize( *valueDefinitionNode, settings ) && totalSuccess;
    }

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

CICloneable* 
CPropertyDefinition::Clone( void ) const
{GUCEF_TRACE;

    return new CPropertyDefinition( *this );
}

/*-------------------------------------------------------------------------*/

const CString& 
CPropertyDefinition::GetClassTypeName( void ) const
{GUCEF_TRACE;

    return ClassTypeName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
