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

#ifndef GUCEF_CORE_CTASKMANAGER_H
#define GUCEF_CORE_CTASKMANAGER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <map>

#ifndef GUCEF_MT_CTMAILBOX_H
#include "gucefMT_CTMailBox.h"
#define GUCEF_MT_CTMAILBOX_H
#endif /* GUCEF_MT_CTMAILBOX_H ? */

#ifndef GUCEF_CORE_CTSGNOTIFIER_H
#include "CTSGNotifier.h"
#define GUCEF_CORE_CTSGNOTIFIER_H
#endif /* GUCEF_CORE_CTSGNOTIFIER_H ? */

#ifndef GUCEF_CORE_CTABSTRACTFACTORY_H
#include "CTAbstractFactory.h"
#define GUCEF_CORE_CTABSTRACTFACTORY_H
#endif /* GUCEF_CORE_CTABSTRACTFACTORY_H ? */

#ifndef GUCEF_CORE_CITASKCONSUMER_H
#include "gucefCORE_CITaskConsumer.h"
#define GUCEF_CORE_CITASKCONSUMER_H
#endif /* GUCEF_CORE_CITASKCONSUMER_H ? */

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
 *  The task manager provides centralized managent of tasks and more importantly
 *  the threads that execute those tasks. A task is defined as the logic you wish to
 *  have executed in a seperare thread. Instead of creating the thread yourself you
 *  create a task consumer and then decide when your task needs to be executed. The
 *  task manager will deal with threads themselves. This seperation of concerns allows
 *  for optimal thread reuse and schedueling. Threads have a global effect on your process
 *  and as such should be tracked globally within your process which is what the task
 *  manager does
 */
class GUCEF_CORE_PUBLIC_CPP CTaskManager : public CTSGNotifier
{
    public:

    static const CEvent ThreadKilledEvent;
    static const CEvent ThreadStartedEvent;
    static const CEvent ThreadPausedEvent;
    static const CEvent ThreadResumedEvent;
    static const CEvent ThreadFinishedEvent;

    static const CEvent TaskQueuedEvent;
    static const CEvent QueuedTaskStartedEvent;
    static const CEvent TaskStartedEvent;
    static const CEvent TaskStartupFailedEvent;
    static const CEvent TaskKilledEvent;
    static const CEvent TaskStoppedEvent;
    static const CEvent TaskPausedEvent;
    static const CEvent TaskResumedEvent;
    static const CEvent TaskFinishedEvent;

    static void RegisterEvents( void );

    public:

    typedef CTFactoryBase< CTaskConsumer > TTaskConsumerFactory;

    /**
     *  Queues a task for execution as soon as a thread is available
     *  to execute it.
     *
     *  @param assumeOwnershipOfTaskData Whether the taskData given (if any) needs a private copy or whether the task manager can assume ownership
     */
    bool QueueTask( const CString& taskType                        ,
                    CICloneable* taskData = GUCEF_NULL             ,
                    CTaskConsumerPtr* outTaskConsumer = GUCEF_NULL ,
                    CObserver* taskObserver = GUCEF_NULL           ,
                    bool assumeOwnershipOfTaskData = false         );

    /**
     *  Immediatly starts executing a task using the task
     *  information provided. Based on the provided information
     *  a task consumer will be constructed to actually carry out the task
     */
    bool StartTask( const CString& taskType                        ,
                    CICloneable* taskData = GUCEF_NULL             ,
                    CTaskConsumerPtr* outTaskConsumer = GUCEF_NULL );

    /**
     *  Immediatly starts executing a task using the task
     *  consumer provided.
     */
    bool StartTask( CTaskConsumerPtr task              ,
                    CICloneable* taskData = GUCEF_NULL );

    bool PauseTask( const UInt32 taskID ,
                    const bool force    );

    bool ResumeTask( const UInt32 taskID );

    bool RequestTaskToStop( const UInt32 taskId   , 
                            bool callerShouldWait );

    bool RequestTaskToStop( CTaskConsumerPtr taskConsumer ,
                            bool callerShouldWait         );

    bool WaitForTaskToFinish( const UInt32 taskId, Int32 timeoutInMs );

