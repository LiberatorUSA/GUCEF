/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
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

#ifndef GUCEF_CORE_CTASKDELEGATOR_H
#define GUCEF_CORE_CTASKDELEGATOR_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

#ifndef GUCEF_MT_CACTIVEOBJECT_H
#include "gucefMT_CActiveObject.h"
#define GUCEF_MT_CACTIVEOBJECT_H
#endif /* GUCEF_MT_CACTIVEOBJECT_H ? */

#ifndef GUCEF_CORE_CTSHAREDPTR_H
#include "CTSharedPtr.h"
#define GUCEF_CORE_CTSHAREDPTR_H
#endif /* GUCEF_CORE_CTSHAREDPTR_H ? */

#ifndef GUCEF_CORE_CNOTIFIER_H
#include "CNotifier.h"
#define GUCEF_CORE_CNOTIFIER_H
#endif /* GUCEF_CORE_CNOTIFIER_H ? */

#ifndef GUCEF_CORE_CTNUMERICID_H
#include "CTNumericID.h"
#define GUCEF_CORE_CTNUMERICID_H
#endif /* GUCEF_CORE_CTNUMERICID_H ? */

#ifndef GUCEF_CORE_CPULSEGENERATOR_H
#include "gucefCORE_CPulseGenerator.h"
#define GUCEF_CORE_CPULSEGENERATOR_H
#endif /* GUCEF_CORE_CPULSEGENERATOR_H ? */

#ifndef GUCEF_CORE_CIPULSEGENERATORDRIVER_H
#include "gucefCORE_CIPulseGeneratorDriver.h"
#define GUCEF_CORE_CIPULSEGENERATORDRIVER_H
#endif /* GUCEF_CORE_CIPULSEGENERATORDRIVER_H ? */

#ifndef GUCEF_CORE_CTASKCONSUMER_H
#include "gucefCORE_CITaskConsumer.h"
#define GUCEF_CORE_CTASKCONSUMER_H
#endif /* GUCEF_CORE_CTASKCONSUMER_H ? */

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

class CThreadPool;

/*-------------------------------------------------------------------------*/

/**
 *  This is an internally used class and is tightly coupled with the 
 *  CTaskManager class. It should not be used directly.
 *
 *  This class implements an active object that delegates the actual logic to 
 *  be threaded to a task consumer implementor. Thus seperating thread lifecycle
 *  management from task lifecycle management. A delegator can execute as many
 *  tasks by invoking as many consumers as desired without having to (re)create
 *  a thread.
 */
class GUCEF_CORE_PRIVATE_CPP CTaskDelegator : public MT::CActiveObject      ,
                                              public CNotifier              ,
                                              private CIPulseGeneratorDriver
{
    public:
    
    static const CEvent ThreadKilledEvent;
    static const CEvent ThreadStartedEvent;
    static const CEvent ThreadPausedEvent;
    static const CEvent ThreadResumedEvent;
    static const CEvent ThreadFinishedEvent;
    
    static void RegisterEvents( void );

    virtual const MT::CILockable* AsLockable( void ) const GUCEF_VIRTUAL_OVERRIDE;
    
    public:

    bool SetTaskConsumer( CTaskConsumerPtr taskConsumer );

    CTaskConsumerPtr GetTaskConsumer( void );

    CPulseGenerator& GetPulseGenerator( void );

    protected:
    friend class CThreadPool;
    friend class CTaskConsumer;

    CTaskDelegator( CThreadPool* threadPool );

    virtual ~CTaskDelegator();

    /**
     *  Startup routine for the task. You should return true if startup succeeded and the task can commence
     *  cycling.
     */
    virtual bool OnThreadStart( void* taskdata ) GUCEF_VIRTUAL_OVERRIDE;
    
    /**
     *  Called after a successfull call to OnTaskStart
     */
    virtual void OnThreadStarted( void* taskdata ) GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  Perform all your main task work in this function.
     *  It will be called repeatedly until true is returned indicating that the task has been completed.
     *  Thus for ongoing tasks you can write this function to take care of a single interation of the task.
     */
    virtual bool OnThreadCycle( void* taskdata ) GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  Last chance notification to decended classes of impending end of the tread
     *  If the 'willBeForced' flag is true the thread will be killed next 
     *  Since this would be used in cases where the thread is misbehaving one should
     *  keep that in mind in the implementation of this callback
     */
    virtual void OnThreadEnding( void* taskdata    ,
                                 bool willBeForced ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void OnThreadPausedForcibly( void* taskdata ) GUCEF_VIRTUAL_OVERRIDE;
    
    virtual void OnThreadResumed( void* taskdata ) GUCEF_VIRTUAL_OVERRIDE;
    
    /**
     *  This is where all the cleanup should be done for task data
     *  Note that this member function will be called from within the spawned thread when ending gracefully
     *  but in the case of a forcefull termination of the spawned thread this member function will be called
     *  from the thread that triggered the forcefull termination.
     */
    virtual void OnThreadEnded( void* taskdata ,
                                bool forced    ) GUCEF_VIRTUAL_OVERRIDE;

    virtual bool ProcessTask( CTaskConsumerPtr taskConsumer ,
                              CICloneable* taskData         );
                              
    virtual void TaskCleanup( CTaskConsumerPtr taskConsumer ,
                              CICloneable* taskData         );
    
    protected:

    virtual bool Lock( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Unlock( void ) const GUCEF_VIRTUAL_OVERRIDE;

    CPulseGenerator m_pulseGenerator;
    
    private:

    CTaskDelegator( const CTaskDelegator& src );
    CTaskDelegator& operator=( const CTaskDelegator& src );

    virtual void RequestImmediatePulse( CPulseGenerator& pulseGenerator ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void RequestPulsesPerImmediatePulseRequest( CPulseGenerator& pulseGenerator                     ,
                                                        const Int32 requestedPulsesPerImmediatePulseRequest ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void RequestPeriodicPulses( CPulseGenerator& pulseGenerator    ,
                                        const UInt32 pulseDeltaInMilliSecs ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void RequestPulseInterval( CPulseGenerator& pulseGenerator    ,
                                       const UInt32 pulseDeltaInMilliSecs ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void RequestStopOfPeriodicUpdates( CPulseGenerator& pulseGenerator ) GUCEF_VIRTUAL_OVERRIDE;

    protected:

    CTaskDelegator( CThreadPool* threadPool       ,
                    CTaskConsumerPtr taskConsumer ,
                    CICloneable* taskData         );

    CTaskConsumerPtr m_taskConsumer;
    CICloneable* m_taskData;

    private:

    CThreadPool* m_threadPool;
    bool m_consumerBusy;
    Int32 m_immediatePulseTickets;
    Int32 m_immediatePulseTicketMax;
};

/*-------------------------------------------------------------------------*/

typedef CTSharedPtr< CTaskDelegator, MT::CMutex >   CTaskDelegatorPtr;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTASKDELEGATOR_H ? */
