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

#ifndef GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H
#include "CNotificationIDRegistry.h"
#define GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H
#endif /* GUCEF_CORE_CNOTIFICATIONIDREGISTRY_H ? */

#include "CURLHandler.h"

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

const CString CURLHandler::URLActivateEvent = "GUCEF::CORE::CURLHandler::URLActivateEvent";
const CString CURLHandler::URLDeactivateEvent = "GUCEF::CORE::CURLHandler::URLDeactivateEvent";
const CString CURLHandler::URLDataRecievedEvent = "GUCEF::CORE::CURLHandler::URLDataRecievedEvent";
const CString CURLHandler::URLAllDataRecievedEvent = "GUCEF::CORE::CURLHandler::URLAllDataRecievedEvent";
const CString CURLHandler::URLDataRetrievalErrorEvent = "GUCEF::CORE::CURLHandler::URLDataRetrievalErrorEvent";

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CURLHandler::CURLHandler( void )
        : CICloneable()
{TRACE;
}
        
/*-------------------------------------------------------------------------*/        
        
CURLHandler::CURLHandler( const CURLHandler& src )
        : CICloneable( src )
{TRACE;    
}

/*-------------------------------------------------------------------------*/

CURLHandler::~CURLHandler()
{TRACE;
}

/*-------------------------------------------------------------------------*/

CURLHandler& 
CURLHandler::operator=( const CURLHandler& src )
{TRACE;
        return *this;
}

/*-------------------------------------------------------------------------*/

void
CURLHandler::RegisterEvents( void )
{TRACE;

    CNotificationIDRegistry* registry( CNotificationIDRegistry::Instance() );
    registry->Register( URLActivateEvent, true );
    registry->Register( URLDeactivateEvent, true );
    registry->Register( URLDataRecievedEvent, true );
    registry->Register( URLAllDataRecievedEvent, true );
    registry->Register( URLDataRetrievalErrorEvent, true );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
