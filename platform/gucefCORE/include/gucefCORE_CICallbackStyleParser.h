/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
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

#ifndef GUCEF_CORE_CICALLBACKSTYLEPARSER_H
#define GUCEF_CORE_CICALLBACKSTYLEPARSER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_CSTRING_H
#include "gucefCORE_CString.h"
#define GUCEF_CORE_CSTRING_H
#endif /* GUCEF_CORE_CSTRING_H ? */

#ifndef GUCEF_CORE_CVARIANT_H
#include "gucefCORE_CVariant.h"
#define GUCEF_CORE_CVARIANT_H
#endif /* GUCEF_CORE_CVARIANT_H ? */

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

class GUCEF_CORE_PUBLIC_CPP CICallbackStyleParser
{
    public:

    virtual void OnObjectStart( void ) = 0;
    virtual void OnObjectEnd( void ) = 0;

    virtual void OnArrayStart( void ) = 0;
    virtual void OnArrayEnd( void ) = 0;

    virtual void OnMapStart( void ) = 0;
    virtual void OnMapEnd( void ) = 0;

    virtual void OnSetStart( void ) = 0;
    virtual void OnSetEnd( void ) = 0;

    virtual void OnKeyValueStart( void ) = 0;
    virtual void OnKeyValueEnd( void ) = 0;

    virtual void OnName( const CString& name ) = 0;
    virtual void OnCommentary( const CString& name ) = 0;
    virtual void OnCommentaryReference( const CString& name ) = 0;
    virtual void OnDocumentation( const CString& name ) = 0;
    virtual void OnDocumentationReference( const CString& name ) = 0;
    
    virtual void OnValue( const CVariant& value ) = 0;

    virtual ~CICallbackStyleParser();
    
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CICALLBACKSTYLEPARSER_H ? */
