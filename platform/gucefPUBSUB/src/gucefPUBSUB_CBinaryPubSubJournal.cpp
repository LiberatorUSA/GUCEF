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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "gucefPUBSUB_CBinaryPubSubJournal.h"    

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

const CORE::CString CBinaryPubSubJournal::JournalType = "binary";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CBinaryPubSubJournal::CBinaryPubSubJournal( void )
    : CIPubSubJournal()
    , m_journal( GUCEF_NULL )
{GUCEF_TRACE;
    
}

/*-------------------------------------------------------------------------*/

CBinaryPubSubJournal::CBinaryPubSubJournal( const CPubSubJournalConfig& config )
    : CIPubSubJournal()
    , m_journal( GUCEF_NULL )   
{GUCEF_TRACE;
    
}

/*-------------------------------------------------------------------------*/

CBinaryPubSubJournal::~CBinaryPubSubJournal() 
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CBinaryPubSubJournal::ReadJournalEntry( UInt64& eventTimestamp   ,
                                       TPubSubActionType& action ,
                                       UInt64& msgActionId       )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_journal )
    {
        if ( m_journal->ReadValue( eventTimestamp ) )
            if ( m_journal->ReadValue( action ) )
                if ( m_journal->ReadValue( msgActionId ) )
                    return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CBinaryPubSubJournal::AddJournalEntry( UInt64 eventTimestamp    ,
                                       TPubSubActionType action ,
                                       UInt64 msgActionId       )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_journal )
    {
        if ( m_journal->WriteValue( eventTimestamp ) )
            if ( m_journal->WriteValue( action ) )
                if ( m_journal->WriteValue( msgActionId ) )
                    return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

void 
CBinaryPubSubJournal::SetJournalResource( CORE::CIOAccess* journal )
{GUCEF_TRACE;

    m_journal = journal;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PUBSUB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
