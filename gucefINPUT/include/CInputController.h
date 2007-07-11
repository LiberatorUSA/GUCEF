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

#ifndef CINPUTCONTROLLER_H
#define CINPUTCONTROLLER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CGUCEFAPPSUBSYSTEM_H
#include "CGUCEFAppSubSystem.h"
#define GUCEF_CORE_CGUCEFAPPSUBSYSTEM_H
#endif /* GUCEF_CORE_CGUCEFAPPSUBSYSTEM_H ? */

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

class EXPORT_CPP CInputController : public CORE::CGUCEFAppSubSystem
{
    public:                

    static const CORE::CEvent InputDriverLoadedEvent;
    static const CORE::CEvent InputDriverUnloadedEvent;

    static void RegisterEvents( void );
    
    public:
    
    static CInputController* Instance( void );                
    
    CInputContext* CreateContext( const CORE::CValueList& params ,
                                  CIInputHandler* handler = NULL );
    
    void DestroyContext( CInputContext* context );
    
    UInt32 GetContextCount( void ) const;
    
    bool SetDriver( CIInputDriver* driver );
    
    const CIInputDriver* GetDriver( void ) const;
            
    bool LoadDriverModule( const CORE::CString& filename  ,
                           const CORE::CValueList& params );
    
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
    
    CInputController( void );
    CInputController( const CInputController& src );
    virtual ~CInputController();
    CInputController& operator=( const CInputController& src );
    
    void UnloadDriverModule( void );
    
    private:
    
    CIInputDriver* m_driver;
    bool m_driverisplugin;        
    CORE::CDynamicArray m_contextlist;
    
    #ifdef GUCEF_MSWIN_BUILD
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
