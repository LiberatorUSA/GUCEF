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

#define GUCEF_CORE_CSTATEMACHINECOMPONENT_CPP
#include "CStateMachineComponent.h"

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

CStateMachineComponent::CStateMachineComponent( void )
    : m_currentState( 0UL )
{
}

/*-------------------------------------------------------------------------*/
    
CStateMachineComponent::CStateMachineComponent( const CStateMachineComponent& src )
    : m_currentState( src.m_currentState ) ,
      m_transitions( src.m_transitions )
{
}

/*-------------------------------------------------------------------------*/
    
CStateMachineComponent& 
CStateMachineComponent::operator=( const CStateMachineComponent& src )
{
    if ( &src != this )
    {
        m_currentState = src.m_currentState;
        m_transitions = src.m_transitions;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/
    
CStateMachineComponent::~CStateMachineComponent()
{

}

/*-------------------------------------------------------------------------*/
    
UInt32 
CStateMachineComponent::GetIDOfCurrentState( void ) const
{
    return m_currentState;
}

/*-------------------------------------------------------------------------*/

void 
CStateMachineComponent::AddTransition( const UInt32 currentStateID ,
                                       const UInt32 newStateID     ,
                                       const UInt32 triggerEventID )
{
    TEventTransitions& trans = m_transitions[ currentStateID ];
    trans[ triggerEventID ] = newStateID;
}                                       

/*-------------------------------------------------------------------------*/
                        
bool 
CStateMachineComponent::DeleteTransition( const UInt32 currentStateID ,
                                          const UInt32 triggerEventID )
{
    TTransitionTable::iterator i( m_transitions.find( currentStateID ) );
    if ( i != m_transitions.end() )
    {
        TEventTransitions& trans = (*i).second;
        TEventTransitions::iterator n( trans.find( triggerEventID ) );
        if ( n != trans.end() )
        {
            trans.erase( n );
            return true;
        }
        return false;
    }
    return false;
}   

/*-------------------------------------------------------------------------*/                                       
                           
void 
CStateMachineComponent::OnNotify( CNotifier* notifier                 ,
                                  const UInt32 eventid                ,
                                  CICloneable* eventdata /* = NULL */ )
{
    /*
     *  First we get the transition table for the current state
     */
    TEventTransitions& trans = m_transitions[ m_currentState ];
    
    /*
     *  Now we try to find a transition for the given eventID combined
     *  with our current state's transition table.
     */
    TEventTransitions::iterator n( trans.find( eventid ) );
    if ( n != trans.end() )
    {
        /*
         *  Found transition, set the new state
         */
        UInt32 oldState( m_currentState );
        m_currentState = (*n).second;
        
        /*
         *  Inform the decending class
         */
        OnStateTransition( oldState       ,
                           m_currentState ,
                           eventid        ,
                           notifier       ,
                           eventdata      );
    }
    
    /* else 
     * {
     *
     *  if the 'if' failed then we are unable to process the given event
     *  in the current state and the event is ignored.
     *
     *  }
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
