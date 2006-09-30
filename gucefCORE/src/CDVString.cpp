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

#ifndef GUCEF_CORE_CSTRINGSTORAGE_H
#include "CStringStorage.h"
#define GUCEF_CORE_CSTRINGSTORAGE_H
#endif /* GUCEF_CORE_CSTRINGSTORAGE_H ? */

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
        : _string( NULL )                      ,
          _length( 0 )                         ,
          _stringid( 0 )                       ,
          _store( CStringStorage::Instance() )   
{
        GUCEF_BEGIN;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CString::CString( const CString &src )
        : _string( NULL )     ,
          _length( 0 )        ,
          _stringid( 0 )      ,
          _store( src._store )
{
        GUCEF_BEGIN;        
        *this = src;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/
 
CString::CString( const std::string& src )
        : _string( NULL )                      ,
          _length( 0 )                         ,
          _stringid( 0 )                       ,
          _store( CStringStorage::Instance() )
{
        GUCEF_BEGIN;
        CHECKMEM( _store, sizeof(CStringStorage) );
        if ( src.size() )
        {
                _stringid = _store->RegisterString( src.c_str()         ,
                                                    (UInt32)src.size()  ,
                                                    &_string            );
                _length = (UInt32)src.size();                                                                
        }
        GUCEF_END;                
}

/*-------------------------------------------------------------------------*/

CString::CString( const char *src )
        : _string( NULL )                      ,
          _length( 0 )                         ,
          _stringid( 0 )                       ,
          _store( CStringStorage::Instance() )
{TRACE;
        //CHECKMEM( _store, sizeof(CStringStorage) );
        if ( src )
        {
                _length = (UInt32)strlen( src );
                _stringid = _store->RegisterString( src      ,
                                                    _length  ,
                                                    &_string );
        }                                           
}

/*-------------------------------------------------------------------------*/

CString::CString( const char *src ,
                  UInt32 length   )
        : _string( NULL )                      ,
          _length( 0 )                         ,
          _stringid( 0 )                       ,
          _store( CStringStorage::Instance() )              
{
        GUCEF_BEGIN;
        CHECKMEM( _store, sizeof(CStringStorage) );
        if ( src && length )
        {
                _stringid = _store->RegisterString( src      ,
                                                    length  ,
                                                    &_string );
                _length = length;                                                    
        }
        GUCEF_END;                                               
}                  

/*-------------------------------------------------------------------------*/

CString::~CString()
{
        GUCEF_BEGIN;
        if ( _stringid ) _store->UnregisterString( _stringid );
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CString&
CString::operator=( const CString &src )
{
        GUCEF_BEGIN;
        if ( this != &src )
        {
                if ( _stringid ) 
                {
                        CHECKMEM( _store, sizeof(CStringStorage) );
                        _store->UnregisterString( _stringid );        
                } 
                if ( src._stringid )
                {                       
                        _store->RegisterStringCopy( src._stringid );
                        _stringid = src._stringid;
                        _string = src._string;
                        _length = src._length;
                        GUCEF_END;
                        return *this;
                }
                _stringid = 0;
                _length = 0;
                _string = NULL;
                GUCEF_END;
                return *this;                 
        }
        GUCEF_END;                
        return *this;
}

/*-------------------------------------------------------------------------*/

CString&
CString::operator=( const char *src )
{
        GUCEF_BEGIN;
        if ( _stringid ) 
        {
                CHECKMEM( _store, sizeof(CStringStorage) );
                int a = _stringid;
                a = _stringid;
                a = _stringid;
                a = _stringid;
                _store->UnregisterString( a );
        }                
        if ( src )
        {                        
                _length = (UInt32)strlen( src );
                if ( _length )
                {
                        _stringid = _store->RegisterString( src      ,
                                                            _length  ,
                                                            &_string );
                        GUCEF_END;                                                            
                        return *this;                                                            
                }
                _string = NULL;
                _stringid = 0;
                GUCEF_END;
                return *this;                                                    
        }
        _stringid = 0;
        _length = 0;
        _string = NULL;
        GUCEF_END;                                                    
        return *this;
}

/*-------------------------------------------------------------------------*/

bool
CString::operator==( const char *other ) const
{TRACE;
        if ( !_string || !other )
        {
                return false;        
        } 
        return strcmp( _string ,
                       other   ) == 0;
}

/*-------------------------------------------------------------------------*/

bool
CString::operator==( const CString &other ) const
{TRACE;
        if ( !_string || !other._string )
        {
                return false;        
        }         
        return strcmp( _string       ,
                       other._string ) == 0;
}

/*-------------------------------------------------------------------------*/

bool
CString::operator!=( const CString &other ) const
{TRACE;
        if ( !_string || !other._string )
        {
                return false;        
        }         
        return strcmp( _string       ,
                       other._string ) != 0;
}

/*-------------------------------------------------------------------------*/

bool
CString::operator!=( const char *other ) const
{TRACE;
        if ( !_string || !other )
        {
                return false;        
        } 
        return strcmp( _string ,
                       other   ) != 0;
}

/*-------------------------------------------------------------------------*/

char
CString::operator[]( const UInt32 index ) const
{TRACE;
        return _string[ index ];
}

/*-------------------------------------------------------------------------*/

bool 
CString::operator<( const CString& other ) const
{TRACE;
    if ( _string && other._string )
    {
        return strcmp( _string, other._string ) < 0;
    }
    return false;        
}

/*-------------------------------------------------------------------------*/

CString::operator std::string() const
{TRACE;
    return std::string( _string );
}

/*-------------------------------------------------------------------------*/
/*
CString::operator const char*() const
{TRACE;
        return _string;
}
*/
/*-------------------------------------------------------------------------*/

/**
 *      This member functions allows you to set the string using a
 *      non null-terminated char array as the source. The source will
 *      be copyed and a null terminator will be added.
 */
void
CString::Set( const char *new_str ,
              UInt32 len          )
{
        GUCEF_BEGIN;
        if ( _stringid )
        { 
                _store->UnregisterString( _stringid );
        }                
        
        if ( new_str && len )
        {        
                _stringid = _store->RegisterString( new_str ,
                                                    len     ,
                                                    &_string );
                _length = len;
                GUCEF_END;
                return;
        }
        
        _string = NULL;
        _length = 0;                
        _stringid = 0;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

/**
 *      This member functions allows you to add to the string using a
 *      non-null-terminated char array as the source. The source will
 *      be copyed and a null terminator will be added.
 */                  
void 
CString::Append( const char *appendstr ,
                 UInt32 len            )
{
        GUCEF_BEGIN;
        if ( appendstr && len )
        {         
                if ( _stringid ) 
                {                  
                        UInt32 oldstrid = _stringid;                              
                        _stringid = _store->RegisterStringCombo( _string   ,
                                                                 _length   ,
                                                                 appendstr ,
                                                                 len       ,
                                                                 &_string  );
                        _length += len;
                        _store->UnregisterString( oldstrid );                                                                                      
                }                                                                               
        }
        GUCEF_END;                                                                    
}                 

/*-------------------------------------------------------------------------*/

CString&
CString::operator+=( const CString &other )
{
        GUCEF_BEGIN;
        if ( other._stringid )
        {         
                if ( _stringid ) 
                {                  
                        UInt32 oldstrid = _stringid;                              
                        _stringid = _store->RegisterStringCombo( _string       ,
                                                                 _length       ,
                                                                 other._string ,
                                                                 other._length ,
                                                                 &_string      );
                        _length += other._length;
                        _store->UnregisterString( oldstrid );                        
                        GUCEF_END;
                        return *this;                                                                                        
                }             
                _store->RegisterStringCopy( _stringid );
                _stringid = other._stringid;
                _string = other._string;
                _length = other._length;
                GUCEF_END;
                return *this;                                                                    
        }
        GUCEF_END;                                                                  
        return *this;
}

/*-------------------------------------------------------------------------*/

CString&
CString::operator+=( const char *other )
{
        GUCEF_BEGIN;
        if ( other )
        {           
                if ( _stringid )
                {    
                        UInt32 oldstrid( _stringid ), length2 = (UInt32)strlen( other ); 
                        _stringid = _store->RegisterStringCombo( _string   ,
                                                                 _length   ,
                                                                 other     ,
                                                                 length2   ,
                                                                 &_string  );
                        _length += length2;                                          
                        if ( oldstrid ) 
                        {
                                _store->UnregisterString( oldstrid );
                        }
                        GUCEF_END;
                        return *this;                                
                }
                UInt32 len( (UInt32)strlen( other ) );    
                _stringid = _store->RegisterString( other    ,
                                                    len      ,
                                                    &_string );
                _length = len;
                GUCEF_END;
                return *this;                                        
        }
        GUCEF_END;                                                                  
        return *this;
}

/*-------------------------------------------------------------------------*/

CString&
CString::operator+=( char lastchar )
{
        GUCEF_BEGIN;
        char tmpstr[ 2 ] = { 0, 0 };
        *tmpstr = lastchar;
        
        if ( _stringid )
        {      
                UInt32 oldstrid( _stringid );
                _stringid = _store->RegisterStringCombo( _string   ,
                                                         _length   ,
                                                         tmpstr    ,
                                                         1         ,
                                                         &_string  );
                ++_length;                                                  
                if ( oldstrid ) _store->UnregisterString( oldstrid );
                GUCEF_END;
                return *this;
        }         
        _stringid = _store->RegisterString( tmpstr   ,
                                            1        ,
                                            &_string );
        _length = 1;
        GUCEF_END;                                                     
        return *this;        
}

/*-------------------------------------------------------------------------*/

const char*
CString::C_String( void ) const
{
        GUCEF_BEGIN;
        GUCEF_END;
        return _string;
}

/*-------------------------------------------------------------------------*/

std::string 
CString::STL_String( void ) const
{
        GUCEF_BEGIN;
        std::string stlstr( _string );
        GUCEF_END;
        return stlstr;
}

/*-------------------------------------------------------------------------*/

UInt32
CString::Length( void ) const
{
        GUCEF_BEGIN;
        GUCEF_END;
        return _length;
}

/*-------------------------------------------------------------------------*/

UInt32 
CString::GetID( void ) const
{
        GUCEF_BEGIN;
        GUCEF_END;
        return _stringid;
}

/*-------------------------------------------------------------------------*/

CString 
CString::ReplaceChar( char oldchar ,
                      char newchar ) const
{
        GUCEF_BEGIN;
        if ( _length )
        {
                char* str = new char[ _length+1 ];
                memcpy( str, _string, _length+1 );
                for ( UInt32 i=0; i<_length; ++i )
                {
                        if ( str[ i ] == oldchar )
                        {
                                str[ i ] = newchar;
                        }
                }
                CString newstr( str );
                delete []str;
                GUCEF_END; 
                return newstr;
        }
        CString emptystr;                        
        GUCEF_END;       
        return emptystr;  
}                             

/*-------------------------------------------------------------------------*/

void 
CString::ReplaceSubstr( const CString& substr      ,
                        const CString& replacement )
{
        GUCEF_BEGIN;
        GUCEF_END;        
}

/*-------------------------------------------------------------------------*/

void 
CString::Clear( void )
{
        GUCEF_BEGIN;
        if ( _stringid ) 
        {
                _store->UnregisterString( _stringid );
                _stringid = 0;
        }                        
        _length = 0;
        _string = NULL;
        GUCEF_END;        
}

/*-------------------------------------------------------------------------*/

CString 
CString::SubstrToChar( char searchchar ,
                       bool startfront ) const
{
        GUCEF_BEGIN;
        if ( startfront )
        {
                for ( UInt32 i=0; i<_length; ++i )
                {
                        if ( _string[ i ] == searchchar )
                        {
                                CString substr;
                                substr.Set( _string ,
                                            i       );
                                GUCEF_END;            
                                return substr;
                        }
                }
                CString substr( *this );
                GUCEF_END;
                return substr;
        }
        
        for ( Int32 i=_length-1; i>=0; --i )
        {
                if ( _string[ i ] == searchchar )
                {
                        CString substr;
                        substr.Set( _string+(_length-i) ,
                                    i                   );
                        GUCEF_END;                                    
                        return substr;
                }
        }
        CString substr( *this );
        GUCEF_END;
        return substr;        
}

/*-------------------------------------------------------------------------*/

CString
CString::SubstrToSubstr( const CString& searchstr ,
                         bool startfront          ) const
{
        GUCEF_BEGIN;
        UInt32 slen = searchstr.Length();
        
        if ( slen > _length )
        {
                CString substr( *this );
                GUCEF_END;
                return substr;
        }
        
        if ( startfront )
        {
                UInt32 max = _length - slen;
                for ( UInt32 i=0; i<max; ++i )
                {
                        if ( memcmp( _string+i, searchstr._string, slen ) == 0 )
                        {
                                CString substr;
                                substr.Set( _string ,
                                            i       );
                                GUCEF_END;            
                                return substr;
                        }
                }
                CString substr( *this );
                GUCEF_END;
                return substr;
        }
        
        UInt32 max = _length - slen;        
        for ( Int32 i=max-1; i>=0; --i )
        {
                if ( memcmp( _string+i, searchstr._string, slen ) == 0 )
                {
                        CString substr;
                        substr.Set( _string+(max-i) ,
                                    i               );
                        GUCEF_END;                                    
                        return substr;
                }
        }
        CString substr( *this );
        GUCEF_END;
        return substr; 
}                         

/*-------------------------------------------------------------------------*/

CString 
CString::CutChars( UInt32 charcount ,
                   bool startfront  ) const
{
        GUCEF_BEGIN;
        if ( startfront )
        {
                if ( charcount < _length )                
                {
                        CString substr( _string+charcount ,
                                        _length-charcount );
                        GUCEF_END;
                        return substr;                                        
                }
                
                CString emptystr;
                GUCEF_END;
                return emptystr;
        }
        
        if ( charcount < _length )        
        {
                CString substr;
                substr.Set( _string+_length-charcount ,
                            charcount                 );
                GUCEF_END;                            
                return substr;                                    
        }
        
        CString emptystr;
        GUCEF_END;
        return emptystr;        
}

/*-------------------------------------------------------------------------*/

void 
CString::SetInt( Int32 value )                   
{
        GUCEF_BEGIN;
        char intval[ 10 ];
        sprintf( intval, "%d", value );        
        *this = intval;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

Int32 
CString::GetInt( void ) const
{
        GUCEF_BEGIN;
        if ( _string )
        {
                GUCEF_END_RET( Int32, Str_To_Int( _string ) );
                
        }                        
        GUCEF_END;
        return 0;
}

/*-------------------------------------------------------------------------*/

CString 
CString::Lowercase( void ) const
{
        GUCEF_BEGIN;
        char* lcstr = new char[ _length+1 ];
        memcpy( lcstr, _string, _length+1 );
        for ( UInt32 i=0; i<_length; ++i )
        {
                if ( ( lcstr[ i ] > 64 ) &&
                     ( lcstr[ i ] < 91 ) )
                {
                        lcstr[ i ] += 32;
                }        
        }
        CString lower( lcstr, _length );
        delete []lcstr;
        GUCEF_END;
        return lower;        
}

/*-------------------------------------------------------------------------*/
        
CString 
CString::Uppercase( void ) const
{
        GUCEF_BEGIN;
        char* ucstr = new char[ _length+1 ];
        memcpy( ucstr, _string, _length+1 );
        for ( UInt32 i=0; i<_length; ++i )
        {
                if ( ( ucstr[ i ] > 96 ) &&
                     ( ucstr[ i ] < 123 ) )
                {
                        ucstr[ i ] -= 32;
                }        
        }
        CString upper( ucstr, _length );
        delete []ucstr;
        GUCEF_END;
        return upper;
}

/*-------------------------------------------------------------------------*/

Int32 
CString::HasChar( char searchchar  ,
                  bool startfront  ) const
{
        if ( startfront )
        {
                for ( UInt32 i=0; i<_length; ++i )
                {
                        if ( _string[ i ] == searchchar )
                        {
                                return i;
                        }                
                }
                return -1;
        }
        
        for ( Int32 i=_length-1; i>=0; --i )
        {
                if ( _string[ i ] == searchchar )
                {
                        return _length-i;
                }                
        }
        return -1;        
}

/*-------------------------------------------------------------------------*/

CStringList 
CString::ParseElements( char seperator ) const
{
        GUCEF_BEGIN;
        CStringList list;
        CString entry;
        UInt32 last = 0;
        for ( UInt32 i=0; i<_length; ++i )
        {
                if ( _string[ i ] == seperator )
                {
                        entry.Set( _string+last ,
                                   i-last       );
                        list.Append( entry );
                        last = i;                    
                }  
        }
        /* add last item */
        entry.Set( _string+last ,
                   _length-last );        
        list.Append( entry ); 
        GUCEF_END;
        return list;        
}

/*-------------------------------------------------------------------------*/
                                
Int32 
CString::HasSubstr( const CString& substr ,
                    bool startfront       ) const
{
        GUCEF_BEGIN;
        UInt32 slen = substr.Length();
        
        if ( slen > _length )
        {
                GUCEF_END;
                return -1;
        }
        
        if ( substr._length == _length )
        {
                if ( memcmp( _string, substr._string, _length ) == 0 )
                {
                        return 0;
                }
                return -1;
        }
        
        if ( startfront )
        {
                UInt32 max = _length - slen;
                for ( UInt32 i=0; i<=max; ++i )
                {
                        if ( memcmp( _string+i, substr._string, slen ) == 0 )
                        {
                                GUCEF_END;            
                                return i;
                        }
                }
                GUCEF_END;
                return -1;
        }
        
        UInt32 max = _length - slen;        
        for ( Int32 i=max; i>=0; --i )
        {
                if ( memcmp( _string+i, substr._string, slen ) == 0 )
                {
                        GUCEF_END;                                    
                        return max-i;
                }
        }
        GUCEF_END;
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
        GUCEF_BEGIN;
        CString tmp( lhs );
        tmp += rhs;
        GUCEF_END;
        return tmp;
}

/*-------------------------------------------------------------------------*/

CString
operator+( const CString& lhs ,
           const char* rhs    )
{TRACE;
        GUCEF_BEGIN;
        CString tmp( lhs );
        tmp += rhs;
        GUCEF_END;
        return tmp;
}

/*-------------------------------------------------------------------------*/

CString 
operator+( const char* lhs    ,  
           const CString& rhs )
{TRACE;
        GUCEF_BEGIN;
        CString tmp( lhs );
        tmp += rhs;
        GUCEF_END;
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
