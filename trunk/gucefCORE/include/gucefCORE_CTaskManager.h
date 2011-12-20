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

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

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
class GUCEF_CORE_PUBLIC_CPP CTaskManager : public CObservingNotifier
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

    static CTaskManager* Instance( void );

    /**
     *  Queues a task for execution as soon as a thread is available
     *  to execute it.
     */
    void QueueTask( const CString& taskType      ,
                    CICloneable* taskData = NULL ,
                    CTaskConsumer** task = NULL  );

    /**
     *  Immediatly starts executing a task using the task
     *  information provided. Based on the provided information
     *  a task consumer will be constructed to actually carry out the task
     */
    bool StartTask( const CString& taskType      ,
                    CICloneable* taskData = NULL ,
                    CTaskConsumer** task = NULL  );

    /**
     *  Immediatly starts executing a task using the task
     *  consumer provided.
     */
    bool StartTask( CTaskConsumer& task          ,
                    CICloneable* taskData = NULL );

    bool PauseTask( const UInt32 taskID ,
                    const bool force    );

    bool ResumeTask( const UInt32 taskID );

    bool RequestTaskToStop( const UInt32 taskID );

    bool KillTask( const UInt32 taskID );

    void SetNrOfThreadsToLogicalCPUs( const UInt32 coreFactor );

    void SetNrOfThreads( const UInt32 nrOfThreads );

    UInt32 GetNrOfThreads( void ) const;

    void RegisterTaskConsumerFactory( const CString& taskType       ,
                                      TTaskConsumerFactory* factory );

    void UnregisterTaskConsumerFactory( const CString& taskType );

    virtual const CString& GetClassTypeName( void ) const;

    bool GetTaskIdForThreadId( const UInt32 threadId ,
                               UInt32& taskId        ) const;

    bool GetThreadIdForTaskId( const UInt32 taskId ,
                               UInt32& threadId    ) const;

    protected:

    virtual void LockData( void ) const;

    virtual void UnlockData( void ) const;

    virtual void OnNotify( CNotifier* notifier           ,
                           const CEvent& eventid         ,
                           CICloneable* eventdata = NULL );

    private:
    friend class CTaskDelegator;

    void RegisterTaskDelegator( CTaskDelegator& delegator );

    void UnregisterTaskDelegator( CTaskDelegator& delegator );

    bool GetQueuedTask( CTaskConsumer** taskConsumer ,
                        CICloneable** taskData       );

    void TaskCleanup( CTaskConsumer* taskConsumer ,
                      CICloneable* taskData       );

    private:
    friend class CTaskConsumer;

    void RegisterTaskConsumer( CTaskConsumer& consumer        ,
                               CTaskConsumer::TTaskId& taskId );

    void UnregisterTaskConsumer( CTaskConsumer& consumer        ,
                                 CTaskConsumer::TTaskId& taskId );

    private:

    class CTaskQueueItem : public CICloneable
    {
        public:

        CTaskQueueItem( CTaskConsumer* consumer ,
                        CICloneable* taskData   );

        CTaskQueueItem( CTaskQueueItem& src );

        virtual ~CTaskQueueItem();

        CTaskConsumer* GetTaskConsumer( void );

        CICloneable* GetTaskData( void );

        virtual CICloneable* Clone( void ) const;

        private:
        CICloneable* m_taskData;
        CTaskConsumer* m_taskConsumer;
    };

    private:
    friend class CGUCEFCOREModule;

    static void Deinstance( void );

    private:

    void EnforceDesiredNrOfThreads( UInt32 desiredNrOfThreads ,
                                    bool gracefullEnforcement );

    void RemoveConsumerFromQueue( CTaskConsumer* consumer );

    CTaskManager( void );
    CTaskManager( const CTaskManager& src );
    CTaskManager& operator=( const CTaskManager& src );
    virtual ~CTaskManager();

    private:

    typedef CTAbstractFactory< CString, CTaskConsumer > TAbstractTaskConsumerFactory;
    typedef MT::CTMailBox< CString > TTaskMailbox;
    typedef std::map< UInt32, CTaskConsumer* > TTaskConsumerMap;
    typedef std::set< CTaskDelegator* > TTaskDelegatorSet;
    typedef CTaskConsumer::TTaskIdGenerator TTaskIdGenerator;

    TAbstractTaskConsumerFactory m_consumerFactory;
    UInt32 m_desiredNrOfThreads;
    UInt32 m_activeNrOfThreads;
    TTaskMailbox m_taskQueue;
    TTaskIdGenerator m_taskIdGenerator;
    TTaskConsumerMap m_taskConsumerMap;
    TTaskDelegatorSet m_taskDelegators;
    MT::CMutex g_mutex;

    static CTaskManager* g_instance;
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
