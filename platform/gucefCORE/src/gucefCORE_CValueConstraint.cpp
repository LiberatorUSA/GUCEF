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

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#include "gucefCORE_CValueConstraint.h"

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

CValueConstraint::CValueConstraint( const CString& functionName           ,
                                    const CVariant::VariantVector& params ,
                                    Int32 constrainedValueParamIndex      ,
                                    CFunctionRegistry* functionRegistry   ,
                                    bool passIfFunctionExecIssue          ,
                                    UInt32 constraintResultIndex          )
    : CIDataNodeSerializable()
    , m_functionName()
    , m_function( GUCEF_NULL )
    , m_params()
    , m_constrainedValueParamIndex( -1 )
    , m_passIfFunctionExecIssue( passIfFunctionExecIssue )
    , m_functionResults()
    , m_constraintResultIndex( constraintResultIndex )
{GUCEF_TRACE;

    SetValueConstraint( functionName               ,
                        params                     ,
                        constrainedValueParamIndex ,
                        functionRegistry           );
}

/*-------------------------------------------------------------------------*/

CValueConstraint::CValueConstraint( CIFunction* function                  ,
                                    const CVariant::VariantVector& params ,
                                    Int32 constrainedValueParamIndex      ,
                                    bool passIfFunctionExecIssue          ,
                                    UInt32 constraintResultIndex          )
    : CIDataNodeSerializable()
    , m_functionName()
    , m_function( GUCEF_NULL )
    , m_params()
    , m_constrainedValueParamIndex( -1 )
    , m_passIfFunctionExecIssue( passIfFunctionExecIssue )
    , m_functionResults()
    , m_constraintResultIndex( constraintResultIndex )
{GUCEF_TRACE;

    SetValueConstraint( function                   ,
                        params                     ,
                        constrainedValueParamIndex );
}

/*-------------------------------------------------------------------------*/

