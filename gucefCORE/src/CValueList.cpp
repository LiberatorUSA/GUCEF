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
{GUCEF_TRACE;
}

/*-------------------------------------------------------------------------*/

CValueList::CValueList( const CValueList& src )
{GUCEF_TRACE;

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
{GUCEF_TRACE;
}

/*-------------------------------------------------------------------------*/

CValueList&
CValueList::operator=( const CValueList& src )
{GUCEF_TRACE;
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

CString
CValueList::operator[]( const CString& key ) const
{GUCEF_TRACE;

    return GetValue( key );
}

/*-------------------------------------------------------------------------*/

void
CValueList::SetMultiple( const CString& keyandvalue ,
                         const char seperator       )
{GUCEF_TRACE;

    CString remnant = keyandvalue;
    while ( remnant.Length() > 0 )
    {
        CString tmp = remnant.SubstrToChar( seperator, true );
        if ( ( tmp.Length() > 0 )          || 
             ( remnant[ 0 ] == seperator )  )
        {
            remnant = remnant.CutChars( tmp.Length()+1, true );
            
            CString keyValueStr = remnant.SubstrToChar( seperator, true );
            if ( keyValueStr.Length() > 0 )
            {
                Set( keyValueStr );
                remnant = remnant.CutChars( keyValueStr.Length()+1, true );
            }
            else
            {
                return;
            }
        }
        else
        {
            return;
        }
    }
}

/*-------------------------------------------------------------------------*/

void 
CValueList::Set( const CString& keyandvalue )
{GUCEF_TRACE;
        CString key( keyandvalue.SubstrToChar( '=', true ) );
        CString value( keyandvalue.SubstrToChar( '=', false ) );
        
        Set( key   ,
             value );
}

/*-------------------------------------------------------------------------*/

void 
CValueList::Set( const CString& key   ,
                 const CString& value )
{GUCEF_TRACE;
        
    if ( key.Length() > 0 )
    {
        CString* thevalue = static_cast<CString*>( m_list.Get( key ) );
        if ( thevalue )
        {
                *thevalue = value;
                return;
        }
        
        m_list.Add( key                  ,
                    new CString( value ) );
    }
}

/*-------------------------------------------------------------------------*/                
        
CString
CValueList::GetValue( const CString& key ) const
{GUCEF_TRACE;
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
{GUCEF_TRACE;
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
{GUCEF_TRACE;
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
{GUCEF_TRACE;
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
{GUCEF_TRACE;
        return m_list.GetKey( index );
}

/*-------------------------------------------------------------------------*/

bool 
CValueList::HasKey( const CString& key ) const
{GUCEF_TRACE;
        return NULL != static_cast<const CString*>( m_list.Get( key ) );
}

/*-------------------------------------------------------------------------*/
        
void 
CValueList::Delete( const CString& key )
{GUCEF_TRACE;
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
{GUCEF_TRACE;
        for ( UInt32 i=0; i<m_list.GetCount(); ++i )
        {
                delete static_cast<CString*>( m_list[ i ] );
        }
        m_list.DeleteAll();
}

/*-------------------------------------------------------------------------*/

UInt32 
CValueList::GetCount( void ) const
{GUCEF_TRACE;
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
