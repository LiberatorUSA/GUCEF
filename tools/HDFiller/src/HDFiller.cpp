/*
 *  HDFiller: A utility for filling up the hard drive
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

// This tool is meant for testing purposes, allowing you to test the behaviour
// of your application when the HD is full

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <stdio.h>
#include <cstdlib>

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

const GUCEF::CORE::UInt32 BLOCKSIZE = 1024000;
const GUCEF::CORE::UInt32 MAXBLOCKS = ( GUCEFCORE_UINT32MAX / BLOCKSIZE ) -1;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Application entry point
 */
GUCEF_OSMAIN_BEGIN
{GUCEF_TRACE;

    GUCEF::CORE::UInt8* buffer = new GUCEF::CORE::UInt8[ BLOCKSIZE ];
    GUCEF::CORE::UInt32 fileCount = 0;
    GUCEF::CORE::UInt32 writtenBytes = 0;
    bool writeSuccess = true;

    // Fill the block with random values, just in case someone wants
    // to use this tool for security purposes
    GUCEF::CORE::UInt8 randomChar = GUCEF::CORE::UInt8( std::rand() / (RAND_MAX / 256 + 1) );
    std::srand( randomChar );
    for ( GUCEF::CORE::UInt32 i=0; i<BLOCKSIZE; ++i )
    {
        randomChar = GUCEF::CORE::UInt8( std::rand() / (RAND_MAX / 256 + 1) );
        buffer[ i ] = randomChar;
    }

    while ( writeSuccess )
    {
        GUCEF::CORE::CString filename = "HDfillerFile" + GUCEF::CORE::UInt32ToString( fileCount ) + ".tmp";
        FILE* fptr = fopen( filename.C_String(), "wb" );

        // Check if we do not overflow out of our 32 bit addressing range
        for ( GUCEF::CORE::UInt32 i=0; i<MAXBLOCKS; ++i )
        {
            writtenBytes = (GUCEF::CORE::UInt32) fwrite( buffer, 1, BLOCKSIZE, fptr );
            if ( writtenBytes != BLOCKSIZE )
            {
                // Fill the last partial block
                fwrite( buffer, 1, BLOCKSIZE-writtenBytes, fptr );

                // HD is now full
                writeSuccess = false;
                break;
            }
        }

        fclose( fptr );
        ++fileCount;
    }

    delete []buffer;

}
GUCEF_OSMAIN_END

/*---------------------------------------------------------------------------*/
