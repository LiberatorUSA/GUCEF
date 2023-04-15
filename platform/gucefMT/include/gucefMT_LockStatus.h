/*
 *  gucefMT: GUCEF module providing multithreading solutions
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

#ifndef GUCEF_MT_LOCKSTATUS_H
#define GUCEF_MT_LOCKSTATUS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_ETYPES_H
#include "gucefMT_ETypes.h"
#define GUCEF_MT_ETYPES_H
#endif /* GUCEF_MT_ETYPES_H ? */

#ifndef GUCEFMT_MACROS_H
#include "gucefMT_macros.h"     /* often used gucef macros */
#define GUCEFMT_MACROS_H
#endif /* GUCEFMT_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
namespace GUCEF {
namespace MT {
#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#define GUCEF_LOCKSTATUS_UNDEFINED               254

#define GUCEF_LOCKSTATUS_OPERATION_FAILED        0                               /**< status that means that lock operation could not be carried out successfully, the lock's status and ownership remains unchanged */
#define GUCEF_LOCKSTATUS_OPERATION_SUCCESS       1                               /**< status that means that the lock has been successfully obtained or successfully released depending on the context of the operation */
#define GUCEF_LOCKSTATUS_WAIT_TIMEOUT            2                               /**< status that means that the specified wait timeout to attain the lock has been exceeded and as such the attempt to obtain the lock is abandoned */
#define GUCEF_LOCKSTATUS_ABANDONED               3                               /**< status that means that the thread that used to own the lock is gone without properly releasing it. ownership has been transfered to the caller */

#define GUCEF_LOCKSTATUS_NOT_APPLICABLE          4                               /**< status that means that while the code supports locking the context does not require locking and as such this is a no-op */

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

enum ELockStatus 
{
    LOCKSTATUS_OPERATION_FAILED     = GUCEF_LOCKSTATUS_OPERATION_FAILED   ,      /**< status that means that lock operation could not be carried out successfully, the lock's status and ownership remains unchanged */
    LOCKSTATUS_OPERATION_SUCCESS    = GUCEF_LOCKSTATUS_OPERATION_SUCCESS  ,      /**< status that means that the lock has been successfully obtained or successfully released depending on the context of the operation */
    LOCKSTATUS_WAIT_TIMEOUT         = GUCEF_LOCKSTATUS_WAIT_TIMEOUT       ,      /**< status that means that the specified wait timeout to attain the lock has been exceeded and as such the attempt to obtain the lock is abandoned */
    LOCKSTATUS_ABANDONED            = GUCEF_LOCKSTATUS_ABANDONED          ,      /**< status that means that the thread that used to own the lock is gone without properly releasing it. ownership has been transfered to the caller */

    LOCKSTATUS_NOT_APPLICABLE       = GUCEF_LOCKSTATUS_NOT_APPLICABLE     ,      /**< status that means that while the code supports locking the context does not require locking and as such this is a no-op */
    LOCKSTATUS_UNDEFINED            = GUCEF_LOCKSTATUS_UNDEFINED                 /**< status that should not show up in a well functioning program as it signals an init state with incomplete initialization */
};
typedef enum ELockStatus TLockStatus;

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Prevent C++ name mangeling
 */
#ifdef __cplusplus
   extern "C" {
#endif   /* __cplusplus */

/*--------------------------------------------------------------------------*/

GUCEF_MT_PUBLIC_C const char* 
LockStatusToLockStatusString( TLockStatus threadStatus );

/*-------------------------------------------------------------------------*/

GUCEF_MT_PUBLIC_C TLockStatus 
LockStatusStringToLockStatus( const char* threadStatusStr );

/*--------------------------------------------------------------------------*/
#ifdef __cplusplus


GUCEF_MT_PUBLIC_CPP bool
LockStatusToLockSuccessStatusBool( TLockStatus threadStatus );


#endif   /* __cplusplus */
/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
   };
#endif /* __cplusplus */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
}; /* namespace MT */
}; /* namespace GUCEF */
#endif /* __cplusplus ? */

/*--------------------------------------------------------------------------*/

#endif /* GUCEF_MT_LOCKSTATUS_H ? */
