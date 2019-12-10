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

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#ifndef GUCEF_CORE_CCODECREGISTRY_H
#include "CCodecRegistry.h"
#define GUCEF_CORE_CCODECREGISTRY_H
#endif /* GUCEF_CORE_CCODECREGISTRY_H ? */

#ifndef GUCEF_IMAGE_CGUIIMAGECODEC_H
#include "gucefIMAGE_CGUIImageCodec.h"
#define GUCEF_IMAGE_CGUIIMAGECODEC_H
#endif /* GUCEF_IMAGE_CGUIIMAGECODEC_H ? */

#ifndef GUCEF_IMAGE_CIMAGECODECREGISTRY_H
#include "gucefIMAGE_CImageCodecRegistry.h"
#define GUCEF_IMAGE_CIMAGECODECREGISTRY_H
#endif /* GUCEF_IMAGE_CIMAGECODECREGISTRY_H ? */

#ifndef GUCEF_IMAGE_CIMAGECODECPLUGINMANAGER_H
#include "gucefIMAGE_CImageCodecPluginManager.h"
#define GUCEF_IMAGE_CIMAGECODECPLUGINMANAGER_H
#endif /* GUCEF_IMAGE_CIMAGECODECPLUGINMANAGER_H ? */

#include "gucefIMAGE_CImageGlobal.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace IMAGE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

CImageGlobal* CImageGlobal::g_instance = NULL;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CImageGlobal*
CImageGlobal::Instance()
{GUCEF_TRACE;

    if ( NULL == g_instance )
    {
        g_instance = new CImageGlobal();
        g_instance->Initialize();
    }
    return g_instance;
}

/*-------------------------------------------------------------------------*/

void
CImageGlobal::Deinstance( void )
{GUCEF_TRACE;

    delete g_instance;
    g_instance = NULL;
}

/*-------------------------------------------------------------------------*/

void
CImageGlobal::Initialize( void )
{GUCEF_TRACE;

    // Ensure the Core systems are initialzed first since this module depends on Core
    CORE::CCoreGlobal::Instance();

    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "Initializing gucefIMAGE Global systems" );

    CORE::CCodecRegistry& codecRegistry = CORE::CCoreGlobal::Instance()->GetCodecRegistry();
    CORE::CCodecRegistry::TCodecFamilyRegistryPtr registry; 
        
    if ( !codecRegistry.IsRegistered( "ImageCodec" ) )
    {
        registry = new CORE::CCodecRegistry::TCodecFamilyRegistry();
        codecRegistry.Register( "ImageCodec", registry );
    }
    else
    {
        registry = codecRegistry.Lookup( "ImageCodec" );
    }

    // Register the dummy codec for our native format
    registry->Register( "gui", new CGUIImageCodec() );

    m_imageCodecRegistry = new CImageCodecRegistry();
    m_imageCodecPluginManager = new CImageCodecPluginManager();

    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "gucefIMAGE Global systems initialized" );
}

/*-------------------------------------------------------------------------*/

CImageGlobal::CImageGlobal( void )
    : m_imageCodecPluginManager( NULL ) ,
      m_imageCodecRegistry( NULL )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CImageGlobal::~CImageGlobal()
{GUCEF_TRACE;

    GUCEF_SYSTEM_LOG( CORE::LOGLEVEL_NORMAL, "Shutting down gucefIMAGE global systems" );
        
    CORE::CCodecRegistry& codecRegistry = CORE::CCoreGlobal::Instance()->GetCodecRegistry();
    CORE::CCodecRegistry::TCodecFamilyRegistryPtr registry;
    
    if ( codecRegistry.IsRegistered( "ImageCodec" ) )
    {
        registry = codecRegistry.Lookup( "ImageCodec" );
        
        // unregister the dummy codec for our native format
        registry->Unregister( "gui" );
    }

    delete m_imageCodecPluginManager;
    m_imageCodecPluginManager = NULL;
    delete m_imageCodecRegistry;
    m_imageCodecRegistry = NULL;
}

/*-------------------------------------------------------------------------*/

CImageCodecPluginManager&
CImageGlobal::GetImageCodecPluginManager( void )
{GUCEF_TRACE;

    return *m_imageCodecPluginManager;
}

/*-------------------------------------------------------------------------*/

CImageCodecRegistry&
CImageGlobal::GetImageCodecRegistry( void )
{GUCEF_TRACE;

    return *m_imageCodecRegistry;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace IMAGE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
