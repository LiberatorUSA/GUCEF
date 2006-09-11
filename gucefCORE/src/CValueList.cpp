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

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#include "CValueList.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

CValueList::CValueList( void )
{TRACE;
}

/*-------------------------------------------------------------------------*/

CValueList::CValueList( const CValueList& src )
{TRACE;

        CORE::CString value, key;
        for ( UInt32 i=0; i<src.GetCount(); ++i )
        {
                value = src.GetValue( i );
                key = src.GetKey( i );
                
                Set( key   ,
                     value );
        }                
}

/*-------------------------------------------------------------------------*/

CValueList::~CValueList()
{TRACE;
}

/*-------------------------------------------------------------------------*/

CValueList&
CValueList::operator=( const CValueList& src )
{TRACE;
        if ( this != &src )
        {
                DeleteAll();

                CORE::CString value, key;
                for ( UInt32 i=0; i<src.GetCount(); ++i )
                {
                        value = src.GetValue( i );
                        key = src.GetKey( i );
                        
                        Set( key   ,
                             value );
                }              
        }
        return *this;
}

/*-------------------------------------------------------------------------*/

void 
CValueList::Set( const CString& keyandvalue )
{TRACE;
        CString key( keyandvalue.SubstrToChar( '=', true ) );
        CString value( keyandvalue.SubstrToChar( '=', false ) );
        
        Set( key   ,
             value );
}

/*-------------------------------------------------------------------------*/

void 
CValueList::Set( const CString& key   ,
                 const CString& value )
{TRACE;
        CString* thevalue = static_cast<CString*>( m_list.Get( key ) );
        if ( thevalue )
        {
                *thevalue = value;
                return;
        }
        
        m_list.Add( key                  ,
                    new CString( value ) );        
}

/*-------------------------------------------------------------------------*/                
        
CString
CValueList::GetValue( const CString& key ) const
{TRACE;
        const CString* thevalue = static_cast<const CString*>( m_list[ key ] );
        if ( thevalue )
        {
                return *thevalue;
        }
        
        CString emptystr;
        return emptystr;       
}

/*-------------------------------------------------------------------------*/

CString 
CValueList::GetValue( const UInt32 index ) const
{TRACE;
        const CString* thevalue = static_cast<const CString*>( m_list[ index ] );
        if ( thevalue )
        {
                return *thevalue;
        }
        
        CString emptystr;
        return emptystr;        
}

/*-------------------------------------------------------------------------*/

CString 
CValueList::GetPair( const UInt32 index ) const
{TRACE;
        const CString* thevalue = static_cast<const CString*>( m_list[ index ] );
        if ( thevalue )
        {
                CString pair( m_list.GetKey( index ) );
                pair += '=';
                pair += *thevalue;
                return pair;
        }
        
        CString emptypair( "=" );
        return emptypair;
}

/*-------------------------------------------------------------------------*/

CString 
CValueList::GetPair( const CString& key ) const
{TRACE;
        const CString* thevalue = static_cast<const CString*>( m_list.Get( key ) );
        if ( thevalue )
        {
                CString pair( key );
                pair += '=';
                pair += *thevalue;
                return pair;
        }
        
        CString emptypair( "=" );
        return emptypair; 
}

/*-------------------------------------------------------------------------*/

CString 
CValueList::GetKey( const UInt32 index ) const
{TRACE;
        return m_list.GetKey( index );
}

/*-------------------------------------------------------------------------*/

bool 
CValueList::Exists( const CString& key ) const
{TRACE;
        return NULL != static_cast<const CString*>( m_list.Get( key ) );
}

/*-------------------------------------------------------------------------*/
        
void 
CValueList::Delete( const CString& key )
{TRACE;
        CString* thevalue = static_cast<CString*>( m_list.Get( key ) );
        if ( thevalue )
        {
                delete thevalue;
                m_list.Delete( key );
        }
}

/*-------------------------------------------------------------------------*/

void 
CValueList::DeleteAll( void )
{TRACE;
        for ( UInt32 i=0; i<m_list.GetCount(); ++i )
        {
                delete static_cast<CString*>( m_list[ i ] );
        }
        m_list.DeleteAll();
}

/*-------------------------------------------------------------------------*/

UInt32 
CValueList::GetCount( void ) const
{TRACE;
        return m_list.GetCount();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
