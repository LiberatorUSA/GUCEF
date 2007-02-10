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

#include "CTONRegistry.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CEvent CTONRegistryEvents::ItemRegisteredEvent = "GUCEF::CORE::CTONRegistryEvents::ItemRegisteredEvent";
const CEvent CTONRegistryEvents::ItemUnregisteredEvent = "GUCEF::CORE::CTONRegistryEvents::ItemUnregisteredEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

void
CTONRegistryEvents::RegisterEvents( void )
{TRACE;

    ItemRegisteredEvent.Initialize();
    ItemUnregisteredEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

CTONRegistryEvents::CTONRegistryEvents( void )
{TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CTONRegistryEvents::CTONRegistryEvents( const CTONRegistryEvents& src )
{TRACE;

    RegisterEvents();
}

/*-------------------------------------------------------------------------*/

CTONRegistryEvents::~CTONRegistryEvents()
{TRACE;

}

/*-------------------------------------------------------------------------*/

CTONRegistryEvents&
CTONRegistryEvents::operator=( const CTONRegistryEvents& src )
{TRACE;

    return *this;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/