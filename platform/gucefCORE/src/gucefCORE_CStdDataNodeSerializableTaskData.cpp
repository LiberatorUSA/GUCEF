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

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H ? */

#include "gucefCORE_CStdDataNodeSerializableTaskData.h"   

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

const CString CStdDataNodeSerializableTaskData::ClassTypeName = "GUCEF::CORE::CStdDataNodeSerializableTaskData";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CStdDataNodeSerializableTaskData::CStdDataNodeSerializableTaskData( void )
    : CIDataNodeSerializableTaskData()
    , m_threadPoolName( CTaskManager::DefaultThreadPoolName )
    , m_taskTypeName()
    , m_onlyUseExistingThreadPool( true )
    , m_taskCanBeQueued( true )
    , m_rawTaskData( GUCEF_DATATYPE_UNKNOWN )
    , m_taskId( 0 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CStdDataNodeSerializableTaskData::CStdDataNodeSerializableTaskData( const CStdDataNodeSerializableTaskData& src )
    : CIDataNodeSerializableTaskData( src ) 
    , m_threadPoolName( src.m_threadPoolName ) 
    , m_taskTypeName( src.m_taskTypeName )  
    , m_onlyUseExistingThreadPool( src.m_onlyUseExistingThreadPool )
    , m_taskCanBeQueued( src.m_taskCanBeQueued )
    , m_rawTaskData( src.m_rawTaskData )
    , m_taskId( src.m_taskId )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CStdDataNodeSerializableTaskData::~CStdDataNodeSerializableTaskData()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CStdDataNodeSerializableTaskData&
CStdDataNodeSerializableTaskData::operator=( const CStdDataNodeSerializableTaskData& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        CIDataNodeSerializableTaskData::operator=( src );
        m_threadPoolName = src.m_threadPoolName;
        m_taskTypeName = src.m_taskTypeName;
        m_onlyUseExistingThreadPool = src.m_onlyUseExistingThreadPool;
        m_taskCanBeQueued = src.m_taskCanBeQueued;
        m_rawTaskData = src.m_rawTaskData;
        m_taskId = src.m_taskId;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CICloneable* 
CStdDataNodeSerializableTaskData::Clone( void ) const
{GUCEF_TRACE;

    return new CStdDataNodeSerializableTaskData( *this );
}

/*-------------------------------------------------------------------------*/

bool
CStdDataNodeSerializableTaskData::Serialize( CDataNode& domRootNode                        ,
                                             const CDataNodeSerializableSettings& settings ) const
{GUCEF_TRACE;

    bool totalSuccess = true;
    totalSuccess = domRootNode.SetAttribute( "taskTypeName", m_taskTypeName ) && totalSuccess;
    totalSuccess = domRootNode.SetAttribute( "threadPoolName", m_threadPoolName ) && totalSuccess;
    totalSuccess = domRootNode.SetAttribute( "onlyUseExistingThreadPool", m_onlyUseExistingThreadPool ) && totalSuccess;    
    totalSuccess = domRootNode.SetAttribute( "taskCanBeQueued", m_taskCanBeQueued ) && totalSuccess;        
    totalSuccess = domRootNode.SetAttribute( "taskId", m_taskId ) && totalSuccess;        
       
    CDataNode* taskDataNode = domRootNode.FindOrAddChild( "taskData", GUCEF_DATATYPE_OBJECT );
    if ( GUCEF_NULL != taskDataNode )
    {
        totalSuccess = SerializeTaskData( *taskDataNode, settings ) && totalSuccess;
    }
    else
    {
        totalSuccess = false;
    }

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CStdDataNodeSerializableTaskData::Deserialize( const CDataNode& domRootNode                  ,
                                               const CDataNodeSerializableSettings& settings ) 
{GUCEF_TRACE;
    
    m_threadPoolName = domRootNode.GetAttributeValueOrChildValueByName( "threadPoolName" ).AsString( m_threadPoolName, true );        
    m_taskTypeName = domRootNode.GetAttributeValueOrChildValueByName( "taskTypeName" ).AsString( m_taskTypeName, true );
    m_onlyUseExistingThreadPool = domRootNode.GetAttributeValueOrChildValueByName( "onlyUseExistingThreadPool" ).AsBool( m_onlyUseExistingThreadPool, true ); 
    m_taskCanBeQueued = domRootNode.GetAttributeValueOrChildValueByName( "taskCanBeQueued" ).AsBool( m_taskCanBeQueued, true ); 
    m_taskId = domRootNode.GetAttributeValueOrChildValueByName( "taskId" ).AsUInt32( m_taskId, true ); 

    // Check to see if we have a task data child
    // A decending class might want to handle this differently, for example by directly parsing into discrete fields, so its optional
    CDataNode* taskDataNode = domRootNode.FindChild( "taskData" );
    if ( GUCEF_NULL != taskDataNode )
    {
        return DeserializeTaskData( *taskDataNode, settings );
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CStdDataNodeSerializableTaskData::SerializeTaskData( CDataNode& domRootNode                        ,
                                                     const CDataNodeSerializableSettings& settings ) const
{GUCEF_TRACE;

    // Check to see if we have a task data child
    // A decending class might want to handle this differently, for example by directly parsing into discrete fields, so its optional
    if ( GUCEF_DATATYPE_UNKNOWN != m_rawTaskData.GetNodeType() )
    {
        // the raw storage was initialized, lets use it
        return GUCEF_NULL != domRootNode.AddChild( m_rawTaskData ); 
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CStdDataNodeSerializableTaskData::DeserializeTaskData( const CDataNode& domRootNode                  ,
                                                       const CDataNodeSerializableSettings& settings ) 
{GUCEF_TRACE;

    // If a decending class did not override this we will just do a dumb storage copy into the raw task data storage
    // we cannot do more since we have no idea what the data actually represents. If you need something more specific
    // just override the DeserializeTaskData() member function
    const CDataNode* optionalRawTaskData = domRootNode.GetFirstChild();
    if ( GUCEF_NULL != optionalRawTaskData )
    {
        m_rawTaskData = *optionalRawTaskData;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

void 
CStdDataNodeSerializableTaskData::SetTaskTypeName( const CString& typeName )
{GUCEF_TRACE;

    m_taskTypeName = typeName;
}

/*-------------------------------------------------------------------------*/

const CString& 
CStdDataNodeSerializableTaskData::GetTaskTypeName( void ) const
{GUCEF_TRACE;

    return m_taskTypeName;
}

/*-------------------------------------------------------------------------*/

void 
CStdDataNodeSerializableTaskData::SetThreadPoolName( const CString& poolName )
{GUCEF_TRACE;

    m_threadPoolName = poolName;
}

/*-------------------------------------------------------------------------*/

const CString& 
CStdDataNodeSerializableTaskData::GetThreadPoolName( void ) const
{GUCEF_TRACE;

    return m_threadPoolName;
}

/*-------------------------------------------------------------------------*/

void
CStdDataNodeSerializableTaskData::SetOnlyUseExistingThreadPool( bool useExistingPoolOnly )
{GUCEF_TRACE;

    m_onlyUseExistingThreadPool = useExistingPoolOnly;
}

/*-------------------------------------------------------------------------*/

bool
CStdDataNodeSerializableTaskData::GetOnlyUseExistingThreadPool( void ) const
{GUCEF_TRACE;

    return m_onlyUseExistingThreadPool;
}

/*-------------------------------------------------------------------------*/

void
CStdDataNodeSerializableTaskData::SetTaskCanBeQueued( bool canBeQueued )
{GUCEF_TRACE;

    m_taskCanBeQueued = canBeQueued;
}

/*-------------------------------------------------------------------------*/

bool
CStdDataNodeSerializableTaskData::GetTaskCanBeQueued( void ) const
{GUCEF_TRACE;

    return m_onlyUseExistingThreadPool;
}

/*-------------------------------------------------------------------------*/

void 
CStdDataNodeSerializableTaskData::SetTaskId( UInt32 taskId )
{GUCEF_TRACE;

    m_taskId = taskId;
}

/*-------------------------------------------------------------------------*/

UInt32 
CStdDataNodeSerializableTaskData::GetTaskId( void ) const
{GUCEF_TRACE;

    return m_taskId;
}

/*-------------------------------------------------------------------------*/

const CString& 
CStdDataNodeSerializableTaskData::GetClassTypeName( void ) const
{GUCEF_TRACE;

    return ClassTypeName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
