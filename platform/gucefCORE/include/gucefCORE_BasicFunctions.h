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

#ifndef GUCEF_CORE_BASICFUNCTIONS_H
#define GUCEF_CORE_BASICFUNCTIONS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CIFUNCTION_H
#include "gucefCORE_CIFunction.h"
#define GUCEF_CORE_CIFUNCTION_H
#endif /* GUCEF_CORE_CIFUNCTION_H ? */

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

class GUCEF_CORE_PRIVATE_CPP CGreaterFunction : public CIFunction
{
    public:

    virtual bool ExecuteFunction( const CVariant::VariantVector& params       ,
                                  const CVariant::VariantVector& functionData ,
                                  CVariant::VariantVector& functionResults    ) const GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;

        if ( params.size() >= 2 )
        {
            functionResults.push_back( params[ 0 ] > params[ 1 ] );
            return true;
        }

        // insufficient nr of params given
        return false;
    }

    virtual const CString& GetName( void ) const GUCEF_VIRTUAL_OVERRIDE
    {
        static CString functionName = ">";
        return functionName;
    }

    virtual ~CGreaterFunction() {GUCEF_TRACE; }
};

/*-------------------------------------------------------------------------*/

class GUCEF_CORE_PRIVATE_CPP CGreaterEqualsFunction : public CIFunction
{
    public:

    virtual bool ExecuteFunction( const CVariant::VariantVector& params       ,
                                  const CVariant::VariantVector& functionData ,
                                  CVariant::VariantVector& functionResults    ) const GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;

        if ( params.size() >= 2 )
        {
            functionResults.push_back( params[ 0 ] >= params[ 1 ] );
            return true;
        }

        // insufficient nr of params given
        return false;
    }

    virtual const CString& GetName( void ) const GUCEF_VIRTUAL_OVERRIDE
    {
        static CString functionName = ">=";
        return functionName;
    }

    virtual ~CGreaterEqualsFunction() {GUCEF_TRACE; }
};

/*-------------------------------------------------------------------------*/

class GUCEF_CORE_PRIVATE_CPP CSmallerFunction : public CIFunction
{
    public:

    virtual bool ExecuteFunction( const CVariant::VariantVector& params       ,
                                  const CVariant::VariantVector& functionData ,
                                  CVariant::VariantVector& functionResults    ) const GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;

        if ( params.size() >= 2 )
        {
            functionResults.push_back( params[ 0 ] < params[ 1 ] );
            return true;
        }

        // insufficient nr of params given
        return false;
    }

    virtual const CString& GetName( void ) const GUCEF_VIRTUAL_OVERRIDE
    {
        static CString functionName = "<";
        return functionName;
    }

    virtual ~CSmallerFunction() {GUCEF_TRACE; }
};

/*-------------------------------------------------------------------------*/

class GUCEF_CORE_PRIVATE_CPP CSmallerEqualsFunction : public CIFunction
{
    public:

    virtual bool ExecuteFunction( const CVariant::VariantVector& params       ,
                                  const CVariant::VariantVector& functionData ,
                                  CVariant::VariantVector& functionResults    ) const GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;

        if ( params.size() >= 2 )
        {
            functionResults.push_back( params[ 0 ] <= params[ 1 ] );
            return true;
        }

        // insufficient nr of params given
        return false;
    }

    virtual const CString& GetName( void ) const GUCEF_VIRTUAL_OVERRIDE
    {
        static CString functionName = "<=";
        return functionName;
    }

    virtual ~CSmallerEqualsFunction() {GUCEF_TRACE; }
};

/*-------------------------------------------------------------------------*/

class GUCEF_CORE_PRIVATE_CPP CEqualsValueFunction : public CIFunction
{
    public:

    virtual bool ExecuteFunction( const CVariant::VariantVector& params       ,
                                  const CVariant::VariantVector& functionData ,
                                  CVariant::VariantVector& functionResults    ) const GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;

