/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
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

#ifndef GUCEF_CORE_CTDYNAMICARRAY_H
#define GUCEF_CORE_CTDYNAMICARRAY_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_ETYPES_H
#include "ETypes.h"               /* simple types used */
#define GUCEF_CORE_ETYPES_H
#endif /* GUCEF_CORE_ETYPES_H ? */

#ifndef GUCEF_CORE_GUCEFCORE_MACROS_H
#include "gucefCORE_macros.h"     /* often used gucef macros */
#define GUCEF_CORE_GUCEFCORE_MACROS_H
#endif /* GUCEF_CORE_GUCEFCORE_MACROS_H ? */

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

/**
 *      Simply implementation of a resizable array. 
 *      This class is NOT thread safe.
 */
template typename< T > 
class CTDynamicArray
{
        public:

        /**
         *      Searches for first entry that is NULL is can find.
         *      if found it will return index
         */
        Int32 FindNULLEntry( void ) const
        {
                Int32 jmp = 1;
                for ( Int32 i=0; i+jmp<=m_last; ++i )
                {
                        if ( !m_entrys[ i ] )
                        {
                                while ( i+jmp <= m_last )
                                {                                
                                        m_entrys[ i ] = m_entrys[ i+jmp ];
                                        if ( m_entrys[ i ] )
                                        {
                                                m_entrys[ i+jmp ] = NULL;
                                                break;
                                        }
                                        ++jmp;
                                }
                                
                                if ( !m_entrys[ i ] )
                                {
                                        m_last-=jmp;
                                        return;        
                                }
                        }
                }        
        }

        /**
         *      Get/Set size of array. Note that this does not effect whatever
         *      is pointed to by the void* so you might have to deallocate
         *      things first.
         */
        void SetArraySize( UInt32 size )
        {
                if ( new_rcs >= 1 )
                {
                        m_resize_change = new_rcs;
                }        
        }
        
        UInt32 GetArraySize( void ) const
        {
                return m_max;
        }

        /**
         *      Appends an entry to the array at the back. If there are no free
         *      slots available the array will be resized by resize_change
         *      amount. Returns the index at which the entry was added.
         */
        UInt32 AppendEntry( void *entry )
        {
                if ( !m_max || m_last >= ((Int32)m_max)-1 )
                {
                        m_max += m_resize_change;
                        m_entrys = ( T** ) realloc( m_entrys           ,
                                                    m_max*sizeof( T* ) );                         
                }
                ++m_last;
                m_entrys[ m_last ] = entry;
                return m_last;        
        }

        /**
         *      Prefixes an entry to the array. The content of the array will be
         *      shifted upwards. If the array size cannot accomodate more entrys
         *      then the array is resized.
         */
        void PrefixEntry( void *entry )
        {
                if ( !m_max || ( m_last >= ((Int32)m_max)-1 ) )
                {
                        m_max += m_resize_change;
                        m_entrys = ( T** ) realloc( m_entrys              ,
                                                    m_max*sizeof( void* ) );
                }
                ++m_last;
                for ( Int32 i=m_last-1; i>=0; --i )
                {
                        m_entrys[ i+1 ] = m_entrys[ i ];
                }
                *m_entrys = entry;        
        }

        /**
         *      Attempts to add entry to a slot which has value NULL.
         *      if no free slot can be found then Append_Entry() is called.
         */
        UInt32 AddEntry( void *entry )
        {
                if ( !m_max || ( m_last >= ((Int32)m_max)-1 ) )
                {
                        m_max += m_resize_change;
                        m_entrys = ( T** ) realloc( m_entrys              ,
                                                    m_max*sizeof( void* ) );
                }
                ++m_last;
                for ( Int32 i=m_last-1; i>=0; --i )
                {
                        m_entrys[ i+1 ] = m_entrys[ i ];
                }
                *m_entrys = entry;        
        }

