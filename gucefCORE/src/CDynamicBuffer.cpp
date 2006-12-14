/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
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

#include <stdlib.h>             /* needed here for realloc() */
#include <string.h>             /* needed for memcpy() */

#ifndef GUCEF_CORE_CIOACCESS_H
#include "CIOAccess.h"
#define GUCEF_CORE_CIOACCESS_H
#endif /* GUCEF_CORE_CIOACCESS_H ? */

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

CDynamicBuffer::CDynamicBuffer( bool autoenlarge ) 
        : _buffer( NULL )             , 
          _bsize( 0 )                 ,
          _autoenlarge( autoenlarge ) ,
          m_dataSize( 0 )             ,
          m_linked( false )
{TRACE;

}

/*-------------------------------------------------------------------------*/

CDynamicBuffer::CDynamicBuffer( CIOAccess& ioAccess )
        : _buffer( NULL )             , 
          _bsize( 0 )                 ,
          _autoenlarge( true )        ,
          m_dataSize( 0 )             ,
          m_linked( false )
{TRACE;
    
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
{TRACE;

    _buffer = (Int8*) malloc( initialsize );
    _bsize = initialsize;               
}

/*-------------------------------------------------------------------------*/

/**
 *      Copy constructor, creates a dynamic buffer that is an exact copy
 *      of the given src.
 */
CDynamicBuffer::CDynamicBuffer( const CDynamicBuffer &src )
        : _buffer( NULL )                  ,
          _bsize( 0 )                      ,
          m_dataSize( 0 )                  ,
          _autoenlarge( src._autoenlarge ) ,
          m_linked( false )
{TRACE;

    _buffer = (Int8*) malloc( src._bsize );
    _bsize = src._bsize;
    m_dataSize = src.m_dataSize;
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
{TRACE;

    CopyFrom( bufferSize     ,
              externalBuffer );
    _autoenlarge = autoenlarge;
}

/*-------------------------------------------------------------------------*/

/**
 *      Destructor, de-alloctes the buffer.
 */
CDynamicBuffer::~CDynamicBuffer()
{TRACE;
        
        Clear( false );
}

/*-------------------------------------------------------------------------*/

/**
 *      Returns the unsigned byte located at the index given.
 */
UInt8&
CDynamicBuffer::GetUByte( UInt32 index )
{TRACE;

    SecureLinkBeforeMutation();
    
    if ( index+1 > m_dataSize )
    {
            m_dataSize = index+1;
    }
    return *((UInt8*)_buffer+index);
}

/*-------------------------------------------------------------------------*/

/**
 *      Sets the unsigned byte located at the index given.
 */
void
CDynamicBuffer::SetUByte( UInt32 index ,
                          UInt8 data   )
{TRACE;
        
    SecureLinkBeforeMutation();
    
    if ( index+1 > m_dataSize )
    {
        m_dataSize = index+1;
    }
    _buffer[ index ] = (char)data;
}

/*-------------------------------------------------------------------------*/

/**
 *      Returns the signed byte located at the index given.
 */
char&
CDynamicBuffer::GetChar( UInt32 index )
{TRACE;

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
{TRACE;

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
{TRACE;
        SecureLinkBeforeMutation();
        
        if ( index+1 > m_dataSize )
        {
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
{TRACE;
        return _buffer[ index ];
}

/*-------------------------------------------------------------------------*/

bool
CDynamicBuffer::operator==( const CDynamicBuffer& other ) const
{TRACE;
        if ( m_dataSize == other.m_dataSize )
        {
                return memcmp( _buffer, other._buffer, m_dataSize ) == 0;
        }       
        return false;
}

/*-------------------------------------------------------------------------*/

bool
CDynamicBuffer::operator!=( const CDynamicBuffer& other ) const
{TRACE;

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
{TRACE;

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
{TRACE;

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
{TRACE;
        return _bsize;
}

/*-------------------------------------------------------------------------*/

void 
CDynamicBuffer::SetAutoEnlarge( bool autoenlarge )
{TRACE;
        _autoenlarge = autoenlarge;
}

/*-------------------------------------------------------------------------*/
        
bool 
CDynamicBuffer::GetAutoEnlarge( void ) const
{TRACE;
        return _autoenlarge;
}

/*-------------------------------------------------------------------------*/

/**
 *      Copys size number of bytes from src to the buffer at the offset
 *      given.
 */
void
CDynamicBuffer::CopyFrom( UInt32 offset   ,
                          UInt32 size     ,
                          const void* src )
{TRACE;

    SecureLinkBeforeMutation();

    if ( _bsize < offset+size )         
    {
            if ( _autoenlarge )
            {
                    SetDataSize( offset+size );
            }
            else
            {       
                    Int32 max = (Int32)_bsize - (Int32)offset - (Int32)size;
                    if ( max < 0 ) 
                    {
                            return;
                    }                                
                    size = max;
            }                
    }                                                
    
    m_dataSize = size;
    memcpy( _buffer+offset, src, size );
}

/*-------------------------------------------------------------------------*/

/**
 *      Copys size number of bytes from src to the buffer
 */
void 
CDynamicBuffer::CopyFrom( UInt32 size     ,
                          const void* src )
{TRACE;
    
    SecureLinkBeforeMutation();
    
    if ( _bsize < size )         
    {
            if ( _autoenlarge )
            {
                    SetDataSize( size );
            }
            else
            {       
                    Int32 max = (Int32)_bsize - (Int32)size;
                    if ( max < 0 ) 
                    {
                            return;
                    }                                
                    size = max;
            }                
    }                                                
    
    m_dataSize = size;
    memcpy( _buffer, src, size );        
}                          

/*-------------------------------------------------------------------------*/

/**
 *      Copys size number of bytes from the buffer to src at the offset
 *      given.
 */
void
CDynamicBuffer::CopyTo( UInt32 offset ,
                        UInt32 size   ,
                        void *dest    ) const
{TRACE;

    if ( offset+size > m_dataSize )
    {
            Int32 max = (Int32)m_dataSize - (Int32)offset - (Int32)size;
            if ( max < 0 ) 
            {
                    return;
            }                        
            size = max;           
    }
    memcpy( dest, _buffer+offset, size );
}

/*-------------------------------------------------------------------------*/

/**
 *      Copys all data from the buffer to dest
 */
void 
CDynamicBuffer::CopyTo( void *dest ) const
{TRACE;

    memcpy( dest, _buffer, m_dataSize );
}

/*-------------------------------------------------------------------------*/

void 
CDynamicBuffer::CopyTo( CDynamicBuffer& dest ) const
{TRACE;

    dest.CopyFrom( m_dataSize, _buffer );        
}

/*-------------------------------------------------------------------------*/

void* 
CDynamicBuffer::GetBufferPtr( void )
{TRACE;
        SecureLinkBeforeMutation();
        
        return _buffer;
}

/*-------------------------------------------------------------------------*/
       
const void* 
CDynamicBuffer::GetConstBufferPtr( void ) const
{TRACE;
        return _buffer;
}

/*-------------------------------------------------------------------------*/

void 
CDynamicBuffer::Append( const void* data  ,
                        const UInt32 size )
{TRACE;

    SecureLinkBeforeMutation();

    SetDataSize( _bsize + size );
    memcpy( _buffer+_bsize, data, size );
    m_dataSize = _bsize;
}

/*-------------------------------------------------------------------------*/

void 
CDynamicBuffer::Clear( const bool logicalClearOnly /* = true */ )
{TRACE;

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
{TRACE;
        SecureLinkBeforeMutation();
        
        SetBufferSize( newSize, false );
        m_dataSize = newSize;        
}

/*-------------------------------------------------------------------------*/

UInt32
CDynamicBuffer::GetDataSize( void ) const
{TRACE;
        return m_dataSize;
}

/*-------------------------------------------------------------------------*/

void
CDynamicBuffer::LinkTo( const void* externalBuffer ,
                        UInt32 bufferSize          )
{TRACE;

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
{TRACE;
    
    return m_linked;
}

/*-------------------------------------------------------------------------*/

void
CDynamicBuffer::SecureLinkBeforeMutation( void )
{TRACE;

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

 