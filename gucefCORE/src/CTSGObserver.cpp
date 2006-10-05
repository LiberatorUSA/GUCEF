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

#define GUCEF_CORE_CTSGOBSERVER_CPP
#include "CTSGObserver.h"

#include <cassert>
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

CTSGObserver::CTSGObserver( void )
    : m_parentNotifier( NULL )
{
}

/*-------------------------------------------------------------------------*/

CTSGObserver::CTSGObserver( const CTSGObserver& src )
    : m_parentNotifier( src.m_parentNotifier )
{
    assert( 0 );
    /* @TODO: makeme */
}

/*-------------------------------------------------------------------------*/

CTSGObserver::~CTSGObserver()
{
    UnsubscribeFromAll();
}

/*-------------------------------------------------------------------------*/

CTSGObserver&
CTSGObserver::operator=( const CTSGObserver& src )
{
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
{
    m_parentNotifier = parentNotifier;
}

/*-------------------------------------------------------------------------*/

void 
CTSGObserver::OnPumpedNotify( CNotifier* notifier                  ,
                              const UInt32 eventid                 ,
                              CICloneable* eventdata /* = NULL  */ )
{
    m_parentNotifier->OnPumpedNotify( notifier  ,
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
                        
