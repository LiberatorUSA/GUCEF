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

#include <functional>           /* needed here for std::min() */
#include <stdlib.h>             /* needed here for realloc() */
#include <string.h>             /* needed for memcpy() */

#ifndef GUCEF_CORE_CIOACCESS_H
#include "CIOAccess.h"
#define GUCEF_CORE_CIOACCESS_H
#endif /* GUCEF_CORE_CIOACCESS_H ? */

#ifndef GUCEF_CORE_DVFILEUTILS_H
#include "dvfileutils.h"
#define GUCEF_CORE_DVFILEUTILS_H
#endif /* GUCEF_CORE_DVFILEUTILS_H ? */

#include "CDynamicBuffer.h"     /* class definition */

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

GUCEF_IMPLEMENT_MSGEXCEPTION( CDynamicBuffer, EIllegalCast );

/*-------------------------------------------------------------------------*/

CDynamicBuffer::CDynamicBuffer( void )
        : _buffer( NULL )             , 
          _bsize( 0 )                 ,
          _autoenlarge( true )        ,
          m_dataSize( 0 )             ,
          m_linked( false )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CDynamicBuffer::CDynamicBuffer( bool autoenlarge ) 
        : _buffer( NULL )             , 
          _bsize( 0 )                 ,
          _autoenlarge( autoenlarge ) ,
          m_dataSize( 0 )             ,
          m_linked( false )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CDynamicBuffer::CDynamicBuffer( CIOAccess& ioAccess )
        : _buffer( NULL )             , 
          _bsize( 0 )                 ,
          _autoenlarge( true )        ,
          m_dataSize( 0 )             ,
          m_linked( false )
{GUCEF_TRACE;
    
    Int32 inputSize = ioAccess.GetSize();
    if ( inputSize > -1 )
    {
        // Copy the recource data all at once
        SetBufferSize( inputSize );
        ioAccess.Read( *this     ,
                       1         ,
                       inputSize );
    }
    else
    {
        // read blocks till we hit the end of the recource
        UInt32 nrOfBytesRead = 0;
        while ( !ioAccess.Eof() )
        {
            nrOfBytesRead = ioAccess.Read( *this ,
                                           1     ,
                                           1024  );
            if ( nrOfBytesRead < 1024 )
            {
                break;
            }
        }
    }
}

/*-------------------------------------------------------------------------*/

CDynamicBuffer::CDynamicBuffer( UInt32 initialsize ,
                                bool autoenlarge   )
        : _autoenlarge( autoenlarge ) ,
          _buffer( NULL )             ,
          _bsize( 0 )                 ,
          m_dataSize( 0 )             ,
          m_linked( false )
{GUCEF_TRACE;

    _buffer = (Int8*) malloc( initialsize );
    _bsize = initialsize;               
}

/*-------------------------------------------------------------------------*/

CDynamicBuffer::CDynamicBuffer( const CDynamicBuffer &src )
        : _buffer( NULL )                  ,
          _bsize( 0 )                      ,
          m_dataSize( 0 )                  ,
          _autoenlarge( src._autoenlarge ) ,
          m_linked( false )
{GUCEF_TRACE;

    _buffer = (Int8*) malloc( src._bsize );
    _bsize = src._bsize;
    m_dataSize = src.m_dataSize;
    memcpy( _buffer, src._buffer, m_dataSize ); 
}

/*-------------------------------------------------------------------------*/

CDynamicBuffer::CDynamicBuffer( const char* externalBuffer    ,
                                const UInt32 bufferSize       ,
                                bool autoenlarge /* = true */ )
        : _buffer( NULL )      ,
          _bsize( 0 )          ,
          m_dataSize( 0 )      ,
          _autoenlarge( true ) ,
          m_linked( false )
{GUCEF_TRACE;

    CopyFrom( bufferSize     ,
              externalBuffer );
    _autoenlarge = autoenlarge;
}

/*-------------------------------------------------------------------------*/

CDynamicBuffer::~CDynamicBuffer()
{GUCEF_TRACE;
        
        Clear( false );
}

/*-------------------------------------------------------------------------*/

UInt8&
CDynamicBuffer::GetUByte( UInt32 index )
{GUCEF_TRACE;

    SecureLinkBeforeMutation();
    
    if ( index+1 > m_dataSize )
    {
            m_dataSize = index+1;
    }
    return *((UInt8*)_buffer+index);
}

