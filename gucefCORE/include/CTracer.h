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

#ifndef GUCEF_CORE_CTRACER_H
#define GUCEF_CORE_CTRACER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
  #include "gucefCORE_macros.h"
  #define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

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

/**
 *      Very simply class that calls the GUCEF callstack functions.
 *      This allows us to use creation and destruction behavior of a C++ object
 *      to ensure that both the begin and end callstack functions are called for
 *      the scope where the tracer object is created.
 *
 *      Note that it is recommended to use the TRACER macro instead of using the class
 *      directly
 */
class GUCEFCORE_EXPORT_CPP CTracer
{
        public:
        
        CTracer( const char* file ,
                 const int line   );
        
        ~CTracer();
        
        private:
        
        CTracer( void );
        
        CTracer( const CTracer& src );
        
        CTracer& operator=( const CTracer& src );
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------//
//                                                                         //
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/ 

/**
 *      Macro that creates a tracer object and fills in the file and line
 *      precompiler directives. This should save you some typing.
 */
#undef TRACE
#ifdef GUCEF_CORE_DEBUG_MODE    
  #define TRACE GUCEF::CORE::CTracer GUCEFStackTracerObject( __FILE__, __LINE__ );
#else
  #define TRACE
#endif  

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTRACER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 12-11-2004 :
        - Designed and implemented this class.

-----------------------------------------------------------------------------*/