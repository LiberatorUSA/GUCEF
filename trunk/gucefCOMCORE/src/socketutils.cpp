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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "socketutils.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COMCORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

bool
SetBlockingMode( SOCKET sock ,
                 bool block  )
{
    #ifdef GUCEF_LINUX_BUILD
	int opts;

	opts = fcntl( sock,F_GETFL);
	if ( opts < 0 )
	{
        return false;    
	}
	if ( block )
	{
	    opts = (opts & O_NONBLOCK);
	}
	else
	{
	    opts = (opts | O_NONBLOCK);
    }
    
	if ( fcntl( sock, F_SETFL, opts ) < 0 )
	{
        return false;
	}
	return true;
	
	#endif /* GUCEF_LINUX_BUILD ? */
	
	#ifdef GUCEF_MSWIN_BUILD	

    // About ioctlsocket():
    // *argp value       Nonblocking mode
    // 0                 Disabled
    // nonzero           Enabled
    
	u_long blockmode = ( block ? 0 : 1 );
    return ioctlsocket( sock                    , 
                        FIONBIO                 , 
                        (u_long FAR*)&blockmode ) != SOCKET_ERROR;
    #endif /* GUCEF_MSWIN_BUILD ? */
    
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace COMCORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
