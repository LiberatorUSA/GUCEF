/*
 *  gucefIMAGE: GUCEF module providing image utilities
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

#ifndef GUCEF_CORE_CCODECREGISTRY_H
#include "CCodecRegistry.h"
#define GUCEF_CORE_CCODECREGISTRY_H
#endif /* GUCEF_CORE_CCODECREGISTRY_H ? */

#ifndef GUCEF_IMAGE_CGUIIMAGECODEC_H
#include "CGUIImageCodec.h"
#define GUCEF_IMAGE_CGUIIMAGECODEC_H
#endif /* GUCEF_IMAGE_CGUIIMAGECODEC_H ? */

#include "CGUCEFIMAGEModule.h"  /* definition of the class implemented here */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace IMAGE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

bool 
CGUCEFIMAGEModule::Load( void )
{TRACE;
        
        CORE::CCodecRegistry* codecRegistry = CORE::CCodecRegistry::Instance();
        CORE::CCodecRegistry::TCodecFamilyRegistryPtr registry; 
        
        if ( !codecRegistry->IsRegistered( "ImageCodec" ) )
        {
            registry = new CORE::CCodecRegistry::TCodecFamilyRegistry();
            codecRegistry->Register( "ImageCodec", registry );
        }
        else
        {
            registry = codecRegistry->Lookup( "ImageCodec" );
        }

        // Register the dummy codec for our native format
        registry->Register( "gui", new CGUIImageCodec() );
        
        return true;
}

/*-------------------------------------------------------------------------*/
        
bool 
CGUCEFIMAGEModule::Unload( void )
{TRACE;

    CORE::CCodecRegistry* codecRegistry = CORE::CCodecRegistry::Instance();
    CORE::CCodecRegistry::TCodecFamilyRegistryPtr registry;
    
    if ( codecRegistry->IsRegistered( "ImageCodec" ) )
    {
        registry = codecRegistry->Lookup( "ImageCodec" );
        
        // unregister the dummy codec for our native format
        registry->Unregister( "gui" );
    }
    return true;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace IMAGE */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
