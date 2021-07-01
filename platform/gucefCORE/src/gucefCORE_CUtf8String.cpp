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

#include <string.h>
#include <assert.h>

#include "gucefCORE_utf8.h"

#ifndef GUCEF_CORE_DVSTRUTILS_H
#include "dvstrutils.h"
#define GUCEF_CORE_DVSTRUTILS_H
#endif /* GUCEF_CORE_DVSTRUTILS_H ? */

#ifndef GUCEF_CORE_CASCIISTRING_H
#include "gucefCORE_CAsciiString.h"
#define GUCEF_CORE_CASCIISTRING_H
#endif /* GUCEF_CORE_CASCIISTRING_H ? */

#include "gucefCORE_CUtf8String.h"

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CUtf8String CUtf8String::Empty;
const CUtf8String::StringVector CUtf8String::EmptyStringVector;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CUtf8String::CUtf8String( void )
    : m_string( GUCEF_NULL )
    , m_length( 0 )
    , m_byteSize( 0 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CUtf8String::CUtf8String( const CUtf8String& src )
    : m_string( GUCEF_NULL )
    , m_length( 0 )
    , m_byteSize( 0 )
{GUCEF_TRACE;

    Set( src.C_String(), src.ByteSize(), src.Length() );
}

/*-------------------------------------------------------------------------*/

CUtf8String::CUtf8String( const CAsciiString& src )
    : m_string( GUCEF_NULL )
    , m_length( 0 )
    , m_byteSize( 0 )
{GUCEF_TRACE;

    Set( src.C_String(), src.ByteSize(), src.Length() );
}

/*-------------------------------------------------------------------------*/

CUtf8String::CUtf8String( const std::string& src )
    : m_string( GUCEF_NULL )
    , m_length( 0 )
    , m_byteSize( 0 )
{GUCEF_TRACE;

    if ( src.size() > 0 )
    {
        Set( src.c_str(), (UInt32) ( sizeof( std::string::value_type ) * ( src.size() + 1 ) ) );
    }
}

/*-------------------------------------------------------------------------*/
#ifdef GUCEF_RVALUE_REFERENCES_SUPPORTED

CUtf8String::CUtf8String( CUtf8String&& src ) GUCEF_NOEXCEPT
    : m_string( src.m_string )
    , m_length( src.m_length )
    , m_byteSize( src.m_byteSize )
{GUCEF_TRACE;

    src.m_string = GUCEF_NULL;
    src.m_length = 0;
    src.m_byteSize = 0;
}

#endif
/*-------------------------------------------------------------------------*/

CUtf8String::CUtf8String( const char* src )
    : m_string( GUCEF_NULL )
    , m_length( 0 )
    , m_byteSize( 0 )
{GUCEF_TRACE;

    Set( src );
}

/*-------------------------------------------------------------------------*/

CUtf8String::CUtf8String( const char *src ,
                          UInt32 byteSize )
    : m_string( GUCEF_NULL )
    , m_length( 0 )
    , m_byteSize( 0 )
{GUCEF_TRACE;

    Set( src, byteSize );
}

/*-------------------------------------------------------------------------*/

CUtf8String::CUtf8String( const char *src ,
                          UInt32 byteSize ,
                          UInt32 length   )
    : m_string( GUCEF_NULL )
    , m_length( 0 )
    , m_byteSize( 0 )
{GUCEF_TRACE;

    Set( src, byteSize, length );
}

/*-------------------------------------------------------------------------*/

CUtf8String::CUtf8String( const char src )
    : m_string( GUCEF_NULL )
    , m_length( 0 )
    , m_byteSize( 0 )
{GUCEF_TRACE;

    Set( &src, (UInt32) sizeof( src ) );
}

/*-------------------------------------------------------------------------*/

CUtf8String::CUtf8String( const Int32 NULLvalueOrUtf32 )
    : m_string( GUCEF_NULL )
    , m_length( 0 )
    , m_byteSize( 0 )
{GUCEF_TRACE;

    Set( (const char*) &NULLvalueOrUtf32, sizeof( NULLvalueOrUtf32 ) );
}
/*-------------------------------------------------------------------------*/

CUtf8String::~CUtf8String()
{GUCEF_TRACE;

    Clear();
}

/*-------------------------------------------------------------------------*/

CUtf8String&
CUtf8String::operator=( const CUtf8String &src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        Set( src.m_string, src.m_byteSize );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CUtf8String&
CUtf8String::operator=( const std::string& src )
{GUCEF_TRACE;

    Set( src.c_str(), (UInt32) ( sizeof( std::string::value_type ) * ( src.size() + 1 ) ) );
    return *this;
}

/*-------------------------------------------------------------------------*/

CUtf8String&
CUtf8String::operator=( const char *src )
{GUCEF_TRACE;

    Set( src );
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
CUtf8String::operator==( const char character ) const
{GUCEF_TRACE;

    return *this == CUtf8String( character );
}

/*-------------------------------------------------------------------------*/

bool
CUtf8String::operator!=( const char character ) const
{GUCEF_TRACE;

    return *this != CUtf8String( character );
}

/*-------------------------------------------------------------------------*/

bool
CUtf8String::operator!=( const Int32 NULLvalueOrUtf32 ) const
{GUCEF_TRACE;

    if ( NULLvalueOrUtf32 == 0 )
    {
        return m_length > 0;
    }

    // Treat as UTF32 code point, compress as needed for compatibility with UTF8 followed by
    // the actual compare
    char utfTempBuffer[ 5 ] = { 0, 0, 0, 0, 0 };
    utf8catcodepoint( utfTempBuffer, NULLvalueOrUtf32, 5 );
    return 0 != utf8cmp( m_string, utfTempBuffer );
}

/*-------------------------------------------------------------------------*/

bool
CUtf8String::operator==( const Int32 NULLvalueOrUtf32 ) const
{GUCEF_TRACE;

    if ( NULLvalueOrUtf32 == 0 )
    {
        return m_length == 0;
    }

    // Treat as UTF32 code point, compress as needed for compatibility with UTF8 followed by
    // the actual compare
    char utfTempBuffer[ 5 ] = { 0, 0, 0, 0, 0 };
    utf8catcodepoint( utfTempBuffer, NULLvalueOrUtf32, 5 );
    return 0 == utf8cmp( m_string, utfTempBuffer );
}

/*-------------------------------------------------------------------------*/

bool
CUtf8String::operator==( const char *other ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL == m_string && GUCEF_NULL == other )
    {
        return true;
    }
    if ( GUCEF_NULL == m_string || GUCEF_NULL == other )
    {
        return false;
    }
    return 0 == utf8cmp( m_string, other );
}

/*-------------------------------------------------------------------------*/

bool
CUtf8String::operator==( const CUtf8String &other ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL == m_string && GUCEF_NULL == other.m_string )
    {
        return true;
    }
    if ( GUCEF_NULL == m_string || GUCEF_NULL == other.m_string )
    {
        return false;
    }
    return 0 == utf8cmp( m_string, other.m_string );
}

/*-------------------------------------------------------------------------*/

bool
CUtf8String::operator!=( const CUtf8String &other ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL == m_string && GUCEF_NULL == other.m_string )
    {
        return false;
    }
    if ( GUCEF_NULL == m_string || GUCEF_NULL == other.m_string )
    {
        return true;
    }
    return 0 != utf8cmp( m_string, other.m_string );
}

