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

#ifndef GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#include "CDynamicBufferAccess.h"
#define GUCEF_CORE_CDYNAMICBUFFERACCESS_H
#endif /* GUCEF_CORE_CDYNAMICBUFFERACCESS_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_CORE_CIOACCESS_H
#include "CIOAccess.h"
#define GUCEF_CORE_CIOACCESS_H
#endif /* GUCEF_CORE_CIOACCESS_H ? */

#ifndef GUCEF_IMAGE_CIMAGE_H
#include "gucefIMAGE_CImage.h"
#define GUCEF_IMAGE_CIMAGE_H
#endif /* GUCEF_IMAGE_CIMAGE_H ? */

#ifndef GUCEF_CORE_CFILEACCESS_H
#include "CFileAccess.h"
#define GUCEF_CORE_CFILEACCESS_H
#endif /* GUCEF_CORE_CFILEACCESS_H ? */

#include "gucefIMAGE_CPluginImageCodecItem.h"

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

CPluginImageCodecItem::CPluginImageCodecItem( TImageCodecPluginPtr pluginPtr ,
                                              const CString& codecTypeName   )
    : m_pluginPtr( pluginPtr )         ,
      m_codecTypeName( codecTypeName )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CPluginImageCodecItem::CPluginImageCodecItem( const CPluginImageCodecItem& src )
    : m_pluginPtr( src.m_pluginPtr )         ,
      m_codecTypeName( src.m_codecTypeName )
{

}

/*-------------------------------------------------------------------------*/

CPluginImageCodecItem::~CPluginImageCodecItem()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CPluginImageCodecItem::Encode( CORE::CIOAccess& source ,
                               CORE::CIOAccess& dest   )
{GUCEF_TRACE;

    return m_pluginPtr->Encode( source          ,
                                dest            ,
                                m_codecTypeName );
}

/*-------------------------------------------------------------------------*/

bool
CPluginImageCodecItem::Decode( CORE::CIOAccess& source ,
                               CORE::CIOAccess& dest   )
{GUCEF_TRACE;

    return m_pluginPtr->Decode( source          ,
                                dest            ,
                                m_codecTypeName );
}

/*-------------------------------------------------------------------------*/

bool
CPluginImageCodecItem::Encode( const CImage& inputImage       ,
                               CORE::CIOAccess& encodedOutput )
{GUCEF_TRACE;


    return m_pluginPtr->Encode( inputImage      ,
                                encodedOutput   ,
                                m_codecTypeName );
}

/*-------------------------------------------------------------------------*/

bool
CPluginImageCodecItem::Decode( CORE::CIOAccess& encodedInput ,
                               CImage& outputImage           )
{GUCEF_TRACE;


    return m_pluginPtr->Decode( encodedInput    ,
                                outputImage     ,
                                m_codecTypeName );
}

/*-------------------------------------------------------------------------*/

CORE::CString
CPluginImageCodecItem::GetFamilyName( void ) const
{GUCEF_TRACE;

    return CImageCodecPlugin::ImageCodecFamilyName;
}

/*-------------------------------------------------------------------------*/

CORE::CString
CPluginImageCodecItem::GetType( void ) const
{GUCEF_TRACE;

    return m_codecTypeName;
}

/*-------------------------------------------------------------------------*/

const CString&
CPluginImageCodecItem::GetClassTypeName( void ) const
{
    static CString classTypeName = "GUCEF::IMAGE::CPluginImageCodecItem";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

CORE::CICloneable*
CPluginImageCodecItem::Clone( void ) const
{
    return new CPluginImageCodecItem( *this );
}
/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace IMAGE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
