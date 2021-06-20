/*
 *  pubsubpluginMSMQ: Generic GUCEF COMCORE plugin for providing pubsub via Microsoft MSMQ
 *
 *  Copyright (C) 1998 - 2020.  Dinand Vanvelzen
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

#include <Objbase.h>
#include <Mq.h>

#include <string>

#ifndef GUCEF_MT_CSCOPEMUTEX_H
#include "gucefMT_CScopeMutex.h"
#define GUCEF_MT_CSCOPEMUTEX_H
#endif /* GUCEF_MT_CSCOPEMUTEX_H ? */

#ifndef GUCEF_CORE_DVOSWRAP_H
#include "DVOSWRAP.h"
#define GUCEF_CORE_DVOSWRAP_H
#endif /* GUCEF_CORE_DVOSWRAP_H */

#ifndef GUCEF_CORE_DVCPPSTRINGUTILS_H
#include "dvcppstringutils.h"
#define GUCEF_CORE_DVCPPSTRINGUTILS_H
#endif /* GUCEF_CORE_DVCPPSTRINGUTILS_H */

#ifndef GUCEF_CORE_METRICSMACROS_H
#include "gucefCORE_MetricsMacros.h"
#define GUCEF_CORE_METRICSMACROS_H
#endif /* GUCEF_CORE_METRICSMACROS_H ? */

#ifndef GUCEF_CORE_CTASKMANAGER_H
#include "gucefCORE_CTaskManager.h"
#define GUCEF_CORE_CTASKMANAGER_H
#endif /* GUCEF_CORE_CTASKMANAGER_H */

#ifndef PUBSUBPLUGIN_MSMQ_CMSMQPUBSUBCLIENT_H
#include "pubsubpluginMSMQ_CMsmqPubSubClient.h"
#define PUBSUBPLUGIN_MSMQ_CMSMQPUBSUBCLIENT_H
#endif /* PUBSUBPLUGIN_MSMQ_CMSMQPUBSUBCLIENT_H ? */

