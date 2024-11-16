/*
 *  PubSubStorageTool: Tool to manipulate archives
 *
 *  Copyright (C) 1998 - 2024.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef PUBSUBSTORAGETOOL_H
#define PUBSUBSTORAGETOOL_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_CGUCEFAPPLICATION_H
#include "CGUCEFApplication.h"
#define GUCEF_CORE_CGUCEFAPPLICATION_H
#endif /* GUCEF_CORE_CGUCEFAPPLICATION_H ? */

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

#ifndef GUCEF_CORE_CVALUELIST_H
#include "CValueList.h"
#define GUCEF_CORE_CVALUELIST_H
#endif /* GUCEF_CORE_CVALUELIST_H ? */

#ifndef GUCEF_CORE_CEVENTAGGREGATETRIGGER_H
#include "gucefCORE_CEventAggregateTrigger.h"
#define GUCEF_CORE_CEVENTAGGREGATETRIGGER_H
#endif /* GUCEF_CORE_CEVENTAGGREGATETRIGGER_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBMSGSERIALIZER_H
#include "gucefPUBSUB_CPubSubMsgSerializer.h"
#define GUCEF_PUBSUB_CPUBSUBMSGSERIALIZER_H
#endif /* GUCEF_PUBSUB_CPUBSUBMSGSERIALIZER_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBMSGCONTAINERBINARYSERIALIZER_H
#include "gucefPUBSUB_CPubSubMsgContainerBinarySerializer.h"
#define GUCEF_PUBSUB_CPUBSUBMSGCONTAINERBINARYSERIALIZER_H
#endif /* GUCEF_PUBSUB_CPUBSUBMSGCONTAINERBINARYSERIALIZER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CTimeBoundContainer : public CORE::CTSharedObjCreator< CTimeBoundContainer, MT::CNoLock >
{
    public:

    typedef CORE::CTSharedObjCreator< CTimeBoundContainer, MT::CNoLock >::TSharedPtrType  TSharedPtrType;

    CORE::CDateTime minClampedDt;
    CORE::CDateTime maxClampedDt;
    CORE::CDateTime creationDt; 
    CORE::CString tempContainerPath;
    VFS::TBasicVfsResourcePtr resource;
    PUBSUB::CPubSubMsgContainerBinarySerializer::TMsgOffsetIndex index;
    CORE::CDateTime earliestMsgDt;
    CORE::CDateTime newestMsgDt;

    CTimeBoundContainer( void );
    virtual ~CTimeBoundContainer();
};

typedef CTimeBoundContainer::TSharedPtrType CTimeBoundContainerPtr;

/*-------------------------------------------------------------------------*/

class PubSubStorageTool : public CORE::CObservingNotifier
{
    public:

    typedef std::map< CORE::CDateTime, PUBSUB::CPubSubClientTopic::TPubSubMsgsRefVector >  TDateTimeMsgRefMap;
    typedef std::map< CORE::CDateTime, CTimeBoundContainerPtr >                            TDateTimeToTimeBoundContainerMap;

    PubSubStorageTool( void );
    virtual ~PubSubStorageTool();

    bool PerformWork( void );

    bool PerformReSplit( void );

    bool AddLocalPathAsVfsRoot( const CORE::CString& fsPath, CORE::CString& vfsPath  );

    bool DetermineStorageOptions( const CORE::CString::StringVector& files                     ,  
                                  PUBSUB::CPubSubMsgBinarySerializerOptions& atLeastOneOptions ,
                                  PUBSUB::CPubSubMsgBinarySerializerOptions& unanimousOptions  ,
                                  bool& sourceFilesAreHomogenous                               ) const;
    
    bool DetermineLargestFileSize( const CORE::CString::StringVector& files ,
                                   UInt64& fileSize                         ) const;

    bool CreateTargetContainerAtTimeBoundary( const CORE::CString& targetRootPath                                , 
                                              CORE::CDateTime::TDateTimeComponent timeBoundaryValue              ,
                                              const CORE::CDateTime& datetime                                    , 
                                              const PUBSUB::CPubSubMsgBinarySerializerOptions& atLeastOneOptions ,
                                              CTimeBoundContainerPtr& container                                  );
    
    bool GetOrCreateTargetContainerAtTimeBoundary( TDateTimeToTimeBoundContainerMap& allContainers                    ,
                                                   const CORE::CString& targetRootPath                                , 
                                                   CORE::CDateTime::TDateTimeComponent timeBoundary                   ,
                                                   const CORE::CDateTime& datetime                                    , 
                                                   const PUBSUB::CPubSubMsgBinarySerializerOptions& atLeastOneOptions ,
                                                   CTimeBoundContainerPtr& container                                  );

    void SetConsoleAccess( CORE::CPlatformNativeConsoleLogger& console );

    void SetParams( const CORE::CValueList& keyValueList );

    private:

    typedef CORE::CTEventHandlerFunctor< PubSubStorageTool > TEventCallback;

    void RegisterEventHandlers( void );
    
    void OnWorkStartTrigger( CORE::CNotifier* notifier    ,
                             const CORE::CEvent& eventId  ,
                             CORE::CICloneable* eventData );


    CORE::CEventAggregateTrigger m_workStartTrigger;
    CORE::CPlatformNativeConsoleLogger* m_console;
    CORE::CValueList m_params;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* PUBSUBSTORAGETOOL_H ? */

/*-------------------------------------------------------------------------*/
