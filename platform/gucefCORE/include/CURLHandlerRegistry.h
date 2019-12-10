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

#ifndef GUCEF_CORE_CURLHANDLERREGISTRY_H
#define GUCEF_CORE_CURLHANDLERREGISTRY_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDE                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"     /* gucefMT mutex implementation */
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

#ifndef GUCEF_CORE_CTREGISTRY_H
#include "CTRegistry.h"         /* registry template  */
#define GUCEF_CORE_CTREGISTRY_H
#endif /* GUCEF_CORE_CTREGISTRY_H ? */

#ifndef GUCEF_CORE_CURLHANDLER_H
#include "CURLHandler.h"         /* base class for protocol URL handlers */
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

class GUCEF_CORE_PUBLIC_CPP CURLHandlerRegistry : public CTRegistry< CURLHandler >
{
    public:

    CURLHandlerRegistry( void );

    virtual ~CURLHandlerRegistry();

    protected:

    virtual void LockData( void ) const;

    virtual void UnlockData( void ) const;

    private:

    CURLHandlerRegistry( const CURLHandlerRegistry& src );

    CURLHandlerRegistry& operator=( const CURLHandlerRegistry& src );

    private:

    MT::CMutex m_datalock;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CURLHANDLERREGISTRY_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 24-04-2005 :
        - Initial implementation

---------------------------------------------------------------------------*/
