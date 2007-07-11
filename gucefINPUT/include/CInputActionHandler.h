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

#ifndef CINPUTACTIONHANDLER_H
#define CINPUTACTIONHANDLER_H

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

/**
 *      If you do not wish to use the profile manager then this class
 *      offers a simple alternative.
 */
class EXPORT_CPP CInputActionHandler : public CIInputHandler
{
        public:
        
        CInputActionHandler( void );
        
        CInputActionHandler( const CInputActionHandler& src );
        
        virtual ~CInputActionHandler();
        
        CInputActionHandler& operator=( const CInputActionHandler& src );                

        void SetActionMap( const CInputActionMap& actionmap );
        
        const CInputActionMap& GetActionMap( void ) const;
        
        bool LoadActionMapConfig( const CORE::CDataNode& treenode );
        
        bool SaveActionMapConfig( CORE::CDataNode& treenode );
        
        protected:
        
        virtual void OnMappedMouseButtonDown( const UInt32 actionindex ,
                                              const UInt32 buttonindex ) = 0;
        
        virtual void OnMappedMouseButtonUp( const UInt32 actionindex ,
                                            const UInt32 buttonindex ) = 0;
        
        virtual void OnMappedKeyboardKeyDown( const UInt32 actionindex ,
                                              const UInt32 keyindex    ) = 0;
        
        virtual void OnMappedKeyboardKeyUp( const UInt32 actionindex ,
                                            const UInt32 keyindex    ) = 0;
        
        virtual void OnMappedDeviceBooleanOn( const UInt32 actionindex ,
                                              const UInt32 deviceid    ,
                                              const UInt32 stateindex  ) = 0;
                                        
        virtual void OnMappedDeviceBooleanOff( const UInt32 actionindex ,
                                               const UInt32 deviceid    ,
                                               const UInt32 stateindex  ) = 0;
                                         
        virtual void OnMappedDeviceVariableChanged( const UInt32 actionindex ,
                                                    const UInt32 deviceid    ,
                                                    const UInt32 stateindex  ,
                                                    const Float32 value      ) = 0;
        
        CInputActionMap& GetMutableActionMap( void );
        
        protected:
        
        virtual void OnMouseButtonDown( const UInt32 buttonindex );
        
        virtual void OnMouseButtonUp( const UInt32 buttonindex );
        
        virtual void OnKeyboardKeyDown( const UInt32 keyindex );
        
        virtual void OnKeyboardKeyUp( const UInt32 keyindex );
        
        virtual void OnDeviceBooleanOn( const UInt32 deviceid   ,
                                        const UInt32 stateindex );
                                        
        virtual void OnDeviceBooleanOff( const UInt32 deviceid   ,
                                         const UInt32 stateindex );
                                         
        virtual void OnDeviceVariableChanged( const UInt32 deviceid   ,
                                              const UInt32 stateindex ,
                                              const Float32 value     );        
                                                                    
        private:
               
        CInputActionMap m_actionmap;                                                                                                                                    
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* CINPUTACTIONHANDLER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 13-09-2005 :
        - Initial implementation

-----------------------------------------------------------------------------*/
