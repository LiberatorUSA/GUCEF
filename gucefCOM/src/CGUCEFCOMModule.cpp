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

#ifndef GUCEF_COM_CHTTPCLIENT_H
#include "CHTTPClient.h"
#define GUCEF_COM_CHTTPCLIENT_H
#endif /* GUCEF_COM_CHTTPCLIENT_H ? */

#ifndef GUCEF_CORE_CHTTPURLHANDLER_H
#include "CHTTPURLHandler.h"
#define GUCEF_CORE_CHTTPURLHANDLER_H
#endif /* GUCEF_CORE_CHTTPURLHANDLER_H ? */

#include "CGUCEFCOMModule.h"  /* definition of the class implemented here */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_NAMESPACE_BEGIN
COM_NAMESPACE_BEGIN

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

CGUCEFCOMModule::CGUCEFCOMModule( void )
{TRACE;
        /* dummy, should never be used */
}

/*-------------------------------------------------------------------------*/

CGUCEFCOMModule::CGUCEFCOMModule( const CGUCEFCOMModule& src )
{TRACE;
        /* dummy, should never be used */
}

/*-------------------------------------------------------------------------*/

CGUCEFCOMModule::~CGUCEFCOMModule()
{TRACE;
        /* dummy, should never be used */
}

/*-------------------------------------------------------------------------*/

CGUCEFCOMModule&
CGUCEFCOMModule::operator=( const CGUCEFCOMModule& src )
{TRACE;
        /* dummy, should never be used */
        return *this;
}
        
/*-------------------------------------------------------------------------*/        

bool 
CGUCEFCOMModule::Load( void )
{TRACE;
        /* simply instantiate our com manager when the module is loaded */
        //CCom::Instance();
        
        CHTTPClient::RegisterEvents();
        CHTTPURLHandler::Register();
        return true;
}

/*-------------------------------------------------------------------------*/
        
bool 
CGUCEFCOMModule::Unload( void )
{TRACE;
        //CCom::Deinstance();
        return true;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

COM_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/
