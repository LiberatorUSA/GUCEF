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

#include "CExclusiveActivationManager.h"
#include "CExclusiveActiveObj.h"

#pragma warning( disable: 4786 ) // identifier was truncated to 'number' characters
#pragma warning( disable: 4251 ) // 'classname' needs to have dll-interface to be used by clients of class 'classname'
#pragma warning( disable: 4284 )

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

CExclusiveActiveObj::CExclusiveActiveObj( const std::string& objTypeName )
    : m_objTypeName( objTypeName ) ,
      m_active( false )            ,
      m_registered( false )
{
    
}

/*-------------------------------------------------------------------------*/
    
CExclusiveActiveObj::~CExclusiveActiveObj()
{

}

/*-------------------------------------------------------------------------*/

bool 
CExclusiveActiveObj::Activate( void )
{
    if ( m_registered )
    {
        return CExclusiveActivationManager::Instance()->SetActivateObj( m_objTypeName ,
                                                                        GetObjName()  );
    }
    return false;                                                                        
}

/*-------------------------------------------------------------------------*/

bool 
CExclusiveActiveObj::Deactivate( void )
{
    if ( m_registered && m_active )
    {
        return CExclusiveActivationManager::Instance()->DisableActivateObj( m_objTypeName );
    }
    return false;   
}

/*-------------------------------------------------------------------------*/
    
bool 
CExclusiveActiveObj::IsActive( void ) const
{
    return m_active;
}

/*-------------------------------------------------------------------------*/
    
const std::string&
CExclusiveActiveObj::GetObjTypeName( void ) const
{
    return m_objTypeName;
}

/*-------------------------------------------------------------------------*/

void 
CExclusiveActiveObj::SetActiveState( const bool active )
{
    m_active = active;
}

/*-------------------------------------------------------------------------*/

void
CExclusiveActiveObj::RegisterAsExclusiveActiveObj( void )
{
    CExclusiveActivationManager::Instance()->RegisterObj( this );
    m_registered = true;
}

/*-------------------------------------------------------------------------*/
    
void 
CExclusiveActiveObj::UnregisterAsExclusiveActiveObj( void )
{
    CExclusiveActivationManager::Instance()->UnregisterObj( this );
    m_registered = false;
}

/*-------------------------------------------------------------------------*/

bool
CExclusiveActiveObj::IsRegistered( void ) const
{
    return m_registered;
}

/*-------------------------------------------------------------------------*/

bool 
CExclusiveActiveObj::SetAsDefault( void )
{
    if ( m_registered )
    {
        return CExclusiveActivationManager::Instance()->SetDefaultObjForType( m_objTypeName ,
                                                                              GetObjName()  );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CExclusiveActiveObj::IsTheDefault( void ) const
{
    if ( m_registered )
    {
        std::string defaultObj;
        CExclusiveActivationManager::Instance()->GetDefaultObjForType( m_objTypeName ,
                                                                       defaultObj    );
        return defaultObj == GetObjName();
    }
    return false;
}

/*-------------------------------------------------------------------------*/

void 
CExclusiveActiveObj::OnPostActivate( const bool /* activationSuccessfull */ )
{
    /*
     *  Don't do anything,.. only implemented here to avoid making implementation
     *  manditory for decending classes.
     */
}

/*-------------------------------------------------------------------------*/

void
CExclusiveActiveObj::OnPostDeactivate( const bool /* deactivationSuccessful */ )
{
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
