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

#ifndef GUCEF_CORE_CLOGMANAGER_H
#include "CLogManager.h"
#define GUCEF_CORE_CLOGMANAGER_H
#endif /* GUCEF_CORE_CLOGMANAGER_H ? */

#ifndef GUCEF_CORE_CCODECREGISTRY_H
#include "CCodecRegistry.h"
#define GUCEF_CORE_CCODECREGISTRY_H
#endif /* GUCEF_CORE_CCODECREGISTRY_H ? */

#include "CGUCEFVFSModule.h"  /* definition of the class implemented here */

#ifndef CVFS_H
#include "CVFS.h"
#define CVFS_H
#endif /* CVFS_H ? */

#ifndef CVFSURLHANDLER_H
#include "CVFSURLHandler.h"     /* URL handler for URL's with protocol "vfs" */
#define CVFSURLHANDLER_H
#endif /* CVFSURLHANDLER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_NAMESPACE_BEGIN
VFS_NAMESPACE_BEGIN

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

bool 
CGUCEFVFSModule::Load( void )
{GUCEF_TRACE;

        GUCEF_SYSTEM_LOG( 0, "gucefVFS Module loaded" );
        
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
        CVFS::Instance();
        
        /*
         *      register all codecs/handlers/notifiers
         */
        CVFSURLHandler::Register();
        return true;
}

/*-------------------------------------------------------------------------*/
        
bool 
CGUCEFVFSModule::Unload( void )
{GUCEF_TRACE;

        GUCEF_SYSTEM_LOG( 0, "gucefVFS Module unloading" );
        
        CVFSURLHandler::Unregister();
        
        CVFS::Deinstance();
        return true;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

VFS_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/
