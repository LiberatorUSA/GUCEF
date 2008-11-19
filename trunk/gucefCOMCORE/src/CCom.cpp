/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
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

#include <string.h>             /* needed for memset() */

#ifndef GUCEF_CORE_CLOGMANAGER_H
#include "CLogManager.h"
#define GUCEF_CORE_CLOGMANAGER_H
#endif /* GUCEF_CORE_CLOGMANAGER_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

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

#ifdef GUCEF_MSWIN_BUILD

bool
GetMSWinInternetProxyFromRegistry( CORE::CString& remoteHost ,
                                   UInt16& port              ,
                                   bool& active              )
{GUCEF_TRACE;

    char lszValue[ 255 ];
    HKEY hKey;
    DWORD dwType = REG_SZ;
    DWORD dwSize = 255;

    if( ERROR_SUCCESS == RegOpenKeyEx( HKEY_CURRENT_USER                                                 , 
                                       "SOFTWARE\\MICROSOFT\\WINDOWS\\CURRENTVERSION\\INTERNET SETTINGS" ,
                                       0L                                                                ,
                                       KEY_ALL_ACCESS                                                    ,
                                       &hKey                                                             ) )
    {
        if ( ERROR_SUCCESS == RegQueryValueEx( hKey              , 
                                               "ProxyServer"     , 
                                               NULL              , 
                                               &dwType           ,
                                               (LPBYTE)&lszValue , 
                                               &dwSize           ) )
        {
            CORE::CString regValue( lszValue );
            remoteHost = regValue.SubstrToChar( ':', true );
            port = 80;

            CORE::CString portStr( regValue.SubstrToChar( ':', false ) );
            if ( portStr.Length() > 0 )
            {
                port = static_cast< UInt16 >( CORE::StringToInt32( portStr ) );
            }
            
            active = false;
            if ( ERROR_SUCCESS == RegQueryValueEx( hKey              , 
                                                   "ProxyEnable"     , 
                                                   NULL              , 
                                                   &dwType           ,
                                                   (LPBYTE)&lszValue , 
                                                   &dwSize           ) )
            {
                active = CORE::StringToBool( lszValue );
            }
            return true;
        }
        RegCloseKey( hKey );
    }
    return false;
}

#endif /* GUCEF_MSWIN_BUILD ? */

/*-------------------------------------------------------------------------*/

CCom*
CCom::Instance( void )
{GUCEF_TRACE;
        _mutex.Lock();
        if ( !_instance ) 
        {
                _instance = new CCom();
                GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "GUCEF::COMCORE::CCom Singleton created" );
        }
        _mutex.Unlock();
        return _instance;
}

/*-------------------------------------------------------------------------*/

void 
CCom::Deinstance( void )
{GUCEF_TRACE;
        _mutex.Lock();
        delete _instance;
        _instance = NULL;
        GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "GUCEF::COMCORE::CCom Singleton destroyed" );
        _mutex.Unlock();                
}

/*-------------------------------------------------------------------------*/

CCom::CCom()
    : _keep_gstats( false )                ,
      _scount( 0 )
{GUCEF_TRACE;

    _sockets.SetResizeChange( HEAP_RESIZE_AMOUNT );
    memset( &_stats, 0, sizeof(TSocketStats) );   
    
    #ifdef GUCEF_MSWIN_BUILD

// @TODO use InternetQueryOption
// http://msdn2.microsoft.com/en-us/library/aa385101.aspx
    
    /*
     *  Read the O/S proxy server settings 
     */
    
    CORE::CString remoteHost;
    UInt16 port;
    bool active;
    
    if ( GetMSWinInternetProxyFromRegistry( remoteHost ,
                                            port       ,
                                            active     ) )
    {
        SetSystemWideProxyServer( "http"     ,
                                  remoteHost ,
                                  port       ,
                                  active     );
    }
    
    #endif /* GUCEF_MSWIN_BUILD ? */
}

/*-------------------------------------------------------------------------*/

CCom::~CCom()
{GUCEF_TRACE;
                     
}

/*-------------------------------------------------------------------------*/

void                       
CCom::RegisterSocketObject( CSocket* socket )
{GUCEF_TRACE;

    _mutex.Lock();
    socket->SetSocketID( _sockets.AddEntry( socket ) );        
    ++_scount;        
    _mutex.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CCom::UnregisterSocketObject( const CSocket* socket )
{GUCEF_TRACE;

    _mutex.Lock();
    _sockets.SetEntry( socket->GetSocketID(), NULL );        
    --_scount;
    _mutex.Unlock();
}            

/*-------------------------------------------------------------------------*/

UInt32 
CCom::GetSocketCount( void ) const
{GUCEF_TRACE;

    return _scount;
}

/*-------------------------------------------------------------------------*/

void 
CCom::SetUseGlobalStats( bool keep_gstats )
{GUCEF_TRACE;

    _keep_gstats = keep_gstats;
}

/*-------------------------------------------------------------------------*/
        
bool 
CCom::GetUseGlobalStats( void ) const
{GUCEF_TRACE;

    return _keep_gstats;
}

/*-------------------------------------------------------------------------*/
        
void 
CCom::ResetGlobalStats( void )
{GUCEF_TRACE;
}

/*-------------------------------------------------------------------------*/
        
const CCom::TSocketStats& 
CCom::GetGlobalStats( void ) const
{GUCEF_TRACE;

        return _stats;
}

/*-------------------------------------------------------------------------*/

bool
CCom::SetSystemWideProxyServer( const CORE::CString& protocol ,
                                const bool active             )
{GUCEF_TRACE;
    
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
{GUCEF_TRACE;
    
    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "Setting system wide proxy server for protocol " + protocol + 
                          " to " + remoteHost + ":" + CORE::UInt16ToString( remotePort ) + " and active state " + CORE::BoolToString( active ) );
    
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
{GUCEF_TRACE;

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
{GUCEF_TRACE;

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
