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

#ifndef GUCEF_CORE_CSTRINGMAP_H
#define GUCEF_CORE_CSTRINGMAP_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CDYNAMICARRAY_H
#include "CDynamicArray.h"           /* dynamic array implementation */
#define GUCEF_CORE_CDYNAMICARRAY_H
#endif /* GUCEF_CORE_CDYNAMICARRAY_H ? */

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"               /* string implementation */
#define GUCEF_CORE_CDVSTRING_H
#endif /* GUCEF_CORE_CDVSTRING_H ? */

#ifndef GUCEF_CORE_CSTRINGLIST_H
#include "CStringList.h"             /* string container */
#define GUCEF_CORE_CSTRINGLIST_H
#endif /* GUCEF_CORE_CSTRINGLIST_H ? */

#ifndef GUCEF_CORE_GUCEFCORE_MACROS_H
#include "gucefCORE_macros.h"        /* often used gucef macros */
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
 *      Class that maps string names to a pointer.
 */
class EXPORT_CPP CStringMap
{
        public:
        
        /**
         *      Default constructor, does nothing atm.
         */
        CStringMap( void );
        
        /**
         *      Creates this string map as an exact copy 
         *      of the given string map.
         *      Note that both link pointers will point to
         *      the same address for each entry, take this
         *      into account when making copys
         */
        CStringMap( const CStringMap& src );
        
        /**
         *      Destructor, de-allocates all mapped links
         */
        ~CStringMap();
        
        /**
         *      Creates an exact copy of the given string map
         *      Note that both link pointers will point to
         *      the same address for each entry, take this
         *      into account when making copys
         */
        CStringMap& operator=( const CStringMap& src );
        
        /**
         *      Retrieves the pointer for the given name
         *      If the name is not found NULL will be returned.
         *
         *      @param name for the pointer you wish to obtain
         *      @return pointer related to the name given.
         */        
        void* operator[]( const CString& name ) const;  
        
        /**
         *      index operator for finding the string for the given data
         *
         *      @param data pointer for the string you wish to obtain
         *      @return pointer related to the given map entry index.
         */        
        CString operator[]( const void* data ) const;               
        
        /**
         *      index operator for array-like access
         *      Retrieves the pointer for the given index
         *
         *      @param index for the pointer you wish to obtain
         *      @return pointer related to the given map entry index.
         */        
        void* operator[]( const UInt32 index ) const;                                    
        
        /**
         *      Retrieves the pointer for the given name
         *      If the name is not found NULL will be returned.
         *      Same as useing the index operator
         *
         *      @param name for the pointer you wish to obtain
         *      @return pointer related to the name given.
         */
        void* Get( const CString& name ) const;
        
        CString GetKey( const UInt32 index ) const;
        
        Int32 GetKeyIndex( const CString& name ) const;
        
        CStringList GetKeyList( void ) const;

        Int32 Add( const CString& name ,
                   void* linkeddata    );
                  
        void Delete( const CString& name );
        
        void DeleteAll( void );
        
        UInt32 GetCount( void ) const;
        
        private:        
        
        CDynamicArray _items;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CSTRINGMAP_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 08-02-2005 :
        - Initial implementation
          
---------------------------------------------------------------------------*/
