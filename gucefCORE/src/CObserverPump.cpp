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

#define GUCEF_CORE_COBSERVERPUMP_CPP
#include "CObserverPump.h"

#include "CPumpedObserver.h"

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

CObserverPump* CObserverPump::m_instance = NULL;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CObserverPump::CObserverPump( void )
{
}

/*-------------------------------------------------------------------------*/

CObserverPump::CObserverPump( const CObserverPump& src )
{
}

/*-------------------------------------------------------------------------*/

CObserverPump::~CObserverPump()
{
}

/*-------------------------------------------------------------------------*/

CObserverPump&
CObserverPump::operator=( const CObserverPump& src )
{
    /* dummy, do not use */

    if ( this != &src )
    {
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CObserverPump*
CObserverPump::Instance( void )
{
    if ( !m_instance )
    {
        m_instance = new CObserverPump();
    }
    return m_instance;
}

/*-------------------------------------------------------------------------*/

void
CObserverPump::Deinstance( void )
{
    delete m_instance;
    m_instance = NULL;
}

/*-------------------------------------------------------------------------*/

void 
CObserverPump::RegisterObserver( CPumpedObserver* observer )
{
    m_dataLock.Lock();
    m_observerList.insert( observer );
    m_dataLock.Unlock();
}

/*-------------------------------------------------------------------------*/
    
void 
CObserverPump::UnregisterObserver( CPumpedObserver* observer )
{
    m_dataLock.Lock();
    m_observerList.erase( observer );
    m_dataLock.Unlock();
}

/*-------------------------------------------------------------------------*/

void 
CObserverPump::OnUpdate( const UInt64 tickCount               ,
                         const Float64 updateDeltaInMilliSecs )
{
    m_dataLock.Lock();
    TObserverList::iterator i( m_observerList.begin() );
    while ( i != m_observerList.end() )
    {
        (*i)->OnUpdate();
        ++i;
    }
    m_dataLock.Unlock();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
