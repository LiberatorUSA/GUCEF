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

#ifndef CINPUTDRIVERPLUGIN_H
#define CINPUTDRIVERPLUGIN_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CIINPUTDRIVER_H
#include "CIInputDriver.h"         /* base class fo input driver implementations */
#define CIINPUTDRIVER_H
#endif /* CIINPUTDRIVER_H ? */

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
 *      Class that connects the input driver interface to a plugin mechanism so
 *      that plugins can be used as an input driver.
 */
class EXPORT_CPP CInputDriverPlugin : public CIInputDriver
{
        public:
        
        CInputDriverPlugin( void );
        
        virtual ~CInputDriverPlugin();        

        bool LoadModule( const CORE::CString& filename  , 
                         const CORE::CValueList& params );
        
        void UnloadModule( void );
                
        virtual const CORE::TVersion* GetVersion( void );
        
        virtual void GetMousePos( CInputContext* context ,
                                  UInt32* xpos           ,
                                  UInt32* ypos           );
        
        virtual const UInt8* GetKeyboardKeyStates( CInputContext* context );
        
        virtual bool GetMouseButtonPressedState( CInputContext* context   ,
                                                 const UInt32 buttonindex );
        
        virtual bool GetKeyboardKeyPressedState( CInputContext* context ,
                                                 const UInt32 keyindex  );
        
        virtual bool GetDeviceBooleanState( CInputContext* context  ,
                                            const UInt32 deviceid   ,
                                            const UInt32 stateindex );  
        
        virtual bool GetDeviceVariableState( CInputContext* context  ,
                                             const UInt32 deviceid   ,
                                             const UInt32 stateindex ,
                                             Float32& varstate       );
                                                
        protected:
        
        virtual bool OnUpdate( const UInt32 tickcount  ,
                               const UInt32 deltaticks ,
                               CInputContext* context  );

        virtual CInputContext* CreateContext( const CORE::CValueList& params ,
                                              CIInputHandler* handler = NULL );
        
        virtual void DeleteContext( CInputContext* context );

        private:
        
        static char*** CreateArgMatrix( const CORE::CValueList& params );
        static void DestroyArgMatrix( char*** argmatrix );
        
        CInputDriverPlugin( const CInputDriverPlugin& src );            /**< dummy, do not use */ 
        CInputDriverPlugin& operator=( const CInputDriverPlugin& src ); /**< dummy, do not use */ 
        
        private:
        
        void* m_sohandle;
        void* m_plugdata;
        void* m_fptable[ 14 ];
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* CINPUTDRIVERPLUGIN_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 16-09-2005 :
        - Initial implementation

-----------------------------------------------------------------------------*/
                                                