/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
 *
 *  Copyright (C) 1998 - 2020.  Dinand Vanvelzen
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

#ifndef GUCEF_COMCORE_CPUBSUBCLIENTFEATURES_H
#define GUCEF_COMCORE_CPUBSUBCLIENTFEATURES_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_COMCORE_MACROS_H
#include "gucefCOMCORE_macros.h"      /* often used gucefCOMCORE macros */
#define GUCEF_COMCORE_MACROS_H
#endif /* GUCEF_COMCORE_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_COMCORE_EXPORT_CPP CPubSubClientFeatures : public CORE::CIConfigurable
{
    public:

    bool supportsPublishing;                /** not every client may support full pub-sub: Is publishing supported? */
    bool supportsSubscribing;               /** not every client may support full pub-sub: Is subscribing supported? */
    bool supportsAutoReconnect;
    
    bool supportsBinaryPayloads;            /** if not supported and a binary payload is given it will be converted to base64 */
    bool supportsMultiHostSharding;         /** if the pub-sub system supports sharding across multiple hosts */
    bool supportsPerMsgIds;                 /** whether per message IDs are supported inside a topic, if not the msgId param is irrelevant and not used */
    bool supportsPrimaryPayloadPerMsg;      /** whether the concept of a per message primary payload is supported. If the backend only supports key-value ideally this feature is not used by the app */
    bool supportsKeyValueSetPerMsg;         /** whether the concept of a per message set of key-value attributes is supported. If the backend only supports a primary payload ideally this feature is not used by the app */
    bool supportsDuplicateKeysPerMsg;       /** whether the concept of a per message set of key-value attributes has the constraint of keys being unique */
    bool supportsMetaDataKeyValueSetPerMsg; /** whether the concept of a per message set of key-value meta data attributes is supported */

    bool supportsBookmarkingConcept;        /** whether the concept of bookmarking where you last were in the pub-sub subscription is supported at all. The other bookmarking related flags are meaningless if this is false */
    bool supportsAutoBookmarking;           /** whether the implementation takes care of remembering where you are wrt pub-sub subscriber consumption without needing help */
    bool supportsMsgIdBasedBookmark;        /** whether the implementation can accept a MsgId to setup pub-sub subscriber consumption */
    bool supportsMsgDateTimeBasedBookmark;  /** whether the implementation can accept a message DateTime to setup pub-sub subscriber consumption */
    
    bool supportsMetrics;


    CPubSubClientFeatures( void );
    virtual ~CPubSubClientFeatures();

    /**
     *      Attempts to store the given tree in the file
     *      given according to the method of the codec metadata
     *
     *      @param tree the data tree you wish to store
     *      @return wheter storing the tree was successfull
     */
    virtual bool SaveConfig( CORE::CDataNode& tree ) const GUCEF_VIRTUAL_OVERRIDE;

    /**
     *      Attempts to load data from the given file to the
     *      root node given. The root data will be replaced
     *      and any children the node may already have will be deleted.
     *
     *      @param treeroot pointer to the node that is to act as root of the data tree
     *      @return whether building the tree from the given file was successfull.
     */
    virtual bool LoadConfig( const CORE::CDataNode& treeroot ) GUCEF_VIRTUAL_OVERRIDE;

    virtual const CString& GetClassTypeName( void ) const GUCEF_VIRTUAL_OVERRIDE;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COMCORE_CPUBSUBCLIENTFEATURES_H ? */

