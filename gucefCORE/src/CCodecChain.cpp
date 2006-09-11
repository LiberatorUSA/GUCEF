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

#ifndef GUCEF_CORE_CICODEC_H
#include "CICodec.h"
#define GUCEF_CORE_CICODEC_H
#endif /* GUCEF_CORE_CICODEC_H ? */

#ifndef GUCEF_CORE_CSTRINGLIST_H
#include "CStringList.h"
#define GUCEF_CORE_CSTRINGLIST_H
#endif /* GUCEF_CORE_CSTRINGLIST_H ? */

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */

#include "CCodecChain.h"

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

CCodecChain::CCodecChain( void )
        : m_codecList() ,
          m_bufferA()   ,
          m_bufferB()
{TRACE;

}

/*-------------------------------------------------------------------------*/

CCodecChain::CCodecChain( const CCodecChain& src )
        : m_codecList()              ,
          m_bufferA( src.m_bufferA ) ,
          m_bufferB( src.m_bufferB )
{TRACE;
        TCodecList::const_iterator i = src.m_codecList.begin();
        while ( i != src.m_codecList.end() )
        {
                AppendCodec( *(*i) );
                ++i;
        }
}

/*-------------------------------------------------------------------------*/

CCodecChain::~CCodecChain()
{TRACE;
        Clear();
}

/*-------------------------------------------------------------------------*/

CCodecChain&
CCodecChain::operator=( const CCodecChain& src )
{TRACE;
        if ( &src != this )
        {
                Clear();                
        }
        return *this;
}
/*-------------------------------------------------------------------------*/


void
CCodecChain::Clear( void )
{TRACE;
        TCodecList::iterator i = m_codecList.begin();
        while ( i != m_codecList.end() )
        {
                // get rid of the codec instance
                delete (*i);
                ++i;
        }
        m_codecList.clear();
        m_bufferA.Clear( false );
        m_bufferB.Clear( false );
}

/*-------------------------------------------------------------------------*/
        
bool
CCodecChain::AddCodec( const CICodec& codec   ,
                       const UInt32 codecSlot )
{TRACE;
        // Set our iterator at the requested slot
        TCodecList::iterator i = m_codecList.begin();
        i += codecSlot;
        
        // Try to insert the new codec
        if ( i != m_codecList.end() )
        {
                m_codecList.insert( i, static_cast< CICodec* >( codec.Clone() ) );
                return true;
        }
        return false;
}

/*-------------------------------------------------------------------------*/

void
CCodecChain::AppendCodec( const CICodec& codec )
{TRACE;
        m_codecList.push_back( static_cast< CICodec* >( codec.Clone() ) );
}

/*-------------------------------------------------------------------------*/

bool
CCodecChain::RemoveCodec( const UInt32 codecSlot )
{TRACE;
        // Set our iterator at the requested slot
        TCodecList::iterator i = m_codecList.begin();
        i += codecSlot;

        // Try to delete the codec
        if ( i != m_codecList.end() )
        {
                delete (*i);                
                m_codecList.erase( i );
                return true;
        }
        return false;
}

/*-------------------------------------------------------------------------*/
        
void
CCodecChain::RemoveCodec( const CString& typeName )
{TRACE;
        TCodecList::iterator i = m_codecList.begin();
        while ( i != m_codecList.end() )
        {
                if ( (*i)->GetType() == typeName )
                {
                        // get rid of the codec instance
                        delete (*i);
                        m_codecList.erase( i );
                        
                        // reset to the start of the list
                        i = m_codecList.begin();
                }
                else
                {
                        ++i;
                }
        }
}

/*-------------------------------------------------------------------------*/
        
bool
CCodecChain::HasCodec( const CString& typeName ) const
{TRACE;

        TCodecList::const_iterator i = m_codecList.begin();
        while ( i != m_codecList.end() )
        {
                if ( (*i)->GetType() == typeName )
                {
                        return true;
                }
                ++i;
        }
        return false;
}
        
/*-------------------------------------------------------------------------*/

