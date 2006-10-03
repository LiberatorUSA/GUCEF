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

#ifndef GUCEF_CORE_CSTATEHANDLER_H
#define GUCEF_CORE_CSTATEHANDLER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CSTATEMACHINE_H
#include "CStateMachine.h"
#define GUCEF_CORE_CSTATEMACHINE_H
#endif /* GUCEF_CORE_CSTATEMACHINE_H ? */

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
 *      Abstract base class that serves as the basis for state handlers for the
 *      CStateMachine class. Override the manditory and optional event handlers 
 *      as needed to handle the machine state.
 *      Note that the return values indicate machine errors. If you return false
 *      in any of the event handlers then it will be considdered to be a fatal 
 *      machine error and the state machine will stop.
 *      After successfully handling the state you should always call SignalStateHandled()
 *      with a condition value. This will trigger a transition to a new state.
 */
class GUCEFCORE_EXPORT_CPP CStateHandler
{               
        public:
        CStateHandler( void );
        
        CStateHandler( const CStateHandler& src );
        
        virtual ~CStateHandler();
        
        CStateHandler& operator=( const CStateHandler& src );

        protected:
        friend class CStateMachine;
        
        /**
         *      @return wheter the handler encountered a fatal error        
         */
        virtual bool OnEnterState( void ) = 0;
        
        /**
         *
         */
        virtual void OnLeaveState( void );
        
        /**
         *      Can be used to process the state over a longer and interrupted period
         *      of time. If the OnEnterState() handler cannot completely handle the state
         *      (if doesnt call SignalStateHandled() before returning) then OnUpdate() 
         *      will be called periodicly untill SignalStateHandled() is called.
         *
         *      If you don't override this handler it will simply call SignalStateHandled()
         *      with condition value 0.
         *
         *      @param tickcount the tick count when the Update process commenced.
         *      @param deltaticks ticks since the last Update process commenced.
         *      @return wheter the handler encountered a fatal error         
         */         
        virtual bool OnUpdate( UInt32 tickcount  ,
                               UInt32 deltaticks );
        
        /**
         *      Called when the state machine is started.
         *      Can be used for setup purposes ect.
         *      Optional, override when needed.
         *
         *      @return wheter the handler encountered a fatal error
         */
        virtual bool OnMachineStart( void );
        
        /**
         *      Called when the state machine is stopped.
         *      Can be used for cleanup purposes ect.
         *      Optional, override when needed.         
         */        
        virtual void OnMachineStop( void );        
                
        /**
         *      Signals the state-machine that the state has been handled.
         *      This will most likely trigger a state transition on the state-machine
         *      using the condition value to select the transition.
         */
        void SignalStateHandled( UInt32 conditionvalue );
        
        void SignalStateFailure( UInt32 errorvalue          ,
                                 const CString& description );
                                 
        /**
         *      Returns the current state-machine state.
         *      Basicly a utility member function, calls the same member fuction
         *      on the state machine.
         */                                 
        UInt32 GetState( void ) const;
        
        /**
         *      Returns a pointer to the state machine to which this handler
         *      is assigned.
         */
        CStateMachine* GetMachine( void ) const;                                             
              
        private:
        friend class CStateMachine;
                
        bool _mstarted;      
        CStateMachine* _machine;                                                      
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CSTATEHANDLER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 14-05-2005 :
        - Designed and implemented this class.

-----------------------------------------------------------------------------*/