#include "pubsubpluginMSMQ_CMsmqPubSubClientTopic.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace PUBSUBPLUGIN {
namespace MSMQ {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CMsmqPubSubClientTopic::CMsmqPubSubClientTopic( CMsmqPubSubClient* client )
    : COMCORE::CPubSubClientTopic()
    , m_client( client )
    , m_config()
    , m_syncReadTimer()
    , m_msmqMsgsQueued( -1 )
    , m_lock()
    , m_receiveQueueHandle( GUCEF_NULL )
{GUCEF_TRACE;
        
    RegisterEventHandlers();
}

/*-------------------------------------------------------------------------*/

CMsmqPubSubClientTopic::~CMsmqPubSubClientTopic()
{GUCEF_TRACE;

    Disconnect();
}


/*-------------------------------------------------------------------------*/

void
CMsmqPubSubClientTopic::RegisterEventHandlers( void )
{GUCEF_TRACE;

}
//
//bool CMSMQWrapper::ReadQueue(const WCHAR* wczQueueName,  // [i]
//                             BYTE*        pBuf        ,  // [i]
//                             size_t       nBufLen     ,  // [i]
//                             int&         nBytesRead   ) // [o]
//{
//    // set value of ReceiveTimout parameter
//    _variant_t vtReceiveTimeout;
//    vtReceiveTimeout= (long)INFINITE;
//
//    try
//    {
//        IMSMQQueueInfoPtr qinfo("MSMQ.MSMQQueueInfo");
//        qinfo->PathName= wczQueueName;
//
//        IMSMQQueuePtr qRec;
//        qRec= qinfo->Open(MQ_RECEIVE_ACCESS, MQ_DENY_NONE); // open queue to retrieve message
//
//        // retrieve messages from queue
//        IMSMQMessagePtr msgRec("MSMQ.MSMQMessage");
//        msgRec= qRec->Receive(&vtMissing, &vtMissing, &vtMissing, &vtReceiveTimeout);
//        if (NULL == msgRec)
//        {
//            nBytesRead= 0; // there are no messages in the queue
//            return true;
//        }
//
//        nBytesRead           = msgRec->BodyLength;
//        _variant_t recVariant= msgRec->Body      ;
//
//        // close queue
//        qRec->Close();
//
//        SAFEARRAY* psa= recVariant.parray;
//        nBytesRead    = __min(psa->rgsabound->cElements, nBufLen);
//
//        for (LONG ind= 0; ind< nBytesRead; ind++)
//            SafeArrayGetElement(psa, &ind, &pBuf[ind]);
//
//        return true;
//    }
//    catch (_com_error comerr)
//    {
//        // report error - failed receiving, (WCHAR*)comerr.Description());
//        return false;
//    }
//}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClientTopic::IsPublishingSupported( void )
{GUCEF_TRACE;
    
    return m_config.needPublishSupport;
}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClientTopic::IsSubscribingSupported( void )
{GUCEF_TRACE;

    return true;
}

/*-------------------------------------------------------------------------*/

const CORE::CString& 
CMsmqPubSubClientTopic::GetTopicName( void ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return m_config.topicName;
}

/*-------------------------------------------------------------------------*/

bool 
CMsmqPubSubClientTopic::Publish( const COMCORE::CIPubSubMsg& msg )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    // Each Message Queuing message can have no more than 4 MB of data.
    
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CMsmqPubSubClientTopic::SetupToSubscribe( COMCORE::CPubSubClientTopicConfig& config )
{GUCEF_TRACE;

    //// Define the required constants and variables.  
    //const int NUMBEROFPROPERTIES = 5;  
    //DWORD cPropId = 0;  
    //HRESULT hr = MQ_OK;                                 // Return code  
    //HANDLE hQueue = NULL;                               // Queue handle  
    //ULONG ulBufferSize = 2;  
  
    //// Define an MQMSGPROPS structure.  
    //MQMSGPROPS msgprops;  
    //MSGPROPID aMsgPropId[NUMBEROFPROPERTIES];  
    //MQPROPVARIANT aMsgPropVar[NUMBEROFPROPERTIES];  
    //HRESULT aMsgStatus[NUMBEROFPROPERTIES];  
  
    //// Specify the message properties to be retrieved.  
    //aMsgPropId[cPropId] = PROPID_M_LABEL_LEN;           // Property ID  
    //aMsgPropVar[cPropId].vt = VT_UI4;                   // Type indicator  
    //aMsgPropVar[cPropId].ulVal = MQ_MAX_MSG_LABEL_LEN;  // Length of label  
    //cPropId++;  
  
    //WCHAR wszLabelBuffer[MQ_MAX_MSG_LABEL_LEN];         // Label buffer  
    //aMsgPropId[cPropId] = PROPID_M_LABEL;               // Property ID  
    //aMsgPropVar[cPropId].vt = VT_LPWSTR;                // Type indicator  
    //aMsgPropVar[cPropId].pwszVal = wszLabelBuffer;      // Label buffer  
    //cPropId++;  
  
    //UCHAR * pucBodyBuffer = NULL;  
    //pucBodyBuffer = (UCHAR*)malloc(ulBufferSize);   
    //if (pucBodyBuffer == NULL)  
    //{  
    //return MQ_ERROR_INSUFFICIENT_RESOURCES;  
    //}  
    //memset(pucBodyBuffer, 0, ulBufferSize);  
    //aMsgPropId[cPropId] = PROPID_M_BODY_SIZE;           // Property ID  
    //aMsgPropVar[cPropId].vt = VT_NULL;                  // Type indicator  
    //cPropId++;  
  
    //aMsgPropId[cPropId] = PROPID_M_BODY;                // Property ID  
    //aMsgPropVar[cPropId].vt = VT_VECTOR | VT_UI1;       // Type indicator  
    //aMsgPropVar[cPropId].caub.pElems = (UCHAR*)pucBodyBuffer;  // Body buffer  
    //aMsgPropVar[cPropId].caub.cElems = ulBufferSize;    // Buffer size  
    //cPropId++;  
  
    //aMsgPropId[cPropId] = PROPID_M_BODY_TYPE;           // Property ID  
    //aMsgPropVar[cPropId].vt = VT_NULL;                  // Type indicator  
    //cPropId++;  
  
    //// Initialize the MQMSGPROPS structure.  
    //msgprops.cProp = cPropId;                           // Number of message properties  
    //msgprops.aPropID = aMsgPropId;                      // IDs of the message properties  
    //msgprops.aPropVar = aMsgPropVar;                    // Values of the message properties  
    //msgprops.aStatus = aMsgStatus;                      // Error reports  

return false;
}

/*-------------------------------------------------------------------------*/

bool 
CMsmqPubSubClientTopic::SaveConfig( COMCORE::CPubSubClientTopicConfig& config ) const
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    config = m_config;
    return true;
}

/*-------------------------------------------------------------------------*/

bool 
CMsmqPubSubClientTopic::LoadConfig( const COMCORE::CPubSubClientTopicConfig& config )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    m_config = config;

    return true;
}

/*-------------------------------------------------------------------------*/

