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

#ifndef GUCEF_MT_CSCOPEMUTEX_H
#include "gucefMT_CScopeMutex.h"
#define GUCEF_MT_CSCOPEMUTEX_H
#endif /* GUCEF_MT_CSCOPEMUTEX_H ? */

#ifndef GUCEF_MT_DVMTOSWRAP_H
#include "gucefMT_dvmtoswrap.h"
#define GUCEF_MT_DVMTOSWRAP_H
#endif /* GUCEF_MT_DVMTOSWRAP_H ? */

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */

#include "gucefCORE_CDynamicBufferSwap.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CDynamicBufferSwap::CBufferEntry::CBufferEntry( void )
    : buffer()
    , lock()
    , hasUnreadData( false )
    , associatedDt()
{GUCEF_TRACE;
    
}

/*-------------------------------------------------------------------------*/

CDynamicBufferSwap::CBufferEntry::CBufferEntry( const CBufferEntry& src )
    : buffer( src.buffer )
    , lock()
    , hasUnreadData( src.hasUnreadData )
    , associatedDt( src.associatedDt )
{GUCEF_TRACE;
    
}

/*-------------------------------------------------------------------------*/

CDynamicBufferSwap::CDynamicBufferSwap( UInt32 nrOfBuffers )
    : MT::CILockable()
    , m_buffers()
    , m_currentReaderBufferIndex( -1 )
    , m_currentWriterBufferIndex( -1 )
    , m_buffersQueuedToRead( 0 )
    , m_lock()
{GUCEF_TRACE;

    if ( nrOfBuffers < 3 )
        nrOfBuffers = 3;
    m_buffers.resize( nrOfBuffers );
}

/*-------------------------------------------------------------------------*/

CDynamicBufferSwap::~CDynamicBufferSwap()
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    m_buffers.clear();
}

/*-------------------------------------------------------------------------*/

UInt32 
CDynamicBufferSwap::GetBuffersQueuedToRead( void ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return m_buffersQueuedToRead;
}             

/*-------------------------------------------------------------------------*/

