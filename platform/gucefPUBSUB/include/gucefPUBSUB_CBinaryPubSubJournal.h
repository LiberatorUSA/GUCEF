/*
 *  gucefPUBSUB: GUCEF module providing pub-sub communication facilities
 *
 *  Copyright (C) 1998 - 2022.  Dinand Vanvelzen
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

#ifndef GUCEF_PUBSUB_CBINARYPUBSUBJOURNAL_H
#define GUCEF_PUBSUB_CBINARYPUBSUBJOURNAL_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_CORE_CIOACCESS_H
#include "CIOAccess.h"
#define GUCEF_CORE_CIOACCESS_H
#endif /* GUCEF_CORE_CIOACCESS_H ? */

#ifndef GUCEF_PUBSUB_CIPUBSUBJOURNAL_H
#include "gucefPUBSUB_CIPubSubJournal.h"    
#define GUCEF_PUBSUB_CIPUBSUBJOURNAL_H
#endif /* GUCEF_PUBSUB_CIPUBSUBJOURNAL_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBJOURNALCONFIG_H
#include "gucefPUBSUB_CPubSubJournalConfig.h"    
#define GUCEF_PUBSUB_CPUBSUBJOURNALCONFIG_H
#endif /* GUCEF_PUBSUB_CPUBSUBJOURNALCONFIG_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Class providing implementing a binary pub-sub messaging journal to trace what happened
 *  This differs from a regular log in that the more specific nature allows performance optimizations to be applied
 *  and as such allows per message tracing without the overhead of a regular log
 */
class GUCEF_PUBSUB_EXPORT_CPP CBinaryPubSubJournal : public CIPubSubJournal
{
    public:

    static const CORE::CString JournalType;

    CBinaryPubSubJournal( void );

    CBinaryPubSubJournal( const CPubSubJournalConfig& config );

    virtual ~CBinaryPubSubJournal();

    virtual bool ReadJournalEntry( UInt64& eventTimestamp    ,
                                   TPubSubActionType& action ,
                                   UInt64& msgActionId       ) GUCEF_VIRTUAL_OVERRIDE;

    /**
     *
     */
    virtual bool AddJournalEntry( UInt64 eventTimestamp    ,
                                  TPubSubActionType action ,
                                  UInt64 msgActionId       ) GUCEF_VIRTUAL_OVERRIDE;


    void SetJournalResource( CORE::IOAccessPtr journal );

    bool LoadConfig( const CPubSubJournalConfig& config );

    private:


    
    private:

    CORE::IOAccessPtr m_journal; /**< the journal file */
    CPubSubJournalConfig m_config;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PUBSUB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PUBSUB_CBINARYPUBSUBJOURNAL_H ? */
