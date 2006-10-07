/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
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
#include "CObserverPump.h"
#include "CICloneable.h"

#include "CPumpedObserver.h"

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
    friend class CPumpedObserver;
    
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
        assert( 0 );
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

CPumpedObserver::CPumpedObserver( void )
{
    CObserverPump::Instance()->RegisterObserver( this );
}

/*-------------------------------------------------------------------------*/

CPumpedObserver::CPumpedObserver( const CPumpedObserver& src )
{
    CObserverPump::Instance()->RegisterObserver( this );
}

/*-------------------------------------------------------------------------*/

CPumpedObserver::~CPumpedObserver()
{
    CObserverPump::Instance()->UnregisterObserver( this );
}

/*-------------------------------------------------------------------------*/

CPumpedObserver&
CPumpedObserver::operator=( const CPumpedObserver& src )
{
    if ( this != &src )
    {
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

void 
CPumpedObserver::OnNotify( CNotifier* notifier                 ,
                           const CEvent& eventid               ,
                           CICloneable* eventdata /* = NULL */ )
{
    if ( eventdata )
    {
        eventdata = eventdata->Clone();
    }

    CMailElement maildata( notifier  ,  
                           eventdata );

    m_mailbox.AddMail( eventid   ,
                       &maildata );
}

/*-------------------------------------------------------------------------*/

void 
CPumpedObserver::OnUpdate( void )
{
    CEvent eventid;
    CICloneable* dataptr( NULL );
    CMailElement* maildata( NULL );
    while ( m_mailbox.GetMail( eventid  ,
                               &dataptr ) )
    {        
        maildata = static_cast< CMailElement* >( dataptr );
        OnPumpedNotify( maildata->GetNotifier() ,
                        eventid                 ,
                        maildata->GetData()     );
                        
        delete maildata->GetData();
        delete maildata;                        
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/