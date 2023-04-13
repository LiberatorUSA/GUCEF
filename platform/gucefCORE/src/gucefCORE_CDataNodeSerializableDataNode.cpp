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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "gucefCORE_CDataNodeSerializableDataNode.h"

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

const CString CDataNodeSerializableDataNode::ClassTypeName = "GUCEF::CORE::CDataNodeSerializableDataNode";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CDataNodeSerializableDataNode::CDataNodeSerializableDataNode( void )
    : CDataNode()
    , CIDataNodeSerializable()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CDataNodeSerializableDataNode::CDataNodeSerializableDataNode( const CDataNodeSerializableDataNode& src )
    : CDataNode( src )
    , CIDataNodeSerializable( src )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CDataNodeSerializableDataNode::~CDataNodeSerializableDataNode()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CDataNodeSerializableDataNode& 
CDataNodeSerializableDataNode::operator=( const CDataNodeSerializableDataNode& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        CDataNode::operator=( src );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool 
CDataNodeSerializableDataNode::Serialize( CDataNode& domRootNode                        ,
                                          const CDataNodeSerializableSettings& settings ) const
{GUCEF_TRACE;

    domRootNode = *this;
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CDataNodeSerializableDataNode::Deserialize( const CDataNode& domRootNode                  ,
                                            const CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;

    CDataNode::operator=( domRootNode );
    return true;
}

/*-------------------------------------------------------------------------*/

CICloneable* 
CDataNodeSerializableDataNode::Clone( void ) const
{GUCEF_TRACE;

    return new CDataNodeSerializableDataNode( *this );
}

/*-------------------------------------------------------------------------*/

const CString& 
CDataNodeSerializableDataNode::GetClassTypeName( void ) const
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
