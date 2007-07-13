/*
 *  gucefINPUT: GUCEF module providing input device interaction
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

#ifndef CDATANODE_H
#include "CDataNode.h"
#define CDATANODE_H
#endif /* CDATANODE_H ? */

#ifndef CINPUTPROFILE_H
#include "CInputProfile.h"
#define CINPUTPROFILE_H
#endif /* CINPUTPROFILE_H ? */

#ifndef CINPUTHANDLERMULTIMAP_H
#include "CInputHandlerMultiMap.h"
#define CINPUTHANDLERMULTIMAP_H
#endif /* CINPUTHANDLERMULTIMAP_H ? */

#ifndef CINPUTTOACTIONHANDLER_H
#include "CInputToActionHandler.h"
#define CINPUTTOACTIONHANDLER_H
#endif /* CINPUTTOACTIONHANDLER_H ? */

#ifndef CINPUTACTIONMULTIMAPHANDLER_H
#include "CInputActionMultiMapHandler.h"
#define CINPUTACTIONMULTIMAPHANDLER_H
#endif /* CINPUTACTIONMULTIMAPHANDLER_H ? */

#ifndef CINPUTBASEDMANIPULATOR_H
#include "CInputBasedManipulator.h"
#define CINPUTBASEDMANIPULATOR_H
#endif /* CINPUTBASEDMANIPULATOR_H ? */

#include "CInputProfileManager.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace INPUT {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

