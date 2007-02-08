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

CStdCodecPluginItem::CStdCodecPluginItem( void* pluginData            ,
                                          TCodecPluginLink* codecLink )
    : CICodec()                  ,
      m_codecLink( codecLink )   ,
      m_pluginData( pluginData )
{TRACE;

    assert( m_codecLink != NULL );
}

/*-------------------------------------------------------------------------*/
    
CStdCodecPluginItem::CStdCodecPluginItem( const CStdCodecPluginItem& src )
    : CICodec()                        ,
      m_codecLink( src.m_codecLink )   ,
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
        
        m_codecLink = src.m_codecLink;
        m_pluginData = src.m_pluginData;
    }
    return *this;
}
    
/*-------------------------------------------------------------------------*/

CStdCodecPluginItem::~CStdCodecPluginItem()
{TRACE;

}

/*-------------------------------------------------------------------------*/

TCodecPluginLink*
CStdCodecPluginItem::GetCodecPluginLink( void ) const
{TRACE;

    return m_codecLink;
}

/*-------------------------------------------------------------------------*/

void*
CStdCodecPluginItem::GetPluginData( void ) const
{TRACE;

    return m_pluginData;
}

/*-------------------------------------------------------------------------*/
    
bool
CStdCodecPluginItem::Encode( CIOAccess& source ,
                             CIOAccess& dest   )
{TRACE;

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
{TRACE;

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
{TRACE;

    return new CStdCodecPluginItem( *this );
}

/*-------------------------------------------------------------------------*/
    
CString
CStdCodecPluginItem::GetType( void ) const
{TRACE;

    return m_codecLink->codecType;
}
    
/*-------------------------------------------------------------------------*/
    
CString
CStdCodecPluginItem::GetFamilyName( void ) const
{TRACE;

    return m_codecLink->codecFamily;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/