/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
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

#include <list>
#include "gucefCOMCORE_CWin32SerialPort.h"

#if GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#include <windows.h>

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

unsigned long 
Win32GetOSVersion()
{
	OSVERSIONINFO osvi;
	memset( &osvi, 0, sizeof(OSVERSIONINFO) );
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	
	// get running os
	::GetVersionEx( &osvi );

	return osvi.dwPlatformId;
}

/*-------------------------------------------------------------------------*/

void 
Win32EnumPorts( std::list<Int32>& portlist )
{
	portlist.clear();

	// use querydosdevice on NT/2000/xp (no need to open ports...)
	if ( VER_PLATFORM_WIN32_NT == Win32GetOSVersion() )
	{
		const int bufferSize = 65535;        
        CORE::CDynamicBuffer deviceNameBuffer( bufferSize, true );

        // Read the list of devices into the buffer
		Int32 len = ::QueryDosDevice( 0, deviceNameBuffer.AsTypePtr< char >(), bufferSize );
		for ( Int32 n=0; n<len; ++n )
		{
			// if found "COM", then add number to list
			if ( 0 == _stricmp( deviceNameBuffer.AsConstTypePtr< char >( n ), "COM" ) )
            {
                portlist.push_back( atoi( deviceNameBuffer.AsConstTypePtr< char >( n+3 ) ) );
            }

			// find next null pointer
			while ( deviceNameBuffer.AsConstType< char >( n ) != '\0' )
            {
				++n;
            }
		}
	}
	
	// else, open port 1-255
	else
	{
		CORE::CString basePath( "\\\\.\\COM" );
        for ( Int32 i=1; i<255; ++i )
		{
			CORE::CString portPath = basePath + CORE::Int32ToString( i );

			// try to open port
			bool result = false;			
			::HANDLE handle = ::CreateFile( portPath.C_String(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0 );
			
			if ( INVALID_HANDLE_VALUE == handle ) continue;
			::CloseHandle( handle );
  		
			portlist.push_back( i );
		}
	}
}

/*-------------------------------------------------------------------------*/

void
CWin32SerialPort::PopulatePortList( TStringList& portList )
{
    std::list< Int32 > portIndexList;
    Win32EnumPorts( portIndexList );

    std::list< Int32 >::iterator i = portIndexList.begin();
    while ( i != portIndexList.end() )
    {
        portList.push_back( "COM" + CORE::Int32ToString( (*i) ) );
        ++i;
    }
}

/*-------------------------------------------------------------------------*/

CWin32SerialPort::CWin32SerialPort( void )
    : CSerialPort()                        ,
      m_portHandle( INVALID_HANDLE_VALUE )
{GUCEF_TRACE;
          
}

/*-------------------------------------------------------------------------*/
    
CWin32SerialPort::~CWin32SerialPort()
{GUCEF_TRACE;

    Close();
}

/*-------------------------------------------------------------------------*/

CWin32SerialPort*
CWin32SerialPort::Create( const CORE::CString& portName )
{GUCEF_TRACE;

    return 0;
}

/*-------------------------------------------------------------------------*/
    
CORE::CString
CWin32SerialPort::GetPortType( void ) const
{GUCEF_TRACE;

    return "serial";
}

/*-------------------------------------------------------------------------*/

const CORE::CString&
CWin32SerialPort::GetClassTypeName( void ) const
{GUCEF_TRACE;

    static CORE::CString classTypeName = "GUCEF::COMCORE::CWin32SerialPort";
    return classTypeName;
}

/*-------------------------------------------------------------------------*/

bool
CWin32SerialPort::IsOpen( void ) const
{GUCEF_TRACE;

    return INVALID_HANDLE_VALUE != m_portHandle;
}

/*-------------------------------------------------------------------------*/

bool
CWin32SerialPort::Open( void )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

void
CWin32SerialPort::Close( void )
{GUCEF_TRACE;

    if( INVALID_HANDLE_VALUE != m_portHandle )
    {
        ::CloseHandle( m_portHandle );
        m_portHandle = INVALID_HANDLE_VALUE;
    }
}

/*-------------------------------------------------------------------------*/

UInt8
CWin32SerialPort::ReadByte( void )
{GUCEF_TRACE;

    return 0;
}

/*-------------------------------------------------------------------------*/

char
CWin32SerialPort::ReadChar( void )
{GUCEF_TRACE;

    return 0;
}

/*-------------------------------------------------------------------------*/

CORE::CString
CWin32SerialPort::ReadString( const Int32 maxStringLength )
{GUCEF_TRACE;

    return CORE::CString();
}

/*-------------------------------------------------------------------------*/

CORE::CString
CWin32SerialPort::ReadLine( const Int32 maxStringLength )
{GUCEF_TRACE;

    return CORE::CString();
}

/*-------------------------------------------------------------------------*/

UInt32
CWin32SerialPort::ReadBytesTo( CORE::CDynamicBuffer& buffer ,
                               const UInt32 elementSize     ,
                               const UInt32 elementsToRead  )
{GUCEF_TRACE;

    return 0;
}

/*-------------------------------------------------------------------------*/

UInt32
CWin32SerialPort::GetDataSizeInBytesInReceiveBuffer( void )
{GUCEF_TRACE;

    return 0;
}

/*-------------------------------------------------------------------------*/

void
CWin32SerialPort::SetReadTimeout( const UInt32 timeOutInMilliSecs )
{GUCEF_TRACE;

    if ( NULL != m_portHandle )
    {
        ::COMMTIMEOUTS commTimeouts;
        if( ::GetCommTimeouts( m_portHandle, &commTimeouts ) )
        {
            commTimeouts.ReadIntervalTimeout        = timeOutInMilliSecs;
            commTimeouts.ReadTotalTimeoutConstant   = timeOutInMilliSecs;
            commTimeouts.ReadTotalTimeoutMultiplier = timeOutInMilliSecs;

            SetCommTimeouts( m_portHandle, &commTimeouts );
        }
    }
}

/*-------------------------------------------------------------------------*/

UInt32
CWin32SerialPort::GetReadTimeout( void ) const
{GUCEF_TRACE;

    if ( NULL != m_portHandle )
    {
        ::COMMTIMEOUTS commTimeouts;
        if( ::GetCommTimeouts( m_portHandle, &commTimeouts ) )
        {
            return (UInt32) commTimeouts.ReadIntervalTimeout;
        }
    }
    return 0;
}

/*-------------------------------------------------------------------------*/
    
void
CWin32SerialPort::SetWriteTimeout( const UInt32 timeOutInMilliSecs )
{GUCEF_TRACE;

    if ( NULL != m_portHandle )
    {
        ::COMMTIMEOUTS commTimeouts;
        if( ::GetCommTimeouts( m_portHandle, &commTimeouts ) )
        {
            commTimeouts.WriteTotalTimeoutConstant   = timeOutInMilliSecs;
            commTimeouts.WriteTotalTimeoutMultiplier = timeOutInMilliSecs;

            SetCommTimeouts( m_portHandle, &commTimeouts );
        }
    }
}

/*-------------------------------------------------------------------------*/

UInt32
CWin32SerialPort::GetWriteTimeout( void ) const
{GUCEF_TRACE;

    if ( NULL != m_portHandle )
    {
        ::COMMTIMEOUTS commTimeouts;
        if( ::GetCommTimeouts( m_portHandle, &commTimeouts ) )
        {
            return (UInt32) commTimeouts.WriteTotalTimeoutConstant;
        }
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

bool
CWin32SerialPort::SetBaudRate( const UInt32 baudRate )
{GUCEF_TRACE;

    return false;
}

/*-------------------------------------------------------------------------*/

UInt32
CWin32SerialPort::GetBaudRate( void ) const
{GUCEF_TRACE;

    return 0;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_PLATFORM == GUCEF_PLATFORM_MSWIN ? */
