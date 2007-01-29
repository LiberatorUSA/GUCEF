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

class CMyHTTPClient : public CHTTPClientInterface
{        
        public:
        
        CMyHTTPClient( void )
        {
                m_client.SetInterface( this );        
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

        public:
        CHTTPClient m_client;
};

/*-------------------------------------------------------------------------*/

class CMyPumpClient : public CGUCEFAppSubSystem
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
                           const CORE::UInt32 eventid                ,
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
