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

#ifndef GUCEF_CORE_CTNUMERICID_H
#define GUCEF_CORE_CTNUMERICID_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CINUMERICIDGENERATORBASE_H
#include "CINumericIDGeneratorBase.h"
#define GUCEF_CORE_CINUMERICIDGENERATORBASE_H
#endif /* GUCEF_CORE_CINUMERICIDGENERATORBASE_H ? */

#ifndef GUCEF_CORE_EXCEPTIONMACROS_H
#include "ExceptionMacros.h"
#define GUCEF_CORE_EXCEPTIONMACROS_H
#endif /* GUCEF_CORE_EXCEPTIONMACROS_H ? */

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF { 
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      TEMPLATES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template < typename intType >
class CTNumericID
{
    public:
    
    CTNumericID( void );
    
    CTNumericID( CTNumericID& src );
    
    CTNumericID( intType id                            ,
                 CINumericIDGeneratorBase* idGenerator );
    
    ~CTNumericID();
    
    CTNumericID& operator=( CTNumericID& src );
    
    bool operator==( const CTNumericID& other ) const;
    
    bool operator!=( const CTNumericID& other ) const;
    
    bool IsInitialized( void ) const;
    
    /**
     *  Conversion operator that allows the numeric ID object to 
     *  be used as if it where of type T.
     */
    operator intType() const;
    
    GUCEF_DEFINE_INLINED_MSGEXCEPTION( ENotInitialized );
    GUCEF_DEFINE_INLINED_MSGEXCEPTION( EIllegalOperation );
    
    private:
    
    intType m_id;
    bool m_initialized;
    CINumericIDGeneratorBase* m_idGenerator;    
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template < typename intType >
CTNumericID< intType >::CTNumericID( void )
    : m_idGenerator( NULL )  ,
      m_id( 0 )              ,
      m_initialized( false )
{GUCEF_TRACE;

    // The ID will have to be initialized before use or an exception will be thrown
}

/*-------------------------------------------------------------------------*/

template < typename intType >
CTNumericID< intType >::CTNumericID( CTNumericID& src )
    : m_idGenerator( src.m_idGenerator ) ,
      m_id( src.m_id )                   ,
      m_initialized( src.m_initialized )
{GUCEF_TRACE;

    // Assume ownership of the ID and invalidate the source ID
    src.m_initialized = false;
    src.m_idGenerator = NULL;
    src.m_id = 0;
}

/*-------------------------------------------------------------------------*/

template < typename intType >
CTNumericID< intType >::CTNumericID( const intType id                      ,
                                     CINumericIDGeneratorBase* idGenerator )
    : m_idGenerator( idGenerator ) ,
      m_id( id )                   ,
      m_initialized( true )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template < typename intType >
CTNumericID< intType >::~CTNumericID()
{GUCEF_TRACE;

    if ( m_initialized && NULL != m_idGenerator )
    {
        m_idGenerator->ReleaseID( this );
    }
}

/*-------------------------------------------------------------------------*/

template < typename intType >
CTNumericID< intType >&
CTNumericID< intType >::operator=( CTNumericID& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        if ( !m_initialized )
        {
            // Copy the attributes
            m_initialized = src.m_initialized;
            m_idGenerator = src.m_idGenerator;
            m_id = src.m_id;
            
            // Assume ownership of the ID and invalidate the source ID
            src.m_initialized = false;
            src.m_idGenerator = NULL;
            src.m_id = 0;        
        }
        else
        {
            GUCEF_EMSGTHROW( EIllegalOperation, "CTNumericID<>::operator=(): the ID is already initialized" );
        }
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

template < typename intType >
bool
CTNumericID< intType >::operator==( const CTNumericID& other ) const
{GUCEF_TRACE;
    
    if ( m_initialized )
    {
        return m_id == other.m_id;
    }
    GUCEF_EMSGTHROW( ENotInitialized, "CTNumericID<>::operator==(): the ID is not initialized" );
}

/*-------------------------------------------------------------------------*/
    
template < typename intType >
bool
CTNumericID< intType >::operator!=( const CTNumericID& other ) const
{GUCEF_TRACE;

    if ( m_initialized )
    {
        return m_id != other.m_id;
    }
    GUCEF_EMSGTHROW( ENotInitialized, "CTNumericID<>::operator!=(): the ID is not initialized" );
}

/*-------------------------------------------------------------------------*/

template < typename intType >
bool
CTNumericID< intType >::IsInitialized( void ) const
{GUCEF_TRACE;

    return m_initialized;
}

/*-------------------------------------------------------------------------*/

template < typename intType >
CTNumericID< intType >::operator intType() const
{GUCEF_TRACE;

    if ( m_initialized )
    {
        return m_id;
    }
    GUCEF_EMSGTHROW( ENotInitialized, "CTNumericID<>::operator intType(): the ID is not initialized" );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTNUMERICID_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 02-03-2007 :
        - Dinand: re-added this header

-----------------------------------------------------------------------------*/