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

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#include "CCodecRegistry.h"

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

CCodecRegistry::CCodecRegistry( void )
    : CTONRegistry< CTONRegistry< CICodec, MT::CMutex >, MT::CMutex >()
    , m_dataLock()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CCodecRegistry::~CCodecRegistry()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

const MT::CILockable* 
CCodecRegistry::AsLockable( void ) const
{GUCEF_TRACE;

    return CTONRegistry< CTONRegistry< CICodec, MT::CMutex >, MT::CMutex >::AsLockable();
}

/*-------------------------------------------------------------------------*/

bool
CCodecRegistry::Lock( UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return m_dataLock.Lock( lockWaitTimeoutInMs );
}

/*-------------------------------------------------------------------------*/

bool
CCodecRegistry::Unlock( void ) const
{GUCEF_TRACE;

    return m_dataLock.Unlock();
}

/*-------------------------------------------------------------------------*/

CCodecRegistry::TICodecPtr 
CCodecRegistry::TryGetCodec( const CString& codecFamily , 
                             const CString& codecName   ) const
{GUCEF_TRACE;

    // First obtain the registery specific to compression codecs
    TCodecFamilyRegistryPtr codecFamilyRegistry;
    if ( TryLookup( codecFamily, codecFamilyRegistry, false ) && codecFamilyRegistry )
    {
        // Locate the configured compression codec to use, if available
        TICodecPtr codec;
        if ( codecFamilyRegistry->TryLookup( codecName, codec, false ) && codec )
        {
            GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "CodecRegistry::TryGetCodec: Obtained compression codec " + codecName + " from codec family " + codecFamily );
            return codec;
        }
    }

    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "CodecRegistry::TryGetCodec: Failed to obtain compression codec " + codecName + " from codec family " + codecFamily );
    return TICodecPtr();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/