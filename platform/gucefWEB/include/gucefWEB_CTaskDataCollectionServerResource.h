/*
 *  gucefWEB: GUCEF module providing Web application functionality 
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

#ifndef GUCEF_WEB_CTASKDATACOLLECTIONSERVERRESOURCE_H
#define GUCEF_WEB_CTASKDATACOLLECTIONSERVERRESOURCE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_CREADWRITELOCK_H
#include "gucefMT_CReadWriteLock.h"
#define GUCEF_MT_CREADWRITELOCK_H
#endif /* GUCEF_MT_CREADWRITELOCK_H ? */

#ifndef GUCEF_CORE_CTSHAREDPTR_H
#include "CTSharedPtr.h"
#define GUCEF_CORE_CTSHAREDPTR_H
#endif /* GUCEF_CORE_CTSHAREDPTR_H ? */

#ifndef GUCEF_CORE_CTASKINFO_H
#include "gucefCORE_CTaskInfo.h"
#define GUCEF_CORE_CTASKINFO_H
#endif /* GUCEF_CORE_CTASKINFO_H ? */

#ifndef GUCEF_WEB_MACROS_H
#include "gucefWEB_macros.h"
#define GUCEF_WEB_MACROS_H
#endif /* GUCEF_WEB_MACROS_H ? */

#ifndef GUCEF_WEB_CCODECBASEDHTTPSERVERRESOURCE_H
#include "gucefWEB_CCodecBasedHTTPServerResource.h"
#define GUCEF_WEB_CCODECBASEDHTTPSERVERRESOURCE_H
#endif /* GUCEF_WEB_CCODECBASEDHTTPSERVERRESOURCE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace WEB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Class which integrates a resource view of the world with the task manager's task data management
 *  Used by CTaskManagerServerResource to facilitate the submission of new tasks via task data
 */
class GUCEF_WEB_PUBLIC_CPP CTaskDataCollectionServerResource : public CCodecBasedHTTPServerResource
{
    public:

    CTaskDataCollectionServerResource( void );

    virtual ~CTaskDataCollectionServerResource() GUCEF_VIRTUAL_OVERRIDE;

    virtual TCreateState CreateResource( const CString& resourcePath                   ,
                                         const CORE::CString& transactionID            ,
                                         const CORE::CDataNode& input                  ,
                                         const CORE::CString& representation           ,
                                         const CORE::CString& params                   ,
                                         THTTPServerResourcePtr& resourceOutput        ,
                                         TStringVector& supportedRepresentationsOutput ) GUCEF_VIRTUAL_OVERRIDE;
    private:   
    
    CTaskDataCollectionServerResource( const CTaskDataCollectionServerResource& src );
    CTaskDataCollectionServerResource& operator=( const CTaskDataCollectionServerResource& src );
};

/*-------------------------------------------------------------------------*/

typedef CORE::CTSharedPtr< CTaskDataCollectionServerResource, MT::CMutex >      CTaskDataCollectionServerResourcePtr;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_WEB_CTASKDATACOLLECTIONSERVERRESOURCE_H ? */
