/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
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
 
#ifndef SOCKETERRORS_H
#define SOCKETERRORS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEFCOMCORE_MACROS_H
#include "gucefCOMCORE_macros.h"     /* often used gucef macros */
#define GUCEFCOMCORE_MACROS_H
#endif /* GUCEFCOMCORE_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

enum ESocketError
{
        SOCKERR_NO_ERROR = 0      ,
        SOCKERR_INTERNAL_ERROR    ,
        SOCKERR_INVALID_SOCKET    ,
        SOCKERR_OUT_OF_MEMORY     ,
        SOCKERR_UDP_SEND_FAILED   ,
        SOCKERR_INVALID_ADDRESS   ,
        SOCKERR_CANT_FIND_ADDRESS ,
        SOCKERR_CANT_SEND_DATA    ,
        SOCKERR_CANT_OPEN_SOCKET  
        
};
typedef enum ESocketError TSocketError;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* SOCKETERRORS_H ? */



