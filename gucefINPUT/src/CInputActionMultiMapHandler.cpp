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
{TRACE;

}

/*-------------------------------------------------------------------------*/
        
CInputActionMultiMapHandler::~CInputActionMultiMapHandler()
{TRACE;

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
{TRACE;
        if ( m_list.Find( handler ) < 0 )
        {
                m_list.AddEntry( handler );        
        }
}

/*-------------------------------------------------------------------------*/
        
bool 
CInputActionMultiMapHandler::UnregisterActionHandler( CIInputActionHandler* handler )
{TRACE;
        return m_list.RemoveEntry( handler );
}

/*-------------------------------------------------------------------------*/

void 
CInputActionMultiMapHandler::Clear( void )
{TRACE;
        m_list.Clear();        
}

/*-------------------------------------------------------------------------*/

bool 
CInputActionMultiMapHandler::IsInputEnabled( void ) const
{TRACE;
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
{TRACE;
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
{TRACE;
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
{TRACE;
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
{TRACE;
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
{TRACE;
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
{TRACE;
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
{TRACE;
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
{TRACE;
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
{TRACE;
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
                                                            