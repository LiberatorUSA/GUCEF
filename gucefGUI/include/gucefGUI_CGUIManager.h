/*
 *  gucefGUI: GUCEF module providing a uniform interface towards GUI backends
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
 */

#ifndef GUCEF_GUI_CGUIMANAGER_H
#define GUCEF_GUI_CGUIMANAGER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CTEVENTHANDLERFUNCTOR_H
#include "gucefCORE_CTEventHandlerFunctor.h"
#define GUCEF_CORE_CTEVENTHANDLERFUNCTOR_H
#endif /* GUCEF_CORE_CTEVENTHANDLERFUNCTOR_H ? */

#ifndef GUCEF_CORE_CLONEABLES_H
#include "cloneables.h"
#define GUCEF_CORE_CLONEABLES_H
#endif /* GUCEF_CORE_CLONEABLES_H ? */

#ifndef GUCEF_GUI_CWIDGETFACTORY_H
#include "gucefGUI_CWidgetFactory.h"
#define GUCEF_GUI_CWIDGETFACTORY_H
#endif /* GUCEF_GUI_CWIDGETFACTORY_H ? */

#ifndef GUCEF_GUI_CFORMFACTORY_H
#include "gucefGUI_CFormFactory.h"
#define GUCEF_GUI_CFORMFACTORY_H
#endif /* GUCEF_GUI_CFORMFACTORY_H ? */

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUI {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_GUI_EXPORT_CPP CGUIManager : public CORE::CObservingNotifier
{
    public:
    
    static const CORE::CEvent FormFactoryRegisteredEvent;
    static const CORE::CEvent FormFactoryUnregisteredEvent;
    
    typedef CORE::TCloneableString TKeyContainer;  /** event data type used for the keys */
    
    static void RegisterEvents( void );
    
    public:
    
    static CGUIManager* Instance( void );
    
    CGUIContext* CreateGUIContext( const CString& guiDriverName );
    
    void DestroyGUIContext( CGUIContext* context );
    
    void RegisterGUIDriver( CIGUIDriver* guiDriver );
    
    void UnregisterGUIDriver( CIGUIDriver* guiDriver );
    
    void UnregisterGUIDriverByName( const CString& guiDriverName );
    
    private:
    friend class CModule;    
    
    static void Deinstance( void );
    
    private:
    
    typedef CORE::CTEventHandlerFunctor< CGUIManager > TEventCallback;
    
    CGUIManager( void );
    CGUIManager( const CGUIManager& src );
    virtual ~CGUIManager();
    CGUIManager& operator=( const CGUIManager& src );
    
    void OnFormFactoryRegistered( CORE::CNotifier* notifier           ,
                                  const CORE::CEvent& eventid         ,
                                  CORE::CICloneable* eventdata = NULL );

    void OnFormFactoryUnregistered( CORE::CNotifier* notifier           ,
                                    const CORE::CEvent& eventid         ,
                                    CORE::CICloneable* eventdata = NULL );
                                      
    private:
    
    typedef std::map< CString, CIGUIDriver* > TGUIDriverMap;
    
    CWidgetFactory m_widgetFactory;
    CFormFactory m_formFactory;
    CFormBackendFactory* m_formBackendFactory;
    TGUIDriverMap m_guiDrivers;
    
    static CGUIManager* g_instance;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_GUI_CGUIMANAGER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-08-2007 :
        - Dinand: Initial implementation

-----------------------------------------------------------------------------*/