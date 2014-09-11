/*
 *  SocketSink: Simple socket app to dump incoming traffic to disk for analysis
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

#ifndef GUCEF_CORE_CGUCEFAPPLICATION_H
#include "CGUCEFApplication.h"
#define GUCEF_CORE_CGUCEFAPPLICATION_H
#endif /* GUCEF_CORE_CGUCEFAPPLICATION_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_DVCPPOSWRAP_H
#include "DVCPPOSWRAP.h"
#define GUCEF_CORE_DVCPPOSWRAP_H
#endif /* GUCEF_CORE_DVCPPOSWRAP_H ? */

#ifndef GUCEF_CORE_CFILEACCESS_H
#include "CFileAccess.h"
#define GUCEF_CORE_CFILEACCESS_H
#endif /* GUCEF_CORE_CFILEACCESS_H ? */

#ifndef GUCEF_CORE_CONFIGSTORE_H
#include "CConfigStore.h"
#define GUCEF_CORE_CONFIGSTORE_H
#endif /* GUCEF_CORE_CONFIGSTORE_H ? */

#ifndef GUCEF_CORE_CTRACER_H
#include "CTracer.h"
#define GUCEF_CORE_CTRACER_H
#endif /* GUCEF_CORE_CTRACER_H ? */

#ifndef GUCEF_CORE_CVALUELIST_H
#include "CValueList.h"
#define GUCEF_CORE_CVALUELIST_H
#endif /* GUCEF_CORE_CVALUELIST_H ? */

#ifndef GUCEF_COMCORE_CUDPSOCKET_H
#include "CUDPSocket.h"
#define GUCEF_COMCORE_CUDPSOCKET_H
#endif /* GUCEF_COMCORE_CUDPSOCKET_H ? */

#ifndef GUCEF_COMCORE_CTCPSERVERSOCKET_H
#include "CTCPServerSocket.h"
#define GUCEF_COMCORE_CTCPSERVERSOCKET_H
#endif /* GUCEF_COMCORE_CTCPSERVERSOCKET_H ? */

#ifndef GUCEF_COMCORE_CCOMCOREGLOBAL_H
#include "gucefCOMCORE_CComCoreGlobal.h"
#define GUCEF_COMCORE_CCOMCOREGLOBAL_H
#endif /* GUCEF_COMCORE_CCOMCOREGLOBAL_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

using namespace GUCEF;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

class SocketSink : public CORE::CObserver
{
    private:

    typedef CORE::CTEventHandlerFunctor< SocketSink > TEventCallback;

    COMCORE::CUDPSocket m_udpSocket;
    COMCORE::CHostAddress m_udpFwdAddr;
    COMCORE::CTCPServerSocket m_tcpServerSocket;
    CORE::CFileAccess m_udpSinkFile;
    CORE::CFileAccess m_tcpSinkFile;
    bool m_doUdpForwarding;
    bool m_udpAddNewLine;
    CORE::Int64 m_flushThreshold;
    CORE::CDynamicBuffer m_udpFileBuffer;

