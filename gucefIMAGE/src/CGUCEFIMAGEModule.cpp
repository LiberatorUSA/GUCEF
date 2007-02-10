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
