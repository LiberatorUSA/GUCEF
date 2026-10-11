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

#ifndef GUCEF_CORE_CTERMINALCONSOLELOGGER_H
#define GUCEF_CORE_CTERMINALCONSOLELOGGER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CMUTEX_H
#include "gucefMT_CMutex.h"
#define GUCEF_MT_CMUTEX_H
#endif /* GUCEF_MT_CMUTEX_H ? */

#ifndef GUCEF_CORE_CTEXTLINECONSOLELOGGER_H
#include "gucefCORE_CTextLineConsoleLogger.h"
#define GUCEF_CORE_CTEXTLINECONSOLELOGGER_H
#endif /* GUCEF_CORE_CTEXTLINECONSOLELOGGER_H ? */

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

/**
 *  Console logger for an application that is run from a terminal: the log lines go to the terminal that
 *  stdout or stderr is connected to. A terminal that cannot take any more data at the moment, for example
 *  because it was paused with ctrl-s, makes the logger drop lines instead of blocking the application.
 */
class GUCEF_CORE_PUBLIC_CPP CTerminalConsoleLogger : public CTextLineConsoleLogger
{
    public:

    /** @return true if stdout or stderr is connected to a terminal */
    static bool IsRunningInTerminal( void );

    /** @return the file descriptor of the terminal, stdout preferred, or -1 if there is none */
    static int GetTerminalFileDescriptor( void );

    /**
     *  @param fileDescriptor   where to write to. -1 picks the terminal as per GetTerminalFileDescriptor().
     *                          The file descriptor is not owned and stays open
     */
    CTerminalConsoleLogger( int fileDescriptor = -1 );

    virtual ~CTerminalConsoleLogger() GUCEF_VIRTUAL_OVERRIDE;

    /** Number of lines that were dropped because the terminal was not ready for them */
    UInt32 GetDroppedLineCount( void ) const;

    protected:

    virtual void WriteLine( const CString& line ) GUCEF_VIRTUAL_OVERRIDE;

    private:

    CTerminalConsoleLogger& operator=( const CTerminalConsoleLogger& src );
    CTerminalConsoleLogger( const CTerminalConsoleLogger& src );

    private:

    int m_fileDescriptor;
    UInt32 m_droppedLines;
    MT::CMutex m_lock;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTERMINALCONSOLELOGGER_H ? */
