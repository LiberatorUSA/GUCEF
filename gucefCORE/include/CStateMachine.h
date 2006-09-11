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

#ifndef GUCEF_CORE_CSTATEMACHINE_H
#define GUCEF_CORE_CSTATEMACHINE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"
#define GUCEF_CORE_CDVSTRING_H
#endif /* GUCEF_CORE_CDVSTRING_H ? */

#ifndef GUCEF_CORE_CDYNAMICARRAY_H
#include "CDynamicArray.h"
#define GUCEF_CORE_CDYNAMICARRAY_H
#endif /* GUCEF_CORE_CDYNAMICARRAY_H ? */ 

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

class CErrorStateHandler;
class CMachineState;
class CStateHandler;

/*-------------------------------------------------------------------------*/

/**
 *      State machine implementation.
 *      Once set in motion the state machine will simply go trough the state handing
 *      path created by the user. The path can vary depending on the condition values
 *      that result from the handlers actions.
 *      
 *      Everything works with integers for performance reasons. 
 *      It is recommended to make an int enum of all your states and condition
 *      values and use those. The value 0 is reserved and should be considdered to have
 *      the following meaning:
 *              - condition value 0 : no condition
 *              - state value 0     : machine default non-state/invalid state
 *
 *      Note that the user remains responible for memory management of
 *      the CStateHandler derived objects. The state machine only uses them, 
 *      it does not assume ownership.
 */
class EXPORT_CPP CStateMachine
{
        public:
        
        CStateMachine( void );
        
        CStateMachine( const CStateMachine& src );
        
        ~CStateMachine();
        
        CStateMachine& operator=( const CStateMachine& src );               
        
        bool Start( const UInt32 initialstate );
        
        void Stop( void );
        
        /**
         *      Resets the state machine to the initial state
         *      and re-starts the machine. Basicly a Stop() Start()
         */
        bool Reset( void );
        
        /**
         * 
         *
         *      @param tickcount the tick count when the Update process commenced.
         *      @param deltaticks ticks since the last Update process commenced.
         *      @return wheter the handler encountered a fatal error         
         */         
        bool Update( UInt32 tickcount  ,
                     UInt32 deltaticks );        

        bool AddState( UInt32 state           ,
                       CStateHandler* handler );
        
        bool DelState( UInt32 state );

        bool SetTransition( const UInt32 oldstate       ,
                            const UInt32 newstate       ,
                            const UInt32 conditionvalue );

        bool GetTransition( const UInt32 oldstateid     ,
                            const UInt32 conditionvalue ,
                            UInt32& newstate            );
                            
        UInt32 GetCurrentState( void ) const;
                            
        void SetErrorHandler( CErrorStateHandler* errorhandler );
                            
        void Clear( void );                                                 
                
        private:                
        friend class CMachineState;

        CMachineState* LookupStateObj( UInt32 state ) const;
                             
        private:
        friend class CStateHandler;
        
        void OnStateHandled( CStateHandler* handler ,
                             UInt32 conditionvalue  );
                             
        void OnStateFailure( UInt32 errorvalue          ,
                             const CString& description );
                             
        private:
        UInt32 _initialstate;
        CMachineState* _curstate;
        bool _updatestate;
        CDynamicArray _states;        
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CSTATEMACHINE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 14-05-2005 :
        - Designed and implemented this class.

-----------------------------------------------------------------------------*/