/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2006.  All rights reserved.
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

#ifndef GUCEF_CORE_CTRANSACTIONEVENTCONTROL_H
#define GUCEF_CORE_CTRANSACTIONEVENTCONTROL_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CTMAILBOX_H
#include "gucefMT_CTMailbox.h"
#define GUCEF_MT_CTMAILBOX_H
#endif /* GUCEF_MT_CTMAILBOX_H ? */

#include "CObservingNotifier.h"
#include "gucefCORE_macros.h"

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

/**
 *  This is a building block that can be used internally by classes that handle and/or
 *  emit events.
 *  
 *  Sometimes a class performes actions on sub-system(s) that trigger events
 *  to which the class in question is actually subscribed. The intermediate events
 *  emitted might not be of any interest and/or should not be handled untill the
 *  transaction with the sub-system(s) is finished. This class can be your tool
 *  for solving those types of problems.
 *  Define your interactions with sub-system(s) as transactions and add filters
 *  to achieve the desired behaviour.
 */
class GUCEFCORE_EXPORT_CPP CTransactionEventControl : public CObservingNotifier
{
    public:
    
    typedef enum TEventFilterAction
    {
        EVENTFILTER_DELETE_EVENT ,   /**< mainly usefull if you listnen to all events of an object but want to exclude a few specific ones */
        EVENTFILTER_GROUP_EVENT      /**< this will group events of the same type as 1 event, updating to the latest occurance */
    };
    
    public:
    
    CTransactionEventControl( void );
    
    virtual ~CTransactionEventControl();    
    
    void StartTransaction( void );
    
    void FinishTransaction( void );
    
    bool AddEventFilter( const CEvent& eventID                 ,
                         const TEventFilterAction filterAction ,
                         const TEventOriginFilter originFilter ,
                         const bool preserveEventData          ,
                         const CNotifier* sender = NULL        );

    bool RemoveEventFilter( const CEvent& eventID           ,
                            const CNotifier* sender = NULL  );
                         
    bool IsEventFiltered( const CEvent& eventID   ,
                          const CNotifier* sender ) const;

    bool GetFilterInfo( const CEvent& eventID            ,
                        const CNotifier* sender          ,
                        TEventFilterAction& filterAction ,
                        TEventOriginFilter& originFilter ,
                        bool& preserveEventData          ) const;

    bool IsTransactionActive( void ) const;

    void ClearAllFilters( void );

    protected:
    
    virtual void OnNotify( CNotifier* notifier           ,
                           const CEvent& eventid         ,
                           CICloneable* eventdata = NULL );    

    private:
    
    struct SFilterData
    {
        TEventFilterAction filterAction;
        TEventOriginFilter originFilter;
        bool preserveEventData;
    };
    typedef struct SFilterData TFilterData;
    
    typedef std::map< CEvent, TFilterData > TFilterMap;
    typedef std::map< const CNotifier*, TFilterMap > TSenderFilterMap;

    private:

    bool ApplyFilter( const TFilterMap& filterMap ,
                      CNotifier* notifier         ,
                      const CEvent& eventID       ,
                      CICloneable* eventData      );

    CTransactionEventControl( const CTransactionEventControl& src );             /**< not implemented */
    CTransactionEventControl& operator=( const CTransactionEventControl& src );  /**< not implemented */

    private:
    
    TFilterMap m_filterMap;
    TSenderFilterMap m_senderFilterMap;
    MT::CTMailBox< CEvent > m_mailbox;
    MT::CMutex m_mutex;
    bool m_transactionBusy;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTRANSACTIONEVENTCONTROL_H ? */