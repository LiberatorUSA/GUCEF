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

#include "gucefCORE_CPropertyConstraints.h"

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

CPropertyConstraints::CPropertyConstraints( void )
    : CIDataNodeSerializable()
    , propertyDefaults()
    , nameIsRequired( false )
    , idIsRequired( false )
    , valueIsRequired( false )
    , nameConstraints()
    , idConstraints()
    , valueConstraints()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPropertyConstraints::CPropertyConstraints( const CPropertyConstraints& src )
    : CIDataNodeSerializable( src )
    , propertyDefaults( src.propertyDefaults )
    , nameIsRequired( src.nameIsRequired )
    , idIsRequired( src.idIsRequired )
    , valueIsRequired( src.valueIsRequired )
    , nameConstraints( src.nameConstraints )
    , idConstraints( src.idConstraints )
    , valueConstraints( src.valueConstraints )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPropertyConstraints::~CPropertyConstraints()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool 
CPropertyConstraints::AreConstraintsSatisfiedBy( const CProperty& propertyToConstrain ) const
{GUCEF_TRACE;

    if ( nameIsRequired && !propertyToConstrain.name.IsInitialized() )
        return false;
    if ( idIsRequired && !propertyToConstrain.id.IsInitialized() )
        return false;
    if ( valueIsRequired && !propertyToConstrain.value.IsInitialized() )
        return false;

    return idConstraints.AreConstraintsSatisfiedBy( propertyToConstrain.id ) &&
           nameConstraints.AreConstraintsSatisfiedBy( propertyToConstrain.name ) &&
           valueConstraints.AreConstraintsSatisfiedBy( propertyToConstrain.value );
}

/*-------------------------------------------------------------------------*/

bool
CPropertyConstraints::Serialize( CDataNode& domRootNode                        ,
                                 const CDataNodeSerializableSettings& settings ) const
{GUCEF_TRACE;

    bool totalSuccess = true;

    domRootNode.Clear();
    domRootNode.SetName( "PropertyConstraints" );

    CDataNode* propertyDefaultsNode = domRootNode.AddChild( "propertyDefaults", GUCEF_DATATYPE_OBJECT );
    if ( GUCEF_NULL != propertyDefaultsNode )
    {
        totalSuccess = propertyDefaults.Serialize( *propertyDefaultsNode, settings ) && totalSuccess;
    }

    totalSuccess = domRootNode.SetAttribute( "nameIsRequired", nameIsRequired ) && totalSuccess;
    totalSuccess = domRootNode.SetAttribute( "idIsRequired", nameIsRequired ) && totalSuccess;
    totalSuccess = domRootNode.SetAttribute( "valueIsRequired", nameIsRequired ) && totalSuccess;

    CDataNode* nameConstraintsNode = domRootNode.AddChild( "nameConstraints", GUCEF_DATATYPE_ARRAY );
    if ( GUCEF_NULL != nameConstraintsNode )
    {
        totalSuccess = nameConstraints.Serialize( *nameConstraintsNode, settings ) && totalSuccess;
    }
    CDataNode* idConstraintsNode = domRootNode.AddChild( "idConstraints", GUCEF_DATATYPE_ARRAY );
    if ( GUCEF_NULL != idConstraintsNode )
    {
        totalSuccess = idConstraints.Serialize( *idConstraintsNode, settings ) && totalSuccess;
    }
    CDataNode* valueConstraintsNode = domRootNode.AddChild( "valueConstraints", GUCEF_DATATYPE_ARRAY );
    if ( GUCEF_NULL != valueConstraintsNode )
    {
        totalSuccess = valueConstraints.Serialize( *valueConstraintsNode, settings ) && totalSuccess;
    }

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CPropertyConstraints::Deserialize( const CDataNode& domRootNode                  ,
                                   const CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;

    bool totalSuccess = true;
    
    const CDataNode* propertyDefaultsNode = domRootNode.FindChild( "propertyDefaults" );
    if ( GUCEF_NULL != propertyDefaultsNode )
    {
        totalSuccess = propertyDefaults.Deserialize( *propertyDefaultsNode, settings ) && totalSuccess;
    }

    nameIsRequired = domRootNode.GetAttributeValueOrChildValueByName( "nameIsRequired", nameIsRequired ).AsBool();
    idIsRequired = domRootNode.GetAttributeValueOrChildValueByName( "idIsRequired", idIsRequired ).AsBool();
    valueIsRequired = domRootNode.GetAttributeValueOrChildValueByName( "valueIsRequired", valueIsRequired ).AsBool();

    const CDataNode* nameConstraintsNode = domRootNode.FindChild( "nameConstraints" );
    if ( GUCEF_NULL != nameConstraintsNode )
    {
        totalSuccess = nameConstraints.Deserialize( *nameConstraintsNode, settings ) && totalSuccess;
    }
    const CDataNode* idConstraintsNode = domRootNode.FindChild( "idConstraints" );
    if ( GUCEF_NULL != idConstraintsNode )
    {
        totalSuccess = idConstraints.Deserialize( *idConstraintsNode, settings ) && totalSuccess;
    }
    CDataNode* valueConstraintsNode = domRootNode.FindChild( "valueConstraints" );
    if ( GUCEF_NULL != valueConstraintsNode )
    {
        totalSuccess = valueConstraints.Serialize( *valueConstraintsNode, settings ) && totalSuccess;
    }

    return totalSuccess;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
