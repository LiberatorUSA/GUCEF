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

#ifndef GUCEF_MT_CSCOPERWLOCK_H
#include "gucefMT_CScopeRwLock.h"
#define GUCEF_MT_CSCOPERWLOCK_H
#endif /* GUCEF_MT_CSCOPERWLOCK_H ? */

#ifndef GUCEF_CORE_BASICFUNCTIONS_H
#include "gucefCORE_BasicFunctions.h"
#define GUCEF_CORE_BASICFUNCTIONS_H
#endif /* GUCEF_CORE_BASICFUNCTIONS_H ? */

#include "gucefCORE_CFunctionRegistry.h"

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

CGreaterFunction            g_greaterFunction;
CGreaterEqualsFunction      g_greaterEqualsFunction;
CIsNullOrEmptyFunction      g_isNullOrEmptyFunction;
CIsOfTypeFunction           g_isOfTypeFunction;
CAsTypeFunction             g_asTypeFunction;

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CFunctionRegistry::CFunctionRegistry( void )
    : m_functions()
    , m_lock( true )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CFunctionRegistry::CFunctionRegistry( const CFunctionRegistry& src )
    : m_functions()
    , m_lock( true )
{GUCEF_TRACE;

    MT::CScopeReaderLock lock( src.m_lock );
    m_functions = src.m_functions;
}

/*-------------------------------------------------------------------------*/

CFunctionRegistry::~CFunctionRegistry()
{GUCEF_TRACE;

    m_functions.clear();
}

/*-------------------------------------------------------------------------*/

void 
CFunctionRegistry::RegisterBasicBuildInFunctions( void )
{GUCEF_TRACE;
    
    MT::CScopeWriterLock lock( m_lock );
    m_functions[ g_greaterFunction.GetName() ] = &g_greaterFunction;
    m_functions[ g_greaterEqualsFunction.GetName() ] = &g_greaterEqualsFunction;
    
    m_functions[ g_isNullOrEmptyFunction.GetName() ] = &g_isNullOrEmptyFunction;
    m_functions[ g_isOfTypeFunction.GetName() ] = &g_isOfTypeFunction;
    m_functions[ g_asTypeFunction.GetName() ] = &g_asTypeFunction;
}

/*-------------------------------------------------------------------------*/

CFunctionRegistry&
CFunctionRegistry::operator=( const CFunctionRegistry& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
    }    
    return *this;
}

/*-------------------------------------------------------------------------*/

CIFunction* 
CFunctionRegistry::GetFunctionByName( const CString& functionName ) const
{GUCEF_TRACE;

    MT::CScopeReaderLock lock( m_lock );
    TStringToFunctionMap::const_iterator i = m_functions.find( functionName );
    if ( i != m_functions.end() )
        return (*i).second;

    return GUCEF_NULL;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
