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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CEVENTPUMP_H
#include "CEventPump.h"
#define GUCEF_CORE_CEVENTPUMP_H
#endif /* GUCEF_CORE_CEVENTPUMP_H ? */

#include "CEventPumpClient.h"

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */ 

#pragma warning( disable : 4100 ) /* unreferenced formal parameter */

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

CEventPumpClient::CEventPumpClient( void )
{TRACE;
        _pump = _pump->Instance();
        _clientid = _pump->RegisterClient( this );
}

/*-------------------------------------------------------------------------*/

CEventPumpClient::CEventPumpClient( const CEventPumpClient& other )
{TRACE;
        _pump = _pump->Instance();
        _clientid = _pump->RegisterClient( this );
}

/*-------------------------------------------------------------------------*/

CEventPumpClient::~CEventPumpClient()
{TRACE;
        _pump->UnregisterClient( _clientid );
}

/*-------------------------------------------------------------------------*/

CEventPumpClient&
CEventPumpClient::operator=( const CEventPumpClient& other )
{TRACE;
        return *this;
}

/*-------------------------------------------------------------------------*/

void 
CEventPumpClient::OnProcessEvent( const CEvent& event )
{TRACE;
        /* dummy to avoid making implementation manditory */
}

/*-------------------------------------------------------------------------*/  

void 
CEventPumpClient::OnUpdate( UInt32 tickcount  ,
                            UInt32 deltaticks )
{TRACE;
        /* dummy to avoid making implementation manditory */
}   

/*-------------------------------------------------------------------------*/                     

bool
CEventPumpClient::SendEvent( CEvent& event )
{TRACE;
        event.SetPumpClientID( _clientid );
        return _pump->SendEvent( event );
}

/*-------------------------------------------------------------------------*/

bool 
CEventPumpClient::SendEvent( UInt32 eventid )
{TRACE;
        CEvent event( eventid );
        event.SetPumpClientID( _clientid );
        return _pump->SendEvent( event );               
}

/*-------------------------------------------------------------------------*/

void
CEventPumpClient::SetPumpClientID( UInt32 clientid )
{TRACE;
        _clientid = clientid;
}

/*-------------------------------------------------------------------------*/

UInt32
CEventPumpClient::GetPumpClientID( void ) const
{TRACE;
        return _clientid;                
}

/*-------------------------------------------------------------------------*/

bool
CEventPumpClient::SendEventAndLockMailbox( CEvent& event )
{TRACE;
        event.SetPumpClientID( _clientid );
        return _pump->SendEventAndLockMailbox( event );
}

/*-------------------------------------------------------------------------*/

bool 
CEventPumpClient::SendEventAndLockMailbox( const UInt32 eventid )
{TRACE;
        CEvent event( eventid );
        event.SetPumpClientID( _clientid );
        return _pump->SendEventAndLockMailbox( event );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