/*-------------------------------------------------------------------------*/

void
CDynamicBuffer::SetUByte( UInt32 index ,
                          UInt8 data   )
{GUCEF_TRACE;
        
    SecureLinkBeforeMutation();
    
    if ( index+1 > m_dataSize )
    {
        m_dataSize = index+1;
    }
    _buffer[ index ] = (char)data;
}

/*-------------------------------------------------------------------------*/

char&
CDynamicBuffer::GetChar( UInt32 index )
{GUCEF_TRACE;

    SecureLinkBeforeMutation();
    
    if ( index+1 > m_dataSize )
    {
        m_dataSize = index+1;
    }
    return reinterpret_cast< char* >( _buffer )[ index ];
}

/*-------------------------------------------------------------------------*/

/**
 *      Sets the signed byte located at the index given.
 */
void
CDynamicBuffer::SetChar( UInt32 index ,
                         char data    )
{GUCEF_TRACE;

    SecureLinkBeforeMutation();
    
    if ( index+1 > m_dataSize )
    {
        m_dataSize = index+1;
    }
    _buffer[ index ] = data;
}

/*-------------------------------------------------------------------------*/

/**
 *      Returns the signed byte located at the index given.
 */
char&
CDynamicBuffer::operator[]( UInt32 index )
{GUCEF_TRACE;
        SecureLinkBeforeMutation();
        
        if ( index+1 > m_dataSize )
        {
                SetBufferSize( index+1, false );
                m_dataSize = index+1;
        }
        return reinterpret_cast< char* >( _buffer )[ index ];
}
/*-------------------------------------------------------------------------*/

/**
 *      Returns the signed byte located at the index given.
 */
char
CDynamicBuffer::operator[]( UInt32 index ) const
{GUCEF_TRACE;
        return _buffer[ index ];
}

/*-------------------------------------------------------------------------*/

bool
CDynamicBuffer::operator==( const CDynamicBuffer& other ) const
{GUCEF_TRACE;
        if ( m_dataSize == other.m_dataSize )
        {
                return memcmp( _buffer, other._buffer, m_dataSize ) == 0;
        }       
        return false;
}

/*-------------------------------------------------------------------------*/

bool
CDynamicBuffer::operator!=( const CDynamicBuffer& other ) const
{GUCEF_TRACE;

    if ( m_dataSize == other.m_dataSize )
    {
        return memcmp( _buffer, other._buffer, m_dataSize ) != 0;
    }       
    return true;
}

/*-------------------------------------------------------------------------*/

/**
 *      Turns the current dynamic buffer into a copy of the source
 *      dynamic buffer and returns the result.
 */
CDynamicBuffer&
CDynamicBuffer::operator=( const CDynamicBuffer &src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        Clear( true );
        
        _buffer = (Int8*) realloc( _buffer, src._bsize );
        memcpy( _buffer, src._buffer, src._bsize );
        _bsize = src._bsize;
        m_dataSize = src.m_dataSize;
        m_linked = false;                 
    }
    return *this;                        
}

/*-------------------------------------------------------------------------*/

/**
 *      Sets the new buffer size.
 */
void
CDynamicBuffer::SetBufferSize( const UInt32 newSize      ,
                               const bool allowreduction )
{GUCEF_TRACE;

    SecureLinkBeforeMutation();
    
    if ( _bsize == newSize ) 
    {
            return;
    }
    if ( newSize < _bsize )
    {
            if ( !allowreduction )
            {
                    return;        
            }
            else
            {
                    if ( m_dataSize > newSize )
                    {
                            m_dataSize = newSize;
                    }
            }
    }
    
    if ( NULL == _buffer )
    {
        _buffer = (Int8*) malloc( newSize );
    }
    else
    {
        _buffer = (Int8*) realloc( _buffer, newSize );
    }
    _bsize = newSize;
}

/*-------------------------------------------------------------------------*/

UInt32
CDynamicBuffer::GetBufferSize( void ) const
{GUCEF_TRACE;
        return _bsize;
}

/*-------------------------------------------------------------------------*/

void 
CDynamicBuffer::SetAutoEnlarge( bool autoenlarge )
{GUCEF_TRACE;
        _autoenlarge = autoenlarge;
}

/*-------------------------------------------------------------------------*/
        
bool 
CDynamicBuffer::GetAutoEnlarge( void ) const
{GUCEF_TRACE;
        return _autoenlarge;
}

