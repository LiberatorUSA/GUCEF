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

#ifndef GUCEF_MT_CACTIVEOBJECT_H
#include "gucefMT_CActiveObject.h"
#define GUCEF_MT_CACTIVEOBJECT_H
#endif /* GUCEF_MT_CACTIVEOBJECT_H ? */

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

class CTaskManager;
class CTaskConsumer;

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
    
    public:

    bool SetTaskConsumer( CTaskConsumer* taskConsumer );

    CTaskConsumer* GetTaskConsumer( void );

    CPulseGenerator& GetPulseGenerator( void );

    protected:
    friend class CTaskManager;

    CTaskDelegator( void );

    virtual ~CTaskDelegator();

    /**
     *  Startup routine for the task. You should return true if startup succeeded and the task can commence
     *  cycling.
     */
    virtual bool OnTaskStart( void* taskdata );
    
    /**
     *  Called after a successfull call to OnTaskStart
     */
    virtual void OnTaskStarted( void* taskdata );

    /**
     *  Perorm all your main task work in this function.
     *  It will be called repeatedly until true is returned indicating that the task has been completed.
     *  Thus for ongoing tasks you can write this function to take care of a single interation of the task.
     */
    virtual bool OnTaskCycle( void* taskdata );

    /**
     *  This is where all the cleanup should be done for task data
     *  Note that this member function will be called from within the spawned thread when ending gracefully
     *  but in the case of a forcefull termination of the spawned thread this member function will be called
     *  from the thread that triggered the forcefull termination.
     */
    virtual void OnTaskEnd( void* taskdata );

    virtual void OnTaskPausedForcibly( void* taskdata );
    
    virtual void OnTaskResumed( void* taskdata );
    
    virtual void OnTaskEnded( void* taskdata ,
                              bool forced    );

    virtual bool ProcessTask( CTaskConsumer& taskConsumer ,
                              CICloneable* taskData       );
                              
    virtual void TaskCleanup( CTaskConsumer* taskConsumer ,
                              CICloneable* taskData       );
    
    protected:

    CPulseGenerator m_pulseGenerator;
    
    private:

    CTaskDelegator( const CTaskDelegator& src );
    CTaskDelegator& operator=( const CTaskDelegator& src );

    virtual void RequestImmediatePulse( CPulseGenerator& pulseGenerator );

    virtual void RequestPulsesPerImmediatePulseRequest( CPulseGenerator& pulseGenerator                     ,
                                                        const Int32 requestedPulsesPerImmediatePulseRequest );

    virtual void RequestPeriodicPulses( CPulseGenerator& pulseGenerator    ,
                                        const UInt32 pulseDeltaInMilliSecs );

    virtual void RequestPulseInterval( CPulseGenerator& pulseGenerator    ,
                                       const UInt32 pulseDeltaInMilliSecs );

    virtual void RequestStopOfPeriodicUpdates( CPulseGenerator& pulseGenerator );

    private:

    CTaskConsumer* m_taskConsumer;
    bool m_consumerBusy;
    Int32 m_immediatePulseTickets;
    Int32 m_immediatePulseTicketMax;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTASKDELEGATOR_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 20-02-2005 :
        - Dinand: Added this class, it is based on some older C implementation
          I made once. Ported but not tested.

----------------------------------------------------------------------------*/
