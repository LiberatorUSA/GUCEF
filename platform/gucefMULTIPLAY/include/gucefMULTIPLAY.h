/*
 *  gucefMULTIPLAY: GUCEF module providing multiplayer RAD functionality
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

/*
 *  This is the main include header for the gucefMULTIPLAY library.
 *  It is part of a platform called GUCEF.
 *  All classes in this library have to do with enabling multiplayer
 *  functionality
 */

#ifndef GUCEF_MULTIPLAY_H
#define GUCEF_MULTIPLAY_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Library specific macros and generic macros
 */
#ifndef GUCEF_MULTIPLAY_MACROS_H
#include "gucefMULTIPLAY_macros.h"
#define GUCEF_MULTIPLAY_MACROS_H
#endif /* GUCEF_MULTIPLAY_MACROS_H ? */

/*---------------------------------------------------------------------------/

        C++ includes

/---------------------------------------------------------------------------*/
#ifdef __cplusplus

#ifndef GUCEF_MULTIPLAY_CSERVERLIST_H
#include "gucefMULTIPLAY_CServerList.h"
#define GUCEF_MULTIPLAY_CSERVERLIST_H
#endif /* GUCEF_MULTIPLAY_CSERVERLIST_H ? */

#ifndef GUCE_MULTIPLAY_CSERVERLISTPROVIDER_H
#include "gucefMULTIPLAY_CServerListProvider.h"
#define GUCE_MULTIPLAY_CSERVERLISTPROVIDER_H
#endif /* GUCE_MULTIPLAY_CSERVERLISTPROVIDER_H ? */

#endif /* __cplusplus ? */
/*--------------------------------------------------------------------------*/

#endif /* GUCEF_MULTIPLAY_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 13-09-2005 :
       - Initial version.

-----------------------------------------------------------------------------*/
