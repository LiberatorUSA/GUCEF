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

#ifndef GUCEF_LOGSERVICELIB_CCLIENTINITMESSAGE_H
#define GUCEF_LOGSERVICELIB_CCLIENTINITMESSAGE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_LOGSERVICELIB_MACROS_H
#include "GucefLogServiceLib_macros.h"
#define GUCEF_LOGSERVICELIB_MACROS_H
#endif /* GUCEF_LOGSERVICELIB_MACROS_H ? */

#ifndef GUCEF_CORE_CDYNAMICBUFFER_H
#include "CDynamicBuffer.h"
#define GUCEF_CORE_CDYNAMICBUFFER_H
#endif /* GUCEF_CORE_CDYNAMICBUFFER_H ? */

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

class GUCEF_LOGSERVICELIB_EXPORT_CPP CClientInitMessage
{
    public:

    CClientInitMessage( void );

    CClientInitMessage( const CClientInitMessage& src );

    ~CClientInitMessage();

    bool WriteToBuffer( CORE::CDynamicBuffer& outputBuffer ,
                        bool writeMsgHeader = true         ) const;

    bool ReadFromBuffer( const CORE::CDynamicBuffer& sourceBuffer ,
                         bool hasMsgheader = true                 );

    void SetProcessName( const CORE::CString& processName );
    
    const CORE::CString& GetProcessName( void ) const;

    void SetProcessId( const CORE::CString& processId );
    
    const CORE::CString& GetProcessId( void ) const;

    void SetApplicationName( const CORE::CString& appName );
    
    const CORE::CString& GetApplicationName( void ) const;

    void SetClientVersion( const CORE::CString& clientVersion );
    
    const CORE::CString& GetClientVersion( void ) const;

    CORE::UInt32 GetStreamedSize( void ) const;

    private:

    CORE::CString m_processName;
    CORE::CString m_processId;
    CORE::CString m_appName;
    CORE::CString m_clientVersion;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace LOGSERVICELIB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_LOGSERVICELIB_CCLIENTINITMESSAGE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 04-05-2005 :
        - Dinand: Initial version.

---------------------------------------------------------------------------*/
