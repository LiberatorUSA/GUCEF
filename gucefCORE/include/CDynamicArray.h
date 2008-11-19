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

#ifndef GUCEF_CORE_CDYNAMICARRAY_H
#define GUCEF_CORE_CDYNAMICARRAY_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_ETYPES_H
#include "gucefCORE_ETypes.h"               /* simple types used */
#define GUCEF_CORE_ETYPES_H
#endif /* GUCEF_CORE_ETYPES_H ? */

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"     /* often used gucef macros */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

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
 *      Simply implementation of a resize-able array. 
 *      This class is NOT thread safe.
 *
 *      @deprecated
 */
class GUCEFCORE_EXPORT_CPP CDynamicArray
{
        public:

        /**
         *      Searches for first entry that is NULL is can find.
         *      if found it will return index
         */
        Int32 FindNULLEntry( void ) const;
        
        /**
         *      Searches the array for the specified pointer value
         */
        Int32 Find( const void* ptr ) const;

        /**
         *      Get/Set size of array. Note that this does not effect whatever
         *      is pointed to by the void* so you might have to deallocate
         *      things first.
         */
        void SetArraySize( UInt32 size );
        
        UInt32 GetArraySize( void ) const;

        /**
         *      Appends an entry to the array at the back. If there are no free
         *      slots available the array will be resized by resize_change
         *      amount. Returns the index at which the entry was added.
         */
        UInt32 AppendEntry( void *entry );

        /**
         *      Prefixes an entry to the array. The content of the array will be
         *      shifted upwards. If the array size cannot accomodate more entrys
         *      then the array is resized.
         */
        void PrefixEntry( void *entry );

        /**
         *      Attempts to add entry to a slot which has value NULL.
         *      if no free slot can be found then Append_Entry() is called.
         */
        UInt32 AddEntry( void *entry );

        /**
         *      Removes the entry at the given index by moving the entry's 
         *      with a higher index down by one.
         */
        void RemoveEntry( UInt32 index );
        
        /**
         *      Removes entry by moving the entry's with a higher index down
         *      by one.
         */
        bool RemoveEntry( void* entry );        

        /**
         *      Returns index of last entry in array. The array itself may
         *      actually be larger then this.
         */
        Int32 GetLast( void ) const;
        
        /**
         *      Returns index of number of entries in the array. 
         *      The array itself may actually be larger then this.
         */
        UInt32 GetCount( void ) const;
        
        /**
         *      NULLs all array entrys and sets the usage count to 0
         *      The actual storage space is not affected.
         */
        void Clear( void );
        
        /**
         *      Inserts the given value at the given index.
         *      the index must be smaller or equal to the index of the last item.         
         */
        bool Insert( const UInt32 index , 
                     void* value        );                            

        /**
         *      Resize change, affects array size when using Append_Entry()
         *      The size you set here is how many entry's will be added
         */
        void SetResizeChange( UInt32 new_rcs );

        UInt32 GetResizeChange( void ) const;

        /**
         *      Returns entry pointer with index given. If the index is invalid
         *      NULL will be returned.
         */
        void* operator[]( UInt32 index ) const;
        
        /**
         *      Returns entry pointer with index given. If the index is invalid
         *      NULL will be returned.
         */
        void*& operator[]( UInt32 index );        

        /**
         *      Set's the pointer of the entry with the index given to
         *      new_value. If the array is not large enough to store the
         *      given item it will be enlarged.
         */
        void SetEntry( UInt32 index    ,
                       void *new_value );
                       
        /**
         *      Get's the pointer of the entry with the index given 
         */                       
        void* GetEntry( UInt32 index ) const;    
        
        void ShiftDown( UInt32 startpos );
        
        void ShiftUp( UInt32 startpos );

        UInt32 GetNonNULLCount( void ) const;
        
        /**
         *      Shifts items down into NULL pointer 'gaps'         
         */
        void FillNULLGaps( void );

        /*
         *      Con&Destructors
         */
        CDynamicArray( UInt32 initial_size ); /* construct array with an initial size as specified */
        CDynamicArray( void );                /* creates an array with size 0 */
        ~CDynamicArray();                     /* destructor, deallocates the array */

        private:
        UInt32 _resize_change;   /* size by which array gets resized if there is no more room when using Append_Entry */
        void **_entrys;          /* array of void* */
        Int32 _last;             /* last entry, used in combo with Append_Entry */
        UInt32 _max;             /* actual size of array */        
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CDYNAMICARRAY_H */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 26-08-2005 : 
        - Fixed a bug in Clear()
- 17-07-2005 :
        - Added Find()
- 23-04-2005 :
        - Fixed: SetEntry() had a bug that caused the last item indicator
          to remain invalid (ie the count stayed 0).
- 12-11-2004 :
        - Bounds checking removed for performance reasons.
        - Added GetNonNULLCount().
        - Conversion to new coding standard.
- 25-09-2004 :
        - Added Prefix_Entry().
- 22-12-2003 :
        - Added Remove_Entry() to simplyfy things on a higher level.
        - Fixed a bug in Append_Entry() which caused the allocated space to be
          to small.
- 25-11-2003 :
        - Fixed access overun when using [] operator on with an index of
          the same size as the array.
- 19-07-2003 :
        - Changed EXPORT into GUCEFCORE_EXPORT_CPP
- 18-07-2003 :
        - Added this section.
        - Fixed a bug in Append_Entry() when appending to an empty array.
          
---------------------------------------------------------------------------*/
