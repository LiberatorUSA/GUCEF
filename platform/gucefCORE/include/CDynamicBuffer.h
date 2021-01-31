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

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#define GUCEF_CORE_CDYNAMICBUFFER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_EXCEPTIONCLASSMACROS_H
#include "ExceptionClassMacros.h"
#define GUCEF_CORE_EXCEPTIONCLASSMACROS_H
#endif /* GUCEF_CORE_EXCEPTIONCLASSMACROS_H ? */

#ifndef GUCEF_CORE_EXCEPTIONTHROWMACROS_H
#include "ExceptionThrowMacros.h"
#define GUCEF_CORE_EXCEPTIONTHROWMACROS_H
#endif /* GUCEF_CORE_EXCEPTIONTHROWMACROS_H ? */

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

class CIOAccess;

/**
 *      Class that implements a simple dynamic buffer. Simply an array of bytes.
 *      Member functions are provided to access the bytes as unsigned or signed
 *      bytes. This class helps prevent having to recode the same basic dynamic
 *      buffer code again and again.
 *      Using a regular class to perform this task allows you to keep buffer
 *      memory management in-scope.
 */
class GUCEF_CORE_PUBLIC_CPP CDynamicBuffer
{
        public:

        CDynamicBuffer( CIOAccess& ioAccess );

        CDynamicBuffer( void );

        explicit CDynamicBuffer( bool autoenlarge );

        CDynamicBuffer( const char* externalBuffer ,
                        const UInt32 bufferSize    ,
                        bool autoenlarge = true    );

        /**
         *      Initializes the buffer to the given size
         */
        CDynamicBuffer( UInt32 initialsize ,
                        bool autoenlarge   );

        /**
         *      Copy constructor, creates a dynamic buffer that is an exact copy
         *      of the given src.
         */
        CDynamicBuffer( const CDynamicBuffer &src    ,
                        bool shrinkToDataSize = true );

        /**
         *      Destructor, de-alloctes the buffer.
         */
        ~CDynamicBuffer();

        /**
         *  Returns the unsigned byte located at the index given.
         *  Note that if the buffer is linked this operation will result in the creation of a private copy
         */
        UInt8& GetUByte( UInt32 index );

        /**
         *      Sets the unsigned byte located at the index given.
         */
        void SetUByte( UInt32 index ,
                       UInt8 data   );

        /**
         *  Returns the signed byte located at the index given.
         *  Note that if the buffer is linked this operation will result in the creation of a private copy
         */
        char& GetChar( UInt32 index );

        /**
         *  Sets the signed byte located at the index given.
         *  Note that if the buffer is linked this operation will result in the creation of a private copy
         */
        void SetChar( UInt32 index ,
                      char data    );

        /**
         *  Returns the signed byte located at the index given.
         *  Note that if the buffer is linked this operation will result in the creation of a private copy
         */
        char& operator[]( UInt32 index );

        /**
         *      Returns the signed byte located at the index given.
         */
        char operator[]( UInt32 index ) const;

        /**
         *      Does a binary compare between this buffer and the given
         *      buffer.
         */
        bool operator==( const CDynamicBuffer& other ) const;

        /**
         *      Does a binary compare between this buffer and the given
         *      buffer.
         */
        bool operator!=( const CDynamicBuffer& other ) const;

        /**
         *      Does a binary compare between this buffer and the given
         *      buffer. Comparable to memcmp()
         */
        bool operator<( const CDynamicBuffer& other ) const;

        /**
         *      Does a binary compare between this buffer and the given
         *      buffer. Comparable to memcmp()
         */
        bool operator>( const CDynamicBuffer& other ) const;

        /**
         *      Turns the current dynamic buffer into a copy of the source
         *      dynamic buffer and returns the result.
         */
        CDynamicBuffer& operator=( const CDynamicBuffer &src );

        /**
         *      Sets the new actual buffer size.
         */
        bool SetBufferSize( const UInt32 newSize       ,
                            bool allowreduction = true );

        /**
         *      Returns the actual buffer size.
         */
        UInt32 GetBufferSize( void ) const;

        /**
         *      Sets the logical buffer size ( used buffer space )
         *      This is assumed to be useful data and the remainder simply extra buffer space
         */
        void SetDataSize( const UInt32 newSize );

        /**
         *      Returns the current used buffer space
         *      This is assumed to be useful data and the remainder simply extra buffer space
         */
        UInt32 GetDataSize( void ) const;

        void SetAutoEnlarge( bool autoenlarge );

        bool GetAutoEnlarge( void ) const;

        /**
         *  Special operation that allows you to link to data outside of the dynamic buffer
         *  Mainly used for optimization purposes & API compatibility reasons.
         *
         *  Automatically calls Clear( false ) for safety reasons before performing
         *  the link operation.
         *
         *  Note that if a dynamic buffer is in a linked state it's data cannot be modified
         *  directly. If you attempt to perform a mutation on the buffer (or simply call a member
         *  function that allows mutations to take place) the linked data will
         *  be copied into a private buffer which can then be modified.
         *  The same is true when you copy/assign the CDynamicBuffer, it will cause the linked
         *  data of the buffer to be copied and not linked.
         */
        void LinkTo( const void* externalBuffer ,
                     UInt32 bufferSize          );

