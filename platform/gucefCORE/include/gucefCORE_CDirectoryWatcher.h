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

#ifndef GUCEF_CORE_CDIRECTORYWATCHER_H
#define GUCEF_CORE_CDIRECTORYWATCHER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

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

class OSSpecificDirectoryWatcher;

/*-------------------------------------------------------------------------*/

class GUCEF_CORE_PUBLIC_CPP CDirectoryWatcher : public CObservingNotifier
{
    public:

    static const CEvent StartedWatchingDirectoryEvent;
    static const CEvent StoppedWatchingDirectoryEvent;

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

    static void RegisterEvents( void );
                              
    class GUCEF_CORE_PUBLIC_CPP CDirWatchOptions
    {
        public:

        bool watchSubTree;
        bool watchForFileCreation;
        bool watchForFileDeletion;
        bool watchForFileRenames;
        bool watchForFileModifications;

        CDirWatchOptions( void );
        CDirWatchOptions( const CDirWatchOptions& src );
        CDirWatchOptions& operator=( const CDirWatchOptions& src );
    };
    
    bool AddDirToWatch( const CString& dirToWatch       ,
                        const CDirWatchOptions& options );

    bool RemoveDirToWatch( const CString& dirToWatch );

    bool RemoveAllWatches( void );

    CDirectoryWatcher( void );

    CDirectoryWatcher( CPulseGenerator& pulseGenerator );

    CDirectoryWatcher( const CDirectoryWatcher& src );

    virtual ~CDirectoryWatcher();

    CDirectoryWatcher& operator=( const CDirectoryWatcher& src );

    private:
    friend class OSSpecificDirectoryWatcher;

    OSSpecificDirectoryWatcher* m_osSpecificImpl;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CDIRECTORYWATCHER_H ? */
