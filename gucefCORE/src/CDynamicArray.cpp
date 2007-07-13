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

#include <malloc.h>		/* memory management */
#include <string.h>             /* needed for memmove() */
/* #include <mem.h> */               /* needed for memmove() */

#include "CDynamicArray.h"	/* definition of this class */

#if !defined( TSPRINTING_H ) && defined( GUCEF_CORE_DEBUG_MODE )
#include "tsprinting.h"		  /* threadsafe printing */ 
#endif /* TSPRINTING_H ? */

#ifndef GUCEF_CORE_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_ESSENTIALS_H
#endif /* GUCEF_CORE_ESSENTIALS_H ? */  

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CDynamicArray::CDynamicArray( UInt32 initial_size )
{GUCEF_TRACE;

        /*
         *	Constructor, initialize array to size given
         */
        _max = initial_size;
        _entrys = ( void** ) malloc( _max*sizeof( void* ) );
        CHECKMEM( _entrys, _max*sizeof( void* ) ); 
        _last = -1;
        _resize_change = 1;
}

/*-------------------------------------------------------------------------*/

CDynamicArray::CDynamicArray( void )
{GUCEF_TRACE;

        /*
         *	creates an array with size 0
         */
        _entrys = NULL;
        _max = 0;
        _last = -1;
        _resize_change = 1;
}

/*-------------------------------------------------------------------------*/

CDynamicArray::~CDynamicArray()
{
        GUCEF_BEGIN;

        /*
         *	destructor, deallocates the array
         */
        free( _entrys );
        GUCEF_END; 
}

/*-------------------------------------------------------------------------*/

void
CDynamicArray::SetEntry( UInt32 index    ,
                         void *new_value )
{GUCEF_TRACE;

        /*
         *	Set's the pointer of the entry with the index given to
         *	new_value.
         */               
        CHECKMEMSEG( _entrys, _entrys+index, sizeof(void*) );
        
        if ( index >= _max )
        {
                SetArraySize( index+1 );
        }
        
        _entrys[ index ] = new_value;
        if ( (Int32)index > _last ) 
        {
                _last = index;
        }                
}

/*-------------------------------------------------------------------------*/

/**
 *      Get's the pointer of the entry with the index given 
 */                       
void* 
CDynamicArray::GetEntry( UInt32 index ) const
{GUCEF_TRACE;
        return _entrys[ index ];        
}

/*-------------------------------------------------------------------------*/

/**
 *      Prefixes an entry to the array. The content of the array will be
 *      shifted upwards. If the array size cannot accomodate more entrys
 *      then the array is resized.
 */
void
CDynamicArray::PrefixEntry( void* entry )
{GUCEF_TRACE;
        if ( !_max || ( _last >= ((Int32)_max)-1 ) )
        {
                _max += _resize_change;
                _entrys = ( void** ) realloc( _entrys              ,
                                              _max*sizeof( void* ) );
        }
        ++_last;
        for ( Int32 i=_last-1; i>=0; --i )
        {
                _entrys[ i+1 ] = _entrys[ i ];
        }
        *_entrys = entry;
}

/*-------------------------------------------------------------------------*/

UInt32
CDynamicArray::AppendEntry( void* entry )
{GUCEF_TRACE;
        
        /*
         *	Adds an entry to the array at the back. If there are no free
         *	slots available the array will be resized by resize_change
         *	amount.
         */
        if ( !_max || _last >= ((Int32)_max)-1 )
        {
                CHECKMEM( _entrys, _max*sizeof( void* ) );
                _max += _resize_change;
                _entrys = ( void** ) realloc( _entrys             ,
                                             _max*sizeof( void* ) );
                CHECKMEM( _entrys, _max*sizeof( void* ) );                             
        }
        ++_last;
        _entrys[ _last ] = entry;
        CHECKMEMSEG( _entrys, _entrys+_last, sizeof(void*) );
        return _last;
}

/*-------------------------------------------------------------------------*/

void
CDynamicArray::SetArraySize( UInt32 size )
{GUCEF_TRACE;

        /*
         *	Resize array to the size given
         */
        if ( size != _max )
        {
                _entrys = ( void** ) realloc( _entrys              ,
                                              size*sizeof( void* ) );
                if ( size > _max )
                {
                        for ( UInt32 i=_max; i<size; ++i )
                        {
                                _entrys[ i ] = NULL;
                        }
                }
                else
                {
                        if ( size )
                        {
                                _last = size-1;
                        }
                        else
                        {
                                _last = -1;
                        }
                }
                _max = size;
        }
}

/*-------------------------------------------------------------------------*/

void 
CDynamicArray::ShiftDown( UInt32 startpos )
{GUCEF_TRACE;
        for ( Int32 i=startpos; i<=_last; ++i )
        {
                _entrys[ i ] = _entrys[ i+1 ];       
        }   
        --_last;            
}

/*-------------------------------------------------------------------------*/
        
void 
CDynamicArray::ShiftUp( UInt32 startpos )
{GUCEF_TRACE;
        
        if ( _last+1 >= (Int32)_max )
        {
                SetArraySize( _max+_resize_change );        
        }
        for ( Int32 i=startpos; i<=_last; ++i )
        {
                _entrys[ i+i ] = _entrys[ i ];       
        }
        ++_last;         
}

