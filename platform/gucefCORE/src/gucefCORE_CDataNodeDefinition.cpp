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

#include "gucefCORE_CDataNodeDefinition.h"

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

const CString CDataNodeDefinition::ClassTypeName = "GUCEF::CORE::CDataNodeDefinition";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CDataNodeDefinition::CDataNodeDefinition( void )
    : CIDataNodeSerializable()
    , identity()
    , attributes()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CDataNodeDefinition::CDataNodeDefinition( const CDataNodeDefinition& src )
    : CIDataNodeSerializable( src )
    , identity( src.identity )
    , attributes( src.attributes )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CDataNodeDefinition::~CDataNodeDefinition()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CDataNodeDefinition& 
CDataNodeDefinition::operator=( const CDataNodeDefinition& src )
{GUCEF_TRACE;

    if ( &src != this )
    {   
        CIDataNodeSerializable::operator=( src );
        identity = src.identity;
        attributes = src.attributes;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool 
CDataNodeDefinition::operator==( const CDataNodeDefinition& other ) const
{GUCEF_TRACE;

    return identity == other.identity &&
           attributes == other.attributes;
}

/*-------------------------------------------------------------------------*/

bool 
CDataNodeDefinition::operator!=( const CDataNodeDefinition& other ) const
{GUCEF_TRACE;

    return identity != other.identity ||
           attributes != other.attributes;
}

/*-------------------------------------------------------------------------*/

bool
CDataNodeDefinition::Serialize( CDataNode& domRootNode                        ,
                                const CDataNodeSerializableSettings& settings ) const
{GUCEF_TRACE;

    bool totalSuccess = true;

    domRootNode.Clear();
    domRootNode.SetName( "DataNodeDefinition" );

    CDataNode* nodeIdentityNode = domRootNode.AddChild( "identity", GUCEF_DATATYPE_OBJECT );
    if ( GUCEF_NULL != nodeIdentityNode )
    {
        totalSuccess = identity.Serialize( *nodeIdentityNode, settings ) && totalSuccess;
    }
    CDataNode* attributesDefinitionNode = domRootNode.AddChild( "attributes", GUCEF_DATATYPE_ARRAY );
    if ( GUCEF_NULL != attributesDefinitionNode )
    {
        totalSuccess = attributes.Serialize( *attributesDefinitionNode, settings ) && totalSuccess;
    }

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CDataNodeDefinition::Deserialize( const CDataNode& domRootNode                  ,
                                  const CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;

    bool totalSuccess = true;
    
    const CDataNode* nodeIdentityNode = domRootNode.FindChild( "identity" );
    if ( GUCEF_NULL != nodeIdentityNode )
    {
        totalSuccess = identity.Deserialize( *nodeIdentityNode, settings ) && totalSuccess;
    }
    const CDataNode* attributesDefinitionNode = domRootNode.FindChild( "attributes" );
    if ( GUCEF_NULL != attributesDefinitionNode )
    {
        totalSuccess = attributes.Deserialize( *attributesDefinitionNode, settings ) && totalSuccess;
    }

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

CICloneable* 
CDataNodeDefinition::Clone( void ) const
{GUCEF_TRACE;

    return new CDataNodeDefinition( *this );
}

/*-------------------------------------------------------------------------*/

const CString& 
CDataNodeDefinition::GetClassTypeName( void ) const
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
