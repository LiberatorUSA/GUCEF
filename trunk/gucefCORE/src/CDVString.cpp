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

#ifndef GUCEF_CORE_CSTRINGLIST_H
#include "CStringList.h"
#define GUCEF_CORE_CSTRINGLIST_H
#endif /* GUCEF_CORE_CSTRINGLIST_H ? */

#ifndef GUCEF_CORE_DVSTRUTILS_H
#include "dvstrutils.h"
#define GUCEF_CORE_DVSTRUTILS_H
#endif /* GUCEF_CORE_DVSTRUTILS_H ? */

#include "CDVString.h"

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
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CString::CString( void )
        : m_string( NULL ) ,
          m_length( 0 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CString::CString( const CString &src )
        : m_string( NULL ) ,
          m_length( 0 )
{GUCEF_TRACE;

    if ( src.m_length > 0 )
    {
        m_string = new char[ src.m_length+1 ];
        assert( m_string != NULL );
        m_length = src.m_length;
        memcpy( m_string, src.m_string, m_length+1 );
    }
}

/*-------------------------------------------------------------------------*/
 
CString::CString( const std::string& src )
        : m_string( NULL ) ,
          m_length( 0 )
{GUCEF_TRACE;
    
    if ( src.size() > 0 )
    {
        m_length = (UInt32)src.size();
        m_string = new char[ m_length+1 ];
        assert( m_string != NULL );                                                                                
        memcpy( m_string, src.c_str(), m_length+1 );
    }                
}

/*-------------------------------------------------------------------------*/

CString::CString( const char *src )
        : m_string( NULL ) ,
          m_length( 0 )
{GUCEF_TRACE;
    
    if ( src != NULL )
    {
        m_length = (UInt32)strlen( src );
        m_string = new char[ m_length+1 ];
        assert( m_string );
        memcpy( m_string, src, m_length+1 );
    }                                           
}

/*-------------------------------------------------------------------------*/

CString::CString( const char *src ,
                  UInt32 length   )
        : m_string( NULL ) ,
          m_length( 0 )           
{GUCEF_TRACE;
    
    if ( src && length )
    {
        m_length = length;
        m_string = new char[ m_length+1 ];
        assert( m_string );                                                                                
        memcpy( m_string, src, m_length );
        m_string[ m_length ] = '\0';                                 
    }                                              
}                  

/*-------------------------------------------------------------------------*/

CString::CString( const char src )
    : m_string( NULL ) ,
      m_length( 0 ) 
{GUCEF_TRACE;
    
    Set( &src, 1 );    
}

/*-------------------------------------------------------------------------*/

CString::CString( const int NULLvalue )
    : m_string( NULL ) ,
      m_length( 0 )
{GUCEF_TRACE;

    assert( NULLvalue == NULL );
}
/*-------------------------------------------------------------------------*/

CString::~CString()
{GUCEF_TRACE;
    
    delete []m_string;
    m_string = NULL;
    m_length = 0;
}

/*-------------------------------------------------------------------------*/

CString&
CString::operator=( const CString &src )
{GUCEF_TRACE;
    
    if ( &src != this )
    {
        delete []m_string;
        m_string = NULL;
        m_length = src.m_length;
        
        if ( m_length > 0 )
        {
            m_string = new char[ m_length+1 ];
            assert( m_string );                                                                                
            memcpy( m_string, src.m_string, m_length+1 );        
        }              
    }              
    return *this;
}

/*-------------------------------------------------------------------------*/

CString&
CString::operator=( const char *src )
{GUCEF_TRACE;
    
    // protect against self-assignment
    if ( src != m_string )
    {
        delete []m_string;
        m_string = NULL;
        m_length = 0;
    
        if ( src != NULL )
        {
            m_length = (UInt32)strlen( src );
            if ( m_length > 0 )
            {
                m_string = new char[ m_length+1 ];
                assert( m_string );
                memcpy( m_string, src, m_length+1 );
            }
        } 
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
CString::operator==( const char character ) const
{GUCEF_TRACE;          //@TODO  //@FIXME
       return *this == CString(  character);
    if ( m_length == 1 )
    {
        return *m_string == character;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CString::operator!=( const char character ) const
{GUCEF_TRACE;
                     //@TODO  //@FIXME
    return *this != CString(  character);
    
    if ( m_length == 1 )
    {
        return *m_string != character;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CString::operator!=( const int NULLvalue ) const
{GUCEF_TRACE;

    if ( NULLvalue == NULL )
    {
        return m_length > 0;
    }
    GUCEF_ASSERT_ALWAYS;
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CString::operator==( const int NULLvalue ) const
{GUCEF_TRACE;

    if ( NULLvalue == NULL )
    {
        return m_length == 0;
    }
    GUCEF_ASSERT_ALWAYS;
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CString::operator==( const char *other ) const
{GUCEF_TRACE;

    if ( !m_string || !other )
    {
        return ( NULL == m_string ) && ( NULL == other );        
    } 
    return strcmp( m_string ,
                   other    ) == 0;
}

/*-------------------------------------------------------------------------*/

bool
CString::operator==( const CString &other ) const
{GUCEF_TRACE;

    if ( !m_string || !other.m_string )
    {
        return false;        
    }         
    return strcmp( m_string       ,
                   other.m_string ) == 0;
}

/*-------------------------------------------------------------------------*/

bool
CString::operator!=( const CString &other ) const
{GUCEF_TRACE;

    if ( !m_string || !other.m_string )
    {
            return false;        
    }         
    return strcmp( m_string       ,
                   other.m_string ) != 0;
}

/*-------------------------------------------------------------------------*/

bool
CString::operator!=( const char *other ) const
{GUCEF_TRACE;

    if ( !m_string || !other )
    {
        return !( ( NULL == m_string ) && ( NULL == other ) );
    } 
    return strcmp( m_string ,
                   other   ) != 0;
}

/*-------------------------------------------------------------------------*/

char
CString::operator[]( const UInt32 index ) const
{GUCEF_TRACE;

    assert( index < m_length );
    return m_string[ index ];
}

/*-------------------------------------------------------------------------*/

bool 
CString::operator<( const CString& other ) const
{GUCEF_TRACE;
    
    if ( m_string && other.m_string )
    {
        return strcmp( m_string, other.m_string ) < 0;
    }
    else
    if ( NULL == m_string )
    {
        return true;
    }
    return false;        
}

/*-------------------------------------------------------------------------*/

bool
CString::IsNULLOrEmpty( void ) const
{GUCEF_TRACE;

    return m_length == 0;
}

/*-------------------------------------------------------------------------*/

CString::operator std::string() const
{GUCEF_TRACE;
    
    if ( m_length > 0 )
    {
        return std::string( m_string, m_length );
    }
    return std::string();
}

/*-------------------------------------------------------------------------*/
/* 
     Disabled in favor of std::string() conversion

CString::operator const char*() const
{GUCEF_TRACE;
        return m_string;
}
*/
/*-------------------------------------------------------------------------*/

void
CString::Set( const char *new_str ,
              UInt32 len          )
{GUCEF_TRACE;
    
    if ( new_str != m_string )
    {
        delete []m_string;
        m_string = NULL;
        m_length = 0;

        if ( ( new_str != NULL ) &&
             ( len > 0 )          )
        {
            m_length = len;
            m_string = new char[ len+1 ];
            assert( m_string != NULL );                                                                                
            memcpy( m_string, new_str, m_length+1 );
            m_string[ len ] = '\0';                                
        }
    }
}

/*-------------------------------------------------------------------------*/

UInt32
CString::GetCharacterCount( const char searchChar ) const
{GUCEF_TRACE;

    UInt32 charCount = 0;
    for ( UInt32 i=0; i<m_length; ++i )
    {
        if ( m_string[ i ] == searchChar )
        {
            ++charCount;
        }
    }
    return charCount;
}

/*-------------------------------------------------------------------------*/
              
void 
CString::Append( const char *appendstr ,
                 UInt32 len            )
{GUCEF_TRACE;
    
    if ( ( appendstr != m_string ) &&
         ( appendstr != NULL )     && 
         ( len > 0 )                )
    {
        if ( m_length > 0 )
        {
            char* newString = new char[ m_length+len+1 ];       
            assert( newString != NULL );                                                                                
            memcpy( newString, m_string, m_length );
            memcpy( newString+m_length, appendstr, len );            
            m_length = m_length+len;
            newString[ m_length ] = 0;            
            delete []m_string;
            m_string = newString;
        }
        else
        {
            delete []m_string;
            m_length = len;
            m_string = new char[ m_length+1 ];
            assert( m_string );                                                                                
            memcpy( m_string, appendstr, m_length );
            m_string[ m_length ] = 0;
        } 
    }                                                                     
}                 

/*-------------------------------------------------------------------------*/

CString&
CString::operator+=( const CString &other )
{GUCEF_TRACE;
    
    Append( other.m_string, other.m_length );                                                          
    return *this;
}

/*-------------------------------------------------------------------------*/

CString&
CString::operator+=( const char *other )
{GUCEF_TRACE;

    Append( other, UInt32( strlen( other ) ) );
    return *this;
}

/*-------------------------------------------------------------------------*/

CString&
CString::operator+=( char lastchar )
{GUCEF_TRACE;
    
    Append( &lastchar, 1 );                                                     
    return *this;        
}

/*-------------------------------------------------------------------------*/

const char*
CString::C_String( void ) const
{GUCEF_TRACE;
        return m_string;
}

/*-------------------------------------------------------------------------*/

std::string 
CString::STL_String( void ) const
{GUCEF_TRACE;

    if ( m_string > 0 )
    {
        return std::string( m_string, m_length );
    }
    return std::string();
}

/*-------------------------------------------------------------------------*/

UInt32
CString::Length( void ) const
{GUCEF_TRACE;
        return m_length;
}

/*-------------------------------------------------------------------------*/

CString 
CString::ReplaceChar( char oldchar ,
                      char newchar ) const
{GUCEF_TRACE;

    if ( m_length > 0 )
    {
        char* str = new char[ m_length+1 ];
        memcpy( str, m_string, m_length+1 );
        for ( UInt32 i=0; i<m_length; ++i )
        {
                if ( str[ i ] == oldchar )
                {
                        str[ i ] = newchar;
                }
        }
        CString newstr( str, m_length );
        delete []str;
        return newstr;
    }
    CString emptystr;       
    return emptystr;  
}                             

/*-------------------------------------------------------------------------*/

CString 
CString::ReplaceSubstr( const CString& substr      ,
                        const CString& replacement ) const
{GUCEF_TRACE;
    
    CString testStr = *this;
    CString newStr;    
    Int32 subStrIndex = 0;
    do
    {
        subStrIndex = testStr.HasSubstr( substr, true );
        if ( subStrIndex >= 0 )
        {
            newStr.Append( testStr.C_String(), subStrIndex ); 
            newStr += replacement;
            testStr = testStr.CutChars( (UInt32)subStrIndex + substr.Length(), true );
        }
    }
    while ( subStrIndex >= 0 );    
    newStr += testStr;
    
    return newStr;    
}

/*-------------------------------------------------------------------------*/

void 
CString::Clear( void )
{GUCEF_TRACE;            

    m_length = 0;
    delete []m_string;
    m_string = NULL;       
}

/*-------------------------------------------------------------------------*/

CString 
CString::SubstrToChar( char searchchar   ,
                       UInt32 startIndex ,
                       bool frontToBack  ) const
{GUCEF_TRACE;

    if ( startIndex < m_length )
    {
        if ( frontToBack )
        {
            for ( UInt32 i=startIndex; i<m_length; ++i )
            {
                if ( m_string[ i ] == searchchar )
                {
                    CString substr;
                    substr.Set( m_string+startIndex ,
                                i-startIndex        );
                    return substr;
                }
            }
            return CString( *this );
        }
        
        for ( Int32 i=startIndex; i>=0; --i )
        {
            if ( m_string[ i ] == searchchar )
            {
                CString substr;
                substr.Set( m_string+i+1 ,
                            startIndex-i );                                   
                return substr;
            }
        }
        return CString( *this );
    }
    return CString();
}

/*-------------------------------------------------------------------------*/

CString 
CString::SubstrToIndex( UInt32 index     ,
                        bool frontToBack ) const
{GUCEF_TRACE;

    UInt32 max = index+1 > m_length ? m_length-1 : index;
    if ( frontToBack )
    {          
        return CString( m_string, max );
    }
    
    return CString( m_string+m_length-max, max );
}

/*-------------------------------------------------------------------------*/

CString 
CString::SubstrToChar( char searchchar  ,
                       bool frontToBack ) const
{GUCEF_TRACE;

    if ( frontToBack )
    {
        return SubstrToChar( searchchar, 0, frontToBack );
    }
    return SubstrToChar( searchchar, m_length-1, frontToBack );
}

/*-------------------------------------------------------------------------*/

CString
CString::Trim( bool frontToBack ) const
{GUCEF_TRACE;
    
    if ( m_length > 0 )
    {    
        if ( frontToBack )
        {
            UInt32 charsToCut = 0;            
            while ( m_string[ charsToCut ] == ' ' ||  m_string[ charsToCut ] == '\t' )
            {
                ++charsToCut;
            }
            return CutChars( charsToCut, frontToBack );
        }
        
        UInt32 i=m_length-1;
        UInt32 charsToCut = 0;            
        while ( i>=0 && ( m_string[ i ] == ' ' ||  m_string[ i ] == '\t' ) )
        {
            --i;
            ++charsToCut;
        }
        return CutChars( charsToCut, frontToBack );
    }
    return CString( *this );    
}

/*-------------------------------------------------------------------------*/

CString
CString::SubstrToSubstr( const CString& searchstr ,
                         bool startfront          ) const
{GUCEF_TRACE;

    UInt32 slen = searchstr.Length();    
    if ( slen > m_length )
    {
        return *this;
    }
    
    if ( startfront )
    {
        UInt32 max = m_length - slen;
        for ( UInt32 i=0; i<max; ++i )
        {
            if ( memcmp( m_string+i, searchstr.m_string, slen ) == 0 )
            {
                CString substr;
                substr.Set( m_string ,
                            i       );
                return substr;
            }
        }
        return *this;
    }
    
    UInt32 max = m_length - slen;        
    for ( Int32 i=max-1; i>=0; --i )
    {
        if ( memcmp( m_string+i, searchstr.m_string, slen ) == 0 )
        {
            CString substr;
            substr.Set( m_string+i+slen ,
                        max-i           );
            return substr;
        }
    }
    return *this; 
}

/*-------------------------------------------------------------------------*/

CString
CString::RemoveChar( const char charToRemove ) const
{GUCEF_TRACE;

    CString newString;
    for ( UInt32 i=0; i<m_length; ++i )
    {
        if ( m_string[ i ] != charToRemove )
        {
            newString += m_string[ i ];
        }
    }
    return newString;
}

/*-------------------------------------------------------------------------*/

CString
CString::CompactRepeatingChar( const char charToCompact ) const
{GUCEF_TRACE;
    
    char* newString = new char[ m_length+1 ];
    UInt32 newStringLength = 0;
    
    for ( UInt32 i=0; i<m_length; ++i )
    {
        newString[ newStringLength ] = m_string[ i ];
        ++newStringLength;
        
        if ( m_string[ i ] == charToCompact )
        {            
            while ( ( i+1 < m_length ) && 
                    ( m_string[ i+1 ] == charToCompact ) )
            {
                ++i;
            }
        }
    }
    
    CString returnStr( newString, newStringLength );    
    delete []newString;
    return returnStr;
}

/*-------------------------------------------------------------------------*/

CString 
CString::CutChars( UInt32 charcount ,
                   bool startfront  ) const
{GUCEF_TRACE;

    if ( startfront )
    {
        if ( charcount < m_length )                
        {
            return CString( m_string+charcount ,
                            m_length-charcount );                                        
        }
        
        return CString();
    }
    
    if ( charcount < m_length )        
    {
        return CString( m_string           ,
                        m_length-charcount );                                                            
    }
    
    return CString();        
}

/*-------------------------------------------------------------------------*/

void 
CString::SetInt( Int32 value )                   
{GUCEF_TRACE;
    
    char intval[ 10 ];
    sprintf( intval, "%d", value );        
    *this = intval;
}

/*-------------------------------------------------------------------------*/

Int32 
CString::GetInt( void ) const
{GUCEF_TRACE;

    if ( m_string )
    {
        return Str_To_Int( m_string );            
    }                        
    return 0;
}

/*-------------------------------------------------------------------------*/

CString 
CString::Lowercase( void ) const
{GUCEF_TRACE;
        
    if ( m_length > 0 )
    {
        char* lcstr = new char[ m_length+1 ];
        memcpy( lcstr, m_string, m_length+1 );
        for ( UInt32 i=0; i<m_length; ++i )
        {
            if ( ( lcstr[ i ] > 64 ) &&
                 ( lcstr[ i ] < 91 ) )
            {
                lcstr[ i ] += 32;
            }        
        }
        CString lower( lcstr, m_length );
        delete []lcstr;
        return lower;
    }
    return CString();        
}

/*-------------------------------------------------------------------------*/
        
CString 
CString::Uppercase( void ) const
{GUCEF_TRACE;

    if ( m_length > 0 )
    {
        char* ucstr = new char[ m_length+1 ];
        memcpy( ucstr, m_string, m_length+1 );
        for ( UInt32 i=0; i<m_length; ++i )
        {
            if ( ( ucstr[ i ] > 96 ) &&
                 ( ucstr[ i ] < 123 ) )
            {
                ucstr[ i ] -= 32;
            }        
        }
        CString upper( ucstr, m_length );
        delete []ucstr;
        return upper;
    }
    return CString();
}

/*-------------------------------------------------------------------------*/

Int32
CString::HasChar( char searchchar         ,
                  const UInt32 startIndex ,
                  bool frontToBack        ) const
{GUCEF_TRACE;

    if ( frontToBack )
    {
        for ( UInt32 i=startIndex; i<m_length; ++i )
        {
            if ( m_string[ i ] == searchchar )
            {
                return i;
            }                
        }
        return -1;
    }

    UInt32 max = startIndex > m_length-1 ? m_length-1 : startIndex;
    for ( Int32 i=max; i>=0; --i )
    {
        if ( m_string[ i ] == searchchar )
        {
            return i;
        }                
    }
    return -1;
}

/*-------------------------------------------------------------------------*/

Int32 
CString::HasChar( char searchchar  ,
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

std::vector< CString > 
CString::ParseElements( char seperator        , 
                        bool addEmptyElements ) const
{GUCEF_TRACE;

    if ( m_length > 0 )
    {
        std::vector< CString > list;
        CString entry;
        UInt32 last = 0;
        for ( UInt32 i=0; i<m_length; ++i )
        {
            if ( m_string[ i ] == seperator )
            {
                UInt32 stringLength = i-last;
                if ( ( 0 == stringLength && addEmptyElements ) ||
                     ( stringLength > 0 ) ) 
                {
                    entry.Set( m_string+last ,
                               stringLength  );
                    list.push_back( entry );
                }    
                last = i+1;                    
            }  
        }
     
        // add last item
        UInt32 stringLength = m_length-last;
        if ( ( 0 == stringLength && addEmptyElements ) ||
             ( stringLength > 0 ) ) 
        {        
            entry.Set( m_string+last ,
                       stringLength );
            list.push_back( entry );
        }                 
        return list;        
    }
    return std::vector< CString >();
}

/*-------------------------------------------------------------------------*/

UInt32
CString::FindMaxSubstrEquality( const CString& searchStr ,
                                const UInt32 startOffset ,
                                bool startFront          ) const
{GUCEF_TRACE;

    // Sanity check on the startOffset
    if ( (Int32)m_length - startOffset > 0 )
    {        
        // Get the smallest of the 2 buffer limits
        UInt32 max = searchStr.Length();
        if ( m_length - startOffset < max )
        {
            max = m_length - startOffset;
        }
        
        if ( startFront )
        {            
            // Loop trough the buffer growing our comparison string
            UInt32 subLength=1;
            while ( subLength<=max )
            {
                if ( memcmp( m_string+startOffset, searchStr.m_string, subLength ) != 0 )
                {
                    // Reached the maximum equality length
                    return subLength-1;
                }
                ++subLength;
            }
            return subLength-1;
        }
        else
        {
            // Loop trough the buffer growing our comparison string
            const char* string = m_string + m_length - startOffset;
            const char* otherString = searchStr.m_string + searchStr.m_length; 
             
            UInt32 subLength=1;
            while ( subLength<=max )
            {
                if ( memcmp( string-subLength, otherString-subLength, subLength ) != 0 )
                {
                    // Reached the maximum equality length
                    return subLength-1;
                }
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
CString::HasSubstr( const CString& substr ,
                    Int32 startIndex      ,
                    bool startfront       ) const
{GUCEF_TRACE;

    // Sanity check on the startindex range
    if ( startIndex < 0 || (UInt32)startIndex > m_length ) return -1;

    UInt32 subStrLength = substr.Length();
    if ( subStrLength > m_length )
    {
        return -1;
    }
    
    if ( startfront )
    {
        UInt32 max = m_length - subStrLength;
        for ( UInt32 i=startIndex; i<=max; ++i )
        {
            if ( memcmp( m_string+i, substr.m_string, subStrLength ) == 0 )
            {           
                return i;
            }
        }
        return -1;
    }
    
    Int32 max = (Int32) ( m_length - subStrLength );
    if ( startIndex-(Int32)subStrLength < max )
    {
        max = startIndex-subStrLength;
    }
    for ( Int32 i=max; i>=0; --i )
    {
        if ( memcmp( m_string+i, substr.m_string, subStrLength ) == 0 )
        {                                    
            return i;
        }
    }
    return -1;
}

/*-------------------------------------------------------------------------*/
                                
Int32 
CString::HasSubstr( const CString& substr ,
                    bool startfront       ) const
{GUCEF_TRACE;
    
    if ( startfront )
    {
        return HasSubstr( substr, 0, startfront );
    }
    if ( m_length > 0 )
    {
        return HasSubstr( substr, m_length, startfront );
    }
    return -1;
}

/*-------------------------------------------------------------------------*/

bool
CString::Equals( const CString& otherStr               ,
                 const bool caseSensitive /* = true */ ) const
{GUCEF_TRACE;

    if ( caseSensitive )
    {
        return *this == otherStr;
    }
    else
    {
        return Lowercase() == otherStr.Lowercase();
    }
}

/*-------------------------------------------------------------------------*/

bool
CString::NotEquals( const CString& otherStr               ,
                    const bool caseSensitive /* = true */ ) const
{GUCEF_TRACE;

    if ( caseSensitive )
    {
        return *this != otherStr;
    }
    else
    {
        return Lowercase() != otherStr.Lowercase();
    }
}

/*-------------------------------------------------------------------------*/

void
CString::Scan( const char* newStr     ,
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

CString 
CString::operator+( const CString& addition )
{GUCEF_TRACE;
        CString tmp( *this );
        tmp += addition;
        return tmp;        
}

/*-------------------------------------------------------------------------*/
        
CString 
CString::operator+( const char* addition )
{GUCEF_TRACE;
        CString tmp( *this );
        tmp += addition;
        return tmp;        
}

/*-------------------------------------------------------------------------*/

CString
operator+( const CString& lhs ,
           const CString& rhs )
{GUCEF_TRACE;
        CString tmp( lhs );
        tmp += rhs;
        return tmp;
}

/*-------------------------------------------------------------------------*/

CString
operator+( const CString& lhs ,
           const char* rhs    )
{GUCEF_TRACE;
        CString tmp( lhs );
        tmp += rhs;
        return tmp;
}

/*-------------------------------------------------------------------------*/

CString 
operator+( const char* lhs    ,  
           const CString& rhs )
{GUCEF_TRACE;
        CString tmp( lhs );
        tmp += rhs;
        return tmp;        
}

/*-------------------------------------------------------------------------*/

bool 
operator==( const char* lhs    , 
            const CString& rhs )
{GUCEF_TRACE;
        return rhs == lhs;
}

/*-------------------------------------------------------------------------*/

bool 
operator!=( const char* lhs    , 
            const CString& rhs )
{GUCEF_TRACE;
        return rhs != lhs;
}

/*-------------------------------------------------------------------------*/

CString
operator+( const char lhs     , 
           const CString& rhs )
{GUCEF_TRACE;

    return CString( &lhs, 1 ) + rhs;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
