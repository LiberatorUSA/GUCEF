/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
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

#ifndef GUCEF_CORE_CGUCEFAPPLICATION_H
#define GUCEF_CORE_CGUCEFAPPLICATION_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <set>

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifdef GUCEF_MSWIN_BUILD
#include <windows.h>                    /* Micro$oft Windows API */
#endif /* GUCEF_MSWIN_BUILD ? */

#ifndef GUCEF_CORE_CTSGNOTIFIER_H
#include "CTSGNotifier.h"
#define GUCEF_CORE_CTSGNOTIFIER_H
#endif /* GUCEF_CORE_CTSGNOTIFIER_H ? */

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

#ifndef GUCEF_CORE_CISYSCONSOLECMDHANDLER_H
#include "CISysConsoleCmdHandler.h"
#define GUCEF_CORE_CISYSCONSOLECMDHANDLER_H
#endif /* GUCEF_CORE_CISYSCONSOLECMDHANDLER_H ? */

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "CIConfigurable.h"             /* abstract base class interface for configurable objects */
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_CORE_CTCLONEABLEOBJ_H
#include "CTCloneableObj.h"
#define GUCEF_CORE_CTCLONEABLEOBJ_H
#endif /* GUCEF_CORE_CTCLONEABLEOBJ_H ? */

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

class CGUCEFAppSubSystem;
class CIGUCEFApplicationDriver;

/*-------------------------------------------------------------------------*/

/**
 *      
 *      Dispatches the following events:
 *       - AppInitEvent:
 *              Indicates that the application is now initialized
 *              at the GUCEF level. All higher level code should
 *              now be initialized.
 *       - AppShutdownEvent
 *              Signals that the application is shutting down at the
 *              GUCEF level. All higher level code should shutdown as well 
 */
class GUCEFCORE_EXPORT_CPP CGUCEFApplication : public CTSGNotifier          , 
                                               public CIConfigurable        ,
                                               public CISysConsoleCmdHandler
{
        public:

        static const CEvent AppInitEvent;
        static const CEvent AppShutdownEvent;

        public:
        
        struct SAppInitEventData
        {
                #ifdef GUCEF_MSWIN_BUILD
                HINSTANCE hinstance;
                LPSTR lpcmdline;
                int ncmdshow;
                #endif 
                
                int argc;
                char** argv;                
        };
        typedef CTCloneableObj< struct SAppInitEventData > TAppInitEventData;

        public:        
        
        static CGUCEFApplication* Instance( void );

        #ifdef GUCEF_MSWIN_BUILD
        int Main( HINSTANCE hinstance     ,
                  LPSTR lpcmdline         ,
                  int ncmdshow            ,
                  bool run                );
        #endif
        
        int main( int argc    ,
                  char** argv ,
                  bool run    );
        

        void Update( void );

        void Stop( void );
                
        void SetPluginDir( const CString& plugindir );        
                       
        CString GetPluginDir( void ) const;
        
        CString GetApplicationDir( void ) const;
        
        UInt32 GetLastUpdateTickCount( void ) const;
        
        void SetCycleDelay( UInt32 maxdeltaticks ,
                            UInt32 delay         );
                            
        void GetCycleDelay( UInt32& maxdeltaticks ,
                            UInt32& delay         ) const;                            
        
        /**
         *      Attempts to store the given tree in the file
         *      given according to the method of the codec metadata
         *
         *      @param tree the data tree you wish to store
         *      @return wheter storing the tree was successfull
         */
        virtual bool SaveConfig( CDataNode& tree );
                                    
        /**
         *      Attempts to load data from the given file to the 
         *      root node given. The root data will be replaced 
         *      and any children the node may already have will be deleted.
         *
         *      @param treeroot pointer to the node that is to act as root of the data tree
         *      @return whether building the tree from the given file was successfull.
         */                                    
        virtual bool LoadConfig( const CDataNode& treeroot );
        
        void SetApplicationDriver( CIGUCEFApplicationDriver* appDriver );
        
        CIGUCEFApplicationDriver* GetApplicationDriver( void ) const;
        
        bool GetRequiresPeriodicUpdate( void ) const;
        
        UInt32 GetMinimalReqUpdateResolution( void ) const;
        
        static void RegisterEvents( void );
        
        protected:
        
        /**
         *  Event callback member function.
         *
         *  @param notifier the notifier that sent the notification
         *  @param eventid the unique event id for an event
         *  @param eventdata optional notifier defined userdata
         */
        virtual void OnPumpedNotify( CNotifier* notifier           ,
                                     const CEvent& eventid         ,
                                     CICloneable* eventdata = NULL );

        virtual bool OnSysConsoleCommand( const CString& path     ,
                                          const CString& command  ,
                                          const CStringList& args ,
                                          CStringList& resultdata );
                                          
        virtual void OnUpdate( const UInt32 tickcount  ,
                               const UInt32 deltaticks );
                               
        virtual void LockData( void );
        
        virtual void UnlockData( void );                                                                                 
        
        private:
        friend class CGUCEFCOREModule;
        
        static void Deinstance( void );
        
        private:
        friend class CGUCEFAppSubSystem;
        
        void RegisterSubSystem( CGUCEFAppSubSystem* subSystem );
        
        void UnregisterSubSystem( CGUCEFAppSubSystem* subSystem );
        
        void RefreshMinimalSubSysInterval( void );
        
        void RefreshPeriodicUpdateRequirement( void );
        
        void DoRequestSubSysUpdate( void );

        private:

        void Run( void );
        CGUCEFApplication( void );
        CGUCEFApplication( const CGUCEFApplication& src );
        virtual ~CGUCEFApplication();
        
        private:
        typedef std::set< CGUCEFAppSubSystem* > TSubSystemList;
        
        TSubSystemList m_subSysList;
        bool m_requiresPeriodicUpdates;
        bool m_inNeedOfAnUpdate;
        UInt32 m_minimalUpdateDelta;
        UInt32 m_appTickCount;
        CIGUCEFApplicationDriver* m_appDriver;
        
        bool _initialized;
        bool _active;
        CString _appdir;
        UInt32 m_maxdeltaticksfordelay;
        UInt32 m_delaytime;
        
        static CGUCEFApplication* _instance;
        static MT::CMutex m_mutex;
};


/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* CGUCEFAPPLICATION_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 27-08-2005 :
        - Added GetCycleDelay()
- 12-11-2004 :
        - Designed and implemented this class.

-----------------------------------------------------------------------------*/
 