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

#define GUCEF_CORE_CTSGNOTIFIER_CPP
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
{
    m_tsgObserver.SetParent( this );
}

/*-------------------------------------------------------------------------*/

CTSGNotifier::CTSGNotifier( const CTSGNotifier& src )
    : m_tsgObserver()
{
    m_tsgObserver.SetParent( this );
    
    assert(0);
    /* @TODO: makeme */
}

/*-------------------------------------------------------------------------*/

CTSGNotifier::~CTSGNotifier()
{
}

/*-------------------------------------------------------------------------*/

CTSGNotifier&
CTSGNotifier::operator=( const CTSGNotifier& src )
{
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
{
    threadedNotifier->Subscribe( &m_tsgObserver );
}

/*-------------------------------------------------------------------------*/
    
void 
CTSGNotifier::UnsubscribeFrom( CNotifier* threadedNotifier )
{
    threadedNotifier->Unsubscribe( &m_tsgObserver );
}

/*-------------------------------------------------------------------------*/

void 
CTSGNotifier::SubscribeTo( CNotifier* threadedNotifier ,
                           const UInt32 eventid        )
{
    threadedNotifier->Subscribe( &m_tsgObserver ,
                                 eventid        );
}

/*-------------------------------------------------------------------------*/                           
                      
void 
CTSGNotifier::UnsubscribeFrom( CNotifier* threadedNotifier ,
                               const UInt32 eventid        )
{
    threadedNotifier->Unsubscribe( &m_tsgObserver ,
                                   eventid        );
}                           

/*-------------------------------------------------------------------------*/

void 
CTSGNotifier::OnPumpedNotify( CNotifier* notifier                 ,
                              const UInt32 eventid                ,
                              CICloneable* eventdata /* = NULL */ )
{
    NotifyObservers( eventid   ,
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
