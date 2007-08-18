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
#undef min
#undef max
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
 *              now begin initialization.
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

        static void RegisterEvents( void );
               
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
        
        UInt64 GetLastUpdateTickCount( void ) const;
        
        /**
         *  If you want to make sure your application does not cycle
         *  to fast (perhaps to prevent the application from hogging the
         *  system resources), you can use this member function to limit
         *  the cycle rate.
         */
        void SetCycleDelay( const Float64 minimalCycleDeltaInMilliSecs ,
                            const UInt32 cycleDelayInMilliSecs         );
                            
        void GetCycleDelay( Float64& minimalCycleDeltaInMilliSecs ,
                            UInt32& cycleDelayInMilliSecs         ) const;                            
        
        /**
         *      Attempts to store the given tree in the file
         *      given according to the method of the codec meta data
         *
         *      @param tree the data tree you wish to store
         *      @return whether storing the tree was successfull
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
        
        /**
         *  returns the minimal required resolution for the application
         *  update interval in milliseconds as requested by the application
         *  sub systems.
         *
         *  @return the minimal required update frequency in milliseconds
         */
        Float64 GetMinimalReqUpdateResolution( void ) const;
        
        protected:
        
        /**
         *  Event callback member function.
         *
         *  @param notifier the notifier that sent the notification
         *  @param eventid the unique event id for an event
         *  @param eventdata optional notifier defined user data
         */
        virtual void OnPumpedNotify( CNotifier* notifier           ,
                                     const CEvent& eventid         ,
                                     CICloneable* eventdata = NULL );

        virtual bool OnSysConsoleCommand( const CString& path     ,
                                          const CString& command  ,
                                          const CStringList& args ,
                                          CStringList& resultdata );
                                          
        virtual void OnUpdate( const UInt64 tickCount               ,
                               const Float64 updateDeltaInMilliSecs );
                               
        virtual void LockData( void );
        
        virtual void UnlockData( void );                                                                                 
        
        private:
        friend class CGUCEFCOREModule;
        
        static void Deinstance( void );
        
        private:
        friend class CGUCEFAppSubSystem;
        
        void RegisterSubSystem( CGUCEFAppSubSystem* subSystem );
        
        void UnregisterSubSystem( CGUCEFAppSubSystem* subSystem );
        
        /**
         *  Ask all sub systems what their desired sub system update 
         *  interval is. This is combined to form the desired application
         *  update interval in milliseconds
         */
        void RefreshMinimalSubSysInterval( void );
        
        /**
         *  Ask all sub systems if they require periodic updates without
         *  explicit update requests. This determines the applications
         *  periodic update requirement
         */
        void RefreshPeriodicUpdateRequirement( void );
        
        void DoRequestSubSysUpdate( void );

        private:

        void Run( void );
        
        void SingleUpdate( const UInt64 tickCount               ,
                           const Float64 updateDeltaInMilliSecs );
                           
        CGUCEFApplication( void );
        CGUCEFApplication( const CGUCEFApplication& src );
        virtual ~CGUCEFApplication();
        CGUCEFApplication& operator=( const CGUCEFApplication& src ); /**< not implemented */
        
        private:
        typedef std::set< CGUCEFAppSubSystem* > TSubSystemList;
        
        TSubSystemList m_subSysList;
        bool m_requiresPeriodicUpdates;
        bool m_inNeedOfAnUpdate;
        Float64 m_minimalUpdateDelta;
        UInt64 m_appTickCount;
        Float64 m_timerFreq;
        CIGUCEFApplicationDriver* m_appDriver;
        
        bool _initialized;
        bool _active;
        CString _appdir;
        Float64 m_minimalCycleDeltaInMilliSecs;
        UInt32 m_cycleDelayInMilliSecs;
        
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
 