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
{GUCEF_TRACE;
}
        
/*-------------------------------------------------------------------------*/        
        
CFileURLHandler::CFileURLHandler( const CFileURLHandler& src )
        : CURLHandler( src )
{GUCEF_TRACE;    
}

/*-------------------------------------------------------------------------*/

CFileURLHandler::~CFileURLHandler()
{GUCEF_TRACE;
}

/*-------------------------------------------------------------------------*/

CFileURLHandler& 
CFileURLHandler::operator=( const CFileURLHandler& src )
{GUCEF_TRACE;
        if ( &src != this )
        {
        }
        return *this;
}

/*-------------------------------------------------------------------------*/

CICloneable*
CFileURLHandler::Clone( void ) const
{GUCEF_TRACE;
        return new CFileURLHandler( *this );
}

/*-------------------------------------------------------------------------*/

bool
CFileURLHandler::Activate( CURL& url )
{GUCEF_TRACE;

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
{GUCEF_TRACE;
        // no need to do anything, this handler works instantly instead of over time
}

/*-------------------------------------------------------------------------*/
                         
bool
CFileURLHandler::IsActive( const CURL& url ) const
{GUCEF_TRACE;
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
