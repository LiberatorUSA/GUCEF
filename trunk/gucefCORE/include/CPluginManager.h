/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
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

#ifndef GUCEF_CORE_CPLUGINMANAGER_H
#define GUCEF_CORE_CPLUGINMANAGER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"         /* often used gucef macros */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

#ifndef GUCEF_CORE_CIPLUGIN_H
#include "CIPlugin.h"
#define GUCEF_CORE_CIPLUGIN_H
#endif /* GUCEF_CORE_CIPLUGIN_H ? */

#ifndef GUCEF_CORE_CTSHAREDPTR_H
#include "CTSharedPtr.h"
#define GUCEF_CORE_CTSHAREDPTR_H
#endif /* GUCEF_CORE_CTSHAREDPTR_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Forward declarations of framework classes used here
 */
class CString;
class CPluginControl;

/*-------------------------------------------------------------------------*/

/**
 *      Generic base class for plugin managers
 */
class GUCEFCORE_EXPORT_CPP CPluginManager : public CObservingNotifier
{
    public:

    static const CEvent PluginLoadedEvent;
    static const CEvent PluginUnloadedEvent;

    static void RegisterEvents( void );
    
    public:
    
    typedef CTSharedPtr< CIPlugin > TPluginPtr;
    
    CPluginManager( void );
    
    CPluginManager( const CPluginManager& src );
    
    virtual ~CPluginManager();              
    
    CString GetPluginDir( void ) const;        
    
    virtual void LoadAll( void );
    
    virtual void UnloadAll( void ) = 0;
    
    virtual TPluginPtr LoadPlugin( const CString& pluginPath );
    
    protected:
    friend class CPluginControl;
    
    virtual void OnSetPluginDir( const CString& path );

    /**
     *  Event callback member function.
     *  Implement this in your descending class to handle
     *  notification events.
     *
     *  @param notifier the notifier that sent the notification
     *  @param eventid the unique event id for an event
     *  @param eventdata optional notifier defined user data
     */
    virtual void OnNotify( CNotifier* notifier           ,
                           const UInt32 eventid          ,
                           CICloneable* eventdata = NULL );

    private:
    
    CPluginManager& operator=( const CPluginManager& src );
    
    private:
    friend class CPluginControl;
    
    UInt32 _managerid; /**< ID used to quickly manage this manager at the plugin control center */
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CPLUGINMANAGER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 30-09-2006 :
        - Dinand: Recoded the notification after crash wiped out the new code
- 27-11-2004 :
        - Dinand: Initial implementation

-----------------------------------------------------------------------------*/