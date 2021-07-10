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

#ifndef GUCEF_CORE_CTHREADPOOL_H
#include "gucefCORE_CThreadPool.h"
#define GUCEF_CORE_CTHREADPOOL_H
#endif /* GUCEF_CORE_CTHREADPOOL_H ? */

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

    typedef CThreadPool::TTaskConsumerFactory TTaskConsumerFactory;

    static const CString DefaultTreadPoolName; 

    ThreadPoolPtr GetThreadPool( const CString& threadPoolName = DefaultTreadPoolName ) const;

    ThreadPoolPtr GetOrCreateThreadPool( const CString& threadPoolName , 
                                         bool createIfNotExists = true );

    void RegisterTaskConsumerFactory( const CString& taskType       ,
                                      TTaskConsumerFactory* factory );

    void UnregisterTaskConsumerFactory( const CString& taskType );

    UInt32 GetGlobalNrOfThreads( void ) const;

    void RequestAllThreadsToStop( bool waitOnStop, bool acceptNewWork );

    virtual const CString& GetClassTypeName( void ) const  GUCEF_VIRTUAL_OVERRIDE;

    CTaskConsumerPtr CreateTaskConsumer( const CString& taskType );

    protected:

    virtual void OnPumpedNotify( CNotifier* notifier           ,
                                 const CEvent& eventid         ,
                                 CICloneable* eventdata = NULL ) GUCEF_VIRTUAL_OVERRIDE;

    private:
    friend class CTaskConsumer;

    void RegisterTaskConsumerId( CTaskConsumer::TTaskId& taskId );

    void UnregisterTaskConsumerId( CTaskConsumer::TTaskId& taskId );

    private:
    friend class CCoreGlobal;

    CTaskManager( void );

    virtual ~CTaskManager();

    private:

    void RemoveConsumer( UInt32 taskID );

    CTaskManager( const CTaskManager& src );            /**< not implemented */
    CTaskManager& operator=( const CTaskManager& src ); /**< not implemented */

    private:

    typedef std::map< CString, ThreadPoolPtr > ThreadPoolMap;
    typedef CTAbstractFactory< CString, CTaskConsumer, MT::CMutex > TAbstractTaskConsumerFactory;
    typedef CTaskConsumer::TTaskIdGenerator TTaskIdGenerator;

    TTaskIdGenerator m_taskIdGenerator;
    TAbstractTaskConsumerFactory m_consumerFactory;
    ThreadPoolMap m_threadPools;
    UInt32 m_desiredGlobalNrOfThreads;
    Int32 m_activeGlobalNrOfThreads;
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
