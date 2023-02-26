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

#include "gucefCORE_CValueDefinition.h"

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

const CString CValueDefinition::ClassTypeName = "GUCEF::CORE::CValueDefinition";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CValueDefinition::CValueDefinition( void )
    : CIDataNodeSerializable()
    , defaultValue()
    , isRequired( false )
    , constraints()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CValueDefinition::CValueDefinition( const CValueDefinition& src )
    : CIDataNodeSerializable( src )
    , defaultValue( src.defaultValue )
    , isRequired( src.isRequired )
    , constraints( src.constraints )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CValueDefinition::~CValueDefinition()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CValueDefinition::AreConstraintsSatisfiedBy( const CVariant& valueToConstrain ) const
{GUCEF_TRACE;

    if ( isRequired && !valueToConstrain.IsInitialized() )
        return false;

    return constraints.AreConstraintsSatisfiedBy( valueToConstrain );
}

/*-------------------------------------------------------------------------*/

CValueDefinition&
CValueDefinition::operator=( const CValueDefinition& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        CIDataNodeSerializable::operator=( src );
        defaultValue = src.defaultValue;
        isRequired = src.isRequired;
        constraints = src.constraints;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
CValueDefinition::operator==( const CValueDefinition& other ) const
{GUCEF_TRACE;

    return defaultValue == other.defaultValue &&
           isRequired == other.isRequired &&
           constraints == other.constraints;
}

/*-------------------------------------------------------------------------*/

bool
CValueDefinition::operator!=( const CValueDefinition& other ) const
{GUCEF_TRACE;

    return defaultValue != other.defaultValue ||
           isRequired != other.isRequired ||
           constraints != other.constraints;
}

/*-------------------------------------------------------------------------*/

bool
CValueDefinition::Serialize( CDataNode& domRootNode                        ,
                             const CDataNodeSerializableSettings& settings ) const
{GUCEF_TRACE;

    bool totalSuccess = true;

    domRootNode.Clear();
    domRootNode.SetName( "ValueDefinition" );

    totalSuccess = domRootNode.SetAttribute( "defaultValue", defaultValue ) && totalSuccess;
    totalSuccess = domRootNode.SetAttribute( "isRequired", isRequired ) && totalSuccess;

    CDataNode* constraintsNode = domRootNode.AddChild( "constraints", GUCEF_DATATYPE_ARRAY );
    if ( GUCEF_NULL != constraintsNode )
    {
        totalSuccess = constraints.Serialize( *constraintsNode, settings ) && totalSuccess;
    }

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CValueDefinition::Deserialize( const CDataNode& domRootNode                  ,
                               const CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;

    bool totalSuccess = true;

    defaultValue = domRootNode.GetAttributeValueOrChildValueByName( "defaultValue", defaultValue );
    isRequired = domRootNode.GetAttributeValueOrChildValueByName( "isRequired", isRequired ).AsBool();

    const CDataNode* constraintsNode = domRootNode.FindChild( "constraints" );
    if ( GUCEF_NULL != constraintsNode )
    {
        totalSuccess = constraints.Deserialize( *constraintsNode, settings ) && totalSuccess;
    }

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

CICloneable* 
CValueDefinition::Clone( void ) const
{GUCEF_TRACE;

    return new CValueDefinition( *this );
}

/*-------------------------------------------------------------------------*/

const CString& 
CValueDefinition::GetClassTypeName( void ) const
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
