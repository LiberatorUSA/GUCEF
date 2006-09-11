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
 *      This is the main include header for the gucefCOM library.
 *      It is part of a framework called GUCEF.
 *      All classes in this library have to do with implementations for
 *      standardized communication facilities
 */

#ifndef GUCEFCOM_H
#define GUCEFCOM_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Library build configuration      
 */
#ifndef GUCEFCOM_CONFIG_H
#include "gucefCOM_config.h"
#define GUCEFCOM_CONFIG_H
#endif /* GUCEFCOM_CONFIG_H ? */

/*
 *      Library specific macros and generic macros
 */
#ifndef GUCEFCOM_MACROS_H
#include "gucefCOM_macros.h"
#define GUCEFCOM_MACROS_H
#endif /* GUCEFCOM_MACROS_H ? */

/*---------------------------------------------------------------------------/

        C++ includes

/---------------------------------------------------------------------------*/
#ifdef __cplusplus

#ifndef CBPTCPCLIENTSOCKET_H
#include "CBPTCPClientSocket.h"
#define CBPTCPCLIENTSOCKET_H
#endif /* CBPTCPCLIENTSOCKET_H ? */

#ifndef CBPTCPSERVERSOCKET_H
#include "CBPTCPServerSocket.h"
#define CBPTCPSERVERSOCKET_H
#endif /* CBPTCPSERVERSOCKET_H ? */

#ifndef CMYSQLCLIENT_H
#include "CMySQLClient.h"
#define CMYSQLCLIENT_H
#endif /* CMYSQLCLIENT_H ? */

#ifndef CMYSQLCLIENTINTERFACE_H
#include "CMySQLClientInterface.h"
#define CMYSQLCLIENTINTERFACE_H
#endif /* CMYSQLCLIENTINTERFACE_H ? */

#ifndef CHTTPCLIENT_H
#include "CHTTPClient.h"
#define CHTTPCLIENT_H
#endif /* CHTTPCLIENT_H ? */

#ifndef CHTTPCLIENTINTERFACE_H
#include "CHTTPClientInterface.h"
#define CHTTPCLIENTINTERFACE_H
#endif /* CHTTPCLIENTINTERFACE_H ? */

#ifndef CDBQUERY_H
#include "CDBQuery.h"
#define CDBQUERY_H
#endif /* CDBQUERY_H ? */

#ifndef CDBFIELD_H
#include "CDBField.h"
#define CDBFIELD_H
#endif /* CDBFIELD_H ? */

#ifndef CDBCOLUMN_H
#include "CDBColumn.h"
#define CDBCOLUMN_H
#endif /* CDBCOLUMN_H ? */

#ifndef CDRNCONTROLLER_H
#include "CDRNController.h"
#define CDRNCONTROLLER_H
#endif /* CDRNCONTROLLER_H ? */

#ifndef CDRNCONTROLLERINTERFACE_H
#include "CDRNControllerInterface.h"
#define CDRNCONTROLLERINTERFACE_H
#endif /* CDRNCONTROLLERINTERFACE_H ? */

#ifndef CDRNCLIENT_H
#include "CDRNClient.h"
#define CDRNCLIENT_H
#endif /* CDRNCLIENT_H ? */

#ifndef CDRNCLIENTINTERFACE_H
#include "CDRNClientInterface.h"
#define CDRNCLIENTINTERFACE_H
#endif /* CDRNCLIENTINTERFACE_H ? */

#endif /* __cplusplus ? */
/*--------------------------------------------------------------------------*/

#endif /* GUCEFCOM_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 04-09-2005 :
       - Added CBPTCPServerSocket
       - Added CBPTCPClientSocket
- 18-08-2005 :
       - Removed CHTTPDownload
       - Added CHTTPClient
- 24-07-2005 :
       - Added CDRNClient
       - Added CDRNClientInterface
       - Added CDRNController
       - Added CDRNControllerInterface
- 31-05-2005 :
       - Initial version

-----------------------------------------------------------------------------*/


 
