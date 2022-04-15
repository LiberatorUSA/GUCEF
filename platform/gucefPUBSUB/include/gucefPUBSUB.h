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

/*
 *      This is the main include header for the gucefPUBSUB library.
 *      It is part of a framework called GUCEF.
 *      All classes in this library have to do utilizing a pub-sub communication concept
 */

#ifndef GUCEF_PUBSUB_H
#define GUCEF_PUBSUB_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Library build configuration      
 */
#ifndef GUCEF_PUBSUB_CONFIG_H
#include "gucefPUBSUB_config.h"
#define GUCEF_PUBSUB_CONFIG_H
#endif /* GUCEF_PUBSUB_CONFIG_H ? */

/*
 *      Library specific macros and generic macros
 */
#ifndef GUCEF_PUBSUB_MACROS_H
#include "gucefPUBSUB_macros.h"
#define GUCEF_PUBSUB_MACROS_H
#endif /* GUCEF_PUBSUB_MACROS_H ? */

/*
 *      Simple types
 */
#ifndef GUCEF_PUBSUB_ETYPES_H
#include "gucefPUBSUB_ETypes.h"
#define GUCEF_PUBSUB_ETYPES_H
#endif /* GUCEF_PUBSUB_ETYPES_H ? */

/*---------------------------------------------------------------------------/

        C++ includes

/---------------------------------------------------------------------------*/
#ifdef __cplusplus

#ifndef GUCEF_PUBSUB_CPUBSUBGLOBAL_H
#include "gucefPUBSUB_CPubSubGlobal.h"
#define GUCEF_PUBSUB_CPUBSUBGLOBAL_H
#endif /* GUCEF_PUBSUB_CPUBSUBGLOBAL_H ? */

#ifndef GUCEF_PUBSUB_CIPUBSUBMSG_H
#include "gucefPUBSUB_CIPubSubMsg.h"
#define GUCEF_PUBSUB_CIPUBSUBMSG_H
#endif /* GUCEF_PUBSUB_CIPUBSUBMSG_H ? */

#ifndef GUCEF_PUBSUB_CBASICPUBSUBMSG_H
#include "gucefPUBSUB_CBasicPubSubMsg.h"
#define GUCEF_PUBSUB_CBASICPUBSUBMSG_H
#endif /* GUCEF_PUBSUB_CBASICPUBSUBMSG_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBMSGSERIALIZER_H
#include "gucefPUBSUB_CPubSubMsgSerializer.h"
#define GUCEF_PUBSUB_CPUBSUBMSGSERIALIZER_H
#endif /* GUCEF_PUBSUB_CPUBSUBMSGSERIALIZER_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBMSGBINARYSERIALIZER_H
#include "gucefPUBSUB_CPubSubMsgBinarySerializer.h"
#define GUCEF_PUBSUB_CPUBSUBMSGBINARYSERIALIZER_H
#endif /* GUCEF_PUBSUB_CPUBSUBMSGBINARYSERIALIZER_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBMSGCONTAINERBINARYSERIALIZER_H
#include "gucefPUBSUB_CPubSubMsgContainerBinarySerializer.h"
#define GUCEF_PUBSUB_CPUBSUBMSGCONTAINERBINARYSERIALIZER_H
#endif /* GUCEF_PUBSUB_CPUBSUBMSGCONTAINERBINARYSERIALIZER_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBBOOKMARK_H
#include "gucefPUBSUB_CPubSubBookmark.h"
#define GUCEF_PUBSUB_CPUBSUBBOOKMARK_H
#endif /* GUCEF_PUBSUB_CPUBSUBBOOKMARK_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBCLIENTFEATURES_H
#include "gucefPUBSUB_CPubSubClientFeatures.h"    
#define GUCEF_PUBSUB_CPUBSUBCLIENTFEATURES_H
#endif /* GUCEF_PUBSUB_CPUBSUBCLIENTFEATURES_H ? */

#ifndef GUCEF_PUBSUB_CPUBSUBCLIENTTOPIC_H
#include "gucefPUBSUB_CPubSubClientTopic.h"    
#define GUCEF_PUBSUB_CPUBSUBCLIENTTOPIC_H
#endif /* GUCEF_PUBSUB_CPUBSUBCLIENTTOPIC_H ? */


#endif /* __cplusplus ? */
/*--------------------------------------------------------------------------*/

#endif /* GUCEF_PUBSUB_H ? */
