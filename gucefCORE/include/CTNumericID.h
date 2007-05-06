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
    
    CTNumericID( intType id                            ,
                 CINumericIDGeneratorBase* idGenerator );
    
    ~CTNumericID();
    
    bool operator==( const CTNumericID& other ) const;
    
    bool operator!=( const CTNumericID& other ) const;
    
    /**
     *  Conversion operator that allows the numeric ID object to 
     *  be used as if it where of type T.
     */
    operator intType() const;
    
    private:
    
    CTNumericID( void );                              /**< not implemented, cannot be used */
    CTNumericID( const CTNumericID& src );            /**< not implemented, makes no sense */
    CTNumericID& operator=( const CTNumericID& src ); /**< not implemented, makes no sense */
    
    private:
    
    intType m_id;
    CINumericIDGeneratorBase* m_idGenerator;    
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template < typename intType >
CTNumericID< intType >::CTNumericID( const intType id                      ,
                                     CINumericIDGeneratorBase* idGenerator )
    : m_idGenerator( idGenerator ) ,
      m_id( id )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

template < typename intType >
CTNumericID< intType >::~CTNumericID()
{GUCEF_TRACE;

    if ( NULL != m_idGenerator )
    {
        idGenerator->ReleaseID( this );
    }
}

/*-------------------------------------------------------------------------*/

template < typename intType >
bool
CTNumericID< intType >::operator==( const CTNumericID& other ) const
{GUCEF_TRACE;
    
    return m_id == other.m_id;
}

/*-------------------------------------------------------------------------*/
    
template < typename intType >
bool
CTNumericID< intType >::operator!=( const CTNumericID& other ) const
{GUCEF_TRACE;

    return m_id != other.m_id;
}

/*-------------------------------------------------------------------------*/

template < typename intType >
CTNumericID< intType >::operator intType() const
{GUCEF_TRACE;

    return m_id;
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