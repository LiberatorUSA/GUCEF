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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <assert.h>

#include "CTransactionEventControl.h"

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */

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
 *  Internally used data container class used for mailing
 *  extra data using the CMailBox class.
 */
class CMailElement : public CICloneable
{
    private:
    friend class CTransactionEventControl;
    
    CMailElement( CNotifier* notifier , 
                  CICloneable* eventdata )
        : m_notifier( notifier )  ,
          m_eventdata( eventdata )                 
    {
    
    }
    
    CMailElement( const CMailElement& src )
        : m_notifier( src.m_notifier )  ,
          m_eventdata( src.m_eventdata )       
    {
        
    }
    
    virtual ~CMailElement()    
    {
    }
    
    CMailElement& operator=( const CMailElement& src )
    {
        if ( this != &src )
        {
            m_notifier = src.m_notifier;
            m_eventdata = src.m_eventdata;    
        }
        return *this;    
    }
    
    inline CNotifier* GetNotifier( void ) const
    {
        return m_notifier;
    }
    
    inline CICloneable* GetData( void ) const
    {
        return m_eventdata;
    }
    
    virtual CICloneable* Clone( void ) const
    {
        return new CMailElement( *this );
    }
    
    private:
    
    CMailElement( void )
    {
        GUCEF_UNREACHABLE;
        /* dummy, do not use */
    }
    
