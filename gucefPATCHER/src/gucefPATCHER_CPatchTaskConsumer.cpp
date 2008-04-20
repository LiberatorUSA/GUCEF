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

#ifndef GUCEF_PATCHER_CPATCHTASKDATA_H
#include "gucefPATCHER_CPatchTaskData.h"
#define GUCEF_PATCHER_CPATCHTASKDATA_H
#endif /* GUCEF_PATCHER_CPATCHTASKDATA_H ? */

#ifndef GUCEF_PATCHER_CPATCHMANAGER_H
#include "gucefPATCHER_CPatchManager.h"
#define GUCEF_PATCHER_CPATCHMANAGER_H
#endif /* GUCEF_PATCHER_CPATCHMANAGER_H ? */

#include "gucefPATCHER_CPatchTaskConsumer.h"

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

CPatchTaskConsumer::CPatchTaskConsumer( void )
    : CTaskConsumer()       ,
      m_patchEngine( NULL ) ,
      m_pulseGenerator()    ,
      m_pulseDriver()       ,
      m_taskName()          
{GUCEF_TRACE;

    m_pulseGenerator.SetPulseGeneratorDriver( &m_pulseDriver );
    m_patchEngine = new CPatchEngine( m_pulseGenerator );
}

/*-------------------------------------------------------------------------*/

CPatchTaskConsumer::~CPatchTaskConsumer()
{GUCEF_TRACE;

    delete m_patchEngine;
    m_patchEngine = NULL;
}

/*-------------------------------------------------------------------------*/

CString
CPatchTaskConsumer::GetType( void ) const
{GUCEF_TRACE;

    return GetTypeString();
}

/*-------------------------------------------------------------------------*/

const CString&
CPatchTaskConsumer::GetTypeString( void )
{GUCEF_TRACE;

    static CString typeName = "GUCEF::PATCHER::CPatchTaskConsumer"; 
    return typeName;
}

/*-------------------------------------------------------------------------*/
    
bool
CPatchTaskConsumer::ProcessTask( CORE::CICloneable* taskData )
{GUCEF_TRACE;
    
    CPatchTaskData* ptData = static_cast< CPatchTaskData* >( taskData );
    m_taskName = ptData->GetTaskName();    
    if ( m_patchEngine->LoadConfig( ptData->GetPatchEngineConfig() ) )
    {
        ptData->GetPatchManager().RegisterTask( this );        
        m_patchEngine->Start();
        ptData->GetPatchManager().UnregisterTask( this );
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

const CString&
CPatchTaskConsumer::GetTaskName( void ) const
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