/*-------------------------------------------------------------------------*/

bool
CDynamicBuffer::LoadContentFromFile( const CString& filePath   ,
                                     const UInt32 offsetInFile ,
                                     const Int32 bytesToRead   )
{GUCEF_TRACE;

    // Set the buffer to be the same size as the file
    Clear( true );    
    
    UInt32 readBlockSize = 0;
    if ( bytesToRead < 0 )
    {
        UInt32 fileSize = CORE::FileSize( filePath );
        readBlockSize = fileSize - offsetInFile;
    }
    else
    {
        readBlockSize = (UInt32) bytesToRead;
    }
    
    SetBufferSize( readBlockSize, true );
    
    // Load the entire file into memory
    UInt32 actuallyRead = 0;
    FILE* filePtr = fopen( filePath.C_String(), "rb" );
    if ( NULL != filePtr )
    {
        fseek( filePtr, offsetInFile, SEEK_SET );
        actuallyRead = fread( _buffer, readBlockSize, 1, filePtr );
        fclose( filePtr );
    }
    else
    {
        // Failed to open the file for loading
        Clear( false );
        return false;
    }
    
    // Sanity check to make sure we loaded the whole block
    if ( actuallyRead == 1 )
    {
        m_dataSize = readBlockSize;
        return true;
    }
    
    // Failed to read the block
    Clear( false );
    return false;
}

/*-------------------------------------------------------------------------*/

UInt32
CDynamicBuffer::CopyFrom( UInt32 destinationOffset   ,
                          UInt32 size                ,
                          const void* src            )
{GUCEF_TRACE;

    SecureLinkBeforeMutation();

    if ( _bsize < destinationOffset+size )         
    {
        if ( _autoenlarge )
        {
            SetBufferSize( destinationOffset+size );
        }
        else
        {       
            Int32 max = (Int32)_bsize - (Int32)destinationOffset - (Int32)size;
            if ( max < 0 ) 
            {
                return 0;
            }                                
            size = max;
        }                
    }                                                
    
    if ( destinationOffset + size > m_dataSize )
    {
        m_dataSize = destinationOffset + size;
    }
    memcpy( _buffer+destinationOffset, src, size );
    return size;
}

/*-------------------------------------------------------------------------*/

UInt32 
CDynamicBuffer::CopyFrom( UInt32 size     ,
                          const void* src )
{GUCEF_TRACE;
    
    SecureLinkBeforeMutation();
    
    if ( _bsize < size )         
    {
            if ( _autoenlarge )
            {
                    SetBufferSize( size );
            }
            else
            {       
                    Int32 max = (Int32)_bsize - (Int32)size;
                    if ( max < 0 ) 
                    {
                            return 0;
                    }                                
                    size = max;
            }                
    }                                                
    
    m_dataSize = size;
    memcpy( _buffer, src, size );        
    return size;
}                          

/*-------------------------------------------------------------------------*/

/**
 *      Copys size number of bytes from the buffer to src at the offset
 *      given.
 */
UInt32
CDynamicBuffer::CopyTo( UInt32 offset ,
                        UInt32 size   ,
                        void *dest    ) const
{GUCEF_TRACE;

    if ( offset+size > m_dataSize )
    {
        Int32 maxPossible = (Int32)m_dataSize - (Int32)offset;
        if ( maxPossible > 0 )
        {
            memcpy( dest, _buffer+offset, maxPossible );
            return maxPossible;
        }
        return 0;
    }
    memcpy( dest, _buffer+offset, size );
    return size;
}

/*-------------------------------------------------------------------------*/

/**
 *      Copys all data from the buffer to dest
 */
UInt32 
CDynamicBuffer::CopyTo( void *dest ) const
{GUCEF_TRACE;

    memcpy( dest, _buffer, m_dataSize );
    return m_dataSize;
}

/*-------------------------------------------------------------------------*/

UInt32 
CDynamicBuffer::CopyTo( CDynamicBuffer& dest ) const
{GUCEF_TRACE;

    dest.CopyFrom( m_dataSize, _buffer );
    return m_dataSize;        
}

/*-------------------------------------------------------------------------*/

void* 
CDynamicBuffer::GetBufferPtr( const UInt32 offset /* = 0 */ )
{GUCEF_TRACE;
        SecureLinkBeforeMutation();
        
        return _buffer + offset;
}

/*-------------------------------------------------------------------------*/
       
