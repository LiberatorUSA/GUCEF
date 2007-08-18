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

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#include "CDynamicBufferAccess.h"      /* definition of the class implemented here */

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

CDynamicBufferAccess::CDynamicBufferAccess( CDynamicBuffer* buffer                               ,
                                            const bool deleteBufferUponDestruction /* = false */ )
        : CIOAccess()                                                  ,
          m_buffer( buffer )                                           ,
          m_carat( 0 )                                                 ,
          m_deleteBufferUponDestruction( deleteBufferUponDestruction )
{GUCEF_TRACE;
        
    assert( m_buffer != NULL );                    
}

/*-------------------------------------------------------------------------*/

CDynamicBufferAccess::~CDynamicBufferAccess()
{GUCEF_TRACE;

    Close();

    if ( m_deleteBufferUponDestruction )
    {
        delete m_buffer;
        m_buffer = NULL;
    }
}

/*-------------------------------------------------------------------------*/

void 
CDynamicBufferAccess::Open( void )
{GUCEF_TRACE;

    m_carat = 0;
}

/*-------------------------------------------------------------------------*/

void 
CDynamicBufferAccess::Close( void )
{GUCEF_TRACE;

    m_carat = 0;
}

/*-------------------------------------------------------------------------*/

bool 
CDynamicBufferAccess::Opened( void ) const
{GUCEF_TRACE;

    return true;
}

/*-------------------------------------------------------------------------*/

CString 
CDynamicBufferAccess::ReadLine( void )
{GUCEF_TRACE;

    // makeme
    CString emptystr;
    return emptystr;
}

/*-------------------------------------------------------------------------*/

CString 
CDynamicBufferAccess::ReadString( void )
{GUCEF_TRACE;

    // makeme
    CString emptystr;
    return emptystr;
}

/*-------------------------------------------------------------------------*/

UInt32 
CDynamicBufferAccess::Read( void *dest      ,
                            UInt32 esize    ,
                            UInt32 elements )
{GUCEF_TRACE;

    UInt32 bytesCopied = m_buffer->CopyTo( m_carat        , 
                                           esize*elements , 
                                           dest           );
    m_carat += bytesCopied;
    return bytesCopied;
}

/*-------------------------------------------------------------------------*/

UInt32 
CDynamicBufferAccess::Tell( void ) const
{GUCEF_TRACE;

    return m_carat;
}

/*-------------------------------------------------------------------------*/

Int32 
CDynamicBufferAccess::Seek( Int32 offset ,
                            Int32 origin )
{GUCEF_TRACE;

    switch ( origin )
    {
        case SEEK_CUR :
        {
            Int32 carat = m_carat + offset;
            if ( ( carat < (Int32)m_buffer->GetDataSize() ) &&
                 ( carat >= 0                             )  )
            {
                m_carat = carat;
                return 0;
            }
            return -1;
        }
        case SEEK_SET :
        {
            if ( offset < (Int32)m_buffer->GetDataSize() )
            {
                m_carat = offset;
                return 0;
            }
            return -1;
        }        
        case SEEK_END :
        {
            m_carat = m_buffer->GetDataSize();
            return 0;
        }
        default :
        {
            return -1;
        }
    }
}

/*-------------------------------------------------------------------------*/

UInt32 
CDynamicBufferAccess::Setpos( UInt32 position )
{GUCEF_TRACE;

    if ( position < m_buffer->GetDataSize() )
    {
        m_carat = position;
    }
    else
    {
        m_carat = m_buffer->GetDataSize();
    }
    return m_carat;
}

/*-------------------------------------------------------------------------*/

char 
CDynamicBufferAccess::GetChar( void )
{GUCEF_TRACE;

    if ( m_carat < m_buffer->GetDataSize() )
    {       
        char retValue = *( static_cast< const char* >( m_buffer->GetConstBufferPtr() ) + m_carat );
        ++m_carat;
        return retValue;
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

bool 
CDynamicBufferAccess::Eof( void ) const
{GUCEF_TRACE;

    return m_carat >= m_buffer->GetDataSize();
}

/*-------------------------------------------------------------------------*/

bool 
CDynamicBufferAccess::IsReadOnly( void ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CDynamicBufferAccess::IsWriteOnly( void ) const
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/
        
bool 
CDynamicBufferAccess::IsReadAndWrite( void ) const
{GUCEF_TRACE;
    
    return true;
}

/*-------------------------------------------------------------------------*/
       
UInt32 
CDynamicBufferAccess::Write( const void* srcdata ,
                             UInt32 esize        ,
                             UInt32 elements     )
{GUCEF_TRACE;

    UInt32 bytesWritten = m_buffer->CopyFrom( m_carat        , 
                                              esize*elements ,
                                              srcdata        );
    m_carat += bytesWritten;
    return bytesWritten;
}

/*-------------------------------------------------------------------------*/

bool 
CDynamicBufferAccess::IsValid( void )
{GUCEF_TRACE;

    return m_buffer != NULL;
}

/*-------------------------------------------------------------------------*/

void
CDynamicBufferAccess::Flush( void )
{GUCEF_TRACE;

    // no-op
}

/*-------------------------------------------------------------------------*/

Int32 
CDynamicBufferAccess::GetSize( void ) const
{GUCEF_TRACE;

    return m_buffer->GetDataSize();          
}

/*-------------------------------------------------------------------------*/

CICloneable* 
CDynamicBufferAccess::Clone( void ) const
{GUCEF_TRACE;
        
    // Cannot be cloned
    return NULL;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