    void
    OnUDPSocketError( CORE::CNotifier* notifier   ,
                      const CORE::CEvent& eventID ,
                      CORE::CICloneable* evenData )
    {GUCEF_TRACE;

        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "SocketSink: UDP Socket experienced an error" );
    }

    void
    OnUDPSocketClosed( CORE::CNotifier* notifier   ,
                       const CORE::CEvent& eventID ,
                       CORE::CICloneable* evenData )
    {GUCEF_TRACE;

        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "SocketSink: UDP Socket has been closed" );
    }

    void
    OnUDPSocketOpened( CORE::CNotifier* notifier   ,
                       const CORE::CEvent& eventID ,
                       CORE::CICloneable* evenData )
    {GUCEF_TRACE;

        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "SocketSink: UDP Socket has been opened" );
    }

    void
    FlushUdpBuffer( void )
    {
        // flush memory buffer
        if ( m_udpSinkFile.IsValid() )
        {
            m_udpSinkFile.Write( m_udpFileBuffer.GetConstBufferPtr(), m_udpFileBuffer.GetDataSize(), 1 );
            m_udpFileBuffer.Clear( true );
            m_udpSinkFile.Flush();
        }
    }

    void
    OnUDPPacketRecieved( CORE::CNotifier* notifier   ,
                         const CORE::CEvent& eventID ,
                         CORE::CICloneable* evenData )
    {GUCEF_TRACE;

        COMCORE::CUDPSocket::UDPPacketRecievedEventData* udpPacketData = static_cast< COMCORE::CUDPSocket::UDPPacketRecievedEventData* >( evenData );
        if ( NULL != udpPacketData )
        {
            const COMCORE::CUDPSocket::TUDPPacketRecievedEventData& data = udpPacketData->GetData();
            const CORE::CDynamicBuffer& udpPacketBuffer = data.dataBuffer.GetData();

            GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "SocketSink: UDP Socket received a packet from " + data.sourceAddress.AddressAndPortAsString() );

            bool writeToFile = true;
            if ( m_flushThreshold > 0 ) 
            {
                if ( m_udpFileBuffer.GetDataSize() < m_flushThreshold ) 
                {
                    // add to in-memory buffer
                    m_udpFileBuffer.Append( udpPacketBuffer.GetConstBufferPtr(), udpPacketBuffer.GetDataSize(), true );
                    if ( m_udpAddNewLine )
                    {
                        static const char newLine = '\n';
                        m_udpFileBuffer.Append( &newLine, 1, true );
                    }
                    writeToFile = false;
                }
                else
                {
                    // flush memory buffer
                    FlushUdpBuffer();
                }
            }

            if ( writeToFile && m_udpSinkFile.IsValid() )
            {
                m_udpSinkFile.Write( udpPacketBuffer.GetConstBufferPtr(), udpPacketBuffer.GetDataSize(), 1 );
                if ( m_udpAddNewLine )
                {
                    static const char newLine = '\n';
                    m_udpSinkFile.Write( &newLine, 1, 1 );
                }
                GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "SocketSink: Wrote UDP packet to file: " + CORE::UInt32ToString( udpPacketBuffer.GetDataSize() ) + " bytes in payload" );
            }

            if ( m_doUdpForwarding )
            {
                m_udpSocket.SendPacketTo( m_udpFwdAddr, udpPacketBuffer.GetConstBufferPtr(), udpPacketBuffer.GetDataSize() );
                GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "SocketSink: Forwarded UDP packet" );
            }
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "SocketSink: UDP Socket has a data received event but no data was provided" );
        }

    }

    void
    OnTCPServerSocketOpened( CORE::CNotifier* notifier   ,
                             const CORE::CEvent& eventID ,
                             CORE::CICloneable* evenData )
    {GUCEF_TRACE;

        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "SocketSink: TCP Server Socket opened" );
    }

    void
    OnTCPServerSocketClosed( CORE::CNotifier* notifier   ,
                             const CORE::CEvent& eventID ,
                             CORE::CICloneable* evenData )
    {GUCEF_TRACE;

        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "SocketSink: TCP Server Socket closed" );
    }

    void
    OnTCPServerSocketError( CORE::CNotifier* notifier   ,
                            const CORE::CEvent& eventID ,
                            CORE::CICloneable* evenData )
    {GUCEF_TRACE;

        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "SocketSink: TCP Server Socket experienced an error" );
    }

    void
    OnTCPServerSocketMaxConnectionsChanged( CORE::CNotifier* notifier   ,
                                            const CORE::CEvent& eventID ,
                                            CORE::CICloneable* evenData )
    {GUCEF_TRACE;

        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "SocketSink: TCP Server Socket changed max client connections" );
    }

    void
    OnTCPServerSocketClientConnected( CORE::CNotifier* notifier   ,
                                      const CORE::CEvent& eventID ,
                                      CORE::CICloneable* evenData )
    {GUCEF_TRACE;

        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "SocketSink: TCP Server Socket has a new client connection" );
    }

    void
    OnTCPServerSocketClientDisconnected( CORE::CNotifier* notifier   ,
                                         const CORE::CEvent& eventID ,
                                         CORE::CICloneable* evenData )
    {GUCEF_TRACE;

        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "SocketSink: TCP Server Socket disconnected a client connection" );
    }

    void
    OnTCPServerSocketClientError( CORE::CNotifier* notifier   ,
                                  const CORE::CEvent& eventID ,
                                  CORE::CICloneable* evenData )
    {GUCEF_TRACE;

        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "SocketSink: TCP Server Socket experienced an error on a client connection" );
    }

    void
    RegisterEventHandlers( void )
    {GUCEF_TRACE;

        // Register UDP socket event handlers
        TEventCallback callback( this, &SocketSink::OnUDPSocketError );
        SubscribeTo( &m_udpSocket                             ,
                     COMCORE::CUDPSocket::UDPSocketErrorEvent ,
                     callback                                 );
        TEventCallback callback2( this, &SocketSink::OnUDPSocketClosed );
        SubscribeTo( &m_udpSocket                              ,
                     COMCORE::CUDPSocket::UDPSocketClosedEvent ,
                     callback2                                 );
        TEventCallback callback3( this, &SocketSink::OnUDPSocketOpened );
        SubscribeTo( &m_udpSocket                              ,
                     COMCORE::CUDPSocket::UDPSocketOpenedEvent ,
                     callback3                                 );
        TEventCallback callback4( this, &SocketSink::OnUDPPacketRecieved );
        SubscribeTo( &m_udpSocket                              ,
                     COMCORE::CUDPSocket::UDPPacketRecievedEvent ,
                     callback4                                 );

        // Register TCP server socket event handlers
        TEventCallback callback5( this, &SocketSink::OnTCPServerSocketOpened );
        SubscribeTo( &m_tcpServerSocket                                 ,
                     COMCORE::CTCPServerSocket::ServerSocketOpenedEvent ,
                     callback5                                          );
        TEventCallback callback6( this, &SocketSink::OnTCPServerSocketClosed );
        SubscribeTo( &m_tcpServerSocket                                 ,
                     COMCORE::CTCPServerSocket::ServerSocketClosedEvent ,
                     callback6                                          );
        TEventCallback callback7( this, &SocketSink::OnTCPServerSocketError );
        SubscribeTo( &m_tcpServerSocket                                ,
                     COMCORE::CTCPServerSocket::ServerSocketErrorEvent ,
                     callback7                                         );
        TEventCallback callback8( this, &SocketSink::OnTCPServerSocketMaxConnectionsChanged );
        SubscribeTo( &m_tcpServerSocket                                                ,
                     COMCORE::CTCPServerSocket::ServerSocketMaxConnectionsChangedEvent ,
                     callback8                                                         );
        TEventCallback callback9( this, &SocketSink::OnTCPServerSocketClientConnected );
        SubscribeTo( &m_tcpServerSocket                              ,
                     COMCORE::CTCPServerSocket::ClientConnectedEvent ,
                     callback9                                       );
        TEventCallback callback10( this, &SocketSink::OnTCPServerSocketClientDisconnected );
        SubscribeTo( &m_tcpServerSocket                                 ,
                     COMCORE::CTCPServerSocket::ClientDisconnectedEvent ,
                     callback10                                         );
        TEventCallback callback11( this, &SocketSink::OnTCPServerSocketClientError );
        SubscribeTo( &m_tcpServerSocket                          ,
                     COMCORE::CTCPServerSocket::ClientErrorEvent ,
                     callback11                                  );

    }

    public:

    virtual const CORE::CString& GetClassTypeName( void ) const
    {GUCEF_TRACE;

        static const CORE::CString classTypeName = "SocketSink";
        return classTypeName;
    }

    bool Setup( const CORE::CValueList& keyValueList )
    {GUCEF_TRACE;

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "SocketSink: Setup starting. " + CORE::UInt32ToString( keyValueList.GetCount() ) + " Config keys were provided" );

        bool enableUdp = false;
        CORE::CString valueStr = keyValueList.GetValueAlways( "EnableUdp" );
        if ( !valueStr.IsNULLOrEmpty() )
        {
            enableUdp = CORE::StringToBool( valueStr );
        }
        bool enableTcp = false;
        valueStr = keyValueList.GetValueAlways( "EnableTcp" );
        if ( !valueStr.IsNULLOrEmpty() )
        {
            enableTcp = CORE::StringToBool( valueStr );
        }

        CORE::Int32 minLogLevel = CORE::LOGLEVEL_BELOW_NORMAL;
        valueStr = keyValueList.GetValueAlways( "MinimalLogLevel" );
        if ( !valueStr.IsNULLOrEmpty() )
        {
            minLogLevel = CORE::StringToInt32( valueStr );
            CORE::CCoreGlobal::Instance()->GetLogManager().SetMinLogLevel( minLogLevel );
        }

        if ( enableUdp )
        {
            CORE::CString udpOutputFilePath = CORE::RelativePath( keyValueList.GetValueAlways( "UdpOutputFile" ) );
            if ( !udpOutputFilePath.IsNULLOrEmpty() && !m_udpSinkFile.Open( udpOutputFilePath, "wb" ) )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "SocketSink: Failed to open udp output file at: " + udpOutputFilePath );
            }

            valueStr = keyValueList.GetValueAlways( "UdpBufferSize" );
            if ( !valueStr.IsNULLOrEmpty() )
            {
                m_udpSocket.SetRecievedDataBufferSize( CORE::StringToUInt32( valueStr ) );
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "SocketSink: Set UDP buffer size as " + valueStr + ". For non-payload-delimited packets this should match the packet size" );
            }

            m_udpAddNewLine = false;
            valueStr = keyValueList.GetValueAlways( "AddNewLineAfterUdpData" );
            if ( !valueStr.IsNULLOrEmpty() )
            {
                m_udpAddNewLine = CORE::StringToBool( valueStr );
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "SocketSink: Add new line is true for UDP: Make sure you are sending text!" );
            }

            m_flushThreshold = -1;
            valueStr = keyValueList.GetValueAlways( "UdpFlushThreshold" );
            if ( !valueStr.IsNULLOrEmpty() )
            {
                m_flushThreshold = CORE::StringToInt64( valueStr );
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "SocketSink: Set UDP flush threshold to " + valueStr );
            }

            m_udpFileBuffer.SetAutoEnlarge( true );
            valueStr = keyValueList.GetValueAlways( "UdpFileBufferSize" );
            if ( !valueStr.IsNULLOrEmpty() )
            {
                m_udpFileBuffer.SetBufferSize( CORE::StringToUInt32( valueStr ) );
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "SocketSink: Set UDP file buffer size to: " + valueStr );
            }

            valueStr = keyValueList.GetValueAlways( "UdpPort" );
            if ( !valueStr.IsNULLOrEmpty() )
            {
                if ( m_udpSocket.Open( CORE::StringToUInt16( valueStr ) ) )
                {
                    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "SocketSink: Opened UDP port " + valueStr );
                }
                else
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "SocketSink: Failed to open udp port " + valueStr );
                }
            }

            m_doUdpForwarding = false;
            valueStr = keyValueList.GetValueAlways( "DoUdpForwarding" );
            if ( !valueStr.IsNULLOrEmpty() )
            {
                m_doUdpForwarding = CORE::StringToBool( valueStr );
            }

            CORE::UInt16 fwdPort = CORE::StringToUInt16( keyValueList.GetValueAlways( "UdpFwdPort" ) );
            m_udpFwdAddr.ResolveDNS( keyValueList.GetValueAlways( "UdpFwdAddr" ), fwdPort );
        }

        if ( enableTcp )
        {
            CORE::CString tcpOutputFilePath = CORE::RelativePath( keyValueList.GetValueAlways( "TcpOutputFile" ) );
            if ( !tcpOutputFilePath.IsNULLOrEmpty() && !m_tcpSinkFile.Open( tcpOutputFilePath, "wb" ) )
            {
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "SocketSink: Failed to open udp output file at: " + tcpOutputFilePath );
            }

            CORE::CString tcpPortStr = keyValueList.GetValueAlways( "TcpPort" );
            if ( !tcpPortStr.IsNULLOrEmpty() )
            {
                if ( m_tcpServerSocket.ListenOnPort( CORE::StringToUInt16( tcpPortStr ) ) )
                {
                    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "SocketSink: Opened TCP server port " + tcpPortStr );
                }
                else
                {
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "SocketSink: Failed to open tcp server port " + tcpPortStr );
                }
            }
        }

        return m_udpSocket.IsActive() || m_tcpServerSocket.IsActive();
    }

    SocketSink()
        : CORE::CObserver()          ,
          m_udpSocket( false )       ,
          m_tcpServerSocket( false ) ,
          m_doUdpForwarding( false )
    {GUCEF_TRACE;

        RegisterEventHandlers();
    }

    virtual ~SocketSink()
    {GUCEF_TRACE;

        m_udpSocket.Close( true );
        m_tcpServerSocket.Close();

        FlushUdpBuffer();
    }
};