        /**
         *  Returns the buffer linkage state
         *  See LinkTo() for more info
         */
        bool IsLinked( void ) const;

        /**
         *  Copies size number of bytes from src to the buffer at the offset given.
         *  Note that if the buffer is linked this operation will result in the creation of a private copy
         *
         *  "destinationOffset" is the offset in this buffer where you want the data to be copied to
         *  if the buffer is not large enough it will be enlarged if auto-enlarge is enabled.
         */
        UInt32 CopyFrom( UInt32 destinationOffset ,
                         UInt32 size              ,
                         const void* src          );

        /**
         *  Copies size number of bytes from src to the buffer
         *  Note that if the buffer is linked this operation will result in the creation of a private copy
         */
        UInt32 CopyFrom( UInt32 size     ,
                         const void* src );

        /**
         *  Copies size number of bytes from the buffer to src from the offset given.
         *
         *  @return number of bytes written
         */
        UInt32 CopyTo( UInt32 offset ,
                       UInt32 size   ,
                       void *dest    ) const;

        /**
         *      Copies all data from the buffer to dest
         */
        UInt32 CopyTo( void *dest ) const;

        UInt32 CopyTo( CDynamicBuffer& dest ) const;

        /**
         *  Appends the given data to the back of the data bytes in the buffer
         *  Note that if the buffer is linked this operation will result in the creation of a private copy
         */
        void Append( const void* data                      ,
                     const UInt32 size                     ,
                     const bool appendToLogicalData = true );

        /**
         *  Appends the given data to the back of the data bytes in the buffer
         *  Note that if the buffer is linked this operation will result in the creation of a private copy
         */
        void Append( const CDynamicBuffer& data            ,
                     const bool appendToLogicalData = true );

        /**
         *  Appends the given data to the back of the data bytes in the buffer
         *  Note that if the buffer is linked this operation will result in the creation of a private copy
         */
        void Append( CIOAccess& data                       ,
                     const bool appendToLogicalData = true );

        /**
         *  Attempts to find a matching block of bytes if any from the given offset and returns the offset
         *  at which the block was found. If no matching block is found -1 is returned.
         */
        Int32 Find( const void* data   ,
                    const UInt32 size  ,
                    UInt32 offset = 0  ) const;

        /**
         *  Down shifts the data in the buffer by "bytesToWipe" bytes and adjust the data size indicator accordingly
         *  Returns false if out of bounds.
         */
        bool Downshift( const UInt32 bytesToWipe );

        /**
         *  Trys to fill the dynamic buffer with the contents of the file given
         */
        bool LoadContentFromFile( const CString& filePath       ,
                                  const UInt32 offsetInFile = 0 ,
                                  const Int32 bytesToRead = -1  );

        /**
         *  Tries to write the dynamic buffer content to the file at the file path given
         */
        bool WriteContentToFile( const CORE::CString& filepath ,
                                 const CORE::UInt64 offset = 0 ,
                                 const bool overwrite = false  ) const;

        bool AppendContentToFile( const CORE::CString& filepath ) const;

        /**
         *  Provides mutable access to the buffer as a block of memory
         *  Note that if the buffer is linked this operation will result in the creation of a private copy
         */
        void* GetBufferPtr( const UInt32 offset = 0 );

        const void* GetConstBufferPtr( const UInt32 offset = 0 ) const;

        void Clear( const bool logicalClearOnly = true );

        /**
         *  Relinquishes ownership of any contained data to the caller
         *  the caller is henceforth responsible for deallocating the memory
         *  Note that if the buffer is linked this operation will result in the creation of a private copy
         */
        void RelinquishDataOwnership( void*& data, UInt32& dataSize );

        /**
         *  Utility member function:
         *  Template'd assignment operator for easy assignment of a unit of the given type.
         *  The raw data will be copied into the buffer.
         */
        template< typename T >
        CDynamicBuffer& operator=( const T& rawData );

        /**
         *  Utility member function:
         *  Performs a cast with size checking from the buffer content to the given type
         *  Note that if the buffer is linked this operation will result in the creation of a private copy
         *
         *  @throw EIllegalCast thrown when the cast cannot be performed with the data available in the buffer
         */
        template< typename T >
        T& AsType( const UInt32 byteOffset = 0 );

        /**
         *  Utility member function:
         *  Performs a cast with size checking from the buffer content to the given type
         *
         *  @throw EIllegalCast thrown when the cast cannot be performed with the data available in the buffer
         */
        template< typename T >
        const T& AsConstType( const UInt32 byteOffset = 0 ) const;

        /**
         *  Utility member function:
         *  Performs easy casting to the given type as a pointer at the given offset
         *
         *  @throw EIllegalCast thrown when the cast cannot be performed with the data available in the buffer
         */
        template< typename T >
        const T* AsConstTypePtr( const UInt32 byteOffset = 0 ) const;

