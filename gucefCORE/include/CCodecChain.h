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

#ifndef GUCEF_CORE_CCODECCHAIN_H
#define GUCEF_CORE_CCODECCHAIN_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

#include <vector>

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"        /* dynamic buffer implementation */
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CICodec;
class CStringList;

/*-------------------------------------------------------------------------*/

/**
 *      Utility class for sequential encoding/decoding steps
 *      Allows you to create a chain of codecs that will operate 
 *      on the given data in order.
 */
class EXPORT_CPP CCodecChain
{
        public:
        
        CCodecChain( void );
        
        CCodecChain( const CCodecChain& src );
        
        ~CCodecChain();
        
        CCodecChain& operator=( const CCodecChain& src );
        
        bool AddCodec( const CICodec& codec   ,
                       const UInt32 codecSlot );
                       
        void AppendCodec( const CICodec& codec );
        
        void RemoveCodec( const CString& typeName );

        bool RemoveCodec( const UInt32 codecSlot );
        
        bool HasCodec( const CString& typeName ) const;
        
        void GetList( CStringList& list ) const;
        
        void SetCodecOrder( const CStringList& codecOrder );
        
        bool Encode( const void* sourceBuffer      ,
                     const UInt32 sourceBufferSize ,
                     CDynamicBuffer& dest          );

        bool Decode( const void* sourceBuffer      ,
                     const UInt32 sourceBufferSize ,
                     CDynamicBuffer& dest          );
                     
        void Clear( void );
        
        private:
        typedef std::vector< CICodec* > TCodecList;
        
        CDynamicBuffer m_bufferA;
        CDynamicBuffer m_bufferB;
        TCodecList m_codecList;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CCODECCHAIN_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 20-07-2005 :
        - Added this class

-----------------------------------------------------------------------------*/
