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

#include "CStringList.h"   /* definition of this abstract base class */

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

CStringList::CStringList( void )
        : _allowmultiple( true )
{TRACE;
}

/*-------------------------------------------------------------------------*/

CStringList::CStringList( const CStringList& src )
        : _entrys( src._entrys.GetArraySize() ) ,
          _allowmultiple( src._allowmultiple )
{TRACE;
        UInt32 count = src._entrys.GetCount();
        for ( UInt32 i=0; i<count; ++i )
        {       
                _entrys.SetEntry( i                                                 ,
                                  new CString( *( ( CString* ) src._entrys[ i ] ) ) );
        }  
}

/*-------------------------------------------------------------------------*/

CStringList::~CStringList()
{TRACE;
        for ( Int32 i=0; i<=_entrys.GetLast(); ++i )
        {       
                CHECKMEM( _entrys[ i ], sizeof( CString ) );
                delete ( CString* ) _entrys[ i ];
        }       
}

/*-------------------------------------------------------------------------*/

Int32 
CStringList::Find( const CString& string ) const
{TRACE;
        for ( Int32 i=0; i<=_entrys.GetLast(); ++i )
        {       
                CHECKMEM( _entrys[ i ], sizeof(CString) );
                if ( *(( CString* )_entrys[ i ]) == string )
                {
                        return i;
                }
        }
        return -1;
}

/*-------------------------------------------------------------------------*/

bool 
CStringList::Delete( UInt32 index )
{TRACE;
        if ( index < _entrys.GetCount() )
        {
                delete static_cast<CString*>( _entrys[ index ] );                
                _entrys.SetEntry( index, NULL );
                _entrys.FillNULLGaps();
                return true;
        }
        return false;
}

/*-------------------------------------------------------------------------*/

void 
CStringList::Clear( void )
{TRACE;
        for ( UInt32 i=0; i<_entrys.GetCount(); ++i )
        {
                delete static_cast<CString*>( _entrys[ i ] );
        }
        _entrys.Clear();
}

/*-------------------------------------------------------------------------*/

void 
CStringList::AddAtIndex( const CString& string ,
                         const UInt32 index    )
{TRACE;
        UInt32 oldMax( _entrys.GetArraySize() );
        
        _entrys.SetEntry( index, new CString( string ) );
        
        if ( index >= oldMax )
        {
                UInt32 newMax( _entrys.GetArraySize() );
                for ( UInt32 i=oldMax; i<newMax; ++i )
                {
                        _entrys[ i ] = new CString();
                }                
        }        
}

/*-------------------------------------------------------------------------*/

void
CStringList::SetAllowDuplicates( bool allow )
{TRACE;
        _allowmultiple = allow;
}

/*-------------------------------------------------------------------------*/

bool
CStringList::GetAllowDuplicates( void ) const
{TRACE;
        return _allowmultiple;        
}

/*-------------------------------------------------------------------------*/

void 
CStringList::Merge( const CStringList& other ,
                    bool allow_duplicates    )
{TRACE;
        if ( allow_duplicates )
        {
                const_iterator n = other.ConstBegin();
                while ( n != other.ConstEnd() )
                {
                        Append( *n );
                        ++n;
                }        
        }
        
        const_iterator n = other.ConstBegin();
        while ( n != other.ConstEnd() )
        {
                if ( Find( *n ) < 0 )
                {
                        Append( *n );
                        ++n;
                }
        }          
}

/*-------------------------------------------------------------------------*/

CString& 
CStringList::operator[]( UInt32 index )
{TRACE;
        CHECKMEM( _entrys[ index ], sizeof(CString) );
        return *( static_cast< CString* >( _entrys[ index ] ) );
}

/*-------------------------------------------------------------------------*/

CString 
CStringList::operator[]( UInt32 index ) const
{TRACE;
        CHECKMEM( _entrys[ index ], sizeof(CString) );
        return *( (CString*)_entrys[ index ] );
}

/*-------------------------------------------------------------------------*/

UInt32
CStringList::GetCount( void ) const
{TRACE;
        return (_entrys.GetLast()+1);
}

/*-------------------------------------------------------------------------*/

CStringList&
CStringList::operator=( const CStringList& src )
{TRACE;
        const_iterator i = src.ConstBegin();
        while ( i != src.ConstEnd() )
        {
                
                ++i;
        }
        return *this;
}

/*-------------------------------------------------------------------------*/