    bool WaitForTaskToFinish( CTaskConsumerPtr taskConsumer, Int32 timeoutInMs );

    bool KillTask( const UInt32 taskID );

    void SetNrOfThreadsToLogicalCPUs( const UInt32 coreFactor );

    void SetNrOfThreads( const UInt32 nrOfThreads );

    UInt32 GetNrOfThreads( void ) const;

    void RequestAllTasksToStop( bool waitOnStop, bool acceptNewWork );

    void RequestAllThreadsToStop( bool waitOnStop, bool acceptNewWork );

    void RegisterTaskConsumerFactory( const CString& taskType       ,
                                      TTaskConsumerFactory* factory );

    void UnregisterTaskConsumerFactory( const CString& taskType );

    virtual const CString& GetClassTypeName( void ) const  GUCEF_VIRTUAL_OVERRIDE;

    bool GetTaskIdForThreadId( const UInt32 threadId ,
                               UInt32& taskId        ) const;

    bool GetThreadIdForTaskId( const UInt32 taskId ,
                               UInt32& threadId    ) const;

    protected:

    virtual void OnPumpedNotify( CNotifier* notifier           ,
                                 const CEvent& eventid         ,
                                 CICloneable* eventdata = NULL ) GUCEF_VIRTUAL_OVERRIDE;

    private:
    friend class CTaskDelegator;

    void RegisterTaskDelegator( CTaskDelegator& delegator );

    void UnregisterTaskDelegator( CTaskDelegator& delegator );

    bool GetQueuedTask( CTaskConsumerPtr& taskConsumer ,
                        CICloneable** taskData         );

    void TaskCleanup( CTaskConsumerPtr taskConsumer ,
                      CICloneable* taskData         );

    private:
    friend class CTaskConsumer;

    void RegisterTaskConsumerId( CTaskConsumer::TTaskId& taskId );

    void UnregisterTaskConsumerId( CTaskConsumer::TTaskId& taskId );

    private:
    friend class CCoreGlobal;

    CTaskManager( void );

    virtual ~CTaskManager();

    private:

    class CTaskQueueItem : public CICloneable
    {
        public:

        CTaskQueueItem( CTaskConsumerPtr consumer      ,
                        CICloneable* taskData          ,
                        bool assumeOwnershipOfTaskData );

        CTaskQueueItem( CTaskQueueItem& src );

        virtual ~CTaskQueueItem();

        CTaskConsumerPtr& GetTaskConsumer( void );

        CICloneable* GetTaskData( void );

        virtual CICloneable* Clone( void ) const GUCEF_VIRTUAL_OVERRIDE;

        private:
        CICloneable* m_taskData;
        CTaskConsumerPtr m_taskConsumer;
        bool m_assumeOwnershipOfTaskData;
    };

    private:

    void EnforceDesiredNrOfThreads( UInt32 desiredNrOfThreads ,
                                    bool gracefullEnforcement );

    void RemoveConsumerFromQueue( const UInt32 taskID );

    CTaskManager( const CTaskManager& src );

    CTaskManager& operator=( const CTaskManager& src );

    private:

    typedef CTAbstractFactory< CString, CTaskConsumer > TAbstractTaskConsumerFactory;
    typedef MT::CTMailBox< CString > TTaskMailbox;
    typedef std::map< UInt32, CTaskConsumerPtr > TTaskConsumerMap;
    typedef std::set< CTaskDelegator* > TTaskDelegatorSet;
    typedef CTaskConsumer::TTaskIdGenerator TTaskIdGenerator;

    TAbstractTaskConsumerFactory m_consumerFactory;
    UInt32 m_desiredNrOfThreads;
    Int32 m_activeNrOfThreads;
    TTaskMailbox m_taskQueue;
    TTaskIdGenerator m_taskIdGenerator;
    TTaskConsumerMap m_taskConsumerMap;
    TTaskDelegatorSet m_taskDelegators;
    bool m_acceptNewWork;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTASKMANAGER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 20-02-2005 :
        - Dinand: Added this class

----------------------------------------------------------------------------*/
