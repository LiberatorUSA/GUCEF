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

#include "gucefCORE_CThreadlnfo.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

const CString CThreadInfo::ClassTypeName = "GUCEF::CORE::CThreadInfo";

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CThreadInfo::CThreadInfo( void )
    : CIDataNodeSerializable()
    , m_threadId( 0 )
    , m_threadStatus( MT::THREADSTATUS_UNDEFINED )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CThreadInfo::CThreadInfo( const CThreadInfo& src )
    : CIDataNodeSerializable( src )
    , m_threadId( src.m_threadId )
    , m_threadStatus( src.m_threadStatus )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CThreadInfo::~CThreadInfo()
{GUCEF_TRACE;

    Clear();
}

/*-------------------------------------------------------------------------*/

void
CThreadInfo::Clear( void )
{GUCEF_TRACE;

    m_threadId = 0;
    m_threadStatus = MT::THREADSTATUS_UNDEFINED;
}

/*-------------------------------------------------------------------------*/

bool 
CThreadInfo::Serialize( CDataNode& domRootNode                        ,
                        const CDataNodeSerializableSettings& settings ) const
{GUCEF_TRACE;

    bool totalSuccess = true;
        
    totalSuccess = domRootNode.SetAttribute( "threadId", m_threadId ) && totalSuccess;
    totalSuccess = domRootNode.SetAttribute( "threadStatusId", (UInt8) m_threadStatus ) && totalSuccess;

    if ( CDataNodeSerializableSettings::DataNodeSerializableLod_MinimumDetails < settings.levelOfDetail )
    {
        totalSuccess = domRootNode.SetAttribute( "threadStatus", MT::ThreadStatusToThreadStatusString( m_threadStatus ) ) && totalSuccess;
    }

    return totalSuccess;
}

/*-------------------------------------------------------------------------*/

bool
CThreadInfo::Deserialize( const CDataNode& domRootNode                  ,
                          const CDataNodeSerializableSettings& settings ) 
{GUCEF_TRACE;

    Clear();
    
    m_threadId = domRootNode.GetAttributeValueOrChildValueByName( "threadId" ).AsUInt32( m_threadId, true );
    m_threadStatus = (TThreadStatus) domRootNode.GetAttributeValueOrChildValueByName( "threadStatusId" ).AsUInt8( (UInt8) m_threadStatus, true );

    return true;
}

/*-------------------------------------------------------------------------*/

void 
CThreadInfo::SetThreadId( UInt32 threadId )
{GUCEF_TRACE;

    m_threadId = threadId;
}

/*-------------------------------------------------------------------------*/

UInt32
CThreadInfo::GetThreadId( void ) const
{GUCEF_TRACE;

    return m_threadId;
}

/*-------------------------------------------------------------------------*/

void 
CThreadInfo::SetThreadStatus( TThreadStatus threadStatus )
{GUCEF_TRACE;

    m_threadStatus = threadStatus;
}

/*-------------------------------------------------------------------------*/

TThreadStatus
CThreadInfo::GetThreadStatus( void ) const
{GUCEF_TRACE;

    return m_threadStatus;
}

/*-------------------------------------------------------------------------*/

const CString& 
CThreadInfo::GetClassTypeName( void ) const
{GUCEF_TRACE;

    return ClassTypeName;
}

/*-------------------------------------------------------------------------*/

CICloneable* 
CThreadInfo::Clone( void ) const
{GUCEF_TRACE;

    return new CThreadInfo( *this );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
