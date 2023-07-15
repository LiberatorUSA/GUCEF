/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
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

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#include "gucefCORE_CThreadPoollnfo.h"

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

const CString CThreadPoolInfo::ClassTypeName = "GUCEF::CORE::CThreadPoolInfo";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CThreadPoolInfo::CThreadPoolInfo( void )
    : CIDataNodeSerializable()
    , m_threadPoolName()
    , m_desiredMaxNrOfThreads( -1 )
    , m_desiredMinNrOfWorkerThreads( 0 )
    , m_activeNrOfDedicatedThreads( 0 )
    , m_activeNrOfWorkerThreads( 0 )
    , m_taskConsumerFactoryTypes()
    , m_taskDataFactoryTypes()
    , m_allowAppThreadToWork( false )
    , m_queuedTaskCount( 0 )
    , m_taskIds()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CThreadPoolInfo::CThreadPoolInfo( const CThreadPoolInfo& src )
    : CIDataNodeSerializable( src )
    , m_threadPoolName( src.m_threadPoolName )
    , m_desiredMaxNrOfThreads( src.m_desiredMaxNrOfThreads )
    , m_desiredMinNrOfWorkerThreads( src.m_desiredMinNrOfWorkerThreads )
    , m_activeNrOfDedicatedThreads( src.m_activeNrOfDedicatedThreads )
    , m_activeNrOfWorkerThreads( src.m_activeNrOfWorkerThreads )
    , m_taskConsumerFactoryTypes( src.m_taskConsumerFactoryTypes )
    , m_taskDataFactoryTypes( src.m_taskDataFactoryTypes )
    , m_allowAppThreadToWork( src.m_allowAppThreadToWork )
    , m_queuedTaskCount( src.m_queuedTaskCount )
    , m_taskIds( src.m_taskIds )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CThreadPoolInfo::~CThreadPoolInfo()
{GUCEF_TRACE;

    Clear();
}

/*-------------------------------------------------------------------------*/

void
CThreadPoolInfo::Clear( void )
{GUCEF_TRACE;

    m_threadPoolName.Clear();
    m_desiredMaxNrOfThreads = -1;
    m_desiredMinNrOfWorkerThreads = 0;
    m_activeNrOfDedicatedThreads = 0;
    m_activeNrOfWorkerThreads = 0;
    m_taskConsumerFactoryTypes.clear();
    m_taskDataFactoryTypes.clear();
    m_allowAppThreadToWork = false;
    m_queuedTaskCount = 0;
}

/*-------------------------------------------------------------------------*/

