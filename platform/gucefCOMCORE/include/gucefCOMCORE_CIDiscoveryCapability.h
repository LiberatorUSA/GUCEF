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

#ifndef GUCEF_COMCORE_CIDISCOVERYCAPABILITY_H
#define GUCEF_COMCORE_CIDISCOVERYCAPABILITY_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <set>

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
class GUCEF_COMCORE_EXPORT_CPP CIDiscoveryCapability
{
    public:

    typedef std::set< CORE::CString > TStringSet;

    CIDiscoveryCapability( void );

    CIDiscoveryCapability( const CIDiscoveryCapability& src );

    virtual ~CIDiscoveryCapability();

    CIDiscoveryCapability& operator=( const CIDiscoveryCapability& src );
    
    /**
     *  Identifier of the application providing the relevant capabilities
     */
    virtual bool SetCapability( const CORE::CString& capability ) = 0;

    virtual bool GetCapability( CORE::CString& capability ) const = 0;

    /**
     *  Identifier of the communication method to be used by applications 
     *  interested in utilizing the provided capability
     *  This would determine whether the communication method utilized is compatible between client and server
     *  These identifiers are essentially the IDs of the possible contracts between clients and the server
     */
    virtual bool SetCapabilityAccessMethods( const TStringSet& capabilityAccessMethods ) = 0;

    virtual bool GetCapabilityAccessMethods( TStringSet& capabilityAccessMethods ) const = 0;

};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_COMCORE_CIDISCOVERYCAPABILITIES_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 12-02-2005 :
        - Initial implementation

---------------------------------------------------------------------------*/
