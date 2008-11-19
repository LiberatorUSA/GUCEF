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
