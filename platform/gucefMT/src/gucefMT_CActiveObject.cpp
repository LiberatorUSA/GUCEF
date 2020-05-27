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
    : _taskdata( NULL )                    
    , m_delayInMilliSecs( 25 )             
    , m_isDeactivationRequested( false )   
    , _suspend( false )                    
    , _active( false )                     
    , _td( NULL )                          
    , m_minimalCycleDeltaInMilliSecs( 10 )
{

}

/*-------------------------------------------------------------------------*/

CActiveObject::CActiveObject( const CActiveObject& src )
    : _taskdata( src._taskdata )         
    , m_delayInMilliSecs( src.m_delayInMilliSecs )       
    , m_isDeactivationRequested( false ) 
    , _suspend( false )                  
    , _active( false )                      
    , _td( NULL )                           
    , m_minimalCycleDeltaInMilliSecs( 10 )
{
    if ( src.IsActive() )
    {
        Activate();
    }
}

/*-------------------------------------------------------------------------*/

CActiveObject::~CActiveObject()
{
    Deactivate( true, true );
}

/*-------------------------------------------------------------------------*/

bool
CActiveObject::IsDeactivationRequested( void ) const
{
    return m_isDeactivationRequested;
}

/*-------------------------------------------------------------------------*/

bool
CActiveObject::IsActive( void ) const
{
    return _active;
}

/*-------------------------------------------------------------------------*/

UInt32
CActiveObject::OnActivate( void* thisobject )
{
    CActiveObject* tao = (CActiveObject*) thisobject;
    void* taskdata = tao->GetTaskData();

    if ( tao->OnTaskStart( taskdata ) )
    {
        tao->OnTaskStarted( taskdata );
       
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
            taskfinished = tao->OnTaskCycle( taskdata );

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

        tao->OnTaskEnd( taskdata );
    }
    tao->_td = NULL;
    tao->_active = false;
    return 1;
}

/*-------------------------------------------------------------------------*/

bool
CActiveObject::Activate( void* taskdata /* = NULL */                   ,
                         const UInt32 cycleDelay /* = 25 */            ,
                         const UInt32 minimalCycleDeltaInMs /* = 10 */ )
{
    if ( _active ) return false;

    _taskdata = taskdata;
    m_delayInMilliSecs = cycleDelay;
    m_minimalCycleDeltaInMilliSecs = minimalCycleDeltaInMs;
    m_isDeactivationRequested = false;
    _suspend = false;
    _active = true;
    _td = ThreadCreate( (TThreadFunc) OnActivate ,
                        this                     );

    return true;
}

/*-------------------------------------------------------------------------*/

void
CActiveObject::OnTaskPausedForcibly( void* taskdata )
{
    // Dummy to avoid mandatory implementation in decending classes
}

/*-------------------------------------------------------------------------*/

void
CActiveObject::OnTaskResumed( void* taskdata )
{
    // Dummy to avoid mandatory implementation in decending classes
}

/*-------------------------------------------------------------------------*/
    
void
CActiveObject::OnTaskEnded( void* taskdata ,
                            bool forced    )
{
    // Dummy to avoid mandatory implementation in decending classes
}

/*-------------------------------------------------------------------------*/
    
void
CActiveObject::OnTaskStarted( void* taskdata )
{
    // Dummy to avoid mandatory implementation in decending classes
}

/*-------------------------------------------------------------------------*/

void
CActiveObject::Deactivate( bool force, bool callerShouldWait )
{
    if ( _active )
    {
        if ( force )
        {
            /*
             *      Kill the thread
             */
            ThreadKill( _td );

            OnTaskEnd( _taskdata );
            OnTaskEnded( _taskdata, true );
        }
        else
        {
            m_isDeactivationRequested = true;

            if ( callerShouldWait )
            {
                ThreadWait( _td, 150000 );
            }
        }
    }
}

/*-------------------------------------------------------------------------*/

bool
CActiveObject::IsPauseRequested( void ) const
{
    return _suspend;
}

/*-------------------------------------------------------------------------*/

void
CActiveObject::Pause( bool force )
{
    if ( force )
    {
        // Don't wait for the thread to cycle
        ThreadSuspend( _td );
        _active = false;
        OnTaskPausedForcibly( _taskdata );
    }
    else
    {
        _suspend = true;
    }
}

/*-------------------------------------------------------------------------*/

void
CActiveObject::Resume( void )
{
    if ( NULL != _td && _suspend )
    {
        _active = true;
        ThreadResume( _td );
        OnTaskResumed( _taskdata );
    }
}

/*-------------------------------------------------------------------------*/

void*
CActiveObject::GetTaskData( void ) const
{
    return _taskdata;
}

/*-------------------------------------------------------------------------*/

CActiveObject&
CActiveObject::operator=( const CActiveObject& src )
{
    if ( this != &src )  
    {
        Deactivate( true, true );

        _taskdata = src._taskdata;
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
{
    if ( NULL != _td )
    {
        return ThreadID( _td );
    }
    return 0;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