bool 
CThreadPoolInfo::Serialize( CDataNode& domRootNode                        ,
                            const CDataNodeSerializableSettings& settings ) const
{GUCEF_TRACE;

    bool totalSuccess = true;
    
    if ( CDataNodeSerializableSettings::DataNodeSerializableLod_KeyOnly == settings.levelOfDetail )
    {
        domRootNode.SetNodeType( GUCEF_DATATYPE_ARRAY );
        totalSuccess = domRootNode.AddAllValuesAsChildren( m_taskIds ) && totalSuccess;
        return totalSuccess;
    }
    
    totalSuccess = domRootNode.SetAttribute( "threadPoolName", m_threadPoolName ) && totalSuccess;
    totalSuccess = domRootNode.SetAttribute( "desiredMinNrOfWorkerThreads", m_desiredMinNrOfWorkerThreads ) && totalSuccess;
    totalSuccess = domRootNode.SetAttribute( "desiredMaxNrOfThreads", m_desiredMaxNrOfThreads ) && totalSuccess;
    totalSuccess = domRootNode.SetAttribute( "activeNrOfDedicatedThreads", m_activeNrOfDedicatedThreads ) && totalSuccess;
    totalSuccess = domRootNode.SetAttribute( "activeNrOfWorkerThreads", m_activeNrOfWorkerThreads ) && totalSuccess;
    totalSuccess = domRootNode.SetAttribute( "allowAppThreadToWork", m_allowAppThreadToWork ) && totalSuccess;
    totalSuccess = domRootNode.SetAttribute( "queuedTaskCount", m_queuedTaskCount ) && totalSuccess;
    
    if ( CDataNodeSerializableSettings::DataNodeSerializableLod_MinimumDetails < settings.levelOfDetail )
    {
        totalSuccess = domRootNode.AddAllValuesAsChildrenOfChild( "taskIds", m_taskIds, GUCEF_DATATYPE_ARRAY ) && totalSuccess;
        totalSuccess = domRootNode.AddAllValuesAsChildrenOfChild( "taskConsumerFactoryTypes", m_taskConsumerFactoryTypes, GUCEF_DATATYPE_ARRAY ) && totalSuccess;
        totalSuccess = domRootNode.AddAllValuesAsChildrenOfChild( "taskDataFactoryTypes", m_taskDataFactoryTypes, GUCEF_DATATYPE_ARRAY ) && totalSuccess;
    }    

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CThreadPoolInfo::Deserialize( const CDataNode& domRootNode                  ,
                              const CDataNodeSerializableSettings& settings ) 
{GUCEF_TRACE;

    Clear();
    
    if ( CDataNodeSerializableSettings::DataNodeSerializableLod_KeyOnly == settings.levelOfDetail )
    {
        m_taskIds = ToStringSet( domRootNode.GetChildrenValues() );
        return true;
    }

    m_threadPoolName = domRootNode.GetAttributeValueOrChildValueByName( "threadPoolName" ).AsString( m_threadPoolName, true );
    m_desiredMaxNrOfThreads = domRootNode.GetAttributeValueOrChildValueByName( "desiredMaxNrOfThreads" ).AsInt32( m_desiredMaxNrOfThreads, true );
    m_desiredMinNrOfWorkerThreads = domRootNode.GetAttributeValueOrChildValueByName( "desiredMinNrOfWorkerThreads" ).AsUInt32( m_desiredMinNrOfWorkerThreads, true );
    m_activeNrOfDedicatedThreads = domRootNode.GetAttributeValueOrChildValueByName( "activeNrOfDedicatedThreads" ).AsUInt32( m_activeNrOfDedicatedThreads, true );
    m_activeNrOfWorkerThreads = domRootNode.GetAttributeValueOrChildValueByName( "activeNrOfWorkerThreads" ).AsUInt32( m_activeNrOfWorkerThreads, true );
    m_allowAppThreadToWork = domRootNode.GetAttributeValueOrChildValueByName( "allowAppThreadToWork" ).AsBool( m_allowAppThreadToWork, true );
    m_queuedTaskCount = domRootNode.GetAttributeValueOrChildValueByName( "globalNrOfQueuedTasks" ).AsUInt32( m_queuedTaskCount, true );

    domRootNode.GetValuesOfChildByName( "taskIds", m_taskIds );
    domRootNode.GetValuesOfChildByName( "taskConsumerFactoryTypes", m_taskConsumerFactoryTypes );
    domRootNode.GetValuesOfChildByName( "taskDataFactoryTypes", m_taskDataFactoryTypes );

    return true;
}

/*-------------------------------------------------------------------------*/

void
CThreadPoolInfo::SetThreadPoolName( const CString& poolName )
{GUCEF_TRACE;

    m_threadPoolName = poolName;
}

/*-------------------------------------------------------------------------*/

const CString& 
CThreadPoolInfo::GetThreadPoolName( void ) const
{GUCEF_TRACE;

    return m_threadPoolName;
}

/*-------------------------------------------------------------------------*/

void 
CThreadPoolInfo::SetActiveNrOfDedicatedThreads( UInt32 nrOfThreads )
{GUCEF_TRACE;

    m_activeNrOfDedicatedThreads = nrOfThreads;
}

/*-------------------------------------------------------------------------*/

UInt32
CThreadPoolInfo::GetActiveNrOfDedicatedThreads( void ) const
{GUCEF_TRACE;

    return m_activeNrOfDedicatedThreads;
}

/*-------------------------------------------------------------------------*/

void 
CThreadPoolInfo::SetActiveNrOfWorkerThreads( UInt32 nrOfThreads )
{GUCEF_TRACE;

    m_activeNrOfWorkerThreads = nrOfThreads;
}

/*-------------------------------------------------------------------------*/

UInt32
CThreadPoolInfo::GetActiveNrOfWorkerThreads( void ) const
{GUCEF_TRACE;

    return m_activeNrOfWorkerThreads;
}

/*-------------------------------------------------------------------------*/

void
CThreadPoolInfo::SetDesiredMaxNrOfThreads( Int32 nrOfThreads )
{GUCEF_TRACE;

    m_desiredMaxNrOfThreads = nrOfThreads;
}

/*-------------------------------------------------------------------------*/

Int32
CThreadPoolInfo::GetDesiredMaxNrOfThreads( void ) const
{GUCEF_TRACE;

    return m_desiredMaxNrOfThreads;
}

/*-------------------------------------------------------------------------*/

void
CThreadPoolInfo::SetDesiredMinNrOfWorkerThreads( UInt32 nrOfThreads )
{GUCEF_TRACE;

    m_desiredMinNrOfWorkerThreads = nrOfThreads;
}

/*-------------------------------------------------------------------------*/

UInt32
CThreadPoolInfo::GetDesiredMinNrOfWorkerThreads( void ) const
{GUCEF_TRACE;

    return m_desiredMinNrOfWorkerThreads;
}

/*-------------------------------------------------------------------------*/

void
CThreadPoolInfo::SetTaskConsumerFactoryTypes( const CString::StringSet& factoryTypes )
{GUCEF_TRACE;

    m_taskConsumerFactoryTypes = factoryTypes;
}

/*-------------------------------------------------------------------------*/

const CString::StringSet& 
CThreadPoolInfo::GetTaskConsumerFactoryTypes( void ) const
{GUCEF_TRACE;

    return m_taskConsumerFactoryTypes;
}

/*-------------------------------------------------------------------------*/

CString::StringSet& 
CThreadPoolInfo::GetTaskConsumerFactoryTypes( void )
{GUCEF_TRACE;

    return m_taskConsumerFactoryTypes;
}

/*-------------------------------------------------------------------------*/

void
CThreadPoolInfo::SetTaskDataFactoryTypes( const CString::StringSet& factoryTypes )
{GUCEF_TRACE;

    m_taskDataFactoryTypes = factoryTypes;
}

/*-------------------------------------------------------------------------*/

const CString::StringSet& 
CThreadPoolInfo::GetTaskDataFactoryTypes( void ) const
{GUCEF_TRACE;

    return m_taskDataFactoryTypes;
}

/*-------------------------------------------------------------------------*/

CString::StringSet& 
CThreadPoolInfo::GetTaskDataFactoryTypes( void )
{GUCEF_TRACE;

    return m_taskDataFactoryTypes;
}

/*-------------------------------------------------------------------------*/

void
CThreadPoolInfo::SetAllowMainApplicationThreadToPickUpQueuedTasks( bool allowAppThreadToWork )
{GUCEF_TRACE;

    m_allowAppThreadToWork = allowAppThreadToWork;
}

/*-------------------------------------------------------------------------*/

bool
CThreadPoolInfo::GetAllowMainApplicationThreadToPickUpQueuedTasks( void ) const
{GUCEF_TRACE;

    return m_allowAppThreadToWork;
}

/*-------------------------------------------------------------------------*/

void
CThreadPoolInfo::SetQueuedTaskCount( UInt32 nrOfTasks )
{GUCEF_TRACE;

    m_queuedTaskCount = nrOfTasks;
}

/*-------------------------------------------------------------------------*/

UInt32
CThreadPoolInfo::GetQueuedTaskCount( void ) const
{GUCEF_TRACE;

    return m_queuedTaskCount;
}

/*-------------------------------------------------------------------------*/

const CString& 
CThreadPoolInfo::GetClassTypeName( void ) const
{GUCEF_TRACE;

    return ClassTypeName;
}

/*-------------------------------------------------------------------------*/

CICloneable* 
CThreadPoolInfo::Clone( void ) const
{GUCEF_TRACE;

    return new CThreadPoolInfo( *this );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
