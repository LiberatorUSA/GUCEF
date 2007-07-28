/*
 *  gucefMT: GUCEF module providing multithreading solutions
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
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
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
          _suspend( false ) ,
          _active( false )
{

}

/*-------------------------------------------------------------------------*/

CActiveObject::CActiveObject( const CActiveObject& src )
        : _taskdata( src._taskdata ) ,
          _delay( src._delay )       ,
          _suspend( false )          ,
          _active( false )
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

UInt32
CActiveObject::OnActivate( void* thisobject )
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
