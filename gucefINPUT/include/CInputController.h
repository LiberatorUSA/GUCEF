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

#ifndef CINPUTCONTROLLER_H
#define CINPUTCONTROLLER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CEVENTPUMPCLIENT_H
#include "CEventPumpClient.h"
#define CEVENTPUMPCLIENT_H
#endif /* CEVENTPUMPCLIENT_H ? */

#ifndef CDYNAMICARRAY_H
#include "CDynamicArray.h"
#define CDYNAMICARRAY_H
#endif /* CDYNAMICARRAY_H ? */

#ifndef CVALUELIST_H
#include "CValueList.h"
#define CVALUELIST_H
#endif /* CVALUELIST_H ? */

#ifndef GUCEFINPUT_MACROS_H
#include "gucefINPUT_macros.h"
#define GUCEFINPUT_MACROS_H
#endif /* GUCEFINPUT_MACROS_H ? */

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
class CIInputDriver;
class CIInputHandler;

/*-------------------------------------------------------------------------*/

class EXPORT_CPP CInputController : public CORE::CEventPumpClient
{
        public:                

        static CInputController* Instance( void );                
        
        CInputContext* CreateContext( const GUCEF::CORE::CValueList& params ,
                                      CIInputHandler* handler = NULL        );
        
        void DestroyContext( CInputContext* context );
        
        UInt32 GetContextCount( void ) const;
        
        bool SetDriver( CIInputDriver* driver );
        
        const CIInputDriver* GetDriver( void ) const;
                
        bool LoadDriverModule( const GUCEF::CORE::CString& filename ,
                               const CORE::CValueList& params       );

        protected:
        
        virtual void OnProcessEvent( const CORE::CEvent& event );
        
        virtual void OnUpdate( const UInt32 tickcount  ,
                               const UInt32 deltaticks );        

        private:
        friend class CGUCEFINPUTModule;
        
        static void Deinstance( void );        
        
        private:
        
        CInputController( void );
        CInputController( const CInputController& src );
        virtual ~CInputController();
        CInputController& operator=( const CInputController& src );
        
        void UnloadDriverModule( void );
        
        private:
        
        CIInputDriver* m_driver;
        bool m_driverisplugin;        
        CORE::CDynamicArray m_contextlist;
        UInt32 m_appinitevent;
        
        #ifdef MSWIN_BUILD
        UInt32 m_hinstance;
        #endif
        
        static CInputController* m_instance;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* CINPUTCONTROLLER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 13-09-2005 :
        - Initial implementation

-----------------------------------------------------------------------------*/
