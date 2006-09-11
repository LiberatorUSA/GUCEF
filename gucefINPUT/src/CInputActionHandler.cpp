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

#include "CInputActionHandler.h"

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

CInputActionHandler::CInputActionHandler( void )
{
}

/*-------------------------------------------------------------------------*/
        
CInputActionHandler::CInputActionHandler( const CInputActionHandler& src )
        : m_actionmap( src.m_actionmap )
{
}

/*-------------------------------------------------------------------------*/
        
CInputActionHandler::~CInputActionHandler()
{
}

/*-------------------------------------------------------------------------*/
        
CInputActionHandler& 
CInputActionHandler::operator=( const CInputActionHandler& src )
{
        if ( this != &src )
        {
                m_actionmap = src.m_actionmap;
        }
        return *this;
}

/*-------------------------------------------------------------------------*/

void 
CInputActionHandler::SetActionMap( const CInputActionMap& actionmap )
{
        m_actionmap = actionmap;
}

/*-------------------------------------------------------------------------*/
        
const CInputActionMap& 
CInputActionHandler::GetActionMap( void ) const
{
        return m_actionmap;
}

/*-------------------------------------------------------------------------*/
        
bool 
CInputActionHandler::LoadActionMapConfig( const CORE::CDataNode& treenode )
{
        return m_actionmap.LoadConfig( treenode );
}

/*-------------------------------------------------------------------------*/
        
bool 
CInputActionHandler::SaveActionMapConfig( CORE::CDataNode& treenode )
{
        return m_actionmap.SaveConfig( treenode );
}

/*-------------------------------------------------------------------------*/
        
CInputActionMap& 
CInputActionHandler::GetMutableActionMap( void )
{
        return m_actionmap;
}

/*-------------------------------------------------------------------------*/

void 
CInputActionHandler::OnMouseButtonDown( const UInt32 buttonindex )
{
        OnMappedMouseButtonDown( m_actionmap.GetMouseButtonDownMap( buttonindex ) ,
                                 buttonindex                                      );
}

/*-------------------------------------------------------------------------*/
        
void 
CInputActionHandler::OnMouseButtonUp( const UInt32 buttonindex )
{
        OnMappedMouseButtonUp( m_actionmap.GetMouseButtonUpMap( buttonindex ) ,
                               buttonindex                                    );
}

/*-------------------------------------------------------------------------*/

void 
CInputActionHandler::OnKeyboardKeyDown( const UInt32 keyindex )
{
        OnMappedKeyboardKeyDown( m_actionmap.GetKeyboardKeyDownMap( keyindex ) ,
                                 keyindex                                      );
}

/*-------------------------------------------------------------------------*/
        
void 
CInputActionHandler::OnKeyboardKeyUp( const UInt32 keyindex )
{
        OnMappedKeyboardKeyUp( m_actionmap.GetKeyboardKeyUpMap( keyindex ) ,
                               keyindex                                    );
}
 
/*-------------------------------------------------------------------------*/ 
        
void 
CInputActionHandler::OnDeviceBooleanOn( const UInt32 deviceid   ,
                                        const UInt32 stateindex )
{
        OnMappedDeviceBooleanOn( m_actionmap.GetDeviceBooleanOnMap( deviceid   ,
                                                                    stateindex ) ,
                                 deviceid                                        ,
                                 stateindex                                      );
}

/*-------------------------------------------------------------------------*/

void 
CInputActionHandler::OnDeviceBooleanOff( const UInt32 deviceid   ,
                                         const UInt32 stateindex )
{
        OnMappedDeviceBooleanOff( m_actionmap.GetDeviceBooleanOffMap( deviceid   ,
                                                                      stateindex ) ,
                                  deviceid                                         ,
                                  stateindex                                       );
}                                         

/*-------------------------------------------------------------------------*/
                                         
void 
CInputActionHandler::OnDeviceVariableChanged( const UInt32 deviceid   ,
                                              const UInt32 stateindex ,
                                              const Float32 value     )
{
        OnMappedDeviceVariableChanged( m_actionmap.GetDeviceVariableChangedMap( deviceid   ,
                                                                                stateindex ) ,
                                       deviceid                                              ,
                                       stateindex                                            ,
                                       value                                                 );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
