/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
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

#ifndef GUCEF_CORE_CVERSIONRANGE_H
#define GUCEF_CORE_CVERSIONRANGE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CVERSION_H
#include "gucefCORE_CVersion.h"
#define GUCEF_CORE_CVERSION_H
#endif /* GUCEF_CORE_CVERSION_H ? */

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
 *  
 */
class GUCEF_CORE_PUBLIC_CPP CVersionRange
{
    public:

    const CVersion& GetMinimum( void ) const;

    bool GetIsMinimumIncluded( void ) const;

    const CVersion& GetMaximum( void ) const;

    bool GetIsMaximumIncluded( void ) const;
    
    CVersionRange( void );
    
    CVersionRange( const CVersion& min , 
                   bool includeMin     ,
                   const CVersion& max ,
                   bool includeMax     );

    CVersionRange( const CVersionRange& src );

    virtual ~CVersionRange();

    // Implemented for sorting purposes
    bool operator<( const CVersionRange& other ) const;

    bool operator==( const CVersionRange& other ) const;

    CVersionRange& operator=( const CVersionRange& other );

    CVersionRange& operator=( const CVersion& other );
    
    TVersionRange GetCStyle( void ) const;

    void SetCStyle( const TVersionRange& version );

    bool FromString( const CString& rangeStr );

    CString ToString( void ) const;

    private:

    CVersion m_minVersion;
    bool m_minIncluded;
    bool m_hasMinimum;
    CVersion m_maxVersion;
    bool m_maxIncluded;
    bool m_hasMaximum;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CVERSIONRANGE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 20-02-2005 :
        - Dinand: Added this class

----------------------------------------------------------------------------*/
