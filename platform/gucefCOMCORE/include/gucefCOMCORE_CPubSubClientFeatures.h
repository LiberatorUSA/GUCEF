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

class GUCEF_COMCORE_EXPORT_CPP CPubSubClientFeatures
{
    public:

    bool supportsPublishing;                /** not every client may support full pub-sub: Is publishing supported? */
    bool supportsSubscribing;               /** not every client may support full pub-sub: Is subscribing supported? */
    bool supportsAutoReconnect;
    
    bool supportsBinaryPayloads;            /** if not supported and a binary payload is given it will be converted to base64 */
    bool supportsMultiHostSharding;         /** if the pub-sub system supports sharding across multiple hosts */
    bool supportsMsgKeysPerTopic;           /** whether per message keys are supported inside a topic, if not the msgKey param is irrelevant and not used */
    bool supportsKeyValuePerMsg;
    bool supportsDuplicateKeysPerMsg;
    
    bool supportsMetrics;


    CPubSubClientFeatures( void );
    ~CPubSubClientFeatures();
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

