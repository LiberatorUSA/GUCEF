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

#ifndef GUCEF_PUBSUB_CPUBSUBJOURNALCONFIG_H
#define GUCEF_PUBSUB_CPUBSUBJOURNALCONFIG_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "gucefCORE_CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_CORE_CURI_H
#include "gucefCORE_CUri.h"
#define GUCEF_CORE_CURI_H
#endif /* GUCEF_CORE_CURI_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_PUBSUB_MACROS_H
#include "gucefPUBSUB_macros.h"    
#define GUCEF_PUBSUB_MACROS_H
#endif /* GUCEF_PUBSUB_MACROS_H ? */

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
 *  Class providing construction configuration options for pub-sub clients
 */
class GUCEF_PUBSUB_EXPORT_CPP CPubSubJournalConfig : public CORE::CIConfigurable
{
    public:

    bool useJournal;                                /**< whether to use a journal or not */
    CORE::CString journalType;                      /**< the string form type name of the pub-sub journal */
    CORE::CUri journalResource;                     /**< where to find the journal resource */

    CPubSubJournalConfig( void );

    CPubSubJournalConfig( const CPubSubJournalConfig& src );

    virtual ~CPubSubJournalConfig();

    CPubSubJournalConfig& operator=( const CPubSubJournalConfig& src );

    /**
     *      @param cfg the data node structure where you'd like to serialize the config to
     *      @return wheter storing the config was successfull
     */
    virtual bool SaveConfig( CORE::CDataNode& cfg ) const GUCEF_VIRTUAL_OVERRIDE;

    /**
     *      @param cfg pertinent node in the config document from which to load the config
     *      @return success or failure to load all required settings correctly from the given config
     */
    virtual bool LoadConfig( const CORE::CDataNode& cfg ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PUBSUB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PUBSUB_CPUBSUBJOURNALCONFIG_H ? */