        if ( params.size() >= 2 )
        {
            functionResults.push_back( params[ 0 ] == params[ 1 ] );
            return true;
        }

        // insufficient nr of params given
        return false;
    }

    virtual const CString& GetName( void ) const GUCEF_VIRTUAL_OVERRIDE
    {
        static CString functionName = "==";
        return functionName;
    }

    virtual ~CEqualsValueFunction() {GUCEF_TRACE; }
};

/*-------------------------------------------------------------------------*/

//class GUCEF_CORE_PRIVATE_CPP CAddValueFunction : public CIFunction
//{
//    public:
//
//    virtual bool ExecuteFunction( const CVariant::VariantVector& params       ,
//                                  const CVariant::VariantVector& functionData ,
//                                  CVariant::VariantVector& functionResults    ) const GUCEF_VIRTUAL_OVERRIDE
//    {GUCEF_TRACE;
//
//        if ( params.size() >= 2 )
//        {
//            functionResults.push_back( params[ 0 ] + params[ 1 ] );
//            return true;
//        }
//
//        // insufficient nr of params given
//        return false;
//    }
//
//    virtual const CString& GetName( void ) const
//    {
//        static CString functionName = "+";
//        return functionName;
//    }
//
//    virtual ~CAddValueFunction() {GUCEF_TRACE; }
//};

/*-------------------------------------------------------------------------*/

class GUCEF_CORE_PRIVATE_CPP CIsNullOrEmptyFunction : public CIFunction
{
    public:

    virtual bool ExecuteFunction( const CVariant::VariantVector& params       ,
                                  const CVariant::VariantVector& functionData ,
                                  CVariant::VariantVector& functionResults    ) const GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;

        if ( params.size() >= 1 )
        {
            functionResults.push_back( params[ 0 ].IsNULLOrEmpty() );
            return true;
        }

        // insufficient nr of params given
        return false;
    }

    virtual const CString& GetName( void ) const GUCEF_VIRTUAL_OVERRIDE
    {
        static CString functionName = "IsNullOrEmpty";
        return functionName;
    }

    virtual ~CIsNullOrEmptyFunction() {GUCEF_TRACE; }
};

/*-------------------------------------------------------------------------*/

class GUCEF_CORE_PRIVATE_CPP CIsOfTypeFunction : public CIFunction
{
    public:

    virtual bool ExecuteFunction( const CVariant::VariantVector& params       ,
                                  const CVariant::VariantVector& functionData ,
                                  CVariant::VariantVector& functionResults    ) const GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;

        if ( params.size() >= 2 )
        {
            functionResults.push_back( params[ 0 ].GetTypeId() == params[ 1 ].AsUInt8() );
            return true;
        }

        // insufficient nr of params given
        return false;
    }

    virtual const CString& GetName( void ) const GUCEF_VIRTUAL_OVERRIDE
    {
        static CString functionName = "IsOfType";
        return functionName;
    }

    virtual ~CIsOfTypeFunction() {GUCEF_TRACE; }
};

/*-------------------------------------------------------------------------*/

class GUCEF_CORE_PRIVATE_CPP CAsTypeFunction : public CIFunction
{
    public:

