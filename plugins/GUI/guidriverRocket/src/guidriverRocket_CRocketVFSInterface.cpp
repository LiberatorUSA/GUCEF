/*
 *  guidriverRocket: GUI backend using Rocket
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

#ifndef GUCEF_VFS_CVFSGLOBAL_H
#include "gucefVFS_CVfsGlobal.h"
#define GUCEF_VFS_CVFSGLOBAL_H
#endif /* GUCEF_VFS_CVFSGLOBAL_H ? */

#include "guidriverRocket_CRocketVFSInterface.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUIDRIVERROCKET {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CRocketVFSInterface::CRocketVFSInterface()
    : Rocket::Core::FileInterface()  ,
      m_vfs( &VFS::CVfsGlobal::Instance()->GetVfs() ) ,
      m_fileHandles()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CRocketVFSInterface::~CRocketVFSInterface()
{GUCEF_TRACE;

    m_fileHandles.clear();
}

/*-------------------------------------------------------------------------*/

Rocket::Core::FileHandle
CRocketVFSInterface::FindOpenHandleSlot( void ) const
{GUCEF_TRACE;

    Rocket::Core::FileHandle testIndex = 1;    
    TFileHandleMap::const_iterator i = m_fileHandles.begin();
    while ( i != m_fileHandles.end() )
    {
        if ( (*i).first > testIndex )
        {
            // Found a free slot since the next entry is not a consecutive number
            return testIndex;
        }
        ++i; ++testIndex;
    }

    // If we get here then we went through the outstanding handle list 
    // and didnt find any gaps. We will simply add a new slot at the end.
    return (Rocket::Core::FileHandle) m_fileHandles.size()+1;
}

/*-------------------------------------------------------------------------*/

Rocket::Core::FileHandle
CRocketVFSInterface::Open( const Rocket::Core::String& path )
{GUCEF_TRACE;

    Rocket::Core::FileHandle openSlot = FindOpenHandleSlot();
    VFS::TBasicVfsResourcePtr vfsFileHandle = m_vfs->GetFile( path.CString() );
    if ( !vfsFileHandle.IsNULL() )
    {
        // Set the VFS on the open slot which is now in use
        m_fileHandles[ openSlot ] = vfsFileHandle;
        return openSlot;
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

void
CRocketVFSInterface::Close( Rocket::Core::FileHandle file )
{GUCEF_TRACE;

    VFS::TBasicVfsResourcePtr vfsFile = m_fileHandles[ file ];
    if ( !vfsFile.IsNULL() )
    {
        vfsFile->GetAccess()->Close();
    }
    m_fileHandles.erase( file );
}

/*-------------------------------------------------------------------------*/

size_t
CRocketVFSInterface::Read( void* buffer, size_t size, Rocket::Core::FileHandle file )
{GUCEF_TRACE;

    VFS::TBasicVfsResourcePtr vfsFile = m_fileHandles[ file ];
    if ( !vfsFile.IsNULL() )
    {
        return vfsFile->GetAccess()->Read( buffer, 1, size );
    }
    else
    {
        // We don't allow null references to files
        m_fileHandles.erase( file );
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

bool
CRocketVFSInterface::Seek( Rocket::Core::FileHandle file, long offset, int origin )
{GUCEF_TRACE;

    VFS::TBasicVfsResourcePtr vfsFile = m_fileHandles[ file ];
    if ( !vfsFile.IsNULL() )
    {
        return vfsFile->GetAccess()->Seek( offset, origin ) == 0;
    }
    else
    {
        // We don't allow null references to files
        m_fileHandles.erase( file );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

size_t
CRocketVFSInterface::Tell( Rocket::Core::FileHandle file )
{GUCEF_TRACE;

    VFS::TBasicVfsResourcePtr vfsFile = m_fileHandles[ file ];
    if ( !vfsFile.IsNULL() )
    {
        return vfsFile->GetAccess()->Tell();
    }
    else
    {
        // We don't allow null references to files
        m_fileHandles.erase( file );
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

void
CRocketVFSInterface::Release()
{GUCEF_TRACE;

    m_fileHandles.clear();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUIDRIVERROCKET */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
