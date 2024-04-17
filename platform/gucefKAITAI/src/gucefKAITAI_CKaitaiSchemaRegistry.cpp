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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#ifndef GUCEF_CORE_CURIRESOURCEACCESSORFACTORY_H
#include "gucefCORE_CUriResourceAccessorFactory.h"
#define GUCEF_CORE_CURIRESOURCEACCESSORFACTORY_H
#endif /* GUCEF_CORE_CURIRESOURCEACCESSORFACTORY_H ? */

#ifndef GUCEF_CORE_CDSTORECODECREGISTRY_H
#include "CDStoreCodecRegistry.h"
#define GUCEF_CORE_CDSTORECODECREGISTRY_H
#endif /* GUCEF_CORE_CDSTORECODECREGISTRY_H ? */

#include "gucefKAITAI_CKaitaiSchemaRegistry.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace KAITAI {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CKaitaiSchemaRegistry::CKaitaiSchemaRegistry( void )
    : CORE::CTONRegistry< CORE::CTONRegistry< CKaitaiSchema, MT::CMutex >, MT::CMutex >()
    , m_dataLock()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CKaitaiSchemaRegistry::~CKaitaiSchemaRegistry()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

const MT::CILockable* 
CKaitaiSchemaRegistry::AsLockable( void ) const
{GUCEF_TRACE;

    return CORE::CTONRegistry< CORE::CTONRegistry< CKaitaiSchema, MT::CMutex >, MT::CMutex >::AsLockable();
}

/*-------------------------------------------------------------------------*/

MT::TLockStatus
CKaitaiSchemaRegistry::Lock( UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return m_dataLock.Lock( lockWaitTimeoutInMs );
}

/*-------------------------------------------------------------------------*/

MT::TLockStatus
CKaitaiSchemaRegistry::Unlock( void ) const
{GUCEF_TRACE;

    return m_dataLock.Unlock();
}

/*-------------------------------------------------------------------------*/

CKaitaiSchemaRegistry::TSchemaPtr 
CKaitaiSchemaRegistry::TryGetSchema( const CORE::CString& schemaFamily , 
                                     const CORE::CString& schemaName   ) const
{GUCEF_TRACE;

    // First obtain the registery specific to the requested family
    TSchemaFamilyRegistryPtr schemaFamilyRegistry;
    if ( TryLookup( schemaFamily, schemaFamilyRegistry, false ) && schemaFamilyRegistry )
    {
        // Locate the specific schema by name, if available
        TSchemaPtr schema;
        if ( schemaFamilyRegistry->TryLookup( schemaName, schema, false ) && schema )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "KaitaiSchemaRegistry:TryGetCodec: Obtained schema " + schemaName + " from schema family " + schemaFamily );
            return schema;
        }
    }

    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "KaitaiSchemaRegistry:TryGetCodec: Failed to obtain schema " + schemaName + " from schema family " + schemaFamily );
    return TSchemaPtr();
}

/*-------------------------------------------------------------------------*/

bool 
CKaitaiSchemaRegistry::LoadSchema( const CORE::CUri& schemaResource  ,
                                   const CORE::CString& schemaFamily )
{GUCEF_TRACE;

    TSchemaPtr schema = CKaitaiSchema::CreateSharedObj();
    if ( !schema.IsNULL() )
    {
        if ( schema->LoadSchema( schemaResource ) )
        {
            TRegisteredObjPtr registry;
            if ( !TryLookup( schemaFamily, registry, true ) || registry.IsNULL() )
            {
                // Lazy init, adding the family as a registry
                registry = TSchemaFamilyRegistryPtr( GUCEF_NEW TSchemaFamilyRegistry() );
                Register( schemaFamily, registry );
            }

            if ( !registry.IsNULL() )
            {
                if ( registry->TryRegister( schema->id, schema ) )
                {
                    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "KaitaiSchemaRegistry:LoadSchema: Successfully registered new schema with id \"" + schema->id + 
                        "\" for family " + schemaFamily + " which was loaded from resource " + CORE::ToString( schemaResource ) );
                    return true;
                }
                else
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "KaitaiSchemaRegistry:LoadSchema: Failed to register new schema with id \"" + schema->id + 
                        "\" for family " + schemaFamily + " which was loaded from resource " + CORE::ToString( schemaResource ) );
                    return false;
                }
            }
        }
    }

    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "KaitaiSchemaRegistry:LoadSchema: Failed to obtain access to resource " + CORE::ToString( schemaResource ) + " for schema family " + schemaFamily );
    return false;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace KAITAI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/