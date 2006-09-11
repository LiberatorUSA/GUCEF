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

#ifndef CINPUTPROFILEMANAGER_H
#include "CInputProfileManager.h"
#define CINPUTPROFILEMANAGER_H
#endif /* CINPUTPROFILEMANAGER_H ? */

#include "CInputBasedManipulator.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace INPUT {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

CInputBasedManipulator::CInputBasedManipulator( const CORE::CString& name )
        : m_name( name ) ,
          m_enabled( true )
{TRACE;
        CInputProfileManager::Instance()->RegisterManipulator( this );        
}

/*-------------------------------------------------------------------------*/

CInputBasedManipulator::~CInputBasedManipulator()
{TRACE;
        CInputProfileManager::Instance()->UnregisterManipulator( this );
}

/*-------------------------------------------------------------------------*/

const CORE::CString&
CInputBasedManipulator::GetName( void ) const
{TRACE;
        return m_name;
}

/*-------------------------------------------------------------------------*/

void 
CInputBasedManipulator::SetInputEnabled( const bool enabled )
{TRACE;
        m_enabled = enabled;
}

/*-------------------------------------------------------------------------*/
        
bool 
CInputBasedManipulator::IsInputEnabled( void ) const
{TRACE;
        return m_enabled;
}

/*-------------------------------------------------------------------------*/

void 
CInputBasedManipulator::SetHandlingOrder( const UInt32 handlingOrder )
{TRACE;
    m_handlingOrder = handlingOrder;
}

/*-------------------------------------------------------------------------*/

UInt32 
CInputBasedManipulator::GetHandlingOrder( void ) const
{TRACE;
        return m_handlingOrder;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
