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

#include "gucefCORE_CFunctionParamMeta.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENATION                                                      //
//                                                                         //
//-------------------------------------------------------------------------*/

CFunctionParamMeta::CFunctionParamMeta( void )
    : m_paramMeta()
    , m_paramName()
    , m_defaultValue( GUCEF_NULL )
{GUCEF_TRACE;

    memset( &m_paramMeta, 0, sizeof m_paramMeta );
}

/*-------------------------------------------------------------------------*/

CFunctionParamMeta::CFunctionParamMeta( const CFunctionParamMeta& src )
    : m_paramMeta( src.m_paramMeta )
    , m_paramName( src.m_paramName )
    , m_defaultValue( GUCEF_NULL )
{GUCEF_TRACE;

    m_paramMeta.paramName = m_paramName.C_String();
    m_paramMeta.defaultValue = GUCEF_NULL;
    if ( GUCEF_NULL != src.m_defaultValue )
    {
        SetDefaultValue( *src.m_defaultValue );
    }
}

/*-------------------------------------------------------------------------*/

CFunctionParamMeta::CFunctionParamMeta( const TFunctionParamMeta* c_style_param_meta )
    : m_paramMeta()
    , m_paramName()
    , m_defaultValue( GUCEF_NULL )
{GUCEF_TRACE;

    if ( GUCEF_NULL != c_style_param_meta )
    {
        m_paramMeta = *c_style_param_meta;
        m_paramName.Set( c_style_param_meta->paramName );
        m_paramMeta.paramName = m_paramName.C_String();
        if ( GUCEF_NULL != c_style_param_meta->defaultValue )
        {
            CVariant defaultValue;
            defaultValue.LinkTo( c_style_param_meta->defaultValue );
            SetDefaultValue( defaultValue );
        }
    }
    else
    {
        memset( &m_paramMeta, 0, sizeof m_paramMeta );
    }    
}

/*-------------------------------------------------------------------------*/

CFunctionParamMeta::~CFunctionParamMeta()
{GUCEF_TRACE;

    SetDefaultValue( CVariant::Empty );
}

/*-------------------------------------------------------------------------*/

CFunctionParamMeta& 
CFunctionParamMeta::operator=( const CFunctionParamMeta& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        m_paramMeta = src.m_paramMeta;
        m_paramName = src.m_paramName;
        m_paramMeta.paramName = m_paramName.C_String();
    }

    return *this;
}

/*-------------------------------------------------------------------------*/

void 
CFunctionParamMeta::SetParamType( TFunctionParamType paramType )
{GUCEF_TRACE;
    
    m_paramMeta.paramType = paramType;
}

/*-------------------------------------------------------------------------*/

TFunctionParamType 
CFunctionParamMeta::GetParamType( void ) const
{GUCEF_TRACE;
    
    return m_paramMeta.paramType;
}

/*-------------------------------------------------------------------------*/

void 
CFunctionParamMeta::SetParamDataType( UInt8 paramDataType )
{GUCEF_TRACE;

    m_paramMeta.paramDataType = paramDataType;
}

/*-------------------------------------------------------------------------*/

UInt8 
CFunctionParamMeta::GetParamDataType( void ) const
{GUCEF_TRACE;
    
    return m_paramMeta.paramDataType;
}

/*-------------------------------------------------------------------------*/

void 
CFunctionParamMeta::SetIsParamConst( bool isParamConst )
{GUCEF_TRACE;

    m_paramMeta.isParamConst = isParamConst ? 1 : 0;
}

/*-------------------------------------------------------------------------*/

bool 
CFunctionParamMeta::GetIsParamConst( void ) const
{GUCEF_TRACE;

    return m_paramMeta.isParamConst != 0;
}

/*-------------------------------------------------------------------------*/

bool 
CFunctionParamMeta::GetIsParamOptional( void ) const
{GUCEF_TRACE;

    return m_paramMeta.defaultValue != GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

void 
CFunctionParamMeta::SetDefaultValue( const CVariant& defaultValue )
{GUCEF_TRACE;

    if ( defaultValue != CVariant::Empty && defaultValue.GetTypeId() != GUCEF_DATATYPE_UNKNOWN )
    {
        if ( GUCEF_NULL == m_defaultValue )
        {
            m_defaultValue = new CVariant( defaultValue );
        }
        else
        {
            *m_defaultValue = defaultValue;
        }    
        m_paramMeta.defaultValue = m_defaultValue->CStyleAccess();
    }
    else
    {
        ClearDefaultValue();
    }
}

/*-------------------------------------------------------------------------*/

const CVariant&
CFunctionParamMeta::GetDefaultValue( void ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_defaultValue )
    {
        return *m_defaultValue;
    }
    return CVariant::Empty;
}

/*-------------------------------------------------------------------------*/

void 
CFunctionParamMeta::ClearDefaultValue( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_defaultValue )
    {
        delete m_defaultValue;
        m_defaultValue = GUCEF_NULL;
    }
    m_paramMeta.defaultValue = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

void 
CFunctionParamMeta::SetParamName( const CString& paramName )
{GUCEF_TRACE;

    m_paramName = paramName;
    m_paramMeta.paramName = m_paramName.C_String();
}

/*-------------------------------------------------------------------------*/

const CString& 
CFunctionParamMeta::GetParamName( void ) const
{GUCEF_TRACE;

    return m_paramName;
}

/*-------------------------------------------------------------------------*/

const TFunctionParamMeta* 
CFunctionParamMeta::GetCStyleParamMeta( void ) const
{GUCEF_TRACE;
    
    return &m_paramMeta;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
