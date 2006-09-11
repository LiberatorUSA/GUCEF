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

#include <assert.h>
#include "CExclusiveActiveObj.h"

#define GUCEF_CORE_CEXCLUSIVEACTIVATIONMANAGER_CPP
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
{
}

/*-------------------------------------------------------------------------*/

CExclusiveActivationManager::~CExclusiveActivationManager()
{
}

/*-------------------------------------------------------------------------*/

CExclusiveActivationManager* 
CExclusiveActivationManager::Instance( void )
{
    if ( !m_instance )
    {
        m_instance = new CExclusiveActivationManager();
    }
    return m_instance;	//TICS !GOOP-002: is Singleton pattern
}
    
/*-------------------------------------------------------------------------*/

bool 
CExclusiveActivationManager::SetActivateObj( const std::string& objTypeName ,
                                             const std::string& objName     )
{
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
{
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
{
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
{
    delete m_instance;
    m_instance = NULL;
}

/*-------------------------------------------------------------------------*/

void 
CExclusiveActivationManager::RegisterObj( CExclusiveActiveObj* newObj )
{
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
{
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
{
    return (UInt32) m_typeList.size();
}

/*-------------------------------------------------------------------------*/

bool 
CExclusiveActivationManager::SetDefaultObjForType( const std::string& objTypeName ,
                                                   const std::string& objName     )
{
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
{
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
