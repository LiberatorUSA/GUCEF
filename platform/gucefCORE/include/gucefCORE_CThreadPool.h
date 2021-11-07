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

#ifndef GUCEF_CORE_CTHREADPOOL_H
#define GUCEF_CORE_CTHREADPOOL_H

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
 *  The thread pool provides centralized managent of tasks and more importantly
 *  the threads that execute those tasks. A task is defined as the logic you wish to
 *  have executed in a seperare thread. Instead of creating the thread yourself you
 *  create a task consumer and then decide when your task needs to be executed. The
 *  thread pool will deal with threads themselves. This seperation of concerns allows
 *  for optimal thread reuse and schedueling. 
 *  Threads have a global effect on your process and as such should be tracked globally 
 *  within your process which is what the task manager does. To enforce this only the 
 *  task manager is allowed to make thread pools
 */
class GUCEF_CORE_PUBLIC_CPP CThreadPool : public CTSGNotifier ,
                                          public CTSharedPtrCreator< CThreadPool, MT::CMutex >
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
     *  @param assumeOwnershipOfTaskData    Whether the taskData given (if any) needs a private copy 
     *                                      or whether the task manager can assume ownership
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

    bool PauseTask( const UInt32 taskID                 ,
                    const bool force                    ,
                    const bool okIfTaskIsUnknown = true );

    bool ResumeTask( const UInt32 taskID                  ,
                     const bool okIfTaskIsUnknown = false );

    bool RequestTaskToStop( const UInt32 taskId           , 
                            bool callerShouldWait         ,
                            bool okIfTaskIsUnknown = true );

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

    void SetAllowMainApplicationThreadToPickUpQueuedTasks( bool allowAppThreadToWork );

    bool GetAllowMainApplicationThreadToPickUpQueuedTasks( void ) const;

    /**
     *  Instead of threads managed by the thread pools always doing all the work you can also use this member
     *  function to 'pitch in' and help out in consuming work from some other calling thread.
     *  Do note that this will block the calling thread for the duration of the queued task so use this wisely.
     *
     *  @param maxTasks the max nr of tasks you wish to consume and handle using the calling thread which will be blocked for the entire duration
     *  @return returns the number of tasks that were carried which should be between 0-maxTasks
     */
    UInt32 CarryOutQueuedTasksIfAny( UInt32 maxTasks );

    void RegisterTaskConsumerFactory( const CString& taskType       ,
                                      TTaskConsumerFactory* factory );

    void UnregisterTaskConsumerFactory( const CString& taskType );

    virtual const CString& GetClassTypeName( void ) const  GUCEF_VIRTUAL_OVERRIDE;

    bool GetTaskIdForThreadId( const UInt32 threadId ,
                               UInt32& taskId        ) const;

    bool GetThreadIdForTaskId( const UInt32 taskId ,
                               UInt32& threadId    ) const;

    virtual ~CThreadPool();
    
    protected:

    virtual void OnPumpedNotify( CNotifier* notifier           ,
                                 const CEvent& eventid         ,
                                 CICloneable* eventdata = NULL ) GUCEF_VIRTUAL_OVERRIDE;

    private:
    friend class CTaskDelegator;

    bool GetQueuedTask( CTaskConsumerPtr& taskConsumer ,
                        CICloneable** taskData         );

    void TaskCleanup( CTaskConsumerPtr taskConsumer ,
                      CICloneable* taskData         );

    private:
    friend class CTaskManager;

    CThreadPool( void );

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

    void RemoveConsumer( const UInt32 taskID );

    void RemoveDelegator( CNotifier* notifier );

    CThreadPool( const CThreadPool& src );                   /**< not supported */
    CThreadPool& operator=( const CThreadPool& src );        /**< not supported */

    private:

    typedef CTAbstractFactory< CString, CTaskConsumer, MT::CMutex > TAbstractTaskConsumerFactory;
    typedef MT::CTMailBox< CString > TTaskMailbox;
    typedef std::map< UInt32, CTaskConsumerPtr > TTaskConsumerMap;
    typedef CTBasicSharedPtr< CTaskDelegator, MT::CMutex >  TTaskDelegatorBasicPtr; 
    typedef std::set< TTaskDelegatorBasicPtr > TTaskDelegatorSet;

    TAbstractTaskConsumerFactory m_consumerFactory;
    UInt32 m_desiredNrOfThreads;
    Int32 m_activeNrOfThreads;
    TTaskMailbox m_taskQueue;
    TTaskConsumerMap m_taskConsumerMap;
    TTaskDelegatorSet m_taskDelegators;
    bool m_acceptNewWork;
    bool m_allowAppThreadToWork;
};

/*-------------------------------------------------------------------------*/

typedef CTSharedPtr< CThreadPool, MT::CMutex >  ThreadPoolPtr;
typedef ThreadPoolPtr::TBasicPtr                TBasicThreadPoolPtr;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTHREADPOOL_H ? */
