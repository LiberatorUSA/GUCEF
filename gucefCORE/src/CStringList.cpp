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
{GUCEF_TRACE;
}

/*-------------------------------------------------------------------------*/

CStringList::CStringList( const CStringList& src )
        : _entrys( src._entrys.GetArraySize() ) ,
          _allowmultiple( src._allowmultiple )
{GUCEF_TRACE;
        UInt32 count = src._entrys.GetCount();
        for ( UInt32 i=0; i<count; ++i )
        {       
                _entrys.SetEntry( i                                                 ,
                                  new CString( *( ( CString* ) src._entrys[ i ] ) ) );
        }  
}

/*-------------------------------------------------------------------------*/

CStringList::~CStringList()
{GUCEF_TRACE;
        for ( Int32 i=0; i<=_entrys.GetLast(); ++i )
        {       
                CHECKMEM( _entrys[ i ], sizeof( CString ) );
                delete ( CString* ) _entrys[ i ];
        }       
}

/*-------------------------------------------------------------------------*/

Int32 
CStringList::Find( const CString& string ) const
{GUCEF_TRACE;
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
{GUCEF_TRACE;
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
{GUCEF_TRACE;
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
{GUCEF_TRACE;
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
{GUCEF_TRACE;
        _allowmultiple = allow;
}

/*-------------------------------------------------------------------------*/

bool
CStringList::GetAllowDuplicates( void ) const
{GUCEF_TRACE;
        return _allowmultiple;        
}

/*-------------------------------------------------------------------------*/

void 
CStringList::Merge( const CStringList& other ,
                    bool allow_duplicates    )
{GUCEF_TRACE;
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
{GUCEF_TRACE;
        CHECKMEM( _entrys[ index ], sizeof(CString) );
        return *( static_cast< CString* >( _entrys[ index ] ) );
}

/*-------------------------------------------------------------------------*/

CString 
CStringList::operator[]( UInt32 index ) const
{GUCEF_TRACE;
        CHECKMEM( _entrys[ index ], sizeof(CString) );
        return *( (CString*)_entrys[ index ] );
}

/*-------------------------------------------------------------------------*/

UInt32
CStringList::GetCount( void ) const
{GUCEF_TRACE;
        return (_entrys.GetLast()+1);
}

/*-------------------------------------------------------------------------*/

CStringList&
CStringList::operator=( const CStringList& src )
{GUCEF_TRACE;
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
{GUCEF_TRACE;
        iterator i( this );
        return i;
}

/*-------------------------------------------------------------------------*/

CStringList::iterator
CStringList::End( void ) 
{GUCEF_TRACE;
        iterator i( this, GetCount() );
        return i;
}

/*-------------------------------------------------------------------------*/

CStringList::const_iterator
CStringList::Begin( void ) const
{GUCEF_TRACE;
        const_iterator i( this );
        return i;
}

/*-------------------------------------------------------------------------*/

CStringList::const_iterator
CStringList::End( void ) const
{GUCEF_TRACE;
        const_iterator i( this, GetCount() );
        return i;
}

/*-------------------------------------------------------------------------*/

CStringList::const_iterator
CStringList::ConstBegin( void ) const
{GUCEF_TRACE;
        const_iterator i( this );
        return i;
}

/*-------------------------------------------------------------------------*/

CStringList::const_iterator
CStringList::ConstEnd( void ) const
{GUCEF_TRACE;
        const_iterator i( this, GetCount() );
        return i;
}

/*-------------------------------------------------------------------------*/

UInt32
CStringList::Append( const CString& string )
{GUCEF_TRACE;
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
{GUCEF_TRACE;
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
{GUCEF_TRACE;
        while ( !file->Eof() )
        {
                Append( file->ReadLine() );
        }
        return true;
}

/*-------------------------------------------------------------------------*/

void
CStringList::SaveToFile( const CString& filename )
{GUCEF_TRACE;
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
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CStringList::iterator::iterator( CStringList* list ,
                                 UInt32 position   )
        : _pos( position ) ,
          _list( list )
{GUCEF_TRACE;
}

/*-------------------------------------------------------------------------*/

CStringList::iterator::iterator( const iterator& src )
{GUCEF_TRACE;
        *this = src;
}

/*-------------------------------------------------------------------------*/

CStringList::iterator::~iterator()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CStringList::iterator&
CStringList::iterator::operator=( const iterator& src )
{GUCEF_TRACE;
        _list = src._list;
        _pos = src._pos;
        return *this;
}

/*-------------------------------------------------------------------------*/

CString&
CStringList::iterator::operator*( void )
{GUCEF_TRACE;
        return *(CString*)(_list->_entrys[ _pos ]);
}

/*-------------------------------------------------------------------------*/

CString&
CStringList::iterator::operator[]( UInt32 offset )
{GUCEF_TRACE;
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
{GUCEF_TRACE;
        if ( _pos + offset < _list->_entrys.GetCount() )
        {
                _pos += offset;
        }                
        return *this;
}

/*-------------------------------------------------------------------------*/

CStringList::iterator&
CStringList::iterator::operator-=( Int32 offset )
{GUCEF_TRACE;
        if ( _pos - offset >= 0 )
        {
                _pos -= offset;
        }                
        return *this;
}

/*-------------------------------------------------------------------------*/

CStringList::iterator&
CStringList::iterator::operator++( void )
{GUCEF_TRACE;
        if ( _pos+1 < _list->_entrys.GetCount() )
        {
                ++_pos;
        }                
        return *this;
}

/*-------------------------------------------------------------------------*/

CStringList::iterator&
CStringList::iterator::operator--( void )
{GUCEF_TRACE;
        if ( _pos > 0 )
        {
                --_pos;
        }                
        return *this;
}

/*-------------------------------------------------------------------------*/

bool
CStringList::iterator::operator!=( const iterator& src ) const
{GUCEF_TRACE;
        return _pos != src._pos;        
}

/*-------------------------------------------------------------------------*/

void
CStringList::iterator::Insert( const CString& string )
{GUCEF_TRACE;
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
{GUCEF_TRACE;
        CHECKMEM( _list->_entrys[ _pos ], sizeof(CString) );
        delete (CString*) _list->_entrys[ _pos ];
        _list->_entrys.ShiftDown( _pos );
}

/*-------------------------------------------------------------------------*/

CStringList::const_iterator::const_iterator( const CStringList* list )
        : _list( list ) ,
          _pos( 0 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CStringList::const_iterator::const_iterator( const CStringList* list ,
                                                   UInt32 position         )
        : _pos( position ) ,
          _list( list )
{GUCEF_TRACE;
}

/*-------------------------------------------------------------------------*/

CStringList::const_iterator::const_iterator( const const_iterator& src )
{GUCEF_TRACE;
        *this = src;
}

/*-------------------------------------------------------------------------*/

CStringList::const_iterator::~const_iterator()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CStringList::const_iterator&
CStringList::const_iterator::operator=( const const_iterator& src )
{GUCEF_TRACE;
        _pos = src._pos;
        _list = src._list;
        return *this;
}

/*-------------------------------------------------------------------------*/

const CString&
CStringList::const_iterator::operator*( void ) const
{GUCEF_TRACE;
        CHECKMEM( _list->_entrys[ _pos ], sizeof(CString) );
        return *(CString*)(_list->_entrys[ _pos ]);
}

/*-------------------------------------------------------------------------*/

const CString&
CStringList::const_iterator::operator[]( UInt32 offset )
{GUCEF_TRACE;
        _pos = offset;
        CHECKMEM( _list->_entrys[ offset ], sizeof(CString) );
        return *(CString*)(_list->_entrys[ offset ]);        
}

/*-------------------------------------------------------------------------*/

CStringList::const_iterator&
CStringList::const_iterator::operator+=( Int32 offset )
{GUCEF_TRACE;
        _pos += offset;
        return *this;
}

/*-------------------------------------------------------------------------*/

CStringList::const_iterator&
CStringList::const_iterator::operator-=( Int32 offset )
{GUCEF_TRACE;
        _pos -= offset;
        return *this;
}

/*-------------------------------------------------------------------------*/

CStringList::const_iterator&
CStringList::const_iterator::operator++( void )
{GUCEF_TRACE;
        ++_pos;
        return *this;
}

/*-------------------------------------------------------------------------*/

CStringList::const_iterator&
CStringList::const_iterator::operator--( void )
{GUCEF_TRACE;
        --_pos;
        return *this;
}

/*-------------------------------------------------------------------------*/

bool
CStringList::const_iterator::operator!=( const const_iterator& src ) const
{GUCEF_TRACE;
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


