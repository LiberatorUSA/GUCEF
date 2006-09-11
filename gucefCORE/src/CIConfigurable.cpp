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
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CCONFIGSTORE_H
#include "CConfigStore.h"       /* central configuration control */
#define GUCEF_CORE_CCONFIGSTORE_H
#endif /* GUCEF_CORE_CCONFIGSTORE_H ? */

#include "CIConfigurable.h"     /* definition of the class implemented here */

#ifndef GUCEF_CORE_GUCEF_ESSENTIALS_H
#include "gucef_essentials.h"
#define GUCEF_CORE_GUCEF_ESSENTIALS_H
#endif /* GUCEF_CORE_GUCEF_ESSENTIALS_H ? */  

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/
 
CIConfigurable::CIConfigurable( void )
        : _useglobal( false ) ,
          _configid( 0 )
{
        GUCEF_BEGIN;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CIConfigurable::CIConfigurable( const CIConfigurable& src )
        : _useglobal( false ) ,
          _configid( 0 )
{
        GUCEF_BEGIN;
        SetUseGlobalConfig( src._useglobal );
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CIConfigurable::CIConfigurable( bool useglobalconfig )
        : _useglobal( false ) ,
          _configid( 0 )
{
        GUCEF_BEGIN;
        SetUseGlobalConfig( useglobalconfig );
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/
 
CIConfigurable::~CIConfigurable()
{
        GUCEF_BEGIN;
        if ( _useglobal )
        {
                CConfigStore::Instance()->Unregister( this );        
        }       
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CIConfigurable&
CIConfigurable::operator=( const CIConfigurable& src )
{
        GUCEF_BEGIN;
        if ( this != &src )
        {
                SetUseGlobalConfig( src._useglobal );
        }
        GUCEF_END;
        return *this;
}

/*-------------------------------------------------------------------------*/

void 
CIConfigurable::SetUseGlobalConfig( bool use )
{
        GUCEF_BEGIN;
        if ( use == _useglobal ) return;
        
        if ( use )
        {
                CConfigStore::Instance()->Register( this );
                _useglobal = true;
                GUCEF_END;
                return;                
        }
        CConfigStore::Instance()->Unregister( this );
        _useglobal = false;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/
        
bool 
CIConfigurable::GetUseGlobalConfig( void ) const
{
        GUCEF_BEGIN;
        GUCEF_END;
        return _useglobal;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
 