CInputProfileManager* CInputProfileManager::m_instance = NULL;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CProfileData
{       
        public:
        
        CProfileData( const CORE::CString& profilename )
                : profile( profilename )
        {TRACE;
        }
        
        CProfileData( const CInputProfile& otherprofile )
                : profile( otherprofile )
        {TRACE;
        }        
        
        CInputProfile profile;
        CInputHandlerMultiMap handler; 
        
        private:
        CProfileData( void );
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CInputProfileManager::CInputProfileManager( void )
    : CIConfigurable( true )
{TRACE;
}

/*-------------------------------------------------------------------------*/

CInputProfileManager::~CInputProfileManager()
{TRACE;
}

/*-------------------------------------------------------------------------*/

CInputProfileManager* 
CInputProfileManager::Instance( void )
{TRACE;
        if ( !m_instance )
        {
                m_instance = new CInputProfileManager();
        }
        return m_instance;
}

/*-------------------------------------------------------------------------*/

void
CInputProfileManager::Deinstance( void )
{TRACE;
        delete m_instance;
        m_instance = NULL;
}

/*-------------------------------------------------------------------------*/
        
void 
CInputProfileManager::SetProfile( const CInputProfile& profile )
{TRACE;
        if ( !profile.GetName().Length() ) return;
        
        UInt32 index;
        CProfileData* data( static_cast<CProfileData*>( m_profiles[ profile.GetName() ] ) );
        if ( !data )
        {
                data = new CProfileData( profile );
                index = (UInt32) m_profiles.Add( profile.GetName() ,
                                                 data              );
        }
        else
        {
                data->profile = profile;
                index = m_profiles.GetKeyIndex( profile.GetName() );
        }
        
        UpdateProfileMapping( index );
}

/*-------------------------------------------------------------------------*/
        
bool 
CInputProfileManager::AddProfile( const CInputProfile& profile )
{TRACE;
        if ( !profile.GetName().Length() ) return false;

        CProfileData* data( static_cast<CProfileData*>( m_profiles[ profile.GetName() ] ) );
        if ( !data )
        {
                data = new CProfileData( profile );
                Int32 index( m_profiles.Add( profile.GetName() ,
                                             data              ) );
                if ( index >= 0 )
                {
                        UpdateProfileMapping( index );                               
                        return true;
                }                                                                
        }
        return false;
}

/*-------------------------------------------------------------------------*/
        
const CInputProfile* 
CInputProfileManager::GetProfile( const GUCEF::CORE::CString& profilename )
{TRACE;
        CProfileData* data( static_cast<CProfileData*>( m_profiles[ profilename ] ) );
        if ( data )
        {
                return &data->profile;
        }
        return NULL;        
}

/*-------------------------------------------------------------------------*/
        
CIInputHandler* 
CInputProfileManager::GetProfileHandler( const GUCEF::CORE::CString& profilename )
{TRACE;
        CProfileData* data( static_cast<CProfileData*>( m_profiles[ profilename ] ) );
        if ( data )
        {
                return &data->handler;
        }
        return NULL;
}

/*-------------------------------------------------------------------------*/
        
bool 
CInputProfileManager::AddActionMap( const CInputActionMap& actionmap )
{TRACE;
        CInputActionMap* a = static_cast<CInputActionMap*>( m_actionmaps[ actionmap.GetName() ] );
        if ( !a )
        {
                m_actionmaps.Add( actionmap.GetName()              ,
                                  new CInputActionMap( actionmap ) );
                UpdateAllMappings();
                return true;                                        
        }
        return false;
}

/*-------------------------------------------------------------------------*/
        
void 
CInputProfileManager::SetActionMap( const CInputActionMap& actionmap )
{TRACE;
        CInputActionMap* a = static_cast<CInputActionMap*>( m_actionmaps[ actionmap.GetName() ] );
        if ( !a )
        {
                m_actionmaps.Add( actionmap.GetName()              ,
                                  new CInputActionMap( actionmap ) );                                        
        }
        else
        {
                *a = actionmap;
        }
        UpdateAllMappings();
}

/*-------------------------------------------------------------------------*/
        
CInputActionMap* 
CInputProfileManager::GetActionMap( const GUCEF::CORE::CString& actionmapname )
{TRACE;
        return static_cast<CInputActionMap*>( m_actionmaps[ actionmapname ] );
}

/*-------------------------------------------------------------------------*/

CORE::CStringList 
CInputProfileManager::GetProfileList( void ) const
{TRACE;
        return m_profiles.GetKeyList();
}

/*-------------------------------------------------------------------------*/
        
void 
CInputProfileManager::RegisterManipulator( CInputBasedManipulator* manipulator )
{TRACE;
        assert( manipulator );
        
        CORE::CDynamicArray* list( static_cast<CORE::CDynamicArray*>( m_manipulators[ manipulator->GetName() ] ) );
        if ( NULL == list )
        {
                list = new CORE::CDynamicArray();

        }
        
        list->AddEntry( manipulator );
        m_manipulators.Add( manipulator->GetName() ,
                           list                   );
                                   
        UpdateAllMappings();
}

/*-------------------------------------------------------------------------*/
        
void 
CInputProfileManager::UnregisterManipulator( CInputBasedManipulator* manipulator )
{TRACE;
        assert( manipulator );
        
        CORE::CDynamicArray* list( static_cast<CORE::CDynamicArray*>( m_manipulators[ manipulator->GetName() ] ) );
        list->RemoveEntry( manipulator );
        
        if ( list->GetCount() == 0 )
        {
                delete list;
                m_manipulators.Delete( manipulator->GetName() );
        }
        UpdateAllMappings();        
}

/*-------------------------------------------------------------------------*/

CORE::CStringList 
CInputProfileManager::GetManipulatorList( void ) const
{TRACE;
        return m_manipulators.GetKeyList();
}

/*-------------------------------------------------------------------------*/

CORE::CStringList 
CInputProfileManager::GetRefrencedManipulatorsList( void ) const
{TRACE;
        CORE::CStringList list;
        CProfileData* data( NULL );
        for ( UInt32 i=0; i<m_profiles.GetCount(); ++i )
        {
                data = static_cast<CProfileData*>( m_profiles[ i ] );
                if ( data )
                {
                        list.Merge( data->profile.GetManipulatorList() ,
                                    true                               ); 
                }
        }
        return list;                
}

/*-------------------------------------------------------------------------*/

CORE::CStringList 
CInputProfileManager::GetActionMapList( void ) const
{TRACE;
        return m_actionmaps.GetKeyList();        
}

/*-------------------------------------------------------------------------*/

void
CInputProfileManager::UpdateAllMappings( void)
{TRACE;        
        for ( UInt32 i=0; i<m_profiles.GetCount(); ++i )
        {
                UpdateProfileMapping( i );
        }        
}

/*-------------------------------------------------------------------------*/

void 
CInputProfileManager::UpdateProfileMapping( const UInt32 index )
{TRACE;
        DEBUGOUTPUTsi( "Updating input profile mapping nr.", index );
        CProfileData* data( static_cast<CProfileData*>( m_profiles[ index ] ) );
        
        /*
         *      First we erase the current mapping structure
         */
        UInt32 count( data->handler.GetCount() );
        CInputToActionHandler* inputtoactionh( NULL );
        for ( UInt32 i=0; i<count; ++i )
        {
                inputtoactionh = static_cast<CInputToActionHandler*>( data->handler.GetHandler( i ) );
                delete static_cast< CInputActionMultiMapHandler* >( inputtoactionh->GetActionHandler() );
                delete inputtoactionh;                
        }
        data->handler.Clear();
        
        /*
         *      Now we connect the desired action maps to our multi map 
         */
        CInputActionMap* actionmap( NULL ); 
        CORE::CStringList actionmaplist( data->profile.GetActionMapList() );
        DEBUGOUTPUTsi( " Action map count: ", actionmaplist.GetCount() );
        for ( UInt32 i=0; i<actionmaplist.GetCount(); ++i )
        {
                actionmap = static_cast<CInputActionMap*>( m_actionmaps[ actionmaplist[ i ] ] );
                if ( actionmap )
                {
                        /*
                         *      The desired action map exists within our system
                         *      We hook it up to our multi map using a new handler
                         *      to action handler convertor
                         */
                        CInputActionMultiMapHandler* actionhandler = new CInputActionMultiMapHandler(); 
                        data->handler.RegisterHandler( new CInputToActionHandler( *actionmap    ,
                                                                                  actionhandler ) );
                        
                        /*
                         *      We attempt to link up as many manipulators as we can find as specified
                         *      in the profile for the current action map.
                         */
                        DEBUGOUTPUTsss( " Mapping action map \"", actionmap->GetName().C_String(), "\" to manipulators" );
                        const CORE::CStringList* actionMapManipulators( data->profile.GetActionMapManipulators( actionmap->GetName() ) );  
                        if ( actionMapManipulators )
                        {
                                CInputBasedManipulator* manipulator( NULL );
                                CORE::CDynamicArray* manipulatorSet( NULL );
                                DEBUGOUTPUTsi( "  Manipulator count: ", actionMapManipulators->GetCount() ); 
                                for ( UInt32 n=0; n<actionMapManipulators->GetCount(); ++n )
                                {
                                        /*
                                         *      Obtain the list of identically named manipulators ( if any exist at all )
                                         *      manipulators can be registered by name alone and need not have any instances
                                         *      linked to them.
                                         */
                                        manipulatorSet = static_cast<CORE::CDynamicArray*>( m_manipulators[ (*actionMapManipulators)[ n ] ] );
                                        if ( manipulatorSet )
                                        {
                                                DEBUGOUTPUTsi( "   Manipulators in name-set: ", manipulatorSet->GetCount() );
                                                for ( UInt32 m=0; m<manipulatorSet->GetCount(); ++m )
                                                {
                                                        manipulator = static_cast<CInputBasedManipulator*>( (*manipulatorSet)[ i ] );
                                                        if ( manipulator )
                                                        {
                                                                actionhandler->RegisterActionHandler( manipulator );
                                                        }
                                                }
                                        }
                                        #ifdef DEBUG_MODE
                                        else
                                        {
                                                DEBUGOUTPUTsss( "   No manipulators instances registered with name \"", ((*actionMapManipulators)[ n ]).C_String(), "\"" );
                                        }
                                        #endif                                                
                                }
                        }                                
                }
        }                
}

/*-------------------------------------------------------------------------*/

bool 
CInputProfileManager::LoadConfig( const CORE::CDataNode& node )
{TRACE;
        //Clear();
        
        const CORE::CDataNode* n = node.Search( "GUCEF%INPUT%CInputProfileManager" ,
                                                '%'                                ,
                                                false                              );
        if ( n )
        {                                                
                const CORE::CDataNode *cn;
                CORE::CDataNode::const_iterator i( n->ConstBegin() );
                CInputActionMap actionmap( "NoName" );
                CInputProfile profile( "NoName" );
                while ( i != n->ConstEnd() )
                {
                        cn = (*i);
                        
                        if ( cn->GetName() == "InputActionMap" )
                        {
                                if ( actionmap.LoadConfig( *cn ) )
                                {
                                        AddActionMap( actionmap );        
                                }
                                else
                                {
                                        ++i;
                                        continue;
                                }                                             
                        }
                        else
                        {
                                if ( cn->GetName() == "InputProfile" )
                                {
                                        if ( profile.LoadConfig( *cn ) )
                                        {
                                                AddProfile( profile );
                                        }
                                        else
                                        {
                                                ++i;
                                                continue;
                                        }                                         
                                }
                        }                                
                        ++i;
                }
                return true;
        }
        return false;                                         
}

/*-------------------------------------------------------------------------*/
        
bool 
CInputProfileManager::SaveConfig( CORE::CDataNode& node )
{TRACE;
        CORE::CDataNode* n = node.Structure( "GUCEF%INPUT%CInputProfileManager" ,
                                             '%'                                );
                                             
        CORE::CString intstr;
        intstr.SetInt( m_actionmaps.GetCount() );
        n->SetAttribute( "ActionMapCount", intstr );
        intstr.SetInt( m_profiles.GetCount() );                                             
        n->SetAttribute( "ProfileCount", intstr );
        
        for ( UInt32 i=0; i<m_actionmaps.GetCount(); ++i )
        {
                static_cast<CInputActionMap*>( m_actionmaps[ i ] )->SaveConfig( *n );
        }
        for ( UInt32 i=0; i<m_profiles.GetCount(); ++i )
        {
                static_cast<CInputProfile*>( m_profiles[ i ] )->SaveConfig( *n );
        }
        return true;        
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
