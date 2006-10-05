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

#ifndef GUCEF_CORE_CURLHANDLERREGISTRY_H
#include "CURLHandlerRegistry.h"      /* gucefCORE registry for URL handlers */
#define GUCEF_CORE_CURLHANDLERREGISTRY_H
#endif /* GUCEF_CORE_CURLHANDLERREGISTRY_H ? */

#ifndef GUCEF_VFS_MACROS_H
#include "gucefVFS_macros.h"          /* often used gucefVFS macros */
#define GUCEF_VFS_MACROS_H
#endif /* GUCEF_VFS_MACROS_H ? */

#ifndef CVFS_H
#include "CVFS.h"
#define CVFS_H
#endif /* CVFS_H ? */

#include "CVFSURLHandler.h"

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

CVFSURLHandler::CVFSURLHandler( void )
        : m_vfs( CVFS::Instance() )           
{TRACE;
        
}
        
/*-------------------------------------------------------------------------*/        
        
CVFSURLHandler::CVFSURLHandler( const CVFSURLHandler& src )
        : m_vfs( CVFS::Instance() ) 
{TRACE;
  
}

/*-------------------------------------------------------------------------*/

CVFSURLHandler::~CVFSURLHandler()
{TRACE;
}

/*-------------------------------------------------------------------------*/

CVFSURLHandler&
CVFSURLHandler::operator=( const CVFSURLHandler& src )
{TRACE;
        return *this;
}

/*-------------------------------------------------------------------------*/

CORE::CICloneable* 
CVFSURLHandler::Clone( void ) const
{TRACE;
        return new CVFSURLHandler( *this );
}

/*-------------------------------------------------------------------------*/

void 
CVFSURLHandler::Register( void )
{TRACE;
        CORE::CURLHandlerRegistry* registry = CORE::CURLHandlerRegistry::Instance();
        if ( registry->IsRegistered( "vfs" ) )
        {
                registry->Register( "vfs", new CVFSURLHandler() );
        }
}

/*-------------------------------------------------------------------------*/
        
void 
CVFSURLHandler::Unregister( void )
{TRACE;
        CORE::CURLHandlerRegistry::Instance()->Unregister( "vfs" );
}

/*-------------------------------------------------------------------------*/

bool
CVFSURLHandler::Activate( CORE::CURL& url )
{TRACE;
        assert( &url );
        
        // Tell the data handlers we have begun our activation sequence
        NotifyObservers( URLActivateEvent );
        
        // Obtain the file
        UInt32 errorcode;
        CVFSHandle* handle = m_vfs->GetFile( url.GetURL().SubstrToSubstr( "://", false ), errorcode );        
        if ( handle )
        {                
                CORE::CIOAccess* access = handle->GetAccess();
                if ( NULL == access )
                {
                    NotifyObservers( URLDataRetrievalErrorEvent );
                    return false;
                }
                else
                {
                    // Pass the file data on to the data handlers
                    NotifyObservers( URLDataRecievedEvent, access );
                    
                    NotifyObservers( URLAllDataRecievedEvent );
                }
                
                return true;
        }
        else
        {
            NotifyObservers( URLDataRetrievalErrorEvent );
            return false;        
        } 
}

/*-------------------------------------------------------------------------*/
        
void
CVFSURLHandler::Deactivate( CORE::CURL& url )
{TRACE;
        /*
         *      This URL handler works instantly and need not be deactivated
         */
}

/*-------------------------------------------------------------------------*/
                         
bool 
CVFSURLHandler::IsActive( const CORE::CURL& url ) const
{TRACE;
        return false;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

VFS_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/
