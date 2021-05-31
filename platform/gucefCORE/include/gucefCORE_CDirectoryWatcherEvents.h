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

#ifndef GUCEF_CORE_CDIRECTORYWATCHEREVENTS_H
#define GUCEF_CORE_CDIRECTORYWATCHEREVENTS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_CEVENT_H
#include "CEvent.h"
#define GUCEF_CORE_CEVENT_H
#endif /* GUCEF_CORE_CEVENT_H ? */

#ifndef GUCEF_CORE_CLONEABLES_H
#include "cloneables.h"
#define GUCEF_CORE_CLONEABLES_H
#endif /* GUCEF_CORE_CLONEABLES_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_CORE_PUBLIC_CPP CDirectoryWatcherEvents
{
    public:

    static const CEvent StartedWatchingDirectoryEvent;
    static const CEvent StoppedWatchingDirectoryEvent;

    typedef TCloneableString TStartedWatchingDirectoryEventData;
    typedef TCloneableString TStoppedWatchingDirectoryEventData;

    static const CEvent FileCreatedEvent;
    static const CEvent FileModifiedEvent;
    static const CEvent FileRenamedEvent;
    static const CEvent FileDeletedEvent;

    typedef TCloneableString TFileCreatedEventData;
    typedef TCloneableString TFileModifiedEventData;
    typedef TCloneableString TFileDeletedEventData;
    struct SFileRenamedEventInfo
    {
        CString oldFilename;
        CString newFilename;
    };
    typedef CTCloneableObj< struct SFileRenamedEventInfo > TFileRenamedEventData;

    static const CEvent DirCreatedEvent;
    static const CEvent DirModifiedEvent;
    static const CEvent DirRenamedEvent;
    static const CEvent DirDeletedEvent;

    typedef TCloneableString TDirCreatedEventData;
    typedef TCloneableString TDirModifiedEventData;
    typedef TCloneableString TDirDeletedEventData;
    struct SDirRenamedEventInfo
    {
        CString oldDirName;
        CString newDirName;
    };
    typedef CTCloneableObj< struct SDirRenamedEventInfo > TDirRenamedEventData;

    static void RegisterEvents( void );

    CDirectoryWatcherEvents( void );
    CDirectoryWatcherEvents( const CDirectoryWatcherEvents& src );
    virtual ~CDirectoryWatcherEvents();
    CDirectoryWatcherEvents& operator=( const CDirectoryWatcherEvents& src );
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CDIRECTORYWATCHEREVENTS_H ? */
