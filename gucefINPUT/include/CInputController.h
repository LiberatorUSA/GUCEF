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

#ifndef GUCEF_INPUT_CINPUTCONTROLLER_H
#define GUCEF_INPUT_CINPUTCONTROLLER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CGUCEFAPPSUBSYSTEM_H
#include "CGUCEFAppSubSystem.h"
#define GUCEF_CORE_CGUCEFAPPSUBSYSTEM_H
#endif /* GUCEF_CORE_CGUCEFAPPSUBSYSTEM_H ? */

#ifndef GUCEF_CORE_CDYNAMICARRAY_H
#include "CDynamicArray.h"
#define GUCEF_CORE_CDYNAMICARRAY_H
#endif /* GUCEF_CORE_CDYNAMICARRAY_H ? */

#ifndef GUCEF_CORE_CVALUELIST_H
#include "CValueList.h"
#define GUCEF_CORE_CVALUELIST_H
#endif /* GUCEF_CORE_CVALUELIST_H ? */

#ifndef GUCEF_INPUT_CKEYBOARD_H
#include "gucefINPUT_CKeyboard.h"
#define GUCEF_INPUT_CKEYBOARD_H
#endif /* GUCEF_INPUT_CKEYBOARD_H ? */

#ifndef GUCEF_INPUT_CMOUSE_H
#include "gucefINPUT_CMouse.h"
#define GUCEF_INPUT_CMOUSE_H
#endif /* GUCEF_INPUT_CMOUSE_H ? */

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
class CInputDriver;

/*-------------------------------------------------------------------------*/

class GUCEF_INPUT_EXPORT_CPP CInputController : public CORE::CGUCEFAppSubSystem
{
    public:                

    static const CORE::CEvent InputDriverLoadedEvent;
    static const CORE::CEvent InputDriverUnloadedEvent;

    static void RegisterEvents( void );
    
    public:
    
    static CInputController* Instance( void );                
    
    CInputContext* CreateContext( const CORE::CValueList& params );
    
    void DestroyContext( CInputContext* context );
    
    UInt32 GetContextCount( void ) const;
    
    bool SetDriver( CInputDriver* driver );
    
    const CInputDriver* GetDriver( void ) const;
            
    bool LoadDriverModule( const CORE::CString& filename  ,
                           const CORE::CValueList& params );

    CKeyboard& GetKeyboard( void ) const;
    
    CMouse& GetMouse( const UInt32 index = 0 ) const;
    
    UInt32 GetMouseCount( void ) const;
    
    GUCEF_DEFINE_MSGEXCEPTION( GUCEF_INPUT_EXPORT_CPP, EInvalidIndex );
    
    protected:
        
    /**
     *  Event callback member function.
     *  Implement this in your decending class to handle
     *  notification events.
     *
     *  @param notifier the notifier that sent the notification
     *  @param eventid the unique event id for an event
     *  @param eventdata optional notifier defined userdata
     */
    virtual void OnNotify( CORE::CNotifier* notifier           ,
                           const CORE::CEvent& eventid         ,
                           CORE::CICloneable* eventdata = NULL );
        
    virtual void OnUpdate( const UInt64 applicationTicks        ,
                           const Float64 updateDeltaInMilliSecs );

    private:
    friend class CGUCEFINPUTModule;
    
    static void Deinstance( void );        

    private:
    friend class CInputDriver;
    
    void SetMouseButtonState( const UInt32 deviceIndex ,
                              const UInt32 buttonIndex , 
                              const bool pressedState  );

    void SetMousePos( const UInt32 deviceIndex ,
                      const UInt32 xPos        ,
                      const UInt32 yPos        ,
                      const Int32 xDelta       ,
                      const Int32 yDelta       );
                         
    void ResetMouseStates( const UInt32 deviceIndex );    

    void SetKeyboardKeyState( const KeyCode keyCode  ,
                              const bool keyPressed  );
    
    private:
    
    CInputController( void );
    CInputController( const CInputController& src );
    virtual ~CInputController();
    CInputController& operator=( const CInputController& src );
    
    void UnloadDriverModule( void );
    
    private:
    
    typedef std::vector< CMouse* > TMouseVector;  
    
    CInputDriver* m_driver;
    bool m_driverisplugin;        
    CORE::CDynamicArray m_contextlist;
    
    #ifdef GUCEF_MSWIN_BUILD
    UInt32 m_hinstance;
    #endif
    
    static CInputController* m_instance;
    
    CKeyboard* m_keyboard;
    TMouseVector m_mouseVector;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace INPUT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_INPUT_CINPUTCONTROLLER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 13-09-2005 :
        - Initial implementation

-----------------------------------------------------------------------------*/
