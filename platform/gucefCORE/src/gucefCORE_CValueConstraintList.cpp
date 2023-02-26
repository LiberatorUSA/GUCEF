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

#include "gucefCORE_CValueConstraintList.h"

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

const CString CValueConstraintList::ClassTypeName = "GUCEF::CORE::CValueConstraintList";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CValueConstraintList::CValueConstraintList( void )
    : CIDataNodeSerializable()
    , m_constraints()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CValueConstraintList::CValueConstraintList( const CValueConstraintList& src )
    : CIDataNodeSerializable( src )
    , m_constraints( src.m_constraints )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CValueConstraintList::~CValueConstraintList()
{GUCEF_TRACE;

    m_constraints.clear();
}

/*-------------------------------------------------------------------------*/

CValueConstraintList::ValueConstraintVector&
CValueConstraintList::GetConstraints( void )
{GUCEF_TRACE;

    return m_constraints;
}

/*-------------------------------------------------------------------------*/

const CValueConstraintList::ValueConstraintVector&
CValueConstraintList::GetConstraints( void ) const
{GUCEF_TRACE;

    return m_constraints;
}

/*-------------------------------------------------------------------------*/

bool 
CValueConstraintList::AreConstraintsSatisfiedBy( const CVariant& valueToConstrain ) const
{GUCEF_TRACE;

    ValueConstraintVector::const_iterator i = m_constraints.begin();
    while ( i != m_constraints.end() )
    {
        if ( !(*i).IsConstraintSatisfiedBy( valueToConstrain ) )
            return false;
        ++i;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CValueConstraintList::operator==( const CValueConstraintList& other ) const
{GUCEF_TRACE;

    return m_constraints == other.m_constraints;
}

/*-------------------------------------------------------------------------*/

bool 
CValueConstraintList::operator!=( const CValueConstraintList& other ) const
{GUCEF_TRACE;

    return m_constraints != other.m_constraints;
}

/*-------------------------------------------------------------------------*/

bool
CValueConstraintList::Serialize( CDataNode& domRootNode                        ,
                                 const CDataNodeSerializableSettings& settings ) const
{GUCEF_TRACE;

    bool totalSuccess = true;

    domRootNode.Clear();
    domRootNode.SetName( "ValueConstraintList" );
    domRootNode.SetNodeType( GUCEF_DATATYPE_ARRAY );

    ValueConstraintVector::const_iterator i = m_constraints.begin();
    while ( i != m_constraints.end() )
    {
        CDataNode* valueConstraintNode = domRootNode.AddChild( "ValueConstraint", GUCEF_DATATYPE_OBJECT );
        if ( GUCEF_NULL != valueConstraintNode )
        {
            totalSuccess = (*i).Serialize( *valueConstraintNode, settings ) && totalSuccess;
        }
        ++i;
    }

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CValueConstraintList::Deserialize( const CDataNode& domRootNode                  ,
                                   const CDataNodeSerializableSettings& settings ) 
{GUCEF_TRACE;

    bool totalSuccess = true;

    CDataNode::TConstDataNodeSet valueConstraintNodes = domRootNode.FindChildrenOfType( "ValueConstraint", false );
    CDataNode::TConstDataNodeSet::const_iterator i = valueConstraintNodes.begin();
    while ( i != valueConstraintNodes.end() )
    {
        const CDataNode* valueConstraintNode = (*i);
        if ( GUCEF_NULL != valueConstraintNode )
        {
            CValueConstraint newConstraint;
            if ( newConstraint.Deserialize( *valueConstraintNode, settings ) )
            {
                m_constraints.push_back( newConstraint );
            }
            else
            {
                totalSuccess = false;
            }
        }
        ++i;
    }

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

CICloneable* 
CValueConstraintList::Clone( void ) const
{GUCEF_TRACE;

    return new CValueConstraintList( *this );
}

/*-------------------------------------------------------------------------*/

const CString& 
CValueConstraintList::GetClassTypeName( void ) const
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
