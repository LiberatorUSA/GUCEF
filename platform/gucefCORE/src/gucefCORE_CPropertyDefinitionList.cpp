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

#include "gucefCORE_CPropertyDefinitionList.h"

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

CPropertyDefinitionList::CPropertyDefinitionList( void )
    : CIDataNodeSerializable()
    , propertyNamesAreUnique( false )
    , propertyIdsAreUnique( false )
    , properties()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPropertyDefinitionList::CPropertyDefinitionList( const CPropertyDefinitionList& src )
    : CIDataNodeSerializable( src )
    , propertyNamesAreUnique( src.propertyNamesAreUnique )
    , propertyIdsAreUnique( src.propertyIdsAreUnique )
    , properties( src.properties )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPropertyDefinitionList::~CPropertyDefinitionList()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPropertyDefinitionList& 
CPropertyDefinitionList::operator=( const CPropertyDefinitionList& src )
{GUCEF_TRACE;

    if ( &src != this )
    {   
        CIDataNodeSerializable::operator=( src );
        propertyNamesAreUnique = src.propertyNamesAreUnique;
        propertyIdsAreUnique = src.propertyIdsAreUnique;
        properties = src.properties;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool 
CPropertyDefinitionList::operator==( const CPropertyDefinitionList& other ) const
{GUCEF_TRACE;

    return propertyNamesAreUnique == other.propertyNamesAreUnique &&
           propertyIdsAreUnique == other.propertyIdsAreUnique &&
           properties == other.properties;
}

/*-------------------------------------------------------------------------*/

bool 
CPropertyDefinitionList::operator!=( const CPropertyDefinitionList& other ) const
{GUCEF_TRACE;

    return propertyNamesAreUnique != other.propertyNamesAreUnique ||
           propertyIdsAreUnique != other.propertyIdsAreUnique ||
           properties != other.properties;
}

/*-------------------------------------------------------------------------*/

bool
CPropertyDefinitionList::Serialize( CDataNode& domRootNode                        ,
                                    const CDataNodeSerializableSettings& settings ) const
{GUCEF_TRACE;

    bool totalSuccess = true;

    domRootNode.Clear();
    domRootNode.SetName( "PropertyDefinitionList" );

    totalSuccess = domRootNode.SetAttribute( "propertyNamesAreUnique", propertyNamesAreUnique ) && totalSuccess;
    totalSuccess = domRootNode.SetAttribute( "propertyIdsAreUnique", propertyIdsAreUnique ) && totalSuccess;

    CDataNode* propertiesNode = domRootNode.AddChild( "properties", GUCEF_DATATYPE_ARRAY );
    if ( GUCEF_NULL != propertiesNode )
    {
        PropertyDefinitionVector::const_iterator i = properties.begin();
        while ( i != properties.end() )
        {
            CDataNode* propertyNode = domRootNode.AddChild( "PropertyDefinition", GUCEF_DATATYPE_OBJECT );
            if ( GUCEF_NULL != propertyNode )
            {
                const CPropertyDefinition& property = (*i);
                totalSuccess = property.Serialize( *propertyNode, settings ) && totalSuccess;                
            }
            ++i;
        }        
    }

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CPropertyDefinitionList::Deserialize( const CDataNode& domRootNode                  ,
                                      const CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;

    bool totalSuccess = true;
    
    propertyNamesAreUnique = domRootNode.GetAttributeValueOrChildValueByName( "propertyNamesAreUnique", propertyNamesAreUnique ).AsBool();
    propertyIdsAreUnique = domRootNode.GetAttributeValueOrChildValueByName( "propertyIdsAreUnique", propertyIdsAreUnique ).AsBool();
    
    properties.clear();
    
    const CDataNode* propertiesNode = domRootNode.FindChild( "properties" );
    if ( GUCEF_NULL != propertiesNode )
    {
        CDataNode::TConstDataNodeSet propertyDefinitionNodes = propertiesNode->FindChildrenOfType( "PropertyDefinition" );
        CDataNode::TConstDataNodeSet::const_iterator i = propertyDefinitionNodes.begin();
        while ( i != propertyDefinitionNodes.end() )
        {
            CPropertyDefinition property;
            if ( property.Deserialize( *propertiesNode, settings ) )
            {
                properties.push_back( property );
            }
            else
            {
                totalSuccess = false;
            }
            
            ++i;
        }
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
