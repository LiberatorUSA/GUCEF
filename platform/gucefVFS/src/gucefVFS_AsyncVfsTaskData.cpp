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
    , asyncRequestId()
    , m_requestorData( GUCEF_NULL )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CAsyncVfsTaskData::CAsyncVfsTaskData( const CAsyncVfsTaskData& src )
    : CORE::CICloneable( src )
    , operationType( src.operationType )
    , asyncRequestId( src.asyncRequestId )
    , m_requestorData( GUCEF_NULL )
{GUCEF_TRACE;

    SetRequestorData( src.m_requestorData );
}

/*-------------------------------------------------------------------------*/

CAsyncVfsTaskData&
CAsyncVfsTaskData::operator=( const CAsyncVfsTaskData& src )
{GUCEF_TRACE;

    if ( this != &src )
    {    
        operationType = src.operationType;
        asyncRequestId = src.asyncRequestId;
        SetRequestorData( src.m_requestorData );
    }
    return *this;
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

    GUCEF_DELETE m_requestorData;
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

CORE::CICloneable* 
CAsyncVfsTaskData::Clone( void ) const
{GUCEF_TRACE;
    
    return GUCEF_NEW CAsyncVfsTaskData( *this );
}

/*-------------------------------------------------------------------------*/

CAsyncVfsTaskResultData::CAsyncVfsTaskResultData( void )
    : CAsyncVfsTaskData()
    , successState( false )
    , durationInMilliSecs( 0 )
    , m_taskData( GUCEF_NULL )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CAsyncVfsTaskResultData::CAsyncVfsTaskResultData( const CAsyncVfsTaskResultData& src )
    : CAsyncVfsTaskData( src )
    , successState( src.successState )
    , durationInMilliSecs( src.durationInMilliSecs )
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
CAsyncVfsTaskResultData::SetTaskData( CAsyncVfsTaskData* data )
{GUCEF_TRACE;

    GUCEF_DELETE m_taskData;
    m_taskData = GUCEF_NULL;

    if ( GUCEF_NULL != data )
    {       
        m_taskData = static_cast< CAsyncVfsTaskData* >( data->Clone() );
        CAsyncVfsTaskData::operator=( *m_taskData );
    }
}

/*-------------------------------------------------------------------------*/

CAsyncVfsTaskData* 
CAsyncVfsTaskResultData::GetTaskData( void ) const
{GUCEF_TRACE;
    
    return m_taskData;
}

/*-------------------------------------------------------------------------*/

CORE::CICloneable* 
CAsyncVfsTaskResultData::Clone( void ) const
{GUCEF_TRACE;
    
    return GUCEF_NEW CAsyncVfsTaskResultData( *this );
}

/*-------------------------------------------------------------------------*/

CStoreAsFileTaskData::CStoreAsFileTaskData( void )
    : CAsyncVfsTaskData()
    , filepath()
    , data()
    , offset()
    , overwrite()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CStoreAsFileTaskData::CStoreAsFileTaskData( const CStoreAsFileTaskData& src )
    : CAsyncVfsTaskData( src )
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

CORE::CICloneable* 
CStoreAsFileTaskData::Clone( void ) const
{GUCEF_TRACE;
    
    return GUCEF_NEW CStoreAsFileTaskData( *this );
}

/*-------------------------------------------------------------------------*/

CMountArchiveTaskData::CMountArchiveTaskData( void )
    : CAsyncVfsTaskData()
    , settings()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CMountArchiveTaskData::CMountArchiveTaskData( const CMountArchiveTaskData& src )
    : CAsyncVfsTaskData( src )
    , settings( src.settings )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CMountArchiveTaskData::~CMountArchiveTaskData()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CORE::CICloneable* 
CMountArchiveTaskData::Clone( void ) const
{GUCEF_TRACE;
    
    return GUCEF_NEW CMountArchiveTaskData( *this );
}

/*-------------------------------------------------------------------------*/

CEncodeFileTaskData::CEncodeFileTaskData( void )
    : CAsyncVfsTaskData()
    , originalFilepath()
    , encodedFilepath()
    , overwrite( false )
    , codecFamily()
    , encodeCodec()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CEncodeFileTaskData::CEncodeFileTaskData( const CEncodeFileTaskData& src )
    : CAsyncVfsTaskData( src )
    , originalFilepath( src.originalFilepath )
    , encodedFilepath( src.encodedFilepath )
    , overwrite( src.overwrite )
    , codecFamily( src.codecFamily )
    , encodeCodec( src.encodeCodec )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CEncodeFileTaskData::~CEncodeFileTaskData()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CORE::CICloneable* 
CEncodeFileTaskData::Clone( void ) const
{GUCEF_TRACE;
    
    return GUCEF_NEW CEncodeFileTaskData( *this );
}

/*-------------------------------------------------------------------------*/

CEncodeBufferAsFileTaskData::CEncodeBufferAsFileTaskData( void )
    : CAsyncVfsTaskData()
    , data()
    , bufferOffset( 0 )
    , encodedFilepath()
    , overwrite( false )
    , codecFamily()
    , encodeCodec()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CEncodeBufferAsFileTaskData::CEncodeBufferAsFileTaskData( const CEncodeBufferAsFileTaskData& src )
    : CAsyncVfsTaskData( src )
    , data( src.data )
    , bufferOffset( src.bufferOffset )
    , encodedFilepath( src.encodedFilepath )
    , overwrite( src.overwrite )
    , codecFamily( src.codecFamily )
    , encodeCodec( src.encodeCodec )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CEncodeBufferAsFileTaskData::~CEncodeBufferAsFileTaskData()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CORE::CICloneable* 
CEncodeBufferAsFileTaskData::Clone( void ) const
{GUCEF_TRACE;
    
    return GUCEF_NEW CEncodeBufferAsFileTaskData( *this );
}

/*-------------------------------------------------------------------------*/

CDecodeFileTaskData::CDecodeFileTaskData( void )
    : CAsyncVfsTaskData()
    , originalFilepath()
    , decodedFilepath()
    , overwrite( false )
    , codecFamily()
    , decodeCodec()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CDecodeFileTaskData::CDecodeFileTaskData( const CDecodeFileTaskData& src )
    : CAsyncVfsTaskData( src )
    , originalFilepath( src.originalFilepath )
    , decodedFilepath( src.decodedFilepath )
    , overwrite( src.overwrite )
    , codecFamily( src.codecFamily )
    , decodeCodec( src.decodeCodec )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CDecodeFileTaskData::~CDecodeFileTaskData()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CORE::CICloneable* 
CDecodeFileTaskData::Clone( void ) const
{GUCEF_TRACE;
    
    return GUCEF_NEW CDecodeFileTaskData( *this );
}

/*-------------------------------------------------------------------------*/

CCopyFileTaskData::CCopyFileTaskData( void )
    : CAsyncVfsTaskData()
    , originalFilepath()
    , copyFilepath()
    , overwrite( false )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CCopyFileTaskData::CCopyFileTaskData( const CCopyFileTaskData& src )
    : CAsyncVfsTaskData( src )
    , originalFilepath( src.originalFilepath )
    , copyFilepath( src.copyFilepath )
    , overwrite( src.overwrite )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CCopyFileTaskData::~CCopyFileTaskData()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CORE::CICloneable* 
CCopyFileTaskData::Clone( void ) const
{GUCEF_TRACE;
    
    return GUCEF_NEW CCopyFileTaskData( *this );
}

/*-------------------------------------------------------------------------*/

CCopyFileContentTaskData::CCopyFileContentTaskData( void )
    : CAsyncVfsTaskData()
    , originalFilepath()
    , copyFilepath()
    , overwrite( false )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CCopyFileContentTaskData::CCopyFileContentTaskData( const CCopyFileContentTaskData& src )
    : CAsyncVfsTaskData( src )
    , originalFilepath( src.originalFilepath )
    , copyFilepath( src.copyFilepath )
    , overwrite( src.overwrite )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CCopyFileContentTaskData::~CCopyFileContentTaskData()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CORE::CICloneable* 
CCopyFileContentTaskData::Clone( void ) const
{GUCEF_TRACE;
    
    return GUCEF_NEW CCopyFileContentTaskData( *this );
}

/*-------------------------------------------------------------------------*/

CMoveFileTaskData::CMoveFileTaskData( void )
    : CAsyncVfsTaskData()
    , originalFilepath()
    , newFilepath()
    , overwrite( false )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CMoveFileTaskData::CMoveFileTaskData( const CMoveFileTaskData& src )
    : CAsyncVfsTaskData( src )
    , originalFilepath( src.originalFilepath )
    , newFilepath( src.newFilepath )
    , overwrite( src.overwrite )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CMoveFileTaskData::~CMoveFileTaskData()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CORE::CICloneable* 
CMoveFileTaskData::Clone( void ) const
{GUCEF_TRACE;
    
    return GUCEF_NEW CMoveFileTaskData( *this );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace VFS */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
