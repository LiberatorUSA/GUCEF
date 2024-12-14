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

#include "gucefCORE_CFunctionMeta.h"

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

CFunctionMeta::CFunctionMeta( void )
    : m_functionMeta()
    , m_paramsMeta()
    , m_functionName()
{GUCEF_TRACE;

    memset( &m_functionMeta, 0, sizeof m_functionMeta );
}

/*-------------------------------------------------------------------------*/

CFunctionMeta::CFunctionMeta( const CFunctionMeta& src )
    : m_functionMeta()
    , m_paramsMeta( src.m_paramsMeta )
    , m_functionName( src.m_functionName )
{GUCEF_TRACE;

    memset( &m_functionMeta, 0, sizeof m_functionMeta );
    m_functionMeta.functionName = m_functionName.C_String();
}

/*-------------------------------------------------------------------------*/

CFunctionMeta::CFunctionMeta( const TFunctionDefinition* c_style_function_meta )
    : m_functionMeta()
    , m_paramsMeta()
    , m_functionName()
{GUCEF_TRACE;

    if ( GUCEF_NULL != c_style_function_meta )
    {
        m_functionMeta = *c_style_function_meta;
        m_functionName.Set( c_style_function_meta->functionName );
        m_functionMeta.functionName = m_functionName.C_String();
    }
    else
    {
        memset( &m_functionMeta, 0, sizeof m_functionMeta );
    }    
}

/*-------------------------------------------------------------------------*/

CFunctionMeta::~CFunctionMeta()
{GUCEF_TRACE;

    memset( &m_functionMeta, 0, sizeof m_functionMeta );
    m_functionName.Clear();
    m_paramsMeta.clear();    
}

/*-------------------------------------------------------------------------*/

CFunctionMeta& 
CFunctionMeta::operator=( const CFunctionMeta& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        m_functionMeta = src.m_functionMeta;
        m_functionName = src.m_functionName;
        m_functionMeta.functionName = m_functionName.C_String();

        m_paramsMeta = src.m_paramsMeta;
    }

    return *this;
}

/*-------------------------------------------------------------------------*/

void 
CFunctionMeta::SetParams( const TFunctionParamMetaVector& params )
{GUCEF_TRACE;
    
    m_paramsMeta = params;
}

/*-------------------------------------------------------------------------*/

const CFunctionMeta::TFunctionParamMetaVector& 
CFunctionMeta::GetParams( void ) const
{GUCEF_TRACE;
    
    return m_paramsMeta;
}

/*-------------------------------------------------------------------------*/

void 
CFunctionMeta::SetIsFunctionConst( bool isFunctionConst )
{GUCEF_TRACE;

    m_functionMeta.isFunctionConst = isFunctionConst ? 1 : 0;
}

/*-------------------------------------------------------------------------*/

bool 
CFunctionMeta::GetIsFunctionConst( void ) const
{GUCEF_TRACE;

    return m_functionMeta.isFunctionConst != 0;
}

/*-------------------------------------------------------------------------*/

void 
CFunctionMeta::SetFunctionName( const CString& functionName )
{GUCEF_TRACE;

    m_functionName = functionName;
    m_functionMeta.functionName = functionName.C_String();
}

/*-------------------------------------------------------------------------*/

const CString& 
CFunctionMeta::GetFunctionName( void ) const
{GUCEF_TRACE;

    return m_functionName;
}

/*-------------------------------------------------------------------------*/

const TFunctionDefinition* 
CFunctionMeta::GetCStyleFunctionMeta( void ) const
{GUCEF_TRACE;
    
    return &m_functionMeta;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