void
CMsmqPubSubClientTopic::PrepStorageForReadMsgs( CORE::UInt32 msgCount )
{GUCEF_TRACE;

    // reset size, note this does not dealloc the underlying memory
    m_pubsubMsgs.clear();
    TPubSubMsgsRefVector& msgRefs = m_pubsubMsgsRefs.GetData();
    msgRefs.clear();

    if ( msgCount > m_pubsubMsgs.capacity() )
    {
        // Grow the actual storage, this is allowed to become larger than msgCount to limit
        // the nr of dynamic allocations
        m_pubsubMsgs.reserve( msgCount );        
        msgRefs.reserve( msgCount );
        m_msmqReceiveMsgs.reserve( msgCount );
    }
}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClientTopic::Disconnect( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    if ( GUCEF_NULL != m_receiveQueueHandle )
    {
        ::MQCloseQueue( m_receiveQueueHandle );
        m_receiveQueueHandle = GUCEF_NULL;
    }

    return true;
}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClientTopic::Subscribe( void )
{GUCEF_TRACE;

    Disconnect();

    MT::CScopeMutex lock( m_lock );

    std::wstring wQueueName;
    CORE::Utf8toUtf16( m_config.topicName, wQueueName );

    HRESULT openQueueResult = ::MQOpenQueue( wQueueName.c_str()    ,  // IN:  Queue format name
                                             MQ_RECEIVE_ACCESS     ,  // IN:  Want to receive from queue
                                             0                     ,  // IN:  Allow sharing
                                             &m_receiveQueueHandle ); // OUT: Handle of open queue

    if ( FAILED( openQueueResult ) )
    {
        GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "MsmqPubSubClientTopic:Subscribe: Failed to subscribe by opening a MSMQ queue for recieving. Queue name: " + m_config.topicName ); 
        return false;
    }

    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CMsmqPubSubClientTopic::SubscribeStartingAtMsgId( const CORE::CVariant& msgIdBookmark )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    
    // Currently not supported.
    // Could theoretically be implemented using a cursor combined with PEEK only reads
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClientTopic::SubscribeStartingAtMsgDateTime( const CORE::CDateTime& msgDtBookmark )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );

    // Currently not supported.
    // Could theoretically be implemented using a cursor combined with PEEK only reads
    // For regular queue consuming reads you could perhaps ignore messages with a send time older then msgDtBookmark
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CMsmqPubSubClientTopic::IsConnected( void )
{GUCEF_TRACE;

    MT::CScopeMutex lock( m_lock );
    return false;
}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClientTopic::InitializeConnectivity( void )
{GUCEF_TRACE;

    Disconnect();
    
    MT::CScopeMutex lock( m_lock );

    return false;
}

/*-------------------------------------------------------------------------*/

CORE::Int64
CMsmqPubSubClientTopic::GetCurrentNrOfMessagesInQueue( void ) const
{GUCEF_TRACE;

    // Define the required constants and variables.  
    const int NUMBEROFPROPERTIES = 1;  
    DWORD cPropId = 0;  

    // Define an MQMGMTROPS structure.  
    ::MQMGMTPROPS mgmtprops;  
    ::MGMTPROPID aMgmtPropId[ NUMBEROFPROPERTIES ];  
    ::MQPROPVARIANT aMgmtPropVar[ NUMBEROFPROPERTIES ];  

    aMgmtPropId[ cPropId ] = PROPID_MGMT_QUEUE_MESSAGE_COUNT;    // Property identifier  
    aMgmtPropVar[ cPropId ].vt = VT_NULL;                        // Type indicator  
    ++cPropId;

    // Initialize the MQMGMTPROPS structure.  
    mgmtprops.cProp = cPropId;   // number of management properties  
    mgmtprops.aPropID = aMgmtPropId;// management property IDs  
    mgmtprops.aPropVar = aMgmtPropVar;// management property values  
    mgmtprops.aStatus  = NULL;// no storage for error codes   
    
    // Now that we formulated the request
    // actually ask for the info
    std::wstring objectName;
    CORE::Utf8toUtf16( "QUEUE=" + m_config.topicName, objectName );  
    HRESULT queueInfoFetchResult = ::MQMgmtGetInfo( NULL, objectName.c_str(), &mgmtprops );
    if ( MQ_OK == queueInfoFetchResult )
    {
        return (CORE::Int64) aMgmtPropVar[ 0 ].ulVal;
    }

    return -1;
}

/*-------------------------------------------------------------------------*/
   
// From: https://stackoverflow.com/questions/45149620/convert-ole-automation-date-oadate-double-to-struct-tm-without-using-variantti
// Slightly altered: used platform types to address ULL type concern

struct tm* SecondsSinceEpochToDateTime(struct tm* pTm, uint64_t SecondsSinceEpoch)
{
   uint64_t sec;
   unsigned int quadricentennials, centennials, quadrennials, annuals/*1-ennial?*/;
   unsigned int year, leap;
   unsigned int yday, hour, min;
   unsigned int month, mday, wday;
   static const unsigned int daysSinceJan1st[2][13]=
   {
      {0,31,59,90,120,151,181,212,243,273,304,334,365}, // 365 days, non-leap
      {0,31,60,91,121,152,182,213,244,274,305,335,366}  // 366 days, leap
  };
/*
   400 years:

   1st hundred, starting immediately after a leap year that's a multiple of 400:
   n n n l  \
   n n n l   } 24 times
   ...      /
   n n n l /
   n n n n

   2nd hundred:
   n n n l  \
   n n n l   } 24 times
   ...      /
   n n n l /
   n n n n

   3rd hundred:
   n n n l  \
   n n n l   } 24 times
   ...      /
   n n n l /
   n n n n

   4th hundred:
   n n n l  \
   n n n l   } 24 times
   ...      /
   n n n l /
   n n n L <- 97'th leap year every 400 years
*/

