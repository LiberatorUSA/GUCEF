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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "gucefCORE_CValueMapBuilder.h"   

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CValueMapBuilder::CValueMapBuilder( void )
    : CIValueMapParserCallbacks()
    , map()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CValueMapBuilder::CValueMapBuilder( const CValueMapBuilder& src )
    : CIValueMapParserCallbacks( src )
    , map( src.map )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CValueMapBuilder::~CValueMapBuilder()
{GUCEF_TRACE;

    map.Clear();
}

/*-------------------------------------------------------------------------*/

CValueMapBuilder&
CValueMapBuilder::operator=( const CValueMapBuilder& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        map = src.map;
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

void 
CValueMapBuilder::OnValueMapBegin( bool allowMultipleValuesPerKey, bool allowDuplicates )
{GUCEF_TRACE;

    map.SetAllowMultipleValues( allowMultipleValuesPerKey );
    map.SetAllowDuplicates( allowDuplicates );
}

/*-------------------------------------------------------------------------*/

void 
CValueMapBuilder::OnValueMapEnd( void )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void 
CValueMapBuilder::OnKeyValuePair( const CVariant& key   , 
                                  const CVariant& value )
{GUCEF_TRACE;

    if ( key != CVariant::Empty && value != CVariant::Empty )
    {
        map.Set( key, value );
    }
}

/*-------------------------------------------------------------------------*/

void 
CValueMapBuilder::OnValueMapParseError( Int32 errorCode            , 
                                        const CString& description )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
