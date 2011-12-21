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

class CIURLDataHandler;

/*-------------------------------------------------------------------------*/

/**
 *      Base class for URL handler implementations.
 *      Handlers for HTTP for example should inherit and implement the
 *      interface from this and lower classes.
 *
 *      The descending handler implementation is expected to emit event
 *      notifications as declared in CIURLEvents
 */
class GUCEF_CORE_PUBLIC_CPP CURLHandler : public CObservingNotifier   ,
                                          public CICloneable          ,
                                          public CIURLEvents
{
    public:

    /**
     *      Doesn't do anything special atm.
     */
    CURLHandler( void );

    /**
     *      Doesn't do anything special atm.
     */
    CURLHandler( const CURLHandler& src );

    /**
     *      Doesn't do anything special atm.
     */
    virtual ~CURLHandler();

    /**
     *      Doesn't do anything special atm.
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
