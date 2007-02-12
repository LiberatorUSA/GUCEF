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
{TRACE;
        DEBUGOUTPUT( "CGUCEFVFSModule::Load( void )" );

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
{TRACE;
        DEBUGOUTPUT( "CGUCEFVFSModule::Unload( void )" );
        
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
