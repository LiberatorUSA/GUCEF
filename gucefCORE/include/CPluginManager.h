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

#ifndef GUCEF_CORE_CPLUGINMANAGER_H
#define GUCEF_CORE_CPLUGINMANAGER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_GUCEFCORE_MACROS_H
#include "gucefCORE_macros.h"         /* often used gucef macros */
#define GUCEF_CORE_GUCEFCORE_MACROS_H
#endif /* GUCEF_CORE_GUCEFCORE_MACROS_H ? */

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
class EXPORT_CPP CPluginManager : public CObservingNotifier
{
    public:

    static const CORE::CString PluginLoadedEvent;
    static const CORE::CString PluginUnloadedEvent;
    
    CPluginManager( void );
    
    CPluginManager( const CPluginManager& src );
    
    virtual ~CPluginManager();              
    
    CString GetPluginDir( void ) const;        
    
    virtual void LoadAll( void ) = 0;
    
    virtual void UnloadAll( void ) = 0;
    
    UInt32 GetPluginLoadedEventID( void ) const;
    
    UInt32 GetPluginUnloadedEventID( void ) const;
    
    static void RegisterEvents( void );
    
    protected:
    friend class CPluginControl;
    
    virtual void OnSetPluginDir( const CString& path );

    /**
     *  Event callback member function.
     *  Implement this in your decending class to handle
     *  notification events.
     *
     *  @param notifier the notifier that sent the notification
     *  @param eventid the unique event id for an event
     *  @param eventdata optional notifier defined userdata
     */
    virtual void OnNotify( CNotifier* notifier           ,
                           const UInt32 eventid          ,
                           CICloneable* eventdata = NULL );

    private:
    
    CPluginManager& operator=( const CPluginManager& src );
    
    private:
    friend class CPluginControl;
    
    UInt32 _managerid; /**< ID used to quickly manage this manager at the plugin control center */
    UInt32 m_pluginLoadedEventID;
    UInt32 m_pluginUnloadedEventID;
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