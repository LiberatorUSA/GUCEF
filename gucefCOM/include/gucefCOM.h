/*
 *  gucefCOM: GUCEF module providing communication 
 *  implementations for standardized protocols.
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

GUCEF_COM_EXPORT_C const char* GUCEF_COM_ModuleCompileTimeStamp( void );

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


 
