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

#ifndef GUCEF_LOGSERVICELIB_PROTOCOL_H
#define GUCEF_LOGSERVICELIB_PROTOCOL_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_LOGSERVICELIB_MACROS_H
#include "GucefLogServiceLib_macros.h"
#define GUCEF_LOGSERVICELIB_MACROS_H
#endif /* GUCEF_LOGSERVICELIB_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace LOGSERVICELIB {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

enum EGucefLogServiceMsgType
{
    LOGSVCMSGTYPE_DELIMITER    ,       
    
    LOGSVCMSGTYPE_CLIENTINFO   ,
    LOGSVCMSGTYPE_INITIALIZED  ,
    LOGSVCMSGTYPE_LOGMSG       ,
    LOGSVCMSGTYPE_FLUSHLOG     ,
    LOGSVCMSGTYPE_INCOMPATIBLE
};
typedef enum EGucefLogServiceMsgType TGucefLogServiceMsgType;

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace LOGSERVICELIB */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_LOGSERVICELIB_PROTOCOL_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 04-05-2005 :
        - Dinand: Initial version.

---------------------------------------------------------------------------*/