/*-------------------------------------------------------------------------*/

bool
LoadConfig( CORE::CValueList& keyValueList )
{GUCEF_TRACE;

    #ifdef GUCEF_DEBUG_MODE
    const CORE::CString configFile = "SocketSink_d.ini";
    #else
    const CORE::CString configFile = "SocketSink.ini";
    #endif

    CORE::CString configFilePath = CORE::CombinePath( "$CURWORKDIR$", configFile );
    configFilePath = CORE::RelativePath( configFilePath );

    if ( !CORE::FileExists( configFilePath ) )
    {
        configFilePath = CORE::CombinePath( "$MODULEDIR$", configFile );
        configFilePath = CORE::RelativePath( configFilePath );

        if ( !FileExists( configFilePath ) )
        {
            return false;
        }
    }
    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Located config file @ " + configFilePath );

    keyValueList.SetConfigNamespace( "Main/AppArgs" );
    keyValueList.SetUseGlobalConfig( true );
    keyValueList.SetAllowDuplicates( false );
    keyValueList.SetAllowMultipleValues( true );

    CORE::CConfigStore& configStore = CORE::CCoreGlobal::Instance()->GetConfigStore();
    configStore.SetConfigFile( configFilePath );
    return configStore.LoadConfig();
}

/*-------------------------------------------------------------------------*/

