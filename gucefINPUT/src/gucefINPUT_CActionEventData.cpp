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

#include "gucefINPUT_CActionEventData.h"

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

CActionEventData::CActionEventData( const UInt32 actionID        ,
                                    const CORE::CEvent& orgEvent )
    : CICloneable()          ,
      m_actionID( actionID ) ,
      m_orgEvent( orgEvent )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CActionEventData::CActionEventData( const CActionEventData& src )
    : CICloneable( src )           ,
      m_actionID( src.m_actionID ) ,
      m_orgEvent( src.m_orgEvent )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
                        
CActionEventData::~CActionEventData()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

UInt32
CActionEventData::GetActionID( void ) const
{GUCEF_TRACE;

    return m_actionID;
}

/*-------------------------------------------------------------------------*/

const CORE::CEvent&
CActionEventData::GetEvent( void ) const
{GUCEF_TRACE;

    return m_orgEvent;
}
    
/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/