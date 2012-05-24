/*
 *  guidriverMyGUIOpenGL: glue module for the MyGUI GUI backend using OpenGL
 *  Copyright (C) 2002 - 2008.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
 */

/*
 *      This is the main include header for the guceCEGUIOgre library.
 *      It is an optional part of an engine called GUCE.
 *      All classes in this library have to do with providing a GUI backend
 *      based on CEGUI with Ogre doing the rendering.
 */

#ifndef GUCEF_MYGUIGL_H
#define GUCEF_MYGUIGL_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Library build configuration      
 */
#ifndef GUCEF_MYGUIGL_CONFIG_H
#include "guidriverMyGUIOpenGL_config.h"
#define GUCEF_MYGUIGL_CONFIG_H
#endif /* GUCEF_MYGUIGL_CONFIG_H ? */

/*
 *      Library specific macros and generic macros
 */
#ifndef GUCEF_MYGUIGL_MACROS_H
#include "guidriverMyGUIOpenGL_macros.h"
#define GUCEF_MYGUIGL_MACROS_H
#endif /* GUCEF_MYGUIGL_MACROS_H ? */

/*---------------------------------------------------------------------------/

        C++ includes

/---------------------------------------------------------------------------*/
#ifdef __cplusplus

#ifndef GUCEF_MYGUIGL_CGUIDRIVERGL_H
#include "guidriverMyGUIOpenGL_CGUIDriverGL.h"
#define GUCEF_MYGUIGL_CGUIDRIVERGL_H
#endif /* GUCEF_MYGUIGL_CGUIDRIVERGL_H ? */

#ifndef GUCEF_MYGUIGL_CGUICONTEXTGL_H
#include "guidriverMyGUIOpenGL_CGUIContextGL.h"
#define GUCEF_MYGUIGL_CGUICONTEXTGL_H
#endif /* GUCEF_MYGUIGL_CGUICONTEXTGL_H ? */

#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_MYGUIGL_EXPORT_C const char* GUCEF_MYGUIGL_ModuleCompileTimeStamp( void );

/*--------------------------------------------------------------------------*/

#endif /* GUCEF_MYGUIGL_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 21-04-2007 :
       - Made the initial version of the library header.

-----------------------------------------------------------------------------*/
