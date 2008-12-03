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

#include "gucefCORE_CObserverSwitch.h"

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
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CObserverSwitch::CObserverSwitch( void )
    : CObservingNotifier()  ,
      m_observerGroups()    ,
      m_activeGroupName()   ,
      m_activeGroup( NULL )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CObserverSwitch::CObserverSwitch( const CObserverSwitch& src )
    : CObservingNotifier( src ) ,
      m_observerGroups()        ,
      m_activeGroupName()       ,
      m_activeGroup( NULL )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CObserverSwitch::~CObserverSwitch()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CObserverSwitch& 
CObserverSwitch::operator=( const CObserverSwitch& src )
{GUCEF_TRACE;

    return *this;
}

/*-------------------------------------------------------------------------*/

void
CObserverSwitch::AddObserverToGroup( const CString& groupName ,
                                     CObserver& observer      )
{GUCEF_TRACE;

    assert( NULL != &observer );
    
    LockData();
    
    (m_observerGroups[ groupName ]).insert( &observer );
    
    // Make sure our shortcut pointer gets set if this is the first item in the
    // already active group
    if ( NULL == m_activeGroup )
    {
        if ( groupName == m_activeGroupName )
        {
            m_activeGroup = &( (*m_observerGroups.find( groupName ) ).second );
        }
    }
    
    UnlockData();
}

/*-------------------------------------------------------------------------*/

void
CObserverSwitch::RemoveObserverFromGroup( const CString& groupName ,
                                          CObserver& observer      )
{GUCEF_TRACE;

    assert( NULL != &observer );
    
    LockData();
    TObserverGroupMap::iterator i = m_observerGroups.find( groupName );
    if ( i != m_observerGroups.end() )
    {
        TObserverSet& set = (*i).second;
        set.erase( &observer );
        if ( set.size() == 0 )
        {
            m_observerGroups.erase( i );
        }
    }
    UnlockData();
}
    
/*-------------------------------------------------------------------------*/

void 
CObserverSwitch::SetActiveGroup( const CString& groupName )
{GUCEF_TRACE;

    LockData();
    m_activeGroupName = groupName;
    TObserverGroupMap::iterator i = m_observerGroups.find( groupName );
    if ( i != m_observerGroups.end() )
    {
        m_activeGroup = &(*i).second;
    }
    else
    {
        m_activeGroup = NULL;
    }
    UnlockData();
}

/*-------------------------------------------------------------------------*/

const CString& 
CObserverSwitch::GetActiveGroup( void ) const
{GUCEF_TRACE;

    return m_activeGroupName;
}

/*-------------------------------------------------------------------------*/

void
CObserverSwitch::OnNotify( CNotifier* notifier                 ,
                           const CEvent& eventid               ,
                           CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;

    LockData();
    if ( NULL != m_activeGroup )
    {
        TObserverSet::iterator i = m_activeGroup->begin();
        while ( i != m_activeGroup->end() )
        {
            if ( !NotifySpecificObserver( *notifier ,
                                          *(*i)     ,
                                          eventid   ,
                                          eventdata ) )
            {
                UnlockData();
                return;
            }
            ++i;
        }
    }
    UnlockData();
}

/*-------------------------------------------------------------------------*/

const CString&
CObserverSwitch::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CString typeName = "GUCEF::CORE::CObserverSwitch";
    return typeName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
