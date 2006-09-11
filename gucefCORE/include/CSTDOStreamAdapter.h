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

#ifndef GUCEF_CORE_CSTDOSTREAMADAPTER_H
#define GUCEF_CORE_CSTDOSTREAMADAPTER_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

#include <iostream>

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"          /* GUCEF string implementation */
#define GUCEF_CORE_CDVSTRING_H
#endif /* GUCEF_CORE_CDVSTRING_H ? */

#ifndef GUCEF_CORE_GUCEFCORE_MACROS_H
#include "gucefCORE_macros.h"   /* often used gucef macros */
#define GUCEF_CORE_GUCEFCORE_MACROS_H
#endif /* GUCEF_CORE_GUCEFCORE_MACROS_H ? */

/*-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CSTDOSTREAMADAPTER_CPP
#pragma warning( push )
#endif

#pragma warning( disable: 4275 ) /* non dll-interface class 'classname' used as base for dll-interface class 'classname' */

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

class CIOAccess;
class CSTDOStreamAdapterBuffer;

/*-------------------------------------------------------------------------*/

class EXPORT_CPP CSTDOStreamAdapter : public std::ostream
{
        public:
        
        CSTDOStreamAdapter( CIOAccess& access );
        
        virtual ~CSTDOStreamAdapter();
        
        private:
        
        CSTDOStreamAdapter( void );
        CSTDOStreamAdapter( const CSTDOStreamAdapter& src );
        CSTDOStreamAdapter& operator=( const CSTDOStreamAdapter& src );
        
        private:
        
        CSTDOStreamAdapterBuffer* m_buffer;
        CIOAccess* m_access;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CSTDOSTREAMADAPTER_CPP
#pragma warning( pop )
#endif

/*-------------------------------------------------------------------------*/

#endif /* CSTDOSTREAMADAPTER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 21-01-2006 :
        - Initial implementation

-----------------------------------------------------------------------------*/