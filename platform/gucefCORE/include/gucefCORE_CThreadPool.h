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

#ifndef GUCEF_CORE_CIDATANODESERIALIZABLETASKDATA_H
#include "gucefCORE_CIDataNodeSerializableTaskData.h"
#define GUCEF_CORE_CIDATANODESERIALIZABLETASKDATA_H
#endif /* GUCEF_CORE_CIDATANODESERIALIZABLETASKDATA_H ? */

#ifndef GUCEF_CORE_CTHREADPOOLINFO_H
#include "gucefCORE_CThreadPoollnfo.h"
#define GUCEF_CORE_CTHREADPOOLINFO_H
#endif /* GUCEF_CORE_CTHREADPOOLINFO_H ? */

#ifndef GUCEF_CORE_CTHREADINFO_H
#include "gucefCORE_CThreadlnfo.h"
#define GUCEF_CORE_CTHREADINFO_H
#endif /* GUCEF_CORE_CTHREADINFO_H ? */

#ifndef GUCEF_CORE_TASKSTATUS_H
#include "gucefCORE_TaskStatus.h"
#define GUCEF_CORE_TASKSTATUS_H
#endif /* GUCEF_CORE_TASKSTATUS_H ? */

#ifndef GUCEF_CORE_CTASKINFO_H
#include "gucefCORE_CTaskInfo.h"
#define GUCEF_CORE_CTASKINFO_H
#endif /* GUCEF_CORE_CTASKINFO_H ? */

