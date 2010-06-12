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

#ifndef GUCEF_CORE_CFILEURLHANDLER_H
#define GUCEF_CORE_CFILEURLHANDLER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CURLHANDLER_H
#include "CURLHandler.h"               /* base class for URL handlers */
#define GUCEF_CORE_CURLHANDLER_H
#endif /* GUCEF_CORE_CURLHANDLER_H ? */

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

/**
 *      Base class for URL handler implementations.
 *      Handlers for HTTP for example should inherit and implement the
 *      interface from this and lower classes.
 */
class GUCEF_CORE_PUBLIC_CPP CFileURLHandler : public CURLHandler
{
        public:

        /**
         *      Doesnt do anything special atm.
         */
        CFileURLHandler( void );

        /**
         *      Doesnt do anything special atm.
         */
        CFileURLHandler( const CFileURLHandler& src );

        /**
         *      Doesnt do anything special atm.
         */
        virtual ~CFileURLHandler();

        /**
         *      Doesnt do anything special atm.
         */
        CFileURLHandler& operator=( const CFileURLHandler& src );

        virtual bool Activate( CURL& url );

        virtual void Deactivate( CURL& url );

        virtual bool IsActive( const CURL& url ) const;

        virtual CICloneable* Clone( void ) const;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CFILEURLHANDLER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 24-04-2005 :
        - Initial implementation

---------------------------------------------------------------------------*/
