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

#include <vector>
#include <set>
#include <string>

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

/**
 *      This class is meant to save some effort when dealing with strings.
 *      Note that a good number of member functions could be made redundant
 *      because a char* parameter can be converted to a CString object implicitly
 *      which would then be the actual parameter. However since this class allocates
 *      memory we want to keep it as fast as possible thus a minimal of memory
 *      management. And no, I do not want to use the STL implementation.
 */
class GUCEF_CORE_PUBLIC_CPP CString
{
    public:

    typedef std::vector< CString >  StringVector;
    typedef std::set< CString >     StringSet;

    static const CString Empty;
    static const StringVector EmptyStringVector;

    CString( void );

    CString( const CString& src );

    #ifdef GUCEF_RVALUE_REFERENCES_SUPPORTED
    CString( CString&& src );
    #endif

    CString( const std::string& src );

    CString( const char *src );

    CString( const char *src ,
             UInt32 length   );

    explicit CString( const char src );

    explicit CString( const int NULLvalue );

    ~CString();

    CString& operator=( const CString &src );

    CString& operator=( const std::string& src );
    
    /**
     *      Copys the source string.
     *      If the src string is NULL then the current string will be
     *      deallocated and also set to NULL.
     */
    CString& operator=( const char *src );

    CString& operator+=( const CString &other );

    CString& operator+=( const char *other );

    CString& operator+=( char lastchar );

    CString operator+( const CString& addition ) const;

    CString operator+( const char* addition ) const;

    CString operator+( const char addition ) const;

    bool operator==( const CString &other ) const;

    bool operator==( const char *other ) const;

    bool operator==( const int NULLvalue ) const;

    bool operator==( const char character ) const;

    bool operator!=( const CString &other ) const;

    bool operator!=( const char *other ) const;

    bool operator!=( const int NULLvalue ) const;

    bool operator!=( const char character ) const;

    bool operator<( const CString& other ) const;

    char operator[]( const UInt32 index ) const;

    bool IsNULLOrEmpty( void ) const;

    operator std::string() const;

    /* operator const char*() const; */

    const char* C_String( void ) const;

    char* C_String( void );

    /**
     *  If you wish to manipulate characters in a char buffer
     *  while still having containment as a string object this function
     *  is for you. It will set the string to have an unititialized char buffer
     *  of stringSize bytes.
     */
    char* Reserve( const UInt32 stringSize );

    std::string STL_String( void ) const;

    UInt32 Length( void ) const;

    /**
     *  Allows you to set the length of the string.
     */
    void SetLength( UInt32 newLength );

    /**
     *  This function will give you the length of the string just like the regular Length()
     *  However it will determine the length based on the null terminator and overwrite the cached
     *  length value. This is useful in combination with the Reserve() function.
     */
    UInt32 DetermineLength( void );

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

    CString ReplaceSubstr( const CString& substr      ,
                           const CString& replacement ) const;

    /**
     *  Replaces an enveloping substring with another
     *  Example: "$MyEnvelope(" ")" -> "%{" "}"
     */
    CString ReplaceEnvelopingSubstr( const CString& envelopPrefix     ,
                                     const CString& envelopPostfix    ,
                                     const CString& newEnvelopPrefix  ,
                                     const CString& newEnvelopPostfix ) const;

    CString ReplaceSubStr( UInt32 startIndex        ,
                           UInt32 length            ,
                           const CString& newSubstr ) const;

    CString SubstrToChar( char searchchar         ,
                          bool frontToBack = true ) const;

    CString SubstrToChar( char searchchar                        ,
                          UInt32 startIndex                      ,
                          bool frontToBack = true                ,
                          bool returnEmptyIfCharNotFound = false ) const;

    CString SubstrToSubstr( const CString& searchstr ,
                            bool frontToBack = true  ) const;

    CString SubstrToSubstr( const CString& searchstr ,
                            UInt32 startIndex        ,
                            bool frontToBack = true  ) const;

    CString SubstrToIndex( UInt32 index            ,
                           bool frontToBack = true ) const;

    /**
     *  As per common convention:
     *  startIndex is inclusive and endIndex is exclusive
     *  Put another way: the char pointed to by endIndex will NOT be included
     */
    CString SubstrFromRange( UInt32 startIndex ,
                             UInt32 endIndex   ) const;

    CString Trim( bool frontToBack ) const;

    Int32 HasChar( char searchchar         ,
                   bool frontToBack = true ) const;

    Int32 HasChar( char searchchar         ,
                   const UInt32 startIndex ,
                   bool frontToBack        ) const;

    Int32 HasSubstr( const CString& substr   ,
                     Int32 startIndex        ,
                     bool frontToBack = true ) const;

    Int32 HasSubstr( const CString& substr   ,
                     bool frontToBack = true ) const;

    UInt32 GetCharacterCount( const char searchChar ) const;

    UInt32 FindMaxSubstrEquality( const CString& searchStr ,
                                  const UInt32 startOffset ,
                                  bool frontToBack         ,
                                  bool isCaseSentive       ) const;

    CString CutChars( UInt32 charcount        ,
                      bool frontToBack = true ,
                      UInt32 startOffset = 0  ) const;

    CString RemoveChar( const char charToRemove ) const;

    CString CompactRepeatingChar( const char charToCompact ) const;

    StringVector ParseElements( char seperator               ,
                                bool addEmptyElements = true ) const;

    StringSet ParseUniqueElements( char seperator               ,
                                   bool addEmptyElements = true ) const;

    bool WildcardEquals( const CString& strWithWildcards ,
                         const char wildCardToken = '*'  ,
                         const bool caseSensitive = true ) const;

    bool Equals( const CString& otherStr        ,
                 const bool caseSensitive = true ) const;

    bool NotEquals( const CString& otherStr         ,
                    const bool caseSensitive = true ) const;

    CString Combine( const StringVector& elements, char seperator ) const;
    
    void Clear( void );

    private:
    char* m_string;    /**< our actual null-terminated string */
    UInt32 m_length;   /**< length of the string */
};

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP bool operator!=( const char* lhs, const CString& rhs );
GUCEF_CORE_PUBLIC_CPP bool operator==( const char* lhs, const CString& rhs );
GUCEF_CORE_PUBLIC_CPP CString operator+( const char* lhs, const CString& rhs );
GUCEF_CORE_PUBLIC_CPP CString operator+( const char lhs, const CString& rhs );

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

- 29-03-2008 :
        - Dinand: Altered ParseElements() to use a vector to store the parsed elements
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
