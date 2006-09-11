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

#include "CDRNDataItem.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_NAMESPACE_BEGIN
COM_NAMESPACE_BEGIN

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CDRNDataItem::CDRNDataItem( UInt32 itemid )
        : m_itemid( itemid )
{TRACE;
}

/*-------------------------------------------------------------------------*/

CDRNDataItem::CDRNDataItem( void )
{TRACE;
}

/*-------------------------------------------------------------------------*/       
        
CDRNDataItem::CDRNDataItem( const CDRNDataItem& src )
        : m_data( src.m_data )
{TRACE;
}

/*-------------------------------------------------------------------------*/
        
CDRNDataItem::~CDRNDataItem()
{TRACE;
}

/*-------------------------------------------------------------------------*/
        
CDRNDataItem& 
CDRNDataItem::operator=( const CDRNDataItem& src )
{TRACE;
        if ( this != &src )
        {
                m_data = src.m_data;
        }
        return *this;
}

/*-------------------------------------------------------------------------*/

bool 
CDRNDataItem::operator==( const CDRNDataItem& other ) const
{TRACE;
        return m_data == other.m_data;
}

/*-------------------------------------------------------------------------*/
        
bool 
CDRNDataItem::operator!=( const CDRNDataItem& other ) const
{TRACE;
        return m_data != other.m_data;
}

/*-------------------------------------------------------------------------*/
        
UInt32 
CDRNDataItem::GetSize( void ) const
{TRACE;
        return m_data.GetDataSize();
}

/*-------------------------------------------------------------------------*/

UInt32 
CDRNDataItem::GetItemID( void ) const
{TRACE;
        return m_itemid;
}

/*-------------------------------------------------------------------------*/
        
Float32 
CDRNDataItem::AsFloat32( void ) const
{TRACE;
        if ( m_data.GetDataSize() >= 4 )
        {
                return (*static_cast<const Float32*>(m_data.GetBufferPtr()));
        }
        return 0.0f;
}

/*-------------------------------------------------------------------------*/
        
UInt32 
CDRNDataItem::AsUInt32( void ) const
{TRACE;
        if ( m_data.GetDataSize() >= 4 )
        {
                return (*static_cast<const UInt32*>(m_data.GetBufferPtr()));
        }
        return 0;
}

/*-------------------------------------------------------------------------*/
        
Int32 
CDRNDataItem::AsInt32( void ) const
{TRACE;
        if ( m_data.GetDataSize() >= 4 )
        {
                return (*static_cast<const Int32*>(m_data.GetBufferPtr()));
        }
        return 0;
}

/*-------------------------------------------------------------------------*/
        
UInt16 
CDRNDataItem::AsUInt16( void ) const
{TRACE;
        if ( m_data.GetDataSize() >= 2 )
        {
                return (*static_cast<const UInt16*>(m_data.GetBufferPtr()));
        }
        return 0;
}

/*-------------------------------------------------------------------------*/
        
Int16 
CDRNDataItem::AsInt16( void ) const
{TRACE;
        if ( m_data.GetDataSize() >= 2 )
        {
                return (*static_cast<const Int16*>(m_data.GetBufferPtr()));
        }
        return 0;
}

/*-------------------------------------------------------------------------*/
        
UInt8 
CDRNDataItem::AsUInt8( void ) const
{TRACE;
        if ( m_data.GetDataSize() >= 1 )
        {
                return (*static_cast<const UInt8*>(m_data.GetBufferPtr()));
        }
        return 0;
}

/*-------------------------------------------------------------------------*/
        
Int8 
CDRNDataItem::AsInt8( void ) const
{TRACE;
        if ( m_data.GetDataSize() >= 1 )
        {
                return (*static_cast<const Int8*>(m_data.GetBufferPtr()));
        }
        return 0;
}

/*-------------------------------------------------------------------------*/
        
bool 
CDRNDataItem::AsBool( void ) const
{TRACE;
        if ( m_data.GetDataSize() >= 1 )
        {
                return (*static_cast<const bool*>(m_data.GetBufferPtr()));
        }
        return false;
}

/*-------------------------------------------------------------------------*/
        
CORE::CString 
CDRNDataItem::AsString( void ) const
{TRACE;
        CORE::CString str;
        str.Set( static_cast<const char*>( m_data.GetBufferPtr() ) ,
                 m_data.GetDataSize()                              );
        return str;                 
}

/*-------------------------------------------------------------------------*/
        
CORE::CDynamicBuffer 
CDRNDataItem::AsBuffer( void ) const
{TRACE;
        return m_data;
}

/*-------------------------------------------------------------------------*/

void 
CDRNDataItem::CopyTo( CORE::CDynamicBuffer& dest )
{TRACE;
        m_data.CopyTo( dest );        
}

/*-------------------------------------------------------------------------*/

void 
CDRNDataItem::Get( void* data        , 
                   UInt32 buffersize ) const
{TRACE;
        m_data.CopyTo( 0, buffersize, data );        
}                   

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

COM_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/
