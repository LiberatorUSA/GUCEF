/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2006.  All rights reserved.
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

#ifndef GUCEF_CORE_CHTTPURLHANDLER_H
#define GUCEF_CORE_CHTTPURLHANDLER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CURLHANDLER_H
#include "CURLHandler.h"               /* base class for URL handlers */
#define GUCEF_CORE_CURLHANDLER_H
#endif /* GUCEF_CORE_CURLHANDLER_H ? */

#ifndef GUCEF_COM_CHTTPCLIENT_H
#include "CHTTPClient.h"
#define GUCEF_COM_CHTTPCLIENT_H
#endif /* GUCEF_COM_CHTTPCLIENT_H ? */

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
 *      URL handler for "http://" URL's
 */
class CHTTPURLHandler : public CORE::CURLHandler
{
    public:
    
    /**
     *      Doesnt do anything special atm.
     */        
    CHTTPURLHandler( void );
    
    /**
     *      Doesnt do anything special atm.
     */        
    CHTTPURLHandler( const CHTTPURLHandler& src );
    
    /**
     *      Doesnt do anything special atm.
     */
    virtual ~CHTTPURLHandler();
    
    /**
     *      Doesnt do anything special atm.
     */        
    CHTTPURLHandler& operator=( const CHTTPURLHandler& src );

    virtual bool Activate( CORE::CURL& url );
    
    virtual void Deactivate( CORE::CURL& url );
                     
    virtual bool IsActive( const CORE::CURL& url ) const;
    
    virtual CORE::CICloneable* Clone( void ) const;
    
    static void Register( void );
    
    protected:
    
    virtual void OnNotify( CORE::CNotifier* notifier           ,
                           const CORE::CEvent& eventid         ,
                           CORE::CICloneable* eventdata = NULL );        
    
    private:
    
    CHTTPClient m_httpClient;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COM */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CHTTPURLHANDLER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 06-10-2006 :
        - Initial implementation
          
---------------------------------------------------------------------------*/