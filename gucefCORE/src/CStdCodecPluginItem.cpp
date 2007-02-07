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

CStdCodecPluginItem::CStdCodecPluginItem( void* pluginData                   ,
                                          const TCodecPluginLink& pluginLink )
    : CICodec()                              ,
      encode( pluginLink.encode )            ,
      decode( pluginLink.decode )            ,
      m_type( pluginLink.codecType )         ,
      m_familyName( pluginLink.codecFamily ) ,
      m_pluginData( pluginData )
{TRACE;

    assert( m_pluginData != NULL );
    assert( encode != NULL );
    assert( decode != NULL );
    assert( m_type.Length() > 0 );
    assert( m_familyName.Length() > 0 );
}

/*-------------------------------------------------------------------------*/
    
CStdCodecPluginItem::CStdCodecPluginItem( const CStdCodecPluginItem& src )
    : CICodec()                        ,
      encode( src.encode )             ,
      decode( src.decode )             ,
      m_type( src.m_type )             ,
      m_familyName( src.m_familyName ) ,
      m_pluginData( src.m_pluginData )
{TRACE;

}

/*-------------------------------------------------------------------------*/
    
CStdCodecPluginItem&
CStdCodecPluginItem::operator=( const CStdCodecPluginItem& src )
{TRACE;

    if ( &src != this )
    {
        CICodec::operator=( src );
        
        encode = src.encode;
        decode = src.decode;
        m_type = src.m_type;
        m_familyName = src.m_familyName;
    }
    return *this;
}
    
/*-------------------------------------------------------------------------*/

CStdCodecPluginItem::~CStdCodecPluginItem()
{TRACE;

}

/*-------------------------------------------------------------------------*/
    
bool
CStdCodecPluginItem::Encode( const void* sourceData         ,
                             const UInt32 sourceBuffersSize ,
                             CIOAccess& dest                )
{TRACE;

    return encode( m_pluginData, sourceData, sourceBuffersSize, dest.CStyleAccess() ) != 0;
}

/*-------------------------------------------------------------------------*/

bool
CStdCodecPluginItem::Decode( const void* sourceData         ,
                             const UInt32 sourceBuffersSize ,
                             CIOAccess& dest                )
{TRACE;

    return decode( m_pluginData, sourceData, sourceBuffersSize, dest.CStyleAccess() ) != 0;
}

/*-------------------------------------------------------------------------*/

CICloneable*
CStdCodecPluginItem::Clone( void ) const
{TRACE;

    return new CStdCodecPluginItem( *this );
}

/*-------------------------------------------------------------------------*/
    
CString
CStdCodecPluginItem::GetType( void ) const
{TRACE;

    return m_type;
}
    
/*-------------------------------------------------------------------------*/
    
CString
CStdCodecPluginItem::GetFamilyName( void ) const
{TRACE;

    return m_familyName;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/