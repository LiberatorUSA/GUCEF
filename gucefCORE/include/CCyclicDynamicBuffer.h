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

#include <vector>

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
class GUCEFCORE_EXPORT_CPP CCyclicDynamicBuffer
{
    public:
    
    CCyclicDynamicBuffer( const UInt32 initialBufferSize = 0 );
    
    virtual ~CCyclicDynamicBuffer();
    
    CCyclicDynamicBuffer( const CCyclicDynamicBuffer& src );
    
    CCyclicDynamicBuffer& operator=( const CCyclicDynamicBuffer& src );
    
    UInt32 Read( void* destBuffer             ,
                 const UInt32 bytesPerElement ,
                 const UInt32 elementsToRead  );
    
    UInt32 Write( void* srcBuffer              ,
                  const UInt32 bytesPerElement ,
                  const UInt32 elementsToWrite );

    void Clear( const bool logicalClearOnly = false );
    
    UInt32 GetBufferedDataSizeInBytes( void ) const;
    
    Float32 GetBufferUsagePercentage( void ) const;
    
    protected:
    
    virtual void LockData( void ) const;
    
    virtual void UnlockData( void ) const;

    private:
    
    void TidyFreeBlocks( void );
    
    private:
    
    struct SDataChunk
    {   
        UInt32 startOffset;
        UInt32 blockSize;
    };
    typedef struct SDataChunk TDataChunk;
    typedef std::vector< TDataChunk > TBlockList;
    
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