CValueConstraint::CValueConstraint( void )
    : CIDataNodeSerializable()
    , m_functionName()
    , m_function( GUCEF_NULL )
    , m_params()
    , m_constrainedValueParamIndex( -1 )
    , m_passIfFunctionExecIssue( true )
    , m_functionResults()
    , m_constraintResultIndex( 0 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CValueConstraint::CValueConstraint( const CValueConstraint& src )
    : CIDataNodeSerializable( src )
    , m_functionName( src.m_functionName )
    , m_function( src.m_function )
    , m_params( src.m_params )
    , m_constrainedValueParamIndex( src.m_constrainedValueParamIndex )
    , m_passIfFunctionExecIssue( src.m_passIfFunctionExecIssue )
    , m_functionResults()
    , m_constraintResultIndex( src.m_constraintResultIndex )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CValueConstraint::~CValueConstraint()
{GUCEF_TRACE;

    m_function = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

bool
CValueConstraint::IsConstraintSatisfiedBy( const CVariant& valueToConstrain ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_function )
    {
        if ( m_constrainedValueParamIndex >= 0 )
        {
            m_params[ m_constrainedValueParamIndex ].LinkTo( valueToConstrain );
        }
        
        if ( m_function->ExecuteFunction( m_params              , 
                                          CVariant::EmptyVector , 
                                          m_functionResults     ) )
        {
            if ( m_constraintResultIndex < m_functionResults.size() )
                return m_functionResults[ m_constraintResultIndex ].AsBool( m_passIfFunctionExecIssue );
            return m_passIfFunctionExecIssue;
        }
    }
    return m_passIfFunctionExecIssue;
}

/*-------------------------------------------------------------------------*/
        
bool 
CValueConstraint::SetValueConstraint( const CString& functionName           ,
                                      const CVariant::VariantVector& params ,
                                      Int32 constrainedValueParamIndex      ,
                                      CFunctionRegistry* functionRegistry   ,
                                      bool passIfFunctionExecIssue          ,
                                      UInt32 constraintResultIndex          )
{GUCEF_TRACE;

    if ( GUCEF_NULL == functionRegistry )
        functionRegistry = &CCoreGlobal::Instance()->GetFunctionRegistry();

    if ( GUCEF_NULL != functionRegistry )
    {
        CIFunction* function = functionRegistry->GetFunctionByName( functionName );
        if ( GUCEF_NULL != function )
        {
            m_params = params;
            m_constrainedValueParamIndex = constrainedValueParamIndex;
            m_passIfFunctionExecIssue = passIfFunctionExecIssue;
            m_constraintResultIndex = constraintResultIndex;
            return true;
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CValueConstraint::SetValueConstraint( CIFunction* function                  ,
                                      const CVariant::VariantVector& params ,
                                      Int32 constrainedValueParamIndex      ,
                                      bool passIfFunctionExecIssue          ,
                                      UInt32 constraintResultIndex          )
{GUCEF_TRACE;

    if ( GUCEF_NULL != function )
    {
        m_functionName = function->GetName();
        m_function = function;
        m_params = params;
        m_constrainedValueParamIndex = constrainedValueParamIndex;
        m_passIfFunctionExecIssue = passIfFunctionExecIssue;
        m_constraintResultIndex = constraintResultIndex;
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CValueConstraint::operator==( const CValueConstraint& other ) const
{GUCEF_TRACE;

    return m_functionName == other.m_functionName &&
           m_params == other.m_params &&
           m_constrainedValueParamIndex == other.m_constrainedValueParamIndex &&
           m_passIfFunctionExecIssue == other.m_passIfFunctionExecIssue &&
           m_constraintResultIndex == other.m_constraintResultIndex;
}

/*-------------------------------------------------------------------------*/

bool 
CValueConstraint::operator!=( const CValueConstraint& other ) const
{GUCEF_TRACE;

    return m_functionName != other.m_functionName ||
           m_params != other.m_params ||
           m_constrainedValueParamIndex != other.m_constrainedValueParamIndex ||
           m_passIfFunctionExecIssue != other.m_passIfFunctionExecIssue ||
           m_constraintResultIndex != other.m_constraintResultIndex;
}

/*-------------------------------------------------------------------------*/

bool
CValueConstraint::Serialize( CDataNode& domRootNode                        ,
                             const CDataNodeSerializableSettings& settings ) const
{GUCEF_TRACE;

    bool totalSuccess = true;

    domRootNode.Clear();
    domRootNode.SetName( "ValueConstraint" );
    totalSuccess = domRootNode.SetAttribute( "functionName", m_functionName ) && totalSuccess;
    totalSuccess = domRootNode.SetAttribute( "constrainedValueParamIndex", m_constrainedValueParamIndex ) && totalSuccess;
    totalSuccess = domRootNode.SetAttribute( "constraintResultIndex", m_constraintResultIndex ) && totalSuccess;
    totalSuccess = domRootNode.SetAttribute( "passIfFunctionExecIssue", m_passIfFunctionExecIssue ) && totalSuccess;
        
    CDataNode* params = domRootNode.AddChild( "params", GUCEF_DATATYPE_ARRAY );
    if ( GUCEF_NULL != params )
    {
        //totalSuccess
    }
    else
    {
        totalSuccess = false;
    }
    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool 
CValueConstraint::Deserialize( const CDataNode& domRootNode                  ,
                               const CDataNodeSerializableSettings& settings )
{GUCEF_TRACE;


    m_functionName = domRootNode.GetAttributeValueOrChildValueByName( "functionName", m_functionName ).AsString();
    m_constrainedValueParamIndex = domRootNode.GetAttributeValueOrChildValueByName( "constrainedValueParamIndex", m_constrainedValueParamIndex ).AsInt32();
    m_constraintResultIndex = domRootNode.GetAttributeValueOrChildValueByName( "constraintResultIndex", m_constraintResultIndex ).AsUInt32();
    m_passIfFunctionExecIssue = domRootNode.GetAttributeValueOrChildValueByName( "passIfFunctionExecIssue", m_passIfFunctionExecIssue ).AsBool();
        
    CDataNode* paramsNode = domRootNode.FindChild( "params" );
    if ( GUCEF_NULL != paramsNode )
    {
        //paramsNode
        
    }
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
