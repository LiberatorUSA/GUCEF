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

#ifndef CCOM_H
#include "CCom.h"      /* header for the main communication manager */
#define CCOM_H
#endif /* CCOM_H ? */

#ifndef CTCPCLIENTSOCKET_H
#include "CTCPClientSocket.h"
#define CTCPCLIENTSOCKET_H
#endif /* CTCPCLIENTSOCKET_H ? */

#ifndef GUCEF_COMCORE_CUDPSOCKET_H
#include "CUDPSocket.h"
#define GUCEF_COMCORE_CUDPSOCKET_H
#endif /* GUCEF_COMCORE_CUDPSOCKET_H ? */

#ifndef GUCEF_COMCORE_CTCPSERVERSOCKET_H
#include "CTCPServerSocket.h"
#define GUCEF_COMCORE_CTCPSERVERSOCKET_H
#endif /* GUCEF_COMCORE_CTCPSERVERSOCKET_H ? */

#include "CGUCEFCOMCOREModule.h"  /* definition of the class implemented here */

#ifdef ACTIVATE_MEMORY_MANAGER
  #ifndef GUCEF_NEW_ON_H
  #include "gucef_new_on.h"   /* Use the GUCEF memory manager instead of the standard manager ? */
  #define GUCEF_NEW_ON_H
  #endif /* GUCEF_NEW_ON_H ? */
#endif /* ACTIVATE_MEMORY_MANAGER ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

CGUCEFCOMCOREModule::CGUCEFCOMCOREModule( void )
{
        /* dummy, should never be used */
}

/*-------------------------------------------------------------------------*/

CGUCEFCOMCOREModule::CGUCEFCOMCOREModule( const CGUCEFCOMCOREModule& src )
{
        /* dummy, should never be used */
}

/*-------------------------------------------------------------------------*/

CGUCEFCOMCOREModule::~CGUCEFCOMCOREModule()
{
        /* dummy, should never be used */
}

/*-------------------------------------------------------------------------*/

CGUCEFCOMCOREModule&
CGUCEFCOMCOREModule::operator=( const CGUCEFCOMCOREModule& src )
{
        /* dummy, should never be used */
        return *this;
}
        
/*-------------------------------------------------------------------------*/        

bool 
CGUCEFCOMCOREModule::Load( void )
{
        /* simply instantiate our com manager when the module is loaded */
        //CCom::Instance();
        
        CTCPServerSocket::RegisterEvents();
        CTCPClientSocket::RegisterEvents();
        CUDPSocket::RegisterEvents();
        
        return true;
}

/*-------------------------------------------------------------------------*/
        
bool 
CGUCEFCOMCOREModule::Unload( void )
{
        CCom::Deinstance();
        return true;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
