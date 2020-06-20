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

#ifndef GUCEF_CORE_CTASKCONSUMER_H
#define GUCEF_CORE_CTASKCONSUMER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

#ifndef GUCEF_CORE_CTNUMERICIDGENERATOR_H
#include "CTNumericIDGenerator.h"
#define GUCEF_CORE_CTNUMERICIDGENERATOR_H
#endif /* GUCEF_CORE_CTNUMERICIDGENERATOR_H ? */

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

class CTaskDelegator;

/*-------------------------------------------------------------------------*/

/**
 *  Base class for classes that carry out tasks within a threaded environment
 *
 *  The method used to invoke the functionatility of decending task classes
 *  is not defined by this class but it is assumed to by one means or another
 *  the task is performed within a thread that is exclusve to the task for the
 *  duration of the task.
 */
class GUCEF_CORE_PUBLIC_CPP CTaskConsumer : public CObservingNotifier
{
    public:

    static const CEvent TaskKilledEvent;
    static const CEvent TaskStartedEvent;
    static const CEvent TaskStartupFailedEvent;
    static const CEvent TaskPausedEvent;
    static const CEvent TaskResumedEvent;
    static const CEvent TaskFinishedEvent;

    static void RegisterEvents( void );

    public:

    typedef CTNumericIDGenerator< UInt32 > TTaskIdGenerator;
    typedef TTaskIdGenerator::TNumericID TTaskId;

    CTaskConsumer( void );

    virtual ~CTaskConsumer();

    virtual CString GetType( void ) const = 0;

    void SetTaskDelegator( CTaskDelegator* delegator );

    CTaskDelegator* GetTaskDelegator( void );

    CPulseGenerator* GetPulseGenerator( void );

    UInt32 GetTaskId( void ) const;

    virtual const CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;

    bool IsOwnedByTaskManager( void ) const;

    /**
     *  Startup routine for the task. You should return true if startup succeeded and the task can commence
     *  cycling.
     */
    virtual bool OnTaskStart( CICloneable* taskdata ) = 0;

    /**
     *  Called after a successfull call to OnTaskStart
     */
    virtual void OnTaskStarted( CICloneable* taskdata );

    /**
     *  Called after a failed call to OnTaskStart
     */
    virtual void OnTaskStartupFailed( CICloneable* taskdata );

    /**
     *  Perorm all your main task work in this function.
     *  It will be called repeatedly until true is returned indicating that the task has been completed.
     *  Thus for ongoing tasks you can write this function to take care of a single interation of the task.
     */
    virtual bool OnTaskCycle( CICloneable* taskdata ) = 0;

    /**
     *  This is where all the cleanup should be done for task data
     *  Note that this member function will be called from within the spawned thread when ending gracefully
     *  but in the case of a forcefull termination of the spawned thread this member function will be called
     *  from the thread that triggered the forcefull termination.
     */
    virtual void OnTaskEnd( CICloneable* taskdata ) = 0;

    /**
     *  Last chance notification to decended classes of impending end of the task
     *  If the 'willBeForced' flag is true the thread hosting the task will be killed next 
     *  Since this would be used in cases where the thread and thus this task is misbehaving 
     *  one should keep that in mind in the implementation of this callback
     */
    virtual void OnTaskEnding( CICloneable* taskdata ,
                               bool willBeForced     );

    virtual void OnTaskPaused( CICloneable* taskdata ,
                               bool forced           );

    virtual void OnTaskResumed( CICloneable* taskdata );

    virtual void OnTaskEnded( CICloneable* taskdata ,
                              bool forced           );
    
    protected:

    virtual bool Lock( void ) const GUCEF_VIRTUAL_OVERRIDE;

    virtual bool Unlock( void ) const GUCEF_VIRTUAL_OVERRIDE;

    private:
    friend class CTaskManager;

    void SetIsOwnedByTaskManager( bool ownedByTaskManager );

    private:

    CTaskConsumer( const CTaskConsumer& src  );
    CTaskConsumer& operator=( const CTaskConsumer& src );

    private:

    TTaskId m_taskId;
    CTaskDelegator* m_delegator;
    bool m_ownedByTaskManager;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTASKCONSUMER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 20-02-2005 :
        - Dinand: Added this class

----------------------------------------------------------------------------*/
