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

#ifndef GUCEF_CORE_CANSISTRING_H
#define GUCEF_CORE_CANSISTRING_H

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

class GUCEF_CORE_PUBLIC_CPP CAnsiString
{
    public:

    typedef std::vector< CAnsiString >  StringVector;
    typedef std::set< CAnsiString >     StringSet;

    static const CAnsiString Empty;
    static const StringVector EmptyStringVector;

    CAnsiString( void );

    CAnsiString( const CAnsiString& src );

    #ifdef GUCEF_RVALUE_REFERENCES_SUPPORTED
    CAnsiString( CAnsiString&& src );
    #endif

    CAnsiString( const std::string& src );

    CAnsiString( const char *src );

    CAnsiString( const char *src ,
                 UInt32 length   );

    explicit CAnsiString( const char src );

    explicit CAnsiString( const int NULLvalue );

    ~CAnsiString();

    CAnsiString& operator=( const CAnsiString &src );

    CAnsiString& operator=( const std::string& src );
    
    /**
     *      Copys the source string.
     *      If the src string is NULL then the current string will be
     *      deallocated and also set to NULL.
     */
    CAnsiString& operator=( const char *src );

    CAnsiString& operator+=( const CAnsiString &other );

    CAnsiString& operator+=( const char *other );

    CAnsiString& operator+=( char lastchar );

    CAnsiString operator+( const CAnsiString& addition ) const;

    CAnsiString operator+( const char* addition ) const;

    CAnsiString operator+( const char addition ) const;

    bool operator==( const CAnsiString &other ) const;

    bool operator==( const char *other ) const;

    bool operator==( const int NULLvalue ) const;

    bool operator==( const char character ) const;

    bool operator!=( const CAnsiString &other ) const;

    bool operator!=( const char *other ) const;

    bool operator!=( const int NULLvalue ) const;

    bool operator!=( const char character ) const;

    bool operator<( const CAnsiString& other ) const;

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

    CAnsiString Lowercase( void ) const;

    CAnsiString Uppercase( void ) const;

    CAnsiString ReplaceChar( char oldchar ,
                         char newchar ) const;

    CAnsiString ReplaceSubstr( const CAnsiString& substr      ,
                           const CAnsiString& replacement ) const;

    /**
     *  Replaces an enveloping substring with another
     *  Example: "$MyEnvelope(" ")" -> "%{" "}"
     */
    CAnsiString ReplaceEnvelopingSubstr( const CAnsiString& envelopPrefix     ,
                                         const CAnsiString& envelopPostfix    ,
                                         const CAnsiString& newEnvelopPrefix  ,
                                         const CAnsiString& newEnvelopPostfix ) const;

    CAnsiString ReplaceSubStr( UInt32 startIndex        ,
                               UInt32 length            ,
                               const CAnsiString& newSubstr ) const;

    CAnsiString SubstrToChar( char searchchar         ,
                              bool frontToBack = true ) const;

    CAnsiString SubstrToChar( char searchchar                        ,
                              UInt32 startIndex                      ,
                              bool frontToBack = true                ,
                              bool returnEmptyIfCharNotFound = false ) const;

    CAnsiString SubstrToSubstr( const CAnsiString& searchstr ,
                                bool frontToBack = true  ) const;

    CAnsiString SubstrToSubstr( const CAnsiString& searchstr ,
                                UInt32 startIndex        ,
                                bool frontToBack = true  ) const;

    CAnsiString SubstrToIndex( UInt32 index            ,
                               bool frontToBack = true ) const;

    /**
     *  As per common convention:
     *  startIndex is inclusive and endIndex is exclusive
     *  Put another way: the char pointed to by endIndex will NOT be included
     */
    CAnsiString SubstrFromRange( UInt32 startIndex ,
                                 UInt32 endIndex   ) const;

    CAnsiString Trim( bool frontToBack ) const;

    Int32 HasChar( char searchchar         ,
                   bool frontToBack = true ) const;

    Int32 HasChar( char searchchar         ,
                   const UInt32 startIndex ,
                   bool frontToBack        ) const;

    Int32 HasSubstr( const CAnsiString& substr   ,
                     Int32 startIndex        ,
                     bool frontToBack = true ) const;

    Int32 HasSubstr( const CAnsiString& substr   ,
                     bool frontToBack = true ) const;

    UInt32 GetCharacterCount( const char searchChar ) const;

    UInt32 FindMaxSubstrEquality( const CAnsiString& searchStr ,
                                  const UInt32 startOffset ,
                                  bool frontToBack         ,
                                  bool isCaseSentive       ) const;

    CAnsiString CutChars( UInt32 charcount        ,
                          bool frontToBack = true ,
                          UInt32 startOffset = 0  ) const;

    CAnsiString RemoveChar( const char charToRemove ) const;

    CAnsiString CompactRepeatingChar( const char charToCompact ) const;

    StringVector ParseElements( char seperator               ,
                                bool addEmptyElements = true ) const;

    StringSet ParseUniqueElements( char seperator               ,
                                   bool addEmptyElements = true ) const;

    bool WildcardEquals( const CAnsiString& strWithWildcards  ,
                         const char wildCardToken = '*'   ,
                         const bool caseSensitive = true  ,
                         const bool biDirectional = false ) const;

    bool Equals( const CAnsiString& otherStr        ,
                 const bool caseSensitive = true ) const;

    bool NotEquals( const CAnsiString& otherStr         ,
                    const bool caseSensitive = true ) const;

    CAnsiString Combine( const StringVector& elements, char seperator ) const;
    
    void Clear( void );

    private:
    char* m_string;    /**< our actual null-terminated string */
    UInt32 m_length;   /**< length of the string */
};

/*-------------------------------------------------------------------------*/

GUCEF_CORE_PUBLIC_CPP bool operator!=( const char* lhs, const CAnsiString& rhs );
GUCEF_CORE_PUBLIC_CPP bool operator==( const char* lhs, const CAnsiString& rhs );
GUCEF_CORE_PUBLIC_CPP CAnsiString operator+( const char* lhs, const CAnsiString& rhs );
GUCEF_CORE_PUBLIC_CPP CAnsiString operator+( const char lhs, const CAnsiString& rhs );

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CANSISTRING_H ? */
