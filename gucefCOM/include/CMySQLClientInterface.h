/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
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
        public:
        typedef COMCORE::CSocket::TSocketError TSocketError;

        protected:
        friend class CMySQLClient;
        
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
 