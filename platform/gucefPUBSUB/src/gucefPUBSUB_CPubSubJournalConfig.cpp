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

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#include "gucefPUBSUB_CPubSubJournalConfig.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CPubSubJournalConfig::CPubSubJournalConfig( void )
    : CORE::CIConfigurable()
    , useJournal( false ) 
    , journalType()
    , journalResource()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubJournalConfig::CPubSubJournalConfig( const CPubSubJournalConfig& src )
    : CORE::CIConfigurable()
    , useJournal( src.useJournal )
    , journalType( src.journalType )
    , journalResource( src.journalResource )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubJournalConfig::~CPubSubJournalConfig()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPubSubJournalConfig&
CPubSubJournalConfig::operator=( const CPubSubJournalConfig& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        // copy the config elements
        useJournal = src.useJournal;
        journalType = src.journalType;
        journalResource = src.journalResource;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubJournalConfig::SaveConfig( CORE::CDataNode& cfg ) const
{GUCEF_TRACE;

    bool totalSuccess = true;

    totalSuccess = cfg.SetAttribute( "useJournal", useJournal ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "journalType", journalType ) && totalSuccess;
    totalSuccess = cfg.SetAttribute( "journalPath", journalResource ) && totalSuccess;

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CPubSubJournalConfig::LoadConfig( const CORE::CDataNode& cfg )
{GUCEF_TRACE;

    useJournal = cfg.GetAttributeValueOrChildValueByName( "useJournal", useJournal ).AsBool( useJournal, true );
    journalType = cfg.GetAttributeValueOrChildValueByName( "journalType", journalType ).AsString( journalType, true );
    journalResource = cfg.GetAttributeValueOrChildValueByName( "journalPath", journalResource.ToUriInStringForm() ).AsString( journalResource, true );


    return true;
}

/*-------------------------------------------------------------------------*/

const CString&
CPubSubJournalConfig::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static const CString classTypeName = "GUCEF::PUBSUB::CPubSubJournalConfig";
    return classTypeName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PUBSUB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
