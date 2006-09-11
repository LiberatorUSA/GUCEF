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

#ifndef CIINPUTACTIONHANDLER_H
#define CIINPUTACTIONHANDLER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CINPUTCONTEXT_H
#include "CInputContext.h"      /* abstract input context class, placeholder for data */
#define CINPUTCONTEXT_H
#endif /* CINPUTCONTEXT_H ? */

#ifndef CIINPUTHANDLER_H
#include "CIInputHandler.h"     /* included for it's macros only */
#define CIINPUTHANDLER_H
#endif /* CIINPUTHANDLER_H ? */

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

class CInputToActionHandler;
class CInputActionMultiMapHandler;

/*-------------------------------------------------------------------------*/

/**
 *      Interface class for action mapped input handlers
 */        
class EXPORT_CPP CIInputActionHandler
{        
        public:
        
        virtual bool IsInputEnabled( void ) const = 0;
        
        virtual CORE::CString GetTypeName( void ) const = 0;
        
        protected:
        friend class CInputToActionHandler;
        friend class CInputActionMultiMapHandler;
        
        virtual void OnMappedMouseButtonDown( CInputContext& context   ,
                                              const UInt32 actionindex ,
                                              const UInt32 buttonindex ) = 0;
        
        virtual void OnMappedMouseButtonUp( CInputContext& context   ,
                                            const UInt32 actionindex ,
                                            const UInt32 buttonindex ) = 0;

        virtual void OnMappedMouseMove( CInputContext& context   ,
                                        const UInt32 actionindex ,
                                        const Int32 xPos         ,
                                        const Int32 yPos         ,
                                        const Int32 xDelta       ,
                                        const Int32 yDelta       ) = 0;

        virtual void OnMappedMouseVariableChanged( CInputContext& context   ,
                                                   const UInt32 actionindex ,
                                                   const UInt32 varindex    ,
                                                   const Int32 value        ,
                                                   const Int32 valueDelta   ) = 0;
                                                           
        virtual void OnMappedKeyboardKeyDown( CInputContext& context   ,
                                              const UInt32 actionindex ,
                                              const UInt32 keyindex    ,
                                              const UInt32 keyModState ) = 0;
        
        virtual void OnMappedKeyboardKeyUp( CInputContext& context   ,
                                            const UInt32 actionindex ,
                                            const UInt32 keyindex    ,
                                            const UInt32 keyModState ) = 0;
        
        virtual void OnMappedDeviceBooleanOn( CInputContext& context   ,
                                              const UInt32 actionindex ,
                                              const UInt32 deviceid    ,
                                              const UInt32 stateindex  ) = 0;
                                        
        virtual void OnMappedDeviceBooleanOff( CInputContext& context   ,
                                               const UInt32 actionindex ,
                                               const UInt32 deviceid    ,
                                               const UInt32 stateindex  ) = 0;
                                         
        virtual void OnMappedDeviceVariableChanged( CInputContext& context   ,
                                                    const UInt32 actionindex ,
                                                    const UInt32 deviceid    ,
                                                    const UInt32 stateindex  ,
                                                    const Float32 value      ) = 0;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* CIINPUTACTIONHANDLER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 09-10-2005 :
        - Initial implementation

-----------------------------------------------------------------------------*/
                                                    