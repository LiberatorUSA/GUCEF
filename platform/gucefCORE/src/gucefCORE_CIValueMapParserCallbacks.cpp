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

#include "gucefCORE_CIValueMapParserCallbacks.h"    

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

void GUCEF_PLUGIN_CALLSPEC_PREFIX
OnValueMapBeginHandler( void* privdata                 , 
                        char allowMultipleValuesPerKey , 
                        char allowDuplicates           ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;

    if ( GUCEF_NULL != privdata )
    {
        CIValueMapParserCallbacks* cppObj = static_cast< CIValueMapParserCallbacks* >( privdata );

        bool cppAllowMultipleValuesPerKey = ( 0 != allowMultipleValuesPerKey ); 
        bool cppAllowDuplicates = ( 0 != allowDuplicates );

        cppObj->OnValueMapBegin( cppAllowMultipleValuesPerKey, cppAllowDuplicates );
    }
}

/*-------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX
OnValueMapEndHandler( void* privdata ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;

    if ( GUCEF_NULL != privdata )
    {
        CIValueMapParserCallbacks* cppObj = static_cast< CIValueMapParserCallbacks* >( privdata );
        cppObj->OnValueMapEnd();
    }
}

/*-------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX
OnKeyValueHandler( void* privdata            ,
                   const TVariantData* key   ,
                   const TVariantData* value ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;

    if ( GUCEF_NULL != privdata && GUCEF_NULL != key && GUCEF_NULL != value )
    {
        CIValueMapParserCallbacks* cppObj = static_cast< CIValueMapParserCallbacks* >( privdata );

        CVariant cppKey;
        cppKey.LinkTo( key );
        CVariant cppValue;
        cppValue.LinkTo( value );

        cppObj->OnKeyValuePair( cppKey, cppValue );
    }
}

/*-------------------------------------------------------------------------*/

void GUCEF_PLUGIN_CALLSPEC_PREFIX
OnValueMapParseErrorHandler( void* privdata          ,
                             Int32 errorcode         ,
                             const char* description ) GUCEF_PLUGIN_CALLSPEC_SUFFIX
{GUCEF_TRACE;

    if ( GUCEF_NULL != privdata )
    {
        CIValueMapParserCallbacks* cppObj = static_cast< CIValueMapParserCallbacks* >( privdata );
        cppObj->OnValueMapParseError( errorcode, description );
    }
}

/*-------------------------------------------------------------------------*/

CIValueMapParserCallbacks::CIValueMapParserCallbacks( void )
    : m_cStyleAccess()
{GUCEF_TRACE;

    InitCStyleAccess();
}

/*-------------------------------------------------------------------------*/

CIValueMapParserCallbacks::CIValueMapParserCallbacks( const CIValueMapParserCallbacks& src )
    : m_cStyleAccess()
{GUCEF_TRACE;

    InitCStyleAccess();       
}

/*-------------------------------------------------------------------------*/

CIValueMapParserCallbacks::~CIValueMapParserCallbacks()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CIValueMapParserCallbacks& 
CIValueMapParserCallbacks::operator=( const CIValueMapParserCallbacks& src )
{GUCEF_TRACE;

    return *this;
}

/*-------------------------------------------------------------------------*/

void
CIValueMapParserCallbacks::InitCStyleAccess( void )
{GUCEF_TRACE;

    memset( &m_cStyleAccess, 0, sizeof( m_cStyleAccess ) );
    m_cStyleAccess.OnValueMapBegin = OnValueMapBeginHandler;
    m_cStyleAccess.OnValueMapEnd = OnValueMapEndHandler;
    m_cStyleAccess.OnKeyValuePair = OnKeyValueHandler;
    m_cStyleAccess.OnValueMapParserError = OnValueMapParseErrorHandler;
    m_cStyleAccess.privateData = this;
}

/*-------------------------------------------------------------------------*/

TValueMapParserCallbacks& 
CIValueMapParserCallbacks::GetCStyleAccess( void )
{GUCEF_TRACE;

    return m_cStyleAccess;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
