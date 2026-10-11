/*
 *  gucefWEB: GUCEF module providing WEB application functionality
 *  Copyright (C) 2002 - 2026.  Dinand Vanvelzen
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

#ifndef GUCEF_WEB_CHTTPCLIENTBACKENDSELECTION_H
#define GUCEF_WEB_CHTTPCLIENTBACKENDSELECTION_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <map>

#ifndef GUCEF_WEB_CIHTTPCLIENTBACKEND_H
#include "gucefWEB_CIHttpClientBackend.h"
#define GUCEF_WEB_CIHTTPCLIENTBACKEND_H
#endif /* GUCEF_WEB_CIHTTPCLIENTBACKEND_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace WEB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Picks the HTTP client backend to use for a URL scheme from an ordered list of preferred backends,
 *  creating each backend once via the factory of CWebGlobal and reusing it for all later requests so that
 *  a backend can keep connections and caches alive across requests.
 *  Thread-safe, shared between a CHTTPClient and the requests it has in progress.
 */
class GUCEF_WEB_PUBLIC_CPP CHttpClientBackendSelection
{
    public:

    typedef CString::StringVector TStringVector;

    /** the default order: a plugin backend when loaded, the OS backend on Windows, the native backend otherwise */
    static TStringVector GetDefaultPreferredBackends( void );

    CHttpClientBackendSelection( void );
    virtual ~CHttpClientBackendSelection();

    /** changing the preference also releases the backend objects that were created so far */
    void SetPreferredBackends( const TStringVector& backendNames );

    TStringVector GetPreferredBackends( void ) const;

    /**
     *  @return the first preferred backend that is registered and supports the scheme, or a NULL pointer
     */
    CIHttpClientBackendPtr GetBackendForScheme( const CString& scheme );

    private:

    CHttpClientBackendSelection( const CHttpClientBackendSelection& src );            /**< not supported */
    CHttpClientBackendSelection& operator=( const CHttpClientBackendSelection& src ); /**< not supported */

    typedef std::map< CString, CIHttpClientBackendPtr > TBackendMap;

    TStringVector m_preferredBackends;
    TBackendMap m_backends;
    MT::CMutex m_lock;
};

/*-------------------------------------------------------------------------*/

typedef CORE::CTSharedPtr< CHttpClientBackendSelection, MT::CMutex > CHttpClientBackendSelectionPtr;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace WEB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_WEB_CHTTPCLIENTBACKENDSELECTION_H ? */
