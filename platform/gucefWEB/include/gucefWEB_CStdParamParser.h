/*
 *  gucefWEB: GUCEF module providing Web application functionality 
 *  for standardized protocols
 *
 *  Copyright (C) 1998 - 2020.  Dinand Vanvelzen
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

#ifndef GUCEF_WEB_CSTDPARAMPARSER_H
#define GUCEF_WEB_CSTDPARAMPARSER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <vector>

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

#ifndef GUCEF_CORE_CICLONEABLE_H
#include "CICloneable.h"
#define GUCEF_CORE_CICLONEABLE_H
#endif /* GUCEF_CORE_CICLONEABLE_H ? */

#ifndef GUCEF_CORE_CSTRING_H
#include "gucefCORE_CString.h"
#define GUCEF_CORE_CSTRING_H
#endif /* GUCEF_CORE_CSTRING_H ? */

#ifndef GUCEF_CORE_CDATANODESERIALIZABLESETTINGS_H
#include "gucefCORE_CDataNodeSerializableSettings.h"
#define GUCEF_CORE_CDATANODESERIALIZABLESETTINGS_H
#endif /* GUCEF_CORE_CDATANODESERIALIZABLESETTINGS_H ? */

#ifndef GUCEF_COMCORE_CHOSTADDRESS_H
#include "CHostAddress.h"
#define GUCEF_COMCORE_CHOSTADDRESS_H
#endif /* GUCEF_COMCORE_CHOSTADDRESS_H ? */

#ifndef GUCEF_WEB_MACROS_H
#include "gucefWEB_macros.h"
#define GUCEF_WEB_MACROS_H
#endif /* GUCEF_WEB_MACROS_H ? */

#ifndef GUCEF_WEB_VIEWTYPES_H
#include "gucefWEB_ViewTypes.h"
#define GUCEF_WEB_VIEWTYPES_H
#endif /* GUCEF_WEB_VIEWTYPES_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace WEB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_WEB_PUBLIC_CPP CStdParams
{
    public:

    TViewType viewType;
    UInt32 levelOfDetail;

    bool viewTypeWasProvided;
    bool levelOfDetailWasProvided;

    CStdParams( void );
    
    /**
     *  For convenience this constructor is provided which will
     *  initialize based on data node serialization settings which is a common use-case
     *  Note that '*WasProvided' is still false since these are just default values to be used IF no params are provided
     */
    CStdParams( const CORE::CDataNodeSerializableSettings& settings );

    CStdParams( const CStdParams& src );
    virtual ~CStdParams();
};

/*-------------------------------------------------------------------------*/

class GUCEF_WEB_PUBLIC_CPP CStdParamParser
{
    public:

    static bool ParseStdParams( const CString& params, CStdParams& outStdParams, bool& foundParams );

    virtual bool ParseParams( const CString& params, CStdParams& outStdParams, bool& foundParams )
                    { return ParseStdParams( params, outStdParams, foundParams ); }

    CStdParamParser( void );
    CStdParamParser( const CStdParamParser& src );
    virtual ~CStdParamParser();
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace WEB */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_WEB_CSTDPARAMPARSER_H ? */