    virtual bool ExecuteFunction( const CVariant::VariantVector& params       ,
                                  const CVariant::VariantVector& functionData ,
                                  CVariant::VariantVector& functionResults    ) const GUCEF_VIRTUAL_OVERRIDE
    {GUCEF_TRACE;

        if ( params.size() >= 2 )
        {
            switch ( params[ 1 ].AsUInt8() )
            {
                case GUCEF_DATATYPE_INT8:
                {
                    Int8 defaultValue = params.size() > 2 ? params[ 2 ].AsInt8() : 0;
                    functionResults.push_back( params[ 0 ].AsInt8( defaultValue ) );
                    return true;
                }
                case GUCEF_DATATYPE_UINT8:
                {
                    UInt8 defaultValue = params.size() > 2 ? params[ 2 ].AsUInt8() : 0;
                    functionResults.push_back( params[ 0 ].AsUInt8( defaultValue ) );
                    return true;
                }
                case GUCEF_DATATYPE_LE_INT16:
                case GUCEF_DATATYPE_BE_INT16:
                {
                    Int16 defaultValue = params.size() > 2 ? params[ 2 ].AsInt16() : 0;
                    functionResults.push_back( params[ 0 ].AsInt16( defaultValue ) );
                    return true;
                }
                case GUCEF_DATATYPE_LE_UINT16:
                case GUCEF_DATATYPE_BE_UINT16:
                {
                    UInt16 defaultValue = params.size() > 2 ? params[ 2 ].AsUInt16() : 0;
                    functionResults.push_back( params[ 0 ].AsUInt16( defaultValue ) );
                    return true;
                }
                case GUCEF_DATATYPE_LE_INT32:
                case GUCEF_DATATYPE_BE_INT32:
                {
                    Int32 defaultValue = params.size() > 2 ? params[ 2 ].AsInt32() : 0;
                    functionResults.push_back( params[ 0 ].AsInt32( defaultValue ) );
                    return true;
                }
                case GUCEF_DATATYPE_LE_UINT32:
                case GUCEF_DATATYPE_BE_UINT32:
                {
                    UInt32 defaultValue = params.size() > 2 ? params[ 2 ].AsUInt32() : 0;
                    functionResults.push_back( params[ 0 ].AsUInt32( defaultValue ) );
                    return true;
                }
                case GUCEF_DATATYPE_LE_INT64:
                case GUCEF_DATATYPE_BE_INT64:
                {
                    Int64 defaultValue = params.size() > 2 ? params[ 2 ].AsInt64() : 0;
                    functionResults.push_back( params[ 0 ].AsInt64( defaultValue ) );
                    return true;
                }
                case GUCEF_DATATYPE_LE_UINT64:
                case GUCEF_DATATYPE_BE_UINT64:
                {
                    UInt64 defaultValue = params.size() > 2 ? params[ 2 ].AsUInt64() : 0;
                    functionResults.push_back( params[ 0 ].AsUInt64( defaultValue ) );
                    return true;
                }
                case GUCEF_DATATYPE_FLOAT32:
                {
                    Float32 defaultValue = params.size() > 2 ? params[ 2 ].AsFloat32() : 0.0f;
                    functionResults.push_back( params[ 0 ].AsFloat32( defaultValue ) );
                    return true;
                }
                case GUCEF_DATATYPE_FLOAT64:
                {
                    Float64 defaultValue = params.size() > 2 ? params[ 2 ].AsFloat64() : 0.0;
                    functionResults.push_back( params[ 0 ].AsFloat64( defaultValue ) );
                    return true;
                }
                case GUCEF_DATATYPE_ASCII_STRING:
                {
                    CAsciiString defaultValue = params.size() > 2 ? params[ 2 ].AsAsciiString() : CAsciiString::Empty;
                    functionResults.push_back( params[ 0 ].AsAsciiString( defaultValue ) );
                    return true;
                }
                case GUCEF_DATATYPE_UTF8_STRING:
                {
                    CUtf8String defaultValue = params.size() > 2 ? params[ 2 ].AsUtf8String() : CUtf8String::Empty;
                    functionResults.push_back( params[ 0 ].AsUtf8String( defaultValue ) );
                    return true;
                }
            }
        }

        // insufficient nr of params given or invalid type id
        return false;
    }

    virtual const CString& GetName( void ) const GUCEF_VIRTUAL_OVERRIDE
    {
        static CString functionName = "AsType";
        return functionName;
    }

    virtual ~CAsTypeFunction() {GUCEF_TRACE; }
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_BASICFUNCTIONS_H ? */