   // Re-bias from 1970 to 1601:
   // 1970 - 1601 = 369 = 3*100 + 17*4 + 1 years (incl. 89 leap days) =
   // (3*100*(365+24/100) + 17*4*(365+1/4) + 1*365)*24*3600 seconds
   sec = SecondsSinceEpoch + 11644473600ULL;

   wday = (CORE::UInt32)((sec / 86400 + 1) % 7); // day of week

   // Remove multiples of 400 years (incl. 97 leap days)
   quadricentennials = (CORE::UInt32)(sec / CORE::UInt64( 12622780800ULL ) ); // 400*365.2425*24*3600
   sec %= CORE::UInt64( 12622780800ULL );

   // Remove multiples of 100 years (incl. 24 leap days), can't be more than 3
   // (because multiples of 4*100=400 years (incl. leap days) have been removed)
   centennials = (CORE::UInt32)(sec / CORE::UInt64( 3155673600ULL ) ); // 100*(365+24/100)*24*3600
   if (centennials > 3)
   {
      centennials = 3;
   }
   sec -= centennials * CORE::UInt64( 3155673600 );

   // Remove multiples of 4 years (incl. 1 leap day), can't be more than 24
   // (because multiples of 25*4=100 years (incl. leap days) have been removed)
   quadrennials = (CORE::UInt32)(sec / 126230400); // 4*(365+1/4)*24*3600
   if (quadrennials > 24)
   {
      quadrennials = 24;
   }
   sec -= quadrennials * CORE::UInt64( 126230400ULL );

   // Remove multiples of years (incl. 0 leap days), can't be more than 3
   // (because multiples of 4 years (incl. leap days) have been removed)
   annuals = (CORE::UInt32)(sec / 31536000); // 365*24*3600
   if (annuals > 3)
   {
      annuals = 3;
   }
   sec -= annuals * CORE::UInt64( 31536000ULL );

   // Calculate the year and find out if it's leap
   year = 1601 + quadricentennials * 400 + centennials * 100 + quadrennials * 4 + annuals;
   leap = !(year % 4) && (year % 100 || !(year % 400));

   // Calculate the day of the year and the time
   yday = sec / 86400;
   sec %= 86400;
   hour = sec / 3600;
   sec %= 3600;
   min = sec / 60;
   sec %= 60;

   // Calculate the month
   for (mday = month = 1; month < 13; month++)
   {
      if (yday < daysSinceJan1st[leap][month])
      {
         mday += yday - daysSinceJan1st[leap][month - 1];
         break;
      }
   }

   // Fill in C's "struct tm"
   memset(pTm, 0, sizeof(*pTm));
   pTm->tm_sec = sec;          // [0,59]
   pTm->tm_min = min;          // [0,59]
   pTm->tm_hour = hour;        // [0,23]
   pTm->tm_mday = mday;        // [1,31]  (day of month)
   pTm->tm_mon = month - 1;    // [0,11]  (month)
   pTm->tm_year = year - 1900; // 70+     (year since 1900)
   pTm->tm_wday = wday;        // [0,6]   (day since Sunday AKA day of week)
   pTm->tm_yday = yday;        // [0,365] (day since January 1st AKA day of year)
   pTm->tm_isdst = -1;         // daylight saving time flag

   return pTm;
}

/*-------------------------------------------------------------------------*/
   
// From: https://stackoverflow.com/questions/45149620/convert-ole-automation-date-oadate-double-to-struct-tm-without-using-variantti
// Slightly altered, got rid of exception throw

struct tm* fromOADate(struct tm* p_Tm, double p_OADate)
{
   static const int64_t OA_UnixTimestamp = -2209161600; /* 30-Dec-1899 */

   if (!(   -109205 <= p_OADate               /* 01-Jan-1601 */
         &&            p_OADate <= 2958465))  /* 31-Dec-9999 */
   {
      return GUCEF_NULL;
   }

   int64_t OADatePassedDays = p_OADate;
   double  OADateDayTime    = p_OADate - OADatePassedDays;
   int64_t OADateSeconds    = OA_UnixTimestamp
                            + OADatePassedDays * 24LL * 3600LL
                            + OADateDayTime * 24.0 * 3600.0;