/*-------------------------------------------------------------------------*/

bool
CUtf8String::operator!=( const char* other ) const
{GUCEF_TRACE;

    // Comparison against ASCII encoded string to UTF8 string
    // This works transparently due to UTF8 being a superset

    if ( GUCEF_NULL == m_string && GUCEF_NULL == other )
    {
        return false;
    }
    if ( GUCEF_NULL == m_string || GUCEF_NULL == other )
    {
        return true;
    }
    return 0 != utf8cmp( m_string, other );
}

/*-------------------------------------------------------------------------*/

Int32
CUtf8String::operator[]( const UInt32 index ) const
{GUCEF_TRACE;

    return CodepointAtIndex( index );
}

/*-------------------------------------------------------------------------*/

Int32
CUtf8String::CodepointAtIndex( const UInt32 index ) const
{GUCEF_TRACE;

    if ( index >= m_length )
        return 0;

    const char* cpPos = m_string;
    Int32 cp = 0;
    UInt32 i=0;
    do
    {
        cpPos = (const char*) utf8codepoint( cpPos, &cp );
        ++i;
    }
    while ( i<=index );
    return cp;
}

/*-------------------------------------------------------------------------*/

char*
CUtf8String::CodepointPtrAtIndex( const UInt32 index, UInt32& bytesFromStart ) const
{GUCEF_TRACE;

    bytesFromStart = 0;

    // We allow up to 'length' because the null terminator is also a
    // valid code point. Its also needed to support index-exclusive-ranging
    if ( index > m_length )
        return GUCEF_NULL;

    char* cpPos = m_string;
    Int32 cp = 0;
    for ( UInt32 i=0; i<index; ++i )
    {
        char* newCpPos = (char*) utf8codepoint( cpPos, &cp );
        bytesFromStart += (UInt32) ( newCpPos - cpPos );
        cpPos = newCpPos;
    }
    return (char*) cpPos;
}

/*-------------------------------------------------------------------------*/

