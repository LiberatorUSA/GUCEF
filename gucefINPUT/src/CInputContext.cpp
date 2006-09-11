/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
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
