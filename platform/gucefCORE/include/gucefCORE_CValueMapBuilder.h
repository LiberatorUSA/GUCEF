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

#ifndef GUCEF_CORE_CVALUEMAPBUILDER_H
#define GUCEF_CORE_CVALUEMAPBUILDER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CIVALUEMAPPARSERCALLBACKS_H
#include "gucefCORE_CIValueMapParserCallbacks.h"   
#define GUCEF_CORE_CIVALUEMAPPARSERCALLBACKS_H
#endif /* GUCEF_CORE_CIVALUEMAPPARSERCALLBACKS_H ? */

#ifndef GUCEF_CORE_CVALUELIST_H
#include "CValueList.h"   
#define GUCEF_CORE_CVALUELIST_H
#endif /* GUCEF_CORE_CVALUELIST_H ? */

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
 *  Implementation which uses value map parser callbacks to contruct a ValueList class based map
 */
class GUCEF_CORE_PUBLIC_CPP CValueMapBuilder : public CIValueMapParserCallbacks
{
    public:

    CValueMapBuilder( void );
    CValueMapBuilder( const CValueMapBuilder& src ); 
    virtual ~CValueMapBuilder() GUCEF_VIRTUAL_OVERRIDE;
    CValueMapBuilder& operator=( const CValueMapBuilder& src );

    virtual void OnValueMapBegin( bool allowMultipleValuesPerKey, bool allowDuplicates ) GUCEF_VIRTUAL_OVERRIDE;
    virtual void OnValueMapEnd( void ) GUCEF_VIRTUAL_OVERRIDE;
    virtual void OnKeyValuePair( const CVariant& key, const CVariant& value ) GUCEF_VIRTUAL_OVERRIDE;
    virtual void OnValueMapParseError( Int32 errorCode, const CString& description ) GUCEF_VIRTUAL_OVERRIDE;

    CValueList map;

};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CDATANODEDOCUMENTBUILDER_H ? */

/*-------------------------------------------------------------------------*/
