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

#ifndef GUCEF_CORE_CMACHINESTATE_H
#include "CMachineState.h"
#define GUCEF_CORE_CMACHINESTATE_H
#endif /* GUCEF_CORE_CMACHINESTATE_H ? */

#ifndef GUCEF_CORE_CSTATEHANDLER_H
#include "CStateHandler.h"
#define GUCEF_CORE_CSTATEHANDLER_H
#endif /* GUCEF_CORE_CSTATEHANDLER_H ? */

#include "CStateMachine.h"

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

CStateMachine::CStateMachine( void )
        : _initialstate( 0 ) ,
          _curstate( 0 )     ,
          _updatestate( true )
{
}

/*-------------------------------------------------------------------------*/

CStateMachine::CStateMachine( const CStateMachine& src )
        : _initialstate( src._initialstate ) 
{
}

/*-------------------------------------------------------------------------*/

CStateMachine::~CStateMachine()
{
}

/*-------------------------------------------------------------------------*/

CStateMachine& 
CStateMachine::operator=( const CStateMachine& src )
{
        return *this;
}

/*-------------------------------------------------------------------------*/

bool 
CStateMachine::Start( const UInt32 initialstate )
{
        if ( !_curstate )
        {
                _initialstate = initialstate;
                
                /*
                 *      Notify each handler of the machine start event and make sure
                 *      that each is notified only once.
                 */                
                CStateHandler* handler;
                for ( UInt32 i=0; i<_states.GetCount(); ++i )
                {
                        handler = static_cast<CMachineState*>(_states[ i ])->GetHandler();
                        if ( !handler->_mstarted )
                        {
                                handler->_mstarted = true;
                                handler->OnMachineStart();
                        }
                }                
                
                /*
                 *      Now we get busy,.. enter the initial state.
                 *      The handlers will do the rest.
                 */
                _curstate = LookupStateObj( initialstate );
                if ( _curstate )
                {
                        if ( _curstate->GetHandler()->OnEnterState() )
                        {                        
                                return true;
                        }
                        Stop();
                        return false;
                }
                return false;                                         
        }
        return false;                                
}

/*-------------------------------------------------------------------------*/
        
void 
CStateMachine::Stop( void )
{
        if ( _curstate )
        {      
                CStateHandler* handler;
                for ( UInt32 i=0; i<_states.GetCount(); ++i )
                {
                        handler = static_cast<CMachineState*>(_states[ i ])->GetHandler();
                        if ( handler->_mstarted )
                        {
                                handler->_mstarted = false;
                                handler->OnMachineStop();
                        }
                }
                
                _curstate = NULL;
        }                
}

/*-------------------------------------------------------------------------*/

bool
CStateMachine::Reset( void )
{
        Stop();
        return Start( _initialstate );        
}

/*-------------------------------------------------------------------------*/                              
                            
UInt32 
CStateMachine::GetCurrentState( void ) const
{
        if ( _curstate )
        {
                return _curstate->GetState();
        }
        return 0;                
}

/*-------------------------------------------------------------------------*/
                            
void 
CStateMachine::SetErrorHandler( CErrorStateHandler* errorhandler )
{
}

/*-------------------------------------------------------------------------*/
                            
void 
CStateMachine::Clear( void )
{
        for ( UInt32 i=0; i<_states.GetCount(); ++i )
        {
                delete static_cast<CMachineState*>(_states[ i ]);
        }
        _states.Clear();
}

/*-------------------------------------------------------------------------*/
                              
bool 
CStateMachine::Update( UInt32 tickcount  ,
                       UInt32 deltaticks )
{
        if ( _curstate )
        {
        }
        return false;
}                       

/*-------------------------------------------------------------------------*/
                                    
void 
CStateMachine::OnStateHandled( CStateHandler* handler ,
                               UInt32 conditionvalue  )
{
        if ( handler == _curstate->GetHandler() )
        {
                handler->OnLeaveState();

                UInt32 newstate = _curstate->GetTransition( conditionvalue );
                if ( newstate )
                {
                        _curstate = LookupStateObj( newstate );
                        _curstate->GetHandler()->OnEnterState();
                        return;                
                }
                Stop();
                return;
        }                        
}

/*-------------------------------------------------------------------------*/                               
                             
void 
CStateMachine::OnStateFailure( UInt32 errorvalue          ,
                               const CString& description )
{
}

/*-------------------------------------------------------------------------*/

CMachineState* 
CStateMachine::LookupStateObj( UInt32 state ) const
{
        CMachineState* mstate;
        for ( UInt32 i=0; i<_states.GetCount(); ++i )
        {       
                mstate = static_cast<CMachineState*>( _states[ i ] );
                if ( mstate->GetState() == state )
                {
                        return mstate;
                }                
        }
        return NULL;
}

/*-------------------------------------------------------------------------*/

bool 
CStateMachine::AddState( UInt32 state           ,
                         CStateHandler* handler )
{
        if ( !_curstate && handler )
        {
                if ( !LookupStateObj( state ) )
                {
                        handler->_machine = this;
                        _states.AppendEntry( new CMachineState( state, handler ) );
                        return true;
                }
        }                
        return false;                
}                         

/*-------------------------------------------------------------------------*/
        
bool 
CStateMachine::DelState( UInt32 state )
{
        CMachineState* mstate;
        for ( UInt32 i=0; i<_states.GetCount(); ++i )
        {       
                mstate = static_cast<CMachineState*>( _states[ i ] );
                if ( mstate->GetState() == state )
                {
                        _states.SetEntry( i, NULL );
                        delete mstate;
                        _states.FillNULLGaps();
                        return true;
                }                
        }
        return false;
}

/*-------------------------------------------------------------------------*/

bool 
CStateMachine::SetTransition( const UInt32 oldstate       ,
                              const UInt32 newstate       ,
                              const UInt32 conditionvalue )
{
        CMachineState* state( LookupStateObj( oldstate ) );
        if ( state )
        { 
                return state->AddTransition( newstate, conditionvalue );                
        }                
        return false;
}

/*-------------------------------------------------------------------------*/

bool 
CStateMachine::GetTransition( const UInt32 oldstate       ,
                              const UInt32 conditionvalue ,
                              UInt32& newstate            )
{
        CMachineState* state( LookupStateObj( oldstate ) );
        if ( state )
        { 
                newstate = state->GetTransition( conditionvalue );
                return true;
        }
        return false;         
}                           

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
                               