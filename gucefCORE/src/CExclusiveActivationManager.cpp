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

#include <assert.h>

#include "CExclusiveActiveObj.h"

#ifndef GUCEF_CORE_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_ESSENTIALS_H
#endif /* GUCEF_CORE_ESSENTIALS_H ? */

#include "CExclusiveActivationManager.h"

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

CExclusiveActivationManager* CExclusiveActivationManager::m_instance = NULL;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CExclusiveActivationManager::CExclusiveActivationManager( void )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CExclusiveActivationManager::~CExclusiveActivationManager()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CExclusiveActivationManager* 
CExclusiveActivationManager::Instance( void )
{GUCEF_TRACE;

    if ( !m_instance )
    {
        m_instance = new CExclusiveActivationManager();
        GUCEF_SYSTEM_LOG( 0, "GUCEF::CORE::CExclusiveActivationManager Singleton created" );
    }
    return m_instance;
}
    
/*-------------------------------------------------------------------------*/

bool 
CExclusiveActivationManager::SetActivateObj( const std::string& objTypeName ,
                                             const std::string& objName     )
{GUCEF_TRACE;

    /*
     *  Validate the input
     */
    if ( !objTypeName.size() || !objName.size() ) return false;
    
    /*
     *  Make sure the requested type is valid
     */
    TObjTypeList::iterator typeIterator( m_typeList.find( objTypeName ) ); 
    if ( typeIterator != m_typeList.end() )
    {
        /*
         *  Make sure we do not activate an object twice 
         */
        TExclusiveObjList& exObjList = (*typeIterator).second;
        CExclusiveActiveObj* exObj = (*typeIterator).second.activeObj;
        if ( exObj )
        {
            if ( exObj->GetObjName() == objName )
            {
                return true;
            }
        }            

        /*
         *  Attempt to find the requested object for activation
         */
        TTypedObjList::iterator objIterator( exObjList.objList.find( objName ) );
        if ( objIterator != exObjList.objList.end() )
        {                    
            /*
             *  Attempt to deactivate the current active object
             */
            CExclusiveActiveObj* prevActive = exObj;
            if ( exObjList.activeObj )
            {
                if ( exObjList.activeObj->OnDeactivate() )
                {
                    exObjList.activeObj->SetActiveState( false );   
                    exObjList.activeObj->OnPostDeactivate( true );
                    exObjList.activeObj = NULL;
                }
                else
                {
                    exObjList.activeObj->OnPostDeactivate( false );
                    return false;
                }
            }
            
            /*
             *  Attempt to activate the requested object
             */
            exObj = (*objIterator).second;
            if ( exObj->OnActivate() )
            {
                /*
                 *  Activation successful !!!
                 */
                exObjList.activeObj = exObj;
				exObj->SetActiveState( true );
                exObj->OnPostActivate( true );
                return true;    
            }
            else
            {
                exObj->OnPostActivate( false );
                
                /*
                 *  Activation failed, fall back to the previously active object
                 *  If that fails then we won't have an active object anymore.
                 */
                if ( prevActive->OnActivate() )
                {
                    exObjList.activeObj = prevActive;                            
					prevActive->SetActiveState( true );
                    prevActive->OnPostActivate( true );
                }
                else
                {
                    prevActive->OnPostActivate( false );

                    /*
                     *  Our last resort is to fall back to the family default
                     */
                    if ( prevActive->GetObjName() != exObjList.defaultObj )
                    {
                        return SetActivateObj( objTypeName          ,
                                               exObjList.defaultObj );
                    }
                    return false;
                }                
            }
        }                    
        return false;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CExclusiveActivationManager::DisableActivateObj( const std::string& objTypeName )    
{GUCEF_TRACE;

    std::string defaultObj;
    if ( GetDefaultObjForType( objTypeName , 
                               defaultObj  ) )
    {                                
        return SetActivateObj( objTypeName , 
                               defaultObj  );
    }
    return false;
}

/*-------------------------------------------------------------------------*/

CExclusiveActiveObj* 
CExclusiveActivationManager::GetActiveObject( const std::string& objTypeName ) const
{GUCEF_TRACE;

    TObjTypeList::const_iterator typeItterator( m_typeList.find( objTypeName ) ); 
    if ( typeItterator != m_typeList.end() )
    {
        return (*typeItterator).second.activeObj;
    }
    return NULL;
}

/*-------------------------------------------------------------------------*/
    
void 
CExclusiveActivationManager::Deinstance( void )
{GUCEF_TRACE;

    delete m_instance;
    m_instance = NULL;
    GUCEF_SYSTEM_LOG( 0, "GUCEF::CORE::CExclusiveActivationManager Singleton destroyed" );
}

/*-------------------------------------------------------------------------*/

void 
CExclusiveActivationManager::RegisterObj( CExclusiveActiveObj* newObj )
{GUCEF_TRACE;

    #ifdef _DEBUG
    assert( newObj );
    #endif
    
    /* 
     *  First we get a pointer to the list of mutual exclusive objects for the
     *  given type
     */
    TExclusiveObjList* exObjList = NULL;
    TObjTypeList::iterator typeItterator( m_typeList.find( newObj->GetObjTypeName() ) ); 
    if ( typeItterator == m_typeList.end() )
    {
        /* 
         *  new object type, add some initialization
         */
        exObjList = &m_typeList[ newObj->GetObjTypeName() ];
        exObjList->activeObj = NULL;
    }
    else
    {
        /*
         *  Known type, simply retrieve a pointer
         */
        exObjList = &(*typeItterator).second;
    }

    #ifdef _DEBUG
    /*
     *  If this assert fails you have more then one object with the same name
     */
    assert( exObjList->objList.find( newObj->GetObjName() ) == exObjList->objList.end() );
    #endif

    exObjList->objList[ newObj->GetObjName() ] = newObj;
    newObj->SetActiveState( false );        
}

/*-------------------------------------------------------------------------*/
    
void 
CExclusiveActivationManager::UnregisterObj( CExclusiveActiveObj* theObj )
{GUCEF_TRACE;

    #ifdef _DEBUG
    assert( theObj );
    #endif
    
    TExclusiveObjList& exObjList = m_typeList[ theObj->GetObjTypeName() ];
    if ( exObjList.activeObj == theObj )
    {
        if ( !SetActivateObj( theObj->GetObjTypeName() ,
                              exObjList.defaultObj     ) )
        {
            exObjList.activeObj = NULL;
        }
    }
    
    exObjList.objList.erase( exObjList.objList.find( theObj->GetObjName() ) );
    
    if ( 0 == exObjList.objList.size() )
    {
        m_typeList.erase( m_typeList.find( theObj->GetObjTypeName() ) );    
    }
}

/*-------------------------------------------------------------------------*/

UInt32 
CExclusiveActivationManager::GetRegisteredTypeCount( void ) const
{GUCEF_TRACE;

    return (UInt32) m_typeList.size();
}

/*-------------------------------------------------------------------------*/

bool 
CExclusiveActivationManager::SetDefaultObjForType( const std::string& objTypeName ,
                                                   const std::string& objName     )
{GUCEF_TRACE;

    TObjTypeList::iterator typeItterator( m_typeList.find( objTypeName ) ); 
    if ( typeItterator != m_typeList.end() )
    {
        (*typeItterator).second.defaultObj = objName;
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CExclusiveActivationManager::GetDefaultObjForType( const std::string& objTypeName ,
                                                   std::string& objName           )
{GUCEF_TRACE;

    TObjTypeList::const_iterator typeItterator( m_typeList.find( objTypeName ) ); 
    if ( typeItterator != m_typeList.end() )
    {
        objName = (*typeItterator).second.defaultObj;
        return true;
    }
    return false;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
