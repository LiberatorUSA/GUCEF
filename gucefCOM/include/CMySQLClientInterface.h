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

#ifndef CMYSQLCLIENTINTERFACE_H
#define CMYSQLCLIENTINTERFACE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CMYSQLCLIENT_H
#include "CMySQLClient.h"       /* MySQL client class for which this is the interface */
#define CMYSQLCLIENT_H
#endif /* CMYSQLCLIENT_H ? */

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

/**
 *      Interface class for the CMySQLClient class.
 *      Inherit from this interface class and implement whatever event handlers
 *      are appropriote for your situation.
 */
class GUCEFCOM_EXPORT_CPP CMySQLClientInterface
{
        protected:
        friend class CMySQLClient;
        
        typedef int TSocketError; /**< temp hack */
        
        /**
         *      A submitted query has been proccessed and the resulting query
         *      object is given. 
         */
        virtual void OnQueryComplete( CMySQLClient &client ,
                                      CDBQuery &query      ) {}
        
        /**
         *      Event handler called when we are about to connect to a server
         */
        virtual void OnConnecting( CMySQLClient &client ) {}

        /**
         *      Event handler called when we have successfully connected to
         *      a server socket.
         */
        virtual void OnConnected( CMySQLClient &client ) {}

        /**
         *      Event handler called when have lost the connection.
         *      The socket is now closed.
         */
        virtual void OnDisconnect( CMySQLClient &client ) {}

        /**
         *      Event handler called when a socket error occured.
         */
        virtual void OnSocketError( CMySQLClient &socket     ,
                                    TSocketError *errorcode  ) {}

};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

COM_NAMESPACE_END
GUCEF_NAMESPACE_END

/*--------------------------------------------------------------------------*/

#endif /* CMYSQLCLIENTINTERFACE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 11-06-2004 :
        - Designed and created this class.
        
-----------------------------------------------------------------------------*/
 