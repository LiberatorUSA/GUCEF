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

#ifndef GUCEF_COM_CHTTPURLHANDLER_H
#define GUCEF_COM_CHTTPURLHANDLER_H

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
           
    CHTTPURLHandler( void );
    
    CHTTPURLHandler( const CHTTPURLHandler& src );
    
    virtual ~CHTTPURLHandler();
         
    CHTTPURLHandler& operator=( const CHTTPURLHandler& src );

    virtual bool Activate( CORE::CURL& url );
    
    virtual void Deactivate( CORE::CURL& url );
                     
    virtual bool IsActive( const CORE::CURL& url ) const;
    
    virtual CORE::CICloneable* Clone( void ) const;
    
    static void Register( void );
    
    virtual const CString& GetClassTypeName( void ) const;
    
    protected:
    
    virtual void OnNotify( CORE::CNotifier* notifier           ,
                           const CORE::CEvent& eventid         ,
                           CORE::CICloneable* eventdata = NULL );        
    
    private:
    
    CHTTPClient* m_httpClient;
    bool m_transferFinished;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COM */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COM_CHTTPURLHANDLER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 06-10-2006 :
        - Initial implementation
          
---------------------------------------------------------------------------*/