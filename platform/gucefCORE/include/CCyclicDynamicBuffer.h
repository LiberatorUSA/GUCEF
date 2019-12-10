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

#ifndef GUCEF_CORE_CCYCLICDYNAMICBUFFER_H
#define GUCEF_CORE_CCYCLICDYNAMICBUFFER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <list>

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"       /* module macro's */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

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

/**
 *  Cyclic buffer implementation.
 *  The read and write operations behave like a stream.
 *  Unused buffer space will be reused, if no unused space exists for storage
 *  of data the buffer will be automatically enlarged.
 */
class GUCEF_CORE_PUBLIC_CPP CCyclicDynamicBuffer
{
    public:

    CCyclicDynamicBuffer( const UInt32 initialBufferSize = 0 );

    virtual ~CCyclicDynamicBuffer();

    CCyclicDynamicBuffer( const CCyclicDynamicBuffer& src );

    CCyclicDynamicBuffer& operator=( const CCyclicDynamicBuffer& src );

    // Copies an entire block, resulting from an original write action,
    // into the given buffer. The target buffer will be overwritten and not appended.
    UInt32 ReadBlockTo( CDynamicBuffer& buffer );

    // Peeks an entire block into the given buffer. The target buffer will be overwritten and not appended.
    // Peeking does not remove the data and subsequent Read/Peek operations will get the same data
    UInt32 PeekBlock( CDynamicBuffer& buffer );

    UInt32 Read( void* destBuffer             ,
                 const UInt32 bytesPerElement ,
                 const UInt32 elementsToRead  );

    /**
     *  Same as Read() except that the data that is read remains available in the buffer after the Peek()
     *  whereas a Read will remove the data
     */
    UInt32 Peek( void* destBuffer             ,
                 const UInt32 bytesPerElement ,
                 const UInt32 elementsToRead  );

    /**
     *  Same as Peek() except that it is optimized for reading a single element.
     *  If you were going to call peak with 'elementsToRead' set to 1 then use this version instead.
     */
    UInt32 PeekElement( void* destBuffer         ,
                        const UInt32 elementSize );

    /**
     *  Attempts to skip the given number of bytes in the buffer as if they have been read but
     *  you do not have to provide a buffer to copy that data into. Typically used in combination with Peek()
     *  The number of bytes skipped is returned, it can be less if there isnt enough data available to skip.
     */
    UInt32 SkipRead( const UInt32 bytesToSkip );

    UInt32 Write( const void* srcBuffer        ,
                  const UInt32 bytesPerElement ,
                  const UInt32 elementsToWrite );

    /**
     *  Writes the entire logical content of the buffer into the cyclic buffer
     *  The total number of bytes written is returned
     */
    UInt32 Write( const CDynamicBuffer& buffer    ,
                  const UInt32 offsetInBuffer = 0 );

    /**
     *  Writes data at the front of the queue bypassing normal
     *  read-write FIFO making it for the purpose of this single write
     *  a FILO operation instead
     */
    UInt32 WriteAtFrontOfQueue( const void* srcBuffer        ,
                                const UInt32 bytesPerElement ,
                                const UInt32 elementsToWrite );

    void Clear( const bool logicalClearOnly = false );

    UInt32 GetBufferedDataSizeInBytes( void ) const;

    Float32 GetBufferUsagePercentage( void ) const;

    UInt32 GetTotalBufferSizeInBytes( void ) const;

    UInt32 GetNrOfFreeBlocks( void ) const;

    UInt32 GetNrOfUsedBlocks( void ) const;

    bool HasBufferedData( void ) const;

    protected:

    virtual void LockData( void ) const;

    virtual void UnlockData( void ) const;

    private:

    struct SDataChunk
    {
        UInt32 startOffset;
        UInt32 blockSize;
    };
    typedef struct SDataChunk TDataChunk;
    typedef std::list< TDataChunk > TBlockList;

    void TidyFreeBlocks( void );

    UInt32 WriteImp( const void* srcBuffer        ,
                     const UInt32 bytesPerElement ,
                     const UInt32 elementsToWrite ,
                     bool writeToBack             );

    UInt32 CopyBlocksToBuffer( const TBlockList& blockList ,
                               void* destBuffer            );

    UInt32 ReadElement( void* destBuffer          ,
                        const UInt32 elementSize  ,
                        bool freeBlocksOnSuccess  ,
                        TBlockList& elementBlocks );

    private:

    TBlockList m_usedBlocks;
    TBlockList m_freeBlocks;
    CDynamicBuffer m_buffer;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CCYCLICDYNAMICBUFFER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 20-02-2005 :
        - Dinand: Added this class, it is based on some older C implementation
          I made once. Ported but not tested.

-----------------------------------------------------------------------------*/
