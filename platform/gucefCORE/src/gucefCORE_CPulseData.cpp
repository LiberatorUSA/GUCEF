/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
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

#include "gucefCORE_CPulseData.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF { 
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CPulseData::CPulseData( const UInt64 tickCount               ,
                        const UInt64 deltaTicks              ,
                        const Float64 updateDeltaInMilliSecs )
    : CICloneable()                                      ,
      m_tickCount( tickCount )                           ,
      m_tickDelta( deltaTicks )                          ,
      m_updateDeltaInMilliSecs( updateDeltaInMilliSecs )
{GUCEF_TRACE;
                        
}

/*--------------------------------------------------------------------------*/
    
CPulseData::CPulseData( const CPulseData& src )
    : CICloneable( src )                                       ,
      m_tickCount( src.m_tickCount )                           ,
      m_tickDelta( src.m_tickDelta )                           ,
      m_updateDeltaInMilliSecs( src.m_updateDeltaInMilliSecs )
{GUCEF_TRACE;
}

/*--------------------------------------------------------------------------*/
    
CPulseData::~CPulseData()
{GUCEF_TRACE;

}

/*--------------------------------------------------------------------------*/
    
UInt64
CPulseData::GetTickCount( void ) const
{GUCEF_TRACE;
    
    return m_tickCount;
}

/*--------------------------------------------------------------------------*/
    
UInt64
CPulseData::GetTickDelta( void ) const
{GUCEF_TRACE;

    return m_tickDelta;
}

/*--------------------------------------------------------------------------*/

Float64
CPulseData::GetUpdateDeltaInMilliSecs( void ) const
{GUCEF_TRACE;

    return m_updateDeltaInMilliSecs;
}

/*--------------------------------------------------------------------------*/
    
CICloneable*
CPulseData::Clone( void ) const
{GUCEF_TRACE;

    return new CPulseData( *this );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
