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

//CCodecChain::CCodecChain( void )
//        : m_codecList() ,
//          m_bufferA()   ,
//          m_bufferB()   ,
//          m_bufferC()
//{TRACE;
//
//}
//
///*-------------------------------------------------------------------------*/
//
//CCodecChain::CCodecChain( const CCodecChain& src )
//        : m_codecList()              ,
//          m_bufferA( src.m_bufferA ) ,
//          m_bufferB( src.m_bufferB ) ,
//          m_bufferC( src.m_bufferC )
//{TRACE;
//        TCodecList::const_iterator i = src.m_codecList.begin();
//        while ( i != src.m_codecList.end() )
//        {
//                AppendCodec( *(*i) );
//                ++i;
//        }
//}
//
///*-------------------------------------------------------------------------*/
//
//CCodecChain::~CCodecChain()
//{TRACE;
//        Clear();
//}
//
///*-------------------------------------------------------------------------*/
//
//CCodecChain&
//CCodecChain::operator=( const CCodecChain& src )
//{TRACE;
//        if ( &src != this )
//        {
//                Clear();                
//        }
//        return *this;
//}
///*-------------------------------------------------------------------------*/
//
//
//void
//CCodecChain::Clear( void )
//{TRACE;
//        TCodecList::iterator i = m_codecList.begin();
//        while ( i != m_codecList.end() )
//        {
//                // get rid of the codec instance
//                delete (*i);
//                ++i;
//        }
//        m_codecList.clear();
//        m_bufferA.clear();
//        m_bufferB.clear();
//        m_bufferC.clear();
//}
//
///*-------------------------------------------------------------------------*/
//        
//bool
//CCodecChain::AddCodec( const CICodec& codec   ,
//                       const UInt32 codecSlot )
//{TRACE;
//        // Set our iterator at the requested slot
//        TCodecList::iterator i = m_codecList.begin();
//        i += codecSlot;
//        
//        // Try to insert the new codec
//        if ( i != m_codecList.end() )
//        {
//                m_codecList.insert( i, static_cast< CICodec* >( codec.Clone() ) );
//                return true;
//        }
//        return false;
//}
//
///*-------------------------------------------------------------------------*/
//
//void
//CCodecChain::AppendCodec( const CICodec& codec )
//{TRACE;
//        m_codecList.push_back( static_cast< CICodec* >( codec.Clone() ) );
//}
//
///*-------------------------------------------------------------------------*/
//
//bool
//CCodecChain::RemoveCodec( const UInt32 codecSlot )
//{TRACE;
//        // Set our iterator at the requested slot
//        TCodecList::iterator i = m_codecList.begin();
//        i += codecSlot;
//
//        // Try to delete the codec
//        if ( i != m_codecList.end() )
//        {
//                delete (*i);                
//                m_codecList.erase( i );
//                return true;
//        }
//        return false;
//}
//
///*-------------------------------------------------------------------------*/
//        
//void
//CCodecChain::RemoveCodec( const CString& typeName )
//{TRACE;
//        TCodecList::iterator i = m_codecList.begin();
//        while ( i != m_codecList.end() )
//        {
//                if ( (*i)->GetType() == typeName )
//                {
//                        // get rid of the codec instance
//                        delete (*i);
//                        m_codecList.erase( i );
//                        
//                        // reset to the start of the list
//                        i = m_codecList.begin();
//                }
//                else
//                {
//                        ++i;
//                }
//        }
//}
//
///*-------------------------------------------------------------------------*/
//        
//bool
//CCodecChain::HasCodec( const CString& typeName ) const
//{TRACE;
//
//        TCodecList::const_iterator i = m_codecList.begin();
//        while ( i != m_codecList.end() )
//        {
//                if ( (*i)->GetType() == typeName )
//                {
//                        return true;
//                }
//                ++i;
//        }
//        return false;
//}
//        
///*-------------------------------------------------------------------------*/
//
//void
//CCodecChain::GetList( CStringList& list ) const
//{TRACE;
//        list.Clear();
//        
//        TCodecList::const_iterator i = m_codecList.begin();
//        while ( i != m_codecList.end() )
//        {
//                list.Append( (*i)->GetType() );
//                ++i;
//        }
//}
//        
///*-------------------------------------------------------------------------*/
//
//void
//CCodecChain::SetCodecOrder( const CStringList& codecOrder )
//{TRACE;
//        // @TODO
//        assert( 0 );
//}
//
///*-------------------------------------------------------------------------*/
//
//bool
//CCodecChain::EncodeBuffers( const TDynamicBufferList& src   ,
//                            const UInt32 sourceBuffersUsed  ,
//                            TDynamicBufferList& dest        ,
//                            UInt32& destBuffsUsed           ,
//                            TDynamicBufferList& swapBuffers ,
//                            CICodec* codec                  ) const
//{TRACE;
//
//    assert( NULL != codec );
//    
//    UInt32 n( 0 ), swapBuffsUsed( 0 );
//    TDynamicBufferList::const_iterator i( src.begin() );
//    while ( ( i != src.end() ) && ( n < sourceBuffersUsed ) )
//    {                                     
//        if ( !codec->Encode( (*i).GetConstBufferPtr() ,
//                             (*i).GetDataSize()       ,
//                             swapBuffers              ,
//                             swapBuffsUsed            ) )
//        {
//                // the encoding failed
//                return false;
//        }
//        
//        // Merge this encoding step into our current destination buffer list
//        TDynamicBufferList::iterator n( swapBuffers.begin() );
//        while ( n != swapBuffers.end() )
//        {        
//            // try to reuse destination buffers
//            if ( destBuffsUsed < dest.size() )
//            {
//                // Copy the buffer contents
//                dest[ destBuffsUsed ] = (*n);
//            }
//            else
//            {
//                // we need more destination buffers then are curently available
//                dest.push_back( (*n) );
//            }
//            ++destBuffsUsed;
//            ++n;
//        }
//        ++n;        
//        ++i;
//    }
//    
//    // batch encode was successfull
//    return true;
//}
//
///*-------------------------------------------------------------------------*/
//        
//bool
//CCodecChain::Encode( const TDynamicBufferList& src  ,
//                     const UInt32 sourceBuffersUsed ,
//                     TDynamicBufferList& dest       ,
//                     UInt32& destBuffersUsed        )
//{TRACE;       
//
//    if ( 0 != m_codecList.size() )
//    {
//        UInt32 destBuffsUsed( 0 );
//        UInt32 srcBuffsUsed( sourceBuffersUsed );
//        const TDynamicBufferList* srcBuffer = &src;
//        TDynamicBufferList* destBuffer = &m_bufferB;
//        TCodecList::const_iterator i = m_codecList.begin();
//        do
//        {            
//            // Encode all buffers
//            if ( !EncodeBuffers( *srcBuffer    ,
//                                 srcBuffsUsed  ,
//                                 *destBuffer   ,
//                                 destBuffsUsed ,
//                                 m_bufferC     ,
//                                 (*i)          ) )
//            {
//                return false;
//            }
//
//            // Swap the buffers
//            if ( destBuffer == &m_bufferA )
//            {
//                srcBuffer = &m_bufferA;
//                destBuffer = &m_bufferB;
//            }
//            else
//            {
//                srcBuffer = &m_bufferB;
//                destBuffer = &m_bufferA;                               
//            }
//            srcBuffsUsed = destBuffsUsed;
//            destBuffsUsed = 0;
//            
//            ++i;
//           
//        }
//        while ( i != m_codecList.end() );
//                        
//        // Assign the output
//        destBuffersUsed = srcBuffsUsed;
//        dest = *destBuffer;
//    }
//    return true;
//}                     
//
///*-------------------------------------------------------------------------*/
//
//bool
//CCodecChain::Decode( const TDynamicBufferList& src  ,
//                     const UInt32 sourceBuffersUsed ,
//                     TDynamicBufferList& dest       ,
//                     UInt32& destBuffersUsed        )
//{TRACE;
//
//    if ( 0 != m_codecList.size() )
//    {
//        UInt32 destBuffsUsed( 0 );
//        UInt32 srcBuffsUsed( sourceBuffersUsed );
//        const TDynamicBufferList* srcBuffer = &src;
//        TDynamicBufferList* destBuffer = &m_bufferB;
//        TCodecList::const_iterator i = m_codecList.begin();
//        do
//        {            
//            // Decode all buffers
//            if ( !DecodeBuffers( *srcBuffer    ,
//                                  srcBuffsUsed  ,
//                                  *destBuffer   ,
//                                  destBuffsUsed ,
//                                  m_bufferC     ,
//                                  (*i)          ) )
//            {
//                return false;
//            }
//
//            // Swap the buffers
//            if ( destBuffer == &m_bufferA )
//            {
//                srcBuffer = &m_bufferA;
//                destBuffer = &m_bufferB;
//            }
//            else
//            {
//                srcBuffer = &m_bufferB;
//                destBuffer = &m_bufferA;                               
//            }
//            srcBuffsUsed = destBuffsUsed;
//            destBuffsUsed = 0;
//            
//            ++i;
//           
//        }
//        while ( i != m_codecList.end() );
//                        
//        // Assign the output
//        destBuffersUsed = srcBuffsUsed;
//        dest = *destBuffer;
//    }
//    return true;
//}
//
///*-------------------------------------------------------------------------*/
//
//bool
//CCodecChain::DecodeBuffers( const TDynamicBufferList& src   ,
//                            const UInt32 sourceBuffersUsed  ,
//                            TDynamicBufferList& dest        ,
//                            UInt32& destBuffsUsed           ,
//                            TDynamicBufferList& swapBuffers ,
//                            CICodec* codec                  ) const
//{TRACE;
//
//    assert( NULL != codec );
//    
//    UInt32 n( 0 ), swapBuffsUsed( 0 );
//    TDynamicBufferList::const_iterator i( src.begin() );
//    while ( ( i != src.end() ) && ( n < sourceBuffersUsed ) )
//    {                                     
//        if ( !codec->Encode( (*i).GetConstBufferPtr() ,
//                             (*i).GetDataSize()       ,
//                             swapBuffers              ,
//                             swapBuffsUsed            ) )
//        {
//                // the decoding failed
//                return false;
//        }
//        
//        // Merge this decoding step into our current destination buffer list
//        TDynamicBufferList::iterator n( swapBuffers.begin() );
//        while ( n != swapBuffers.end() )
//        {        
//            // try to reuse destination buffers
//            if ( destBuffsUsed < dest.size() )
//            {
//                // Copy the buffer contents
//                dest[ destBuffsUsed ] = (*n);
//            }
//            else
//            {
//                // we need more destination buffers then are curently available
//                dest.push_back( (*n) );
//            }
//            ++destBuffsUsed;
//            ++n;
//        }
//        ++n;        
//        ++i;
//    }
//    
//    // batch decode was successfull
//    return true;
//}
//
/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/