void
ParseParams( const int argc                 ,
             char* argv[]                   ,
             CORE::CValueList& keyValueList )
{GUCEF_TRACE;

    GUCEF::CORE::CString argString;
    if ( argc > 0 )
    {
        argString = *argv;

        // Combine the argument strings back into a single string because we don't want to use
        // a space as the seperator
        for ( int i=1; i<argc; ++i )
        {
            argString += ' ' + CORE::CString( argv[ i ] );
        }

        // Parse the param list based on the ' symbol
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Application parameters: " + argString );
        keyValueList.SetMultiple( argString, '*' );
    }
}

/*-------------------------------------------------------------------------*/

/*
 *      Application entry point
 */
GUCEF_OSMAIN_BEGIN
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "This tool was compiled on: " __DATE__ " @ " __TIME__ );

    CORE::CCoreGlobal::Instance();
    COMCORE::CComCoreGlobal::Instance();

    // Load settings from a config file (if any) and then override with params (if any)
    CORE::CValueList keyValueList;
    LoadConfig( keyValueList );
    ParseParams( argc, argv, keyValueList );

    CORE::CString outputDir = keyValueList.GetValueAlways( "outputDir" );
    if ( outputDir.IsNULLOrEmpty() )
    {
        outputDir = CORE::RelativePath( "$CURWORKDIR$" );
    }
    CORE::Create_Directory( CORE::RelativePath( outputDir ).C_String() );

    CORE::CString logFilename = CORE::CombinePath( outputDir, "SocketSink_Log.txt" );

    keyValueList.Set( "logfile", logFilename );

    CORE::CFileAccess logFileAccess( logFilename, "w" );
    CORE::CStdLogger logger( logFileAccess );
    CORE::CCoreGlobal::Instance()->GetLogManager().AddLogger( &logger );

    CORE::CPlatformNativeConsoleLogger console;
    CORE::CCoreGlobal::Instance()->GetLogManager().AddLogger( console.GetLogger() );

    CORE::CCoreGlobal::Instance()->GetLogManager().FlushBootstrapLogEntriesToLogs();

    SocketSink socketSink;
    if ( socketSink.Setup( keyValueList ) )
    {
        return CORE::CCoreGlobal::Instance()->GetApplication().main( argc, argv, true );
    }
    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "SocketSink: Exiting because setup failed" );
    return -1;
}
GUCEF_OSMAIN_END

/*---------------------------------------------------------------------------*/
