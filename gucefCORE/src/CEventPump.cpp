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

#include "CEventPump.h"

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "dvoswrap.h"           /* needed for GetAppTickCount */
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H ? */ 

#ifndef GUCEF_CORE_CEVENTPUMPCLIENT_H
#include "CEventPumpClient.h"
#define GUCEF_CORE_CEVENTPUMPCLIENT_H
#endif /* GUCEF_CORE_CEVENTPUMPCLIENT_H ? */

#ifndef GUCEF_MT_CMUTEX_H
#include "CMutex.h"
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

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
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

CEventPump* CEventPump::_instance = NULL;
MT::CMutex CEventPump::_mutex;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CEventPump*
CEventPump::Instance( void )
{TRACE;
        _mutex.Lock();
        if ( !_instance )
        {
                _instance = new CEventPump();
                CHECKMEM( _instance, sizeof(CEventPump) );                               
        }
        _mutex.Unlock();
        return _instance;
}

/*-------------------------------------------------------------------------*/

#pragma warning( push )
#pragma warning( disable : 4100 ) /* unreferenced formal parameter */  
CEventPump::CEventPump( const CEventPump& src )
{TRACE;
        /* dummy implementation */        
}
#pragma warning( pop )

/*-------------------------------------------------------------------------*/

#pragma warning( push )
#pragma warning( disable : 4100 ) /* unreferenced formal parameter */  
CEventPump&
CEventPump::operator=( CEventPump& src )
{TRACE;

        return *this;       
}
#pragma warning( pop )

/*-------------------------------------------------------------------------*/

CEventPump::CEventPump( void )
        : _lastupdate( GUCEFGetTickCount() ) ,
          m_enabled( true )                  ,
          m_mailLocked( false )              ,
          m_updateClients( true )
{TRACE;

}

/*-------------------------------------------------------------------------*/

CEventPump::~CEventPump()
{TRACE;

}

/*-------------------------------------------------------------------------*/

void
CEventPump::Update( void )
{TRACE;
        _mutex.Lock();        
        if ( m_enabled )
        {
                CEvent event;
                while ( _eventstack.GetTop( event ) )
                {
                        DEBUGOUTPUTss( "Dispatching event ", event.GetTypeName().C_String() );
                
                        for ( Int32 i=0; i<=_clients.GetLast(); ++i )
                        {
                                if ( _clients[ i ] )
                                {
                                        ( (CEventPumpClient*)_clients[ i ] )->OnProcessEvent( event );
                                }
                                if ( !m_enabled )
                                {
                                        _mutex.Unlock();
                                        return;
                                }
                        }
                }
                
                if ( m_updateClients )
                {
                        UInt32 tickcount( GUCEFGetTickCount() );
                        UInt32 deltaticks( tickcount - _lastupdate );
                        if ( deltaticks == 0 )
                        {      //@FIXME: quick hack,.. delta can be 0 :/
                                deltaticks++;
                        }
                        for ( UInt32 i=0; i<_clients.GetCount(); ++i )
                        {
                                if ( _clients[ i ] )
                                {
                                        ( (CEventPumpClient*)_clients[ i ] )->OnUpdate( tickcount   ,
                                                                                        deltaticks  );
                                }
                                if ( !m_enabled )
                                {
                                        _lastupdate = tickcount;
                                        _mutex.Unlock();
                                        return;
                                        
                                }
                        }  
                        _lastupdate = tickcount;
                }                        
        }                       
        _mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

UInt32 
CEventPump::GetLastUpdateTickCount( void ) const
{TRACE;
    return _lastupdate;
}

/*-------------------------------------------------------------------------*/

UInt32
CEventPump::RegisterClient( CEventPumpClient* newclient )
{TRACE;
        _mutex.Lock();
        UInt32 id = _clients.AddEntry( newclient );
        newclient->SetPumpClientID( id );
        _mutex.Unlock();
        return id;
}

/*-------------------------------------------------------------------------*/

void
CEventPump::UnregisterClient( UInt32 clientid )
{TRACE;
        _mutex.Lock();
        _clients.SetEntry( clientid ,
                           NULL     );
        _mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

bool
CEventPump::SendEvent( const CEvent& event )
{TRACE;
        _mutex.Lock();
        if ( !m_mailLocked )
        {
                _eventstack.PushEvent( event );
                _mutex.Unlock();
                return true;
        }
        _mutex.Unlock();
        return false;
}

/*-------------------------------------------------------------------------*/

UInt32
CEventPump::GetPumpClientCount( void )
{TRACE;
        _mutex.Lock();
        UInt32 count = _clients.GetNonNULLCount();
        _mutex.Unlock();
        return count;
}

/*-------------------------------------------------------------------------*/

void 
CEventPump::SetIsEnabled( const bool enabled )
{TRACE;
        m_enabled = enabled;
}

/*-------------------------------------------------------------------------*/

bool 
CEventPump::GetIsEnabled( void ) const
{TRACE;
        return m_enabled;
}

/*-------------------------------------------------------------------------*/

bool 
CEventPump::SendEventAndLockMailbox( const CEvent& event )
{TRACE;
        _mutex.Lock();
        if ( !m_mailLocked )
        {
                _eventstack.PushEvent( event );
                m_mailLocked = true;
                _mutex.Unlock(); 
                return true;
        }
        _mutex.Unlock();
        return false;               
}

/*-------------------------------------------------------------------------*/

void 
CEventPump::SetMailboxIsLocked( const bool locked )
{
        m_mailLocked = locked;
}

/*-------------------------------------------------------------------------*/
        
bool 
CEventPump::GetMailboxIsLocked( void ) const
{
        return m_mailLocked;
}

/*-------------------------------------------------------------------------*/

void 
CEventPump::SetPerformClientUpdates( const bool updateClients )
{
        m_updateClients = updateClients;
}

/*-------------------------------------------------------------------------*/
        
bool 
CEventPump::GetPerformClientUpdates( void ) const
{
        return m_updateClients;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
