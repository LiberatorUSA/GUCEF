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
        
        virtual bool OnUpdate( const UInt64 tickcount               ,
                               const Float64 updateDeltaInMilliSecs ,
                               CInputContext* context               );

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
                                                