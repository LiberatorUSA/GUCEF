/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
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
