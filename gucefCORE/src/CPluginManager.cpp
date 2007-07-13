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
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CPLUGINCONTROL_H
#include "CPluginControl.h"     /* global plugin manager control center */
#define GUCEF_CORE_CPLUGINCONTROL_H
#endif /* GUCEF_CORE_CPLUGINCONTROL_H ? */

#ifndef GUCEF_CORE_DVFILEUTILS_H
#include "dvfileutils.h"                /* file handling utils */
#define GUCEF_CORE_DVFILEUTILS_H
#endif /* GUCEF_CORE_DVFILEUTILS_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"           /* C++ string utils */ 
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#include "CPluginManager.h"    /* definition of the class implemented here */

#ifndef GUCEF_CORE_CLOGMANAGER_H
#include "CLogManager.h"
#define GUCEF_CORE_CLOGMANAGER_H
#endif /* GUCEF_CORE_CLOGMANAGER_H ? */

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */  

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

const CEvent CPluginManager::PluginLoadedEvent = "GUCEF::CORE::CPluginManager::PluginLoadedEvent";
const CEvent CPluginManager::PluginUnloadedEvent = "GUCEF::CORE::CPluginManager::PluginUnloadedEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *      Generic interface for plugin managers
 */
CPluginManager::CPluginManager( void )
    : CObservingNotifier()
{GUCEF_TRACE;

    RegisterEvents();
    
    CPluginControl::Instance()->Register( this );       
}

/*-------------------------------------------------------------------------*/

CPluginManager::CPluginManager( const CPluginManager& src )
    : CObservingNotifier( src )
{GUCEF_TRACE;

    RegisterEvents();
    
    CPluginControl::Instance()->Register( this );
}

/*-------------------------------------------------------------------------*/
        
CPluginManager::~CPluginManager()
{GUCEF_TRACE;
    
    CPluginControl::Instance()->Unregister( this );
}

/*-------------------------------------------------------------------------*/

CString
CPluginManager::GetPluginDir( void ) const
{GUCEF_TRACE;
    
    return CPluginControl::Instance()->GetPluginDir();
}

/*-------------------------------------------------------------------------*/

void 
CPluginManager::OnSetPluginDir( const CString& path )
{GUCEF_TRACE;
      
}

/*-------------------------------------------------------------------------*/

void
CPluginManager::RegisterEvents( void )
{GUCEF_TRACE;
        
    PluginLoadedEvent.Initialize();
    PluginUnloadedEvent.Initialize();
}

/*-------------------------------------------------------------------------*/

void
CPluginManager::OnNotify( CNotifier* notifier                 ,
                          const UInt32 eventid                ,
                          CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;

    /* dummy to avoid mandatory implementation in the descending class */
}
/*-------------------------------------------------------------------------*/

CPluginManager::TPluginPtr
CPluginManager::LoadPlugin( const CString& pluginPath )
{GUCEF_TRACE;

    // return a NULL pointer
    return TPluginPtr();
}

/*-------------------------------------------------------------------------*/

void 
CPluginManager::LoadAll( void )
{GUCEF_TRACE;

    CString file;
    CString filepath; 
    CString path = GetPluginDir();
    
    struct SDI_Data* did = DI_First_Dir_Entry( path.C_String() );
    if ( did )
    {
        do 
        {                
            if ( DI_Is_It_A_File( did ) == 1 )
            {
                GUCEF_SYSTEM_LOG( 0, "Attempt to load " + CString( DI_Name( did ) ) + " as a plugin" );
                
                filepath = path; 
                file = DI_Name( did );
                AppendToPath( filepath, file );
                LoadPlugin( filepath );
            }
        }
        while ( DI_Next_Dir_Entry( did ) != 0 );
        
        DI_Cleanup( did );
    }
}
        
/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
