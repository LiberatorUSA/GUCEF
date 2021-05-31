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

#include "gucefCORE_CIDirectoryWatcher.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CIDirectoryWatcher::CDirWatchOptions::CDirWatchOptions( void )
    : watchSubTree( false )
    , watchForFileCreation( true )
    , watchForFileDeletion( true )
    , watchForFileRenames( true )
    , watchForFileModifications( true )
    , watchForDirCreation( true )
    , watchForDirDeletion( true )
    , watchForDirRenames( true )
    , watchForDirModifications( true )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CIDirectoryWatcher::CDirWatchOptions::CDirWatchOptions( const CIDirectoryWatcher::CDirWatchOptions& src )
    : watchSubTree( src.watchSubTree )
    , watchForFileCreation( src.watchForFileCreation )
    , watchForFileDeletion( src.watchForFileDeletion )
    , watchForFileRenames( src.watchForFileRenames )
    , watchForFileModifications( src.watchForFileModifications )
    , watchForDirCreation( src.watchForDirCreation )
    , watchForDirDeletion( src.watchForDirDeletion )
    , watchForDirRenames( src.watchForDirRenames )
    , watchForDirModifications( src.watchForDirModifications )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CIDirectoryWatcher::CDirWatchOptions&
CIDirectoryWatcher::CDirWatchOptions::operator=( const CIDirectoryWatcher::CDirWatchOptions& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        watchSubTree = src.watchSubTree;
        watchForFileCreation = src.watchForFileCreation;
        watchForFileDeletion = src.watchForFileDeletion;
        watchForFileRenames = src.watchForFileRenames;
        watchForFileModifications = src.watchForFileModifications;
        watchForDirCreation = src.watchForDirCreation;
        watchForDirDeletion = src.watchForDirDeletion;
        watchForDirRenames = src.watchForDirRenames;
        watchForDirModifications = src.watchForDirModifications;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CIDirectoryWatcher::CIDirectoryWatcher( void )
    : CDirectoryWatcherEvents()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CIDirectoryWatcher::CIDirectoryWatcher( const CIDirectoryWatcher& src )
    : CDirectoryWatcherEvents( src )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CIDirectoryWatcher::~CIDirectoryWatcher()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CIDirectoryWatcher&
CIDirectoryWatcher::operator=( const CIDirectoryWatcher& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        CDirectoryWatcherEvents::operator=( src );
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
