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

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#ifndef GUCEF_CORE_CLOGMANAGER_H
#include "CLogManager.h"
#define GUCEF_CORE_CLOGMANAGER_H
#endif /* GUCEF_CORE_CLOGMANAGER_H ? */

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
{GUCEF_TRACE;
        
        GUCEF_SYSTEM_LOG( 0, "gucefINPUT Module loaded" );
        
        CInputController::RegisterEvents();
        CInputController::Instance();
        return true;
}

/*-------------------------------------------------------------------------*/
        
bool 
CGUCEFINPUTModule::Unload( void )
{GUCEF_TRACE;
        
        GUCEF_SYSTEM_LOG( 0, "gucefINPUT Module unloading" );
        
        CInputController::Deinstance();        
        return true;
}

/*-------------------------------------------------------------------------*/
        
CGUCEFINPUTModule::CGUCEFINPUTModule( void )
{GUCEF_TRACE;
        /* dummy, do not use */
        assert( 0 );
}

/*-------------------------------------------------------------------------*/

CGUCEFINPUTModule::CGUCEFINPUTModule( const CGUCEFINPUTModule& src )
{GUCEF_TRACE;
        /* dummy, do not use */
        assert( 0 );
}

/*-------------------------------------------------------------------------*/

CGUCEFINPUTModule::~CGUCEFINPUTModule()
{GUCEF_TRACE;
        /* dummy, do not use */
        assert( 0 );
}

/*-------------------------------------------------------------------------*/

CGUCEFINPUTModule& 
CGUCEFINPUTModule::operator=( const CGUCEFINPUTModule& src )
{GUCEF_TRACE;
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
