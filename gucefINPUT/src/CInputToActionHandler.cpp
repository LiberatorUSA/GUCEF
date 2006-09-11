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

#ifndef CINPUTACTIONMAP_H
#include "CInputActionMap.h"
#define CINPUTACTIONMAP_H
#endif /* CINPUTACTIONMAP_H ? */

#ifndef CIINPUTACTIONHANDLER_H
#include "CIInputActionHandler.h"
#define CIINPUTACTIONHANDLER_H
#endif /* CIINPUTACTIONHANDLER_H ? */

#include "CInputToActionHandler.h"

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

CInputToActionHandler::CInputToActionHandler( CInputActionMap& actionmap                 ,
                                              CIInputActionHandler* handler /* = NULL */ )
        : m_actionmap( actionmap ) ,
          m_handler( handler )
{TRACE;
}

/*-------------------------------------------------------------------------*/
        
CInputToActionHandler::~CInputToActionHandler()
{TRACE;
}

/*-------------------------------------------------------------------------*/
        
void 
CInputToActionHandler::SetActionHandler( CIInputActionHandler* handler )
{TRACE;
        m_handler = handler;
}

/*-------------------------------------------------------------------------*/

CIInputActionHandler* 
CInputToActionHandler::GetActionHandler( void ) const
{TRACE;
        return m_handler;
}

/*-------------------------------------------------------------------------*/
        
void 
CInputToActionHandler::OnMouseButtonDown( CInputContext& context   ,
                                          const UInt32 buttonindex )
{TRACE;
        if ( m_handler )
        {
                m_handler->OnMappedMouseButtonDown( context                                          ,
                                                    m_actionmap.GetMouseButtonDownMap( buttonindex ) ,
                                                    buttonindex                                      );
        }                                
}

/*-------------------------------------------------------------------------*/
        
void 
CInputToActionHandler::OnMouseButtonUp( CInputContext& context   ,
                                        const UInt32 buttonindex )
{TRACE;
        if ( m_handler )
        {
                m_handler->OnMappedMouseButtonUp( context                                        ,
                                                  m_actionmap.GetMouseButtonUpMap( buttonindex ) ,
                                                  buttonindex                                    );
        }
}

/*-------------------------------------------------------------------------*/

void 
CInputToActionHandler::OnMouseMove( CInputContext& context   ,
                                    const Int32 xPos         ,
                                    const Int32 yPos         ,
                                    const Int32 xDelta       ,
                                    const Int32 yDelta       )
{TRACE;
        if ( m_handler )
        {
                m_handler->OnMappedMouseMove( context ,
                                              0       ,
                                              xPos    ,
                                              yPos    ,
                                              xDelta  ,
                                              yDelta  );
        }
}

/*-------------------------------------------------------------------------*/

void 
CInputToActionHandler::OnMouseVariableChanged( CInputContext& context ,
                                               const UInt32 varindex  ,
                                               const Int32 value      ,
                                               const Int32 valueDelta )
{TRACE;
        if ( m_handler )
        {
                m_handler->OnMappedMouseVariableChanged( context    ,
                                                         0          ,
                                                         varindex   ,
                                                         value      ,
                                                         valueDelta );
        }
}

/*-------------------------------------------------------------------------*/
        
void 
CInputToActionHandler::OnKeyboardKeyDown( CInputContext& context   ,
                                          const UInt32 keyindex    ,
                                          const UInt32 keyModState )
{TRACE;
        if ( m_handler )
        {
                m_handler->OnMappedKeyboardKeyDown( context                                       ,
                                                    m_actionmap.GetKeyboardKeyDownMap( keyindex ) ,
                                                    keyindex                                      ,
                                                    keyModState                                   );
        }
}
        
/*-------------------------------------------------------------------------*/

void 
CInputToActionHandler::OnKeyboardKeyUp( CInputContext& context   , 
                                        const UInt32 keyindex    ,
                                        const UInt32 keyModState )
{TRACE;
        if ( m_handler )
        {
                m_handler->OnMappedKeyboardKeyUp( context                                     ,
                                                  m_actionmap.GetKeyboardKeyUpMap( keyindex ) ,
                                                  keyindex                                    ,
                                                  keyModState                                 );
        }
}
        
/*-------------------------------------------------------------------------*/

void 
CInputToActionHandler::OnDeviceBooleanOn( CInputContext& context  ,
                                          const UInt32 deviceid   ,
                                          const UInt32 stateindex )
{TRACE;
        if ( m_handler )
        {
                m_handler->OnMappedDeviceBooleanOn( context                                        ,
                                                    m_actionmap.GetDeviceBooleanOnMap( deviceid   ,
                                                                                       stateindex ) ,
                                                    deviceid                                        ,
                                                    stateindex                                      );
        }
}                                          

/*-------------------------------------------------------------------------*/
                                        
void 
CInputToActionHandler::OnDeviceBooleanOff( CInputContext& context  ,
                                           const UInt32 deviceid   ,
                                           const UInt32 stateindex )
{TRACE;
        if ( m_handler )
        {
                m_handler->OnMappedDeviceBooleanOff( context                                          ,
                                                     m_actionmap.GetDeviceBooleanOffMap( deviceid   ,
                                                                                         stateindex ) ,
                                                     deviceid                                         ,
                                                     stateindex                                       );
        }
}                                           

/*-------------------------------------------------------------------------*/
                                         
void 
CInputToActionHandler::OnDeviceVariableChanged( CInputContext& context  ,
                                                const UInt32 deviceid   ,
                                                const UInt32 stateindex ,
                                                const Float32 value     )
{TRACE;
        if ( m_handler )
        {
                m_handler->OnMappedDeviceVariableChanged( context                                               ,
                                                          m_actionmap.GetDeviceVariableChangedMap( deviceid   ,
                                                                                                   stateindex ) ,
                                                          deviceid                                              ,
                                                          stateindex                                            ,
                                                          value                                                 );
        }
}                                                

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
