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

#ifndef GUCEF_CORE_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_ESSENTIALS_H
#endif /* GUCEF_CORE_ESSENTIALS_H ? */

#include "gucefCORE_CDirectoryWatcherEvents.h"

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

const CEvent CDirectoryWatcherEvents::StartedWatchingDirectoryEvent       = "GUCEF::CORE::CDirectoryWatcherEvents::StartedWatchingDirectoryEvent";
const CEvent CDirectoryWatcherEvents::StoppedWatchingDirectoryEvent       = "GUCEF::CORE::CDirectoryWatcherEvents::StoppedWatchingDirectoryEvent";
const CEvent CDirectoryWatcherEvents::FileCreatedEvent                    = "GUCEF::CORE::CDirectoryWatcherEvents::FileCreatedEvent";
const CEvent CDirectoryWatcherEvents::FileModifiedEvent                   = "GUCEF::CORE::CDirectoryWatcherEvents::FileModifiedEvent";
const CEvent CDirectoryWatcherEvents::FileRenamedEvent                    = "GUCEF::CORE::CDirectoryWatcherEvents::FileRenamedEvent";
const CEvent CDirectoryWatcherEvents::FileDeletedEvent                    = "GUCEF::CORE::CDirectoryWatcherEvents::FileDeletedEvent";
const CEvent CDirectoryWatcherEvents::DirCreatedEvent                     = "GUCEF::CORE::CDirectoryWatcherEvents::DirCreatedEvent";
const CEvent CDirectoryWatcherEvents::DirModifiedEvent                    = "GUCEF::CORE::CDirectoryWatcherEvents::DirModifiedEvent";
const CEvent CDirectoryWatcherEvents::DirRenamedEvent                     = "GUCEF::CORE::CDirectoryWatcherEvents::DirRenamedEvent";
const CEvent CDirectoryWatcherEvents::DirDeletedEvent                     = "GUCEF::CORE::CDirectoryWatcherEvents::DirDeletedEvent";

/*-------------------------------------------------------------------------*/

void
CDirectoryWatcherEvents::RegisterEvents( void )
{GUCEF_TRACE;

    StartedWatchingDirectoryEvent.Initialize();
    StoppedWatchingDirectoryEvent.Initialize();
    FileCreatedEvent.Initialize();
    FileModifiedEvent.Initialize();
    FileRenamedEvent.Initialize();
    FileDeletedEvent.Initialize();
    DirCreatedEvent.Initialize();
    DirModifiedEvent.Initialize();
    DirRenamedEvent.Initialize();
    DirDeletedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

bool 
CDirectoryWatcherEvents::IsDirectoryWatcherEvent( const CEvent& eventId )
{GUCEF_TRACE;

    return eventId == StartedWatchingDirectoryEvent ||
           eventId == StoppedWatchingDirectoryEvent ||
           eventId == FileCreatedEvent ||
           eventId == FileModifiedEvent ||
           eventId == FileRenamedEvent ||
           eventId == FileDeletedEvent ||
           eventId == DirCreatedEvent ||
           eventId == DirModifiedEvent ||
           eventId == DirRenamedEvent ||
           eventId == DirDeletedEvent;
}

/*-------------------------------------------------------------------------*/

bool 
CDirectoryWatcherEvents::IsDirectoryWatcherFileEvent( const CEvent& eventId )
{GUCEF_TRACE;

    return eventId == FileCreatedEvent ||
           eventId == FileModifiedEvent ||
           eventId == FileRenamedEvent ||
           eventId == FileDeletedEvent;
}

/*-------------------------------------------------------------------------*/

bool 
CDirectoryWatcherEvents::IsDirectoryWatcherDirEvent( const CEvent& eventId )
{GUCEF_TRACE;

    return eventId == DirCreatedEvent ||
           eventId == DirModifiedEvent ||
           eventId == DirRenamedEvent ||
           eventId == DirDeletedEvent;
}

/*-------------------------------------------------------------------------*/

CDirectoryWatcherEvents::CDirectoryWatcherEvents( void )
{GUCEF_TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CDirectoryWatcherEvents::CDirectoryWatcherEvents( const CDirectoryWatcherEvents& src )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CDirectoryWatcherEvents::~CDirectoryWatcherEvents()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CDirectoryWatcherEvents&
CDirectoryWatcherEvents::operator=( const CDirectoryWatcherEvents& src )
{GUCEF_TRACE;

    if ( this != &src )
    {

    }
    return *this;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
