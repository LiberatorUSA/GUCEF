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

#ifndef GUCEF_CORE_CSTRINGLIST_H
#define GUCEF_CORE_CSTRINGLIST_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"     /* often used gucef macros */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_ETYPES_H
#include "ETypes.h"               /* simple types used */
#define GUCEF_CORE_ETYPES_H
#endif /* GUCEF_CORE_ETYPES_H ? */

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"            /* string class */
#define GUCEF_CORE_CDVSTRING_H
#endif /* GUCEF_CORE_CDVSTRING_H ? */

#ifndef GUCEF_CORE_CDYNAMICARRAY_H
#include "CDynamicArray.h"
#define GUCEF_CORE_CDYNAMICARRAY_H
#endif /* GUCEF_CORE_CDYNAMICARRAY_H ? */

#ifndef GUCEF_CORE_CIOACCESS_H
#include "CIOAccess.h"
#define GUCEF_CORE_CIOACCESS_H
#endif /* GUCEF_CORE_CIOACCESS_H ? */

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

/**
 *      Enum type for text formatting,
 *      more specificly the seperation between lines.
 */
typedef enum
{
        PC_TEXT,        /* LF & CR */
        UNIX_TEXT,      /* LF */
        MAC_TEXT        /* CR */
} TTextType;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *      Class to function as a container for a set of strings with
 *      manipulation and sorting options
 *
 *      Note that the methods used for random access have a speed penalty
 *      because we are dealing with a linked list. for itterating trough the
 *      list use the Sequential access methods.
 *      This class is not threadsafe.
 *      Also note that this class uses the ASCII char set so don't pass buffers
 *      with 8 bit integers, meaning a 0 value in the middle of the buffer ect.
 *
 *      @deprecated
 */
class GUCEFCORE_EXPORT_CPP CStringList
{
        public:

        CStringList( void );

        CStringList( const CStringList& src );

        ~CStringList();

        CStringList& operator=( const CStringList& src );
        
        CString& operator[]( UInt32 index );
        
        CString operator[]( UInt32 index ) const;
        
        void Merge( const CStringList& other ,
                    bool allow_duplicates    );

        UInt32 Append( const CString& string );
        
        void AddAtIndex( const CString& string ,
                         const UInt32 index    );
        
        Int32 Find( const CString& string ) const;
        
        void SetAllowDuplicates( bool allow );
        
        bool GetAllowDuplicates( void ) const;

        bool LoadFromFile( const CString& filename );
        
        bool LoadFromFile( CIOAccess* file );

        void SaveToFile( const CString& filename );

        UInt32 GetCount( void ) const;
        
        bool Delete( UInt32 index );
        
        void Clear( void );

        class GUCEFCORE_EXPORT_CPP iterator
        {
                public:

                iterator( CStringList* list );

                iterator( const iterator& src );

                iterator( CStringList* list ,
                          UInt32 position   );

                ~iterator();

                iterator& operator=( const iterator& src );

                CString& operator*( void );

                CString& operator[]( UInt32 offset );

                iterator& operator+=( Int32 offset );

                iterator& operator-=( Int32 offset ); 

                iterator& operator++( void );

                iterator& operator--( void );

                bool operator!=( const iterator& src ) const;

                void Insert( const CString& string );

                void Delete( void );                

                private:
                CStringList* _list;
                UInt32 _pos;
        };

        class GUCEFCORE_EXPORT_CPP const_iterator
        {
                public:

                const_iterator( const CStringList* list );

                const_iterator( const CStringList* list ,
                                UInt32 position         );

                const_iterator( const const_iterator& src );

                ~const_iterator();

                const_iterator& operator=( const const_iterator& src );

                const CString& operator*( void ) const;

                const CString& operator[]( UInt32 offset );

                const_iterator& operator+=( Int32 offset );

                const_iterator& operator-=( Int32 offset );

                const_iterator& operator++( void );

                const_iterator& operator--( void );

                bool operator!=( const const_iterator& src ) const;

                private:
                const CStringList* _list;
                UInt32 _pos;                
        };

        iterator Begin( void );

        iterator End( void );
        
        const_iterator Begin( void ) const;

        const_iterator End( void ) const;

        const_iterator ConstBegin( void ) const;

        const_iterator ConstEnd( void ) const;

        private:
        friend iterator;
        friend const_iterator;

        CDynamicArray _entrys;
        bool _allowmultiple;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CSTRINGLIST_H */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 22-05-2005 :
        - Added Clear()
        - Added Delete()
- 01-05-2005 :
        - Fixed a bug in the copy constructor, the copy stringlist has the 
          correct amount of storage but no entry's.
- 26-11-2004 :
        - Redesigned and recoded this class. It now uses iterators and the
          refrence counted string class. void* links are no longer available. 
- 18-08-2003 :
        - Changed EXPORT into GUCEFCORE_EXPORT_CPP
- 06-06-2003 :
        - Moved more functionality into this abstarct base class from it's
          decendants.
        - Directlink functionality is removed. It's basicly hardcoded to true
          now. This is done to make memory management in-scope.
        - Add this note: This originally was the first C++ class i ever made
          which has changed drasticly over time.
- 04-04-2003  :
        - Made this an abstract base class to serve as base for both a linked
          list and a array based implementation. swithing between the 2 could be
          used as an optimization of a stringlist.
- 11-03-2003 :
        - Continued making changes to internal working of class.
        - Direct link is now a public data member since encapsulation adds
          no additional safety or whatever for it.
        - using the 3 linked list pointers random access 'hops' can be reduced
          by 75% on average.
- 09-03-2003 :
        - Started making changes to class structure. Class is no longer a
          decendant of CNode. This results in a reduced memory footprint for an
          instance of this class, especially with entry's.

--------------------------------------------------------------------------*/
