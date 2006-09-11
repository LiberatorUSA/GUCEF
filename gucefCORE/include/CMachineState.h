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

#ifndef GUCEF_CORE_CMACHINESTATE_H
#define GUCEF_CORE_CMACHINESTATE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CSTATEHANDLER_H
#include "CStateHandler.h"
#define GUCEF_CORE_CSTATEHANDLER_H
#endif /* GUCEF_CORE_CSTATEHANDLER_H ? */

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
 *      Internally used class for the CStateMachine class.
 *      Represents a system state and stores the state transition 
 *      pathways.
 */
class CMachineState
{
        public:
        
        CMachineState( UInt32 state           ,
                       CStateHandler* handler );

        virtual ~CMachineState();
        
        UInt32 GetState( void ) const;
        
        CStateHandler* GetHandler( void ) const;
        
        bool AddTransition( UInt32 state          ,
                            UInt32 conditionvalue );
        
        /**
         *      Returns the new state if this state is handled
         *      with the given condition.
         *      In case of error 0 is returned.
         *
         *      @param conditionvalue the state handler condition
         *      @return the new state for the given condition
         */
        UInt32 GetTransition( UInt32 conditionvalue );        
        
        private:
        CMachineState( void );
        CMachineState( const CMachineState& src );    
        CMachineState& operator=( const CMachineState& src );    
                
        CDynamicArray _transitions;
        CStateHandler* _handler;
        UInt32 _state;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CMACHINESTATE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 14-05-2005 :
        - Designed and implemented this class.

-----------------------------------------------------------------------------*/