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

#ifndef CHTTPCLIENTINTERFACE_H
#define CHTTPCLIENTINTERFACE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef CHTTPCLIENT_H
#include "CHTTPClient.h"
#define CHTTPCLIENT_H
#endif /* CHTTPCLIENT_H ? */

#ifndef GUCEFCOM_MACROS_H
#include "gucefCOM_macros.h"
#define GUCEFCOM_MACROS_H
#endif /* GUCEFCOM_MACROS_H ? */

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

class EXPORT_CPP CHTTPClientInterface
{
        public:
        
        CHTTPClientInterface( void );
        
        CHTTPClientInterface( const CHTTPClientInterface& src );
        
        CHTTPClientInterface& operator=( const CHTTPClientInterface& src );
        
        virtual ~CHTTPClientInterface();

        protected:                
        friend class CHTTPClient;
        
        virtual void OnConnecting( CHTTPClient& client );
        
        virtual void OnConnected( CHTTPClient& client );
        
        virtual void OnDisconnected( CHTTPClient& client );
        
        virtual void OnHTTPError( CHTTPClient& client ,
                                  THTTPCODE errorcode );
        
        virtual void OnError( CHTTPClient& client );
        
        virtual void OnDataSend( CHTTPClient &client ,
                                 const char* data    ,
                                 UInt32 length       );        
        
        virtual void OnRedirect( CHTTPClient& client           ,
                                 const CORE::CString& location );
      
        virtual void OnContent( CHTTPClient& client      ,
                                const UInt32 size        ,
                                const bool resumeable    ,
                                const THTTPCODE httpcode );                                                               
        
        virtual void OnDataRecieved( CHTTPClient& client ,
                                     const char *data    ,
                                     UInt32 length       );                                    
                                 
        virtual void OnFinish( CHTTPClient &client );                                                                      
        
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

COM_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/

#endif /* CHTTPCLIENTINTERFACE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-09-2005 :
        - Initial version

-----------------------------------------------------------------------------*/