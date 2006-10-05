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

#ifndef GUCEF_CORE_CURLHANDLER_H
#define GUCEF_CORE_CURLHANDLER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CURL_H
#include "CURL.h"               /* control class for URL handling*/
#define GUCEF_CORE_CURL_H
#endif /* GUCEF_CORE_CURL_H ? */

#ifndef GUCEF_CORE_CICLONEABLE_H
#include "CICloneable.h"       /* base class for cloneable objects */
#define GUCEF_CORE_CICLONEABLE_H
#endif /* GUCEF_CORE_CICLONEABLE_H ? */

#ifndef GUCEF_CORE_CNOTIFIER_H
#include "CNotifier.h"
#define GUCEF_CORE_CNOTIFIER_H
#endif /* GUCEF_CORE_CNOTIFIER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CIURLDataHandler;

/*-------------------------------------------------------------------------*/

/**
 *      Base class for URL handler implementations.
 *      Handlers for HTTP for example should inherit and implement the
 *      interface from this and lower classes.
 */
class GUCEFCORE_EXPORT_CPP CURLHandler : public CNotifier   ,
                                         public CICloneable
{
        public:

        static const CString URLActivateEvent;
        static const CString URLDeactivateEvent;
        static const CString URLDataRecievedEvent;
        static const CString URLAllDataRecievedEvent;
        static const CString URLDataRetrievalErrorEvent;
        
        /**
         *      Doesnt do anything special atm.
         */        
        CURLHandler( void );
        
        /**
         *      Doesnt do anything special atm.
         */        
        CURLHandler( const CURLHandler& src );
        
        /**
         *      Doesnt do anything special atm.
         */
        virtual ~CURLHandler();
        
        /**
         *      Doesnt do anything special atm.
         */        
        CURLHandler& operator=( const CURLHandler& src );

        virtual bool Activate( CURL& url ) = 0;
        
        virtual void Deactivate( CURL& url ) = 0;
                         
        virtual bool IsActive( const CURL& url ) const = 0;       
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CURLHANDLER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 24-04-2005 :
        - Initial implementation
          
---------------------------------------------------------------------------*/