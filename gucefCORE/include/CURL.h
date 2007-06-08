/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
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

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

#ifndef GUCEF_CORE_CIURLEVENTS_H
#include "CIURLEvents.h"
#define GUCEF_CORE_CIURLEVENTS_H
#endif /* GUCEF_CORE_CIURLEVENTS_H ? */

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

/*-------------------------------------------------------------------------*/

/**
 *  class that allows you to retrieve data using URL's
 */
class GUCEFCORE_EXPORT_CPP CURL : public CObservingNotifier ,
                                  public CIURLEvents
{
        public:
        
        CURL( void );
        
        CURL( const CString& url );
        
        virtual ~CURL();
        
        CURL& operator=( const CURL& src );
        
        bool operator==( const CURL& other ) const;
        
        bool operator!=( const CURL& other ) const;
        
        bool SetURL( const CString& newurl );
        
        CString GetURL( void ) const;
        
        bool Activate( void );
        
        void Deactivate( void );
        
        /**
         *      Is a URL resource retrieval action in progress
         */
        bool IsActive( void ) const;
        
        void Refresh( void );

        protected:
        
        virtual void OnNotify( CNotifier* notifier           ,
                               const CEvent& eventid         ,
                               CICloneable* eventdata = NULL );

        private:
        
        CURLHandler* GetHandlerForURL( const CString& url ) const;
        
        private:        
       
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
