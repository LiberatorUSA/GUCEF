/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL, 
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER 
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF 
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT 
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
 */

#ifndef GUCEF_CORE_COBSERVER_H
#define GUCEF_CORE_COBSERVER_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <vector>
#include "gucefCORE_macros.h"
#include "CICloneable.h"
#include "gucefCORE_ETypes.h"

#ifndef GUCEF_CORE_CEVENT_H
#include "CEvent.h"
#define GUCEF_CORE_CEVENT_H
#endif /* GUCEF_CORE_CEVENT_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF { 
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CNotifier;
class CNotifierImplementor;

/*-------------------------------------------------------------------------*/

/**
 *  Implementation of the observation component in the notifier-observer 
 *  design pattern.
 *
 *  Note that you should unsubscribe the decending class before deallocating it.
 *  This can be done by calling UnsubscribeAll() from the destructor of your 
 *  decending class. Failure to do this will not cause any problems but should be 
 *  considdered to be dirty coding.
 */
class GUCEFCORE_EXPORT_CPP CObserver
{
    public:

    CObserver( void );
    
    CObserver( const CObserver& src );
    
    virtual ~CObserver();
    
    CObserver& operator=( const CObserver& src );
    
    void SubscribeTo( CNotifier* notifier );
    
    void SubscribeTo( CNotifier* notifier   ,
                      const CEvent& eventid );
                               
    /**
     *  Unsubscribes the observer from all notifiers.
     */
    void UnsubscribeFromAll( void );
    
    /**
     *  Detaches from the given notifier.
     *  All the observers subscriptions will be cancelled
     *  This includes both standard notifier events aswell 
     *  as custom events.
     */    
    void UnsubscribeFrom( CNotifier& notifier );    
    
    UInt32 GetSubscriptionCount( void );

    UInt32 GetNotifierCount( void ) const;

    protected:
    friend class CNotifierImplementor;
    
    /**
     *  Event callback member function.
     *  Implement this in your decending class to handle
     *  notification events.
     *
     *  @param notifier the notifier that sent the notification
     *  @param eventid the unique event id for an event
     *  @param eventdata optional notifier defined userdata
     */
    virtual void OnNotify( CNotifier* notifier           ,
                           const CEvent& eventid         ,
                           CICloneable* eventdata = NULL ) = 0;

    virtual void LockData( void );
    
    virtual void UnlockData( void );

    private:
    friend class CNotifierImplementor;
    
    /*
     *  Simply updates the observer administration, nothing more
     */
    void LinkTo( CNotifier* notifier );
                     
    /*
     *  Simply updates the observer administration, nothing more
     */    
    void UnlinkFrom( CNotifier* notifier             ,
                     const bool forAllEvents = false );    
                           
    private:
    struct SNotifierRef
    {
        CNotifier* notifier;
        UInt32 refCount;    
    };
    typedef struct SNotifierRef TNotifierRef;
    typedef std::vector<TNotifierRef> TNotifierList;

    TNotifierList m_notifiers;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_COBSERVER_H ? */
