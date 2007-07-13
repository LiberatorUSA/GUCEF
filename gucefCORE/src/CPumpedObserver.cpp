/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
 */
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <assert.h>
#include "CObserverPump.h"
#include "CICloneable.h"

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

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
    {GUCEF_TRACE;
    
    }
    
    CMailElement( const CMailElement& src )
        : m_notifier( src.m_notifier )  ,
          m_eventdata( src.m_eventdata )       
    {GUCEF_TRACE;
        
    }
    
    virtual ~CMailElement()    
    {GUCEF_TRACE;
    
    }
    
    CMailElement& operator=( const CMailElement& src )
    {GUCEF_TRACE;
    
        if ( this != &src )
        {
            m_notifier = src.m_notifier;
            m_eventdata = src.m_eventdata;    
        }
        return *this;    
    }
    
    inline CNotifier* GetNotifier( void ) const
    {GUCEF_TRACE;
    
        return m_notifier;
    }
    
    inline CICloneable* GetData( void ) const
    {GUCEF_TRACE;
    
        return m_eventdata;
    }
    
    virtual CICloneable* Clone( void ) const
    {GUCEF_TRACE;
    
        return new CMailElement( *this );
    }
    
    private:
    
    CMailElement( void )
    {GUCEF_TRACE;
    
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
{GUCEF_TRACE;

    CObserverPump::Instance()->RegisterObserver( this );
}

/*-------------------------------------------------------------------------*/

CPumpedObserver::CPumpedObserver( const CPumpedObserver& src )
{GUCEF_TRACE;

    CObserverPump::Instance()->RegisterObserver( this );
}

/*-------------------------------------------------------------------------*/

CPumpedObserver::~CPumpedObserver()
{GUCEF_TRACE;

    CObserverPump::Instance()->UnregisterObserver( this );
}

/*-------------------------------------------------------------------------*/

CPumpedObserver&
CPumpedObserver::operator=( const CPumpedObserver& src )
{GUCEF_TRACE;

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
{GUCEF_TRACE;

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
{GUCEF_TRACE;

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