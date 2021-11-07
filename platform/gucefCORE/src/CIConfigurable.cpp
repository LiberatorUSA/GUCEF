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
    : _useglobal( false )
    , _configid( 0 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CIConfigurable::CIConfigurable( const CIConfigurable& src )
    : _useglobal( false )
    , _configid( 0 )
{GUCEF_TRACE;

    SetUseGlobalConfig( src._useglobal );
}

/*-------------------------------------------------------------------------*/

CIConfigurable::CIConfigurable( bool useglobalconfig )
    : _useglobal( false ) 
   ,  _configid( 0 )
{GUCEF_TRACE;

    SetUseGlobalConfig( useglobalconfig );
}

/*-------------------------------------------------------------------------*/

CIConfigurable::~CIConfigurable()
{GUCEF_TRACE;

    if ( _useglobal )
    {
        CORE::CCoreGlobal::Instance()->GetConfigStore().Unregister( this );
    }
}

/*-------------------------------------------------------------------------*/

CIConfigurable&
CIConfigurable::operator=( const CIConfigurable& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        SetUseGlobalConfig( src._useglobal );
    }

    return *this;
}

/*-------------------------------------------------------------------------*/

void
CIConfigurable::SetUseGlobalConfig( bool use )
{GUCEF_TRACE;

    if ( use == _useglobal ) 
        return;

    if ( use )
    {
        CORE::CCoreGlobal::Instance()->GetConfigStore().Register( this );
        _useglobal = true;
        return;
    }
    CORE::CCoreGlobal::Instance()->GetConfigStore().Unregister( this );
    _useglobal = false;
}

/*-------------------------------------------------------------------------*/

bool
CIConfigurable::GetUseGlobalConfig( void ) const
{GUCEF_TRACE;

    return _useglobal;
}

/*-------------------------------------------------------------------------*/

bool
CIConfigurable::IsGlobalConfigLoadInProgress( void )
{GUCEF_TRACE;

    return CORE::CCoreGlobal::Instance()->GetConfigStore().IsGlobalConfigLoadInProgress();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
