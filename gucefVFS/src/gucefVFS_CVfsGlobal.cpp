/*
 *  gucefVFS: GUCEF module implementing a Virtual File System
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

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#ifndef GUCEF_CORE_CCODECREGISTRY_H
#include "CCodecRegistry.h"
#define GUCEF_CORE_CCODECREGISTRY_H
#endif /* GUCEF_CORE_CCODECREGISTRY_H ? */

#ifndef GUCEF_VFS_CVFS_H
#include "gucefVFS_CVFS.h"
#define GUCEF_VFS_CVFS_H
#endif /* GUCEF_VFS_CVFS_H ? */

#ifndef GUCEF_VFS_CVFSURLHANDLER_H
#include "gucefVFS_CVFSURLHandler.h"     /* URL handler for URL's with protocol "vfs" */
#define GUCEF_VFS_CVFSURLHANDLER_H
#endif /* GUCEF_VFS_CVFSURLHANDLER_H ? */

#include "gucefVFS_CVfsGlobal.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace VFS {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

CVfsGlobal* CVfsGlobal::g_instance = NULL;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CVfsGlobal*
CVfsGlobal::Instance()
{GUCEF_TRACE;

    if ( NULL == g_instance )
    {
        g_instance = new CVfsGlobal();
        g_instance->Initialize();
    }
    return g_instance;
}

/*-------------------------------------------------------------------------*/

void
CVfsGlobal::Deinstance( void )
{GUCEF_TRACE;

    delete g_instance;
    g_instance = NULL;
}

/*-------------------------------------------------------------------------*/

void
CVfsGlobal::Initialize( void )
{GUCEF_TRACE;

    // Ensure the Core systems are initialzed first since this module depends on Core
    CORE::CCoreGlobal::Instance();

    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "Initializing gucefVFS Global systems" );

    try
    {
        CORE::CCodecRegistry::Instance()->Register( "VFSPackCodec", new CORE::CCodecRegistry::TCodecFamilyRegistry() );
    }
    catch ( CORE::CCodecRegistry::EAlreadyRegistered& )
    {
    }
    
    /*
     *      Instantiate all singletons
     */
    m_vfs = new CVFS();
    
    /*
     *      register all codecs/handlers/notifiers
     */
    CVFSURLHandler::Register();

    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "gucefVFS Global systems initialized" );
}

/*-------------------------------------------------------------------------*/

CVfsGlobal::CVfsGlobal( void )
    : m_vfs( NULL )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CVfsGlobal::~CVfsGlobal()
{GUCEF_TRACE;

    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "Shutting down gucefVFS global systems" );

    /* 
     *  Unregister handlers
     */
    CVFSURLHandler::Unregister();

    /*
     *      cleanup all singletons
     *      Take care to deinstance them in the correct order !!!
     */

    delete m_vfs;
    m_vfs = NULL;
}

/*-------------------------------------------------------------------------*/

CVFS&
CVfsGlobal::GetVfs( void )
{GUCEF_TRACE;

    return *m_vfs;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
