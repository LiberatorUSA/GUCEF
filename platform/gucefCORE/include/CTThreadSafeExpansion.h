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

#ifndef GUCEF_CORE_CTTHREADSAFEEXPANSION_H
#define GUCEF_CORE_CTTHREADSAFEEXPANSION_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"         /* mutex implementation */
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"       /* module macro's */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

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
 *  Simple utility template for implementing threadsafety on classes
 *  that have the canonical class interface combined with the Lock()
 *  Unlock() mechanism.
 */
template < class Base >
class CTTreadSafeExpansion : public Base
{
    public:
    
    CTTreadSafeExpansion( void );
    
    CTTreadSafeExpansion( const CTTreadSafeExpansion& src );
    
    virtual ~CTTreadSafeExpansion();
    
    CTTreadSafeExpansion& operator=( const CTTreadSafeExpansion& src );
    
    protected:
    
    virtual bool Lock( void ) const;
    
    virtual bool Unlock( void ) const;
    
    private:
    
    MT::CMutex m_dataLock;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template < class Base >
CTTreadSafeExpansion< Base >::CTTreadSafeExpansion( void )
    : Base()       ,
      m_dataLock()
{TRACE;

}

/*-------------------------------------------------------------------------*/

template < class Base >
CTTreadSafeExpansion< Base >::CTTreadSafeExpansion( const CTTreadSafeExpansion& src )
    : Base( src )  ,
      m_dataLock()
{TRACE;

}

/*-------------------------------------------------------------------------*/

template < class Base >
CTTreadSafeExpansion< Base >::~CTTreadSafeExpansion()
{TRACE;

}

/*-------------------------------------------------------------------------*/

template < class Base >
CTTreadSafeExpansion< Base >&
CTTreadSafeExpansion< Base >::operator=( const CTTreadSafeExpansion& src )
{TRACE;

    if ( &src != this )
    {
        Base::operator=( src );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/

template < class Base >
void
CTTreadSafeExpansion< Base >::Lock( void ) const
{TRACE;

    m_dataLock.Lock();
}

/*-------------------------------------------------------------------------*/
    
template < class Base >
void
CTTreadSafeExpansion< Base >::Unlock( void ) const
{TRACE;

    m_dataLock.Unlock();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTTHREADSAFEEXPANSION_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 20-02-2007 :
        - Dinand: Designed and implemented this class

-----------------------------------------------------------------------------*/