   return SecondsSinceEpochToDateTime(p_Tm, OADateSeconds);
}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClientTopic::MsmqPropertyToVariant( MQPROPVARIANT& msmqSourceVariant, CORE::CVariant& targetVariant, bool linkIfPossible, CORE::UInt32 lengthIfApplicable )
{GUCEF_TRACE;

    switch ( msmqSourceVariant.vt )
    {
        case VT_UI1:                    { targetVariant = (CORE::UInt8) msmqSourceVariant.bVal; return true; }
        case VT_I2:                     { targetVariant = (CORE::Int16) msmqSourceVariant.iVal; return true; }
        case VT_UI2:                    { targetVariant = (CORE::UInt16) msmqSourceVariant.uiVal; return true; }
        case VT_I4:                     { targetVariant = (CORE::Int32) msmqSourceVariant.lVal; return true; }
        case VT_UI4:                    { targetVariant = (CORE::UInt32) msmqSourceVariant.ulVal; return true; }
        case VT_UI8:                    { targetVariant = (CORE::UInt64) msmqSourceVariant.uhVal.QuadPart; return true; }
        case VT_ERROR:                  { targetVariant = (CORE::Int32) msmqSourceVariant.scode; return true; }        
        case VT_BOOL:                   { targetVariant = ( VARIANT_FALSE != msmqSourceVariant.boolVal ); return true; } // Actually stored as a short
        
        // The CLSID (GUID) record type is 16 bytes long. The adapter does not need a size field for this record because it is a fixed length field. 
        // It is critical that the data must be exactly 16 bytes long for proper I/O with MSMQ.
        case VT_CLSID:                  { return targetVariant.Set( msmqSourceVariant.pszVal, 16, GUCEF_DATATYPE_ASCII_STRING, linkIfPossible ); }

        // Rememeber that like the COM concept the length is actually stored at (BSTR pointer address)-4  
        // Since in the COM world this type is often used to shuttle binary data around we will mark it as a binary and not GUCEF_DATATYPE_UTF16_STRING
        case VT_BSTR:                   { return targetVariant.Set( msmqSourceVariant.pwszVal, SysStringLen( msmqSourceVariant.bstrVal ), GUCEF_DATATYPE_BINARY, linkIfPossible ); } 

        case VT_LPSTR:                  { return targetVariant.Set( msmqSourceVariant.pszVal, lengthIfApplicable, GUCEF_DATATYPE_UTF8_STRING, linkIfPossible ); } 
        case VT_LPWSTR:                 { return targetVariant.Set( msmqSourceVariant.pwszVal, lengthIfApplicable, GUCEF_DATATYPE_UTF16_STRING, linkIfPossible ); } 
        case VT_BLOB:                   { return targetVariant.Set( msmqSourceVariant.blob.pBlobData, msmqSourceVariant.blob.cbSize, GUCEF_DATATYPE_BINARY, linkIfPossible ); }
        
        case VT_DATE:                   
        { 
            // Sadly date is stored as a OLE Automation DATE which is a pain to convert
            struct tm tmStruct;
            if ( GUCEF_NULL != fromOADate( &tmStruct, msmqSourceVariant.date ) )
            {
                targetVariant = CORE::CDateTime( &tmStruct, true ).ToIso8601DateTimeString( true, true );
                return true;
            } 
            return false;             
        }
        
        // We dont have support for a special type for more complex data arrangements like vectors
        // until (if ever) we do, just note these as BINARY data
        case VT_VECTOR | VT_UI1:
        case VT_VECTOR | VT_I2:
        case VT_VECTOR | VT_UI2:
        case VT_VECTOR | VT_BOOL:
        case VT_VECTOR | VT_I4:
        case VT_VECTOR | VT_CLSID:
        case VT_VECTOR | VT_BSTR:
        case VT_VECTOR | VT_LPWSTR:
        case VT_VECTOR | VT_VARIANT:
        {
            return targetVariant.Set( msmqSourceVariant.pszVal, lengthIfApplicable, GUCEF_DATATYPE_BINARY, linkIfPossible ); 
        }
        case VT_EMPTY:
        default:
        {
            targetVariant.Clear();
            return false;
        }
    }
}

/*-------------------------------------------------------------------------*/