const void* 
CDynamicBuffer::GetConstBufferPtr( const UInt32 offset /* = 0 */ ) const
{GUCEF_TRACE;
        return _buffer + offset;
}

/*-------------------------------------------------------------------------*/

void 
CDynamicBuffer::Append( const void* data                            ,
                        const UInt32 size                           ,
                        const bool appendToLogicalData /* = true */ )
{GUCEF_TRACE;

    SecureLinkBeforeMutation();

    if ( appendToLogicalData )
    {
        SetBufferSize( m_dataSize + size, false );
        memcpy( _buffer + m_dataSize, data, size );
        m_dataSize += size;
    }
    else
    {
        UInt32 oldBufferSize = _bsize;
        SetBufferSize( oldBufferSize + size, false );
        memcpy( _buffer + oldBufferSize, data, size );
        m_dataSize = _bsize;
    }
}

/*-------------------------------------------------------------------------*/

void 
CDynamicBuffer::Clear( const bool logicalClearOnly /* = true */ )
{GUCEF_TRACE;

    if ( m_linked )
    {
        // Nothing to deallocate, we don't have ownership of the data
        _buffer = NULL;
        _bsize = 0;
        m_dataSize = 0;
    }
    else
    {
        
        if ( !logicalClearOnly )
        {
            // Cleanup our toys
            free( _buffer );
            _buffer = NULL;
            _bsize = 0;
        } 
        // else: reset carat only
        
        m_dataSize = 0;        
    }
}

/*-------------------------------------------------------------------------*/

void
CDynamicBuffer::SetDataSize( const UInt32 newSize )
{GUCEF_TRACE;
        SecureLinkBeforeMutation();
        
        SetBufferSize( newSize, false );
        m_dataSize = newSize;        
}

/*-------------------------------------------------------------------------*/

UInt32
CDynamicBuffer::GetDataSize( void ) const
{GUCEF_TRACE;
        return m_dataSize;
}

/*-------------------------------------------------------------------------*/

bool
CDynamicBuffer::operator<( const CDynamicBuffer& other ) const
{GUCEF_TRACE;

    return memcmp( _buffer, other._buffer, SMALLEST( GetDataSize(), other.GetDataSize() ) ) < 0;
}

/*-------------------------------------------------------------------------*/

bool
CDynamicBuffer::operator>( const CDynamicBuffer& other ) const
{GUCEF_TRACE;

    return memcmp( _buffer, other._buffer, SMALLEST( GetDataSize(), other.GetDataSize() ) ) > 0;
}

/*-------------------------------------------------------------------------*/

void
CDynamicBuffer::LinkTo( const void* externalBuffer ,
                        UInt32 bufferSize          )
{GUCEF_TRACE;

    // Always clear first for safety,.. we don't want memory leaks or invalid pointers
    Clear( false );
    
    /*  Link to the external buffer
     *
     *  We cast away constness but constness is preserved/gaurded by the
     *  SecureLinkBeforeMutation() mechanism so it's not as bad as it looks :)
     */
    _bsize = bufferSize;
    m_dataSize = bufferSize;
    _buffer = static_cast< Int8* >( const_cast< void* >( externalBuffer ) );
    m_linked = true;
}

/*-------------------------------------------------------------------------*/

bool
CDynamicBuffer::IsLinked( void ) const
{GUCEF_TRACE;
    
    return m_linked;
}

/*-------------------------------------------------------------------------*/

void
CDynamicBuffer::SecureLinkBeforeMutation( void )
{GUCEF_TRACE;

    if ( m_linked )
    {
        /*  Buffer mutation requested but the buffer is linked to an
         *  external resource for which this object does not have ownership.
         *  We will copy the data of the external buffer and make it private.
         *  The makes the mutation a safe operation.
         *
         *  First we copy our link locally
         */
        Int8* externalBuffer = _buffer;
        UInt32 extBufferSize = _bsize;
        UInt32 dataSize = m_dataSize;
        const bool autoEnlarge = _autoenlarge;
        
        /*
         *  Now we unlink
         */
        Clear( false );
        m_linked = false; 
        
        /*
         *  And then we simply copy from the external buffer
         *  After the copy it is safe to mutate the buffer because we have ownership
         */
        _autoenlarge = true; 
        CopyFrom( extBufferSize  ,
                  externalBuffer );
        _autoenlarge = autoEnlarge;
        m_dataSize = dataSize;
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

 