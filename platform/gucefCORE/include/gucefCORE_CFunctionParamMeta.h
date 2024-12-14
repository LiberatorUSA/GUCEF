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

#ifndef GUCEF_CORE_CFUNCTIONPARAMMETA_H
#define GUCEF_CORE_CFUNCTIONPARAMMETA_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_C_FUNCTION_DEFINITION_H
#include "gucefCORE_c_function_definition.h"
#define GUCEF_CORE_C_FUNCTION_DEFINITION_H
#endif /* GUCEF_CORE_C_FUNCTION_DEFINITION_H ? */

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

/**
 *  Class to hold meta-data for a runtime registered function parameter
 * 
 *  Intended to work with the execution engine flow as a building block in a 
 *  generic scripting system
 * 
 *  This class provides an underlying C style structure
 *  this allows for easy integration with C style APIs
 *  which are a common way to interface with scripting languages
 */
class GUCEF_CORE_PUBLIC_CPP CFunctionParamMeta
{
    public:

    CFunctionParamMeta( void );

    CFunctionParamMeta( const CFunctionParamMeta& src );

    CFunctionParamMeta( const TFunctionParamMeta* c_style_param_meta );

    ~CFunctionParamMeta();

    CFunctionParamMeta& operator=( const CFunctionParamMeta& src );

    void SetParamType( TFunctionParamType paramType );

    TFunctionParamType GetParamType( void ) const;

    void SetParamDataType( UInt8 paramDataType );

    UInt8 GetParamDataType( void ) const;

    void SetIsParamConst( bool isParamConst );

    bool GetIsParamConst( void ) const;

    /**
     *  If a parameter is optional, it means that it can be omitted
     *  from the function call. This requires that a default value is set
     *  The presence of a default value signifies that the parameter is optional
     */
    bool GetIsParamOptional( void ) const;

    /**
     *  The default value is only used when the parameter is optional
     *  It should only be used if nothing is passed or 'undefined' is passed as the param type
     */
    void SetDefaultValue( const CVariant& defaultValue );

    /**
     *  Intended to clear the default value and set it to 'no default value'
     *  This also sets the parameter to non-optional as a result
     */
    void ClearDefaultValue( void );

    /**
     *  Returns the default value if one exists. Otherwise just Variant::Empty  
     */
    const CVariant& GetDefaultValue( void ) const;

    void SetParamName( const CString& paramName );

    const CString& GetParamName( void ) const;

    const TFunctionParamMeta* GetCStyleParamMeta( void ) const;

    private:

    TFunctionParamMeta m_paramMeta;
    CString m_paramName;
    CVariant* m_defaultValue;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CFUNCTIONPARAMMETA_H ? */
