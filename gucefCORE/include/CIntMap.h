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

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"         /* often used gucef macros */
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
 *      Class that maps integers to pointers.
 *      Maintains a sorted list.
 *      key values do not have to be unique but Get() and the index operator
 *      will only return the first entry.
 *
 *      @deprecated
 */
class GUCEFCORE_EXPORT_CPP CIntMap
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
