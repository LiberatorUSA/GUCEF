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
