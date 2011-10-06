/*
 *  gucefGUI: GUCEF module providing a uniform interface towards GUI backends
 *  Copyright (C) 2002 - 2011.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
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

#include "gucefGUI_CWindowManager.h"    /* definition of the class implemented here */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUI {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

MT::CMutex CWindowManager::g_lock;
CWindowManager* CWindowManager::g_instance = NULL;
         
/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CWindowManager::CWindowManager( void )
    : m_backends()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CWindowManager::~CWindowManager()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CWindowManager* 
CWindowManager::Instance( void )
{GUCEF_TRACE;

    if ( NULL == g_instance )
    {
        g_lock.Lock();
        if ( NULL == g_instance )
        {
            g_instance = new CWindowManager();
        }
        g_lock.Unlock();
    }
    return g_instance;
}

/*-------------------------------------------------------------------------*/

void
CWindowManager::Deinstance( void )
{GUCEF_TRACE;

    g_lock.Lock();
    delete g_instance;
    g_instance = NULL;
    g_lock.Unlock();
}

/*-------------------------------------------------------------------------*/

TWindowManagerBackendPtr 
CWindowManager::GetBackend( const CORE::CString& typeName )
{GUCEF_TRACE;

    g_lock.Lock();
    TWindowManagerBackendMap::iterator i = m_backends.find( typeName );
    if ( i != m_backends.end() )
    {
        g_lock.Unlock();
        return (*i).second;
    }
    g_lock.Unlock();
    return TWindowManagerBackendPtr();
}

/*-------------------------------------------------------------------------*/

void
CWindowManager::RegisterBackend( const CORE::CString& typeName    ,
                                 TWindowManagerBackendPtr backend )
{GUCEF_TRACE;

    g_lock.Lock();
    m_backends[ typeName ] = backend;
    g_lock.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CWindowManager::UnregisterBackend( const CORE::CString& typeName )
{GUCEF_TRACE;

    g_lock.Lock();
    m_backends.erase( typeName );
    g_lock.Unlock();
}

/*-------------------------------------------------------------------------*/

void
CWindowManager::GetListOfAvailableBackends( TWindowManagerBackendMap& map )
{GUCEF_TRACE;

    g_lock.Lock();
    map = m_backends;
    g_lock.Unlock();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace GUI */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
