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

#include "CNotificationIDRegistry.h"

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

CPatcherApplication::CPatcherApplication( void )
        : m_parser() ,
          m_appStartEventID( 0 )
{
    m_appStartEventID = CORE::CNotificationIDRegistry::Instance()->Lookup( CORE::CGUCEFApplication::AppInitEvent );
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
CPatcherApplication::OnUpdate( const UInt32 applicationTicks ,
                               const UInt32 deltaTicks       )
{
}

/*-------------------------------------------------------------------------*/

void
CPatcherApplication::OnNotify( CNotifier* notifier           ,
                               const UInt32 eventid          ,
                               CICloneable* eventdata = NULL )
{
    if ( eventid == m_appStartEventID )
    {
        CORE::CDataNode oldLocalList;
        CORE::CDataNode newLocalList;
        CORE::CDataNode localListDiff;
        
        m_parser.SubtractOldFromNewLocalList( oldLocalList  ,
                                              newLocalList  ,
                                              localListDiff );
                                              
        m_parser.ProcessPatchList( localListDiff )
    }
}

/*-------------------------------------------------------------------------*/