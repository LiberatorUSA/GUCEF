/*
 *  gucefAUDIO: GUCEF module providing audio playback, mixing and decoding
 *
 *  Copyright (C) 1998 - 2026.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
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

#include "gucefAUDIO_CAudioCodecRegistry.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace AUDIO {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CAudioCodecRegistry::CAudioCodecRegistry( void )
    : CORE::CTGlobalONRegistry< CIAudioCodec, MT::CMutex >()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CAudioCodecRegistry::~CAudioCodecRegistry()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CAudioCodecRegistry::TryFindCodec( const CString& codecType, TAudioCodecPtr& codec ) const
{GUCEF_TRACE;

    std::string name = codecType.STL_String();
    if ( !name.empty() && '.' == name[ 0 ] )
        name.erase( 0, 1 );
    return TryLookup( CString( name ).Lowercase(), codec );
}

/*-------------------------------------------------------------------------*/

bool
CAudioCodecRegistry::TryFindCodecForFilename( const CString& filename, TAudioCodecPtr& codec ) const
{GUCEF_TRACE;

    const std::string path = filename.STL_String();
    const std::string::size_type separator = path.find_last_of( "/\\" );
    const std::string::size_type dot = path.find_last_of( '.' );
    if ( std::string::npos == dot || ( std::string::npos != separator && dot < separator ) || dot + 1 >= path.size() )
        return false;
    return TryFindCodec( CString( path.substr( dot + 1 ) ), codec );
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace AUDIO */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
