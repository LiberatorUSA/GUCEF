/*
 *  gucefMT: GUCEF module providing multithreading solutions
 *
 *  Copyright (C) 1998 - 2020.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_DVMTOSWRAP_H
#include "gucefMT_dvmtoswrap.h"         /* OS wrappers for threading functionality */
#define GUCEF_MT_DVMTOSWRAP_H
#endif /* GUCEF_MT_DVMTOSWRAP_H ? */

#ifndef GUCEF_MT_COBJECTSCOPELOCK_H
#include "gucefMT_CObjectScopeLock.h"
#define GUCEF_MT_COBJECTSCOPELOCK_H
#endif /* GUCEF_MT_COBJECTSCOPELOCK_H ? */

#include "gucefMT_CActiveObject.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace MT {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CActiveObject::CActiveObject( void )
    : MT::CILockable()
    , m_threadData( NULL )                    
    , m_delayInMilliSecs( 25 )             
    , m_isDeactivationRequested( false )   
    , _suspend( false )                    
    , _active( false )                     
    , _td( ThreadDataReserve() )                          
    , m_minimalCycleDeltaInMilliSecs( 10 )
    , m_datalock()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CActiveObject::CActiveObject( const CActiveObject& src )
    : MT::CILockable()
    , m_threadData( src.m_threadData )         
    , m_delayInMilliSecs( src.m_delayInMilliSecs )       
    , m_isDeactivationRequested( false ) 
    , _suspend( false )                  
    , _active( false )                      
    , _td( ThreadDataReserve() )                           
    , m_minimalCycleDeltaInMilliSecs( 10 )
    , m_datalock()
{GUCEF_TRACE;

    if ( src.IsActive() )
    {
        Activate();
    }
}

/*-------------------------------------------------------------------------*/

