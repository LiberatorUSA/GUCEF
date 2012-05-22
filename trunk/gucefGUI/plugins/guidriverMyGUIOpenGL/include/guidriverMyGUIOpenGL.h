/*
 *  guidriverMyGUI: glue module for the MyGUI GUI backend
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
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

#ifndef GUCEF_MYGUI_H
#define GUCEF_MYGUI_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Library build configuration      
 */
#ifndef GUCEF_MYGUI_CONFIG_H
#include "guceMyGUI_config.h"
#define GUCEF_MYGUI_CONFIG_H
#endif /* GUCEF_MYGUI_CONFIG_H ? */

/*
 *      Library specific macros and generic macros
 */
#ifndef GUCEF_MYGUI_MACROS_H
#include "guceMyGUI_macros.h"
#define GUCEF_MYGUI_MACROS_H
#endif /* GUCEF_MYGUI_MACROS_H ? */

/*
 *      Simple types
 */
#ifndef GUCEF_MYGUI_ETYPES_H
#include "guceMyGUI_ETypes.h"
#define GUCEF_MYGUI_ETYPES_H
#endif /* GUCEF_MYGUI_ETYPES_H ? */

/*---------------------------------------------------------------------------/

        C++ includes

/---------------------------------------------------------------------------*/
#ifdef __cplusplus

#ifndef GUCEF_MYGUI_CFORMBACKENDIMP_H
#include "guceMyGUI_CFormBackendImp.h"
#define GUCEF_MYGUI_CFORMBACKENDIMP_H
#endif /* GUCEF_MYGUI_CFORMBACKENDIMP_H ? */

#ifndef GUCEF_MYGUI_CWIDGETIMP_H
#include "guceMyGUI_CWidgetImp.h"
#define GUCEF_MYGUI_CWIDGETIMP_H
#endif /* GUCEF_MYGUI_CWIDGETIMP_H ? */

#ifndef GUCEF_MYGUI_CBUTTONIMP_H
#include "guceMyGUI_CButtonImp.h"
#define GUCEF_MYGUI_CBUTTONIMP_H
#endif /* GUCEF_MYGUI_CBUTTONIMP_H ? */

#ifndef GUCEF_MYGUI_CPUSHBUTTONIMP_H
#include "guceMyGUI_CPushButtonImp.h"
#define GUCEF_MYGUI_CPUSHBUTTONIMP_H
#endif /* GUCEF_MYGUI_CPUSHBUTTONIMP_H ? */

#ifndef GUCEF_MYGUI_CEDITBOXIMP_H
#include "guceMyGUI_CEditboxImp.h"
#define GUCEF_MYGUI_CEDITBOXIMP_H
#endif /* GUCEF_MYGUI_CEDITBOXIMP_H ? */

#ifndef GUCEF_MYGUI_CLISTBOXIMP_H
#include "guceMyGUI_CListboxImp.h"
#define GUCEF_MYGUI_CLISTBOXIMP_H
#endif /* GUCEF_MYGUI_CLISTBOXIMP_H ? */

#ifndef GUCEF_MYGUI_CWINDOWIMP_H
#include "guceMyGUI_CWindowImp.h"
#define GUCEF_MYGUI_CWINDOWIMP_H
#endif /* GUCEF_MYGUI_CWINDOWIMP_H ? */

#ifndef GUCEF_MYGUI_CIMAGEFRAMEIMP_H
#include "guceMyGUI_CImageFrameImp.h"
#define GUCEF_MYGUI_CIMAGEFRAMEIMP_H
#endif /* GUCEF_MYGUI_CIMAGEFRAMEIMP_H ? */

#ifndef GUCEF_MYGUI_CHECKBOXIMP_H
#include "guceMyGUI_CCheckboxImp.h"
#define GUCEF_MYGUI_CHECKBOXIMP_H
#endif /* GUCEF_MYGUI_CHECKBOXIMP_H ? */

#ifndef GUCEF_MYGUI_CLABELIMP_H
#include "guceMyGUI_CLabelImp.h"
#define GUCEF_MYGUI_CLABELIMP_H
#endif /* GUCEF_MYGUI_CLABELIMP_H ? */

#ifndef GUCEF_MYGUI_CCOMBOBOXIMP_H
#include "guceMyGUI_CComboboxImp.h"
#define GUCEF_MYGUI_CCOMBOBOXIMP_H
#endif /* GUCEF_MYGUI_CCOMBOBOXIMP_H ? */

#ifndef GUCEF_MYGUI_CTABCONTROLIMP_H
#include "guceMyGUI_CTabControlImp.h"
#define GUCEF_MYGUI_CTABCONTROLIMP_H
#endif /* GUCEF_MYGUI_CTABCONTROLIMP_H ? */

#ifndef GUCEF_MYGUI_CTABCONTENTPANEIMP_H
#include "guceMyGUI_CTabContentPaneImp.h"
#define GUCEF_MYGUI_CTABCONTENTPANEIMP_H
#endif /* GUCEF_MYGUI_CTABCONTENTPANEIMP_H ? */

#ifndef GUCEF_MYGUI_CSPINNERIMP_H
#include "guceMyGUI_CSpinnerImp.h"
#define GUCEF_MYGUI_CSPINNERIMP_H
#endif /* GUCEF_MYGUI_CSPINNERIMP_H ? */

#ifndef GUCEF_MYGUI_CGRIDVIEWIMP_H
#include "guceMyGUI_CGridViewImp.h"
#define GUCEF_MYGUI_CGRIDVIEWIMP_H
#endif /* GUCEF_MYGUI_CGRIDVIEWIMP_H ? */

#ifndef GUCEF_MYGUI_CFILEOPENDIALOGIMP_H
#include "guceMyGUI_CFileOpenDialogImp.h"
#define GUCEF_MYGUI_CFILEOPENDIALOGIMP_H
#endif /* GUCEF_MYGUI_CFILEOPENDIALOGIMP_H ? */

#ifndef GUCEF_MYGUI_CFILESAVEDIALOGIMP_H
#include "guceMyGUI_CFileSaveDialogImp.h"
#define GUCEF_MYGUI_CFILESAVEDIALOGIMP_H
#endif /* GUCEF_MYGUI_CFILESAVEDIALOGIMP_H ? */

#ifndef GUCEF_MYGUI_CFILESYSTEMDIALOGIMP_H
#include "guceMyGUI_CFileSystemDialogImp.h"
#define GUCEF_MYGUI_CFILESYSTEMDIALOGIMP_H
#endif /* GUCEF_MYGUI_CFILESYSTEMDIALOGIMP_H ? */

#ifndef GUCEF_MYGUI_CRENDERCONTEXTIMP_H
#include "guceMyGUI_CRenderContextImp.h"
#define GUCEF_MYGUI_CRENDERCONTEXTIMP_H
#endif /* GUCEF_MYGUI_CRENDERCONTEXTIMP_H ? */

#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_MYGUI_EXPORT_C const char* GUCEF_MYGUI_ModuleCompileTimeStamp( void );

/*--------------------------------------------------------------------------*/

#endif /* GUCEF_MYGUI_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 21-04-2007 :
       - Made the initial version of the library header.

-----------------------------------------------------------------------------*/
