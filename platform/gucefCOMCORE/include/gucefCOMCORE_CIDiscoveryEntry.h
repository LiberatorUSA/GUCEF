/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
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

#ifndef GUCEF_COMCORE_CIDISCOVERYENTRY_H
#define GUCEF_COMCORE_CIDISCOVERYENTRY_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <set>

#ifndef GUCEF_CORE_CITYPENAMED_H
#include "CITypeNamed.h"
#define GUCEF_CORE_CITYPENAMED_H
#endif /* GUCEF_CORE_CITYPENAMED_H ? */

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_CORE_CTSHAREDPTR_H
#include "CTSharedPtr.h"
#define GUCEF_CORE_CTSHAREDPTR_H
#endif /* GUCEF_CORE_CTSHAREDPTR_H ? */

#ifndef GUCEF_COMCORE_CHOSTADDRESS_H
#include "CHostAddress.h"
#define GUCEF_COMCORE_CHOSTADDRESS_H
#endif /* GUCEF_COMCORE_CHOSTADDRESS_H ? */

#ifndef GUCEF_COMCORE_CIDISCOVERYCAPABILITY_H
#include "gucefCOMCORE_CIDiscoveryCapability.h"
#define GUCEF_COMCORE_CIDISCOVERYCAPABILITY_H
#endif /* GUCEF_COMCORE_CIDISCOVERYCAPABILITY_H ? */

#ifndef GUCEF_COMCORE_MACROS_H
#include "gucefCOMCORE_macros.h"      /* often used gucefCOMCORE macros */
#define GUCEF_COMCORE_MACROS_H
#endif /* GUCEF_COMCORE_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Interface class which serves as the base class for all discovery clients
 */
class GUCEF_COMCORE_EXPORT_CPP CIDiscoveryEntry
{
    public:

    typedef std::set< CORE::CString > TStringSet;

    CIDiscoveryEntry( void );

    CIDiscoveryEntry( const CIDiscoveryEntry& src );

    virtual ~CIDiscoveryEntry();

    CIDiscoveryEntry& operator=( const CIDiscoveryEntry& src );

    /**
     *  Identifier of the application providing the relevant capabilities
     */
    virtual bool SetApplicationId( const CORE::CString& appId ) = 0;

    virtual bool GetApplicationId( CORE::CString& appId ) const = 0;

    /**
     *  Identifier of the application providing the relevant capabilities
     */
    virtual bool SetApplicationInstanceId( const CORE::CString& appId ) = 0;

    virtual bool GetApplicationInstanceId( CORE::CString& appId ) const = 0;

    /**
     *  Identifier of the host where the relevant capabilities are running
     */
    virtual bool SetHostInstanceId( const CORE::CString& appId ) = 0;

    virtual bool GetHostInstanceId( CORE::CString& appId ) const = 0;

    /**
     *  Identifier of the host where the relevant capabilities are running
     */
    virtual bool SetHost( const COMCORE::CHostAddress& host ) = 0;

    virtual bool GetHost( COMCORE::CHostAddress& host ) const = 0;

    /**
     *  Identifier of the application providing the relevant capabilities
     */
    virtual bool SetCapabilities( const CORE::CString& capability ) = 0;

    virtual bool GetCapabilities( CORE::CString& capability ) const = 0;

};

typedef CORE::CTSharedPtr< CIDiscoveryEntry > CIDiscoveryEntryPtr;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COMCORE_CIDISCOVERYENTRY_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 12-02-2005 :
        - Initial implementation

---------------------------------------------------------------------------*/