        /**
         *  Utility member function:
         *  Performs easy casting to the given type as a pointer at the given offset
         *
         *  @throw EIllegalCast thrown when the cast cannot be performed with the data available in the buffer
         */
        template< typename T >
        T* AsTypePtr( const UInt32 byteOffset = 0 );

        /**
         *  Utility member function:
         *  Performs easy casting to the given type as a pointer at the given offset while also taking into account
         *  the minimal memory size available as pointed to by T specified as requiredSizeOfT
         *
         *  @throw EIllegalCast thrown when the cast cannot be performed with the data available in the buffer
         */
        template< typename T >
        const T* AsConstTypePtr( const UInt32 byteOffse, const UInt32 requiredSizeOfT ) const;

        GUCEF_DEFINE_MSGEXCEPTION( GUCEF_CORE_PUBLIC_CPP, EIllegalCast );

        private:

        /**
         *  Called when a mutation operation is about to be performed
         *  on the buffer. If the buffer is in a linked state the
         *  external buffer will first be copied, causing us to unlink,
         *  before proceeding with the mutation.
         */
        void SecureLinkBeforeMutation( void );

        private:

        bool m_linked;     /**< is the buffer only linked to data owned by someone else ? */
        bool _autoenlarge; /**< automatically enlarge buffer ? */
        Int8* _buffer;     /**< our byte buffer */
        UInt32 _bsize;     /**< current size of the buffer */
        UInt32 m_dataSize; /**< logical buffer size, extend of the buffer actually filled with data */
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template< typename T >
CDynamicBuffer&
CDynamicBuffer::operator=( const T& rawData )
{GUCEF_TRACE;

    CopyFrom( sizeof( T ), &rawData );
    return *this;
}

/*-------------------------------------------------------------------------*/

template< typename T >
T&
CDynamicBuffer::AsType( const UInt32 byteOffset /* = 0 */ )
{GUCEF_TRACE;

    if ( byteOffset + sizeof( T ) <= GetDataSize() )
    {
        return *reinterpret_cast< T* >( static_cast< const char* >( GetBufferPtr() ) + byteOffset );
    }

    GUCEF_EMSGTHROW( EIllegalCast, "GUCEF::CORE::CDynamicBuffer::AsType(): Cannot cast to the given type" );
}

/*-------------------------------------------------------------------------*/

template< typename T >
const T&
CDynamicBuffer::AsConstType( const UInt32 byteOffset /* = 0 */ ) const
{GUCEF_TRACE;

    if ( byteOffset + sizeof( T ) <= GetDataSize() )
    {
        return *reinterpret_cast< const T* >( static_cast< const char* >( GetConstBufferPtr() ) + byteOffset );
    }

    GUCEF_EMSGTHROW( EIllegalCast, "GUCEF::CORE::CDynamicBuffer::AsConstType(): Cannot cast to the given type" );
}

/*-------------------------------------------------------------------------*/

template< typename T >
const T*
CDynamicBuffer::AsConstTypePtr( const UInt32 byteOffset /* = 0 */ ) const
{GUCEF_TRACE;

    // We should have room for at least 1 element of T before the end of the buffer
    if ( byteOffset + sizeof( T ) <= GetDataSize() )
    {
        return reinterpret_cast< const T* >( static_cast< const char* >( GetConstBufferPtr() ) + byteOffset );
    }

    GUCEF_EMSGTHROW( EIllegalCast, "GUCEF::CORE::CDynamicBuffer::AsConstTypePtr(): Cannot cast to the given type" );
}

/*-------------------------------------------------------------------------*/

template< typename T >
T*
CDynamicBuffer::AsTypePtr( const UInt32 byteOffset /* = 0 */ )
{GUCEF_TRACE;

    // We should have room for at least 1 element of T before the end of the buffer
    if ( byteOffset + sizeof( T ) <= GetDataSize() )
    {
        return reinterpret_cast< T* >( static_cast< char* >( GetBufferPtr() ) + byteOffset );
    }

    GUCEF_EMSGTHROW( EIllegalCast, "GUCEF::CORE::CDynamicBuffer::AsTypePtr(): Cannot cast to the given type" );
}

/*-------------------------------------------------------------------------*/

template< typename T >
const T*
CDynamicBuffer::AsConstTypePtr( const UInt32 byteOffset, const UInt32 requiredSizeOfT ) const
{GUCEF_TRACE;

    // We should have room for at least 1 element of T before the end of the buffer
    if ( byteOffset + requiredSizeOfT <= GetDataSize() )
    {
        return reinterpret_cast< const T* >( static_cast< const char* >( GetConstBufferPtr() ) + byteOffset );
    }

    GUCEF_EMSGTHROW( EIllegalCast, "GUCEF::CORE::CDynamicBuffer::AsConstTypePtr(): Cannot cast to the given type" );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 14-12-2006 :
        - Added some conversion utilities for ease of use using templated
          member functions
- 06-10-2006 :
        - Added LinkTo()
- 13-03-2005 :
        - Updated to match new coding style
        - Added SetAutoEnlarge() and GetAutoEnlarge()
- 27-07-2004 :
        - Designed and implemented this class.

-----------------------------------------------------------------------------*/
