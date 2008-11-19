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
#include "gucefMT_CTMailbox.h"
#define GUCEF_MT_CTMAILBOX_H
#endif /* GUCEF_MT_CTMAILBOX_H ? */

#ifndef GUCEF_CORE_COBSERVER_H
#include "CObserver.h"
#define GUCEF_CORE_COBSERVER_H
#endif /* GUCEF_CORE_COBSERVER_H ? */

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

class GUCEF_CORE_EXPORT_CPP CTaskManager : public CObserver
{
    public:
    
    typedef CTFactoryBase< CTaskConsumer > TTaskConsumerFactory;
    
    static CTaskManager* Instance( void );
    
    void QueueTask( const CString& taskType ,
                    CICloneable* taskData   );

    bool StartTask( const CString& taskType ,
                    CICloneable* taskData   ,
                    UInt32* taskID = NULL   );
                    
    bool PauseTask( const UInt32 taskID );
    
    bool ResumeTask( const UInt32 taskID );
    
    bool RequestTaskToStop( const UInt32 taskID );
    
    bool KillTask( const UInt32 taskID );
    
    void SetNrOfThreadsToLogicalCPUs( const UInt32 coreFactor );    
    
    void SetNrOfThreads( const UInt32 nrOfThreads );
    
    UInt32 GetNrOfThreads( void ) const;
    
    void RegisterTaskConsumerFactory( const CString& taskType       ,
                                      TTaskConsumerFactory* factory );
                                      
    void UnregisterTaskConsumerFactory( const CString& taskType );

    protected:
    
    virtual void OnNotify( CNotifier* notifier           ,
                           const CEvent& eventid         ,
                           CICloneable* eventdata = NULL );

    private:
    friend class CTaskDelegator;
    
    bool GetQueuedTask( CTaskConsumer** taskConsumer ,
                        CICloneable** taskData       );

    void TaskCleanup( CTaskConsumer* taskConsumer ,
                      CICloneable* taskData       );

    void FlagTaskAsActive( CTaskDelegator& task );
    
    void FlagTaskAsNonActive( CTaskDelegator& task );

    private:
    friend class CGUCEFCOREModule;
    
    static void Deinstance( void );

    private:
    
    void EnforceDesiredNrOfThreads( void );
    
    CTaskManager( void );
    CTaskManager( const CTaskManager& src );
    CTaskManager& operator=( const CTaskManager& src );
    virtual ~CTaskManager();
    
    private:
    
    typedef CTAbstractFactory< CString, CTaskConsumer > TAbstractTaskConsumerFactory;
    typedef std::map< UInt32, CTaskDelegator* > TTaskMap;
    typedef std::pair< UInt32, CTaskDelegator* > TTaskEntry;
    
    TAbstractTaskConsumerFactory m_consumerFactory;
    TTaskMap m_activeTasks;
    TTaskMap m_nonactiveTasks;
    UInt32 m_desiredNrOfThreads;
    MT::CTMailBox< CString > m_taskQueue;
    static MT::CMutex g_mutex;
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
