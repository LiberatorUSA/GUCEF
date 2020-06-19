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

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_VFS_CVFS_H
#include "gucefVFS_CVFS.h"
#define GUCEF_VFS_CVFS_H
#endif /* GUCEF_VFS_CVFS_H ? */

#include "gucefVFS_AsyncVfsTaskData.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace VFS {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CAsyncVfsTaskData::CAsyncVfsTaskData( void )
    : CORE::CICloneable()
    , operationType( ASYNCVFSOPERATIONTYPE_UNKNOWN )
    , m_requestorData( GUCEF_NULL )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CAsyncVfsTaskData::CAsyncVfsTaskData( const CAsyncVfsTaskData& src )
    : CORE::CICloneable( src )
    , operationType( src.operationType )
    , m_requestorData( GUCEF_NULL )
{GUCEF_TRACE;

    SetRequestorData( src.m_requestorData );
}

/*-------------------------------------------------------------------------*/

CAsyncVfsTaskData::~CAsyncVfsTaskData()
{GUCEF_TRACE;
    
    SetRequestorData( GUCEF_NULL );
}

/*-------------------------------------------------------------------------*/

void 
CAsyncVfsTaskData::SetRequestorData( CORE::CICloneable* data )
{GUCEF_TRACE;

    delete m_requestorData;
    m_requestorData = GUCEF_NULL;

    if ( GUCEF_NULL != data )
    {       
        m_requestorData = data->Clone();    
    }
}

/*-------------------------------------------------------------------------*/

CORE::CICloneable* 
CAsyncVfsTaskData::GetRequestorData( void ) const
{GUCEF_TRACE;
    
    return m_requestorData;
}

/*-------------------------------------------------------------------------*/

CAsyncVfsTaskResultData::CAsyncVfsTaskResultData( void )
    : CCloneableAsyncVfsTaskData()
    , successState( false )
    , durationInSecs( 0 )
    , m_taskData( GUCEF_NULL )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CAsyncVfsTaskResultData::CAsyncVfsTaskResultData( const CAsyncVfsTaskResultData& src )
    : CCloneableAsyncVfsTaskData( src )
    , successState( src.successState )
    , durationInSecs( src.durationInSecs )
    , m_taskData( GUCEF_NULL )
{GUCEF_TRACE;

    SetTaskData( src.m_taskData );
    m_requestorData = m_taskData->GetRequestorData();
}

/*-------------------------------------------------------------------------*/

CAsyncVfsTaskResultData::~CAsyncVfsTaskResultData()
{GUCEF_TRACE;

    m_requestorData = GUCEF_NULL; 
    SetTaskData( GUCEF_NULL );
}

/*-------------------------------------------------------------------------*/

void 
CAsyncVfsTaskResultData::SetRequestorData( CORE::CICloneable* data )
{GUCEF_TRACE;

    if ( GUCEF_NULL != m_taskData )
    {
        m_taskData->SetRequestorData( data );
    }
}

/*-------------------------------------------------------------------------*/

CORE::CICloneable* 
CAsyncVfsTaskResultData::GetRequestorData( void ) const
{GUCEF_TRACE;
    
    if ( GUCEF_NULL != m_taskData )
    {
        return m_taskData->GetRequestorData();
    }
    return GUCEF_NULL;
}

/*-------------------------------------------------------------------------*/

void 
CAsyncVfsTaskResultData::SetTaskData( CCloneableAsyncVfsTaskData* data )
{GUCEF_TRACE;

    delete m_taskData;
    m_taskData = GUCEF_NULL;

    if ( GUCEF_NULL != data )
    {       
        m_taskData = static_cast< CCloneableAsyncVfsTaskData* >( data->Clone() );    
    }
}

/*-------------------------------------------------------------------------*/

CCloneableAsyncVfsTaskData* 
CAsyncVfsTaskResultData::GetTaskData( void ) const
{GUCEF_TRACE;
    
    return m_taskData;
}

/*-------------------------------------------------------------------------*/

CStoreAsFileTaskData::CStoreAsFileTaskData( void )
    : CCloneableAsyncVfsTaskData()
    , filepath()
    , data()
    , offset()
    , overwrite()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CStoreAsFileTaskData::CStoreAsFileTaskData( const CStoreAsFileTaskData& src )
    : CCloneableAsyncVfsTaskData( src )
    , filepath( src.filepath )
    , data( src.data )
    , offset( src.offset )
    , overwrite( src.overwrite )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CStoreAsFileTaskData::~CStoreAsFileTaskData()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CMountArchiveTaskData::CMountArchiveTaskData( void )
    : CCloneableAsyncVfsTaskData()
    , settings()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CMountArchiveTaskData::CMountArchiveTaskData( const CMountArchiveTaskData& src )
    : CCloneableAsyncVfsTaskData( src )
    , settings( src.settings )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CMountArchiveTaskData::~CMountArchiveTaskData()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace VFS */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
