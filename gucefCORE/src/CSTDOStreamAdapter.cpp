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

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#ifndef GUCEF_CORE_CIOACCESS_H
#include "CIOAccess.h"
#define GUCEF_CORE_CIOACCESS_H
#endif /* GUCEF_CORE_CIOACCESS_H ? */

#define GUCEF_CORE_CSTDOSTREAMADAPTER_CPP
#include "CSTDOStreamAdapter.h"

#ifndef GUCEF_CORE_GUCEFCORE_MACROS_H
#include "gucefCORE_macros.h"  /* often used gucef macros */
#define GUCEF_CORE_GUCEFCORE_MACROS_H
#endif /* GUCEF_CORE_GUCEFCORE_MACROS_H ? */

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

class CSTDOStreamAdapterBuffer : public std::ostream::_Mysb
{
        public:
        typedef std::ostream::_Mysb             base;
        typedef base::int_type                  int_type;
        
        
        CSTDOStreamAdapterBuffer( CIOAccess& access ); 
        
        protected:
        
        virtual int_type overflow( int_type value );       
        
        private:
        
        CSTDOStreamAdapterBuffer( const CSTDOStreamAdapterBuffer& src );
        CSTDOStreamAdapterBuffer& operator=( const CSTDOStreamAdapterBuffer& src );
        
        private:
        
        CIOAccess* m_access;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CSTDOStreamAdapterBuffer::CSTDOStreamAdapterBuffer( CIOAccess& access )
        : base( std::_Noinit ) ,
          m_access( &access )
{TRACE;
        assert( m_access );
}

/*-------------------------------------------------------------------------*/

CSTDOStreamAdapterBuffer::int_type 
CSTDOStreamAdapterBuffer::overflow( int_type value )
{TRACE;
        // put a character to stream
        if ( m_access->Write( &value, 1, 1 ) == 1 )
        {
        }
        return traits_type::eof();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

CSTDOStreamAdapter::CSTDOStreamAdapter( CIOAccess& access )
        : std::ostream( std::_Noinit )                     ,
          m_buffer( new CSTDOStreamAdapterBuffer( access ) )
{TRACE;        
        std::ostream::_Myios::init( m_buffer, false ); 
}

/*-------------------------------------------------------------------------*/

CSTDOStreamAdapter::~CSTDOStreamAdapter()
{TRACE;
        delete m_buffer;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