VARTYPE
CMsmqPubSubClientTopic::GetMsmqVariantTypeForMsmqProperty( PROPID propertyId )
{GUCEF_TRACE;
    
    switch ( propertyId )
    {
        case PROPID_M_ABORT_COUNT: { return VT_UI4; }
        case PROPID_M_ACKNOWLEDGE: { return VT_UI1; }
        case PROPID_M_ADMIN_QUEUE: { return VT_LPWSTR; }
        case PROPID_M_ADMIN_QUEUE_LEN: { return VT_UI4; }
        case PROPID_M_APPSPECIFIC: { return VT_UI4; }
        case PROPID_M_ARRIVEDTIME: { return VT_UI4; }
        case PROPID_M_AUTH_LEVEL: { return VT_UI4; }
        case PROPID_M_AUTHENTICATED: { return VT_UI1; }
        case PROPID_M_AUTHENTICATED_EX: { return VT_UI1; }
        case PROPID_M_BODY: { return VT_VECTOR | VT_UI1; }
        case PROPID_M_BODY_SIZE: { return VT_UI4; }
        case PROPID_M_BODY_TYPE: { return VT_UI4; }
        case PROPID_M_CLASS: { return VT_UI2; }
        case PROPID_M_COMPOUND_MESSAGE: { return VT_VECTOR | VT_UI1; }  // An array of bytes that represents the entire contents of an SRMP message, including both the SOAP envelope and the SOAP attachments (MIME binary attachments) associated with it.
        case PROPID_M_COMPOUND_MESSAGE_SIZE: { return VT_UI4; }         // The size (in bytes) of the entire SRMP message, including both the SOAP envelope and the SOAP attachments, provided by PROPID_M_COMPOUND_MESSAGE.
        case PROPID_M_CONNECTOR_TYPE: { return VT_CLSID; }              // Pointer to a connector application identifier.
        case PROPID_M_CORRELATIONID: { return VT_VECTOR | VT_UI1; }     // Application-defined 20-byte message identifier (default is 0).
        case PROPID_M_DEADLETTER_QUEUE: { return VT_LPWSTR; }           // The path name of an application-specific DLQ.
        case PROPID_M_DEADLETTER_QUEUE_LEN: { return VT_UI4; }          // Input: The length of the application-specific dead letter queue (DLQ) name buffer (in Unicode characters) allocated by the receiving application. | Return: The length (in Unicode characters) of the application-specific DLQ name string (including the null-terminating character) returned by PROPID_M_DEADLETTER_QUEUE.
        case PROPID_M_DELIVERY: { return VT_UI1; }                      // The PROPID_M_DELIVERY property specifies how Message Queuing delivers the message to the queue.
        case PROPID_M_DEST_FORMAT_NAME: { return VT_LPWSTR; }           // String that contains a public, private, direct, distribution list, or multiple-element format name.
        case PROPID_M_DEST_FORMAT_NAME_LEN: { return VT_UI4; }          // On input: The length of the format name buffer (in Unicode characters) allocated by the receiving application. | On return: The length (in Unicode characters) of the format name string (including the null-terminating character) returned in PROPID_M_DEST_FORMAT_NAME.
        case PROPID_M_DEST_QUEUE: { return VT_LPWSTR; }                 // Format name of the original destination queue.
        
        default: { return VT_EMPTY; }
    }
}


/*-------------------------------------------------------------------------*/

void 
CMsmqPubSubClientTopic::PrepMsmqVariantStorageForProperty( PROPID propertyId, MQPROPVARIANT& msmqVariant )
{GUCEF_TRACE;
    
    msmqVariant.vt = GetMsmqVariantTypeForMsmqProperty( propertyId );
}

/*-------------------------------------------------------------------------*/

void 
CMsmqPubSubClientTopic::PrepMsmqMsgStorage( void )
{GUCEF_TRACE;

    if ( m_config.needSubscribeSupport )
    {
        m_msmqReceiveMsgs.resize( m_config.maxMsmqMsgsToReadPerSyncCycle );
        MsmqMsgVector::iterator i = m_msmqReceiveMsgs.begin();
        while ( i != m_msmqReceiveMsgs.end() )
        {
            TMsmqMsg& msg = (*i);

            msg.aMsgPropId.resize( m_config.msmqPropIdsToReceive.size() );
            msg.aMsgPropVar.resize( m_config.msmqPropIdsToReceive.size() );

            msg.msgprops.cProp = (DWORD) m_config.msmqPropIdsToReceive.size();   // Number of properties we want to receive per message
            msg.msgprops.aPropID = &msg.aMsgPropId[ 0 ];                         // pointer to MSMQ PROPID array
            msg.msgprops.aPropVar = &msg.aMsgPropVar[ 0 ];                       // pointer to MSMQ VARIANT struct array
            msg.msgprops.aStatus = 0;                                            // Error reports

            CORE::UInt32 propIndex = 0;
            MSGPROPIDVector::iterator p = m_config.msmqPropIdsToReceive.begin();
            while ( p != m_config.msmqPropIdsToReceive.end() )
            {
                msg.aMsgPropId[ propIndex ] = (*p);
                PrepMsmqVariantStorageForProperty( (*p), msg.msgprops.aPropVar[ propIndex ] );                
                ++p; ++propIndex;
            }
            ++i;
        }
    }
}

