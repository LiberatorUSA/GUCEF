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
