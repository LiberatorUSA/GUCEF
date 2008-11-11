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
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEFCORE_H
#include "gucefCORE.h"
#define GUCEFCORE_H
#endif /* GUCEFCORE_H ? */

#ifndef GUCEFCOMCORE_H
#include "gucefCOMCORE.h"
#define GUCEFCOMCORE_H
#endif /* GUCEFCOMCORE_H ? */

#ifndef GUCEFCOM_H
#include "gucefCOM.h"
#define GUCEFCOM_H
#endif /* GUCEFCOM_H ? */

#include "TestCode_CHTTPClient.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/ 

using namespace GUCEF;
using namespace GUCEF::CORE;
using namespace GUCEF::COMCORE;
using namespace GUCEF::COM;

/*-------------------------------------------------------------------------*/

class CMyHTTPClient : public GUCEF::CORE::CObserver
{        
        public:
        
        CMyHTTPClient( void )
        {
            SubscribeTo( &m_client );
        }
        
        protected:
        
        virtual void OnConnected( CHTTPClient& client )
        {
                printf( "CMyHTTPClient::OnConnected()\n" );
        }
        
        virtual void OnDisconnected( CHTTPClient& client )
        {
                printf( "CMyHTTPClient::OnDisconnected()\n" );
        }
        
        virtual void OnHTTPError( CHTTPClient& client ,
                                  THTTPCODE errorcode )
        {
                printf( "CMyHTTPClient::OnHTTPError()\n" );
        }                                  
        
        virtual void OnError( CHTTPClient& client )
        {
                printf( "CMyHTTPClient::OnError()\n" );
        }
        
        virtual void OnRedirect( CHTTPClient& client                  ,
                                 const GUCEF::CORE::CString& location )
        {
                printf( "CMyHTTPClient::OnRedirect()\n" );
                printf( "CMyHTTPClient: location: %s\n", location.C_String() );
        }
        
        virtual void OnDataRecieved( CHTTPClient& client ,
                                     const char *data    ,
                                     CORE::UInt32 length       )
        {
                printf( "CMyHTTPClient::OnDataRead()\n" );
                printf( "CMyHTTPClient: recieved %d bytes\n", length );
        }
        
        virtual void OnDataSend( CHTTPClient& client ,
                                 const char *data    ,
                                 CORE::UInt32 length       )
        {
                printf( "CMyHTTPClient::OnDataSend()\n" );
                printf( "CMyHTTPClient: sending %d bytes\n", length );
        }
        
        virtual void OnContent( CHTTPClient& client      ,
                                const CORE::UInt32 size        ,
                                const bool resumeable    ,
                                const THTTPCODE httpcode )
        {
                printf( "CMyHTTPClient::OnContent()\n" );
                printf( "CMyHTTPClient: size: %d\n", size );        
                printf( "CMyHTTPClient: resumeable: %d\n", resumeable );
                printf( "CMyHTTPClient: httpcode: %d\n", httpcode );
        }                                   
                                 
        virtual void OnFinish( CHTTPClient &client )
        {
                printf( "CMyHTTPClient::OnFinish()\n" );
        }                                              

        virtual void OnNotify( CNotifier* notifier           ,
                               const CEvent& eventid         ,
                               CICloneable* eventdata = NULL )
        {
            
        }

        public:
        CHTTPClient m_client;
};

/*-------------------------------------------------------------------------*/

class CMyPumpClient : public CORE::CObserver
{
    public:
    
    CMyPumpClient( void )
    {
            SubscribeTo( GUCEF::CORE::CGUCEFApplication::Instance(), GUCEF::CORE::CGUCEFApplication::AppInitEvent );       
    }
    
    protected:
    
    virtual void OnUpdate( const CORE::UInt64 tickcount               ,
                           const CORE::Float64 updateDeltaInMilliSecs )
    {
    }

    virtual void OnNotify( CORE::CNotifier* notifier                 ,
                           const CORE::CEvent& eventid               ,
                           CORE::CICloneable* eventdata /* = NULL */ )
    {
        if ( eventid == GUCEF::CORE::CGUCEFApplication::AppInitEvent )
        {
                if ( client.m_client.Get( "www.google.com" ,
                                          80               ,
                                          ""               ) )
                {
                        printf( "Get succeeded\n" );
                }
                else                        
                {
                        printf( "Get failed\n" );
                }                                                  
        }                                                 
    }
        
        private:
        CMyHTTPClient client;   
                                              
};

/*-------------------------------------------------------------------------*/

void
SetupHTTPClientTest( void )
{
        CMyPumpClient* pclient = new CMyPumpClient();
}

/*-------------------------------------------------------------------------*/
