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

#include "gucefINPUT_CMouseMovedEventData.h"

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

CMouseMovedEventData::CMouseMovedEventData( const Int32 contextId ,
                                            const UInt32 deviceID ,
                                            const Int32 xPos      ,
                                            const Int32 yPos      ,
                                            const Int32 prevXPos  ,
                                            const Int32 prevYPos  )
    : CICloneable()            ,
      m_contextId( contextId ) ,
      m_deviceID( deviceID )   ,
      m_xPos( xPos )           ,
      m_yPos( yPos )           ,
      m_prevXPos( prevXPos )   ,
      m_prevYPos( prevYPos )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CMouseMovedEventData::CMouseMovedEventData( const CMouseMovedEventData& src )
    : CICloneable( src )             ,
      m_contextId( src.m_contextId ) ,
      m_deviceID( src.m_deviceID )   ,
      m_xPos( m_xPos )               ,
      m_yPos( m_yPos )               ,
      m_prevXPos( m_prevXPos )       ,
      m_prevYPos( m_prevYPos )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
                        
CMouseMovedEventData::~CMouseMovedEventData()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

Int32
CMouseMovedEventData::GetContextId( void ) const
{GUCEF_TRACE;
    
    return m_contextId;
}

/*-------------------------------------------------------------------------*/

UInt32
CMouseMovedEventData::GetDeviceID( void ) const
{GUCEF_TRACE;

    return m_deviceID;
}

/*-------------------------------------------------------------------------*/

void
CMouseMovedEventData::GetPos( Int32& xPos ,
                              Int32& yPos ) const
{GUCEF_TRACE;

    xPos = m_xPos;
    yPos = m_yPos;
}

/*-------------------------------------------------------------------------*/

void
CMouseMovedEventData::GetPrevPos( Int32& xPos ,
                                  Int32& yPos ) const
{GUCEF_TRACE;

    xPos = m_prevXPos;
    yPos = m_prevYPos;
}

/*-------------------------------------------------------------------------*/
    
Int32
CMouseMovedEventData::GetXPos( void ) const
{GUCEF_TRACE;

    return m_xPos;
}

/*-------------------------------------------------------------------------*/
    
Int32
CMouseMovedEventData::GetYPos( void ) const
{GUCEF_TRACE;

    return m_yPos;
}

/*-------------------------------------------------------------------------*/

Int32
CMouseMovedEventData::GetPrevXPos( void ) const
{GUCEF_TRACE;
    
    return m_prevXPos;
}

/*-------------------------------------------------------------------------*/
    
Int32
CMouseMovedEventData::GetPrevYPos( void ) const
{GUCEF_TRACE;

    return m_prevYPos;
}

/*-------------------------------------------------------------------------*/

CORE::CICloneable*
CMouseMovedEventData::Clone( void ) const
{GUCEF_TRACE;

    return new CMouseMovedEventData( *this );
}
    
/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/