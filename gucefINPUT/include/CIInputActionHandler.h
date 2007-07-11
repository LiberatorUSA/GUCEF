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
                                                    