CStringList::iterator
CStringList::Begin( void )
{TRACE;
        iterator i( this );
        return i;
}

/*-------------------------------------------------------------------------*/

CStringList::iterator
CStringList::End( void ) 
{TRACE;
        iterator i( this, GetCount() );
        return i;
}

/*-------------------------------------------------------------------------*/

CStringList::const_iterator
CStringList::Begin( void ) const
{TRACE;
        const_iterator i( this );
        return i;
}

/*-------------------------------------------------------------------------*/

CStringList::const_iterator
CStringList::End( void ) const
{TRACE;
        const_iterator i( this, GetCount() );
        return i;
}

/*-------------------------------------------------------------------------*/

CStringList::const_iterator
CStringList::ConstBegin( void ) const
{TRACE;
        const_iterator i( this );
        return i;
}

/*-------------------------------------------------------------------------*/

CStringList::const_iterator
CStringList::ConstEnd( void ) const
{TRACE;
        const_iterator i( this, GetCount() );
        return i;
}

/*-------------------------------------------------------------------------*/

UInt32
CStringList::Append( const CString& string )
{TRACE;
        if ( _allowmultiple )
        {
                return _entrys.AppendEntry( new CString( string ) );
        }
        Int32 findIndex( Find( string ) );
        if ( findIndex < 0 )
        {
                return _entrys.AppendEntry( new CString( string ) );       
        }
        return (UInt32) findIndex;               
}

/*-------------------------------------------------------------------------*/

bool
CStringList::LoadFromFile( const CString& filename )
{TRACE;
        FILE* fptr = fopen( filename.C_String(), "rb" );
        fseek( fptr, 0, SEEK_END );
        UInt32 fsize = ftell( fptr );
        fseek( fptr, 0, SEEK_SET );
        
        char* filebuffer = new char[ fsize+1 ];
        CHECKMEM( filebuffer, fsize+1 ); 
        fread( filebuffer, fsize, 1, fptr );
        filebuffer[ fsize ] = '\0';
        
        CString linestr;
        UInt32 n=0;
        for ( UInt32 i=0; i<fsize; ++i )
        {
                while ( ( i < fsize ) &&
                        ( ( filebuffer[ i ] != '\n' ) ||
                          ( filebuffer[ i ] != '\r' ) ) )
                {
                        linestr += filebuffer[ i ];                
                }
                Append( linestr );
                n=i;                          
        }
        return false;
}

/*-------------------------------------------------------------------------*/

bool 
CStringList::LoadFromFile( CIOAccess* file )
{TRACE;
        while ( !file->Eof() )
        {
                Append( file->ReadLine() );
        }
        return true;
}

/*-------------------------------------------------------------------------*/

void
CStringList::SaveToFile( const CString& filename )
{TRACE;
        FILE* fptr = fopen( filename.C_String(), "wb" );
        if ( fptr )
        {
                for ( Int32 i=0; i<=_entrys.GetLast(); ++i )
                {
                        fprintf( fptr, "%s\n", _entrys[ i ] );
                }                       
                fclose( fptr );
        }
}

/*-------------------------------------------------------------------------*/

CStringList::iterator::iterator( CStringList* list )
        : _list( list ) ,
          _pos( 0 )
{TRACE;

}

/*-------------------------------------------------------------------------*/

CStringList::iterator::iterator( CStringList* list ,
                                 UInt32 position   )
        : _pos( position ) ,
          _list( list )
{TRACE;
}

/*-------------------------------------------------------------------------*/

CStringList::iterator::iterator( const iterator& src )
{TRACE;
        *this = src;
}

/*-------------------------------------------------------------------------*/

CStringList::iterator::~iterator()
{TRACE;

}

/*-------------------------------------------------------------------------*/

CStringList::iterator&
CStringList::iterator::operator=( const iterator& src )
{TRACE;
        _list = src._list;
        _pos = src._pos;
        return *this;
}

/*-------------------------------------------------------------------------*/

CString&
CStringList::iterator::operator*( void )
{TRACE;
        return *(CString*)(_list->_entrys[ _pos ]);
}

/*-------------------------------------------------------------------------*/

CString&
CStringList::iterator::operator[]( UInt32 offset )
{TRACE;
        if ( _list->_entrys.GetCount() )
        {  
                if ( offset >= _list->_entrys.GetCount() )
                {
                        _pos = _list->_entrys.GetCount()-1;
                }
                else
                {                
                        _pos = offset;
                }                        
        }                
        return *(CString*)(_list->_entrys[ _pos ]);
}

