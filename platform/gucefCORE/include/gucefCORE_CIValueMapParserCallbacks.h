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

#ifndef GUCEF_CORE_CIVALUEMAPPARSERCALLBACKS_H
#define GUCEF_CORE_CIVALUEMAPPARSERCALLBACKS_H

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

#ifndef GUCEF_CORE_C_VALUEMAP_PARSER_CALLBACKS_H
#include "gucefCORE_c_valuemap_parser_callbacks.h"
#define GUCEF_CORE_C_VALUEMAP_PARSER_CALLBACKS_H
#endif /* GUCEF_CORE_C_VALUEMAP_PARSER_CALLBACKS_H ? */

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
 *  Interface class for key-value map parse callbacks
 * 
 *  Implementing this type of interface allows you to interpret a varied range of key-value map formats
 *  due to the abstraction provided. 
 *  What matters is the information contained within the map itself, which can be accessed through this type of parser
 */
class GUCEF_CORE_PUBLIC_CPP CIValueMapParserCallbacks
{
    public:

    CIValueMapParserCallbacks( void );
    CIValueMapParserCallbacks( const CIValueMapParserCallbacks& src ); 
    virtual ~CIValueMapParserCallbacks();
    CIValueMapParserCallbacks& operator=( const CIValueMapParserCallbacks& src );

    virtual void OnValueMapBegin( bool allowMultipleValuesPerKey, bool allowDuplicates ) = 0;
    virtual void OnValueMapEnd( void ) = 0;
    virtual void OnKeyValuePair( const CVariant& key, const CVariant& value ) = 0;
    virtual void OnValueMapParseError( Int32 errorCode, const CString& description ) = 0;

    TValueMapParserCallbacks& GetCStyleAccess( void );
    
    private:

    void InitCStyleAccess( void );

    TValueMapParserCallbacks m_cStyleAccess;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CIVALUEMAPPARSERCALLBACKS_H ? */

/*-------------------------------------------------------------------------*/
