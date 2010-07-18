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

#include <assert.h>

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_CIOACCESS_H
#include "CIOAccess.h"
#define GUCEF_CORE_CIOACCESS_H
#endif /* GUCEF_CORE_CIOACCESS_H ? */

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#ifdef GUCEF_LINUX_BUILD

#define _XOPEN_SOURCE
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
//#include <util.h>    // for platforms that don't have pty.h
//#include <libutil.h> // for platforms that don't have pty.h
//#include <pty.h>



#endif /* GUCEF_LINUX_BUILD ? */

#include "CXTermConsoleLogger.h"

#ifdef GUCEF_LINUX_BUILD

#include "gucefCORE_pty.h"

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

CXTermConsoleLogger::CXTermConsoleLogger( bool initialize )
    : CILogger()                                 ,
      m_minimalLogLevel( LOGLEVEL_BELOW_NORMAL ) ,
      m_xtermpid( -1 )                           ,
      m_masterfd( -1 )                           ,
      m_slavefd( -1 )                            ,
      m_slaveFptr( NULL )
{GUCEF_TRACE;

    if ( initialize )
    {
        Initialize();
    }
}

/*-------------------------------------------------------------------------*/

CXTermConsoleLogger::~CXTermConsoleLogger()
{GUCEF_TRACE;

    if ( m_masterfd != -1 )
    {
        close( m_masterfd );
        m_masterfd = -1;
    }
    if ( m_slavefd != -1 )
    {
        close( m_slavefd );
        m_slavefd = -1;
    }
    if ( NULL != m_slaveFptr )
    {
        fclose( m_slaveFptr );
        m_slaveFptr = NULL;
    }
}

/*-------------------------------------------------------------------------*/

bool
CXTermConsoleLogger::Initialize( void )
{GUCEF_TRACE;

    if ( ( m_masterfd != -1 ) || ( m_slavefd != -1 ) )
    {
        // Do not initialize twice, already initialized
        return true;
    }

    if ( 0 != GUCEF_pty_open( &m_masterfd, &m_slavefd ) )
    {
        // some error occured opening the pseudo terminal
        m_masterfd = -1;
        m_slavefd = -1;
        GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "CXTermConsoleLogger: failed to open pseudo terminal master and slave" );
        return false;
    }

    m_slaveFptr = fdopen( m_slavefd, "w" );
    if ( NULL == m_slaveFptr )
    {
        close( m_masterfd );
        m_masterfd = -1;
        close( m_slavefd );
        m_slavefd = -1;
        GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "CXTermConsoleLogger: failed to open file stream based on slave file descriptor" );
        return false;
    }
    return true;

//    if ( ( m_masterfd != -1 ) || ( m_slavefd != -1 ) )
//    {
//        // Do not initialize twice, already initialized
//        return true;
//    }
//
//    // try to open master for pseudo terminal
//    m_masterfd = ptym_open( ptsname,  );
//    if ( 0 > m_masterfd )
//    {
//        // error opening pseudo terminal
//        GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "CXTermConsoleLogger: ptym_open: failed to open pseudo terminal" );
//        return false;
//    }
//
//    // try to open slave for pseudo terminal
//    m_slavefd = ptys_open( m_masterfd, m_ptsname );
//
//    // HERE: do any termios reconfiguration which is required
//    // If you don't want the windowid to be echoed, you must
//    // at least switch off echo here.
//
//    // Fire up an xterm on the master side of the pseudo tty
//    sprintf( sopt, "-Sxx%d", m_masterfd );
//    fflush( stderr );
//    fflush( stdout );
//    m_xtermpid = fork();
//
//    switch( m_xtermpid )
//    {
//        case -1:
//        {
//            close( m_slavefd );
//            m_slavefd = -1;
//            close( m_masterfd );
//            m_masterfd = -1;
//
//            GUCEF_ERROR_LOG( LOGLEVEL_NORMAL, "CXTermConsoleLogger: failed fork a xterm" );
//            return false;
//        }
//        case 0:
//        {
//            for (i=0; i<100; i++)
//            {
//                if ( i != STDERR_FILENO && i != m_masterfd )
//                {
//                    close(i);
//                }
//            }
//            execl("/usr/openwin/bin/xterm", "xterm", sopt, NULL);
//            perror("/usr/openwin/bin/xterm");
//            return false;
//        }
//        default:
//        {
//            close( m_masterfd );
//            fprintf( stderr, "starting xterm (pid %d) on %s\n", m_xtermpid, pts_name );
//
//            // Discard windowid, which comes back as first line from xterm
//            // (this over-simple bit of code isn't really good enough...)
//            if ( read( m_slavefd, pts_name, sizeof(pts_name) - 1 ) < 0 )
//            {
//                perror( "read slavefd" );
//                return false;
//            }
//        }
//    }
//
//    // now do i/o on slavefd to read/write the xterm
//    return true;

}

/*-------------------------------------------------------------------------*/

void
CXTermConsoleLogger::Log( const TLogMsgType logMsgType ,
                          const Int32 logLevel         ,
                          const CString& logMessage    ,
                          const UInt32 threadId        )
{GUCEF_TRACE;

    if ( m_slavefd != -1 )
    {
        if ( logLevel >= m_minimalLogLevel )
        {
            CString actualLogMsg( "[THREAD=" + UInt32ToString( threadId ) +
                 "] [TYPE=" + CLogManager::GetLogMsgTypeString( logMsgType ) +
                 "] [LVL=" + LogLevelToString( logLevel ) + "] " +
                 logMessage + "\n" );

            fprintf( m_slaveFptr, actualLogMsg.C_String() );
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CXTermConsoleLogger::FlushLog( void )
{GUCEF_TRACE;

    if ( m_slavefd != -1 )
    {
        fflush( m_slaveFptr );
    }
}

/*-------------------------------------------------------------------------*/

void
CXTermConsoleLogger::SetMinimalLogLevel( const Int32 minimalLogLevel )
{GUCEF_TRACE;

    m_minimalLogLevel = minimalLogLevel;
}

/*-------------------------------------------------------------------------*/

Int32
CXTermConsoleLogger::GetMinimalLogLevel( void ) const
{GUCEF_TRACE;

    return m_minimalLogLevel;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
#endif /* GUCEF_LINUX_BUILD ? */
