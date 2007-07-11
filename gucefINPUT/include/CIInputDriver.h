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
