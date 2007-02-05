/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL, 
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER 
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF 
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT 
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
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