/*
 *  gucefCOMCORE: GUCEF module providing basic communication facilities
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
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

#ifndef SERVERPORTEXTENDERPROTOCOL_H
#define SERVERPORTEXTENDERPROTOCOL_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

enum ServerPortExtenderProtocolEnum
{
    SPEP_INITIAL_CONTROL_CLIENT_MSG                   ,
    SPEP_INITIAL_CONTROL_SERVER_MSG                   ,
    SPEP_CONTROL_SERVER_MSG_CREATE_CONNECTION_REQUEST
};

typedef enum ServerPortExtenderProtocolEnum TServerPortExtenderProtocolEnum;

/*-------------------------------------------------------------------------*/

#define SPEP_PROTOCOL_VERSION 1

/*-------------------------------------------------------------------------*/

#endif /* SERVERPORTEXTENDERPROTOCOL_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 09-06-2009 :
        - Dinand: First version

---------------------------------------------------------------------------*/