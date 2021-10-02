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

#ifndef GUCEF_COMCORE_CPUBSUBBOOKMARK_H
#include "gucefCOMCORE_CPubSubBookmark.h"
#define GUCEF_COMCORE_CPUBSUBBOOKMARK_H
#endif /* GUCEF_COMCORE_CPUBSUBBOOKMARK_H ? */

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

    typedef CIPubSubMsg::TIPubSubMsgConstRawPtrVector               TIPubSubMsgConstRawPtrVector;
    typedef CIPubSubMsg::TIPubSubMsgRawPtrVector                    TIPubSubMsgRawPtrVector;
    typedef CORE::CTLinkedCloneable< CIPubSubMsg >                  TPubSubMsgRef;
    typedef std::vector< TPubSubMsgRef >                            TPubSubMsgsRefVector;
    typedef CORE::CTCloneableObj< TPubSubMsgsRefVector >            TMsgsRecievedEventData;
    typedef CORE::CTCloneableExpansion< std::vector< UInt64 > >     TPublishActionIdVector;


    static const CORE::CEvent ConnectedEvent;
    static const CORE::CEvent DisconnectedEvent;
    static const CORE::CEvent ConnectionErrorEvent;
    static const CORE::CEvent MsgsRecievedEvent;                /**< event msg sent when the backend has received 1-N messages from the pubsub system as a subscriber */
    static const CORE::CEvent MsgsPublishedEvent;               /**< event msg sent when the backend has successfully published messages */
    static const CORE::CEvent MsgsPublishFailureEvent;          /**< event msg sent when the backend has failed to successfully publish messages */
    static const CORE::CEvent LocalPublishQueueFullEvent;       /**< if the backend supports queuing messages locally then we have to deal with the possibility of said queue reaching a max capacity */
    static const CORE::CEvent PublishThrottleEvent;             /**< if you overload the backend system as a published you may receive a throttle event msg. In such a case you should back off your publish rate if possible */

    typedef CORE::CTLinkedCloneable< TPublishActionIdVector >       TMsgsPublishFailureEventData;
    typedef CORE::CTLinkedCloneable< TPublishActionIdVector >       TMsgsPublishedEventData;

    static void RegisterEvents( void );

    CPubSubClientTopic( void );

    virtual ~CPubSubClientTopic();

    /**
     *  Explicitly calls for initialization of connectivity if relevant for the given backend
     *  The intent is to allow the application to choose when to take the setup cost hit and also provide better error reporting
     *  Backends should be implement such that if no explicit call to InitializeConnectivity() is made and such a call is relevant that 
     *  lazy initialization is used upon subscribe/publish
     */
    virtual bool InitializeConnectivity( void ) = 0;

    virtual bool Subscribe( void ) = 0;                                                /**< attempt to commence subscription. This will subscribe per config and defaults of the given backend */
    virtual bool SubscribeStartingAtBookmark( const CPubSubBookmark& bookmark ) = 0;   /**< attempt to commence subscription starting at the given client-side provided bookmark */

    virtual CPubSubBookmark GetCurrentBookmark( void ) = 0;

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
    virtual bool Publish( TPublishActionIdVector& publishActionIds, const CBasicPubSubMsg::TBasicPubSubMsgVector& msgs );
    virtual bool Publish( TPublishActionIdVector& publishActionIds, const TIPubSubMsgConstRawPtrVector& msgs );
    virtual bool Publish( TPublishActionIdVector& publishActionIds, const TIPubSubMsgRawPtrVector& msgs );
    virtual bool Publish( TPublishActionIdVector& publishActionIds, const TPubSubMsgsRefVector& msgs );
    virtual bool Publish( UInt64& publishActionId, const CORE::CString& msgId, const CORE::CString& key, const CORE::CString& value );
    virtual bool Publish( UInt64& publishActionId, const CORE::CString& msgId, const CORE::CDynamicBuffer& payload );
    virtual bool Publish( UInt64& publishActionId, const CORE::CString& msgId, const CORE::CString& payload );    
    virtual bool Publish( UInt64& publishActionId, const CORE::CVariant& msgId, const CORE::CVariant& payload );
    virtual bool Publish( UInt64& publishActionId, const CORE::CString& msgId, const CORE::CString& key, const CORE::CDynamicBuffer& value );
    virtual bool Publish( UInt64& publishActionId, const CORE::CString& msgId, const CORE::CDynamicBuffer& key, const CORE::CDynamicBuffer& value );
    virtual bool Publish( UInt64& publishActionId, const CORE::CVariant& msgId, const CORE::CDynamicBuffer& key, const CORE::CDynamicBuffer& value );
    virtual bool Publish( UInt64& publishActionId, const CORE::CString& msgId, const CORE::CValueList& kvPairs );    

    /**
     *  Publish the given message to the pub-sub backend system using that system's specifics
     *  If the backend does not support publishing messages this operation will always fail
     *
     *  @return success flag with true on success per backend criterea or false on failure
     */
    virtual bool Publish( UInt64& publishActionId, const CIPubSubMsg& msg ) = 0;

    /**
     *  Ack that the given message was successfully received and/or handled by the application
     *  If the backend does not support ack'ing messages this operation will always fail
     *
     *  @return success flag with true on success per backend criterea or false on failure     
     */    
    virtual bool AcknowledgeReceipt( const CIPubSubMsg& msg );
    virtual bool AcknowledgeReceipt( const CPubSubBookmark& bookmark );
    virtual bool AcknowledgeReceipt( const UInt64 consumeActionId );

    /**
     *  If supported by the backend implementation this will set the given bookmark
     *  to information that act for said bookmark indicates locality of this message 
     */
    virtual bool DeriveBookmarkFromMsg( const CIPubSubMsg& msg, CPubSubBookmark& bookmark ) const;
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

