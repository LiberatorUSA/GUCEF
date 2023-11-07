/*
 *  gucefVFS: GUCEF module implementing a Virtual File System
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
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "gucefVFS_CArchive.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace VFS {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CArchive::CArchive( void )
    : CORE::CObservingNotifier()
    , CORE::CIDirectoryWatcher()
    , CORE::CTDynamicDestructorBase< CVFSHandle >() 
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CArchive::CArchive( const CArchive& src )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CArchive::~CArchive()
{GUCEF_TRACE;

}
    
/*-------------------------------------------------------------------------*/
    
CArchive&
CArchive::operator=( const CArchive& src )
{GUCEF_TRACE;

    return *this;
}

/*-------------------------------------------------------------------------*/

bool 
CArchive::AddDirToWatch( const CString& dirToWatch       ,
                         const CDirWatchOptions& options ,
                         bool tryToCreatePathIfNotExists )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CArchive::RemoveDirToWatch( const CString& dirToWatch )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool
CArchive::RemoveAllWatches( void )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CArchive::GetAllWatchedDirs( CString::StringSet& dirs ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CArchive::IsDirectoryWatchingSupported( void ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/
 
bool
CArchive::IsHealthy( void ) const
{GUCEF_TRACE;

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CArchive::IsConnected( void ) const
{GUCEF_TRACE;

    return true;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace VFS */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
