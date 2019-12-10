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

#ifndef GUCEF_CORE_CTSINGLETON_H
#define GUCEF_CORE_CTSINGLETON_H 

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/ 

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

template < class BaseClass >
class CTSingleton : public BaseClass
{
    public:
    
    static BaseClass* Instance( void );
    
    static void Deinstance( void );
    
    protected:
    
    virtual void LockData( void ) const;
    
    virtual void UnlockData( void ) const;
    
    private:
    
    CTSingleton( void );
    virtual ~CTSingleton();
    CTSingleton( const CTSingleton& src );              /**< not implemented, don't use */
    CTSingleton& operator=( const CTSingleton& src );   /**< not implemented, don't use */ 
    
    private:
    
    static BaseClass* m_instance;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template< class BaseClass >
CTSingleton< BaseClass >::CTSingleton( void )
    : BaseClass()
{
    
}

/*-------------------------------------------------------------------------*/

template< class BaseClass >
CTSingleton< BaseClass >::~CTSingleton()
{

}

/*-------------------------------------------------------------------------*/

template< class BaseClass >
BaseClass*
CTSingleton< BaseClass >::Instance( void )
{
    LockData();
    
    if ( m_instance == NULL )
    {
        m_instance = new CTSingleton< BaseClass >();
    }
    
    UnlockData();
    
    return m_instance;
}

/*-------------------------------------------------------------------------*/

template< class BaseClass >
void
CTSingleton< BaseClass >::Deinstance( void )
{
    LockData();
    
    delete m_instance;
    m_instance = NULL;
    
    UnlockData();
}

/*-------------------------------------------------------------------------*/

template< class BaseClass >
void
CTSingleton< BaseClass >::LockData( void ) const
{
    /* dummy to avoid mandatory implementation by descending classes */
}

/*-------------------------------------------------------------------------*/

template< class BaseClass >
void
CTSingleton< BaseClass >::UnlockData( void ) const
{
    /* dummy to avoid mandatory implementation by descending classes */
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTSINGLETON_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 20-07-2005 :
        - Dinand: Added this class

-----------------------------------------------------------------------------*/