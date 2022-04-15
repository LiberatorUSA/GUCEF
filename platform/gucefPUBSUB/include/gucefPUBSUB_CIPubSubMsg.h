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

#ifndef GUCEF_COMCORE_CIPUBSUBMSG_H
#define GUCEF_COMCORE_CIPUBSUBMSG_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_COMCORE_CIMESSAGE_H
#include "gucefCOMCORE_CIMessage.h"
#define GUCEF_COMCORE_CIMESSAGE_H
#endif /* GUCEF_COMCORE_CIMESSAGE_H ? */

#ifndef GUCEF_COMCORE_CPUBSUBBOOKMARK_H
#include "gucefPUBSUB_CPubSubBookmark.h"
#define GUCEF_COMCORE_CPUBSUBBOOKMARK_H
#endif /* GUCEF_COMCORE_CPUBSUBBOOKMARK_H ? */

#ifndef GUCEF_COMCORE_MACROS_H
#include "gucefPUBSUB_macros.h"    
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

class CPubSubClientTopic;

/**
 *  Class providing a generic representation of a pub-sub message
 *  Based on the client used different capabilities will be available and 
 *  client will use beft effort to simulate others
 */
class GUCEF_COMCORE_EXPORT_CPP CIPubSubMsg : public CIMessage
{
    public:

    typedef std::vector< CIPubSubMsg* >                     TIPubSubMsgRawPtrVector;
    typedef std::vector< const CIPubSubMsg* >               TIPubSubMsgConstRawPtrVector;
    typedef CORE::CTSharedPtr< CIPubSubMsg, MT::CNoLock >   TNoLockSharedPtr;
    typedef std::vector< TNoLockSharedPtr >                 TIPubSubMsgSPtrVector;

    CIPubSubMsg( void );

    CIPubSubMsg( const CIPubSubMsg& src );

    virtual ~CIPubSubMsg() GUCEF_VIRTUAL_OVERRIDE;

    /**
     *  For a message that was received by a pub-sub topic this would provide
     *  a link back to the topic where the message originated via a subscription
     *
     *  For a message constructed by the application for publishing this can be left 
     *  as GUCEF_NULL
     */ 
    virtual CPubSubClientTopic* GetOriginClientTopic( void ) const = 0;

    /**
     *  Receive Action ID is a backend runtime unique ID used for referencing
     *  the message run-instance-uniquely to said backend without having to retain said message
     *  Generic Pubsub system requirement, not backend specific
     *
     *  Note that if this mesage is constructed application side and not the result
     *  of a subscription the value must be set to 0 which is the mandatory default value
     */ 
    virtual void SetReceiveActionId( UInt64 receiveActionId ) = 0;

    /**
     *  Receive Action ID is a backend runtime unique ID used for referencing
     *  the message run-instance-uniquely to said backend without having to retain said message
     *  Generic Pubsub system requirement, not backend specific
     *
     *  Note that if this mesage is constructed application side and not the result
     *  of a subscription the value must be set to 0 which is the mandatory default value
     */
    virtual UInt64 GetReceiveActionId( void ) const = 0;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

inline const COMCORE::CIPubSubMsg* AsIPubSubMsgPointer( const COMCORE::CIPubSubMsg& msg ) { return &msg; }
inline const COMCORE::CIPubSubMsg* AsIPubSubMsgPointer( const COMCORE::CIPubSubMsg* msg ) { return msg; }
inline COMCORE::CIPubSubMsg* AsIPubSubMsgPointer( COMCORE::CIPubSubMsg& msg ) { return &msg; }
inline COMCORE::CIPubSubMsg* AsIPubSubMsgPointer( COMCORE::CIPubSubMsg* msg ) { return msg; }

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COMCORE_CIPUBSUBMSG_H ? */
