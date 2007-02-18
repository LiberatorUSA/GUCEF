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
{TRACE;
        
    assert( m_buffer != NULL );                    
}

/*-------------------------------------------------------------------------*/

CDynamicBufferAccess::~CDynamicBufferAccess()
{TRACE;

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
{TRACE;

    m_carat = 0;
}

/*-------------------------------------------------------------------------*/

void 
CDynamicBufferAccess::Close( void )
{TRACE;

    m_carat = 0;
}

/*-------------------------------------------------------------------------*/

bool 
CDynamicBufferAccess::Opened( void ) const
{TRACE;

    return true;
}

/*-------------------------------------------------------------------------*/

CString 
CDynamicBufferAccess::ReadLine( void )
{TRACE;

    // makeme
    CString emptystr;
    return emptystr;
}

/*-------------------------------------------------------------------------*/

CString 
CDynamicBufferAccess::ReadString( void )
{TRACE;

    // makeme
    CString emptystr;
    return emptystr;
}

/*-------------------------------------------------------------------------*/

UInt32 
CDynamicBufferAccess::Read( void *dest      ,
                            UInt32 esize    ,
                            UInt32 elements )
{TRACE;

    UInt32 bytesCopied = m_buffer->CopyTo( m_carat        , 
                                           esize*elements , 
                                           dest           );
    m_carat += bytesCopied;
    return bytesCopied;
}

/*-------------------------------------------------------------------------*/

UInt32 
CDynamicBufferAccess::Tell( void ) const
{TRACE;

    return m_carat;
}

/*-------------------------------------------------------------------------*/

Int32 
CDynamicBufferAccess::Seek( Int32 offset ,
                            Int32 origin )
{TRACE;

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
{TRACE;

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
{TRACE;

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
{TRACE;

    return m_carat >= m_buffer->GetDataSize();
}

/*-------------------------------------------------------------------------*/

bool 
CDynamicBufferAccess::IsReadOnly( void ) const
{TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CDynamicBufferAccess::IsWriteOnly( void ) const
{TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/
        
bool 
CDynamicBufferAccess::IsReadAndWrite( void ) const
{TRACE;
    
    return true;
}

/*-------------------------------------------------------------------------*/
       
UInt32 
CDynamicBufferAccess::Write( const void* srcdata ,
                             UInt32 esize        ,
                             UInt32 elements     )
{TRACE;

    UInt32 bytesWritten = m_buffer->CopyFrom( m_carat        , 
                                              esize*elements ,
                                              srcdata        );
    m_carat += bytesWritten;
    return bytesWritten;
}

/*-------------------------------------------------------------------------*/

/**
 *      Is the access to the resource a valid one or
 *      has something gone wrong ?
 */
bool 
CDynamicBufferAccess::IsValid( void )
{TRACE;

    return m_buffer != NULL;
}

/*-------------------------------------------------------------------------*/

Int32 
CDynamicBufferAccess::GetSize( void ) const
{TRACE;

    return m_buffer->GetDataSize();          
}

/*-------------------------------------------------------------------------*/

CICloneable* 
CDynamicBufferAccess::Clone( void ) const
{TRACE;
        
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