/*-------------------------------------------------------------------------*/

Int32
CDynamicArray::FindNULLEntry( void ) const
{GUCEF_TRACE;
        
        /*
         *	Searches for first entry that is NULL is can find.
         *	if found it will return index
         */
        for( UInt32 i=0; i<_max; i++ )
        {
                if ( !_entrys[ i ] )
                { 
                        return i;
                }                        
        }
        return -1;
}

/*-------------------------------------------------------------------------*/

void
CDynamicArray::SetResizeChange( UInt32 new_rcs )
{GUCEF_TRACE;
        if ( new_rcs >= 1 )
        {
                _resize_change = new_rcs;
        }
}

/*-------------------------------------------------------------------------*/

void*
CDynamicArray::operator[]( UInt32 index ) const
{GUCEF_TRACE;
        CHECKMEMSEG( _entrys, (const void*)(((INTPTR)_entrys)+(index*sizeof(void*))), sizeof(void*) ); 

        return _entrys[ index ];
}

/*-------------------------------------------------------------------------*/

void*& 
CDynamicArray::operator[]( UInt32 index )
{GUCEF_TRACE;
        return _entrys[ index ];
}

/*-------------------------------------------------------------------------*/

/**
 *	Removes entry by moving the entry's with a higher index down
 *	by one.
 */
void
CDynamicArray::RemoveEntry( UInt32 index )
{GUCEF_TRACE;
        for ( UInt32 i=index; i<_max-1; ++i )
        {
                _entrys[ i ] = _entrys[ i+1 ];
        }
        if ( _max )
        {
                --_last;
                if ( _resize_change <= _max-_last+1 )
                {
                        SetArraySize( _last+1 );
                }
        }
}

/*-------------------------------------------------------------------------*/

/**
 *	Attempts to add entry to a slot which has value NULL.
 *	if no free slot can be found then Append_Entry() is called.
 */
UInt32
CDynamicArray::AddEntry( void *entry )
{GUCEF_TRACE;        
        Int32 idx = FindNULLEntry();
        if ( idx >= 0 )
        {
                SetEntry( idx, entry );
                return (UInt32)idx;
        }
        return AppendEntry( entry );
}

/*-------------------------------------------------------------------------*/

UInt32
CDynamicArray::GetNonNULLCount( void ) const
{GUCEF_TRACE;        
        UInt32 c( 0 );
        for ( Int32 i=0; i<=_last; ++i )
        {
                if ( _entrys[ i ] ) ++c;
        }
        return c;
}

/*-------------------------------------------------------------------------*/

UInt32
CDynamicArray::GetResizeChange( void ) const
{GUCEF_TRACE;
        return _resize_change;
}

/*-------------------------------------------------------------------------*/

Int32
CDynamicArray::GetLast( void ) const
{GUCEF_TRACE;
        return _last;
}

/*-------------------------------------------------------------------------*/

UInt32 
CDynamicArray::GetCount( void ) const
{GUCEF_TRACE;
        return _last+1;
}

/*-------------------------------------------------------------------------*/

UInt32
CDynamicArray::GetArraySize( void ) const
{GUCEF_TRACE;
        return _max;
}

/*-------------------------------------------------------------------------*/

void
CDynamicArray::FillNULLGaps( void )
{GUCEF_TRACE;
        
    Int32 jmp = 0;
    for ( Int32 i=0; i+jmp<=_last; ++i )
    {
        if ( _entrys[ i ] == NULL )
        {
            while ( i+jmp <= _last )
            {                                
                _entrys[ i ] = _entrys[ i+jmp ];
                if ( _entrys[ i ] )
                {
                    _entrys[ i+jmp ] = NULL;
                    break;
                }
                ++jmp;
            }
            
            if ( !_entrys[ i ] )
            {
                _last-=jmp;
                return;        
            }
        }
    }
 
}

/*-------------------------------------------------------------------------*/

void
CDynamicArray::Clear( void )
{GUCEF_TRACE;
        memset( _entrys, 0, _max*sizeof(void*) );
        _last = -1;      
}

/*-------------------------------------------------------------------------*/

Int32 
CDynamicArray::Find( const void* ptr ) const
{GUCEF_TRACE;
        for ( Int32 i=0; i<=_last; ++i )
        {
                if ( _entrys[ i ] == ptr )
                {
                        return i;
                }
        }
        return -1;                
}

/*-------------------------------------------------------------------------*/

bool 
CDynamicArray::Insert( const UInt32 index , 
                       void* value        )
{GUCEF_TRACE;
        if ( (Int32)index <= _last )
        {
                ShiftUp( index );
                SetEntry( index, value );
                return true;
        }
        return false;        
}

/*-------------------------------------------------------------------------*/

bool 
CDynamicArray::RemoveEntry( void* entry )
{GUCEF_TRACE;
        Int32 index( Find( entry ) );
        if ( index > 0 )
        {
                RemoveEntry( index );
                return true;         
        }
        return false;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
