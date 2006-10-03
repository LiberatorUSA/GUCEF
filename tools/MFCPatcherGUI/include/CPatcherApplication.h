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

#ifndef CPATCHERAPPLICATION_H
#define CPATCHERAPPLICATION_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_EXCEPTIONMACROS_H
#include "ExceptionMacros.h"
#define GUCEF_CORE_EXCEPTIONMACROS_H
#endif /* GUCEF_CORE_EXCEPTIONMACROS_H ? */

#ifndef GUCEF_CORE_CGUCEFAPPSUBSYSTEM_H
#include "CGUCEFAppSubSystem.h"
#define GUCEF_CORE_CGUCEFAPPSUBSYSTEM_H
#endif /* GUCEF_CORE_CGUCEFAPPSUBSYSTEM_H ? */

#ifndef GUCEF_PATCHER_CPATCHSETPARSER_H
#include "gucefPATCHER_CPatchSetParser.h"
#define GUCEF_PATCHER_CPATCHSETPARSER_H
#endif /* GUCEF_PATCHER_CPATCHSETPARSER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF::PATCHER::CPatchSetParserEventHandler;

/*-------------------------------------------------------------------------*/
          
class CPatcherApplication : public GUCEF::CORE::CGUCEFAppSubSystem
{
    public:
    
    typedef GUCEF::PATCHER::CPatchSetParserEventHandler TEventHandler;
    typedef std::vector< TEventHandler* > TEventHandlerList;
    
    static CPatcherApplication* Instance( void );
    
    void SetEventHandlers( const TEventHandlerList& eventHandler );

    GUCEF_DEFINE_MSGEXCEPTION( GUCEF_VOIDDEF, ENoEventHandler );
    
    protected:
    
    virtual void OnUpdate( const GUCEF::CORE::UInt32 applicationTicks ,
                           const GUCEF::CORE::UInt32 deltaTicks       );

    /**
     *  Event callback member function.
     *  Implement this in your decending class to handle
     *  notification events.
     *
     *  @param notifier the notifier that sent the notification
     *  @param eventid the unique event id for an event
     *  @param eventdata optional notifier defined userdata
     */
    virtual void OnNotify( GUCEF::CORE::CNotifier* notifier           ,
                           const GUCEF::CORE::UInt32 eventid          ,
                           GUCEF::CORE::CICloneable* eventdata = NULL );    

    private:
    friend class CPatcherGUIApp;
    
    static void Deinstance( void );
    
    private:
    CPatcherApplication( void );        
    CPatcherApplication( const CPatcherApplication& src );
    virtual ~CPatcherApplication();
    CPatcherApplication& operator=( const CPatcherApplication& src );
    
    private:        
    static CPatcherApplication* m_instance;
    TEventHandler* m_eventHandler;
    GUCEF::PATCHER::CPatchSetParser m_parser;
    TEventHandlerList m_eventHandlers;
    GUCEF::CORE::UInt32 m_appStartEventID;
};

/*-------------------------------------------------------------------------*/

#endif /* CPATCHERAPPLICATION_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 12-12-2005 :
        - Designed and implemented this class.

-----------------------------------------------------------------------------*/
