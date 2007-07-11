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

#ifndef CINPUTHANDLERMULTIMAP_H
#define CINPUTHANDLERMULTIMAP_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CDYNAMICARRAY_H
#include "CDynamicArray.h"
#define CDYNAMICARRAY_H
#endif /* CDYNAMICARRAY_H ? */ 

#ifndef CINPUTCONTEXT_H
#include "CInputContext.h"
#define CINPUTCONTEXT_H
#endif /* CINPUTCONTEXT_H ? */

#ifndef CIINPUTHANDLER_H
#include "CIInputHandler.h"
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

/**
 *      Interface class for input handlers
 */
class EXPORT_CPP CInputHandlerMultiMap : public CIInputHandler
{
        public:
        
        CInputHandlerMultiMap( void );
        
        virtual ~CInputHandlerMultiMap();
        
        void RegisterHandler( CIInputHandler* handler );
        
        void UnregisterHandler( CIInputHandler* handler );
        
        void Clear( void );
        
        UInt32 GetCount( void ) const;
        
        CIInputHandler* GetHandler( const UInt32 index ) const;
        
        public:        
        
        virtual void OnMouseButtonDown( CInputContext& context   ,
                                        const UInt32 buttonindex );
        
        virtual void OnMouseButtonUp( CInputContext& context   ,
                                      const UInt32 buttonindex );

        virtual void OnMouseMove( CInputContext& context   ,
                                  const Int32 xPos         ,
                                  const Int32 yPos         ,
                                  const Int32 xDelta       ,
                                  const Int32 yDelta       );

        virtual void OnMouseVariableChanged( CInputContext& context ,
                                             const UInt32 varindex  ,
                                             const Int32 value      ,
                                             const Int32 valueDelta );
        
        virtual void OnKeyboardKeyDown( CInputContext& context   ,
                                        const UInt32 keyindex    ,
                                        const UInt32 keyModState );
        
        virtual void OnKeyboardKeyUp( CInputContext& context   ,
                                      const UInt32 keyindex    ,
                                      const UInt32 keyModState );
        
        virtual void OnDeviceBooleanOn( CInputContext& context  ,
                                        const UInt32 deviceid   ,
                                        const UInt32 stateindex );
                                        
        virtual void OnDeviceBooleanOff( CInputContext& context  ,
                                         const UInt32 deviceid   ,
                                         const UInt32 stateindex );
                                         
        virtual void OnDeviceVariableChanged( CInputContext& context  ,
                                              const UInt32 deviceid   ,
                                              const UInt32 stateindex ,
                                              const Float32 value     );
                                              
        private:
        
        CORE::CDynamicArray m_list;                                              
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* CINPUTHANDLERMULTIMAP_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 09-10-2005 :
        - Initial implementation

-----------------------------------------------------------------------------*/
