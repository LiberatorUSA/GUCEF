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

#ifndef GUCEF_CORE_CDVSTRING_H
#define GUCEF_CORE_CDVSTRING_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "gucef_new_off.h"        /* don't scare the STL with our memory manager */

#include <string>                 /* STL string class */

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"    /* macros that are GUCEF specific and generic macros */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_ETYPES_H
#include "ETypes.h"              /* simple types used */
#define GUCEF_CORE_ETYPES_H
#endif /* GUCEF_CORE_ETYPES_H ? */

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

class CStringList;

/*-------------------------------------------------------------------------*/

/**
 *      This class is meant to save some effort when dealing with strings.
 *      Note that a good number of member functions could be made redundant
 *      because a char* parameter can be converted to a CString object implicitly 
 *      which would then be the actual parameter. However since this class allocates
 *      memory we want to keep it as fast as possible thus a minimal of memory
 *      management. And no, I do not want to use the STL implementation. 
 */
class GUCEFCORE_EXPORT_CPP CString
{
        public:
        
        CString( void );

        CString( const CString& src );
        
        CString( const std::string& src );

        CString( const char *src );
        
        CString( const char *src ,
                 UInt32 length   );

        CString( const char src );
        
        CString( const int NULLvalue );
        
        ~CString();
                
        CString& operator=( const CString &src );

        /**
         *      Copys the source string.
         *      If the src string is NULL then the current string will be
         *      deallocated and also set to NULL.
         */
        CString& operator=( const char *src );

        CString& operator+=( const CString &other );

        CString& operator+=( const char *other );
        
        CString& operator+=( char lastchar );
        
        CString operator+( const CString& addition );
        
        CString operator+( const char* addition );

        bool operator==( const CString &other ) const;        
        
        bool operator==( const char *other ) const;
        
        bool operator!=( const CString &other ) const;

        bool operator!=( const char *other ) const;
        
        bool operator<( const CString& other ) const;

        char operator[]( const UInt32 index ) const;
        
        operator std::string() const;
        
        /* operator const char*() const; */

        const char* C_String( void ) const;        
        
        std::string STL_String( void ) const;

        UInt32 Length( void ) const;
        
        void SetInt( Int32 value );
        
        Int32 GetInt( void ) const;

        /**
         *      This member functions allows you to set the string using a
         *      non-null-terminated char array as the source. The source will
         *      be copied and a null terminator will be added.
         */
        void Set( const char *new_str ,
                  UInt32 len          );
                  
        /**
         *  This member functions allows you to set the string using a
         *  null-terminated char array as the source while specifying a
         *  maximum buffer scan length. If no null-terminator is found
         *  the maximum number of bytes indicated will be used as the string
         *  and a null-terminator will be added.
         */
        void Scan( const char* newStr     ,
                   const UInt32 maxLength );                  
            
        /**
         *      This member functions allows you to add to the string using a
         *      non-null-terminated char array as the source. The source will
         *      be copyed and a null terminator will be added.
         */                  
        void Append( const char *appendstr ,
                     UInt32 len            );                  
                  
        CString Lowercase( void ) const;                 
        
        CString Uppercase( void ) const;
                  
        CString ReplaceChar( char oldchar ,
                             char newchar ) const;                  
                  
        void ReplaceSubstr( const CString& substr      ,
                            const CString& replacement );
                            
        CString SubstrToChar( char searchchar        ,
                              bool startfront = true ) const;
                              
        CString SubstrToSubstr( const CString& searchstr ,
                                bool startfront = true   ) const;
                                
        Int32 HasChar( char searchchar        ,
                       bool startfront = true ) const;                                
                                
        Int32 HasSubstr( const CString& substr  ,
                         bool startfront = true ) const;
                              
        CString CutChars( UInt32 charcount        ,
                          bool startfront = false ) const;
                          
        CStringList ParseElements( char seperator ) const;                          
                            
        bool Equals( const CString& otherStr          ,
                     const bool syntaxSpecific = true ) const;

        bool NotEquals( const CString& otherStr          ,
                        const bool syntaxSpecific = true ) const;
        
        void Clear( void );
        
        /**
         *      Utility member function for easy access to string caching
         */
        UInt32 Cache( void ) const;                                                                              

        private:
        char* m_string;    /**< our actual null-terminated string */
        UInt32 m_length;   /**< length of the string */  
};

/*-------------------------------------------------------------------------*/

GUCEFCORE_EXPORT_CPP bool operator!=( const char* lhs, const CString& rhs );
GUCEFCORE_EXPORT_CPP bool operator==( const char* lhs, const CString& rhs );
GUCEFCORE_EXPORT_CPP CString operator+( const CString& lhs, const CString& rhs );
GUCEFCORE_EXPORT_CPP CString operator+( const CString& lhs, const char* rhs );        
GUCEFCORE_EXPORT_CPP CString operator+( const char* lhs, const CString& rhs ); 
GUCEFCORE_EXPORT_CPP CString operator+( const char lhs, const CString& rhs );

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CDVSTRING_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 01-10-2006 :
        - Dinand: This string implementation is no longer reference counted.
          There is no way to make it both reference counted and threadsafe at the same time
          without using some global mechanism which in turn causes problems when using strings
          as global variables.
          Since most people already assume strings don't have stellar performance I decided to
          rewrite the code to get rid of the reference counting and simply allocate memory 
          when needed
- 05-05-2005 :
        - Dinand: Fixed a bug in operator+=( const CString &other ): the old string was 
          unregistered before the combo was made causing data loss.
- 24-04-2005 :
        - Dinand: Added Lowercase()
        - Dinand: Added Uppercase()
        - Dinand: Added SubstrToSubstr()
- 23-04-2005 :
        - Dinand: Fixed a length setting bug in the constructor that accepts a C-string
          with a given length.
- 18-04-2005 :
        - Dinand: Added lots of safety code in the mutation operators (=, +=, ect.)
          NULL pointers can now safely be added or assigned. They will be treated
          as an string with length 0.
- 11-04-2005 :
        - Dinand: Added include that disabled the memory manager defines before including
          the STL header. The STL can't handle our manager and would freak out.
          The subsequent inclusion of the macro file will re enable the defines
          if needed.
- 10-04-2005 :
        - Dinand: Fixed: String length wasn't set when using Set()
- 05-04-2005 :
        - Dinand: Empty strings (ie length 0) will no longer contact the central depot
- 04-04-2005 :
        - Dinand: Added Clear()
        - Dinand: Added SubstrToChar()
        - Dinand: Fixed: self assign protection for assignment operator
        - Dinand: Fixed: you can now safely assign an NULL pointer
        - Dinand: Added pointer to the central string storage depot. 
          This reduces the number of mutex locks at the depot by about half so it's worth the 4
          extra bytes of memory.
- 03-04-2005 :
        - Dinand: Fixed several string length bugs. The length data member simply wasnt
          updated in the = operator ect.
- 13-11-2004 :
        - Dinand: Converted class to use reference counting. 
- 19-04-2004 :
        - Dinand: Designed and implemented this class.

-----------------------------------------------------------------------------*/
 