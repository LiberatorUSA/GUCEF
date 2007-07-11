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

#ifndef CINPUTTOACTIONHANDLER_H
#define CINPUTTOACTIONHANDLER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CIINPUTHANDLER_H
#include "CIInputHandler.h"
#define CIINPUTHANDLER_H
#endif /* CIINPUTHANDLER_H ? */

#ifndef CINPUTACTIONMAP_H
#include "CInputActionMap.h"
#define CINPUTACTIONMAP_H
#endif /* CINPUTACTIONMAP_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF { namespace CORE { class CDataNode; } }

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

class CInputActionMap;
class CIInputActionHandler;

/*-------------------------------------------------------------------------*/

/**
 *      Converts normal input events into action mapped events.
 *      both the action map and the handler are not owned by instances of 
 *      this class, they are merely refrenced.
 */
class EXPORT_CPP CInputToActionHandler : public CIInputHandler
{
        public:
        
        CInputToActionHandler( CInputActionMap& actionmap           ,
                               CIInputActionHandler* handler = NULL );
        
        virtual ~CInputToActionHandler();
        
        void SetActionHandler( CIInputActionHandler* handler );
        
        CIInputActionHandler* GetActionHandler( void ) const;
        
        protected:
        
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
        
        CInputToActionHandler( void );
        CInputToActionHandler( const CInputToActionHandler& src );
        CInputToActionHandler& operator=( const CInputToActionHandler& src );
        
        private:
        
        CInputActionMap& m_actionmap;
        CIInputActionHandler* m_handler;                                                      
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* CINPUTTOACTIONHANDLER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 09-10-2005 :
        - Initial implementation

-----------------------------------------------------------------------------*/
