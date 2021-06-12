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

#ifndef GUCEF_COMCORE_CPUBSUBCLIENTTOPIC_H
#define GUCEF_COMCORE_CPUBSUBCLIENTTOPIC_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

#ifndef GUCEF_CORE_CTSHAREDPTR_H
#include "CTSharedPtr.h"
#define GUCEF_CORE_CTSHAREDPTR_H
#endif /* GUCEF_CORE_CTSHAREDPTR_H ? */

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_CORE_CVALUELIST_H
#include "CValueList.h"
#define GUCEF_CORE_CVALUELIST_H
#endif /* GUCEF_CORE_CVALUELIST_H ? */

#ifndef GUCEF_COMCORE_CIPUBSUBMSG_H
#include "gucefCOMCORE_CIPubSubMsg.h"
#define GUCEF_COMCORE_CIPUBSUBMSG_H
#endif /* GUCEF_COMCORE_CIPUBSUBMSG_H ? */

#ifndef GUCEF_COMCORE_CBASICPUBSUBMSG_H
#include "gucefCOMCORE_CBasicPubSubMsg.h"
#define GUCEF_COMCORE_CBASICPUBSUBMSG_H
#endif /* GUCEF_COMCORE_CBASICPUBSUBMSG_H ? */

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

/**
 *  Class providing the abstract client-side representation of a 'topic' within the publish-subscribe system
 *
 *  A topic is the logical representation of that to which one can publish and subscribe.
 *  The actual nomenclature would depend on the underlying implementation but the pub-sub concept should apply. 
 */
class GUCEF_COMCORE_EXPORT_CPP CPubSubClientTopic : public CORE::CObservingNotifier
{
    public:

    static const CORE::CEvent ConnectedEvent;
    static const CORE::CEvent DisconnectedEvent;
    static const CORE::CEvent ConnectionErrorEvent;
    static const CORE::CEvent MsgsRecievedEvent;

    static void RegisterEvents( void );

    typedef CORE::CTLinkedCloneable< CIPubSubMsg >          TPubSubMsgRef;
    typedef std::vector< TPubSubMsgRef >                    TPubSubMsgsRefVector;
    typedef CORE::CTCloneableObj< TPubSubMsgsRefVector >    TMsgsRecievedEventData;

    CPubSubClientTopic( void );

    virtual ~CPubSubClientTopic();

    virtual bool InitializeConnectivity( void ) = 0;

    virtual bool Subscribe( void ) = 0;
    virtual bool SubscribeStartingAtMsgId( const CORE::CVariant& msgIdBookmark ) = 0;
    virtual bool SubscribeStartingAtMsgDateTime( const CORE::CDateTime& msgDtBookmark ) = 0;

    virtual bool Disconnect( void ) = 0;

    virtual bool IsConnected( void ) = 0;

    virtual bool IsPublishingSupported( void ) = 0;

    virtual bool IsSubscribingSupported( void ) = 0;

    virtual const CORE::CString& GetTopicName( void ) const = 0;

    /**
     *  Utility/convenience member functions that allow common calling patterns
     *  Note that these all call Publish( const CIPubSubMsg& msg )
     *  Specific backend implementations may optionally override these if there is a gain from doing so
     */
    virtual bool Publish( const CBasicPubSubMsg::TBasicPubSubMsgVector& msgs );
    virtual bool Publish( const CORE::CString& msgId, const CORE::CString& key, const CORE::CString& value );
    virtual bool Publish( const CORE::CString& msgId, const CORE::CDynamicBuffer& payload );
    virtual bool Publish( const CORE::CString& msgId, const CORE::CString& payload );    
    virtual bool Publish( const CORE::CVariant& msgId, const CORE::CVariant& payload );
    virtual bool Publish( const CORE::CString& msgId, const CORE::CString& key, const CORE::CDynamicBuffer& value );
    virtual bool Publish( const CORE::CString& msgId, const CORE::CDynamicBuffer& key, const CORE::CDynamicBuffer& value );
    virtual bool Publish( const CORE::CVariant& msgId, const CORE::CDynamicBuffer& key, const CORE::CDynamicBuffer& value );
    virtual bool Publish( const CORE::CString& msgId, const CORE::CValueList& kvPairs );    

    virtual bool Publish( const CIPubSubMsg& msg ) = 0;

};

typedef CORE::CTSharedPtr< CPubSubClientTopic, MT::CMutex > CPubSubClientTopicPtr;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COMCORE_CPUBSUBCLIENTTOPIC_H ? */

