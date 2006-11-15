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

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#define GUCEF_CORE_CDYNAMICBUFFER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"       /* config & macros specific to GUCEF */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_ETYPES_H
#include "ETypes.h"             /* simple types used */
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

class CIOAccess;

/**
 *      Class that implements a simple dynamic buffer. Simply an array of bytes.
 *      Member functions are provided to access the bytes as unsigned or signed
 *      bytes. This class helps prevent having to recode the same basic dynamic
 *      buffer code again and again. 
 *      Using a regular class to perform this task allows you to keep buffer 
 *      memory management in-scope.
 */
class GUCEFCORE_EXPORT_CPP CDynamicBuffer
{
        public:
        
        CDynamicBuffer( CIOAccess& ioAccess );
        
        /**
         *      Default constructor, initializes to a zero-length buffer.
         */
        CDynamicBuffer( bool autoenlarge = true );
        
        CDynamicBuffer( const char* externalBuffer ,
                        const UInt32 bufferSize    ,
                        bool autoenlarge = true    );
        
        /**
         *      Initializes the buffer to the given size
         */
        CDynamicBuffer( UInt32 initialsize      ,
                        bool autoenlarge = true );

        /**
         *      Copy constructor, creates a dynamic buffer that is an exact copy
         *      of the given src.
         */
        CDynamicBuffer( const CDynamicBuffer &src );

        /**
         *      Destructor, de-alloctes the buffer.
         */
        ~CDynamicBuffer();        

        /**
         *      Returns the unsigned byte located at the index given.
         */
        UInt8& GetUByte( UInt32 index );

        /**
         *      Sets the unsigned byte located at the index given.
         */
        void SetUByte( UInt32 index ,
                       UInt8 data   );

        /**
         *      Returns the signed byte located at the index given.
         */                        
        char& GetChar( UInt32 index );

        /**
         *      Sets the signed byte located at the index given.
         */
        void SetChar( UInt32 index ,
                      char data    );

        /**
         *      Returns the signed byte located at the index given.
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
         *      Turns the current dynamic buffer into a copy of the source
         *      dynamic buffer and returns the result.
         */            
        CDynamicBuffer& operator=( const CDynamicBuffer &src );

        /**
         *      Sets the new buffer size.
         */
        void SetBufferSize( const UInt32 newSize       ,
                            bool allowreduction = true );       
        
        /**
         *      Returns the current buffer size.
         */
        UInt32 GetBufferSize( void ) const;

        /**
         *      Sets the new buffer size.
         */
        void SetDataSize( const UInt32 newSize );       
        
        /**
         *      Returns the current used buffer space
         *      This is assumed to be usefull data and the remainder simply extra buffer space
         */
        UInt32 GetDataSize( void ) const;

        void SetAutoEnlarge( bool autoenlarge );
        
        bool GetAutoEnlarge( void ) const;
        
        /**
         *  Special operation that allows you to link to data outside of the dynamic buffer
         *  Mainly used for optimization purposes & API compatibility reasons.
         *
         *  Automaticly calls Clear( false ) for safety reasons before performing 
         *  the link operation.
         *
         *  Note that if a dynamic buffer is in a linked state it's data cannot be modified
         *  directly. If you attempt to perform a mutation on the buffer the linked data will
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
         *      Copys size number of bytes from src to the buffer at the offset
         *      given.
         */
        void CopyFrom( UInt32 offset   ,
                       UInt32 size     ,
                       const void* src );
                       
        /**
         *      Copys size number of bytes from src to the buffer
         */
        void CopyFrom( UInt32 size     ,
                       const void* src );                       

        /**
         *      Copys size number of bytes from the buffer to src from the offset
         *      given.
         */
        void CopyTo( UInt32 offset ,
                     UInt32 size   ,
                     void *dest    ) const;
                     
        /**
         *      Copys all data from the buffer to dest
         */
        void CopyTo( void *dest ) const;
        
        void CopyTo( CDynamicBuffer& dest ) const;
                     
        void Append( const void* data  ,
                     const UInt32 size );
                     
        void* GetBufferPtr( void );
        
        const void* GetBufferPtr( void ) const;
        
        void Clear( const bool logicalClearOnly = true );
        
        private:
        
        /**
         *  Called when a mutation operation is about to be performed
         *  on the buffer. If the buffer is in a linked state the
         *  external buffer will first be copied, causing us to unlink, 
         *  before proceding with the mutation.
         */
        void SecureLinkBeforeMutation( void );

        private:
        
        bool m_linked;     /**< is the buffer only linked to data owned by someone else ? */
        bool _autoenlarge; /**< automaticly enlarge buffer ? */
        char* _buffer;     /**< our byte buffer */
        UInt32 _bsize;     /**< current size of the buffer */
        UInt32 m_dataSize; /**< logical buffer size, extend of the buffer actually filled with data */        
};

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

- 06-10-2006 :
        - Added LinkTo()
- 13-03-2005 :
        - Updated to match new coding style
        - Added SetAutoEnlarge() and GetAutoEnlarge()
- 27-07-2004 :
        - Designed and implemented this class.

-----------------------------------------------------------------------------*/
