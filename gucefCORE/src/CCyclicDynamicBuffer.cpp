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

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#include "CCyclicDynamicBuffer.h"

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

CCyclicDynamicBuffer::CCyclicDynamicBuffer( const UInt32 initialBufferSize /* = 0 */ )
    : m_buffer( initialBufferSize, true ) ,
      m_freeBlocks()                      ,
      m_usedBlocks()
{GUCEF_TRACE;
    
    if ( initialBufferSize > 0 )
    {
        TDataChunk freeBlock;
        freeBlock.blockSize = initialBufferSize;
        freeBlock.startOffset = 0;
        m_freeBlocks.push_back( freeBlock );
    }
}

/*-------------------------------------------------------------------------*/
    
CCyclicDynamicBuffer::~CCyclicDynamicBuffer()
{GUCEF_TRACE;

    Clear();
}

/*-------------------------------------------------------------------------*/
    
CCyclicDynamicBuffer::CCyclicDynamicBuffer( const CCyclicDynamicBuffer& src )
    : m_buffer( src.m_buffer )         ,
      m_freeBlocks( src.m_freeBlocks ) ,
      m_usedBlocks( src.m_usedBlocks )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CCyclicDynamicBuffer&
CCyclicDynamicBuffer::operator=( const CCyclicDynamicBuffer& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        Clear();
        
        src.LockData();
        LockData();
        
        m_buffer = src.m_buffer;
        m_usedBlocks = src.m_usedBlocks;
        m_freeBlocks = src.m_freeBlocks;
        
        UnlockData();
        src.UnlockData();
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

UInt32
CCyclicDynamicBuffer::ReadBlockTo( CDynamicBuffer& buffer )
{GUCEF_TRACE;
    
    UInt32 bytesRead = 0;
    
    LockData();
    
    // We will simply pop a FIFO element
    TBlockList::iterator i = m_usedBlocks.begin();
    if ( i != m_usedBlocks.end() )
    {        
        TDataChunk& dataChunck = (*i);
        
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CCyclicDynamicBuffer: Reading an entire block of size " + UInt32ToString( dataChunck.blockSize ) + "bytes out of the buffer" );
        
        // Ensure that the receiving buffer is large enough to handle the block
        buffer.SetBufferSize( dataChunck.blockSize, false );
        
        // Copy the entire element into the given buffer and set the data size
        bytesRead = m_buffer.CopyTo( dataChunck.startOffset ,
                                     dataChunck.blockSize   ,
                                     buffer.GetBufferPtr()  );
        buffer.SetDataSize( bytesRead );
        
        // Mark the element as free'd
        TDataChunk freeDataChunck = dataChunck;
        m_freeBlocks.push_back( freeDataChunck );
        m_usedBlocks.erase( i );
    }
    
    TidyFreeBlocks();
    
    UnlockData();
    
    return bytesRead;
}

/*-------------------------------------------------------------------------*/

UInt32
CCyclicDynamicBuffer::Read( void* destBuffer             ,
                            const UInt32 bytesPerElement ,
                            const UInt32 elementsToRead  )
{GUCEF_TRACE;

    LockData();
    
    UInt32 bytesRead = 0, totalBytesRead = 0, totalBytes = bytesPerElement * elementsToRead;
    
    // We will simply pop FIFO elements until we hit the byte count
    for ( TBlockList::iterator i = m_usedBlocks.begin(); i != m_usedBlocks.end(); ++i )
    {
        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CCyclicDynamicBuffer: Reading data out of the buffer" );
        
        TDataChunk& dataChunck = (*i);
        if ( totalBytes > dataChunck.blockSize )
        {
            bytesRead = m_buffer.CopyTo( dataChunck.startOffset ,
                                         dataChunck.blockSize   ,
                                         destBuffer             );
            totalBytesRead += bytesRead;
            totalBytes -= bytesRead;             

            m_freeBlocks.push_back( dataChunck );            
            m_usedBlocks.erase( i );
            i = m_usedBlocks.begin();
        }
        else
        {
            bytesRead = m_buffer.CopyTo( dataChunck.startOffset ,
                                         totalBytes             ,
                                         destBuffer             );
            totalBytesRead += bytesRead;
            totalBytes -= bytesRead;
            
            TDataChunk freeDataChunck;
            freeDataChunck.blockSize = bytesRead;
            freeDataChunck.startOffset = (*i).startOffset;
            m_freeBlocks.push_back( freeDataChunck );            
            
            dataChunck.blockSize -= bytesRead;            
            dataChunck.startOffset += bytesRead;
        }
    }
    
    TidyFreeBlocks();
    
    UnlockData();
    
    return totalBytesRead;
}

/*-------------------------------------------------------------------------*/
    
UInt32
CCyclicDynamicBuffer::Write( const void* srcBuffer        ,
                             const UInt32 bytesPerElement ,
                             const UInt32 elementsToWrite )
{GUCEF_TRACE;

    // Initial sanity check
    if ( bytesPerElement == 0 || elementsToWrite == 0 ) return 0;
    
    LockData();
    
    UInt32 totalBytesWritten = 0;
    UInt32 totalBytes = bytesPerElement * elementsToWrite;
    UInt32 remainingElements = elementsToWrite;
    
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CCyclicDynamicBuffer: Writing " + UInt32ToString( totalBytes ) + " bytes of data into the buffer" );
    
    for ( TBlockList::iterator i = m_freeBlocks.begin(); i != m_freeBlocks.end(); ++i )
    {            
        if ( (*i).blockSize >= bytesPerElement )
        {
            // Calculate how many elements and thus bytes we can fit in the block
            UInt32 bytesToWrite, elementsToWrite;
            UInt32 nrOfElementsThatFitInBlock = (*i).blockSize / bytesPerElement;
            if ( nrOfElementsThatFitInBlock >= remainingElements )
            {
                elementsToWrite = remainingElements;
                bytesToWrite = bytesPerElement * remainingElements;
            }
            else
            {
                elementsToWrite = remainingElements - nrOfElementsThatFitInBlock;
                bytesToWrite = bytesPerElement * elementsToWrite;
            }
            
            // Copy the max number of elements
            UInt32 bytesWritten = m_buffer.CopyFrom( (*i).startOffset                       ,
                                                     bytesToWrite                           ,
                                                     ((Int8*)srcBuffer) + totalBytesWritten );
            totalBytesWritten += bytesToWrite;
            remainingElements -= elementsToWrite;
            
            // Sanity check
            if ( bytesWritten < bytesToWrite )
            {
                // something went wrong,.. intentionally do not update administration
                // as if the write to buffer never happened
                
                UnlockData();                
                return totalBytesWritten;
            }
                    
            // push the used-block-info on our FIFO stack
            TDataChunk usedDataChunck;
            usedDataChunck.blockSize = bytesWritten;
            usedDataChunck.startOffset = (*i).startOffset;
            m_usedBlocks.push_back( usedDataChunck );
            
            // Check if we have a block remnant in the now (partially?) used free block
            if ( (*i).blockSize > bytesWritten )
            {
                // store the free-block-info
                (*i).blockSize -= bytesWritten;
                (*i).startOffset += bytesWritten;
            }
            else
            {
                // remove the block from our list of available blocks
                m_freeBlocks.erase( i );
                i = m_freeBlocks.begin();
            }
            
            // Check if we should stop this free block matching excercise
            if ( 0 == remainingElements || i == m_freeBlocks.end() )
            {
                break;
            }
        }
    }
    
    // Check if we where able to store the given data using free blocks 
    // in the existing buffer space
    if ( remainingElements > 0 )
    {
        // We where unable to store everything using the available free blocks
        // We will create a new used block and enlarge the buffer if needed to
        // store the excess data
        UInt32 bytesToWrite = totalBytes - totalBytesWritten;
        UInt32 startOffset = m_buffer.GetDataSize();
        m_buffer.Append( ((Int8*)srcBuffer) + totalBytesWritten ,
                         bytesToWrite                           ,
                         true                                   );
                         
        // push the used-block-info on our FIFO stack
        TDataChunk usedDataChunck;
        usedDataChunck.blockSize = bytesToWrite;
        usedDataChunck.startOffset = startOffset;
        m_usedBlocks.push_back( usedDataChunck );
        
        // We have now written everything into the buffer
        totalBytesWritten = totalBytes;
    }
    
    TidyFreeBlocks();
    
    UnlockData(); 
    
    return totalBytesWritten;
}

/*-------------------------------------------------------------------------*/

UInt32
CCyclicDynamicBuffer::GetNrOfUsedBlocks( void ) const
{GUCEF_TRACE;

    LockData();    
    UInt32 nrOfUsedBlocks = (UInt32) m_usedBlocks.size();    
    UnlockData();
    return nrOfUsedBlocks;
}

/*-------------------------------------------------------------------------*/

UInt32
CCyclicDynamicBuffer::GetNrOfFreeBlocks( void ) const
{GUCEF_TRACE;

    LockData();    
    UInt32 nrOfFreeBlocks = (UInt32) m_freeBlocks.size();    
    UnlockData();
    return nrOfFreeBlocks;
}

/*-------------------------------------------------------------------------*/

void
CCyclicDynamicBuffer::TidyFreeBlocks( void )
{GUCEF_TRACE;

    if ( !m_freeBlocks.empty() )
    {        
        // first we (bubble-)sort the blocks based on their offset
        bool listMutation = true;
        while ( listMutation )
        {            
            listMutation = false;
            TBlockList::iterator i = m_freeBlocks.begin();
            UInt32 prevOffset = (*i).startOffset;
            for ( ++i;  i != m_freeBlocks.end(); ++i )
            {                
                if ( (*i).startOffset < prevOffset )
                {
                    TBlockList::iterator n = i;
                    --n;
                    TDataChunk chunck = (*n);
                    (*n) = (*i);
                    (*i) = chunck;
                    
                    listMutation = true;
                }
            }
        }

        // now we (bubble) merge blocks that are adjecent
        listMutation = true;
        while ( listMutation )
        {
            listMutation = false;
            
            TBlockList::iterator i = m_freeBlocks.begin();
            TBlockList::iterator n = i;
            ++i;
            
            TDataChunk chunck = (*n);
            for ( ; i != m_freeBlocks.end(); ++i )
            {
                if ( chunck.startOffset + chunck.blockSize == (*i).startOffset )
                {
                    // Perform a block merge
                    TDataChunk freeDataChunck;
                    freeDataChunck.blockSize = chunck.blockSize + (*i).blockSize;
                    freeDataChunck.startOffset = chunck.startOffset;
                    
                    m_freeBlocks.erase( i );
                    m_freeBlocks.erase( n );
                    m_freeBlocks.push_back( freeDataChunck );
                    
                    listMutation = true;
                    break;
                }
                else
                {
                    ++n;
                }
            }
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CCyclicDynamicBuffer::Clear( const bool logicalClearOnly /* = false */ )
{GUCEF_TRACE;

    LockData();
    
    GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CCyclicDynamicBuffer: Clearing the buffer" );
    
    m_usedBlocks.clear();
    m_freeBlocks.clear();
    m_buffer.Clear( logicalClearOnly );
    
    UnlockData();
}

/*-------------------------------------------------------------------------*/

bool
CCyclicDynamicBuffer::HasBufferedData( void ) const
{GUCEF_TRACE;

    return !m_usedBlocks.empty();
}

/*-------------------------------------------------------------------------*/

UInt32
CCyclicDynamicBuffer::GetBufferedDataSizeInBytes( void ) const
{GUCEF_TRACE;

    LockData();
    
    UInt32 totalBytes = 0;
    for ( TBlockList::const_iterator i = m_usedBlocks.begin(); i != m_usedBlocks.end(); ++i )
    {
        totalBytes += (*i).blockSize;
    }    
    
    UnlockData();
    
    return totalBytes;
}

/*-------------------------------------------------------------------------*/

UInt32
CCyclicDynamicBuffer::GetTotalBufferSizeInBytes( void ) const
{GUCEF_TRACE;

    LockData();
    
    UInt32 totalBufferSize = m_buffer.GetBufferSize();
    
    UnlockData();
    
    return totalBufferSize;
}

/*-------------------------------------------------------------------------*/

Float32
CCyclicDynamicBuffer::GetBufferUsagePercentage( void ) const
{GUCEF_TRACE;
    
    LockData();
    
    Float32 usagePerc = GetBufferedDataSizeInBytes() / ( 1.0f * m_buffer.GetBufferSize() );
    
    UnlockData();
    
    return usagePerc;
}

/*-------------------------------------------------------------------------*/
    
void
CCyclicDynamicBuffer::LockData( void ) const
{GUCEF_TRACE;

    // non-operation, implement in descending class for threadsafety if desired
}

/*-------------------------------------------------------------------------*/
    
void
CCyclicDynamicBuffer::UnlockData( void ) const
{GUCEF_TRACE;

    // non-operation, implement in descending class for threadsafety if desired
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