        /**
         *      Removes entry by moving the entry's with a higher index down
         *      by one.
         */
        void RemoveEntry( UInt32 index )
        {
                for ( UInt32 i=index; i<m_max-1; ++i )
                {
                        m_entrys[ i ] = m_entrys[ i+1 ];
                }
                if ( m_max )
                {
                        --m_last;
                        if ( m_resize_change <= m_max-m_last+1 )
                        {
                                SetArraySize( m_last+1 );
                        }
                }        
        }

        /**
         *      Returns index of last entry in array. The array itself may
         *      actually be larger then this.
         */
        Int32 GetLast( void ) const
        {
                return m_last;
        }
        
        /**
         *      Returns index of number of entries in the array. 
         *      The array itself may actually be larger then this.
         */
        UInt32 GetCount( void ) const
        {
                return m_last+1;
        }
        
        /**
         *      NULLs all array entrys and sets the usage count to 0
         *      The actual storage space is not affected.
         */
        void Clear( void )
        {
                memset( _entrys, 0, m_max*sizeof(T*) );
                m_last = 0;        
        }        

        /**
         *      Resize change, affects array size when using Append_Entry()
         *      The size you set here is how many entry's will be added
         */
        void SetResizeChange( UInt32 new_rcs )        
        {
                if ( new_rcs >= 1 )
                {
                        m_resize_change = new_rcs;
                }                
        }

        UInt32 GetResizeChange( void ) const
        {
                return m_resize_change;
        }

        /**
         *      Returns entry pointer with index given. If the index is invalid
         *      NULL will be returned.
         */
        T* operator[]( UInt32 index ) const
        {
                return m_entrys[ index ];
        }

        /**
         *      Set's the pointer of the entry with the index given to
         *      new_value.
         */
        void SetEntry( UInt32 index ,
                       T* new_value )
        {
                m_entrys[ index ] = new_value;
                if ( (Int32)index > m_last ) 
                {
                       m_last = index;
                }            
        }                       
                       
        /**
         *      Get's the pointer of the entry with the index given 
         */                       
        void* GetEntry( UInt32 index ) const
        {
                return _entrys[ index ];
        }    
        
        void ShiftDown( UInt32 startpos )
        {
                for ( Int32 i=startpos; i<=m_last; ++i )
                {
                        m_entrys[ i ] = m_entrys[ i+1 ];       
                }   
                --m_last;        
        }
        
        void ShiftUp( UInt32 startpos )
        {
                if ( m_last+1 >= (Int32)m_max )
                {
                        SetArraySize( m_max+m_resize_change );        
                }
                for ( Int32 i=startpos; i<=m_last; ++i )
                {
                        m_entrys[ i+i ] = m_entrys[ i ];       
                }
                ++m_last;        
        }

        UInt32 GetNonNULLCount( void ) const
        {
                UInt32 c( 0 );
                for ( Int32 i=0; i<=m_last; ++i )
                {
                        if ( m_entrys[ i ] ) ++c;
                }                
        }
        
        /**
         *      Shifts items down into NULL pointer 'gaps'         
         */
        void FillNULLGaps( void )
        {
                for( UInt32 i=0; i<m_max; i++ )
                {
                        if ( !m_entrys[ i ] )
                        {
                                GUCEF_END; 
                                return i;
                        }                        
                }        
        }

        /*
         *      Con&Destructors
         */
        CDynamicArray( UInt32 initial_size )
        {
                m_max = initial_size;
                m_entrys = ( T** ) malloc( m_max*sizeof( T* ) );
                m_last = -1;
                m_resize_change = 1;                
        }
        
        CDynamicArray( void )
        {
                m_entrys = NULL;
                m_max = 0;
                m_last = -1;
                m_resize_change = 1;        
        }
        
        ~CDynamicArray()
        {
                free( m_entrys );
        }

        private:
        
        UInt32 m_resize_change;   /* size by which array gets resized if there is no more room when using Append_Entry */
        T** m_entrys;             /* array of void* */
        Int32 m_last;             /* last entry, used in combo with Append_Entry */
        UInt32 m_max;             /* actual size of array */        
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTDYNAMICARRAY_H */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 23-04-2005 :
        - Initial version
          
---------------------------------------------------------------------------*/
