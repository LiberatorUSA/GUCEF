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

#include "CMachineState.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

struct SStateTransition
{
        UInt32 state;
        UInt32 conditionvalue;
};
typedef struct SStateTransition TStateTransition;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

CMachineState::CMachineState( void )
{
        /* dummy, should not be used */
}

/*-------------------------------------------------------------------------*/

CMachineState::CMachineState( UInt32 state           ,
                              CStateHandler* handler )
        : _state( state )     ,
          _handler( handler ) 
{

}

/*-------------------------------------------------------------------------*/
                          
CMachineState::CMachineState( const CMachineState& src )
         : _state( src._state )     ,
           _handler( src._handler )
{
        /* dummy, should not be used */        
}

/*-------------------------------------------------------------------------*/

CMachineState::~CMachineState()
{
        for ( UInt32 i=0; i<_transitions.GetCount(); ++i )
        {
                delete static_cast<TStateTransition*>(_transitions[ i ]);
        }
}

/*-------------------------------------------------------------------------*/

CMachineState& 
CMachineState::operator=( const CMachineState& src )
{
        /* dummy, should not be used */              
        return *this;
}

/*-------------------------------------------------------------------------*/

UInt32 
CMachineState::GetState( void ) const
{
        return _state;
}

/*-------------------------------------------------------------------------*/

CStateHandler* 
CMachineState::GetHandler( void ) const
{
        return _handler;
}

/*-------------------------------------------------------------------------*/

bool 
CMachineState::AddTransition( UInt32 state          ,
                              UInt32 conditionvalue )
{
        if ( !GetTransition( conditionvalue ) )
        {
                TStateTransition* trans = new TStateTransition;
                trans->conditionvalue = conditionvalue;
                trans->state = state;
                _transitions.AppendEntry( trans );
                return true;        
        }
        return false;                
}                              

/*-------------------------------------------------------------------------*/
        
UInt32 
CMachineState::GetTransition( UInt32 conditionvalue )
{
        TStateTransition* trans;
        for ( UInt32 i=0; i<_transitions.GetCount(); ++i )
        {
                trans = static_cast<TStateTransition*>( _transitions[ i ] );
                if ( trans->conditionvalue == conditionvalue )
                {
                        return trans->state;
                }                        
        }
        return 0;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
