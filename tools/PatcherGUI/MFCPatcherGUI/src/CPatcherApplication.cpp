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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CGUCEFAPPLICATION_H
#include "CGUCEFApplication.h"
#define GUCEF_CORE_CGUCEFAPPLICATION_H
#endif /* GUCEF_CORE_CGUCEFAPPLICATION_H ? */

#ifndef GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H
#include "CNotificationIDRegistry.h"
#define GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H
#endif /* GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H ? */

#include "CPluginControl.h"
#include "CDStoreCodecRegistry.h"
#include "dvcppstringutils.h"
#include "CVFS.h"

#include "CPatcherApplication.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

CPatcherApplication* CPatcherApplication::m_instance = NULL;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

using namespace GUCEF;

/*-------------------------------------------------------------------------*/

GUCEF_IMPLEMENT_MSGEXCEPTION( CPatcherApplication, ENoEventHandler );

/*-------------------------------------------------------------------------*/

CPatcherApplication::CPatcherApplication( void )
        : GUCEF::CORE::CGUCEFAppSubSystem( true ) ,
          m_parser()                              ,
          m_appStartEventID()                     ,
          m_eventHandler( NULL )                  ,
          m_vfs( VFS::CVFS::Instance() )
{
    m_appStartEventID = CORE::CNotificationIDRegistry::Instance()->Lookup( CORE::CGUCEFApplication::AppInitEvent );     
    m_vfs->AddRoot( CORE::RelativePath( "$CURWORKDIR$" ), false );
    m_vfs->AddRoot( CORE::RelativePath( "$CURWORKDIR$\\TEMP" ), true );
}

/*-------------------------------------------------------------------------*/

CPatcherApplication::~CPatcherApplication()
{

}

/*-------------------------------------------------------------------------*/

CPatcherApplication*
CPatcherApplication::Instance( void )
{
        if ( !m_instance )
        {
                m_instance = new CPatcherApplication();
        }
        return m_instance;
}

/*-------------------------------------------------------------------------*/

void
CPatcherApplication::Deinstance( void )
{
        delete m_instance;
        m_instance = NULL;
}

/*-------------------------------------------------------------------------*/

void
CPatcherApplication::OnUpdate( const CORE::UInt32 applicationTicks ,
                               const CORE::UInt32 deltaTicks       )
{
}

/*-------------------------------------------------------------------------*/

void
CPatcherApplication::OnNotify( CORE::CNotifier* notifier                 ,
                               const CORE::CEvent& eventid               ,
                               CORE::CICloneable* eventdata /* = NULL */ )
{
    if ( eventid == m_appStartEventID )
    {
        if ( m_eventHandlers.size() == 0 )
        {
            GUCEF_EMSGTHROW( ENoEventHandler, "CPatcherApplication::OnNotify(): Application has started but no event handler has been set" );
            
            // Just in case the exception doesn't work:
            CORE::CGUCEFApplication::Instance()->Stop();
        }
        
        CORE::CPluginControl::Instance()->SetPluginDir( CORE::RelativePath( "$CURWORKDIR$\\plugins" ) );
        CORE::CPluginControl::Instance()->LoadAll();
        
        CORE::UInt32 errorCode;
        VFS::CVFSHandle* oldListFile = m_vfs->GetFile( "oldLocalList.xml" ,
                                                       errorCode         );
        
        CORE::CDataNode oldLocalList;
        if ( oldListFile )
        {
            GUCEF::CORE::CDStoreCodecRegistry::TDStoreCodecPtr codecPtr = GUCEF::CORE::CDStoreCodecRegistry::Instance()->Lookup( "xml" );
            codecPtr->BuildDataTree( &oldLocalList, oldListFile->GetAccess() );
        }
        
        
        CORE::CDataNode newLocalList;
        CORE::CDataNode localListDiff;
        
        m_parser.SubtractOldFromNewLocalList( oldLocalList  ,
                                              newLocalList  ,
                                              localListDiff );
                                              
        m_parser.ProcessPatchList( localListDiff, m_eventHandlers );
    }
}

/*-------------------------------------------------------------------------*/

void
CPatcherApplication::SetEventHandlers( const TEventHandlerList& eventHandlers )
{
    m_eventHandlers = eventHandlers;
}

/*-------------------------------------------------------------------------*/