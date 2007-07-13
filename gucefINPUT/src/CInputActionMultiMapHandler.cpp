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

#include "CInputActionMultiMapHandler.h"

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

CInputActionMultiMapHandler::CInputActionMultiMapHandler( void )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
        
CInputActionMultiMapHandler::~CInputActionMultiMapHandler()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CORE::CString
CInputActionMultiMapHandler::GetTypeName( void ) const
{
        return "GUCEF::INPUT::CInputActionMultiMapHandler";
}

/*-------------------------------------------------------------------------*/
        
void 
CInputActionMultiMapHandler::RegisterActionHandler( CIInputActionHandler* handler )
{GUCEF_TRACE;
        if ( m_list.Find( handler ) < 0 )
        {
                m_list.AddEntry( handler );        
        }
}

/*-------------------------------------------------------------------------*/
        
bool 
CInputActionMultiMapHandler::UnregisterActionHandler( CIInputActionHandler* handler )
{GUCEF_TRACE;
        return m_list.RemoveEntry( handler );
}

/*-------------------------------------------------------------------------*/

void 
CInputActionMultiMapHandler::Clear( void )
{GUCEF_TRACE;
        m_list.Clear();        
}

/*-------------------------------------------------------------------------*/

bool 
CInputActionMultiMapHandler::IsInputEnabled( void ) const
{GUCEF_TRACE;
        CIInputActionHandler* handler;
        for ( UInt32 i=0; i<m_list.GetCount(); ++i )
        {
                handler = static_cast< CIInputActionHandler* >( m_list[ i ] );
                if ( handler )
                {
                        if ( handler->IsInputEnabled() )
                        { 
                                return true;
                        }
                }
        }
        return false;
}

/*-------------------------------------------------------------------------*/
        
void 
CInputActionMultiMapHandler::OnMappedMouseButtonDown( CInputContext& context   ,
                                                      const UInt32 actionindex ,
                                                      const UInt32 buttonindex )
{GUCEF_TRACE;
        CIInputActionHandler* handler;
        for ( UInt32 i=0; i<m_list.GetCount(); ++i )
        {
                handler = static_cast< CIInputActionHandler* >( m_list[ i ] );
                if ( handler )
                {
                        if ( handler->IsInputEnabled() )
                        { 
                                handler->OnMappedMouseButtonDown( context     , 
                                                                  actionindex ,
                                                                  buttonindex );
                        }
                }
        }
}

/*-------------------------------------------------------------------------*/                                                      
        
void 
CInputActionMultiMapHandler::OnMappedMouseButtonUp( CInputContext& context   ,
                                                    const UInt32 actionindex ,
                                                    const UInt32 buttonindex )
{GUCEF_TRACE;
        CIInputActionHandler* handler( NULL );
        for ( UInt32 i=0; i<m_list.GetCount(); ++i )
        {
                handler = static_cast< CIInputActionHandler* >( m_list[ i ] );
                if ( handler )
                {
                        if ( handler->IsInputEnabled() )
                        {
                                handler->OnMappedMouseButtonUp( context     , 
                                                                actionindex ,
                                                                buttonindex );
                        }
                }
        }
}

/*-------------------------------------------------------------------------*/

void 
CInputActionMultiMapHandler::OnMappedMouseMove( CInputContext& context   ,
                                                const UInt32 actionindex ,
                                                const Int32 xPos         ,
                                                const Int32 yPos         ,
                                                const Int32 xDelta       ,
                                                const Int32 yDelta       )
{GUCEF_TRACE;
        CIInputActionHandler* handler( NULL );
        for ( UInt32 i=0; i<m_list.GetCount(); ++i )
        {
                handler = static_cast< CIInputActionHandler* >( m_list[ i ] );
                if ( handler )
                {
                        if ( handler->IsInputEnabled() )
                        {
                                handler->OnMappedMouseMove( context     , 
                                                            actionindex ,
                                                            xPos        ,
                                                            yPos        ,
                                                            xDelta      ,
                                                            yDelta      );
                        }
                }
        }        
}

/*-------------------------------------------------------------------------*/

void 
CInputActionMultiMapHandler::OnMappedMouseVariableChanged( CInputContext& context   ,
                                                           const UInt32 actionindex ,
                                                           const UInt32 varindex    ,
                                                           const Int32 value        ,
                                                           const Int32 valueDelta   )
{GUCEF_TRACE;
        CIInputActionHandler* handler;
        for ( UInt32 i=0; i<m_list.GetCount(); ++i )
        {
                handler = static_cast< CIInputActionHandler* >( m_list[ i ] );
                if ( handler )
                {
                        if ( handler->IsInputEnabled() )
                        {
                                handler->OnMappedMouseVariableChanged( context     , 
                                                                       actionindex ,
                                                                       varindex    ,
                                                                       value       ,
                                                                       valueDelta  );
                        }
                }
        }        
}

