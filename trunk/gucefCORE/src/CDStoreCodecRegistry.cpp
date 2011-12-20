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
//      INCLUDE                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CLOGMANAGER_H
#include "CLogManager.h"
#define GUCEF_CORE_CLOGMANAGER_H
#endif /* GUCEF_CORE_CLOGMANAGER_H ? */

#include "CDStoreCodecRegistry.h"

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
//      GLOBAL VARS                                                        //
//                                                                         //
//-------------------------------------------------------------------------*/

CDStoreCodecRegistry* CDStoreCodecRegistry::_instance = NULL;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

CDStoreCodecRegistry::CDStoreCodecRegistry( void )
{
        GUCEF_BEGIN;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CDStoreCodecRegistry::CDStoreCodecRegistry( const CDStoreCodecRegistry& src )
{
        GUCEF_BEGIN;
        /* dummy, do not use */
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CDStoreCodecRegistry::~CDStoreCodecRegistry()
{
        GUCEF_BEGIN;
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

CDStoreCodecRegistry*
CDStoreCodecRegistry::Instance( void )
{GUCEF_TRACE;

    if ( NULL == _instance )
    {
        _instance = new CDStoreCodecRegistry();
        GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "GUCEF::CORE::CDStoreCodecRegistry Singleton created" );
    }
    return _instance;
}

/*-------------------------------------------------------------------------*/

void
CDStoreCodecRegistry::Deinstance( void )
{GUCEF_TRACE;

    delete _instance;
    _instance = NULL;
    GUCEF_SYSTEM_LOG( LOGLEVEL_NORMAL, "GUCEF::CORE::CDStoreCodecRegistry Singleton destroyed" );
}

/*-------------------------------------------------------------------------*/

void
CDStoreCodecRegistry::LockData( void ) const
{
        GUCEF_BEGIN;
        _datalock.Lock();
        GUCEF_END;
}

/*-------------------------------------------------------------------------*/

void
CDStoreCodecRegistry::UnlockData( void ) const
{
        GUCEF_BEGIN;
        _datalock.Unlock();
        GUCEF_END;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
