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

#ifndef GUCEF_IMAGE_CIMAGECODECPLUGIN_H
#include "gucefIMAGE_CImageCodecPlugin.h"
#define GUCEF_IMAGE_CIMAGECODECPLUGIN_H
#endif /* GUCEF_IMAGE_CIMAGECODECPLUGIN_H ? */

#ifndef GUCEF_IMAGE_CPLUGINIMAGECODECITEM_H
#include "gucefIMAGE_CPluginImageCodecItem.h"
#define GUCEF_IMAGE_CPLUGINIMAGECODECITEM_H
#endif /* GUCEF_IMAGE_CPLUGINIMAGECODECITEM_H ? */

#ifndef GUCEF_IMAGE_CIMAGECODECREGISTRY_H
#include "gucefIMAGE_CImageCodecRegistry.h"
#define GUCEF_IMAGE_CIMAGECODECREGISTRY_H
#endif /* GUCEF_IMAGE_CIMAGECODECREGISTRY_H ? */

#ifndef GUCEF_IMAGE_CIMAGEGLOBAL_H
#include "gucefIMAGE_CImageGlobal.h"
#define GUCEF_IMAGE_CIMAGEGLOBAL_H
#endif /* GUCEF_IMAGE_CIMAGEGLOBAL_H ? */

#include "gucefIMAGE_CImageCodecPluginManager.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace IMAGE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CImageCodecPluginManager::CImageCodecPluginManager( void )
    : CPluginManager()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CImageCodecPluginManager::~CImageCodecPluginManager()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CORE::TPluginPtr
CImageCodecPluginManager::RegisterPlugin( void* modulePtr                         ,
                                          CORE::TPluginMetaDataPtr pluginMetaData )
{GUCEF_TRACE;

    TImageCodecPluginPtr plugin( new CImageCodecPlugin() );
    if ( plugin->Link( modulePtr      ,
                       pluginMetaData ) )
    {
        CORE::CValueList pluginParams;
        pluginMetaData->GetParams( pluginParams );

        bool overrideExistingCodecs = false;
        CORE::CString overrideSettingStr = pluginParams.GetValueAlways( "overrideExistingCodecs", "false" );
        overrideExistingCodecs = CORE::StringToBool( overrideSettingStr );

        CImageCodecRegistry& registry = CImageGlobal::Instance()->GetImageCodecRegistry();
        CORE::CStdCodecPlugin::CCodecList codecList;
        plugin->GetCodecList( codecList );

        CORE::CStdCodecPlugin::CCodecList::iterator i = codecList.find( CImageCodecPlugin::ImageCodecFamilyName );
        CORE::CStdCodecPlugin::CCodecFamilyList imageCodecList = (*i).second;

        CORE::CStdCodecPlugin::CCodecFamilyList::iterator n = imageCodecList.begin();
        while ( n != imageCodecList.end() )
        {
            bool isAlreadyRegistered = registry.IsRegistered( *n );
            if ( overrideExistingCodecs || !isAlreadyRegistered )
            {
                if ( isAlreadyRegistered && overrideExistingCodecs )
                {
                    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "ImageCodec " + *n + " is already registered but overrideExistingCodecs=True, will unregister current codec" );
                    registry.Unregister( *n );
                }
                
                TPluginImageCodecItemPtr codecItem( new CPluginImageCodecItem( plugin, (*n) ) );
                if ( !registry.TryRegister( codecItem->GetType(), codecItem ) )
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "Unable to register image codec " + codecItem->GetType() );
                }
            }
            ++n;
        }

        return plugin;
    }
    return CORE::TPluginPtr();
}

/*-------------------------------------------------------------------------*/

void
CImageCodecPluginManager::UnregisterPlugin( CORE::TPluginPtr plugin )
{GUCEF_TRACE;

    TImageCodecPluginPtr imageCodecPlugin = plugin.StaticCast< CImageCodecPlugin >();

    CImageCodecRegistry& registry = CImageGlobal::Instance()->GetImageCodecRegistry();
    CORE::CStdCodecPlugin::CCodecList codecList;
    imageCodecPlugin->GetCodecList( codecList );

    CORE::CStdCodecPlugin::CCodecList::iterator i = codecList.find( CImageCodecPlugin::ImageCodecFamilyName );
    CORE::CStdCodecPlugin::CCodecFamilyList imageCodecList = (*i).second;

    CORE::CStdCodecPlugin::CCodecFamilyList::iterator n = imageCodecList.begin();
    while ( n != imageCodecList.end() )
    {
        registry.Unregister( (*n) );
        ++n;
    }
}

/*-------------------------------------------------------------------------*/

CString
CImageCodecPluginManager::GetPluginType( void ) const
{GUCEF_TRACE;

    return "GucefImageCodecPlugin";
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace IMAGE */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