#ifndef GUCEF_CORE_CLONEABLES_H
#include "cloneables.h"
#define GUCEF_CORE_CLONEABLES_H
#endif /* GUCEF_CORE_CLONEABLES_H ? */

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

    static const CString ClassTypeName;

    static const CEvent ThreadKilledEvent;
    static const CEvent ThreadStartedEvent;
    static const CEvent ThreadPausedEvent;
    static const CEvent ThreadResumedEvent;
    static const CEvent ThreadFinishedEvent;

    static const CEvent TaskQueuedEvent;
    static const CEvent TaskStartupEvent;
    static const CEvent TaskStartedEvent;
    static const CEvent TaskStartupFailedEvent;
    static const CEvent TaskKilledEvent;
    static const CEvent TaskStoppedEvent;
    static const CEvent TaskPausedEvent;
    static const CEvent TaskResumedEvent;
    static const CEvent TaskFinishedEvent;

    static void RegisterEvents( void );

    public:

    typedef CTFactoryBase< CTaskConsumer, MT::CMutex >                      TTaskConsumerFactory;
    typedef CTFactoryBase< CIDataNodeSerializableTaskData, MT::CMutex >     TTaskDataFactory;
    typedef std::map< UInt32, CTaskInfo >                                   TTaskInfoMap;
    typedef std::map< UInt32, CThreadInfo >                                 TThreadInfoMap;

    typedef TCloneableUInt32                                    TThreadKilledEventData;
    typedef TCloneableUInt32                                    TThreadStartedEventData;
    typedef TCloneableUInt32                                    TThreadPausedEventData;
    typedef TCloneableUInt32                                    TThreadResumedEventData;
    typedef TCloneableUInt32                                    TThreadFinishedEventData;
    typedef TCloneableUInt32                                    TThreadUpdateEventData;
    
    typedef TCloneableUInt32                                    TTaskQueuedEventData;
    typedef TCloneableUInt32                                    TTaskStartupEventData;
    typedef TCloneableUInt32                                    TTaskStartedEventData;
    typedef TCloneableUInt32                                    TTaskStartupFailedEventData;
    typedef TCloneableUInt32                                    TTaskKilledEventData;
    typedef TCloneableUInt32                                    TTaskStoppedEventData;
    typedef TCloneableUInt32                                    TTaskPausedEventData;
    typedef TCloneableUInt32                                    TTaskResumedEventData;
    typedef TCloneableUInt32                                    TTaskFinishedEventData;
    typedef TCloneableUInt32                                    TTaskUpdateEventData;

    /**
     *  Queues a task for execution as soon as a thread is available
     *  to execute it.
     *
     *  @param assumeOwnershipOfTaskData    Whether the taskData given (if any) needs a private copy 
     *                                      or whether the task manager can assume ownership
     */
    TTaskStatus QueueTask( const CString& taskType                        ,
                           CICloneable* taskData = GUCEF_NULL             ,
                           CTaskConsumerPtr* outTaskConsumer = GUCEF_NULL ,
                           CObserver* taskObserver = GUCEF_NULL           ,
                           bool assumeOwnershipOfTaskData = false         );

    /**
     *  Immediatly starts executing a task using the task
     *  information provided. Based on the provided information
     *  a task consumer will be constructed to actually carry out the task
     */
    TTaskStatus StartTask( const CString& taskType                        ,
                           CICloneable* taskData = GUCEF_NULL             ,
                           CTaskConsumerPtr* outTaskConsumer = GUCEF_NULL ,
                           bool assumeOwnershipOfTaskData = false         );

    /**
     *  Checks if a task of the given type already exists, if yes nothing new happens
     *  If no a new task one would be started right away
     */
    TTaskStatus StartTaskIfNoneExists( const CString& taskType                        ,
                                       CICloneable* taskData = GUCEF_NULL             ,
                                       CTaskConsumerPtr* outTaskConsumer = GUCEF_NULL ,
                                       bool assumeOwnershipOfTaskData = false         );

    /**
     *  Checks if a task of the given type already exists, if yes nothing new happens
     *  If no a new task one would be started right away
     */
    TTaskStatus StartTaskIfNoneExists( const CString& taskType                        ,
                                       const CDataNode& taskData                      ,
                                       CTaskConsumerPtr* outTaskConsumer = GUCEF_NULL );

    /**
     *  Immediatly starts executing a task using the task consumer provided.
     *
     *  Note that any task that was setup using SetupTask() still requires to be started via a call to
     *  StartTask()
     */
    TTaskStatus StartTask( CTaskConsumerPtr task                  ,
                           CICloneable* taskData = GUCEF_NULL     ,
                           bool assumeOwnershipOfTaskData = false );
    
    /**
     *  Same as other StartTask() variant except it will construct task data from the given DOM
     *  This requires a task data factory to be registered to construct a relevant data object
     */
    TTaskStatus StartTask( const CString& taskType                        ,
                           const CDataNode& taskData                      ,
                           CTaskConsumerPtr* outTaskConsumer = GUCEF_NULL ,
                           bool assumeOwnershipOfTaskData = false         );

    /**
     *  Performs setup for a task (thread association) but does not start the task yet
     *  starts executing the task via an additional call to StartTask()
     *
     *  If you have an interdepency between threads and their associated tasks you 
     *  might need the thread to already be available and linked to the consumer without actually
     *  starting the consumer just to perform initial setup between said tasks.
     *  A typical case is needing a PulseGenerator during setup, which is provided by the to-be-associated 
     *  delegator.
     *
     *  This functionality allows you to break out thread association into an independent step
     */
    TTaskStatus SetupTask( CTaskConsumerPtr task                  ,
                           CICloneable* taskData = GUCEF_NULL     ,
                           bool assumeOwnershipOfTaskData = false );

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

    void SetNrOfWorkerThreadsToLogicalCPUs( const UInt32 coreFactor );

    void SetDesiredMaxTotalNrOfThreads( const Int32 nrOfThreads );

    Int32 GetDesiredMaxTotalNrOfThreads( void ) const;

    void SetDesiredMinNrOfWorkerThreads( const UInt32 nrOfThreads );

    UInt32 GetDesiredMinNrOfWorkerThreads( void ) const;

    UInt32 GetActiveNrOfThreads( void ) const;

    UInt32 GetActiveNrOfDedicatedThreads( void ) const;

    UInt32 GetActiveNrOfWorkerThreads( void ) const;

    UInt32 GetNrOfQueuedTasks( void ) const;

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

    void GetAllRegisteredTaskConsumerFactoryTypes( CString::StringSet& taskTypes ) const;

    void RegisterTaskDataFactory( const CString& taskType   ,
                                  TTaskDataFactory* factory );

    void UnregisterTaskDataFactory( const CString& taskType );

    bool IsTaskOfTaskTypeExecutable( const CString& taskType ) const;
    
    bool IsCustomTaskDataForTaskTypeSerializable( const CString& taskType ) const;

    CIDataNodeSerializableTaskDataBasicPtr CreateCustomTaskDataForTaskTypeIfAvailable( const CString& taskType ) const;
    
    void GetAllRegisteredTaskDataFactoryTypes( CString::StringSet& taskTypes ) const;

    bool TaskOfTypeExists( const CString& taskType               ,
                           UInt32* taskIdIfExists = GUCEF_NULL   , 
                           UInt32* threadIdIfExists = GUCEF_NULL ,
                           TTaskStatus* taskStatus = GUCEF_NULL  ) const;
    
    virtual const CString& GetClassTypeName( void ) const  GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  Obtains a snapshot in time of which task which what id is being processed by a thread with the given thread id
     *  Note that since this is information that can change at any time this is for informational/debugging (display?) purposes only
     *
     *  @param taskId set to 0 by default/failure and set to the relevant task id on success
     */
    bool GetTaskIdForThreadId( const UInt32 threadId ,
                               UInt32& taskId        ) const;

    /**
     *  Obtains a snapshot in time of which thread with a given thread id is processing a task with the given task id, if any
     *  Note that since this is information that can change at any time this is for informational/debugging (display?) purposes only
     *
     *  @param threadId set to 0 by default/failure and set to the relevant thread id on success 
     */
    bool GetThreadIdForTaskId( const UInt32 taskId ,
                               UInt32& threadId    ) const;    
    
    /**
     *  Obtains a coherent snapshot of threadpool information in a single lock protected call
     */
    bool GetInfo( CThreadPoolInfo& info ) const;

    /**
     *  Obtains a coherent snapshot of task information in a single lock protected call
     */
    bool GetTaskInfo( UInt32 taskId                                                                , 
                      CTaskInfo& info                                                              ,  
                      bool obtainTaskDataCopyIfPossible                                            ,
                      const CDataNodeSerializableSettings* taskDataSerializerSettings = GUCEF_NULL ) const;

    /**
     *  Obtains a coherent snapshot of all task information in a single lock protected call
     */
    bool GetAllTaskInfo( TTaskInfoMap& info                                                           ,
                         bool obtainTaskDataCopyIfPossible                                            ,
                         const CDataNodeSerializableSettings* taskDataSerializerSettings = GUCEF_NULL ) const;

    /**
     *  Obtains a coherent snapshot of thread information in a single lock protected call
     */
    bool GetThreadInfo( UInt32 threadId, CThreadInfo& info ) const;

    /**
     *  Obtains a coherent snapshot of all thread information in a single lock protected call
     */
    bool GetAllThreadInfo( TThreadInfoMap& info ) const;

    /**
     *  If the task with the given id was provided with any 'work' data and said data is serializable this can be used to 
     *  obtain a copy of said data
     */
    bool GetSerializedTaskDataCopy( UInt32 taskId                                           ,
                                    CDataNode& domNode                                      ,
                                    const CDataNodeSerializableSettings& serializerSettings ) const;

    const CString& GetThreadPoolName( void ) const;
    
    virtual ~CThreadPool() GUCEF_VIRTUAL_OVERRIDE;
    
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

    void OnTaskStartup( CTaskConsumerPtr taskConsumer );
    void OnTaskStarted( CTaskConsumerPtr taskConsumer );
    void OnTaskStartupFailed( CTaskConsumerPtr taskConsumer );
    void OnTaskKilled( CTaskConsumerPtr taskConsumer );
    void OnTaskStopped( CTaskConsumerPtr taskConsumer );
    void OnTaskPaused( CTaskConsumerPtr taskConsumer );
    void OnTaskResumed( CTaskConsumerPtr taskConsumer );
    void OnTaskFinished( CTaskConsumerPtr taskConsumer );

    private:
    friend class CTaskManager;

    CThreadPool( PulseGeneratorPtr threadPoolPulseContext , 
                 const CString& poolName                  );

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

    void EnforceDesiredNrOfThreads( Int32 desiredMaxTotalNrOfThreads   ,
                                    UInt32 desiredMinNrOfWorkerThreads ,
                                    bool gracefullEnforcement          );

    void RemoveConsumer( const UInt32 taskID );

    void RemoveDelegator( CNotifier* notifier );

    void SubscribeToTaskConsumerEvents( CTaskConsumerPtr& taskConsumer );

    CThreadPool( const CThreadPool& src );                   /**< not supported */
    CThreadPool& operator=( const CThreadPool& src );        /**< not supported */

    private:

    typedef CTAbstractFactory< CString, CTaskConsumer, MT::CMutex > TAbstractTaskConsumerFactory;
    typedef CTAbstractFactory< CString, CIDataNodeSerializableTaskData, MT::CMutex > TAbstractTaskDataFactory;
    typedef MT::CTMailBox< CString > TTaskMailbox;
    typedef std::map< UInt32, CTaskConsumerPtr > TTaskConsumerMap;
    typedef CTBasicSharedPtr< CTaskDelegator, MT::CMutex >  TTaskDelegatorBasicPtr; 
    typedef std::set< TTaskDelegatorBasicPtr > TTaskDelegatorSet;

    CString m_poolName;
    TAbstractTaskConsumerFactory m_consumerFactory;
    TAbstractTaskDataFactory m_taskDataFactory;
    Int32 m_desiredMaxTotalNrOfThreads;
    UInt32 m_desiredMinNrOfWorkerThreads;
    TTaskMailbox m_taskQueue;
    TTaskConsumerMap m_taskConsumerMap;
    TTaskDelegatorSet m_taskDedicatedDelegators;
    TTaskDelegatorSet m_taskGenericDelegators;
    bool m_acceptNewWork;
    bool m_allowAppThreadToWork;
};

/*-------------------------------------------------------------------------*/

typedef CTSharedPtr< CThreadPool, MT::CMutex >          ThreadPoolPtr;
typedef typename ThreadPoolPtr::TBasicSharedPtrBase     TBasicThreadPoolPtr;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTHREADPOOL_H ? */
