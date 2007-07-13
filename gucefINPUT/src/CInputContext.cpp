/*
 *  gucefINPUT: GUCEF module providing input device interaction
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

#include <assert.h>

#include "CInputContext.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace INPUT {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CInputContext::CInputContext( void )
        : m_id( 0UL )       ,
          m_handler( NULL )
{
        assert( 0 );
}

/*-------------------------------------------------------------------------*/

CInputContext::CInputContext( const CORE::CValueList& params       ,
                              CIInputHandler* handler /* = NULL */ )
        : m_valuelist( params ) ,
          m_id( 0UL )           ,
          m_handler( handler ) 
{

}

/*-------------------------------------------------------------------------*/

CInputContext::CInputContext( const CInputContext& src )
        : m_valuelist( src.m_valuelist ) ,
          m_id( 0UL )                    ,
          m_handler( src.m_handler )
{
        assert( 0 ); 
}

/*-------------------------------------------------------------------------*/

CInputContext::~CInputContext()
{
}

/*-------------------------------------------------------------------------*/

CInputContext&
CInputContext::operator=( const CInputContext& src )
{
        assert( 0 );
        return *this;
}

/*-------------------------------------------------------------------------*/

const CORE::CValueList&
CInputContext::GetContextParams( void ) const
{
        return m_valuelist;
}

/*-------------------------------------------------------------------------*/
        
void 
CInputContext::SetInputHandler( CIInputHandler* handler )
{
        m_handler = handler;
}

/*-------------------------------------------------------------------------*/
        
const CIInputHandler* 
CInputContext::GetInputHandler( void ) const
{
        return m_handler;
}

/*-------------------------------------------------------------------------*/

void 
CInputContext::SetID( const UInt32 id )
{
        m_id = id;
}

/*-------------------------------------------------------------------------*/

UInt32 
CInputContext::GetID( void ) const
{
        return m_id;
}

/*-------------------------------------------------------------------------*/

CIInputHandler*
CInputContext::GetMutableHandler( void )
{
        return m_handler;
}
       
/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
