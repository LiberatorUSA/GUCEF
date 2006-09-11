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

#ifndef GUCEF_CORE_CINTMAP_H
#define GUCEF_CORE_CINTMAP_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CDYNAMICARRAY_H
#include "CDynamicArray.h"           /* dynamic array implementation */
#define GUCEF_CORE_CDYNAMICARRAY_H
#endif /* GUCEF_CORE_CDYNAMICARRAY_H ? */

#ifndef GUCEF_CORE_GUCEFCORE_MACROS_H
#include "gucefCORE_macros.h"         /* often used gucef macros */
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
 *      Class that maps integers to pointers.
 *      Maintains a sorted list.
 *      key values do not have to be unique but Get() and the index operator
 *      will only return the first entry.
 */
class EXPORT_CPP CIntMap
{
        public:
        
        /**
         *      Default constructor, does nothing atm.
         */
        CIntMap( void );
        
        /**
         *      Creates this string map as an exact copy 
         *      of the given string map.
         *      Note that both link pointers will point to
         *      the same address for each entry, take this
         *      into account when making copys
         */
        CIntMap( const CIntMap& src );
        
        /**
         *      Destructor, de-allocates all mapped links
         */
        ~CIntMap();
        
        /**
         *      Creates an exact copy of the given string map
         *      Note that both link pointers will point to
         *      the same address for each entry, take this
         *      into account when making copys
         */
        CIntMap& operator=( const CIntMap& src );                            
        
        /**
         *      index operator for array-like access
         *      Retrieves the pointer for the given index
         *
         *      @param index for the pointer you wish to obtain
         *      @return pointer related to the given map entry index.
         */        
        void* operator[]( Int32 key );                                    
        
        /**
         *      Retrieves the pointer for the given name
         *      If the name is not found NULL will be returned.
         *      Same as useing the index operator
         *
         *      @param name for the pointer you wish to obtain
         *      @return pointer related to the name given.
         */
        void* Get( const Int32 key ) const;                

        void* GetAtIndex( const UInt32 index ) const;
        
        bool Exists( const Int32 key ) const;

        bool Add( const Int32 key  ,
                  void* linkeddata );
                  
        void Delete( const Int32 key );
        
        void DeleteAtIndex( const UInt32 index );
        
        void DeleteAll( void );
        
        UInt32 Count( void ) const;
        
        private:        
        
        CDynamicArray m_items;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CINTMAP_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 08-02-2005 :
        - Initial implementation
          
---------------------------------------------------------------------------*/
