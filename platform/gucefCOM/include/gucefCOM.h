/*
 *  gucefCOM: GUCEF module providing communication implementations 
 *  for standardized protocols
 *
 *  Copyright (C) 1998 - 2020.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 *      This is the main include header for the gucefCOM library.
 *      It is part of a framework called GUCEF.
 *      All classes in this library have to do with implementations for
 *      standardized communication facilities
 */

#ifndef GUCEF_COM_H
#define GUCEF_COM_H

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

#ifndef GUCEF_COM_CPHUDPSOCKET_H
#include "CPHUDPSocket.h"
#define GUCEF_COM_CPHUDPSOCKET_H
#endif /* GUCEF_COM_CPHUDPSOCKET_H ? */

#ifndef GUCEF_COM_CHTTPSERVER_H
#include "gucefCOM_CHTTPServer.h"
#define GUCEF_COM_CHTTPSERVER_H
#endif /* GUCEF_COM_CHTTPSERVER_H ? */

#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Prevent C++ name mangeling
 */
#ifdef __cplusplus
   extern "C" {
#endif   /* __cplusplus */

/*-------------------------------------------------------------------------*/

GUCEF_COM_PUBLIC_C const char* GUCEF_COM_ModuleCompileTimeStamp( void );

/*-------------------------------------------------------------------------*/

#ifdef __cplusplus
   }
#endif /* __cplusplus */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COM_H ? */

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


 
