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

#ifndef GUCEF_CORE_CGUCEFAPPLICATION_H
#include "CGUCEFApplication.h"
#define GUCEF_CORE_CGUCEFAPPLICATION_H
#endif /* GUCEF_CORE_CGUCEFAPPLICATION_H ? */

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#include "CTSGNotifier.h"

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

CTSGNotifier::CTSGNotifier( void )
    : CNotifier()                    ,
      m_tsgObserver( CGUCEFApplication::Instance()->GetPulseGenerator() )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CTSGNotifier::CTSGNotifier( CPulseGenerator& pulsGenerator )
    : CNotifier()                    ,
      m_tsgObserver( pulsGenerator )
{GUCEF_TRACE;

    m_tsgObserver.SetParent( this );
}

/*-------------------------------------------------------------------------*/

CTSGNotifier::CTSGNotifier( const CTSGNotifier& src )
    : CNotifier( src )                   ,
      m_tsgObserver( src.m_tsgObserver )
{GUCEF_TRACE;

    m_tsgObserver.SetParent( this );
}

/*-------------------------------------------------------------------------*/

CTSGNotifier::~CTSGNotifier()
{GUCEF_TRACE;
    
    m_tsgObserver.SetParent( NULL );
}

/*-------------------------------------------------------------------------*/

CTSGNotifier&
CTSGNotifier::operator=( const CTSGNotifier& src )
{GUCEF_TRACE;

    return *this;
}

/*-------------------------------------------------------------------------*/

void 
CTSGNotifier::SubscribeTo( CNotifier* threadedNotifier )
{GUCEF_TRACE;

    threadedNotifier->Subscribe( &m_tsgObserver );
}

/*-------------------------------------------------------------------------*/
    
void 
CTSGNotifier::UnsubscribeFrom( CNotifier* threadedNotifier )
{GUCEF_TRACE;

    threadedNotifier->Unsubscribe( &m_tsgObserver );
}

/*-------------------------------------------------------------------------*/

void 
CTSGNotifier::SubscribeTo( CNotifier* threadedNotifier ,
                           const CEvent& eventid       )
{GUCEF_TRACE;

    threadedNotifier->Subscribe( &m_tsgObserver ,
                                 eventid        );
}

/*-------------------------------------------------------------------------*/                           
                      
void 
CTSGNotifier::UnsubscribeFrom( CNotifier* threadedNotifier ,
                               const CEvent& eventid       )
{GUCEF_TRACE;

    threadedNotifier->Unsubscribe( &m_tsgObserver ,
                                   eventid        );
}                           

/*-------------------------------------------------------------------------*/

void 
CTSGNotifier::OnPumpedNotify( CNotifier* notifier                 ,
                              const CEvent& eventid               ,
                              CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;

    NotifyObservers( eventid   ,
                     eventdata );       
}                              

/*-------------------------------------------------------------------------*/

void
CTSGNotifier::NotifyObserversFromThread( void )
{GUCEF_TRACE;

    m_tsgObserver.AddEventToMailbox( this                   ,
                                     CNotifier::ModifyEvent ,
                                     NULL                   );
}

/*-------------------------------------------------------------------------*/
    
void
CTSGNotifier::NotifyObserversFromThread( const CEvent& eventid               ,
                                         CICloneable* eventData /* = NULL */ )
{GUCEF_TRACE;
    
    m_tsgObserver.AddEventToMailbox( this      ,
                                     eventid   ,
                                     eventData );
}

/*-------------------------------------------------------------------------*/

const CString&
CTSGNotifier::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CString typeName = "GUCEF::CORE::CTSGNotifier";
    return typeName;
}

/*-------------------------------------------------------------------------*/

void
CTSGNotifier::LockData( void ) const
{GUCEF_TRACE;
    
    m_tsgObserver.DoLockData();
}

/*-------------------------------------------------------------------------*/
    
void
CTSGNotifier::UnlockData( void ) const
{GUCEF_TRACE;
    
    m_tsgObserver.DoUnlockData();
}

/*-------------------------------------------------------------------------*/

CObserver&
CTSGNotifier::AsObserver( void )
{GUCEF_TRACE;

    return m_tsgObserver;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