/*-------------------------------------------------------------------------*/

CStringList::iterator&
CStringList::iterator::operator+=( Int32 offset )
{TRACE;
        if ( _pos + offset < _list->_entrys.GetCount() )
        {
                _pos += offset;
        }                
        return *this;
}

/*-------------------------------------------------------------------------*/

CStringList::iterator&
CStringList::iterator::operator-=( Int32 offset )
{TRACE;
        if ( _pos - offset >= 0 )
        {
                _pos -= offset;
        }                
        return *this;
}

/*-------------------------------------------------------------------------*/

CStringList::iterator&
CStringList::iterator::operator++( void )
{TRACE;
        if ( _pos+1 < _list->_entrys.GetCount() )
        {
                ++_pos;
        }                
        return *this;
}

/*-------------------------------------------------------------------------*/

CStringList::iterator&
CStringList::iterator::operator--( void )
{TRACE;
        if ( _pos > 0 )
        {
                --_pos;
        }                
        return *this;
}

/*-------------------------------------------------------------------------*/

bool
CStringList::iterator::operator!=( const iterator& src ) const
{TRACE;
        return _pos != src._pos;        
}

/*-------------------------------------------------------------------------*/

void
CStringList::iterator::Insert( const CString& string )
{TRACE;
        if ( _list->_allowmultiple )
        {
                _list->_entrys.ShiftUp( _pos );
                _list->_entrys.SetEntry( _pos                  ,
                                         new CString( string ) );
                return;                                         
        }
        if ( _list->Find( string ) < 0 )
        {        
                _list->_entrys.ShiftUp( _pos );
                _list->_entrys.SetEntry( _pos                  ,
                                         new CString( string ) );        
        }                                         
}

/*-------------------------------------------------------------------------*/

void
CStringList::iterator::Delete( void )
{TRACE;
        CHECKMEM( _list->_entrys[ _pos ], sizeof(CString) );
        delete (CString*) _list->_entrys[ _pos ];
        _list->_entrys.ShiftDown( _pos );
}

/*-------------------------------------------------------------------------*/

CStringList::const_iterator::const_iterator( const CStringList* list )
        : _list( list ) ,
          _pos( 0 )
{TRACE;

}

/*-------------------------------------------------------------------------*/

CStringList::const_iterator::const_iterator( const CStringList* list ,
                                                   UInt32 position         )
        : _pos( position ) ,
          _list( list )
{TRACE;
}

/*-------------------------------------------------------------------------*/

CStringList::const_iterator::const_iterator( const const_iterator& src )
{TRACE;
        *this = src;
}

/*-------------------------------------------------------------------------*/

CStringList::const_iterator::~const_iterator()
{TRACE;

}

/*-------------------------------------------------------------------------*/

CStringList::const_iterator&
CStringList::const_iterator::operator=( const const_iterator& src )
{TRACE;
        _pos = src._pos;
        _list = src._list;
        return *this;
}

/*-------------------------------------------------------------------------*/

const CString&
CStringList::const_iterator::operator*( void ) const
{TRACE;
        CHECKMEM( _list->_entrys[ _pos ], sizeof(CString) );
        return *(CString*)(_list->_entrys[ _pos ]);
}

/*-------------------------------------------------------------------------*/

const CString&
CStringList::const_iterator::operator[]( UInt32 offset )
{TRACE;
        _pos = offset;
        CHECKMEM( _list->_entrys[ offset ], sizeof(CString) );
        return *(CString*)(_list->_entrys[ offset ]);        
}

/*-------------------------------------------------------------------------*/

CStringList::const_iterator&
CStringList::const_iterator::operator+=( Int32 offset )
{TRACE;
        _pos += offset;
        return *this;
}

/*-------------------------------------------------------------------------*/

CStringList::const_iterator&
CStringList::const_iterator::operator-=( Int32 offset )
{TRACE;
        _pos -= offset;
        return *this;
}

/*-------------------------------------------------------------------------*/

CStringList::const_iterator&
CStringList::const_iterator::operator++( void )
{TRACE;
        ++_pos;
        return *this;
}

/*-------------------------------------------------------------------------*/

CStringList::const_iterator&
CStringList::const_iterator::operator--( void )
{TRACE;
        --_pos;
        return *this;
}

/*-------------------------------------------------------------------------*/

bool
CStringList::const_iterator::operator!=( const const_iterator& src ) const
{TRACE;
        return _pos != src._pos;        
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/


