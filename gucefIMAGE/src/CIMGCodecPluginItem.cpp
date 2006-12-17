/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2004.  All rights reserved.
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
#include "CIMGCodecPlugin.h"
#include "CIMGCodecPluginItem.h"

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

CIMGCodecPluginItem::CIMGCodecPluginItem( const CIMGCodecPluginItem& src )
    : CIMGCodec( src )             ,
      m_plugin( src.m_plugin )     ,
      m_typeName( src.m_typeName )
{TRACE;

    assert( NULL != plugin );
    assert( typeName.Length() > 0 );
}

/*-------------------------------------------------------------------------*/

CIMGCodecPluginItem::CIMGCodecPluginItem( CIMGCodecPlugin* plugin       ,
                                          const CORE::CString& typeName )
    : CIMGCodec()            ,
      m_plugin( plugin )     ,
      m_typeName( typeName )
{TRACE;

    assert( NULL != plugin );
    assert( typeName.Length() > 0 );
}

/*-------------------------------------------------------------------------*/

CIMGCodecPluginItem::~CIMGCodecPluginItem()
{TRACE;

}

/*-------------------------------------------------------------------------*/

CORE::CICloneable*
CIMGCodecPluginItem::Clone( void ) const
{TRACE;

    return new CIMGCodecPluginItem( *this );
}

/*-------------------------------------------------------------------------*/

CORE::CString
CIMGCodecPluginItem::GetType( void ) const
{TRACE;

    return m_typeName;
}

/*-------------------------------------------------------------------------*/

bool
CIMGCodecPluginItem::Encode( const void* sourceData         ,
                             const UInt32 sourceBuffersSize ,
                             TDynamicBufferList& dest       ,
                             UInt32& destBuffersUsed        )
{TRACE;

    return m_plugin->Encode( sourceData        ,
                             sourceBuffersSize ,
                             dest              ,
                             destBuffersUsed   ,
                             m_typeName        );
}

/*-------------------------------------------------------------------------*/

bool
CIMGCodecPluginItem::Decode( const void* sourceData         ,
                             const UInt32 sourceBuffersSize ,
                             TDynamicBufferList& dest       ,
                             UInt32& destBuffersUsed        )
{TRACE;

    return m_plugin->Decode( sourceData        ,
                             sourceBuffersSize ,
                             dest              ,
                             destBuffersUsed   ,
                             m_typeName        );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace IMAGE */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
