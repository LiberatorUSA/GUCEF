/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL, 
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER 
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF 
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT 
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
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
