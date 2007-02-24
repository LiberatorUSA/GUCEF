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

#include <string.h>             /* needed for memset() */

#ifndef CACTIVECOMPUMP_H
#include "CActiveComPump.h"     /* threaded com poller */
#define CACTIVECOMPUMP_H
#endif /* CACTIVECOMPUMP_H ? */

#ifndef DVWINSOCK_H
#include "dvwinsock.h"          /* socket API for ms windows */
#define DVWINSOCK_H
#endif /* DVWINSOCK_H ? */

#ifndef CSOCKET_H
#include "CSocket.h"            /* base class for all sockets */
#define CSOCKET_H
#endif /* CSOCKET_H ? */  

#include "CCom.h"               /* definition of CCom class */

#ifdef ACTIVATE_MEMORY_MANAGER
  #ifndef GUCEF_NEW_ON_H
  #include "gucef_new_on.h"   /* Use the GUCEF memory manager instead of the standard manager ? */
  #define GUCEF_NEW_ON_H
  #endif /* GUCEF_NEW_ON_H ? */
#endif /* ACTIVATE_MEMORY_MANAGER ? */ 

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Define indicating the amount by which the heap is to be resized if there
 *      is insufficient room on the heap or reduce the size of the heap if
 *      sufficient entry's have been deleted.
 */
#define HEAP_RESIZE_AMOUNT	5

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

CCom* CCom::_instance = NULL;
MT::CMutex CCom::_mutex;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CCom*
CCom::Instance( void )
{TRACE;
        _mutex.Lock();
        if ( !_instance ) 
        {
                _instance = new CCom();                          
        }
        _mutex.Unlock();
        return _instance;
}

/*-------------------------------------------------------------------------*/

void 
CCom::Deinstance( void )
{TRACE;
        _mutex.Lock();
        delete _instance;
        _instance = NULL;
        _mutex.Unlock();                
}

/*-------------------------------------------------------------------------*/

CCom::CCom()
        : CGUCEFAppSubSystem( true )           ,
          _keep_gstats( false )                ,
          _scount( 0 )                         ,
          _pumpthread( false )                 ,
          _threadedpump( NULL )   
{TRACE;
        _sockets.SetResizeChange( HEAP_RESIZE_AMOUNT );
        memset( &_stats, 0, sizeof(TSocketStats) );   
}

/*-------------------------------------------------------------------------*/

CCom::~CCom()
{TRACE;

        SetPumpThreading( false );                     
}

/*-------------------------------------------------------------------------*/

/**
 *	A new socket object has been created and wishes to be registered.
 *	This registration allows the socket object to get periodic updates
 *      for polling reasons
 */
void                       
CCom::RegisterSocketObject( CSocket* socket )
{TRACE;

        _mutex.Lock();
        socket->SetSocketID( _sockets.AddEntry( socket ) );        
        ++_scount;
        _mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CCom::UnregisterSocketObject( const CSocket* socket )
{TRACE;
        _mutex.Lock();
        _sockets.SetEntry( socket->GetSocketID(), NULL );        
        --_scount;
        _mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

void 
CCom::OnUpdate( const UInt64 tickcount               ,
                const Float64 updateDeltaInMilliSecs )
{TRACE;    

        _mutex.Lock();
        for ( Int32 i=0; i <= _sockets.GetLast(); ++i )
        {
                if ( _sockets[ i ] )
                {
                        ( (CSocket*)_sockets[ i ] )->Update();
                }
        }
        _mutex.Unlock();   
}              

/*-------------------------------------------------------------------------*/

UInt32 
CCom::GetSocketCount( void ) const
{TRACE;

        return _scount;
}

/*-------------------------------------------------------------------------*/

void 
CCom::SetUseGlobalStats( bool keep_gstats )
{TRACE;

        _keep_gstats = keep_gstats;
}

/*-------------------------------------------------------------------------*/
        
bool 
CCom::GetUseGlobalStats( void ) const
{TRACE;

        return _keep_gstats;
}

/*-------------------------------------------------------------------------*/
        
void 
CCom::ResetGlobalStats( void )
{TRACE;
}

/*-------------------------------------------------------------------------*/
        
const CCom::TSocketStats& 
CCom::GetGlobalStats( void ) const
{TRACE;

        return _stats;
}

/*-------------------------------------------------------------------------*/

void 
CCom::SetPumpThreading( bool thread )
{TRACE;
        _mutex.Lock();
        
        if ( _pumpthread == thread )
        {
                _mutex.Unlock();
                return;
        } 
        
        if ( thread )
        {
                _pumpthread = true;
                _threadedpump = new CActiveComPump();
                _threadedpump->Activate();
                _mutex.Unlock();
                return;
        }
        
        _threadedpump->Deactivate( true );
        delete _threadedpump;
        _threadedpump = NULL;
        _pumpthread = false;
        _mutex.Unlock();
}        

/*-------------------------------------------------------------------------*/

bool 
CCom::GetPumpThreading( void ) const
{TRACE;

        return _pumpthread;        
}

/*-------------------------------------------------------------------------*/

bool
CCom::SetSystemWideProxyServer( const CORE::CString& protocol ,
                                const bool active             )
{TRACE;
    
    TProxyList::iterator i = m_proxyList.find( protocol );
    if ( i != m_proxyList.end() )
    {
        (*i).second.active = active;
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CCom::SetSystemWideProxyServer( const CORE::CString& protocol   ,
                                const CORE::CString& remoteHost ,
                                const UInt16 remotePort         ,
                                const bool active               )
{TRACE;
    
    TProxyServer& proxyServer = m_proxyList[ protocol ];
    proxyServer.host = remoteHost;
    proxyServer.port = remotePort;
    proxyServer.active = active;
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CCom::GetSystemWideProxyServer( const CORE::CString& protocol ,
                                CORE::CString& remoteHost     ,
                                UInt16& remotePort            ,
                                bool& active                  ) const
{TRACE;

    TProxyList::const_iterator i = m_proxyList.find( protocol );
    if ( i != m_proxyList.end() )
    {
        const TProxyServer& proxyServer = (*i).second;
        remoteHost = proxyServer.host;
        remotePort = proxyServer.port;
        active = proxyServer.active;
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CCom::IsSystemWideProxyServerActive( const CORE::CString& protocol ) const
{TRACE;

    TProxyList::const_iterator i = m_proxyList.find( protocol );
    if ( i != m_proxyList.end() )
    {
        return (*i).second.active;
    }
    return false;    
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
