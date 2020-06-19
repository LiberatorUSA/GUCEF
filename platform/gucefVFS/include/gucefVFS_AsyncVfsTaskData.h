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

#ifndef GUCEF_VFS_ASYNCVFSTASKDATA_H
#define GUCEF_VFS_ASYNCVFSTASKDATA_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_CORE_CTCLONEABLEEXPANSION_H
#include "CTCloneableExpansion.h"
#define GUCEF_CORE_CTCLONEABLEEXPANSION_H
#endif /* GUCEF_CORE_CTCLONEABLEEXPANSION_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_CORE_CLONEABLES_H
#include "cloneables.h"
#define GUCEF_CORE_CLONEABLES_H
#endif /* GUCEF_CORE_CLONEABLES_H ? */

#ifndef GUCEF_VFS_CARCHIVESETTINGS_H
#include "gucefVFS_CArchiveSettings.h"
#define GUCEF_VFS_CARCHIVESETTINGS_H
#endif /* GUCEF_VFS_CARCHIVESETTINGS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace VFS {

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

enum EAsyncVfsOperationType : int
{
    ASYNCVFSOPERATIONTYPE_UNKNOWN           = 0,
    ASYNCVFSOPERATIONTYPE_MOUNTARCHIVE         ,
    ASYNCVFSOPERATIONTYPE_STOREDATAASFILE
};
typedef enum EAsyncVfsOperationType TAsyncVfsOperationType;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_VFS_PUBLIC_CPP CAsyncVfsTaskData : public CORE::CICloneable
{
    public:

    TAsyncVfsOperationType operationType;
    
    virtual void SetRequestorData( CORE::CICloneable* data );

    virtual CORE::CICloneable* GetRequestorData( void ) const;

    CAsyncVfsTaskData( void );
    CAsyncVfsTaskData( const CAsyncVfsTaskData& src );
    virtual ~CAsyncVfsTaskData();

    protected:
    CORE::CICloneable* m_requestorData;
};
typedef CORE::CTCloneableExpansion< CAsyncVfsTaskData > CCloneableAsyncVfsTaskData;

/*-------------------------------------------------------------------------*/

class GUCEF_VFS_PUBLIC_CPP CAsyncVfsTaskResultData : public CCloneableAsyncVfsTaskData
{
    public:

    bool successState;
    UInt32 durationInSecs;

    virtual void SetTaskData( CCloneableAsyncVfsTaskData* data );

    virtual CCloneableAsyncVfsTaskData* GetTaskData( void ) const;

    virtual void SetRequestorData( CORE::CICloneable* data ) GUCEF_VIRTUAL_OVERRIDE;

    virtual CORE::CICloneable* GetRequestorData( void ) const GUCEF_VIRTUAL_OVERRIDE;

    CAsyncVfsTaskResultData( void );
    CAsyncVfsTaskResultData( const CAsyncVfsTaskResultData& src );
    virtual ~CAsyncVfsTaskResultData();

    protected:
    CCloneableAsyncVfsTaskData* m_taskData;
};
typedef CORE::CTCloneableExpansion< CAsyncVfsTaskResultData > CCloneableAsyncVfsTaskResultData;

/*-------------------------------------------------------------------------*/

class GUCEF_VFS_PUBLIC_CPP CStoreAsFileTaskData : public CCloneableAsyncVfsTaskData
{
    public:

    CORE::CString filepath;
    CORE::TLinkedCloneableBuffer data;
    CORE::UInt64 offset;
    bool overwrite;

    CStoreAsFileTaskData( void );
    CStoreAsFileTaskData( const CStoreAsFileTaskData& src );
    virtual ~CStoreAsFileTaskData();
};
typedef CORE::CTCloneableExpansion< CStoreAsFileTaskData > CCloneableStoreAsFileTaskData;

/*-------------------------------------------------------------------------*/

class GUCEF_VFS_PUBLIC_CPP CMountArchiveTaskData : public CCloneableAsyncVfsTaskData
{
    public:
    CArchiveSettings settings;

    CMountArchiveTaskData( void );
    CMountArchiveTaskData( const CMountArchiveTaskData& src );
    virtual ~CMountArchiveTaskData();
};
typedef CORE::CTCloneableExpansion< CMountArchiveTaskData > CCloneableMountArchiveTaskData;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace VFS */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_VFS_ASYNCVFSTASKDATA_H ? */
