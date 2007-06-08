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

#include <cassert>
#include "CTSGNotifier.h"

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#include "CTSGObserver.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF { 
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CTSGObserver::CTSGObserver( void )
    : m_parentNotifier( NULL )
{TRACE;

}

/*-------------------------------------------------------------------------*/

CTSGObserver::CTSGObserver( const CTSGObserver& src )
    : m_parentNotifier( src.m_parentNotifier )
{TRACE;

    assert( 0 );
    /* @TODO: makeme */
}

/*-------------------------------------------------------------------------*/

CTSGObserver::~CTSGObserver()
{TRACE;

    UnsubscribeFromAll();
}

/*-------------------------------------------------------------------------*/

CTSGObserver&
CTSGObserver::operator=( const CTSGObserver& src )
{TRACE;

    assert( 0 );
    /* @TODO: makeme */

    if ( this != &src )
    {
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

void 
CTSGObserver::SetParent( CTSGNotifier* parentNotifier )
{TRACE;

    m_parentNotifier = parentNotifier;
}

/*-------------------------------------------------------------------------*/

void 
CTSGObserver::OnNotify( CNotifier* notifier                  ,
                        const CEvent& eventid                ,
                        CICloneable* eventdata /* = NULL  */ )
{TRACE;

    if ( m_parentNotifier != NULL )
    {
        CPumpedObserver::OnNotify( notifier   ,
                                   eventid    ,
                                   eventdata  );        
    }
}

/*-------------------------------------------------------------------------*/

void 
CTSGObserver::OnPumpedNotify( CNotifier* notifier                  ,
                              const CEvent& eventid                ,
                              CICloneable* eventdata /* = NULL  */ )
{TRACE;

    // Make sure we still have a parent
    if ( m_parentNotifier != NULL )
    {
        // Pass on the message
        m_parentNotifier->OnPumpedNotify( notifier  ,
                                          eventid   ,
                                          eventdata );
    }
}

/*-------------------------------------------------------------------------*/

void
CTSGObserver::AddEventToMailbox( CNotifier* notifier                 ,
                                 const CEvent& eventid               ,
                                 CICloneable* eventdata /* = NULL */ )
{TRACE;

    OnNotify( notifier  ,
              eventid   ,
              eventdata );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
                        