///*-------------------------------------------------------------------------*/
//
//
//
//HRESULT MsmqQueue::read(     char    *szMessageBody,
//                             int     *iMessageBodySize,
//							 int     *iMessageBodyType,
//                             char    *szLabel,
//                             char    *szCorrelationID,
//							 bool    isTransactional,
//                             int     iTimeout, 
//                             bool    isPeekOnly
//                             )
//{
//    const int NUMBEROFPROPERTIES = 8;
//    DWORD i = 0;
//    HRESULT hr = MQ_OK;
//	
//    DWORD dwAction= isPeekOnly ? MQ_ACTION_PEEK_CURRENT : MQ_ACTION_RECEIVE;
//	ITransaction* pTransaction = isTransactional ? MQ_SINGLE_MESSAGE : MQ_NO_TRANSACTION;
//
//    MQMSGPROPS msgprops;
//    MSGPROPID aMsgPropId[NUMBEROFPROPERTIES];
//    PROPVARIANT aMsgPropVar[NUMBEROFPROPERTIES];
//
//	// Message Body Buffer
//	if (szMessageBody!=NULL)
//	{
//		memset(szMessageBody, 0, *iMessageBodySize);
//		aMsgPropId[i] = PROPID_M_BODY;
//		aMsgPropVar[i].vt = VT_VECTOR | VT_UI1;
//		aMsgPropVar[i].caub.pElems = (LPBYTE)szMessageBody;
//		aMsgPropVar[i].caub.cElems = *iMessageBodySize;
//		i++;
//	}
//
//	// Message Body Size
//	aMsgPropId[i] = PROPID_M_BODY_SIZE;
//	aMsgPropVar[i].vt = VT_NULL;
//	i++;
//	
//	// Message Body Type
//	aMsgPropId[i] = PROPID_M_BODY_TYPE;
//	aMsgPropVar[i].vt = VT_NULL;
//	i++;
//
//	// Correlation ID
//	aMsgPropId[i] = PROPID_M_CORRELATIONID;
//    aMsgPropVar[i].vt = VT_VECTOR | VT_UI1;
//    aMsgPropVar[i].caub.pElems = (LPBYTE)szCorrelationID;
//    aMsgPropVar[i].caub.cElems = PROPID_M_CORRELATIONID_SIZE;
//	memset(szCorrelationID, 0, PROPID_M_CORRELATIONID_SIZE);
//    i++;
//
//	// Label Size
//    aMsgPropId[i] = PROPID_M_LABEL_LEN;            // Property ID
//    aMsgPropVar[i].vt =VT_UI4;                     // Type indicator
//    aMsgPropVar[i].ulVal = MQ_MAX_MSG_LABEL_LEN;   // Label buffer size
//    i++;
//
//	// Label Buffer
//    WCHAR wszLabelBuffer[MQ_MAX_MSG_LABEL_LEN] = {0};
//    aMsgPropId[i] = PROPID_M_LABEL;
//    aMsgPropVar[i].vt = VT_LPWSTR;
//    aMsgPropVar[i].pwszVal = wszLabelBuffer;
//    i++;
//
//    // NB: if you want to get PROPID_M_RESP_QUEUE, you must also request PROPID_M_RESP_QUEUE_LEN. 
//    // that implies 2 additional properties to request. 
//    // see http://msdn.microsoft.com/library/en-us/msmq/html/ea8cfb28-43e0-4041-9add-11080375690c.asp
//
//    msgprops.cProp = i;                         // Number of message properties
//    msgprops.aPropID = aMsgPropId;                    // IDs of the message properties
//    msgprops.aPropVar = aMsgPropVar;                  // Values of the message properties
//    msgprops.aStatus  = 0;			  // Error reports
//
//    hr = MQReceiveMessage(
//			  hQueue,             // Handle to the destination queue
//			  iTimeout,	      // Time out interval
//			  dwAction,           // Peek?  or Dequeue.  Receive action
//			  &msgprops,          // Pointer to the MQMSGPROPS structure
//			  NULL, NULL, NULL,   // No OVERLAPPED structure etc.
//			  pTransaction  // MQ_SINGLE_MESSAGE | MQ_MTS_TRANSACTION |
//			                     // MQ_XA_TRANSACTION   
//			  );

/*-------------------------------------------------------------------------*/

void
CMsmqPubSubClientTopic::OnMsmqMsgReceived( const MQMSGPROPS& msg, CORE::UInt32 msgCycleIndex, bool linkIfPossible )
{GUCEF_TRACE;

    COMCORE::CBasicPubSubMsg& translatedMsg = m_pubsubMsgs[ msgCycleIndex ];

    CORE::UInt32 bodyType = VT_EMPTY;
    CORE::UInt32 bodySize = 0;

    for ( CORE::UInt32 i=0; i<msg.cProp; ++i )
    {
        // Depending on what the property is we may perform special handling
        // Note that we rely here on config loading having enforced an ordering in the message
        // properties that ensures that things like length/size or type come before the actual payload

        CORE::CVariant keyVar( (CORE::UInt64) msg.aPropID[ i ] );
        CORE::CVariant ValueVar;

        switch ( msg.aPropID[ i ] )
        {
            
            case PROPID_M_BODY_TYPE: { MsmqPropertyToVariant( msg.aPropVar[ i ], ValueVar, linkIfPossible ); bodyType = ValueVar.AsUInt32( bodyType ); break; }
            case PROPID_M_BODY_SIZE: { MsmqPropertyToVariant( msg.aPropVar[ i ], ValueVar, linkIfPossible ); bodySize = ValueVar.AsUInt32( bodySize ); break; }
            case PROPID_M_BODY:
            {
                if ( MsmqPropertyToVariant( msg.aPropVar[ i ], ValueVar, linkIfPossible, bodySize ) )
                {                
                    if ( linkIfPossible )
                        translatedMsg.GetPrimaryPayload().LinkTo( ValueVar );
                    else
                        ValueVar.TransferOwnershipTo( translatedMsg.GetPrimaryPayload() );
                }
            }
            
            default:
            {
                // No special processing, treat as meta-data

                if ( !MsmqPropertyToVariant( msg.aPropVar[ i ], ValueVar, linkIfPossible ) )
                {
                    // Skip this property, continue best effort
                    GUCEF_ERROR_LOG( CORE::LOGLEVEL_NORMAL, "MsmqPubSubClientTopic::OnMsmqMsgReceived: Unable to translate MSMQ meta-data propery value variant for property " + keyVar.AsString() +
                        " which is property " + CORE::ToString( i ) + " of " + CORE::ToString( (CORE::UInt32) msg.cProp ) );
                    continue;
                }

                if ( linkIfPossible )
                    translatedMsg.AddLinkedMetaDataKeyValuePair( keyVar, ValueVar );
                else
                    translatedMsg.AddMetaDataKeyValuePair( keyVar, ValueVar );
                break;
            }
        }
    }    
}

