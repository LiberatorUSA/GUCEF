/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
 *  Copyright (C) 2002 - 2026.  Dinand Vanvelzen
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

#include <unistd.h>
#include <poll.h>
#include <errno.h>

#ifndef GUCEF_MT_CSCOPEMUTEX_H
#include "gucefMT_CScopeMutex.h"
#define GUCEF_MT_CSCOPEMUTEX_H
#endif /* GUCEF_MT_CSCOPEMUTEX_H ? */

#include "gucefCORE_CTerminalConsoleLogger.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

bool
CTerminalConsoleLogger::IsRunningInTerminal( void )
{GUCEF_TRACE;

    return -1 != GetTerminalFileDescriptor();
}

/*-------------------------------------------------------------------------*/

int
CTerminalConsoleLogger::GetTerminalFileDescriptor( void )
{GUCEF_TRACE;

    if ( 0 != isatty( STDOUT_FILENO ) )
        return STDOUT_FILENO;
    if ( 0 != isatty( STDERR_FILENO ) )
        return STDERR_FILENO;
    return -1;
}

/*-------------------------------------------------------------------------*/

CTerminalConsoleLogger::CTerminalConsoleLogger( int fileDescriptor )
    : CTextLineConsoleLogger()
    , m_fileDescriptor( fileDescriptor )
    , m_droppedLines( 0 )
    , m_lock()
{GUCEF_TRACE;

    if ( -1 == m_fileDescriptor )
        m_fileDescriptor = GetTerminalFileDescriptor();
}

/*-------------------------------------------------------------------------*/

CTerminalConsoleLogger::~CTerminalConsoleLogger()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

UInt32
CTerminalConsoleLogger::GetDroppedLineCount( void ) const
{GUCEF_TRACE;

    return m_droppedLines;
}

/*-------------------------------------------------------------------------*/

void
CTerminalConsoleLogger::WriteLine( const CString& line )
{GUCEF_TRACE;

    if ( -1 == m_fileDescriptor )
        return;

    CString text = line + "\n";
    const char* data = text.C_String();
    size_t remaining = text.ByteSize() - 1;

    // Lines of different threads must not end up in the middle of each other
    MT::CScopeMutex lock( m_lock );

    while ( remaining > 0 )
    {
        // Only write what the terminal accepts at once, if it does not accept anything it is stalled
        struct pollfd pollInfo;
        pollInfo.fd = m_fileDescriptor;
        pollInfo.events = POLLOUT;
        pollInfo.revents = 0;
        int pollResult = poll( &pollInfo, 1, 0 );
        if ( 1 != pollResult || 0 == ( pollInfo.revents & POLLOUT ) )
        {
            ++m_droppedLines;
            return;
        }

        ssize_t written = write( m_fileDescriptor, data, remaining );
        if ( written < 0 )
        {
            if ( EINTR == errno )
                continue;
            ++m_droppedLines;
            return;
        }
        data += written;
        remaining -= (size_t) written;
    }
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
