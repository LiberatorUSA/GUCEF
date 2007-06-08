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
#undef GUCEF_TRACE
#ifdef GUCEF_CORE_DEBUG_MODE    
  #define GUCEF_TRACE GUCEF::CORE::CTracer GUCEFStackTracerObject( __FILE__, __LINE__ );
#else
  #define GUCEF_TRACE
#endif  

/*
 *  Legacy define
 */
#undef TRACE
#define TRACE GUCEF_TRACE

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