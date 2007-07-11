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

#ifndef CINPUTACTIONMULTIMAPHANDLER_H
#define CINPUTACTIONMULTIMAPHANDLER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CDYNAMICARRAY_H
#include "CDynamicArray.h"
#define CDYNAMICARRAY_H
#endif /* CDYNAMICARRAY_H ? */

#ifndef CIINPUTACTIONHANDLER_H
#include "CIInputActionHandler.h"
#define CIINPUTACTIONHANDLER_H
#endif /* CIINPUTACTIONHANDLER_H ? */

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
 *      Maps the input action event handlers to multiple other event handlers.
 */
class EXPORT_CPP CInputActionMultiMapHandler : public CIInputActionHandler
{
        public:
        
        CInputActionMultiMapHandler( void );
        
        ~CInputActionMultiMapHandler();
        
        void RegisterActionHandler( CIInputActionHandler* handler );
        
        bool UnregisterActionHandler( CIInputActionHandler* handler );
        
        void Clear( void );
        
        UInt32 GetCount( void ) const;
        
        CIInputActionHandler* GetHandler( const UInt32 index ) const;        
        
        virtual bool IsInputEnabled( void ) const;
        
        virtual CORE::CString GetTypeName( void ) const;
        
        protected:
        
        virtual void OnMappedMouseButtonDown( CInputContext& context   ,
                                              const UInt32 actionindex ,
                                              const UInt32 buttonindex );
        
        virtual void OnMappedMouseButtonUp( CInputContext& context   ,
                                            const UInt32 actionindex ,
                                            const UInt32 buttonindex );
                                            
        virtual void OnMappedMouseMove( CInputContext& context   ,
                                        const UInt32 actionindex ,
                                        const Int32 xPos         ,
                                        const Int32 yPos         ,
                                        const Int32 xDelta       ,
                                        const Int32 yDelta       );

        virtual void OnMappedMouseVariableChanged( CInputContext& context   ,
                                                   const UInt32 actionindex ,
                                                   const UInt32 varindex    ,
                                                   const Int32 value        ,
                                                   const Int32 valueDelta   );
        
        virtual void OnMappedKeyboardKeyDown( CInputContext& context   ,
                                              const UInt32 actionindex ,
                                              const UInt32 keyindex    ,
                                              const UInt32 keyModState );
        
        virtual void OnMappedKeyboardKeyUp( CInputContext& context   ,
                                            const UInt32 actionindex ,
                                            const UInt32 keyindex    ,
                                            const UInt32 keyModState );
        
        virtual void OnMappedDeviceBooleanOn( CInputContext& context   ,
                                              const UInt32 actionindex ,
                                              const UInt32 deviceid    ,
                                              const UInt32 stateindex  );
                                        
        virtual void OnMappedDeviceBooleanOff( CInputContext& context   ,
                                               const UInt32 actionindex ,
                                               const UInt32 deviceid    ,
                                               const UInt32 stateindex  );
                                         
        virtual void OnMappedDeviceVariableChanged( CInputContext& context   ,
                                                    const UInt32 actionindex ,
                                                    const UInt32 deviceid    ,
                                                    const UInt32 stateindex  ,
                                                    const Float32 value      );
                                                    
        private:
        
        CInputActionMultiMapHandler( const CInputActionMultiMapHandler& src );            /**< not implemented */
        CInputActionMultiMapHandler& operator=( const CInputActionMultiMapHandler& src ); /**< not implemented */
        
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
          
#endif /* CINPUTACTIONMULTIMAPHANDLER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 09-10-2005 :
        - Initial implementation

-----------------------------------------------------------------------------*/
