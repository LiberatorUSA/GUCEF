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

CMachineState::CMachineState( UInt32 state           ,
                              CStateHandler* handler )
        : _state( state )     ,
          _handler( handler ) 
{

}

/*-------------------------------------------------------------------------*/

CMachineState::~CMachineState()
{
        for ( UInt32 i=0; i<_transitions.size(); ++i )
        {
                delete _transitions[ i ];
        }
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
                _transitions.push_back( trans );
                return true;        
        }
        return false;                
}                              

/*-------------------------------------------------------------------------*/
        
UInt32 
CMachineState::GetTransition( UInt32 conditionvalue )
{
        TStateTransition* trans;
        for ( UInt32 i=0; i<_transitions.size(); ++i )
        {
                trans = _transitions[ i ];
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
