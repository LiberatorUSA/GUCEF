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

#include "CInputProfile.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace INPUT {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/
        
CInputProfile::CInputProfile( void )
{TRACE;
        assert( 0 );
}

/*-------------------------------------------------------------------------*/
        
CInputProfile::CInputProfile( const CORE::CString& profilename )
        : m_name( profilename )
{TRACE;
        m_manipulators.SetAllowDuplicates( false );
}

/*-------------------------------------------------------------------------*/
        
CInputProfile::CInputProfile( const CInputProfile& src )
{TRACE;
        for ( UInt32 i=0; i<src.m_map.GetCount(); ++i )
        {
                m_map.Add( src.m_map.GetKey( i )                                                       ,
                           new CORE::CStringList( *static_cast<CORE::CStringList*>( src.m_map[ i ] ) ) );
        }
}

/*-------------------------------------------------------------------------*/
        
CInputProfile::~CInputProfile()
{TRACE;
        Clear();
}

/*-------------------------------------------------------------------------*/
        
CInputProfile& 
CInputProfile::operator=( const CInputProfile& src )
{TRACE;
        if ( this != &src )
        {
                Clear();
                
                for ( UInt32 i=0; i<src.m_map.GetCount(); ++i )
                {
                        m_map.Add( src.m_map.GetKey( i )                                                       ,
                                   new CORE::CStringList( *static_cast<CORE::CStringList*>( src.m_map[ i ] ) ) );
                }                
        }
        return *this;
}

/*-------------------------------------------------------------------------*/
        
bool 
CInputProfile::SetActionMapLink( const CORE::CString& actionmapname   ,
                                 const CORE::CString& manipulatorname )
{TRACE;
        /*
         *      check for valid input
         */
        if ( !actionmapname.Length() || !manipulatorname.Length() )
        {
                return false;
        }
        
        /*
         *      Check if the manipulator is already linked to an action map
         */
        if ( m_manipulators.Find( manipulatorname ) >= 0 )
        {
                /*
                 *      Get rid of the old mapping
                 */        
                CORE::CString actionmap( GetManipulatorActionMap( manipulatorname ) );
                if ( actionmap != actionmapname )
                {
                        CORE::CStringList* list( static_cast<CORE::CStringList*>( m_map[ actionmap ] ) );
                        list->Delete( list->Find( manipulatorname ) );
                }
                else
                {
                        return true;
                }                        
        }

        /*
         *      Create a new mapping
         */
        CORE::CStringList* list( static_cast<CORE::CStringList*>( m_map.Get( actionmapname ) ) );
        if ( list )
        {
                list->Append( manipulatorname );                        
        }
        else
        {                
                list = new CORE::CStringList();
                list->SetAllowDuplicates( false );
                list->Append( manipulatorname );
                m_map.Add( actionmapname ,
                           list          );
        }
        m_manipulators.Append( manipulatorname );
        return true;                              
}

/*-------------------------------------------------------------------------*/                                 
                               
CORE::CStringList 
CInputProfile::GetActionMapList( void ) const
{TRACE;
        return m_map.GetKeyList();
}

/*-------------------------------------------------------------------------*/
        
const CORE::CString& 
CInputProfile::GetName( void ) const
{TRACE;
        return m_name;
}

/*-------------------------------------------------------------------------*/
        
CORE::CString 
CInputProfile::GetManipulatorActionMap( const CORE::CString& manipulatorname ) const
{TRACE;
        CORE::CStringList* list;
        for ( UInt32 i=0; i<m_map.GetCount(); ++i )
        {
                list = static_cast<CORE::CStringList*>( m_map[ i ] );
                if ( list->Find( manipulatorname ) >= 0 )
                {
                        return m_map.GetKey( i ); 
                }       
        }
        CORE::CString dummy;
        return dummy;
}

/*-------------------------------------------------------------------------*/
        
const CORE::CStringList* 
CInputProfile::GetActionMapManipulators( const CORE::CString& actionmapname ) const
{TRACE;
        return static_cast<CORE::CStringList*>( m_map.Get( actionmapname ) );
}

/*-------------------------------------------------------------------------*/

void 
CInputProfile::Clear( void )
{TRACE;
        for ( UInt32 i=0; i<m_map.GetCount(); ++i )
        {
                delete static_cast<CORE::CStringList*>( m_map[ i ] );
        }
        m_map.DeleteAll();        
}

/*-------------------------------------------------------------------------*/

UInt32 
CInputProfile::GetActionMapCount( void ) const
{TRACE;
        return m_map.GetCount();
}

/*-------------------------------------------------------------------------*/
        
UInt32 
CInputProfile::GetManipulatorCount( void ) const
{TRACE;
        return m_manipulators.GetCount();
}

/*-------------------------------------------------------------------------*/

const CORE::CStringList& 
CInputProfile::GetManipulatorList( void ) const
{TRACE;
        return m_manipulators;
}

/*-------------------------------------------------------------------------*/

bool 
CInputProfile::UsesActionMap( const CORE::CString& actionmapname ) const
{TRACE;
        return NULL != m_map[ actionmapname ];
}

/*-------------------------------------------------------------------------*/

bool 
CInputProfile::LoadConfig( const CORE::CDataNode& node )
{TRACE;        
        Clear();
        
        const CORE::CDataNode* n = node.Search( "InputProfile" ,
                                                '%'            ,
                                                true           );
        if ( n )
        {                                                
                const CORE::CDataNode::TNodeAtt* att = n->GetAttribute( "name" );
                if ( att )
                {                
                        m_name = att->value;
                        const CORE::CDataNode *cn, *cnn;
                        CORE::CDataNode::const_iterator i( n->ConstBegin() );
                        CORE::CString actionmapname;
                        while ( i != n->ConstEnd() )
                        {
                                cn = (*i);
                                if ( cn->GetName() == "ActionMap" )
                                {
                                        att = n->GetAttribute( "name" );
                                        if ( att )
                                        {
                                                actionmapname = att->value;
                                                
                                                CORE::CDataNode::const_iterator m( cn->ConstBegin() );
                                                while ( m != cn->ConstEnd() )
                                                {
                                                        cnn = (*m);
                                                        if ( cnn->GetName() == "Manipulator" )
                                                        {
                                                                att = cnn->GetAttribute( "name" );
                                                                if ( att )
                                                                {
                                                                        SetActionMapLink( actionmapname ,
                                                                                          att->value    );
                                                                }
                                                        }                                                        
                                                        ++m;
                                                }
                                        }                                                
                                }                                
                                ++i;
                        } 
                }
                return true;                      
        }
        return false;                
}

/*-------------------------------------------------------------------------*/

bool 
CInputProfile::SaveConfig( CORE::CDataNode& node )
{TRACE;
        CORE::CDataNode* n = node.Structure( "InputProfile" ,
                                             '%'            );                     
        n->SetAttribute( "name", m_name );
        CORE::CDataNode mapnode( "ActionMap" );
        CORE::CDataNode linknode( "Manipulator" );
        CORE::CStringList* list( NULL );
                
        for ( UInt32 i=0; i<m_map.GetCount(); ++i )
        {
                mapnode.SetAttribute( "name", m_map.GetKey( i ) );
                CORE::CDataNode* ln = n->AddChild( mapnode );
                
                list = static_cast<CORE::CStringList*>( m_map[ i ] );
                for ( UInt32 n=0; n<list->GetCount(); ++n )
                {
                        linknode.SetAttribute( "name", (*list)[ n ] );
                        ln->AddChild( linknode );
                }                        
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