/*-------------------------------------------------------------------------*/

void
CMsmqPubSubClientTopic::OnSyncReadTimerCycle( CORE::CNotifier* notifier    ,
                                              const CORE::CEvent& eventId  ,
                                              CORE::CICloneable* eventData )
{GUCEF_TRACE;
    
    // In order to fetch messages in bulk we obtain the snapshot of how many messages are in the queue right now
    m_msmqMsgsQueued = GetCurrentNrOfMessagesInQueue(); 
    if ( m_msmqMsgsQueued > 0 )
    {
        PrepStorageForReadMsgs( (CORE::UInt32) m_msmqMsgsQueued );
    }
    else
    if ( m_msmqMsgsQueued == 0 )
    {
        // There are no messages queued, we are done
        return;
    }
    else
    {
        // Something went wrong
        // Just try to read 1 message trial and error
        m_msmqMsgsQueued = 1;
    }
    
    DWORD dwAction = MQ_ACTION_RECEIVE;                 // could support peeking: isPeekOnly ? MQ_ACTION_PEEK_CURRENT : MQ_ACTION_RECEIVE;
    DWORD dwTimeout = 0;                                // We dont wait for messages during sync reads, we get what we can this cycle and thats it
    ITransaction* pTransaction = MQ_NO_TRANSACTION;     // could support transactions: isTransactional ? MQ_SINGLE_MESSAGE : MQ_NO_TRANSACTION;
    for ( CORE::UInt32 i=0; i<m_msmqMsgsQueued; ++i )
    {
        CORE::UInt64 receiveResult = (CORE::UInt64) ::MQReceiveMessage( m_receiveQueueHandle             ,  // Handle to the destination queue
                                                                        dwTimeout                        ,  // Time out interval
                                                                        dwAction                         ,  // Peek?  or Dequeue.  Receive action
                                                                        &m_msmqReceiveMsgs[ i ].msgprops ,  // Pointer to the MQMSGPROPS structure
                                                                        NULL                             ,  // No OVERLAPPED structure etc.
                                                                        NULL                             ,  // No OVERLAPPED structure etc.
                                                                        NULL                             ,  // No OVERLAPPED structure etc.
                                                                        pTransaction                     ); // MQ_SINGLE_MESSAGE | MQ_MTS_TRANSACTION | MQ_XA_TRANSACTION   
        switch ( receiveResult )
        {
            case MQ_OK:
            {
                // Successfully received a message
                // Since we are doing a sync cycle we can use linking
                OnMsmqMsgReceived( m_msmqReceiveMsgs[ i ].msgprops, i, true );
            }

            case MQ_ERROR_IO_TIMEOUT : break; // No more messages are available
        }
    }
}

/*-------------------------------------------------------------------------*/

void
CMsmqPubSubClientTopic::OnMetricsTimerCycle( CORE::CNotifier* notifier    ,
                                             const CORE::CEvent& eventId  ,
                                             CORE::CICloneable* eventData )
{GUCEF_TRACE;


}

/*-------------------------------------------------------------------------*/

const MT::CILockable* 
CMsmqPubSubClientTopic::AsLockable( void ) const
{GUCEF_TRACE;

    return this;
}

/*-------------------------------------------------------------------------*/

 bool
 CMsmqPubSubClientTopic::Lock( CORE::UInt32 lockWaitTimeoutInMs ) const
{GUCEF_TRACE;

    return m_lock.Lock( lockWaitTimeoutInMs );
}

/*-------------------------------------------------------------------------*/

bool
CMsmqPubSubClientTopic::Unlock( void ) const
{GUCEF_TRACE;

    return m_lock.Unlock();
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MSMQ */
}; /* namespace PUBSUBPLUGIN */
}; /* namespace GUCEF */

/*--------------------------------------------------------------------------*/
