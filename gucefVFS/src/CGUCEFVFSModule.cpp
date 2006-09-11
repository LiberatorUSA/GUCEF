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

#include "CGUCEFVFSModule.h"  /* definition of the class implemented here */

#ifndef CFPCODECREGISTRY_H
#include "CFPCodecRegistry.h"
#define CFPCODECREGISTRY_H
#endif /* CFPCODECREGISTRY_H ? */

#ifndef CFPCODECPLUGINMANAGER_H
#include "CFPCodecPluginManager.h"
#define CFPCODECPLUGINMANAGER_H 
#endif /* CFPCODECPLUGINMANAGER_H ? */

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

CGUCEFVFSModule::CGUCEFVFSModule( void )
{TRACE;
        DEBUGOUTPUT( "CGUCEFVFSModule::CGUCEFVFSModule( void )" );
        
        /* dummy, should never be used */
}

/*-------------------------------------------------------------------------*/

CGUCEFVFSModule::CGUCEFVFSModule( const CGUCEFVFSModule& src )
{TRACE;
        DEBUGOUTPUT( "CGUCEFVFSModule::CGUCEFVFSModule( const CGUCEFVFSModule& src )" );
        
        /* dummy, should never be used */
}

/*-------------------------------------------------------------------------*/

CGUCEFVFSModule::~CGUCEFVFSModule()
{TRACE;
        DEBUGOUTPUT( "CGUCEFVFSModule::~CGUCEFVFSModule()" );
        
        /* dummy, should never be used */
}

/*-------------------------------------------------------------------------*/

CGUCEFVFSModule&
CGUCEFVFSModule::operator=( const CGUCEFVFSModule& src )
{TRACE;
        DEBUGOUTPUT( "CGUCEFVFSModule::operator=( const CGUCEFVFSModule& src )" );
        
        /* dummy, should never be used */
        return *this;
}
        
/*-------------------------------------------------------------------------*/        

bool 
CGUCEFVFSModule::Load( void )
{TRACE;
        DEBUGOUTPUT( "CGUCEFVFSModule::Load( void )" );
        
        /*
         *      Instantiate all singletons
         */
        CFPCodecRegistry::Instance();
        CFPCodecPluginManager::Instance();
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
        CFPCodecPluginManager::Deinstance();
        CFPCodecRegistry::Deinstance();
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
