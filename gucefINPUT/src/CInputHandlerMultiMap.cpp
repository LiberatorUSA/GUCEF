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

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#include "CInputHandlerMultiMap.h"

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

CInputHandlerMultiMap::CInputHandlerMultiMap( void )
{GUCEF_TRACE;
}

/*-------------------------------------------------------------------------*/
        
CInputHandlerMultiMap::~CInputHandlerMultiMap()
{GUCEF_TRACE;
}

/*-------------------------------------------------------------------------*/
        
void
CInputHandlerMultiMap::RegisterHandler( CIInputHandler* handler )
{GUCEF_TRACE;
        if ( m_list.Find( handler ) < 0 )
        {
                m_list.AddEntry( handler );        
        }
}

/*-------------------------------------------------------------------------*/
        
void 
CInputHandlerMultiMap::UnregisterHandler( CIInputHandler* handler )
{GUCEF_TRACE;
        m_list.RemoveEntry( handler );
}

/*-------------------------------------------------------------------------*/

void 
CInputHandlerMultiMap::Clear( void )
{
        m_list.Clear();        
}

/*-------------------------------------------------------------------------*/
        
void 
CInputHandlerMultiMap::OnMouseButtonDown( CInputContext& context   ,
                                          const UInt32 buttonindex )
{GUCEF_TRACE;
        CIInputHandler* handler;
        for ( UInt32 i=0; i<m_list.GetCount(); ++i )
        {
                handler = static_cast< CIInputHandler* >( m_list[ i ] );
                if ( handler )
                {
                        handler->OnMouseButtonDown( context     , 
                                                    buttonindex );
                }
        }
}                                          
        
/*-------------------------------------------------------------------------*/

void 
CInputHandlerMultiMap::OnMouseButtonUp( CInputContext& context   ,
                                        const UInt32 buttonindex )
{GUCEF_TRACE;
        CIInputHandler* handler;
        for ( UInt32 i=0; i<m_list.GetCount(); ++i )
        {
                handler = static_cast< CIInputHandler* >( m_list[ i ] );
                if ( handler )
                {
                        handler->OnMouseButtonUp( context     , 
                                                  buttonindex );
                }
        }
}

/*-------------------------------------------------------------------------*/

void 
CInputHandlerMultiMap::OnMouseMove( CInputContext& context   ,
                                    const Int32 xPos         ,
                                    const Int32 yPos         ,
                                    const Int32 xDelta       ,
                                    const Int32 yDelta       )
{GUCEF_TRACE;
        CIInputHandler* handler;
        for ( UInt32 i=0; i<m_list.GetCount(); ++i )
        {
                handler = static_cast< CIInputHandler* >( m_list[ i ] );
                if ( handler )
                {
                        handler->OnMouseMove( context , 
                                              xPos    ,
                                              yPos    ,
                                              xDelta  ,
                                              yDelta  );
                }
        }        
}

/*-------------------------------------------------------------------------*/                                        

void 
CInputHandlerMultiMap::OnMouseVariableChanged( CInputContext& context ,
                                               const UInt32 varindex  ,
                                               const Int32 value      ,
                                               const Int32 valueDelta )
{GUCEF_TRACE;
        CIInputHandler* handler;
        for ( UInt32 i=0; i<m_list.GetCount(); ++i )
        {
                handler = static_cast< CIInputHandler* >( m_list[ i ] );
                if ( handler )
                {
                        handler->OnMouseVariableChanged( context    , 
                                                         varindex   ,
                                                         value      ,
                                                         valueDelta );
                }
        }        
}

/*-------------------------------------------------------------------------*/
        
void 
CInputHandlerMultiMap::OnKeyboardKeyDown( CInputContext& context   ,
                                          const UInt32 keyindex    ,
                                          const UInt32 keyModState )
{GUCEF_TRACE;
        CIInputHandler* handler;
        for ( UInt32 i=0; i<m_list.GetCount(); ++i )
        {
                handler = static_cast< CIInputHandler* >( m_list[ i ] );
                if ( handler )
                {
                        handler->OnKeyboardKeyDown( context     , 
                                                    keyindex    ,
                                                    keyModState );
                }
        }
}                                          

/*-------------------------------------------------------------------------*/

void 
CInputHandlerMultiMap::OnKeyboardKeyUp( CInputContext& context   ,
                                        const UInt32 keyindex    ,
                                        const UInt32 keyModState )
{GUCEF_TRACE;
        CIInputHandler* handler;
        for ( UInt32 i=0; i<m_list.GetCount(); ++i )
        {
                handler = static_cast< CIInputHandler* >( m_list[ i ] );
                if ( handler )
                {
                        handler->OnKeyboardKeyUp( context     , 
                                                  keyindex    ,
                                                  keyModState );
                }
        }
}                                        

/*-------------------------------------------------------------------------*/

void 
CInputHandlerMultiMap::OnDeviceBooleanOn( CInputContext& context  ,
                                          const UInt32 deviceid   ,
                                          const UInt32 stateindex )
{GUCEF_TRACE;
        CIInputHandler* handler;
        for ( UInt32 i=0; i<m_list.GetCount(); ++i )
        {
                handler = static_cast< CIInputHandler* >( m_list[ i ] );
                if ( handler )
                {
                        handler->OnDeviceBooleanOn( context    , 
                                                    deviceid   ,
                                                    stateindex );
                }
        }
}                                          
                                        
/*-------------------------------------------------------------------------*/

void 
CInputHandlerMultiMap::OnDeviceBooleanOff( CInputContext& context  ,
                                           const UInt32 deviceid   ,
                                           const UInt32 stateindex )
{GUCEF_TRACE;
        CIInputHandler* handler;
        for ( UInt32 i=0; i<m_list.GetCount(); ++i )
        {
                handler = static_cast< CIInputHandler* >( m_list[ i ] );
                if ( handler )
                {
                        handler->OnDeviceBooleanOff( context    , 
                                                     deviceid   ,
                                                     stateindex );
                }
        }
}                                           

/*-------------------------------------------------------------------------*/
                                         
void 
CInputHandlerMultiMap::OnDeviceVariableChanged( CInputContext& context  ,
                                                const UInt32 deviceid   ,
                                                const UInt32 stateindex ,
                                                const Float32 value     )
{GUCEF_TRACE;
        CIInputHandler* handler;
        for ( UInt32 i=0; i<m_list.GetCount(); ++i )
        {
                handler = static_cast< CIInputHandler* >( m_list[ i ] );
                if ( handler )
                {
                        handler->OnDeviceVariableChanged( context    , 
                                                          deviceid   ,
                                                          stateindex ,
                                                          value      );
                }
        }
}

/*-------------------------------------------------------------------------*/

UInt32 
CInputHandlerMultiMap::GetCount( void ) const
{
        return m_list.GetCount();
}

/*-------------------------------------------------------------------------*/
        
CIInputHandler* 
CInputHandlerMultiMap::GetHandler( const UInt32 index ) const
{
        return static_cast< CIInputHandler* >( m_list[ index ] );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
