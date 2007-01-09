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
    : m_tsgObserver()
{TRACE;

    m_tsgObserver.SetParent( this );
}

/*-------------------------------------------------------------------------*/

CTSGNotifier::CTSGNotifier( const CTSGNotifier& src )
    : m_tsgObserver()
{TRACE;

    m_tsgObserver.SetParent( this );
    
    assert(0);
    /* @TODO: makeme */
}

/*-------------------------------------------------------------------------*/

CTSGNotifier::~CTSGNotifier()
{TRACE;
    
    m_tsgObserver.SetParent( NULL );
}

/*-------------------------------------------------------------------------*/

CTSGNotifier&
CTSGNotifier::operator=( const CTSGNotifier& src )
{TRACE;

    assert(0);
    /* @TODO: makeme */    

    if ( this != &src )
    {
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

void 
CTSGNotifier::SubscribeTo( CNotifier* threadedNotifier )
{TRACE;

    threadedNotifier->Subscribe( &m_tsgObserver );
}

/*-------------------------------------------------------------------------*/
    
void 
CTSGNotifier::UnsubscribeFrom( CNotifier* threadedNotifier )
{TRACE;

    threadedNotifier->Unsubscribe( &m_tsgObserver );
}

/*-------------------------------------------------------------------------*/

void 
CTSGNotifier::SubscribeTo( CNotifier* threadedNotifier ,
                           const CEvent& eventid       )
{TRACE;

    threadedNotifier->Subscribe( &m_tsgObserver ,
                                 eventid        );
}

/*-------------------------------------------------------------------------*/                           
                      
void 
CTSGNotifier::UnsubscribeFrom( CNotifier* threadedNotifier ,
                               const CEvent& eventid       )
{TRACE;

    threadedNotifier->Unsubscribe( &m_tsgObserver ,
                                   eventid        );
}                           

/*-------------------------------------------------------------------------*/

void 
CTSGNotifier::OnPumpedNotify( CNotifier* notifier                 ,
                              const CEvent& eventid               ,
                              CICloneable* eventdata /* = NULL */ )
{TRACE;

    NotifyObservers( eventid   ,
                     eventdata );       
}                              

/*-------------------------------------------------------------------------*/

void
CTSGNotifier::NotifyObserversFromThread( void )
{TRACE;

    m_tsgObserver.AddEventToMailbox( this                   ,
                                     CNotifier::ModifyEvent ,
                                     NULL                   );
}

/*-------------------------------------------------------------------------*/
    
void
CTSGNotifier::NotifyObserversFromThread( const CEvent& eventid               ,
                                         CICloneable* eventData /* = NULL */ )
{TRACE;
    
    m_tsgObserver.AddEventToMailbox( this      ,
                                     eventid   ,
                                     eventData );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
