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

#include <assert.h>

#ifndef GUCEF_CORE_CIOACCESS_H
#include "CIOAccess.h"
#define GUCEF_CORE_CIOACCESS_H
#endif /* GUCEF_CORE_CIOACCESS_H ? */

#include "CStdCodecPluginItem.h"

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CStdCodecPluginItem::CStdCodecPluginItem( void* pluginData            ,
                                          TCodecPluginLink* codecLink )
    : CICodec()                  ,
      m_codecLink( codecLink )   ,
      m_pluginData( pluginData )
{GUCEF_TRACE;

    assert( m_codecLink != NULL );
}

/*-------------------------------------------------------------------------*/
    
CStdCodecPluginItem::CStdCodecPluginItem( const CStdCodecPluginItem& src )
    : CICodec()                        ,
      m_codecLink( src.m_codecLink )   ,
      m_pluginData( src.m_pluginData )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
    
CStdCodecPluginItem&
CStdCodecPluginItem::operator=( const CStdCodecPluginItem& src )
{GUCEF_TRACE;

    if ( &src != this )
    {
        CICodec::operator=( src );
        
        m_codecLink = src.m_codecLink;
        m_pluginData = src.m_pluginData;
    }
    return *this;
}
    
/*-------------------------------------------------------------------------*/

CStdCodecPluginItem::~CStdCodecPluginItem()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

TCodecPluginLink*
CStdCodecPluginItem::GetCodecPluginLink( void ) const
{GUCEF_TRACE;

    return m_codecLink;
}

/*-------------------------------------------------------------------------*/

void*
CStdCodecPluginItem::GetPluginData( void ) const
{GUCEF_TRACE;

    return m_pluginData;
}

/*-------------------------------------------------------------------------*/
    
bool
CStdCodecPluginItem::Encode( CIOAccess& source ,
                             CIOAccess& dest   )
{GUCEF_TRACE;

    return m_codecLink->encode( m_pluginData             , 
                                m_codecLink->codecData   , 
                                m_codecLink->codecFamily ,
                                m_codecLink->codecType   ,
                                source.CStyleAccess()    ,
                                dest.CStyleAccess()      ) != 0;
}

/*-------------------------------------------------------------------------*/

bool
CStdCodecPluginItem::Decode( CIOAccess& source ,
                             CIOAccess& dest   )
{GUCEF_TRACE;

    return m_codecLink->decode( m_pluginData             , 
                                m_codecLink->codecData   , 
                                m_codecLink->codecFamily ,
                                m_codecLink->codecType   ,
                                source.CStyleAccess()    ,
                                dest.CStyleAccess()      ) != 0;
}

/*-------------------------------------------------------------------------*/

CICloneable*
CStdCodecPluginItem::Clone( void ) const
{GUCEF_TRACE;

    return new CStdCodecPluginItem( *this );
}

/*-------------------------------------------------------------------------*/
    
CString
CStdCodecPluginItem::GetType( void ) const
{GUCEF_TRACE;

    return m_codecLink->codecType;
}
    
/*-------------------------------------------------------------------------*/
    
CString
CStdCodecPluginItem::GetFamilyName( void ) const
{GUCEF_TRACE;

    return m_codecLink->codecFamily;
}

/*-------------------------------------------------------------------------*/

const CString&
CStdCodecPluginItem::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CString classTypeName = "GUCEF::CORE::CStdCodecPluginItem";
    return classTypeName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/