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

#ifndef GUCEF_CORE_CTNUMERICIDGENERATOR_H
#define GUCEF_CORE_CINUMERICIDGENERATOR_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <set>

#ifndef GUCEF_CORE_EXCEPTIONMACROS_H
#include "ExceptionMacros.h"
#define GUCEF_CORE_EXCEPTIONMACROS_H
#endif /* GUCEF_CORE_EXCEPTIONMACROS_H ? */

#ifndef GUCEF_CORE_CTNUMERICID_H
#include "CTNumericID.h"
#define GUCEF_CORE_CTNUMERICID_H
#endif /* GUCEF_CORE_CTNUMERICID_H ? */

#ifndef GUCEF_CORE_CINUMERICIDGENERATORBASE_H
#include "CINumericIDGeneratorBase.h"
#define GUCEF_CORE_CINUMERICIDGENERATORBASE_H
#endif /* GUCEF_CORE_CINUMERICIDGENERATORBASE_H ? */

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

/**
 *  class template for a numeric ID generator
 *  The IDs generated per generator are guaranteed to be unique
 *
 *  Note that the generator template is meant to be used with
 *  integer arguments, nothing else.
 */
template < typename intType >
class CTNumericIDGenerator : public CINumericIDGeneratorBase
{
    public:

    typedef CTNumericID< intType >   TNumericID;

    CTNumericIDGenerator( void );

    virtual ~CTNumericIDGenerator();

    TNumericID GenerateID( const bool releaseIDOnDestruction = true );

    virtual void ReleaseID( void* idObj );

    GUCEF_DEFINE_INLINED_MSGEXCEPTION( EMaximumReached );

    private:

    CTNumericIDGenerator( const CTNumericIDGenerator& src );            /**< not implemented, makes no sense */
    CTNumericIDGenerator& operator=( const CTNumericIDGenerator& src ); /**< not implemented, makes no sense */

    private:

    intType m_lastID;
    intType m_maxValue;
    std::set< intType > m_availableIDs;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template < typename intType >
CTNumericIDGenerator< intType >::CTNumericIDGenerator( void )
    : m_lastID( 0 )     ,
      m_maxValue( 256 )
{GUCEF_TRACE;

    // simple version of pow(), we multiple until
    // we have the maximum value for this type.
    // Note that an overflow is not a problem due to the way we check
    // against the max value
    for ( unsigned int i=0; i<sizeof( intType ); ++i )
    {
        m_maxValue *= 256;
    }
}

/*-------------------------------------------------------------------------*/

template < typename intType >
CTNumericIDGenerator< intType >::~CTNumericIDGenerator()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template < typename intType >
typename CTNumericIDGenerator< intType >::TNumericID
CTNumericIDGenerator< intType >::GenerateID( const bool releaseIDOnDestruction /* = true */ )
{GUCEF_TRACE;

    // Check against the max value
    // Note that if T is a signed type max will most likely be a negative value
    // so we cannot use > or < operators
    if ( m_lastID+1 != m_maxValue )
    {
        ++m_lastID;
        typename CTNumericIDGenerator< intType >::TNumericID id( m_lastID, releaseIDOnDestruction ? this : NULL );
        return id;
    }

    // We ran out of numbers we can dish out in a fast manner.
    // Now we have to do some more work and check which ID is still available
    intType idValue = 0;
    while ( idValue != m_maxValue )
    {
        if ( m_availableIDs.find( idValue ) != m_availableIDs.end() )
        {
            typename CTNumericIDGenerator< intType >::TNumericID id( idValue, releaseIDOnDestruction ? this : NULL );
            return id;
        }
        ++idValue;
    }

    // If we get here then we ran out of IDs and we also ran out of options
    GUCEF_EMSGTHROW( EMaximumReached, "GUCEF::CORE::CTNumericIDGenerator: ID maximum reached, no more IDs are available" );
}

/*-------------------------------------------------------------------------*/

template < typename intType >
void
CTNumericIDGenerator< intType >::ReleaseID( void* idObj )
{GUCEF_TRACE;

    TNumericID* id = static_cast< TNumericID* >( idObj );
    m_availableIDs.insert( *id );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTNUMERICIDGENERATOR_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 02-03-2007 :
        - Dinand: re-added this header

-----------------------------------------------------------------------------*/
