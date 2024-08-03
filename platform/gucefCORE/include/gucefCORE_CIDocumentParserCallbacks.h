/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
 *
 *  Copyright (C) 1998 - 2024.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef GUCEF_CORE_CIDOCUMENTPARSERCALLBACKS_H
#define GUCEF_CORE_CIDOCUMENTPARSERCALLBACKS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CSTRING_H
#include "gucefCORE_CString.h"        /* framework string implementation */
#define GUCEF_CORE_CSTRING_H
#endif /* GUCEF_CORE_CSTRING_H ? */

#ifndef GUCEF_CORE_CVARIANT_H
#include "gucefCORE_CVariant.h"       /* framework variant implementation */
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

/**
 *  Interface class for document parse callbacks
 *  In XML lingo these are referred to as SAX parsers
 * 
 *  Implementing this type of interface allows you to interpret a varied range of schema formats
 *  due to the abstraction provided. You dont need to know if you are dealing with a JSON or XML or YAML or whatever document
 *  What matters is the information contained within the document itself, which can be accessed through this type of parser
 */
class GUCEF_CORE_PUBLIC_CPP CIDocumentParserCallbacks
{
    public:

    CIDocumentParserCallbacks( void );
    CIDocumentParserCallbacks( const CIDocumentParserCallbacks& src ); 
    virtual ~CIDocumentParserCallbacks();
    CIDocumentParserCallbacks& operator=( const CIDocumentParserCallbacks& src );

    virtual void OnDocumentBegin( void ) = 0;
    virtual void OnDocumentEnd( void ) = 0;
    virtual void OnNodeBegin( const CVariant& nodeId, int nodeType ) = 0;
    virtual void OnNodeEnd( const CVariant& nodeId ) = 0;
    virtual void OnNodeAttribute( const CVariant& nodeId, const CVariant& attributeId, const CVariant& attributeValue ) = 0;
    virtual void OnNodeValue( const CVariant& nodeId, const CVariant& nodeValue ) = 0;
    virtual void OnNodeChildrenBegin( const CVariant& nodeId ) = 0;
    virtual void OnNodeChildrenEnd( const CVariant& nodeId ) = 0;
    virtual void OnParseError( Int32 errorCode, const CString& description ) = 0;

};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CIDOCUMENTPARSERCALLBACKS_H ? */

/*-------------------------------------------------------------------------*/
