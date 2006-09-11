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
