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

#ifndef CDRNBROADCASTER_H
#include "CDRNBroadcaster.h"
#define CDRNBROADCASTER_H
#endif /* CDRNBROADCASTER_H ? */

#include "CDRNLocalDataItem.h"

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

CDRNLocalDataItem::CDRNLocalDataItem( CDRNBroadcaster* broadcaster ,
                                      UInt32 itemid                )
        : CDRNDataItem( itemid )       ,
          m_broadcaster( broadcaster ) ,
          m_initialized( false )
{TRACE;
        
}

/*-------------------------------------------------------------------------*/

CDRNLocalDataItem::CDRNLocalDataItem( const CDRNLocalDataItem& src )
        : CDRNDataItem( src )
{TRACE;
}

/*-------------------------------------------------------------------------*/

CDRNLocalDataItem::~CDRNLocalDataItem()
{TRACE;
}

/*-------------------------------------------------------------------------*/

CDRNLocalDataItem&
CDRNLocalDataItem::operator=( const CDRNLocalDataItem& src )
{TRACE;
        return *this;
}

/*-------------------------------------------------------------------------*/

CDRNLocalDataItem& 
CDRNLocalDataItem::operator=( const Float32& floatvalue )
{TRACE;
        m_data.CopyFrom( 4, &floatvalue );
        m_initialized = true;
        m_broadcaster->Broadcast( this );     
        return *this;
}

/*-------------------------------------------------------------------------*/
        
CDRNLocalDataItem& 
CDRNLocalDataItem::operator=( const Int8& intvalue )
{TRACE;
        m_data.CopyFrom( 1, &intvalue );
        m_initialized = true;
        m_broadcaster->Broadcast( this );                
        return *this;
}

/*-------------------------------------------------------------------------*/
        
CDRNLocalDataItem& 
CDRNLocalDataItem::operator=( const UInt8& intvalue )
{TRACE;
        m_data.CopyFrom( 1, &intvalue );
        m_initialized = true;
        m_broadcaster->Broadcast( this );                
        return *this;
}

/*-------------------------------------------------------------------------*/
        
CDRNLocalDataItem& 
CDRNLocalDataItem::operator=( const Int16& intvalue )
{TRACE;
        m_data.CopyFrom( 2, &intvalue );
        m_initialized = true;
        m_broadcaster->Broadcast( this );                
        return *this;
}

/*-------------------------------------------------------------------------*/
        
CDRNLocalDataItem& 
CDRNLocalDataItem::operator=( const UInt16& intvalue )
{TRACE;
        m_data.CopyFrom( 2, &intvalue );
        m_initialized = true;
        m_broadcaster->Broadcast( this );                
        return *this;
}

/*-------------------------------------------------------------------------*/
        
CDRNLocalDataItem& 
CDRNLocalDataItem::operator=( const Int32& intvalue )
{TRACE;
        m_data.CopyFrom( 4, &intvalue );
        m_initialized = true;
        m_broadcaster->Broadcast( this );                
        return *this;
}

/*-------------------------------------------------------------------------*/
        
CDRNLocalDataItem& 
CDRNLocalDataItem::operator=( const UInt32& intvalue )
{TRACE;
        m_data.CopyFrom( 4, &intvalue );
        m_initialized = true;
        m_broadcaster->Broadcast( this );                
        return *this;
}

/*-------------------------------------------------------------------------*/
        
CDRNLocalDataItem& 
CDRNLocalDataItem::operator=( const GUCEF::CORE::CString& stringvalue )
{TRACE;
        m_data.CopyFrom( stringvalue.Length(), stringvalue.C_String() );
        m_initialized = true;
        m_broadcaster->Broadcast( this );               
        return *this;
}

/*-------------------------------------------------------------------------*/
        
CDRNLocalDataItem& 
CDRNLocalDataItem::operator=( const bool& boolvalue )
{TRACE;
        Int8 boolval;
        boolvalue ? boolval = 1 : boolval = 0;
        m_data.CopyFrom( 1, &boolval );
        m_initialized = true;
        m_broadcaster->Broadcast( this );                
        return *this;
}

/*-------------------------------------------------------------------------*/

void 
CDRNLocalDataItem::Set( const void* data , 
                        UInt32 datasize  )
{TRACE;
        m_data.CopyFrom( datasize, data );
        m_initialized = true;
        m_broadcaster->Broadcast( this );         
}

/*-------------------------------------------------------------------------*/

bool 
CDRNLocalDataItem::IsInitialized( void ) const
{TRACE;
        return m_initialized;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

COM_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/
                            