void 
CDynamicBufferSwap::SetMinimalBufferSize( UInt32 minimalBufferSize )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    TBufferEntryVector::iterator i = m_buffers.begin();
    while ( i != m_buffers.end() )
    {
        (*i).buffer.SetBufferSize( minimalBufferSize, false );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

Int32
CDynamicBufferSwap::GetCurrentReaderBufferIndex( void ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return m_currentReaderBufferIndex;
}

/*-------------------------------------------------------------------------*/

Int32
CDynamicBufferSwap::GetCurrentWriterBufferIndex( void ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return m_currentWriterBufferIndex;
}

/*-------------------------------------------------------------------------*/

CDynamicBuffer* 
CDynamicBufferSwap::GetNextReaderBuffer( CDateTime& associatedDt               ,
                                         bool alwaysBlockForBufferAvailability ,
                                         UInt32 lockWaitTimeoutInMs            )
{GUCEF_TRACE;

    do
    {
        MT::CScopeMutex lock( m_lock );          

        if ( m_currentReaderBufferIndex < 0 )
        {
            CBufferEntry& firstBuffer = m_buffers[ 0 ];        
            if ( firstBuffer.hasUnreadData && firstBuffer.lock.Lock( lockWaitTimeoutInMs ) )
            {
                associatedDt = firstBuffer.associatedDt;
                m_currentReaderBufferIndex = 0;
                return &firstBuffer.buffer;
            }
            else
            {
                if ( alwaysBlockForBufferAvailability )
                {
                    lock.EarlyUnlock();
                    MT::PrecisionDelay( 100 );
                    continue;
                }
                else
                    return GUCEF_NULL;
            }
        }
    
        Int32 nextReaderBufferIndex = m_currentReaderBufferIndex+1;
    
        // loop around as needed
        if ( nextReaderBufferIndex == m_buffers.size() )
            nextReaderBufferIndex = 0;

        // Cheap early out check
        // The reader and the writer should never be on the same index
        if ( nextReaderBufferIndex == m_currentWriterBufferIndex )
        {
            if ( alwaysBlockForBufferAvailability )
            {
                // Wait on the writer to finish a buffer
                lock.EarlyUnlock();
                MT::PrecisionDelay( 100 );
                continue;
            }
            else
                return GUCEF_NULL;
        }

        CBufferEntry& currentBuffer = m_buffers[ m_currentReaderBufferIndex ];
        CBufferEntry& nextBuffer = m_buffers[ nextReaderBufferIndex ];
    
        currentBuffer.hasUnreadData = false;
        currentBuffer.buffer.SetDataSize( 0 );

        if ( nextBuffer.hasUnreadData && nextBuffer.lock.Lock( lockWaitTimeoutInMs ) )
        {           
            currentBuffer.lock.Unlock();

            m_currentReaderBufferIndex = nextReaderBufferIndex;  
            --m_buffersQueuedToRead;
            associatedDt = nextBuffer.associatedDt;
            return &nextBuffer.buffer;
        }
        else
        {
            if ( alwaysBlockForBufferAvailability )
            {
                lock.EarlyUnlock();
                MT::PrecisionDelay( 100 );
                continue;
            }
            else
                return GUCEF_NULL;
        }
    }
    while ( alwaysBlockForBufferAvailability );
    return GUCEF_NULL;
}            

/*-------------------------------------------------------------------------*/

CDynamicBuffer* 
CDynamicBufferSwap::GetNextReaderBuffer( bool alwaysBlockForBufferAvailability ,
                                         UInt32 lockWaitTimeoutInMs            )
{GUCEF_TRACE;

    CDateTime associatedDt; 
    return GetNextReaderBuffer( associatedDt                     , 
                                alwaysBlockForBufferAvailability ,
                                lockWaitTimeoutInMs              );
}

/*-------------------------------------------------------------------------*/

bool 
CDynamicBufferSwap::SignalEndOfReading( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    if ( m_currentReaderBufferIndex >= 0 )
    {
        CBufferEntry& currentBuffer = m_buffers[ m_currentReaderBufferIndex ];
        currentBuffer.hasUnreadData = false;
        currentBuffer.buffer.SetDataSize( 0 );
        return currentBuffer.lock.Unlock();
    }
    return true;
}

/*-------------------------------------------------------------------------*/

CDynamicBuffer* 
CDynamicBufferSwap::GetNextWriterBuffer( const CDateTime& associatedDt         ,
                                         bool alwaysBlockForBufferAvailability ,
                                         UInt32 lockWaitTimeoutInMs            )
{GUCEF_TRACE;

    do
    {
        MT::CScopeMutex lock( m_lock );

        if ( m_currentWriterBufferIndex < 0 )
        {
            m_currentWriterBufferIndex = m_currentReaderBufferIndex + 1;
            CBufferEntry& firstBuffer = m_buffers[ m_currentWriterBufferIndex ];
        
            lock.EarlyUnlock();

            if ( firstBuffer.lock.Lock( lockWaitTimeoutInMs ) )
            {
                if ( &associatedDt != &CDateTime::Empty )
                    firstBuffer.associatedDt = associatedDt;
                else
                    firstBuffer.associatedDt = CDateTime::NowUTCDateTime();
                        
                return &firstBuffer.buffer;
            }
            else
            {
                if ( alwaysBlockForBufferAvailability )
                {
                    lock.EarlyUnlock();
                    MT::PrecisionDelay( 100 );
                    continue;
                }
                else
                    return GUCEF_NULL;
            }
        }
    
        Int32 nextWriterBufferIndex = m_currentWriterBufferIndex+1;
    
        // loop around as needed
        if ( nextWriterBufferIndex == m_buffers.size() )
            nextWriterBufferIndex = 0;

        // Cheap early out check
        // The reader and the writer should never be on the same index
        if ( nextWriterBufferIndex == m_currentReaderBufferIndex )
        {
            if ( alwaysBlockForBufferAvailability )
            {
                // Wait on the writer to finish a buffer
                lock.EarlyUnlock();
                MT::PrecisionDelay( 100 );
                continue;
            }
            else
                return GUCEF_NULL;
        }

        CBufferEntry& currentBuffer = m_buffers[ m_currentWriterBufferIndex ];
        CBufferEntry& nextBuffer = m_buffers[ nextWriterBufferIndex ];

        if ( nextBuffer.hasUnreadData )
        {
            // We need to wait for the reader to keep up with the writer
            if ( alwaysBlockForBufferAvailability )
            {
                lock.EarlyUnlock();
                MT::PrecisionDelay( 100 );
                continue;
            }
            else
                return GUCEF_NULL;
        }
    
        if ( nextBuffer.lock.Lock( lockWaitTimeoutInMs ) )
        {   
            currentBuffer.hasUnreadData = true;
            currentBuffer.lock.Unlock();

            m_currentWriterBufferIndex = nextWriterBufferIndex;
            ++m_buffersQueuedToRead;

            if ( &associatedDt != &CDateTime::Empty )
                nextBuffer.associatedDt = associatedDt;
            else
                nextBuffer.associatedDt = CDateTime::NowUTCDateTime();

            return &nextBuffer.buffer;
        }
        else
        {
            if ( alwaysBlockForBufferAvailability )
            {
                lock.EarlyUnlock();
                MT::PrecisionDelay( 100 );
                continue;
            }
            else
                return GUCEF_NULL;
        }
    }
    while ( alwaysBlockForBufferAvailability );
    return GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

CDynamicBuffer* 
CDynamicBufferSwap::GetNextWriterBuffer( bool alwaysBlockForBufferAvailability ,
                                         UInt32 lockWaitTimeoutInMs            )
{GUCEF_TRACE;

    return GetNextWriterBuffer( CDateTime::Empty                 ,
                                alwaysBlockForBufferAvailability ,
                                lockWaitTimeoutInMs              );
}

/*-------------------------------------------------------------------------*/

bool 
CDynamicBufferSwap::SignalEndOfWriting( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    if ( m_currentWriterBufferIndex >= 0 )
    {
        CBufferEntry& currentBuffer = m_buffers[ m_currentWriterBufferIndex ];
        currentBuffer.hasUnreadData = true;
        return currentBuffer.lock.Unlock();
    }
    return true;
}

/*-------------------------------------------------------------------------*/

const MT::CILockable* 
CDynamicBufferSwap::AsLockable(void) const
{GUCEF_TRACE;

    return this;
}

/*-------------------------------------------------------------------------*/

bool 
CDynamicBufferSwap::Lock( UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return m_lock.Lock( lockWaitTimeoutInMs );
}

/*-------------------------------------------------------------------------*/

bool 
CDynamicBufferSwap::Unlock( void ) const
{GUCEF_TRACE;

    return m_lock.Unlock();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
