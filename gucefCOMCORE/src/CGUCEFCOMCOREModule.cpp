/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
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

#ifndef GUCEF_COMCORE_CPING_H
#include "CPing.h"
#define GUCEF_COMCORE_CPING_H
#endif /* GUCEF_COMCORE_CPING_H ? */

#include "CGUCEFCOMCOREModule.h"  /* definition of the class implemented here */

#ifdef GUCEF_MSWIN_BUILD
  #ifndef DVWINSOCK_H
  #include "dvwinsock.h"
  #define DVWINSOCK_H
  #endif /* DVWINSOCK_H */
#endif

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
        #ifdef GUCEF_MSWIN_BUILD
        InitWinsock( 1 );
        #endif
        
        /* simply instantiate our com manager when the module is loaded */
        //CCom::Instance();
        
        CTCPConnection::RegisterEvents();
        CTCPServerSocket::RegisterEvents();
        CTCPClientSocket::RegisterEvents();
        CUDPSocket::RegisterEvents();
        CPing::RegisterEvents();
        
        return true;
}

/*-------------------------------------------------------------------------*/
        
bool 
CGUCEFCOMCOREModule::Unload( void )
{
        CCom::Deinstance();
        
        #ifdef GUCEF_MSWIN_BUILD
        ShutdownWinsock();
        #endif
                
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
