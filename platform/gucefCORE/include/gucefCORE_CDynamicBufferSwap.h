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

#ifndef GUCEF_CORE_CDYNAMICBUFFERSWAP_H
#define GUCEF_CORE_CDYNAMICBUFFERSWAP_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <vector>

#ifndef GUCEF_MT_CILOCKABLE_H
#include "gucefMT_CILockable.h"
#define GUCEF_MT_CILOCKABLE_H
#endif /* GUCEF_MT_CILOCKABLE_H ? */

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_CORE_CDATETIME_H
#include "gucefCORE_CDateTime.h"
#define GUCEF_CORE_CDATETIME_H
#endif /* GUCEF_CORE_CDATETIME_H ? */

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"   
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
 *  Class that implements a set of buffers that are switched in active
 *  ownership between a reader and a writer
 */
class GUCEF_CORE_PUBLIC_CPP CDynamicBufferSwap : public MT::CILockable
{
    public:

    CDynamicBufferSwap( UInt32 nrOfBuffers = 3 );

    virtual ~CDynamicBufferSwap();

    UInt32 GetNrOfBuffers( void ) const;
    
    void SetMinimalBufferSize( UInt32 minimalBufferSize );

    void GetBufferSizeRange( UInt32& smallest, UInt32& largest ) const;

    Int32 GetCurrentReaderBufferIndex( void ) const;

    Int32 GetCurrentWriterBufferIndex( void ) const;

    UInt32 GetBuffersQueuedToRead( void ) const;

    /**
     *  Releases the lock on the current read buffer (if any) and attempts to aquire a lock 
     *  on the next read buffer if there is a next buffer ready for reading
     *  If the lock could not be obtainned within the specified timeout period GUCEF_NULL will be returned
     */
    CDynamicBuffer* GetNextReaderBuffer( CDateTime& associatedDt                       ,
                                         bool alwaysBlockForBufferAvailability = false ,
                                         UInt32 lockWaitTimeoutInMs = 1000             );

    CDynamicBuffer* GetNextReaderBuffer( bool alwaysBlockForBufferAvailability = false ,
                                         UInt32 lockWaitTimeoutInMs = 1000             );

    CDynamicBuffer* GetCurrenReaderBuffer( CDateTime& associatedDt           ,
                                           UInt32 lockWaitTimeoutInMs = 1000 );
        
    CDateTime GetCurrenReaderBufferAssociatedDt( void ) const;
    
    /**
     *  Releases the lock on the current read buffer
     */
    bool SignalEndOfReading( void );

    /**
     *  Releases the lock on the current write buffer (if any) and attempts to aquire a lock 
     *  on the next write buffer. 
     *  If the lock could not be obtainned within the specified timeout period GUCEF_NULL will be returned
     */
    CDynamicBuffer* GetNextWriterBuffer( bool alwaysBlockForBufferAvailability ,
                                         UInt32 lockWaitTimeoutInMs            );

    CDynamicBuffer* GetNextWriterBuffer( const CDateTime& associatedDt         ,
                                         bool alwaysBlockForBufferAvailability ,
                                         UInt32 lockWaitTimeoutInMs            );

    /**
     *  Releases the lock on the current write buffer if any and resets the writer buffer index 
     */
    bool SignalEndOfWriting( void );

    virtual const MT::CILockable * AsLockable( void ) const GUCEF_VIRTUAL_OVERRIDE;

    protected:
    
    virtual bool Lock( UInt32 lockWaitTimeoutInMs = GUCEF_MT_DEFAULT_LOCK_TIMEOUT_IN_MS ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Unlock( void ) const GUCEF_VIRTUAL_OVERRIDE;

    private:

    CDynamicBufferSwap( const CDynamicBufferSwap& src ); /**< not implemented, not allowed */
    CDynamicBufferSwap& operator=( const CDynamicBufferSwap& src ); /**< not implemented, not allowed */

    class CBufferEntry
    {
        public:

        CDynamicBuffer buffer;
        MT::CMutex lock;
        bool hasUnreadData;
        CDateTime associatedDt;

        CBufferEntry( void );
        CBufferEntry( const CBufferEntry& src );
    };

    typedef std::vector< CBufferEntry > TBufferEntryVector;
    
    TBufferEntryVector m_buffers;
    Int32 m_currentReaderBufferIndex;
    Int32 m_currentWriterBufferIndex;
    UInt32 m_buffersQueuedToRead;
    MT::CMutex m_lock;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CDYNAMICBUFFERSWAP_H ? */