bool
CUtf8String::operator<( const CUtf8String& other ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_string && GUCEF_NULL != other.m_string )
    {
        return utf8cmp( m_string, other.m_string ) < 0;
    }
    if ( GUCEF_NULL == m_string && GUCEF_NULL != other.m_string )
    {
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CUtf8String::IsNULLOrEmpty( void ) const
{GUCEF_TRACE;

    return 0 == m_length;
}

/*-------------------------------------------------------------------------*/

CUtf8String::operator std::string() const
{GUCEF_TRACE;

    if ( m_length > 0 )
    {
        return std::string( (const char*) m_string, m_byteSize );
    }
    return std::string();
}

/*-------------------------------------------------------------------------*/

void
CUtf8String::Set( const char* str           ,
                  UInt32 byteSize           ,
                  UInt32 lengthInCodePoints )
{GUCEF_TRACE;

    if ( GUCEF_NULL == str || 0 == byteSize || 0 == lengthInCodePoints )
    {
        Clear();
    }
    else
    {
        assert( byteSize >= lengthInCodePoints );

        // Protect against self-assignment
        if ( (void*) str != (void*) m_string )
        {
            if ( lengthInCodePoints > byteSize )
                lengthInCodePoints = byteSize-1;

            // Check for a null terminator
            if ( str[ byteSize-1 ] == '\0' )
            {
                if ( GUCEF_NULL != Reserve( byteSize ) )
                {
                    memcpy( m_string, str, byteSize );
                    m_length = lengthInCodePoints;
                }
            }
            else
            {
                // Add the null-terminator since input didnt have one
                if ( GUCEF_NULL != Reserve( byteSize+1 ) )
                {
                    memcpy( m_string, str, byteSize );
                    m_string[ byteSize ] = '\0';
                    m_length = lengthInCodePoints;
                }
            }
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CUtf8String::Set( const char* str ,
                  UInt32 byteSize )
{GUCEF_TRACE;

    if ( GUCEF_NULL == str || 0 == byteSize )
    {
        Clear();
    }
    else
    {
        // Protect against self-assignment
        if ( (void*) str != (void*) m_string )
        {
            // Check for a null terminator
            if ( str[ byteSize-1 ] == '\0' )
            {
                if ( GUCEF_NULL != Reserve( byteSize ) )
                {
                    memcpy( m_string, str, byteSize );
                    m_length = (UInt32) utf8len( m_string );
                }
            }
            else
            {
                // Add the null-terminator since input didnt have one
                if ( GUCEF_NULL != Reserve( byteSize+1 ) )
                {
                    memcpy( m_string, str, byteSize );
                    m_string[ byteSize ] = '\0';
                    m_length = (UInt32) utf8len( m_string );
                }
            }
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CUtf8String::Set( const char* str )
{GUCEF_TRACE;

    if ( GUCEF_NULL != str )
    {
        Set( str, (UInt32) utf8size( str ) );
    }
    else
    {
        Clear();
    }
}

/*-------------------------------------------------------------------------*/

UInt32
CUtf8String::GetCharacterCount( const Int32 searchChar ) const
{GUCEF_TRACE;

    UInt32 charCount = 0;
    void* cpPos = m_string;
    Int32 cp = 0;
    for ( UInt32 i=0; i<m_length; ++i )
    {
        cpPos = utf8codepoint( cpPos, &cp );
        if ( cp == searchChar )
        {
            ++charCount;
        }
    }
    return charCount;
}

/*-------------------------------------------------------------------------*/

UInt32
CUtf8String::GetCharactersCount( Int32* searchChars     ,
                                 UInt32 nrOfSearchChars ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL == searchChars )
        return 0;

    UInt32 charCount = 0;
    void* cpPos = m_string;
    Int32 cp = 0;
    for ( UInt32 i=0; i<m_length; ++i )
    {
        cpPos = utf8codepoint( cpPos, &cp );
        for ( UInt32 n=0; n<nrOfSearchChars; ++n )
        {
            if ( cp == searchChars[ n ] )
            {
                ++charCount;
            }
        }
    }
    return charCount;
}

/*-------------------------------------------------------------------------*/

UInt32
CUtf8String::GetCharacterRepeatCount( const Int32 searchChar ) const
{GUCEF_TRACE;

    UInt32 charRepeatCount = 0;
    void* cpPos = m_string;
    Int32 cp = 0;
    for ( UInt32 i=0; i<m_length; ++i )
    {
        cpPos = utf8codepoint( cpPos, &cp );
        if ( cp == searchChar )
        {
            ++i;
            for ( i; i<m_length; ++i )
            {
                cpPos = utf8codepoint( cpPos, &cp );
                if ( cp == searchChar )
                    ++charRepeatCount;
                else
                    break;
            }

        }
    }
    return charRepeatCount;
}

/*-------------------------------------------------------------------------*/

void
CUtf8String::Append( const char* appendstr ,
                     UInt32 byteSize       )
{GUCEF_TRACE;

    if ( appendstr == GUCEF_NULL || 0 == byteSize )
        return;

    if ( 0 == m_length )
    {
        Set( appendstr, byteSize );
        return;
    }

    // Check for a null terminator
    if ( appendstr[ byteSize-1 ] == '\0' )
    {
        UInt32 originalByteSize = m_byteSize;
        if ( GUCEF_NULL != Reserve( (m_byteSize-1) + byteSize ) )
        {
            memcpy( m_string+(originalByteSize-1), appendstr, byteSize );
            m_length = (UInt32) utf8len( m_string );
        }
    }
    else
    {
        // Add the null-terminator since input didnt have one
        UInt32 originalByteSize = m_byteSize;
        if ( GUCEF_NULL != Reserve( m_byteSize + byteSize ) )
        {
            memcpy( m_string+(originalByteSize-1), appendstr, byteSize );
            m_string[ m_byteSize-1 ] = '\0';
            m_length = (UInt32) utf8len( m_string );
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CUtf8String::Append( const char* appendstr )
{GUCEF_TRACE;

    if ( appendstr == GUCEF_NULL )
        return;

    Append( appendstr, (UInt32) utf8size( appendstr ) );
}

/*-------------------------------------------------------------------------*/

CUtf8String&
CUtf8String::operator+=( const CUtf8String &other )
{GUCEF_TRACE;


    Append( other.m_string, other.m_byteSize );
    return *this;
}

/*-------------------------------------------------------------------------*/

CUtf8String&
CUtf8String::operator+=( const char *other )
{GUCEF_TRACE;

    Append( other, (UInt32) utf8size( other ) );
    return *this;
}

/*-------------------------------------------------------------------------*/

CUtf8String&
CUtf8String::operator+=( char lastchar )
{GUCEF_TRACE;

    Append( &lastchar, 1 );
    return *this;
}


/*-------------------------------------------------------------------------*/

const char*
CUtf8String::C_String( void ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_string )
        return reinterpret_cast< const char* >( m_string );
    return "";
}

/*-------------------------------------------------------------------------*/

char*
CUtf8String::C_String( void )
{GUCEF_TRACE;

    return reinterpret_cast< char* >( m_string );
}

/*-------------------------------------------------------------------------*/

char*
CUtf8String::Reserve( const UInt32 byteSize, Int32 newLength )
{GUCEF_TRACE;

    char* newBuffer = (char*) realloc( m_string, byteSize );

    assert( GUCEF_NULL != newBuffer );
    if ( GUCEF_NULL != newBuffer )
    {
        m_string = newBuffer;

        if ( byteSize < m_byteSize )
            m_length = 0;
        if ( newLength >= 0 && (UInt32) newLength < byteSize )
        {
            m_length = (UInt32) newLength;            
            newBuffer[ m_length ] = '\0';
        }
        m_byteSize = byteSize;
    }
    else
    {
        m_byteSize = 0;
        m_length = 0;
    }
    return newBuffer;
}

/*-------------------------------------------------------------------------*/

std::string
CUtf8String::STL_String( void ) const
{GUCEF_TRACE;

    if ( m_length > 0 )
    {
        return std::string( reinterpret_cast< const char* >( m_string ), m_byteSize );
    }
    return std::string();
}

/*-------------------------------------------------------------------------*/

UInt32
CUtf8String::Length( void ) const
{GUCEF_TRACE;

    return m_length;
}

/*-------------------------------------------------------------------------*/

UInt32
CUtf8String::ByteSize( void ) const
{GUCEF_TRACE;

    return m_byteSize;
}

/*-------------------------------------------------------------------------*/

void
CUtf8String::SetLength( UInt32 newLength, UInt32 maxCodePointSize )
{GUCEF_TRACE;

    if ( 0 == newLength || 0 == maxCodePointSize )
    {
        Clear();
        return;
    }

    UInt32 originalByteSize = m_byteSize;
    if ( GUCEF_NULL != Reserve( newLength * maxCodePointSize ) )
    {
        if ( originalByteSize < m_byteSize )
        {
            memset( m_string+originalByteSize, 0, m_byteSize-originalByteSize );
        }
        else
        {
            if ( GUCEF_NULL != m_string && 0 < m_byteSize )
                m_string[ m_byteSize-1 ] = '\0'; // Garantee a null terminator
        }
        m_length = newLength;
    }
}

/*-------------------------------------------------------------------------*/

UInt32
CUtf8String::DetermineLength( void )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_string )
    {
        m_length = (UInt32) utf8len( m_string );
    }
    else
    {
        m_length = 0;
    }
    return m_length;
}

/*-------------------------------------------------------------------------*/

CUtf8String
CUtf8String::ReplaceChar( Int32 oldchar ,
                          Int32 newchar ) const
{GUCEF_TRACE;

    // Calculate new space required

    Int32 codePointSizeDiff = (Int32) ( utf8codepointsize( newchar ) - utf8codepointsize( oldchar ) );
    Int32 charsToReplace = (Int32) GetCharacterCount( oldchar );

    if ( 0 < charsToReplace )
    {
        // Allocate enough space

        Int32 bufferSize = (Int32)m_byteSize + ( charsToReplace * codePointSizeDiff );
        Int32 bufferSpaceLeft = bufferSize;
        CUtf8String newStr;
        if ( 0 < bufferSize && GUCEF_NULL != newStr.Reserve( bufferSize ) )
        {
            // Iterate all current string code points replacing as we go

            const char* srcCpPos = m_string;
            char* destCpPos = newStr.m_string;
            Int32 cp = 0;
            newStr.m_string[ 0 ] = '\0';
            for ( UInt32 i=0; i<m_length; ++i )
            {
                // Grab code point from current string
                srcCpPos = (const char*) utf8codepoint( srcCpPos, &cp );
                char* newDestCpPos = GUCEF_NULL;
                if ( cp != oldchar )
                {
                    // This one doesnt need replacing, just concat it into the pre-allocated destination buffer
                    newDestCpPos = (char*) utf8catcodepoint( destCpPos, cp, (size_t) bufferSpaceLeft );
                }
                else
                {
                    // This one needs replacing, just concat the given new code point into the pre-allocated destination buffer
                    newDestCpPos = (char*) utf8catcodepoint( destCpPos, newchar, (size_t) bufferSpaceLeft );
                }
                bufferSpaceLeft -= (Int32)( newDestCpPos - destCpPos );
                destCpPos = newDestCpPos;
            }
            newStr.m_length = m_length;
            newStr.m_string[ newStr.m_byteSize-1 ] = '\0';
            return newStr;
        }
        else
        {
            GUCEF_ASSERT_ALWAYS;
            newStr.Clear();
            return newStr;
        }
    }
    else
    {
        return *this;
    }
}

/*-------------------------------------------------------------------------*/

CUtf8String
CUtf8String::ReplaceChars( Int32* oldchars     ,
                           UInt32 nrOfOldChars ,
                           Int32 newchar       ) const
{GUCEF_TRACE;

    // @TODO: Optimize

    CUtf8String result( *this );
    for ( UInt32 i=0; i<nrOfOldChars; ++i )
    {
        result = result.ReplaceChar( oldchars[ i ], newchar );
    }
    return result;
}

/*-------------------------------------------------------------------------*/

CUtf8String
CUtf8String::ReplaceSubstr( const CUtf8String& substr      ,
                            const CUtf8String& replacement ) const
{GUCEF_TRACE;

    CUtf8String testStr = *this;
    CUtf8String newStr;
    Int32 subStrIndex = 0;
    do
    {
        subStrIndex = testStr.HasSubstr( substr, true );
        if ( subStrIndex >= 0 )
        {
            UInt32 bytesFromStart=0;
            testStr.CodepointPtrAtIndex( subStrIndex, bytesFromStart );
            newStr.Append( testStr.C_String(), bytesFromStart );
            newStr += replacement;
            testStr = testStr.CutChars( (UInt32)subStrIndex + substr.Length(), true );
        }
    }
    while ( subStrIndex >= 0 );
    newStr += testStr;

    return newStr;
}

/*-------------------------------------------------------------------------*/

CUtf8String
CUtf8String::ReplaceEnvelopingSubstr( const CUtf8String& envelopPrefix     ,
                                      const CUtf8String& envelopPostfix    ,
                                      const CUtf8String& newEnvelopPrefix  ,
                                      const CUtf8String& newEnvelopPostfix ) const
{GUCEF_TRACE;

    CUtf8String resultStr;

    Int32 startIndex = 0;
    Int32 envSegIndex = HasSubstr( envelopPrefix, startIndex, true );
    while ( envSegIndex >= 0 )
    {
        if ( startIndex < envSegIndex )
        {
            resultStr += SubstrFromRange( startIndex, envSegIndex );
        }

        envSegIndex+=envelopPrefix.Length();
        CUtf8String envelopedSegment = SubstrToSubstr( envelopPostfix, envSegIndex, true );
        resultStr += newEnvelopPrefix + envelopedSegment + newEnvelopPostfix;

        startIndex = envSegIndex+envelopedSegment.Length()+envelopPostfix.Length();
        envSegIndex = HasSubstr( envelopPrefix, startIndex, true );
    }

    // append the remainder, if any
    resultStr += SubstrToIndex( startIndex, false );

    return resultStr;
}

/*-------------------------------------------------------------------------*/

CUtf8String
CUtf8String::ReplaceSubStr( UInt32 startIndex            ,
                            UInt32 length                ,
                            const CUtf8String& newSubstr ) const
{GUCEF_TRACE;

    // Sanity check on the given range
    if ( 0 == m_length )
        return CUtf8String();
    if ( startIndex >= m_length )
        startIndex = m_length -1;
    if ( startIndex+length >= m_length )
        length = (m_length-1) - startIndex;

    CUtf8String prefix = SubstrFromRange( 0, startIndex );
    CUtf8String postfix = SubstrFromRange( startIndex+length+1, m_length );

    return prefix + newSubstr + postfix;
}

/*-------------------------------------------------------------------------*/

void
CUtf8String::Clear( void )
{GUCEF_TRACE;

    m_length = 0;
    m_byteSize = 0;
    if ( GUCEF_NULL != m_string )
    {
        free( m_string );
        m_string = GUCEF_NULL;
    }
}

/*-------------------------------------------------------------------------*/

CUtf8String
CUtf8String::SubstrToChar( Int32 searchchar               ,
                           UInt32 startIndex              ,
                           bool frontToBack               ,
                           bool returnEmptyIfCharNotFound ) const
{GUCEF_TRACE;

    if ( startIndex < m_length )
    {
        if ( frontToBack )
        {
            UInt32 bytesFromStart = 0;
            const char* startPos = CodepointPtrAtIndex( startIndex, bytesFromStart );
            const char* cpPos = startPos;
            const char* prevCpPos = cpPos;
            Int32 codePoint = 0;
            for ( UInt32 i=startIndex; i<m_length; ++i )
            {
                cpPos = (const char*) utf8codepoint( prevCpPos, &codePoint );
                if ( codePoint == searchchar )
                {
                    CUtf8String substr;
                    substr.Set( startPos, (UInt32)(prevCpPos - startPos), i-startIndex );
                    return substr;
                }
                prevCpPos = cpPos;
            }
            if ( returnEmptyIfCharNotFound )
                return CUtf8String();
            else
                return CUtf8String( *this );
        }
        else
        {
            UInt32 bytesFromStart = 0;
            const char* startPos = CodepointPtrAtIndex( startIndex, bytesFromStart );
            const char* cpPos = startPos;
            const char* prevCpPos = cpPos;
            Int32 codePoint = 0;
            for ( Int32 i=startIndex; i>=0; --i )
            {
                cpPos = (const char*) utf8rcodepoint( prevCpPos, &codePoint );
                if ( codePoint == searchchar )
                {
                    const char* prevPrevCpPos = (const char*) utf8codepoint( prevCpPos, &codePoint );
                    CUtf8String substr;
                    substr.Set( prevPrevCpPos, (UInt32)(startPos - prevCpPos), startIndex-i );
                    return substr;
                }
                prevCpPos = cpPos;
            }
            if ( returnEmptyIfCharNotFound )
                return CUtf8String();
            else
                return CUtf8String( *this );
        }
    }
    return CUtf8String();
}

/*-------------------------------------------------------------------------*/

CUtf8String
CUtf8String::SubstrToIndex( UInt32 index     ,
                            bool frontToBack ) const
{GUCEF_TRACE;

    if ( !frontToBack )
    {
         if ( index >= m_length )
            return CUtf8String();
         return SubstrFromRange( index, m_length );
    }

    if ( index >= m_length )
        return CUtf8String( m_string, m_length );
    return SubstrFromRange( 0, index );
}

/*-------------------------------------------------------------------------*/

CUtf8String
CUtf8String::SubstrFromRange( UInt32 startIndex ,
                              UInt32 endIndex   ) const
{GUCEF_TRACE;

    if ( startIndex == endIndex )
        return CUtf8String();

    // we want the user to be able to pass a range conveniently
    if  ( startIndex > endIndex )
    {
        // swap
        UInt32 swapIndex = startIndex;
        startIndex = endIndex;
        endIndex = swapIndex;
    }

    // gracefully protect against out-of-bounds index
    // keep in mind that endIndex is exclusive, thus out of bounds is endIndex
    // beyond the null terminator
    UInt32 maxEnd = endIndex > m_length ? m_length : endIndex;
    UInt32 maxStart = startIndex > m_length ? m_length : startIndex;

    // Check for 0 length string
    if ( maxEnd >= maxStart )
    {
        // make the new string using the given range
        UInt32 bytesFromStartAtStart = 0;
        UInt32 bytesFromStartAtEnd = 0;
        const char* subStrStartPtr = CodepointPtrAtIndex( maxStart, bytesFromStartAtStart );
        CodepointPtrAtIndex( maxEnd, bytesFromStartAtEnd );

        // create the string from the sub-segment, a null terminator will be auto added
        return CUtf8String( subStrStartPtr, bytesFromStartAtEnd-bytesFromStartAtStart );
    }
    return CUtf8String();
}

/*-------------------------------------------------------------------------*/

CUtf8String
CUtf8String::SubstrToChar( Int32 searchchar ,
                           bool frontToBack ) const
{GUCEF_TRACE;

    if ( frontToBack )
    {
        return SubstrToChar( searchchar, 0, frontToBack );
    }
    return SubstrToChar( searchchar, m_length-1, frontToBack );
}

/*-------------------------------------------------------------------------*/

CUtf8String
CUtf8String::Trim( bool frontToBack ) const
{GUCEF_TRACE;

    if ( m_length > 0 )
    {
        if ( frontToBack )
        {
            UInt32 charsToCut = 0;
            void* cpPos = m_string;
            Int32 cp = 0;
            UInt32 i=0;
            do
            {
                cpPos = utf8codepoint( cpPos, &cp );
                if ( cp == ' ' ||  cp == '\t' )
                {
                    ++charsToCut;
                }
                else
                {
                    break;
                }
                ++i;
            }
            while ( i<m_length );

            return CutChars( charsToCut, frontToBack );
        }
        else
        {
            UInt32 bytesFromStart = 0;
            UInt32 charsToCut = 0;
            void* cpPos = CodepointPtrAtIndex( m_length-1, bytesFromStart );
            Int32 cp = 0;
            Int32 i = (Int32) m_length-1;
            do
            {
                cpPos = utf8rcodepoint( cpPos, &cp );
                if ( cp == ' ' ||  cp == '\t' )
                {
                    ++charsToCut;
                }
                else
                {
                    break;
                }
                --i;
            }
            while ( i>=0 );

            return CutChars( charsToCut, frontToBack );
        }
    }
    return CUtf8String();
}

/*-------------------------------------------------------------------------*/

CUtf8String
CUtf8String::SubstrToSubstr( const CUtf8String& searchstr ,
                             bool startfront              ) const
{GUCEF_TRACE;

    if ( searchstr.Length() > m_length || 0 == searchstr.Length() || 0 == m_length )
    {
        return *this;
    }

    if ( startfront )
    {
        return SubstrToSubstr( searchstr, 0, startfront );
    }

    return SubstrToSubstr( searchstr, m_length-1, startfront );
}

/*-------------------------------------------------------------------------*/

CUtf8String
CUtf8String::SubstrToSubstr( const CUtf8String& searchstr ,
                             UInt32 startIndex            ,
                             bool startfront              ) const
{GUCEF_TRACE;

    Int32 subStrIndex = HasSubstr( searchstr, startIndex, startfront );
    if ( subStrIndex >= 0 )
    {
        if ( startfront )
            return SubstrFromRange( startIndex, subStrIndex );
        else
            return SubstrFromRange( subStrIndex + searchstr.m_length, startIndex+1 );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CUtf8String
CUtf8String::RemoveChar( const Int32 charToRemove ) const
{GUCEF_TRACE;

    // Calculate new space required

    Int32 codePointSize = (Int32) utf8codepointsize( charToRemove );
    Int32 charsToRemove = (Int32) GetCharacterCount( charToRemove );

    if ( 0 < charsToRemove )
    {
        // Allocate enough space

        Int32 bufferSize = (Int32)m_byteSize - ( charsToRemove * codePointSize );
        Int32 bufferSpaceLeft = bufferSize;
        CUtf8String newStr;
        if ( 0 < bufferSize && GUCEF_NULL != newStr.Reserve( bufferSize ) )
        {
            // Iterate all current string code points skipping matches as we go

            const char* srcCpPos = m_string;
            char* destCpPos = newStr.m_string;
            Int32 cp = 0;
            newStr.m_string[ 0 ] = '\0';
            for ( UInt32 i=0; i<m_length; ++i )
            {
                // Grab code point from current string
                srcCpPos = (const char*) utf8codepoint( srcCpPos, &cp );
                if ( cp != charToRemove )
                {
                    // This one can stay, just concat it into the pre-allocated destination buffer
                    char* newDestCpPos = (char*) utf8catcodepoint( destCpPos, cp, (size_t) bufferSpaceLeft );
                    bufferSpaceLeft -= (Int32)( newDestCpPos - destCpPos );
                    destCpPos = newDestCpPos;
                }
                // else: This one needs removal, just skip the copy into the pre-allocated destination buffer
            }
            newStr.m_length = (UInt32)( m_length - charsToRemove );
            newStr.m_string[ newStr.m_byteSize-1 ] = '\0';
            return newStr;
        }
        else
        {
            GUCEF_ASSERT_ALWAYS;
            return newStr;
        }
    }
    else
    {
        return *this;
    }
}

/*-------------------------------------------------------------------------*/

CUtf8String
CUtf8String::CompactRepeatingChar( const Int32 charToCompact ) const
{GUCEF_TRACE;

    // Calculate new space required

    Int32 codePointSize = (Int32) utf8codepointsize( charToCompact );
    Int32 charsToRemove = (Int32) GetCharacterRepeatCount( charToCompact );

    if ( 0 < charsToRemove )
    {
        // Allocate enough space

        Int32 bufferSize = (Int32)m_byteSize - ( charsToRemove * codePointSize );
        Int32 bufferSpaceLeft = bufferSize;
        CUtf8String newStr;
        if ( 0 < bufferSize && GUCEF_NULL != newStr.Reserve( bufferSize ) )
        {
            // Iterate all current string code points skipping repeat matches as we go

            const char* srcCpPos = m_string;
            char* destCpPos = newStr.m_string;
            Int32 cp = 0;
            newStr.m_string[ 0 ] = '\0';
            for ( UInt32 i=0; i<m_length; ++i )
            {
                // Grab code point from current string
                srcCpPos = (const char*) utf8codepoint( srcCpPos, &cp );

                // This one can stay, just concat it into the pre-allocated destination buffer
                char* newDestCpPos = (char*) utf8catcodepoint( destCpPos, cp, (size_t) bufferSpaceLeft );
                bufferSpaceLeft -= (Int32)( newDestCpPos - destCpPos );
                destCpPos = newDestCpPos;

                if ( cp == charToCompact )
                {
                    ++i;
                    for ( i; i<m_length; ++i )
                    {
                        // Grab code point from current string
                        srcCpPos = (const char*) utf8codepoint( srcCpPos, &cp );

                        // Skip all that now match the code point we wish to compact
                        if ( cp != charToCompact )
                        {
                            // This one can stay, just concat it into the pre-allocated destination buffer
                            newDestCpPos = (char*) utf8catcodepoint( destCpPos, cp, (size_t) bufferSpaceLeft );
                            bufferSpaceLeft -= (Int32)( newDestCpPos - destCpPos );
                            destCpPos = newDestCpPos;

                            // This was the end of the repeat series since a non-matching code point was found
                            break;
                        }
                    }
                }
            }
            newStr.m_length = m_length - charsToRemove;
            newStr.m_string[ newStr.m_byteSize-1 ] = '\0';
            return newStr;
        }
        else
        {
            GUCEF_ASSERT_ALWAYS;
            newStr.Clear();
            return newStr;
        }
    }
    else
    {
        return *this;
    }
}

/*-------------------------------------------------------------------------*/

CUtf8String
CUtf8String::CutChars( UInt32 charcount   ,
                       bool startfront    ,
                       UInt32 startOffset ) const
{GUCEF_TRACE;

    if ( startfront )
    {
        if ( 0 == startOffset )
        {
            if ( charcount < m_length )
            {
                return SubstrFromRange( charcount, m_length );
            }

            return CUtf8String::Empty;
        }

        return SubstrFromRange( 0, startOffset ) + SubstrFromRange( startOffset+charcount, m_length );
    }
    else
    {
        if ( 0 == startOffset )
        {
            if ( charcount < m_length )
            {
                return SubstrFromRange( 0, m_length-charcount );
            }

            return CUtf8String::Empty;
        }

        return SubstrFromRange( 0, m_length-startOffset-charcount ) + SubstrFromRange( m_length-1-startOffset, m_length );
    }
}

/*-------------------------------------------------------------------------*/

CUtf8String
CUtf8String::Lowercase( void ) const
{GUCEF_TRACE;

    if ( 0 < m_length && GUCEF_NULL != m_string )
    {
        CUtf8String lwrCopy( *this );
        utf8lwr( lwrCopy.m_string );
        return lwrCopy;
    }
    return CUtf8String();
}

/*-------------------------------------------------------------------------*/

CUtf8String
CUtf8String::Uppercase( void ) const
{GUCEF_TRACE;

    if ( 0 < m_length && GUCEF_NULL != m_string )
    {
        CUtf8String uprCopy( *this );
        utf8upr( uprCopy.m_string );
        return uprCopy;
    }
    return CUtf8String();
}

/*-------------------------------------------------------------------------*/

Int32
CUtf8String::HasChar( Int32 searchchar        ,
                      const UInt32 startIndex ,
                      bool frontToBack        ) const
{GUCEF_TRACE;

    if ( startIndex >= m_length )
        return -1;

    if ( frontToBack )
    {
        void* cpPos = m_string;
        Int32 cp = 0;
        UInt32 i=0;
        do
        {
            cpPos = utf8codepoint( cpPos, &cp );
            if ( i >= startIndex && cp == searchchar )
                return i;
            ++i;
        }
        while ( i<m_length );
        return -1;
    }
    else
    {
        UInt32 bytesFromStart = 0;
        Int32 max = startIndex > m_length-1 ? m_length-1 : startIndex;
        void* cpPos = CodepointPtrAtIndex( max, bytesFromStart );
        Int32 cp = 0;
        Int32 i = (Int32) max;
        do
        {
            cpPos = utf8rcodepoint( cpPos, &cp );
            if ( cp == searchchar )
                return i;
            --i;
        }
        while ( i>=0 );
        return -1;
    }
}

/*-------------------------------------------------------------------------*/

Int32
CUtf8String::HasChar( Int32 searchchar ,
                      bool startfront  ) const
{GUCEF_TRACE;

    if ( startfront )
    {
        return HasChar( searchchar, 0, startfront );
    }

    if ( m_length > 0 )
    {
        return HasChar( searchchar, m_length-1, startfront );
    }
    return -1;
}

/*-------------------------------------------------------------------------*/

CUtf8String::StringSet
CUtf8String::ParseUniqueElements( Int32 seperator       ,
                                  bool addEmptyElements ) const
{GUCEF_TRACE;

    if ( m_length > 0 )
    {
        StringSet list;
        CUtf8String entry;

        char* cpPos = m_string;
        UInt32 lastSepCpIndex = 0;
        char* lastSepCpPtr = m_string;
        UInt32 i=0;
        do
        {
            Int32 codePoint=0;
            char* newCpPos = (char*) utf8codepoint( cpPos, &codePoint );

            if ( codePoint == seperator )
            {
                UInt32 stringLength = i-lastSepCpIndex;
                if ( ( 0 == stringLength && addEmptyElements ) ||
                     ( stringLength > 0 ) )
                {
                    entry.Set( lastSepCpPtr, (UInt32)(cpPos-lastSepCpPtr), stringLength );
                    list.insert( entry );
                }

                lastSepCpIndex = i+1;
                lastSepCpPtr = newCpPos;
            }

            cpPos = newCpPos;
            ++i;
        }
        while ( i<m_length );

        // add last item
        UInt32 stringLength = m_length-lastSepCpIndex;
        if ( ( 0 == stringLength && addEmptyElements ) ||
             ( stringLength > 0 ) )
        {
            entry.Set( lastSepCpPtr, (UInt32)(cpPos-lastSepCpPtr), stringLength );
            list.insert( entry );
        }
        return list;
    }
    return StringSet();
}

/*-------------------------------------------------------------------------*/

CUtf8String::StringVector
CUtf8String::ParseElements( Int32 seperator       ,
                            bool addEmptyElements ) const
{GUCEF_TRACE;

    if ( m_length > 0 )
    {
        StringVector list;
        CUtf8String entry;

        char* cpPos = m_string;
        UInt32 lastSepCpIndex = 0;
        char* lastSepCpPtr = m_string;
        UInt32 i=0;
        do
        {
            Int32 codePoint=0;
            char* newCpPos = (char*) utf8codepoint( cpPos, &codePoint );

            if ( codePoint == seperator )
            {
                UInt32 stringLength = i-lastSepCpIndex;
                if ( ( 0 == stringLength && addEmptyElements ) ||
                     ( stringLength > 0 ) )
                {
                    entry.Set( lastSepCpPtr, (UInt32)(cpPos-lastSepCpPtr), stringLength );
                    list.push_back( entry );
                }

                lastSepCpIndex = i+1;
                lastSepCpPtr = newCpPos;
            }

            cpPos = newCpPos;
            ++i;
        }
        while ( i<m_length );

        // add last item
        UInt32 stringLength = m_length-lastSepCpIndex;
        if ( ( 0 == stringLength && addEmptyElements ) ||
             ( stringLength > 0 ) )
        {
            entry.Set( lastSepCpPtr, (UInt32)(cpPos-lastSepCpPtr), stringLength );
            list.push_back( entry );
        }
        return list;
    }
    return StringVector();
}

/*-------------------------------------------------------------------------*/

UInt32
CUtf8String::FindMaxSubstrEquality( const CUtf8String& searchStr ,
                                    const UInt32 startOffset     ,
                                    bool startFront              ,
                                    bool isCaseSentive           ) const
{GUCEF_TRACE;

    // Sanity check on the startOffset
    if ( (Int32)m_length - startOffset > 0 )
    {
        // Here we want to be able to support case insensitive compares
        // without having to suffer the performance penalty of making string lowercase
        // when not doing case insensitive compares. This we do some pointer magic to
        // avoid this overhead when we can
        CUtf8String lowercaseSearchStrStorage;
        CUtf8String lowercaseThisStrStorage;
        const CUtf8String* theSearchStr = &searchStr;
        const CUtf8String* thisStr = this;

        if ( !isCaseSentive )
        {
            // Caller wants a case-insensitive compare,..
            // take the performance hit
            lowercaseSearchStrStorage = searchStr.Lowercase();
            theSearchStr = &lowercaseSearchStrStorage;
            lowercaseThisStrStorage = Lowercase();
            thisStr = &lowercaseThisStrStorage;
        }

        // Get the smallest of the 2 buffer limits
        UInt32 maxMatchLength = theSearchStr->m_length;
        if ( thisStr->m_length - startOffset < maxMatchLength )
        {
            maxMatchLength = thisStr->m_length - startOffset;
        }

        if ( startFront )
        {
            // Loop trough the buffer growing our comparison string
            UInt32 bytesFromStart=0;
            const char* startPos = thisStr->CodepointPtrAtIndex( startOffset, bytesFromStart );
            const char* cpPos = startPos;
            UInt32 subLength=1;
            size_t subByteSize = utf8codepointcalcsize( cpPos );
            while ( subLength<=maxMatchLength )
            {
                if ( memcmp( startPos, theSearchStr->m_string, subByteSize ) != 0 )
                {
                    // Reached the maximum equality length
                    return subLength-1;
                }

                // Grow the comparison segment enough bytes to fit 1 extra codepoint
                Int32 cp;
                cpPos = (const char*) utf8codepoint( cpPos, &cp );
                subByteSize = (UInt32)( cpPos - startPos );

                ++subLength;
            }
            return subLength-1;
        }
        else
        {
            // Loop trough the buffer growing our comparison string
            UInt32 bytesFromStart=0;
            const char* startPos = thisStr->CodepointPtrAtIndex( thisStr->m_length-1-startOffset, bytesFromStart );
            const char* cpPos = startPos;
            const char* otherString = theSearchStr->CodepointPtrAtIndex( theSearchStr->m_length-1-startOffset, bytesFromStart );

            UInt32 subLength=1;
            size_t subByteSize = utf8codepointcalcsize( cpPos );
            while ( subLength<=maxMatchLength )
            {
                if ( memcmp( startPos-subByteSize, otherString-subByteSize, subByteSize ) != 0 )
                {
                    // Reached the maximum equality length
                    return subLength-1;
                }

                // Grow the comparison segment enough bytes to fit 1 extra codepoint
                Int32 cp;
                cpPos = (const char*) utf8rcodepoint( cpPos, &cp );
                subByteSize = (UInt32)( startPos - cpPos );

                ++subLength;
            }
            return subLength-1;
        }
    }

    // Unable to find an equality
    return 0;
}

/*-------------------------------------------------------------------------*/

Int32
CUtf8String::CodepointIndexAtPtr( const char* subStrPtr, Int32& codePoint ) const
{GUCEF_TRACE;

     codePoint = 0;

     // Sanity check on the pointer
     if ( m_string > subStrPtr || m_string + m_byteSize < subStrPtr )
        return -1;

    char* cpPos = m_string;
    UInt32 i=0;
    do
    {
        // Check for exact offset match
        if ( cpPos == subStrPtr )
            return (Int32) i;

        char* newCpPos = (char*) utf8codepoint( cpPos, &codePoint );

        // The pointer given could be at an offset inside the boundaries of a single multi-byte codepoint
        // in this case we effectively rewind back to the start offset of the current code point index wise
        if ( newCpPos > subStrPtr )
            return (Int32) i;

        cpPos = newCpPos;
        ++i;
    }
    while ( i<m_length );

    codePoint = 0;
    return -1;
}

/*-------------------------------------------------------------------------*/

Int32
CUtf8String::HasSubstr( const CUtf8String& substr ,
                        Int32 startIndex          ,
                        bool startfront           ) const
{GUCEF_TRACE;

    // Sanity check on the startindex range
    if ( startIndex < 0 || (UInt32) startIndex > m_length )
        return -1;

    // Cheap check for obvious mismatches
    if ( substr.m_length > m_length || substr.m_byteSize > m_byteSize )
    {
        return -1;
    }

    if ( startfront )
    {
        // Sanity check on the startindex range
        if ( ((UInt32)startIndex) + substr.m_length > m_length )
            return -1;

        UInt32 bytesFromStart = 0;
        char* cpPos = CodepointPtrAtIndex( startIndex, bytesFromStart );
        UInt32 maxByteOffset = m_byteSize - substr.m_byteSize;

        for ( UInt32 i=bytesFromStart; i<=maxByteOffset; ++i )
        {
            if ( 0 == memcmp( m_string+i, substr.m_string, substr.m_byteSize-1 ) )
            {
                Int32 codePoint = 0;
                return CodepointIndexAtPtr( m_string+i, codePoint );
            }
        }
        return -1;
    }
    else
    {
        // Sanity check on the startindex range
        if ( startIndex - (Int32)substr.m_length < 0 )
            return -1;

        UInt32 bytesFromStart = 0;
        char* cpPos = CodepointPtrAtIndex( startIndex, bytesFromStart );
        for ( Int32 i=bytesFromStart-(substr.m_byteSize-1); i>=0; --i )
        {
            if ( 0 == memcmp( m_string+i, substr.m_string, substr.m_byteSize-1 ) )
            {
                Int32 codePoint = 0;
                return CodepointIndexAtPtr( m_string+i, codePoint );
            }
        }
        return -1;
    }
}

/*-------------------------------------------------------------------------*/

Int32
CUtf8String::HasSubstr( const CUtf8String& substr ,
                        bool startfront           ) const
{GUCEF_TRACE;

    if ( startfront )
    {
        return HasSubstr( substr, 0, startfront );
    }
    if ( m_length > 0 )
    {
        return HasSubstr( substr, m_length-1, startfront );
    }
    return -1;
}

/*-------------------------------------------------------------------------*/

CAsciiString
CUtf8String::ForceToAscii( char asciiReplacement ) const
{GUCEF_TRACE;

    CAsciiString ascii;
    if ( m_length > 0 )
    {
        char* asciiBuffer = ascii.Reserve( m_length+1 );

        const char* cpPos = m_string;
        Int32 cp = 0;
        UInt32 i=0;
        do
        {
            cpPos = (const char*) utf8codepoint( cpPos, &cp );

            if ( cp >= 0 && cp <= 127 )
                *asciiBuffer = (char) cp;
            else
                *asciiBuffer = asciiReplacement;

            ++i; ++asciiBuffer;
        }
        while ( i<=m_length );
        ascii.SetLength( m_length );
    }
    return ascii;
}

/*-------------------------------------------------------------------------*/

bool
CUtf8String::WildcardEquals( const CUtf8String& strWithWildcards    ,
                             const Int32 wildCardToken /* = '*' */  ,
                             const bool caseSensitive /* = true */  ,
                             const bool biDirectional /* = false */ ) const
{GUCEF_TRACE;

    if ( biDirectional )
    {
        if ( strWithWildcards.WildcardEquals( *this, wildCardToken, caseSensitive, false ) )
            return true;
    }

    if ( strWithWildcards == wildCardToken || *this == wildCardToken )
        return true;

    std::vector< CUtf8String > segs = strWithWildcards.ParseElements( wildCardToken, false );
    Int32 lastSeg = 0;
    std::vector< CUtf8String >::iterator i = segs.begin();
    while ( i != segs.end() )
    {
        lastSeg = HasSubstr( (*i), lastSeg, true );
        if ( 0 > lastSeg )
            return false;
        ++i;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CUtf8String::Equals( const CUtf8String& otherStr           ,
                     const bool caseSensitive /* = true */ ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL == m_string && GUCEF_NULL == otherStr.m_string ) 
        return true;
    else
    if ( GUCEF_NULL == m_string || GUCEF_NULL == otherStr.m_string ) 
        return false;
    
    if ( caseSensitive )
    {
        return 0 == utf8cmp( m_string, otherStr.m_string );
    }
    else
    {
        return 0 == utf8casecmp( m_string, otherStr.m_string );
    }
}

/*-------------------------------------------------------------------------*/

bool
CUtf8String::NotEquals( const CUtf8String& otherStr           ,
                        const bool caseSensitive /* = true */ ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL == m_string && GUCEF_NULL == otherStr.m_string ) 
        return false;
    else
    if ( GUCEF_NULL == m_string || GUCEF_NULL == otherStr.m_string ) 
        return true;

    if ( caseSensitive )
    {
        return 0 != utf8cmp( m_string, otherStr.m_string );
    }
    else
    {
        return 0 != utf8casecmp( m_string, otherStr.m_string );
    }
}

/*-------------------------------------------------------------------------*/

void
CUtf8String::Scan( const char* newStr     ,
                   const UInt32 maxLength )
{GUCEF_TRACE;

    if ( newStr != NULL )
    {
        for ( UInt32 i=0; i<maxLength; ++i )
        {
            if ( newStr[ i ] == '\0' )
            {
                // null-terminator found
                Set( newStr ,
                     i      );
                return;
            }
        }

        // If we get here then no null-terminator was found in the buffer
        // before the maximum was reached
        Set( newStr    ,
             maxLength );
    }
}

/*-------------------------------------------------------------------------*/

CUtf8String
CUtf8String::Combine( const StringVector& elements, Int32 seperator ) const
{GUCEF_TRACE;

    if ( elements.empty() && 0 == m_length )
        return CUtf8String();

    // Determine storage size needed

    size_t cpSize = utf8codepointsize( seperator );
    UInt32 bufferSize = 1;
    if ( m_byteSize > 0 )
        bufferSize += m_byteSize + (UInt32) cpSize;

    StringVector::const_iterator i = elements.begin();
    while ( i != elements.end() )
    {
        bool elementAdded = false;
        if ( !(*i).IsNULLOrEmpty() )
        {
            bufferSize += (*i).m_byteSize-1;
            elementAdded = true;
        }
        ++i;
        if ( elementAdded && i != elements.end() && !(*i).IsNULLOrEmpty() )
        {
            bufferSize += (UInt32) cpSize;
        }
    }

    // Allocate total storage

    CUtf8String comboStr;
    if ( GUCEF_NULL == comboStr.Reserve( bufferSize ) )
        return CUtf8String::Empty;

    // Write contents
    char* cpPos = comboStr.m_string;
    size_t bufferSpaceLeft = bufferSize;
    if ( GUCEF_NULL != m_string && 0 < m_length )
    {
        memcpy( comboStr.m_string, m_string, m_byteSize );
        bufferSpaceLeft -= m_byteSize;

        cpPos = (char*) utf8catcodepoint( cpPos, seperator, bufferSpaceLeft );
        bufferSpaceLeft -= cpSize;
    }
    else
    {
        comboStr.m_string[ 0 ] = '\0';
    }
    i = elements.begin();
    while ( i != elements.end() )
    {
        bool elementAdded = false;
        if ( !(*i).IsNULLOrEmpty() )
        {
            size_t bytesWritten = (*i).m_byteSize-1;
            memcpy( cpPos, (*i).m_string, bytesWritten );
            cpPos += bytesWritten;
            *cpPos = '\0';
            bufferSpaceLeft -= bytesWritten;
            elementAdded = true;
        }
        ++i;
        if ( elementAdded && i != elements.end() && !(*i).IsNULLOrEmpty() )
        {
            cpPos = (char*) utf8catcodepoint( cpPos, seperator, bufferSpaceLeft );
            bufferSpaceLeft -= cpSize;
        }
    }

    // Determine new string length codepoint wise
    comboStr.m_length = (UInt32) utf8len( comboStr.m_string );
    comboStr.m_string[ comboStr.m_byteSize-1 ] = '\0';
    return comboStr;
}

/*-------------------------------------------------------------------------*/

bool
CUtf8String::IsFormattingValid( void ) const
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_string )
    {
        // Check if an invalid UTF8 code point is found
        return GUCEF_NULL == utf8valid( m_string );
    }
    return true;
}

/*-------------------------------------------------------------------------*/

CUtf8String
CUtf8String::operator+( const CUtf8String& addition ) const
{GUCEF_TRACE;

    CUtf8String tmp( *this );
    tmp += addition;
    return tmp;
}

/*-------------------------------------------------------------------------*/

CUtf8String
CUtf8String::operator+( const char* addition ) const
{GUCEF_TRACE;

    CUtf8String tmp( *this );
    tmp += addition;
    return tmp;
}

/*-------------------------------------------------------------------------*/

CUtf8String
CUtf8String::operator+( const char addition ) const
{GUCEF_TRACE;

    CUtf8String tmp( *this );
    tmp += addition;
    return tmp;
}

/*-------------------------------------------------------------------------*/

CUtf8String
operator+( const char* lhs    ,
           const CUtf8String& rhs )
{GUCEF_TRACE;

    CUtf8String tmp( lhs );
    tmp += rhs;
    return tmp;
}

/*-------------------------------------------------------------------------*/

bool
operator==( const char* lhs    ,
            const CUtf8String& rhs )
{GUCEF_TRACE;

    return rhs == lhs;
}

/*-------------------------------------------------------------------------*/

bool
operator!=( const char* lhs    ,
            const CUtf8String& rhs )
{GUCEF_TRACE;

    return rhs != lhs;
}

/*-------------------------------------------------------------------------*/

CUtf8String
operator+( const char lhs     ,
           const CUtf8String& rhs )
{GUCEF_TRACE;

    return CUtf8String( &lhs, 1 ) + rhs;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