void
CCodecChain::GetList( CStringList& list ) const
{TRACE;
        list.Clear();
        
        TCodecList::const_iterator i = m_codecList.begin();
        while ( i != m_codecList.end() )
        {
                list.Append( (*i)->GetType() );
                ++i;
        }
}
        
/*-------------------------------------------------------------------------*/

void
CCodecChain::SetCodecOrder( const CStringList& codecOrder )
{TRACE;
        assert( 0 );
}

/*-------------------------------------------------------------------------*/
        
bool
CCodecChain::Encode( const void* sourceBuffer      ,
                     const UInt32 sourceBufferSize ,
                     CDynamicBuffer& dest          )
{TRACE;
        if ( 0 != m_codecList.size() )
        {
                // initialize our swapping vars
                CDynamicBuffer* destBuffer = &m_bufferA;
                const void* srcBuffer( sourceBuffer );
                UInt32 srcBufferSize( sourceBufferSize );
                                
                TCodecList::const_iterator i = m_codecList.begin();
                do
                {
                        // Perform a logical clear of the buffer, resetting the data carat
                        destBuffer->Clear( true );
                        
                        // Try to encode the given data
                        if ( !(*i)->Encode( srcBuffer     ,
                                            srcBufferSize ,
                                            *destBuffer   ) )
                        {
                                // One of the encoding steps failed
                                destBuffer->CopyTo( dest );
                                return false;
                        }
                        
                        // Swap the buffers
                        if ( destBuffer == &m_bufferA )
                        {
                                destBuffer = &m_bufferB;
                                srcBuffer = m_bufferA.GetBufferPtr();
                                srcBufferSize = m_bufferA.GetDataSize();
                        }
                        else
                        {
                                destBuffer = &m_bufferA;
                                srcBuffer = m_bufferB.GetBufferPtr();
                                srcBufferSize = m_bufferB.GetDataSize();                                
                        }
                        
                        ++i;
                }
                while ( i != m_codecList.end() );
                
                // Finished encoding, copy the result into the destination buffer
                destBuffer->CopyTo( dest );
                return true;
        }
        else
        {
                // No encoding required, simply copy the data
                dest.CopyFrom( sourceBufferSize, sourceBuffer );
                return true;
        }        
}                     

/*-------------------------------------------------------------------------*/

bool
CCodecChain::Decode( const void* sourceBuffer      ,
                     const UInt32 sourceBufferSize ,
                     CDynamicBuffer& dest          )
{TRACE;
        if ( 0 != m_codecList.size() )
        {
                // initialize our swapping vars
                CDynamicBuffer* destBuffer = &m_bufferA;
                const void* srcBuffer( sourceBuffer );
                UInt32 srcBufferSize( sourceBufferSize );
                                
                Int32 i( (Int32)m_codecList.size()-1 );
                do
                {
                        // Perform a logical clear of the buffer, resetting the data carat
                        destBuffer->Clear( true );
                        
                        // Try to decode the given data
                        if ( !( (  m_codecList[ i ] )->Decode( srcBuffer     ,
                                                               srcBufferSize ,
                                                               *destBuffer   ) ) )
                        {
                                // One of the encoding steps failed
                                destBuffer->CopyTo( dest );
                                return false;
                        }
                        
                        // Swap the buffers
                        if ( destBuffer == &m_bufferA )
                        {
                                destBuffer = &m_bufferB;
                                srcBuffer = m_bufferA.GetBufferPtr();
                                srcBufferSize = m_bufferA.GetDataSize();
                        }
                        else
                        {
                                destBuffer = &m_bufferA;
                                srcBuffer = m_bufferB.GetBufferPtr();
                                srcBufferSize = m_bufferB.GetDataSize();                                
                        }
                        
                        --i;
                }
                while ( i >= 0 );
                
                // Finished encoding, copy the result into the destination buffer
                destBuffer->CopyTo( dest );
                return true;
        }
        else
        {
                // No encoding required, simply copy the data
                dest.CopyFrom( sourceBufferSize, sourceBuffer );
                return true;
        }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/