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

#ifndef GUCEF_CORE_CTVARIANTVECTORAPI_H
#define GUCEF_CORE_CTVARIANTVECTORAPI_H
#ifdef __cplusplus

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <vector>

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"  /* often used gucef macros */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifndef GUCEF_CORE_C_VARIANTVECTOR_H
#include "gucefCORE_c_variantvector.h"
#define GUCEF_CORE_C_VARIANTVECTOR_H
#endif /* GUCEF_CORE_C_VARIANTVECTOR_H ? */

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
//      CLASS TEMPLATES                                                    //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  template which adds a C API adapter to the std::vector class
 *  Since we cannot use templates in C, we need to create a C API adapter 
 *  which is build around the variant concept
 */
template < typename T, class AllocType >
class CTVariantVectorApi
{
    public:
    
    typedef std::vector< T, AllocType >      TLinkedVectorType;
    
    CTVariantVectorApi( void );
    ~CTVariantVectorApi();

    void SetVectorAccessPtr( TLinkedVectorType* vector );

    static void push_back( void* privdata, TVariantData* data, UInt8 linkIfPossible );
    static void clear( void* privdata );
    static size_t size( void* privdata );
    static void back( void* privdata, TVariantData** backData );
    static void front( void* privdata, TVariantData** frontData );
    static void pop_back( void* privdata );
    
    private:
    
    TLinkedVectorType* m_vector;
    TVariantVectorApi m_cApi;
};

/*-------------------------------------------------------------------------*/

template < typename T, class AllocType >
CTVariantVectorApi< T, AllocType >::CTVariantVectorApi( void )
{GUCEF_TRACE;

    m_cApi.push_back = &CTVariantVectorApi< T >::push_back;
    m_cApi.clear = &CTVariantVectorApi< T >::clear;
    m_cApi.size = &CTVariantVectorApi< T >::size;
    m_cApi.back = &CTVariantVectorApi< T >::back;
    m_cApi.front = &CTVariantVectorApi< T >::front;
    m_cApi.pop_back = &CTVariantVectorApi< T >::pop_back;
    m_cApi.privateData = static_cast<void*>( this );
}

/*-------------------------------------------------------------------------*/

template < typename T, class AllocType >
CTVariantVectorApi< T, AllocType >::~CTVariantVectorApi( void )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template < typename T, class AllocType >
void 
CTVariantVectorApi< T, AllocType >::SetVectorAccessPtr( TLinkedVectorType* vector )
{GUCEF_TRACE;

    m_vector = vector;
}

/*-------------------------------------------------------------------------*/

template < typename T, class AllocType >
void 
CTVariantVectorApi< T, AllocType >::push_back( void* privdata, TVariantData* data, UInt8 linkIfPossible )
{GUCEF_TRACE;

    if ( GUCEF_NULL != privdata && GUCEF_NULL != m_vector )
    {
        CTVariantVectorApi< T, AllocType >* api = static_cast< CTVariantVectorApi< T, AllocType >* >( privdata );
        CVariant cppVariant;
        cppVariant.LinkTo( data );
        m_vector->push_back( cppVariant.AsTValue< T >() );
    }
}

/*-------------------------------------------------------------------------*/

template < typename T, class AllocType >
void 
CTVariantVectorApi< T, AllocType >::clear( void* privdata )
{GUCEF_TRACE;

    if ( GUCEF_NULL != privdata && GUCEF_NULL != m_vector )
    {
        CTVariantVectorApi< T, AllocType >* api = static_cast< CTVariantVectorApi< T, AllocType >* >( privdata );
        api->m_vector->clear();
    }
}

/*-------------------------------------------------------------------------*/

template < typename T, class AllocType >
size_t 
CTVariantVectorApi< T, AllocType >::size( void* privdata )
{GUCEF_TRACE;

    if ( GUCEF_NULL != privdata && GUCEF_NULL != m_vector )
    {
        CTVariantVectorApi< T, AllocType >* api = static_cast< CTVariantVectorApi< T, AllocType >* >( privdata );
        return api->m_vector->size();
    }
}

/*-------------------------------------------------------------------------*/

template < typename T, class AllocType >
void 
CTVariantVectorApi< T, AllocType >::back( void* privdata, TVariantData** backData )
{GUCEF_TRACE;

    if ( GUCEF_NULL != privdata && GUCEF_NULL != m_vector && GUCEF_NULL != backData )
    {
        CTVariantVectorApi< T, AllocType >* api = static_cast< CTVariantVectorApi< T, AllocType >* >( privdata );
        CVariant cppVariant( api->m_vector->back() );
        return ;
    }
}

/*-------------------------------------------------------------------------*/


    static void front( void* privdata, TVariantData** front );
    static void pop_back( void* privdata );

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* __cplusplus ? */
#endif /* GUCEF_CORE_CTVARIANTVECTORAPI_H ? */
