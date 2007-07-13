/*
 *  gucefINPUT: GUCEF module providing input device interaction
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

#include <assert.h>

#ifndef CINPUTCONTROLLER_H
#include "CInputController.h"
#define CINPUTCONTROLLER_H
#endif /* CINPUTCONTROLLER_H ? */

#include "CGUCEFINPUTModule.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace INPUT {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

bool 
CGUCEFINPUTModule::Load( void )
{TRACE;
        CInputController::RegisterEvents();
        CInputController::Instance();
        return true;
}

/*-------------------------------------------------------------------------*/
        
bool 
CGUCEFINPUTModule::Unload( void )
{TRACE;
        CInputController::Deinstance();        
        return true;
}

/*-------------------------------------------------------------------------*/
        
CGUCEFINPUTModule::CGUCEFINPUTModule( void )
{TRACE;
        /* dummy, do not use */
        assert( 0 );
}

/*-------------------------------------------------------------------------*/

CGUCEFINPUTModule::CGUCEFINPUTModule( const CGUCEFINPUTModule& src )
{TRACE;
        /* dummy, do not use */
        assert( 0 );
}

/*-------------------------------------------------------------------------*/

CGUCEFINPUTModule::~CGUCEFINPUTModule()
{TRACE;
        /* dummy, do not use */
        assert( 0 );
}

/*-------------------------------------------------------------------------*/

CGUCEFINPUTModule& 
CGUCEFINPUTModule::operator=( const CGUCEFINPUTModule& src )
{TRACE;
        /* dummy, do not use */
        assert( 0 );
        
        return *this;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