CActiveObject::~CActiveObject()
{GUCEF_TRACE;

    Deactivate( true, true );
    
    ThreadDataCleanup( _td );
    _td = GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

bool
CActiveObject::IsDeactivationRequested( void ) const
{GUCEF_TRACE;

    return m_isDeactivationRequested;
}

/*-------------------------------------------------------------------------*/

bool
CActiveObject::IsActive( void ) const
{GUCEF_TRACE;

    return _active;
}

/*-------------------------------------------------------------------------*/

UInt32
CActiveObject::OnActivate( void* thisobject )
{GUCEF_TRACE;

    CActiveObject* tao = (CActiveObject*) thisobject;
    void* taskdata = tao->GetThreadData();

    if ( tao->OnThreadStart( taskdata ) )
    {
        tao->OnThreadStarted( taskdata );
       
        Float64 ticksPerMs = PrecisionTimerResolution() / 1000.0;
        UInt64 tickCount = PrecisionTickCount();
        UInt64 newTickCount = tickCount;
        Float64 timeDeltaInMs = 0;

        bool taskfinished = false;
        while ( !taskfinished && !tao->m_isDeactivationRequested )
        {
            // Check if the order has been given to suspend the thread
            if ( tao->_suspend )
            {
                ThreadSuspend( tao->_td );
            }

            // We can do a cycle
            taskfinished = tao->OnThreadCycle( taskdata );

            // check if we are finished
            if ( !taskfinished )
            {
                // If we are going to do another cycle then make sure we
                // stay within the time slice range requested.
                // Here we calculate the time that has passed in seconds
                newTickCount = PrecisionTickCount();
                timeDeltaInMs = ( newTickCount - tickCount ) / ticksPerMs;

                if ( timeDeltaInMs < tao->m_minimalCycleDeltaInMilliSecs )
                {
                    PrecisionDelay( tao->m_delayInMilliSecs );
                    tickCount = PrecisionTickCount();
                }
                else
                {
                    tickCount = newTickCount;
                }
            }
        }

        tao->OnThreadEnded( taskdata, false );
    }
    tao->_td = NULL;
    tao->_active = false;
    return 1;
}

/*-------------------------------------------------------------------------*/

bool
CActiveObject::Activate( void* threadData /* = NULL */                 ,
                         const UInt32 cycleDelay /* = 25 */            ,
                         const UInt32 minimalCycleDeltaInMs /* = 10 */ )
{GUCEF_TRACE;

    CObjectScopeLock lock( this );    

    if ( _active ) 
        return false;

    m_threadData = threadData;
    m_delayInMilliSecs = cycleDelay;
    m_minimalCycleDeltaInMilliSecs = minimalCycleDeltaInMs;
    m_isDeactivationRequested = false;
    _suspend = false;
    _active = true;
    
    bool createSuccesss = 0 != ThreadCreate( _td                      ,
                                             (TThreadFunc) OnActivate ,
                                             this                     );
    return createSuccesss;
}

/*-------------------------------------------------------------------------*/

void
CActiveObject::OnThreadPausedForcibly( void* taskdata )
{GUCEF_TRACE;

    // Dummy to avoid mandatory implementation in decending classes
}

/*-------------------------------------------------------------------------*/

void
CActiveObject::OnThreadResumed( void* taskdata )
{GUCEF_TRACE;

    // Dummy to avoid mandatory implementation in decending classes
}

/*-------------------------------------------------------------------------*/
    
void
CActiveObject::OnThreadEnded( void* taskdata ,
                              bool forced    )
{GUCEF_TRACE;

    // Dummy to avoid mandatory implementation in decending classes
}

/*-------------------------------------------------------------------------*/
    
void
CActiveObject::OnThreadEnding( void* taskdata    ,
                               bool willBeForced )
{GUCEF_TRACE;

    // Dummy to avoid mandatory implementation in decending classes
}

/*-------------------------------------------------------------------------*/
    
void
CActiveObject::OnThreadStarted( void* taskdata )
{GUCEF_TRACE;

    // Dummy to avoid mandatory implementation in decending classes
}

/*-------------------------------------------------------------------------*/

void
CActiveObject::Deactivate( bool force, bool callerShouldWait )
{GUCEF_TRACE;

    if ( force )
    {
        Lock();
        if ( _active )
        {
            // Lets give the thread 5 seconds to shut down
            // If it refuses we kill it
            m_isDeactivationRequested = true;
            OnThreadEnding( m_threadData, false );
            Unlock();

            ThreadWait( _td, 5000 );

            Lock();
            if ( _active )
            {
                ThreadSuspend( _td );
                OnThreadEnding( m_threadData, true );
            
                /*
                 *      Kill the thread :(
                 */
                ThreadKill( _td );
            }
            OnThreadEnded( m_threadData, true );
            Unlock();
        }
        else
        {
            Unlock();
        }
    }
    else
    {
        {
            CObjectScopeLock lock( this );
            if ( _active )
            {
                m_isDeactivationRequested = true;
                OnThreadEnding( m_threadData, false );
            }
        }
        if ( callerShouldWait )
        {
            ThreadWait( _td, 15000 );
        }
    }
}

/*-------------------------------------------------------------------------*/

bool
CActiveObject::IsPauseRequested( void ) const
{GUCEF_TRACE;

    return _suspend;
}

/*-------------------------------------------------------------------------*/

void
CActiveObject::Pause( bool force )
{GUCEF_TRACE;

    CObjectScopeLock lock( this );
    
    if ( force )
    {
        // Don't wait for the thread to cycle
        ThreadSuspend( _td );
        _active = false;
        OnThreadPausedForcibly( m_threadData );
    }
    else
    {
        _suspend = true;
    }
}

/*-------------------------------------------------------------------------*/

void
CActiveObject::Resume( void )
{GUCEF_TRACE;

    CObjectScopeLock lock( this );

    if ( GUCEF_NULL != _td && _suspend )
    {
        _active = true;
        ThreadResume( _td );
        OnThreadResumed( m_threadData );
    }
}

/*-------------------------------------------------------------------------*/

void*
CActiveObject::GetThreadData( void ) const
{GUCEF_TRACE;

    return m_threadData;
}

/*-------------------------------------------------------------------------*/

CActiveObject&
CActiveObject::operator=( const CActiveObject& src )
{GUCEF_TRACE;

    if ( this != &src )  
    {
        CObjectScopeLock lock( this );

        Deactivate( true, true );

        m_threadData = src.m_threadData;
        m_delayInMilliSecs = src.m_delayInMilliSecs;
        _suspend = false;

        if ( src.IsActive() )
        {
            Activate();
        }
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

UInt32
CActiveObject::GetThreadID( void ) const
{GUCEF_TRACE;

    CObjectScopeLock lock( this );

    if ( GUCEF_NULL != _td )
    {
        return ThreadID( _td );
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

bool 
CActiveObject::SetCpuAffinityMask( UInt32 affinityMaskSize ,
                                   void* affinityMask      )
{GUCEF_TRACE;

    CObjectScopeLock lock( this );

    if ( GUCEF_NULL != _td )
    {
        return ThreadSetCpuAffinity( _td, affinityMaskSize, affinityMask ) != 0;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CActiveObject::SetCpuAffinityByCpuId( UInt32 cpuId )
{GUCEF_TRACE;

    if ( cpuId >= 64 )
        return false;

    UInt64 cpuMask = 0;
    cpuMask |= (UInt64)1 << cpuId;

    return SetCpuAffinityMask( sizeof( cpuMask ), &cpuMask );
}

/*-------------------------------------------------------------------------*/

bool
CActiveObject::Lock( UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return m_datalock.Lock( lockWaitTimeoutInMs );
}

/*-------------------------------------------------------------------------*/

bool
CActiveObject::Unlock( void ) const
{GUCEF_TRACE;

    return m_datalock.Unlock();
}

/*-------------------------------------------------------------------------*/

void 
CActiveObject::WaitForThreadToFinish( Int32 timeoutInMs ) const
{GUCEF_TRACE;

    ThreadWait( _td, timeoutInMs );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
