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

#include "gucefINPUT_CMouseButtonEventData.h"

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

CMouseButtonEventData::CMouseButtonEventData( const UInt32 deviceID    ,
                                              const UInt32 buttonIndex ,
                                              const bool pressedState  ,
                                              const Int32 xPos         ,
                                              const Int32 yPos         )
    : CICloneable()                  ,
      m_deviceID( deviceID )         ,
      m_buttonIndex( buttonIndex )   ,
      m_pressedState( pressedState ) ,
      m_xPos( xPos )                 ,
      m_yPos( yPos )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CMouseButtonEventData::CMouseButtonEventData( const CMouseButtonEventData& src )
    : CICloneable( src )                   ,
      m_deviceID( src.m_deviceID )         ,
      m_buttonIndex( src.m_buttonIndex )   ,
      m_pressedState( src.m_pressedState ) ,
      m_xPos( src.m_xPos )                 ,
      m_yPos( src.m_yPos )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
                        
CMouseButtonEventData::~CMouseButtonEventData()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

UInt32
CMouseButtonEventData::GetDeviceID( void ) const
{GUCEF_TRACE;

    return m_deviceID;
}

/*-------------------------------------------------------------------------*/

UInt32
CMouseButtonEventData::GetButtonIndex( void ) const
{GUCEF_TRACE;

    return m_buttonIndex;
}

/*-------------------------------------------------------------------------*/
    
bool
CMouseButtonEventData::GetPressedState( void ) const
{GUCEF_TRACE;

    return m_pressedState;
}

/*-------------------------------------------------------------------------*/

CORE::CICloneable*
CMouseButtonEventData::Clone( void ) const
{GUCEF_TRACE;

    return new CMouseButtonEventData( *this );
}

/*-------------------------------------------------------------------------*/

Int32
CMouseButtonEventData::GetXPos( void ) const
{GUCEF_TRACE;

    return m_xPos;
}

/*-------------------------------------------------------------------------*/
    
Int32
CMouseButtonEventData::GetYPos( void ) const
{GUCEF_TRACE;

    return m_yPos;
}
    
/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/