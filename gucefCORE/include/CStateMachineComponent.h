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

#ifndef GUCEF_CORE_CSTATEMACHINECOMPONENT_H
#define GUCEF_CORE_CSTATEMACHINECOMPONENT_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <map>
#include "CObservingNotifier.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF { 
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  One of the essential building blocks for realtime applications.
 *  This class implements the 'state memory' and the decending class the
 *  transition handling based on a defined state transition table that
 *  can be copied from an STD (State Transition Diagram).
 */
class GUCEFCORE_EXPORT_CPP CStateMachineComponent : public CObservingNotifier
{
    public:
    
    explicit CStateMachineComponent( const UInt32 initialState );
    
    explicit CStateMachineComponent( const CStateMachineComponent& src );
    
    CStateMachineComponent& operator=( const CStateMachineComponent& src );
    
    virtual ~CStateMachineComponent();
    
    UInt32 GetIDOfCurrentState( void ) const;

    protected:
    
    virtual void OnStateTransition( const UInt32 oldStateID       ,
                                    const UInt32 newStateID       ,
                                    const UInt32 triggerEventID   ,
                                    CNotifier* triggerNotifier    ,
                                    CICloneable* eventdata = NULL ) = 0;
    
    void AddTransition( const UInt32 currentStateID ,
                        const UInt32 newStateID     ,
                        const UInt32 triggerEventID );
                        
    bool DeleteTransition( const UInt32 currentStateID ,
                           const UInt32 triggerEventID );
                           
    /**
     *  Event notification handler. Do not override.
     *  It processes the event id's and based on the contents of the 
     *  transition table either ignores the event or changes the state.
     *  If the case of a state change OnStateTransition() is called with
     *  all relevant information.
     *
     *  @param notifier the notifier that sent the notification
     *  @param eventid the unique event id for an event
     *  @param eventdata optional notifier defined userdata
     */
    virtual void OnNotify( CNotifier* notifier           ,
                           const UInt32 eventid          ,
                           CICloneable* eventdata = NULL );                           

    private:
    
    CStateMachineComponent( void );
                           
    private:
    typedef std::map< UInt32, UInt32 > TEventTransitions;
    typedef std::map< UInt32, TEventTransitions > TTransitionTable;
    
    TTransitionTable m_transitions;
    UInt32 m_currentState;                           
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CSTATEMACHINECOMPONENT_H  ? */
