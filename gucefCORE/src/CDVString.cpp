/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
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
{TRACE;

}

/*-------------------------------------------------------------------------*/

CString::CString( const CString &src )
        : m_string( NULL ) ,
          m_length( 0 )
{TRACE;        
        *this = src;
}

/*-------------------------------------------------------------------------*/
 
CString::CString( const std::string& src )
        : m_string( NULL ) ,
          m_length( 0 )
{TRACE;
    
    if ( src.size() )
    {
        m_length = (UInt32)src.size();
        m_string = new char[ m_length+1 ];
        assert( m_string );                                                                                
        memcpy( m_string, src.c_str(), m_length+1 );
    }                
}

/*-------------------------------------------------------------------------*/

CString::CString( const char *src )
        : m_string( NULL ) ,
          m_length( 0 )
{TRACE;
    
    if ( src )
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
{TRACE;
    
    if ( src && length )
    {
        m_length = length;
        m_string = new char[ m_length+1 ];
        assert( m_string );                                                                                
        memcpy( m_string, src, m_length+1 );                                 
    }                                              
}                  

/*-------------------------------------------------------------------------*/

CString::~CString()
{TRACE;
    
    delete []m_string;
    m_string = NULL;
    m_length = 0;
}

/*-------------------------------------------------------------------------*/

CString&
CString::operator=( const CString &src )
{TRACE;
    
    if ( &src != this )
    {
        delete []m_string;
        m_string = NULL;
        m_length = src.m_length;
        
        if ( m_length )
        {
            m_string = new char[ m_length+1 ];
            assert( m_string );                                                                                
            memcpy( m_string, src.m_string, m_length+1 );        
        }
        
        return *this;                 
    }              
    return *this;
}

/*-------------------------------------------------------------------------*/

CString&
CString::operator=( const char *src )
{TRACE;
    
    delete []m_string;
    m_string = NULL;
    m_length = 0;
    
    if ( src )
    {
        m_length = (UInt32)strlen( src );
        m_string = new char[ m_length+1 ];
        assert( m_string );
        memcpy( m_string, src, m_length+1 );       
        
        return *this;                 
    } 
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
CString::operator==( const char *other ) const
{TRACE;
        if ( !m_string || !other )
        {
                return false;        
        } 
        return strcmp( m_string ,
                       other   ) == 0;
}

/*-------------------------------------------------------------------------*/

bool
CString::operator==( const CString &other ) const
{TRACE;
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
{TRACE;

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
{TRACE;

    if ( !m_string || !other )
    {
            return false;        
    } 
    return strcmp( m_string ,
                   other   ) != 0;
}

/*-------------------------------------------------------------------------*/

char
CString::operator[]( const UInt32 index ) const
{TRACE;

    assert( index < m_length );
    return m_string[ index ];
}

/*-------------------------------------------------------------------------*/

bool 
CString::operator<( const CString& other ) const
{TRACE;
    
    if ( m_string && other.m_string )
    {
        return strcmp( m_string, other.m_string ) < 0;
    }
    return false;        
}

/*-------------------------------------------------------------------------*/

CString::operator std::string() const
{TRACE;
    
    return std::string( m_string );
}

/*-------------------------------------------------------------------------*/
/*
CString::operator const char*() const
{TRACE;
        return m_string;
}
*/
/*-------------------------------------------------------------------------*/

void
CString::Set( const char *new_str ,
              UInt32 len          )
{TRACE;
    
    if ( new_str && len )
    {
        m_length = len;
        m_string = new char[ m_length+1 ];
        assert( m_string );                                                                                
        memcpy( m_string, new_str, m_length+1 );                                 
    }    
}

/*-------------------------------------------------------------------------*/
              
void 
CString::Append( const char *appendstr ,
                 UInt32 len            )
{TRACE;
    
    if ( appendstr && len )
    {
        if ( m_length )
        {
            char* newString = new char[ m_length+len+1 ];       
            assert( newString );                                                                                
            memcpy( newString, m_string, m_length );
            memcpy( newString+m_length, appendstr, len );            
            m_length = m_length+len+1;
            newString[ m_length ] = 0;            
            delete []m_string;
            m_string = newString;
        }
        else
        {
            m_length = len;
            m_string = new char[ m_length+1 ];
            assert( m_string );                                                                                
            memcpy( m_string, appendstr, m_length+1 );            
        } 
    }                                                                     
}                 

/*-------------------------------------------------------------------------*/

CString&
CString::operator+=( const CString &other )
{TRACE;
    
    Append( other.m_string, other.m_length );                                                          
    return *this;
}

/*-------------------------------------------------------------------------*/

CString&
CString::operator+=( const char *other )
{TRACE;

    Append( other, UInt32( strlen( other ) ) );
    return *this;
}

/*-------------------------------------------------------------------------*/

CString&
CString::operator+=( char lastchar )
{TRACE;
    
    Append( &lastchar, 1 );                                                     
    return *this;        
}

/*-------------------------------------------------------------------------*/

const char*
CString::C_String( void ) const
{TRACE;
        return m_string;
}

/*-------------------------------------------------------------------------*/

std::string 
CString::STL_String( void ) const
{TRACE;
        std::string stlstr( m_string );
        return stlstr;
}

/*-------------------------------------------------------------------------*/

UInt32
CString::Length( void ) const
{TRACE;
        return m_length;
}

/*-------------------------------------------------------------------------*/

CString 
CString::ReplaceChar( char oldchar ,
                      char newchar ) const
{TRACE;
        if ( m_length )
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
                CString newstr( str );
                delete []str;
                return newstr;
        }
        CString emptystr;       
        return emptystr;  
}                             

/*-------------------------------------------------------------------------*/

void 
CString::ReplaceSubstr( const CString& substr      ,
                        const CString& replacement )
{TRACE;
    
    //@TODO @MAKEME
    assert( 0 );      
}

/*-------------------------------------------------------------------------*/

void 
CString::Clear( void )
{TRACE;            

    m_length = 0;
    delete []m_string;
    m_string = NULL;       
}

/*-------------------------------------------------------------------------*/

CString 
CString::SubstrToChar( char searchchar ,
                       bool startfront ) const
{TRACE;

        if ( startfront )
        {
                for ( UInt32 i=0; i<m_length; ++i )
                {
                        if ( m_string[ i ] == searchchar )
                        {
                                CString substr;
                                substr.Set( m_string ,
                                            i       );            
                                return substr;
                        }
                }
                CString substr( *this );
                return substr;
        }
        
        for ( Int32 i=m_length-1; i>=0; --i )
        {
                if ( m_string[ i ] == searchchar )
                {
                        CString substr;
                        substr.Set( m_string+(m_length-i) ,
                                    i                   );                                   
                        return substr;
                }
        }
        CString substr( *this );
        return substr;        
}

/*-------------------------------------------------------------------------*/

CString
CString::SubstrToSubstr( const CString& searchstr ,
                         bool startfront          ) const
{TRACE;

        UInt32 slen = searchstr.Length();
        
        if ( slen > m_length )
        {
                CString substr( *this );
                return substr;
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
                CString substr( *this );
                return substr;
        }
        
        UInt32 max = m_length - slen;        
        for ( Int32 i=max-1; i>=0; --i )
        {
                if ( memcmp( m_string+i, searchstr.m_string, slen ) == 0 )
                {
                        CString substr;
                        substr.Set( m_string+(max-i) ,
                                    i               );
                        return substr;
                }
        }
        CString substr( *this );
        return substr; 
}                         

/*-------------------------------------------------------------------------*/

CString 
CString::CutChars( UInt32 charcount ,
                   bool startfront  ) const
{TRACE;
        if ( startfront )
        {
                if ( charcount < m_length )                
                {
                        CString substr( m_string+charcount ,
                                        m_length-charcount );
                        return substr;                                        
                }
                
                CString emptystr;
                return emptystr;
        }
        
        if ( charcount < m_length )        
        {
                CString substr;
                substr.Set( m_string+m_length-charcount ,
                            charcount                 );                            
                return substr;                                    
        }
        
        CString emptystr;
        return emptystr;        
}

/*-------------------------------------------------------------------------*/

void 
CString::SetInt( Int32 value )                   
{TRACE;
    
    char intval[ 10 ];
    sprintf( intval, "%d", value );        
    *this = intval;
}

/*-------------------------------------------------------------------------*/

Int32 
CString::GetInt( void ) const
{TRACE;

    if ( m_string )
    {
        return Str_To_Int( m_string );
            
    }                        
    return 0;
}

/*-------------------------------------------------------------------------*/

CString 
CString::Lowercase( void ) const
{TRACE;
        
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

/*-------------------------------------------------------------------------*/
        
CString 
CString::Uppercase( void ) const
{TRACE;

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

/*-------------------------------------------------------------------------*/

Int32 
CString::HasChar( char searchchar  ,
                  bool startfront  ) const
{TRACE;
        if ( startfront )
        {
                for ( UInt32 i=0; i<m_length; ++i )
                {
                        if ( m_string[ i ] == searchchar )
                        {
                                return i;
                        }                
                }
                return -1;
        }
        
        for ( Int32 i=m_length-1; i>=0; --i )
        {
                if ( m_string[ i ] == searchchar )
                {
                        return m_length-i;
                }                
        }
        return -1;        
}

/*-------------------------------------------------------------------------*/

CStringList 
CString::ParseElements( char seperator ) const
{TRACE;

        CStringList list;
        CString entry;
        UInt32 last = 0;
        for ( UInt32 i=0; i<m_length; ++i )
        {
                if ( m_string[ i ] == seperator )
                {
                        entry.Set( m_string+last ,
                                   i-last       );
                        list.Append( entry );
                        last = i;                    
                }  
        }
        /* add last item */
        entry.Set( m_string+last ,
                   m_length-last );        
        list.Append( entry ); 
        return list;        
}

/*-------------------------------------------------------------------------*/
                                
Int32 
CString::HasSubstr( const CString& substr ,
                    bool startfront       ) const
{TRACE;
        UInt32 slen = substr.Length();
        
        if ( slen > m_length )
        {
                return -1;
        }
        
        if ( substr.m_length == m_length )
        {
                if ( memcmp( m_string, substr.m_string, m_length ) == 0 )
                {
                        return 0;
                }
                return -1;
        }
        
        if ( startfront )
        {
                UInt32 max = m_length - slen;
                for ( UInt32 i=0; i<=max; ++i )
                {
                        if ( memcmp( m_string+i, substr.m_string, slen ) == 0 )
                        {           
                                return i;
                        }
                }
                return -1;
        }
        
        UInt32 max = m_length - slen;        
        for ( Int32 i=max; i>=0; --i )
        {
                if ( memcmp( m_string+i, substr.m_string, slen ) == 0 )
                {                                    
                        return max-i;
                }
        }
        return -1;
}

/*-------------------------------------------------------------------------*/

CString 
CString::operator+( const CString& addition )
{TRACE;
        CString tmp( *this );
        tmp += addition;
        return tmp;        
}

/*-------------------------------------------------------------------------*/
        
CString 
CString::operator+( const char* addition )
{TRACE;
        CString tmp( *this );
        tmp += addition;
        return tmp;        
}

/*-------------------------------------------------------------------------*/

CString
operator+( const CString& lhs ,
           const CString& rhs )
{TRACE;
        CString tmp( lhs );
        tmp += rhs;
        return tmp;
}

/*-------------------------------------------------------------------------*/

CString
operator+( const CString& lhs ,
           const char* rhs    )
{TRACE;
        CString tmp( lhs );
        tmp += rhs;
        return tmp;
}

/*-------------------------------------------------------------------------*/

CString 
operator+( const char* lhs    ,  
           const CString& rhs )
{TRACE;
        CString tmp( lhs );
        tmp += rhs;
        return tmp;        
}

/*-------------------------------------------------------------------------*/

bool 
operator==( const char* lhs    , 
            const CString& rhs )
{TRACE;
        return rhs == lhs;
}

/*-------------------------------------------------------------------------*/

bool 
operator!=( const char* lhs    , 
            const CString& rhs )
{TRACE;
        return rhs != lhs;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
