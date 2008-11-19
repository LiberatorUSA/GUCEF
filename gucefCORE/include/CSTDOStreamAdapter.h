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

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"   /* often used gucef macros */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

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

class GUCEFCORE_EXPORT_CPP CSTDOStreamAdapter : public std::ostream
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