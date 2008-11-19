/*
 *  gucefINPUT: GUCEF module providing input device interaction
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

#ifndef GUCEFINPUT_H
#define GUCEFINPUT_H

#ifdef __cplusplus
namespace GUCEF {

/**
 *      This is the main include header for the gucefINPUT library.
 *      It is part of a framework called GUCEF.
 *      All classes in this library have to do with user input management and
 *      the interfacing with the hardware.
 */
namespace INPUT {} }
#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Library specific macros and generic macros
 */
#ifndef GUCEFINPUT_MACROS_H
#include "gucefINPUT_macros.h"
#define GUCEFINPUT_MACROS_H
#endif /* GUCEFINPUT_MACROS_H ? */

/*---------------------------------------------------------------------------/

        C++ includes

/---------------------------------------------------------------------------*/
#ifdef __cplusplus

#ifndef CINPUTCONTROLLER_H
#include "CInputController.h"
#define CINPUTCONTROLLER_H
#endif /* CINPUTCONTROLLER_H ? */

#ifndef CINPUTCONTEXT_H
#include "CInputContext.h"
#define CINPUTCONTEXT_H
#endif /* CINPUTCONTEXT_H ? */

#ifndef CINPUTACTIONMAP_H
#include "CInputActionMap.h"
#define CINPUTACTIONMAP_H
#endif /* CINPUTACTIONMAP_H ? */

#endif /* __cplusplus ? */
/*--------------------------------------------------------------------------*/

#endif /* GUCEFINPUT_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 13-09-2005 :
       - Initial version.

-----------------------------------------------------------------------------*/
