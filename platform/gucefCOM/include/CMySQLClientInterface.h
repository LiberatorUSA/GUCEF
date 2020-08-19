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

namespace GUCEF {
namespace COM {

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
class GUCEF_COM_EXPORT_CPP CMySQLClientInterface
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

}; /* namespace COM */
}; /* namespace GUCEF */

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
 