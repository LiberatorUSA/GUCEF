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
        : _taskdata( NULL ) ,
          _delay( 10 )      ,
          _active( false )  ,
          _suspend( false )
{

}

/*-------------------------------------------------------------------------*/

CActiveObject::CActiveObject( const CActiveObject& src )
        : _taskdata( src._taskdata ) ,
          _delay( src._delay )       ,
          _active( false )           ,
          _suspend( false )
{
        if ( src.IsActive() )
        {
                Activate();
        }
}

/*-------------------------------------------------------------------------*/

CActiveObject::~CActiveObject()
{
        Deactivate( true );
}

/*-------------------------------------------------------------------------*/

bool
CActiveObject::IsActive( void ) const
{
        return _active;
}

/*-------------------------------------------------------------------------*/

UInt32 GUCEF_CALLSPEC_STD_PREFIX
CActiveObject::OnActivate( void* thisobject ) GUCEF_CALLSPEC_STD_SUFFIX
{
    CActiveObject* tao = (CActiveObject*) thisobject;
    void* taskdata = tao->GetTaskData();

    if ( tao->OnTaskStart( taskdata ) )
    {
        Float64 timerRes = ( PrecisionTimerResolution() * 1.0 );
        UInt64 tickCount = PrecisionTickCount(); 
        UInt64 newTime = tickCount;
        Float64 timeDelta = 0;
        
        bool taskfinished = false;
        while ( !taskfinished && tao->_active )
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
                newTime = PrecisionTickCount();
                timeDelta = ( tickCount - newTime ) / timerRes;
                if ( timeDelta < tao->m_minimalCycleDelta )
                {
                    PrecisionDelay( tao->_delay );
                    tickCount = PrecisionTickCount();
                }
                else
                {
                    tickCount = newTime;
                }
            }
        }

        tao->OnTaskEnd( taskdata );
    }
    tao->_td = NULL;
    return 1;
}

/*-------------------------------------------------------------------------*/

void
CActiveObject::Activate( void* taskdata /* = NULL */               ,
                         const UInt32 cycleDelay /* = 0 */         ,
                         const UInt32 minimalCycleDelta /* = 10 */ )
{
        if ( _active ) return;

        _taskdata = taskdata;
        _delay = cycleDelay;
        m_minimalCycleDelta = minimalCycleDelta / 1000.0; // <- the unit used here is seconds not milliseconds
        _td = ThreadCreate( (void*) OnActivate ,
                            this               );
        _active = true;
}

/*-------------------------------------------------------------------------*/

void
CActiveObject::Deactivate( bool force )
{
    if ( _active )
    {
        if ( force )
        {
                /*
                 *      Kill the thread
                 */
                ThreadKill( _td );
        }
        _active = false;
    }
}

/*-------------------------------------------------------------------------*/

void
CActiveObject::Pause( void )
{
        _suspend = true;
        _active = false;
}

/*-------------------------------------------------------------------------*/

void
CActiveObject::Resume( void )
{
        if ( _td && _suspend )
        {
                ThreadResume( _td );
                _active = true;
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
        Deactivate( true );

        _taskdata = src._taskdata;
        _delay = src._delay;
        _suspend = false;

        if ( src.IsActive() )
        {
                Activate();
        }

        return *this;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
