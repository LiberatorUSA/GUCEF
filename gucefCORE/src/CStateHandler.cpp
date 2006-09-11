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

#include "CStateHandler.h"

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

CStateHandler::CStateHandler( void )
        : _machine( NULL )
{

}

/*-------------------------------------------------------------------------*/
                          
CStateHandler::CStateHandler( const CStateHandler& src )
        : _machine( src._machine )
{
        
}

/*-------------------------------------------------------------------------*/

CStateHandler::~CStateHandler()
{
}

/*-------------------------------------------------------------------------*/

CStateHandler& 
CStateHandler::operator=( const CStateHandler& src )
{
        if ( this != &src )
        {

        }                
        return *this;
}

/*-------------------------------------------------------------------------*/
        
bool 
CStateHandler::OnMachineStart( void )
{
        return true;
}

/*-------------------------------------------------------------------------*/
        
void 
CStateHandler::OnMachineStop( void )
{

}

/*-------------------------------------------------------------------------*/

void 
CStateHandler::OnLeaveState( void )
{
        
}

/*-------------------------------------------------------------------------*/

bool 
CStateHandler::OnUpdate( UInt32 tickcount  ,
                         UInt32 deltaticks )
{
        SignalStateHandled( 0 );
        return true;
}                         

/*-------------------------------------------------------------------------*/
        
void 
CStateHandler::SignalStateHandled( UInt32 conditionvalue )
{
        _machine->OnStateHandled( this, conditionvalue );
}

/*-------------------------------------------------------------------------*/
        
void 
CStateHandler::SignalStateFailure( UInt32 errorvalue          ,
                                   const CString& description )
{
}

/*-------------------------------------------------------------------------*/

UInt32
CStateHandler::GetState( void ) const
{
        return _machine->GetCurrentState();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
