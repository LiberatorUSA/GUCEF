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

#ifndef CIINPUTDRIVER_H
#define CIINPUTDRIVER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef ESTRUCTS_H
#include "EStructs.h"
#define ESTRUCTS_H
#endif /* ESTRUCTS_H ? */

#ifndef CINPUTCONTEXT_H
#include "CInputContext.h"
#define CINPUTCONTEXT_H
#endif /* CINPUTCONTEXT_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF{ namespace CORE { class CValueList; } }

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

class CInputContext;

/*-------------------------------------------------------------------------*/

class EXPORT_CPP CIInputDriver
{
        public:
        
        virtual const CORE::TVersion* GetVersion( void ) = 0;

        virtual void GetMousePos( CInputContext* context ,
                                  UInt32* xpos           ,
                                  UInt32* ypos           ) = 0;
        
        virtual const UInt8* GetKeyboardKeyStates( CInputContext* context ) = 0;
        
        virtual bool GetMouseButtonPressedState( CInputContext* context   ,
                                                 const UInt32 buttonindex ) = 0;
        
        virtual bool GetKeyboardKeyPressedState( CInputContext* context ,
                                                 const UInt32 keyindex  ) = 0;
        
        virtual bool GetDeviceBooleanState( CInputContext* context  ,
                                            const UInt32 deviceid   ,
                                            const UInt32 stateindex ) = 0;  
        
        virtual bool GetDeviceVariableState( CInputContext* context  ,
                                             const UInt32 deviceid   ,
                                             const UInt32 stateindex ,
                                             Float32& varstate       ) = 0;
        
        protected:
        friend class CInputController;
        
        virtual bool OnUpdate( const UInt64 tickcount               ,
                               const Float64 updateDeltaInMilliSecs ,
                               CInputContext* context               ) = 0;        
        
        virtual CInputContext* CreateContext( const CORE::CValueList& params ,
                                              CIInputHandler* handler = NULL ) = 0;
        
        virtual void DeleteContext( CInputContext* context ) = 0;                                      
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
