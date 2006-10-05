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

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */

#ifndef GUCEF_CORE_CFILEACCESS_H
#include "CFileAccess.h"
#define GUCEF_CORE_CFILEACCESS_H
#endif /* GUCEF_CORE_CFILEACCESS_H ? */

#include "CFileURLHandler.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CFileURLHandler::CFileURLHandler( void )
        : CURLHandler()
{TRACE;
}
        
/*-------------------------------------------------------------------------*/        
        
CFileURLHandler::CFileURLHandler( const CFileURLHandler& src )
        : CURLHandler( src )
{TRACE;    
}

/*-------------------------------------------------------------------------*/

CFileURLHandler::~CFileURLHandler()
{TRACE;
}

/*-------------------------------------------------------------------------*/

CFileURLHandler& 
CFileURLHandler::operator=( const CFileURLHandler& src )
{TRACE;
        if ( &src != this )
        {
        }
        return *this;
}

/*-------------------------------------------------------------------------*/

CICloneable*
CFileURLHandler::Clone( void ) const
{TRACE;
        return new CFileURLHandler( *this );
}

/*-------------------------------------------------------------------------*/

bool
CFileURLHandler::Activate( CURL& url )
{TRACE;

    // Tell the data handlers we have been activated
    NotifyObservers( URLActivateEvent );

    // Obtain access to the file given
    CString filePath( url.GetURL().SubstrToSubstr( "://", false ) );
    CFileAccess fileAccess( filePath, "rb" );
    if ( fileAccess.IsValid() )
    {
        NotifyObservers( URLDataRecievedEvent, &fileAccess );

        // Tell the data handlers we have finished recieving data
        NotifyObservers( URLAllDataRecievedEvent );        

        return true;
    }
    else
    {
        // Tell the data handlers an error occured
        NotifyObservers( URLDataRetrievalErrorEvent );
        return false;
    }
}

/*-------------------------------------------------------------------------*/
        
void
CFileURLHandler::Deactivate( CURL& url )
{TRACE;
        // no need to do anything, this handler works instantly instead of over time
}

/*-------------------------------------------------------------------------*/
                         
bool
CFileURLHandler::IsActive( const CURL& url ) const
{TRACE;
        return false;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