/*-------------------------------------------------------------------------*/                                                    
        
void 
CInputActionMultiMapHandler::OnMappedKeyboardKeyDown( CInputContext& context   ,
                                                      const UInt32 actionindex ,
                                                      const UInt32 keyindex    ,
                                                      const UInt32 keyModState )
{GUCEF_TRACE;
        CIInputActionHandler* handler;
        for ( UInt32 i=0; i<m_list.GetCount(); ++i )
        {
                handler = static_cast< CIInputActionHandler* >( m_list[ i ] );
                if ( handler )
                {
                        if ( handler->IsInputEnabled() )
                        {
                                handler->OnMappedKeyboardKeyDown( context     , 
                                                                  actionindex ,
                                                                  keyindex    ,
                                                                  keyModState );
                        }
                }
        }
}

/*-------------------------------------------------------------------------*/                                                      
        
void 
CInputActionMultiMapHandler::OnMappedKeyboardKeyUp( CInputContext& context   ,
                                                    const UInt32 actionindex ,
                                                    const UInt32 keyindex    ,
                                                    const UInt32 keyModState )
{GUCEF_TRACE;
        CIInputActionHandler* handler;
        for ( UInt32 i=0; i<m_list.GetCount(); ++i )
        {
                handler = static_cast< CIInputActionHandler* >( m_list[ i ] );
                if ( handler )
                {
                        if ( handler->IsInputEnabled() )
                        {
                                handler->OnMappedKeyboardKeyUp( context     , 
                                                                actionindex ,
                                                                keyindex    ,
                                                                keyModState );
                        }
                }
        }
}

/*-------------------------------------------------------------------------*/                                                    
        
void 
CInputActionMultiMapHandler::OnMappedDeviceBooleanOn( CInputContext& context   ,
                                                      const UInt32 actionindex ,
                                                      const UInt32 deviceid    ,
                                                      const UInt32 stateindex  )
{GUCEF_TRACE;
        CIInputActionHandler* handler;
        for ( UInt32 i=0; i<m_list.GetCount(); ++i )
        {
                handler = static_cast< CIInputActionHandler* >( m_list[ i ] );
                if ( handler )
                {
                        if ( handler->IsInputEnabled() )
                        {
                                handler->OnMappedDeviceBooleanOn( context     , 
                                                                  actionindex ,
                                                                  deviceid    ,
                                                                  stateindex  );
                        }
                }
        }
}

/*-------------------------------------------------------------------------*/                                                      
                                        
void 
CInputActionMultiMapHandler::OnMappedDeviceBooleanOff( CInputContext& context   ,
                                                       const UInt32 actionindex ,
                                                       const UInt32 deviceid    ,
                                                       const UInt32 stateindex  )
{GUCEF_TRACE;
        CIInputActionHandler* handler;
        for ( UInt32 i=0; i<m_list.GetCount(); ++i )
        {
                handler = static_cast< CIInputActionHandler* >( m_list[ i ] );
                if ( handler )
                {
                        if ( handler->IsInputEnabled() )
                        {
                                handler->OnMappedDeviceBooleanOff( context     , 
                                                                   actionindex ,
                                                                   deviceid    ,
                                                                   stateindex  );
                        }
                }
        }
}                                                       

/*-------------------------------------------------------------------------*/
                                         
void 
CInputActionMultiMapHandler::OnMappedDeviceVariableChanged( CInputContext& context   ,
                                                            const UInt32 actionindex ,
                                                            const UInt32 deviceid    ,
                                                            const UInt32 stateindex  ,
                                                            const Float32 value      )
{GUCEF_TRACE;
        CIInputActionHandler* handler;
        for ( UInt32 i=0; i<m_list.GetCount(); ++i )
        {
                handler = static_cast< CIInputActionHandler* >( m_list[ i ] );
                if ( handler )
                {
                        if ( handler->IsInputEnabled() )
                        {
                                handler->OnMappedDeviceVariableChanged( context     , 
                                                                        actionindex ,
                                                                        deviceid    ,
                                                                        stateindex  ,
                                                                        value       );
                        }
                }
        }
}

/*-------------------------------------------------------------------------*/

UInt32 
CInputActionMultiMapHandler::GetCount( void ) const
{
        return m_list.GetCount();
}

/*-------------------------------------------------------------------------*/
        
CIInputActionHandler* 
CInputActionMultiMapHandler::GetHandler( const UInt32 index ) const
{
        return static_cast< CIInputActionHandler* >( m_list[ index ] );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
                                                            