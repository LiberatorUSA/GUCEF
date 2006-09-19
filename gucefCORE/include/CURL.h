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

#ifndef GUCEF_CORE_CURL_H
#define GUCEF_CORE_CURL_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <set>

#ifndef GUCEF_CORE_CSTRING_H
#include "CDVString.h"
#define GUCEF_CORE_CSTRING_H
#endif /* GUCEF_CORE_CSTRING_H ? */

#ifndef GUCEF_CORE_CTSHAREDPTR_H
#include "CTSharedPtr.h"
#define GUCEF_CORE_CTSHAREDPTR_H
#endif /* GUCEF_CORE_CTSHAREDPTR_H ? */

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

class CURLHandler;
class CIURLDataHandler;

/*-------------------------------------------------------------------------*/

/**
 *      Simple class that allows you to use URL's with ease.
 *      Use of this class is the recommended method for utilizing 
 *      URL resource retrieval.
 */
class EXPORT_CPP CURL
{
        public:
        
        /**
         *      initializes the object, no URL will be set and
         *      no resource will be available. Mainly to be used
         *      when you want to recycle the CURL object and you
         *      don't know the URL yet.
         */
        CURL( void );
        
        CURL( const CString& url );
        
        virtual ~CURL();
        
        CURL& operator=( const CURL& src );
        
        bool operator==( const CURL& other ) const;
        
        bool operator!=( const CURL& other ) const;
        
        void SetURL( const CString& newurl );
        
        CString GetURL( void ) const;
        
        bool Activate( void );
        
        void Deactivate( void );
        
        /**
         *      Is a URL resource retrieval action in progress
         */
        bool IsActive( void ) const;
        
        /**
         *      Reloads the resource regardless whether it is already available in a local
         *      cache
         */
        void Refresh( void );
        
        void RegisterDataHandler( CIURLDataHandler* dataHandler );
        
        void UnregisterDataHandler( CIURLDataHandler* dataHandler );
        
        private:
        friend class CURLHandler;
        
        typedef std::set< CIURLDataHandler* > TDataHandlerSet;
                
        private:
        
        CURLHandler* GetHandlerForURL( const CString& url ) const;
        
        private:        
        
        TDataHandlerSet m_dataHandlers;
        CString m_url;             /**< the URL string */
        CURLHandler* m_handler;    /**< URL handler for the specified URL protocol */ 
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CURL_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 24-04-2005 :
        - Initial implementation
          
---------------------------------------------------------------------------*/
