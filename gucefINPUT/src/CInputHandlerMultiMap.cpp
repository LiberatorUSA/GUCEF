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
{TRACE;
}

/*-------------------------------------------------------------------------*/
        
CInputHandlerMultiMap::~CInputHandlerMultiMap()
{TRACE;
}

/*-------------------------------------------------------------------------*/
        
void
CInputHandlerMultiMap::RegisterHandler( CIInputHandler* handler )
{TRACE;
        if ( m_list.Find( handler ) < 0 )
        {
                m_list.AddEntry( handler );        
        }
}

/*-------------------------------------------------------------------------*/
        
void 
CInputHandlerMultiMap::UnregisterHandler( CIInputHandler* handler )
{TRACE;
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
{TRACE;
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
{TRACE;
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
{TRACE;
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
{TRACE;
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
{TRACE;
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
{TRACE;
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
{TRACE;
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
{TRACE;
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
{TRACE;
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