    CICloneable* m_eventdata;
    CNotifier* m_notifier;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CTransactionEventControl::CTransactionEventControl( void )
    : CObservingNotifier()       ,
      m_mutex()                  ,
      m_transactionBusy( false ) ,
      m_mailbox()                ,
      m_filterMap()              ,
      m_senderFilterMap()
{
}

/*-------------------------------------------------------------------------*/

CTransactionEventControl::~CTransactionEventControl()
{TRACE;
    
    ClearAllFilters();
}

/*-------------------------------------------------------------------------*/

void
CTransactionEventControl::ClearAllFilters( void )
{TRACE;

    m_mutex.Lock();
    m_filterMap.clear();
    m_senderFilterMap.clear();
    m_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CTransactionEventControl::StartTransaction( void )
{TRACE;

    m_mutex.Lock();
    m_transactionBusy = true;
    m_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/
    
void
CTransactionEventControl::FinishTransaction( void )
{TRACE;

    m_mutex.Lock();
    
    CEvent eventID;
    CICloneable* eventData;
    while ( m_mailbox.GetMail( eventID    ,
                               &eventData ) )
    {
        NotifyObservers( eventID, eventData );
    }
    
    m_transactionBusy = false;
    m_mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

bool
CTransactionEventControl::IsTransactionActive( void ) const
{TRACE;

    return m_transactionBusy;
}

/*-------------------------------------------------------------------------*/
    
bool
CTransactionEventControl::AddEventFilter( const CEvent& eventID                 ,
                                          const TEventFilterAction filterAction ,
                                          const TEventOriginFilter originFilter ,
                                          const bool preserveEventData          ,
                                          const CNotifier* sender /* = NULL */  )
{TRACE;
    
    m_mutex.Lock();
    
    if ( !IsEventFiltered( eventID ,
                           sender  ) )
    {
    if ( NULL != sender )
    {
            TFilterData& filterdata = (m_senderFilterMap[ sender ])[ eventID ];
        filterdata.filterAction = filterAction;
        filterdata.originFilter = originFilter;
        filterdata.preserveEventData = preserveEventData;
    }
    else
    {
            TFilterData& filterdata = m_filterMap[ eventID ];
        filterdata.filterAction = filterAction;
        filterdata.originFilter = originFilter;
        filterdata.preserveEventData = preserveEventData;
    }
        
    m_mutex.Unlock();
        return true;
}

    m_mutex.Unlock();
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CTransactionEventControl::RemoveEventFilter( const CEvent& eventID                ,
                                             const CNotifier* sender /* = NULL */ )
{TRACE;

    bool retValue = false;

    m_mutex.Lock();
    if ( NULL != sender )
    {
        TSenderFilterMap::iterator i = m_senderFilterMap.find( sender );
        if ( i != m_senderFilterMap.end() )
        {
            TFilterMap& filterMap = (*i).second;
            TFilterMap::iterator n = filterMap.find( eventID );
            if ( n != filterMap.end() )
            {
                filterMap.erase( n );
            }
        }
    }
    else
    {
        TFilterMap::iterator i = m_filterMap.find( eventID );
        if ( i != m_filterMap.end() )
        {
            m_filterMap.erase( i );
        }
    }
    m_mutex.Unlock();
    
    return retValue;
}

/*-------------------------------------------------------------------------*/
                         
bool
CTransactionEventControl::IsEventFiltered( const CEvent& eventID   ,
                                           const CNotifier* sender ) const
{TRACE;

    bool retValue = false;

    m_mutex.Lock();
    if ( NULL != sender )
    {
        TSenderFilterMap::const_iterator i = m_senderFilterMap.find( sender );
        if ( i != m_senderFilterMap.end() )
        {
            const TFilterMap& filterMap = (*i).second;
            TFilterMap::const_iterator n = filterMap.find( eventID );
            if ( n != filterMap.end() )
            {
                retValue = true;
            }
        }
    }
    else
    {
        TFilterMap::const_iterator i = m_filterMap.find( eventID );
        if ( i != m_filterMap.end() )
        {
            retValue = true;
        }
    }
    m_mutex.Unlock();
    
    return retValue;
}

/*-------------------------------------------------------------------------*/

bool
CTransactionEventControl::GetFilterInfo( const CEvent& eventID            ,
                                         const CNotifier* sender          ,
                                         TEventFilterAction& filterAction ,
                                         TEventOriginFilter& originFilter ,
                                         bool& preserveEventData          ) const
{TRACE;

    bool retValue = false;

    m_mutex.Lock();
    if ( NULL != sender )
    {
        TSenderFilterMap::const_iterator i = m_senderFilterMap.find( sender );
        if ( i != m_senderFilterMap.end() )
        {
            const TFilterMap& filterMap = (*i).second;
            TFilterMap::const_iterator n = filterMap.find( eventID );
            if ( n != filterMap.end() )
            {
                const TFilterData& filterdata = (*n).second;
                filterAction = filterdata.filterAction;
                originFilter = filterdata.originFilter;
                preserveEventData = filterdata.preserveEventData;
                
                retValue = true;
            }
        }
    }
    else
    {
        TFilterMap::const_iterator i = m_filterMap.find( eventID );
        if ( i != m_filterMap.end() )
        {
            const TFilterData& filterdata = (*i).second;
            filterAction = filterdata.filterAction;
            originFilter = filterdata.originFilter;
            preserveEventData = filterdata.preserveEventData;
            
            retValue = true;
        }
    }
    m_mutex.Unlock();
    
    return retValue;
}

/*-------------------------------------------------------------------------*/

bool
CTransactionEventControl::ApplyFilter( const TFilterMap& filterMap ,
                                       CNotifier* notifier         ,
                                       const CEvent& eventID       ,
                                       CICloneable* eventData      )
{TRACE;

    TFilterMap::const_iterator i = filterMap.find( eventID );
    if ( i != filterMap.end() )
    {
        const TFilterData& filterdata = (*i).second;
        
        // Apply the main action filter to the event
        switch ( filterdata.filterAction )
        {
            case EVENTFILTER_DELETE_EVENT :
            {
                // No need to do anything, the message will simply be dropped
                return true;
            }
            case EVENTFILTER_GROUP_EVENT :
            {
                // Clear any previous message of this event
                m_mailbox.Delete( eventID );
                
                // Apply the origin filter to the message
                CNotifier* sender = NULL;
                switch ( filterdata.originFilter )
                {
                    case EVENTORIGINFILTER_NO_SENDER :
                    {
                        sender = NULL;
                        break;
                    }
                    case EVENTORIGINFILTER_TRANSFER :
                    {
                        sender = this;
                        break;
                    }
                    case EVENTORIGINFILTER_UNMODIFIED : 
                    {
                        sender = notifier;
                    }
                    default:
                    {
                        GUCEF_UNREACHABLE;
                        return false;                        
                    }
                }
                
                // Add the message to the mailbox
                if ( filterdata.preserveEventData )
                {
                    // Add the latest version and preserve it's data
                    CMailElement mail( sender, eventData );
                    m_mailbox.AddMail( eventID, &mail );
                }
                else
                {
                    // Add the latest version and drop it's data
                    CMailElement mail( sender, NULL );
                    m_mailbox.AddMail( eventID, &mail );
                }
                
                // Filter applied successfully
                return true;
            }
            default:
            {
                GUCEF_UNREACHABLE;
                return false;
            }
        }        
    }    

    // No matching ID found in the given filter map
    return false;
}
                        
/*-------------------------------------------------------------------------*/

void
CTransactionEventControl::OnNotify( CNotifier* notifier                 ,
                                    const CEvent& eventID               ,
                                    CICloneable* eventData /* = NULL */ )
{TRACE;

    m_mutex.Lock();
    
    // First we attempt to handle the event using the event global filters
    if ( !ApplyFilter( m_filterMap, notifier, eventID, eventData ) )
    {
        // No success so far,.. now try the notifier-instance-specific-filters
        TSenderFilterMap::const_iterator i = m_senderFilterMap.find( notifier );
        if ( i != m_senderFilterMap.end() )
        {
            const TFilterMap& filterMap = (*i).second;
            if ( ApplyFilter( filterMap, notifier, eventID, eventData ) )
            {
                // Yay, it worked, we applied a filter to the event
                m_mutex.Unlock();
                return;
            }
        }
    }
    
    // The given eventID is unknown in any filter so we simply shove it into our
    // mailbox so it can be processed when the transaction is complete
    CMailElement mail( notifier, eventData );
    m_mailbox.AddMail( eventID, &mail );
                        
    m_mutex.Unlock();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
