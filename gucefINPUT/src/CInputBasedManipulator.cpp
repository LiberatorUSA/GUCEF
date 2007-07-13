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
