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

#include "CIntMap.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE { 

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

struct SIntMapEntry
{
        Int32 key;
        void* ptr;
};
typedef struct SIntMapEntry TIntMapEntry;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CIntMap::CIntMap( void )
{
}

/*-------------------------------------------------------------------------*/
        
CIntMap::CIntMap( const CIntMap& src )
{
        /*
         *      No need to init anything so simply call the assignment
         *      operator 
         */
        *this = src;
}

/*-------------------------------------------------------------------------*/
                
CIntMap::~CIntMap()
{
        DeleteAll();
}

/*-------------------------------------------------------------------------*/
        
CIntMap& 
CIntMap::operator=( const CIntMap& src )
{
        if ( this != &src )
        {        
                DeleteAll();
                
                TIntMapEntry* entry;
                const TIntMapEntry* srcentry;
                for ( UInt32 i=0; i<src.m_items.GetCount(); ++i )
                {
                        srcentry = static_cast<const TIntMapEntry*>( src.m_items[ i ] );
                        entry = new TIntMapEntry;
                        *entry = *srcentry;
                        m_items.AppendEntry( entry );
                }                
        }
        return *this;                               
}

/*-------------------------------------------------------------------------*/

void* 
CIntMap::operator[]( Int32 key )
{
        return Get( key );
}

/*-------------------------------------------------------------------------*/

bool 
CIntMap::Exists( const Int32 key ) const
{
        if ( m_items.GetCount() )
        {
                UInt32 i( 0UL );
                Int32 curkey;
                do
                {
                        curkey = static_cast<TIntMapEntry*>( m_items[ i ] )->key;
                        if ( curkey == key )
                        {
                                return true;
                        }
                        ++i;
                }
                while ( ( i <= m_items.GetCount() ) &&
                        ( key < curkey )             );
        }                
        return false;                
}

/*-------------------------------------------------------------------------*/
        
void* 
CIntMap::Get( const Int32 key ) const
{
        TIntMapEntry* entry;
        for ( UInt32 i=0; i<m_items.GetCount(); ++i )
        {
                entry = static_cast<TIntMapEntry*>( m_items[ i ] );
                if ( entry->key == key )
                {
                        return entry->ptr;
                }
                if ( entry->key > key ) 
                {
                        return NULL;
                }                                                        
        }
        return NULL;
}

/*-------------------------------------------------------------------------*/

void* 
CIntMap::GetAtIndex( const UInt32 index ) const
{        
        return static_cast<TIntMapEntry*>( m_items[ index ] )->ptr;
}

/*-------------------------------------------------------------------------*/

bool 
CIntMap::Add( const Int32 key  ,
              void* linkeddata )
{
        if ( !Exists( key ) )
        {                
                Int32 curkey;
                for ( UInt32 i=0; i<m_items.GetCount(); ++i )
                {
                        curkey = static_cast<TIntMapEntry*>( m_items[ i ] )->key;
                        if ( curkey > key )
                        {
                                TIntMapEntry* entry = new TIntMapEntry;
                                entry->key = key;
                                entry->ptr = linkeddata;
                                return m_items.Insert( i, entry );        
                        }
                }
                TIntMapEntry* entry = new TIntMapEntry;
                entry->key = key;
                entry->ptr = linkeddata;                
                m_items.AppendEntry( entry );
                return true;                
        }
        return false;                
}

/*-------------------------------------------------------------------------*/              
                  
void 
CIntMap::Delete( const Int32 key )
{
        TIntMapEntry* entry;
        for ( UInt32 i=0; i<m_items.GetCount(); ++i )
        {
                entry = static_cast<TIntMapEntry*>( m_items[ i ] );
                if ( entry->key == key )
                {
                        delete entry;
                        m_items.RemoveEntry( i );
                        return;
                }
                if ( entry->key > key ) 
                {
                        return;
                }                                                        
        }
}

/*-------------------------------------------------------------------------*/

void 
CIntMap::DeleteAtIndex( const UInt32 index )
{
        delete static_cast<TIntMapEntry*>( m_items[ index ] );        
        m_items.RemoveEntry( index );
}

/*-------------------------------------------------------------------------*/
        
void 
CIntMap::DeleteAll( void )
{
        for ( UInt32 i=0; i<m_items.GetCount(); ++i )
        {
                delete static_cast<TIntMapEntry*>( m_items[ i ] );                
        }        
        m_items.Clear();
}

/*-------------------------------------------------------------------------*/
        
UInt32
CIntMap::Count( void ) const
{
        return m_items.GetCount();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
