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

#ifndef CINPUTACTIONMAP_H
#include "CInputActionMap.h"
#define CINPUTACTIONMAP_H
#endif /* CINPUTACTIONMAP_H ? */

#ifndef CIINPUTACTIONHANDLER_H
#include "CIInputActionHandler.h"
#define CIINPUTACTIONHANDLER_H
#endif /* CIINPUTACTIONHANDLER_H ? */

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

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
{GUCEF_TRACE;
}

/*-------------------------------------------------------------------------*/
        
CInputToActionHandler::~CInputToActionHandler()
{GUCEF_TRACE;
}

/*-------------------------------------------------------------------------*/
        
void 
CInputToActionHandler::SetActionHandler( CIInputActionHandler* handler )
{GUCEF_TRACE;
        m_handler = handler;
}

/*-------------------------------------------------------------------------*/

CIInputActionHandler* 
CInputToActionHandler::GetActionHandler( void ) const
{GUCEF_TRACE;
        return m_handler;
}

/*-------------------------------------------------------------------------*/
        
void 
CInputToActionHandler::OnMouseButtonDown( CInputContext& context   ,
                                          const UInt32 buttonindex )
{GUCEF_TRACE;
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
{GUCEF_TRACE;
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
{GUCEF_TRACE;
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
{GUCEF_TRACE;
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
{GUCEF_TRACE;
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
{GUCEF_TRACE;
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
{GUCEF_TRACE;
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
{GUCEF_TRACE;
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
{GUCEF_TRACE;
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
