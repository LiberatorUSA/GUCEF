/*
 *  gucefPATCHER: GUCEF RAD module providing a patch delivery system
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

#include "gucefPATCHER_CPatchTaskData.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PATCHER {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CPatchTaskData::CPatchTaskData( CPatchManager& patchManager       ,
                                const CORE::CDataNode& configData ,
                                const CString& taskName           )
    : CICloneable()                   ,
      m_patchManager( &patchManager ) ,
      m_configData( configData )      ,
      m_taskName( taskName )          
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CPatchTaskData::CPatchTaskData( const CPatchTaskData& src  )
    : CICloneable( src )                   ,
      m_patchManager( src.m_patchManager ) ,
      m_configData( src.m_configData )     ,
      m_taskName( src.m_taskName )         
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CPatchTaskData&
CPatchTaskData::operator=( const CPatchTaskData& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        m_patchManager = src.m_patchManager;
        m_configData.Copy( src.m_configData );
        m_taskName = src.m_taskName;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

CPatchTaskData::~CPatchTaskData()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CORE::CICloneable*
CPatchTaskData::Clone( void ) const
{GUCEF_TRACE;
    
    return new CPatchTaskData( *this );
}

/*-------------------------------------------------------------------------*/

void
CPatchTaskData::SetPatchManager( CPatchManager& patchManager )
{GUCEF_TRACE;

    m_patchManager = &patchManager; 
}

/*-------------------------------------------------------------------------*/
    
CPatchManager&
CPatchTaskData::GetPatchManager( void ) const
{GUCEF_TRACE;

    return *m_patchManager;
}

/*-------------------------------------------------------------------------*/
    
void
CPatchTaskData::SetPatchEngineConfig( const CORE::CDataNode& configData )
{GUCEF_TRACE;

    m_configData.Copy( configData );
}

/*-------------------------------------------------------------------------*/
    
const CORE::CDataNode&
CPatchTaskData::GetPatchEngineConfig( void ) const
{GUCEF_TRACE;

    return m_configData;
}

/*-------------------------------------------------------------------------*/

const CString&
CPatchTaskData::GetTaskName( void ) const
{GUCEF_TRACE;

    return m_taskName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace PATCHER */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
