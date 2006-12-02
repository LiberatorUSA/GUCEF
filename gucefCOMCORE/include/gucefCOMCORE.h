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

/*
 *      This is the main include header for the gucefCOMCORE library.
 *      It is part of a framework called GUCEF.
 *      All classes in this library have to do with basic communication facilitys
 */

#ifndef GUCEFCOMCORE_H
#define GUCEFCOMCORE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Library build configuration      
 */
#ifndef GUCEFCOMCORE_CONFIG_H
#include "gucefCOMCORE_config.h"
#define GUCEFCOMCORE_CONFIG_H
#endif /* GUCEFCOMCORE_CONFIG_H ? */

/*
 *      Library specific macros and generic macros
 */
#ifndef GUCEFCOMCORE_MACROS_H
#include "gucefCOMCORE_macros.h"
#define GUCEFCOMCORE_MACROS_H
#endif /* GUCEFCOMCORE_MACROS_H ? */

/*
 *      Simple types
 */
#ifndef GUCEF_COMCORE_ETYPES_H
#include "gucefCOMCORE_ETypes.h"
#define GUCEF_COMCORE_ETYPES_H
#endif /* GUCEF_COMCORE_ETYPES_H ? */

#ifndef SOCKETERRORS_H
#include "socketerrors.h" 
#define SOCKETERRORS_H
#endif /* SOCKETERRORS_H ? */

/*---------------------------------------------------------------------------/

        C++ includes

/---------------------------------------------------------------------------*/
#ifdef __cplusplus

#ifndef CCOM_H
#include "CCom.h"
#define CCOM_H
#endif /* CCOM_H ? */

#ifndef CSOCKET_H
#include "CSocket.h"
#define CSOCKET_H
#endif /* CSOCKET_H ? */

#ifndef CTCPSERVERSOCKET_H
#include "CTCPServerSocket.h"
#define CTCPSERVERSOCKET_H
#endif /* CTCPSERVERSOCKET_H ? */

#ifndef CTCPSERVERSOCKETINTERFACE_H
#include "CTCPServerSocketInterface.h"
#define CTCPSERVERSOCKETINTERFACE_H
#endif /* CTCPSERVERSOCKETINTERFACE_H ? */

#ifndef CTCPSERVERCONNECTION_H
#include "CTCPServerConnection.h"
#define CTCPSERVERCONNECTION_H
#endif /* CTCPSERVERCONNECTION_H ? */

#ifndef CTCPCLIENTSOCKET_H
#include "CTCPClientSocket.h"
#define CTCPCLIENTSOCKET_H
#endif /* CTCPCLIENTSOCKET_H ? */ 

#ifndef CUDPSOCKET_H
#include "CUDPSocket.h"
#define CUDPSOCKET_H
#endif /* CUDPSOCKET_H ? */

#endif /* __cplusplus ? */
/*--------------------------------------------------------------------------*/

#endif /* GUCEFCOMCORE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 07-05-2005 :
       - Added CUDPSocket
       - Added CIUDPSocketEventHandler
- 12-03-2005 :
       - Made the initial version of the library header. The classes here
         where originally part of one large GUCEF codebase without segmentation.

-----------------------------------------------------------------------------*/


 
