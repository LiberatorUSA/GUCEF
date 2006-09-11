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

#ifndef CIINPUTHANDLER_H
#define CIINPUTHANDLER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CINPUTCONTEXT_H
#include "CInputContext.h"
#define CINPUTCONTEXT_H
#endif /* CINPUTCONTEXT_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace INPUT {

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

#define GUCEFINPUT_KEYMOD_LEFT_ALT_STATE( keyModFlags )        GETBITX( keyModFlags, 0 )
#define GUCEFINPUT_KEYMOD_RIGHT_ALT_STATE( keyModFlags )       GETBITX( keyModFlags, 1 )
#define GUCEFINPUT_KEYMOD_ALT_STATE( keyModFlags )             ( (GUCEFINPUT_KEYMOD_LEFT_ALT_STATE( keyModFlags )) || (GUCEFINPUT_KEYMOD_RIGHT_ALT_STATE( keyModFlags )) )

#define GUCEFINPUT_KEYMOD_LEFT_CONTROL_STATE( keyModFlags )    GETBITX( keyModFlags, 2 )
#define GUCEFINPUT_KEYMOD_RIGHT_CONTROL_STATE( keyModFlags )   GETBITX( keyModFlags, 3 )
#define GUCEFINPUT_KEYMOD_CONTROL_STATE( keyModFlags )         ( (GUCEFINPUT_KEYMOD_LEFT_CONTROL_STATE( keyModFlags )) || (GUCEFINPUT_KEYMOD_RIGHT_CONTROL_STATE( keyModFlags )) )

#define GUCEFINPUT_KEYMOD_LEFT_SHIFT_STATE( keyModFlags )      GETBITX( keyModFlags, 4 )
#define GUCEFINPUT_KEYMOD_RIGHT_SHIFT_STATE( keyModFlags )     GETBITX( keyModFlags, 5 )
#define GUCEFINPUT_KEYMOD_SHIFT_STATE( keyModFlags )           ( (GUCEFINPUT_KEYMOD_LEFT_SHIFT_STATE( keyModFlags )) || (GUCEFINPUT_KEYMOD_RIGHT_SHIFT_STATE( keyModFlags )) )

#define GUCEFINPUT_KEYMOD_CAPITAL_STATE( keyModFlags )         GETBITX( keyModFlags, 6 )

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *      Interface class for input handlers
 */
class EXPORT_CPP CIInputHandler
{
        public:        
        
        virtual void OnMouseButtonDown( CInputContext& context   ,
                                        const UInt32 buttonindex ) = 0;
        
        virtual void OnMouseButtonUp( CInputContext& context   ,
                                      const UInt32 buttonindex ) = 0;

        virtual void OnMouseMove( CInputContext& context   ,
                                  const Int32 xPos         ,
                                  const Int32 yPos         ,
                                  const Int32 xDelta       ,
                                  const Int32 yDelta       ) = 0;

        virtual void OnMouseVariableChanged( CInputContext& context ,
                                             const UInt32 varindex  ,
                                             const Int32 value      ,
                                             const Int32 valueDelta ) = 0;
        
        virtual void OnKeyboardKeyDown( CInputContext& context   ,
                                        const UInt32 keyindex    ,
                                        const UInt32 keyModState ) = 0;
        
        virtual void OnKeyboardKeyUp( CInputContext& context   ,
                                      const UInt32 keyindex    ,
                                      const UInt32 keyModState ) = 0;
        
        virtual void OnDeviceBooleanOn( CInputContext& context  ,
                                        const UInt32 deviceid   ,
                                        const UInt32 stateindex ) = 0;
                                        
        virtual void OnDeviceBooleanOff( CInputContext& context  ,
                                         const UInt32 deviceid   ,
                                         const UInt32 stateindex ) = 0;
                                         
        virtual void OnDeviceVariableChanged( CInputContext& context  ,
                                              const UInt32 deviceid   ,
                                              const UInt32 stateindex ,
                                              const Float32 value     ) = 0;                                                                                
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* CIINPUTDRIVER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 13-09-2005 :
        - Initial implementation

-----------------------------------------------------------------------------*/
