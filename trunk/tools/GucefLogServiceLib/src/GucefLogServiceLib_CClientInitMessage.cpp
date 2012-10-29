/*
 *  GucefLogServiceLib: Library containing the main logic for the Logging service
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "GucefLogServiceLib_CClientInitMessage.h"

#ifndef GUCEF_LOGSERVICELIB_PROTOCOL_H
#include "GucefLogServiceLib_protocol.h"
#define GUCEF_LOGSERVICELIB_PROTOCOL_H
#endif /* GUCEF_LOGSERVICELIB_PROTOCOL_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace LOGSERVICELIB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

CClientInitMessage::CClientInitMessage( void )
    : m_processName(),
      m_processId(),
      m_appName(),
      m_clientVersion()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CClientInitMessage::CClientInitMessage( const CClientInitMessage& src )
    : m_processName( src.m_processName ),
      m_processId( src.m_processId ),
      m_appName( src.m_appName ),
      m_clientVersion( src.m_clientVersion )
{GUCEF_TRACE;
}

/*-------------------------------------------------------------------------*/

CClientInitMessage::~CClientInitMessage()
{GUCEF_TRACE;
}

/*-------------------------------------------------------------------------*/

CORE::UInt32
CClientInitMessage::GetStreamedSize( void ) const
{GUCEF_TRACE;

    return m_processName.Length()   +
           m_processId.Length()     +
           m_appName.Length()       +
           m_clientVersion.Length() +
           22;   // 1 + 4 for msg size field + 1 for msg type + str lengths + 4 bytes for size per str
}

/*-------------------------------------------------------------------------*/

bool
CClientInitMessage::WriteToBuffer( CORE::CDynamicBuffer& outputBuffer ,
                                   bool writeMsgHeader                ) const
{GUCEF_TRACE;
    
    const CORE::UInt8 delimiter = LOGSVCMSGTYPE_DELIMITER;
    const CORE::UInt8 msgType = LOGSVCMSGTYPE_CLIENTINFO;

    CORE::UInt32 strLength = 0;
    CORE::UInt32 offset = 0;
    bool error = false;

    // Construct the message
    if ( writeMsgHeader )
    {
        error = 1 != outputBuffer.CopyFrom( offset, 1, &delimiter );
        if ( error ) return false;
        offset += 5; // <- we will offset an extra 4 bytes here to reserve room for the message length
    }

    error = 1 != outputBuffer.CopyFrom( offset, 1, &msgType );
    if ( error ) return false;
    offset += 1;
    strLength = m_clientVersion.Length();
    error = 4 != outputBuffer.CopyFrom( offset, 4, &strLength );
    if ( error ) return false;
    offset += 4;
    error = strLength != outputBuffer.CopyFrom( offset, strLength, m_clientVersion.C_String() );
    if ( error ) return false;
    offset += strLength;
    strLength = m_appName.Length();
    error = 4 != outputBuffer.CopyFrom( offset, 4, &strLength );
    if ( error ) return false;
    offset += 4;
    error = strLength != outputBuffer.CopyFrom( offset, strLength, m_appName.C_String() );
    if ( error ) return false;
    offset += strLength;
    strLength = m_processName.Length();
    error = 4 != outputBuffer.CopyFrom( offset, 4, &strLength );
    if ( error ) return false;
    offset += 4;
    error = strLength != outputBuffer.CopyFrom( offset, strLength, m_processName.C_String() );
    if ( error ) return false;
    offset += strLength;
    strLength = m_processId.Length();
    error = 4 != outputBuffer.CopyFrom( offset, 4, &strLength );
    if ( error ) return false;
    offset += 4;
    error = strLength != outputBuffer.CopyFrom( offset, strLength, m_processId.C_String() );
    if ( error ) return false;
    offset += strLength;

    if ( writeMsgHeader )
    {
        // Now that we contructed the whole message write the message length itself into
        // its reserved slot
        CORE::UInt32 msgSize = offset - 5;
        error = 4 != outputBuffer.CopyFrom( 1, 4, &msgSize );
        if ( error ) return false;
    }
    return true;
}

/*-------------------------------------------------------------------------*/

bool
CClientInitMessage::ReadFromBuffer( const CORE::CDynamicBuffer& sourceBuffer ,
                                    bool hasMsgheader                        )
{GUCEF_TRACE;

    try
    {
        // Parse the client info from the message
        CORE::UInt32 offset = 0;
        if ( hasMsgheader )
        {
            // We just skip the message header, we dont use it here
            // The header is a delimiter + message size field
            offset += 5;
        }

        // Read the message type, this is a simple sanity check on the data
        CORE::UInt8 msgType = sourceBuffer.AsConstType< CORE::UInt8 >( offset );
        ++offset;
        if ( msgType != LOGSVCMSGTYPE_CLIENTINFO ) return false;

        // Set log client version number
        CORE::UInt32 strLength = sourceBuffer.AsConstType< CORE::UInt32 >( offset );
        offset += 4;
        m_clientVersion.Set( sourceBuffer.AsConstTypePtr< char >( offset, strLength ), strLength );
        offset += strLength;

        // Set the application name
        strLength = sourceBuffer.AsConstType< CORE::UInt32 >( offset );
        offset += 4;
        m_appName.Set( sourceBuffer.AsConstTypePtr< char >( offset, strLength ), strLength );
        offset += strLength;

        // Set the process name
        strLength = sourceBuffer.AsConstType< CORE::UInt32 >( offset );
        offset += 4;
        m_processName.Set( sourceBuffer.AsConstTypePtr< char >( offset, strLength ), strLength );
        offset += strLength;

        // Set the process Id
        strLength = sourceBuffer.AsConstType< CORE::UInt32 >( offset );
        offset += 4;
        m_processId.Set( sourceBuffer.AsConstTypePtr< char >( offset, strLength ), strLength );

        return true;
    }
    catch ( CORE::CDynamicBuffer::EIllegalCast& )
    {
        // Something is wrong with the source buffer
        return false;
    }
}

/*-------------------------------------------------------------------------*/

void
CClientInitMessage::SetProcessName( const CORE::CString& processName )
{GUCEF_TRACE;

    m_processName = processName;
}

/*-------------------------------------------------------------------------*/
    
const CORE::CString&
CClientInitMessage::GetProcessName( void ) const
{GUCEF_TRACE;

    return m_processName;
}

/*-------------------------------------------------------------------------*/

void
CClientInitMessage::SetProcessId( const CORE::CString& processId )
{GUCEF_TRACE;

    m_processId = processId;
}
    
/*-------------------------------------------------------------------------*/

const CORE::CString&
CClientInitMessage::GetProcessId( void ) const
{GUCEF_TRACE;

    return m_processId;
}

/*-------------------------------------------------------------------------*/

void
CClientInitMessage::SetApplicationName( const CORE::CString& appName )
{GUCEF_TRACE;

    m_appName = appName;
}

/*-------------------------------------------------------------------------*/
    
const CORE::CString&
CClientInitMessage::GetApplicationName( void ) const
{GUCEF_TRACE;

    return m_appName;
}

/*-------------------------------------------------------------------------*/

void
CClientInitMessage::SetClientVersion( const CORE::CString& clientVersion )
{GUCEF_TRACE;

    m_clientVersion = clientVersion; 
}

/*-------------------------------------------------------------------------*/
    
const CORE::CString&
CClientInitMessage::GetClientVersion( void ) const
{GUCEF_TRACE;

    return m_clientVersion;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace LOGSERVICELIB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
