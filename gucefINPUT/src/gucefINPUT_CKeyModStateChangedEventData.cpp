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

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#include "gucefINPUT_CKeyModStateChangedEventData.h"

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

CKeyModStateChangedEventData::CKeyModStateChangedEventData( const KeyModifier keyMod ,
                                                            const bool pressedState  )
    : CICloneable()                  ,
      m_keyMod( keyMod )             ,
      m_pressedState( pressedState ) 
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CKeyModStateChangedEventData::CKeyModStateChangedEventData( const CKeyModStateChangedEventData& src )
    : CICloneable( src )                 ,
      m_keyMod( src.m_keyMod )           ,
      m_pressedState( src.m_pressedState )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
                        
CKeyModStateChangedEventData::~CKeyModStateChangedEventData()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

KeyModifier
CKeyModStateChangedEventData::GetModifier( void ) const
{GUCEF_TRACE;

    return m_keyMod;
}

/*-------------------------------------------------------------------------*/
    
bool
CKeyModStateChangedEventData::GetPressedState( void ) const
{GUCEF_TRACE;

    return m_pressedState;
}

/*-------------------------------------------------------------------------*/

CORE::CICloneable*
CKeyModStateChangedEventData::Clone( void ) const
{GUCEF_TRACE;

    return new CKeyModStateChangedEventData( *this );
}
    
/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/