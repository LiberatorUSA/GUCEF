/*
 *  gucefCOM: GUCEF module providing communication implementations 
 *  for standardized protocols
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

#ifndef GUCEF_COM_CASYNCHTTPSERVERREQUESTHANDLER_H
#define GUCEF_COM_CASYNCHTTPSERVERREQUESTHANDLER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CITASKCONSUMER_H
#include "gucefCORE_CITaskConsumer.h"
#define GUCEF_CORE_CITASKCONSUMER_H
#endif /* GUCEF_CORE_CITASKCONSUMER_H ? */

#ifndef GUCEF_COM_CHTTPREQUESTDATA_H
#include "gucefCOM_CHttpRequestData.h"
#define GUCEF_COM_CHTTPREQUESTDATA_H
#endif /* GUCEF_COM_CHTTPREQUESTDATA_H ? */

#ifndef GUCEF_COM_CIHTTPSERVERREQUESTHANDLER_H
#include "gucefCOM_CIHttpServerRequestHandler.h"
#define GUCEF_COM_CIHTTPSERVERREQUESTHANDLER_H
#endif /* GUCEF_COM_CIHTTPSERVERREQUESTHANDLER_H ? */

#ifndef GUCEF_COM_MACROS_H
#include "gucefCOM_macros.h"
#define GUCEF_COM_MACROS_H
#endif /* GUCEF_COM_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COM {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_HIDDEN CAsyncHttpServerRequestHandler : public CORE::CTaskConsumer
{
    public:

    static const CORE::CEvent AsyncHttpServerRequestHandlingCompletedEvent;
    static const CORE::CString TaskType;
    
    static void RegisterEvents( void );

    CAsyncHttpServerRequestHandler();
    CAsyncHttpServerRequestHandler( const CAsyncHttpServerRequestHandler& src );
    virtual ~CAsyncHttpServerRequestHandler();

    virtual bool OnTaskStart( CORE::CICloneable* taskData ) GUCEF_VIRTUAL_OVERRIDE;
    
    virtual bool OnTaskCycle( CORE::CICloneable* taskData ) GUCEF_VIRTUAL_OVERRIDE;

    virtual void OnTaskEnding( CORE::CICloneable* taskdata ,
                               bool willBeForced           ) GUCEF_VIRTUAL_OVERRIDE;
    
    virtual void OnTaskEnded( CORE::CICloneable* taskdata ,
                               bool wasForced             ) GUCEF_VIRTUAL_OVERRIDE;

    virtual CORE::CString GetType( void ) const GUCEF_VIRTUAL_OVERRIDE;

    private:

    CIHttpServerRequestHandler* m_requestHandler;
};

/*-------------------------------------------------------------------------*/

typedef CORE::CTFactory< CORE::CTaskConsumer, CAsyncHttpServerRequestHandler > TAsyncHttpServerRequestHandlerFactory;

/*-------------------------------------------------------------------------*/

class CHTTPServer;

class GUCEF_HIDDEN CAsyncHttpRequestData : public CHttpRequestData
{
    public:

    CHTTPServer* httpServer;
    UInt32 clientConnectionId;
    COMCORE::CHostAddress remoteClientAddress;

    virtual CORE::CICloneable* Clone( void ) const GUCEF_VIRTUAL_OVERRIDE;

    CAsyncHttpRequestData( void );
    CAsyncHttpRequestData( const CAsyncHttpRequestData& src );
    virtual ~CAsyncHttpRequestData();
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COM */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_COM_CASYNCHTTPSERVERREQUESTHANDLER_H ? */
