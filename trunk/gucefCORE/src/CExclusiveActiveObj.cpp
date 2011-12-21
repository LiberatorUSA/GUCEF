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

#ifndef GUCEF_CORE_CEXCLUSIVEACTIVATIONMANAGER_H
#include "CExclusiveActivationManager.h"
#define GUCEF_CORE_CEXCLUSIVEACTIVATIONMANAGER_H
#endif /* GUCEF_CORE_CEXCLUSIVEACTIVATIONMANAGER_H ? */

#include "CExclusiveActiveObj.h"

#ifndef GUCEF_CORE_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_ESSENTIALS_H
#endif /* GUCEF_CORE_ESSENTIALS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CExclusiveActiveObj::CExclusiveActiveObj( const CString& objTypeName )
    : m_objTypeName( objTypeName ) ,
      m_active( false )            ,
      m_registered( false )
{GUCEF_TRACE;
    
}

/*-------------------------------------------------------------------------*/
    
CExclusiveActiveObj::~CExclusiveActiveObj()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool 
CExclusiveActiveObj::Activate( void )
{GUCEF_TRACE;

    if ( m_registered )
    {
        return CCoreGlobal::Instance()->GetExclusiveActivationManager().SetActivateObj( m_objTypeName ,
                                                                                        GetObjName()  );
    }
    return false;                                                                        
}

/*-------------------------------------------------------------------------*/

bool 
CExclusiveActiveObj::Deactivate( void )
{GUCEF_TRACE;

    if ( m_registered && m_active )
    {
        return CCoreGlobal::Instance()->GetExclusiveActivationManager().DisableActivateObj( m_objTypeName );
    }
    return false;   
}

/*-------------------------------------------------------------------------*/
    
bool 
CExclusiveActiveObj::IsActive( void ) const
{GUCEF_TRACE;

    return m_active;
}

/*-------------------------------------------------------------------------*/
    
const CString&
CExclusiveActiveObj::GetObjTypeName( void ) const
{GUCEF_TRACE;

    return m_objTypeName;
}

/*-------------------------------------------------------------------------*/

void 
CExclusiveActiveObj::SetActiveState( const bool active )
{GUCEF_TRACE;

    m_active = active;
}

/*-------------------------------------------------------------------------*/

void
CExclusiveActiveObj::RegisterAsExclusiveActiveObj( void )
{GUCEF_TRACE;

    CCoreGlobal::Instance()->GetExclusiveActivationManager().RegisterObj( this );
    m_registered = true;
}

/*-------------------------------------------------------------------------*/
    
void 
CExclusiveActiveObj::UnregisterAsExclusiveActiveObj( void )
{GUCEF_TRACE;

    CCoreGlobal::Instance()->GetExclusiveActivationManager().UnregisterObj( this );
    m_registered = false;
}

/*-------------------------------------------------------------------------*/

bool
CExclusiveActiveObj::IsRegistered( void ) const
{GUCEF_TRACE;

    return m_registered;
}

/*-------------------------------------------------------------------------*/

bool 
CExclusiveActiveObj::SetAsDefault( void )
{GUCEF_TRACE;

    if ( m_registered )
    {
        return CCoreGlobal::Instance()->GetExclusiveActivationManager().SetDefaultObjForType( m_objTypeName ,
                                                                                              GetObjName()  );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CExclusiveActiveObj::IsTheDefault( void ) const
{GUCEF_TRACE;

    if ( m_registered )
    {
        CString defaultObj;
        CCoreGlobal::Instance()->GetExclusiveActivationManager().GetDefaultObjForType( m_objTypeName ,
                                                                                       defaultObj    );
        return defaultObj == GetObjName();
    }
    return false;
}

/*-------------------------------------------------------------------------*/

void 
CExclusiveActiveObj::OnPostActivate( const bool /* activationSuccessfull */ )
{GUCEF_TRACE;

    /*
     *  Don't do anything,.. only implemented here to avoid making implementation
     *  manditory for decending classes.
     */
}

/*-------------------------------------------------------------------------*/

void
CExclusiveActiveObj::OnPostDeactivate( const bool /* deactivationSuccessful */ )
{GUCEF_TRACE;

    /*
     *  Don't do anything,.. only implemented here to avoid making implementation
     *  manditory for decending classes.
     */
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
