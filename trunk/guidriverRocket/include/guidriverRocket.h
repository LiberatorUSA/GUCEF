/*
 *  guceMyGUIOgre: glue module for the MyGUI+Ogre GUI backend
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

#ifndef GUCE_MYGUIOGRE_H
#define GUCE_MYGUIOGRE_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Library build configuration      
 */
#ifndef GUCE_MYGUIOGRE_CONFIG_H
#include "guceMyGUIOgre_config.h"
#define GUCE_MYGUIOGRE_CONFIG_H
#endif /* GUCE_MYGUIOGRE_CONFIG_H ? */

/*
 *      Library specific macros and generic macros
 */
#ifndef GUCE_MYGUIOGRE_MACROS_H
#include "guceMyGUIOgre_macros.h"
#define GUCE_MYGUIOGRE_MACROS_H
#endif /* GUCE_MYGUIOGRE_MACROS_H ? */

/*
 *      Simple types
 */
#ifndef GUCE_MYGUIOGRE_ETYPES_H
#include "guceMyGUIOgre_ETypes.h"
#define GUCE_MYGUIOGRE_ETYPES_H
#endif /* GUCE_MYGUIOGRE_ETYPES_H ? */

/*---------------------------------------------------------------------------/

        C++ includes

/---------------------------------------------------------------------------*/
#ifdef __cplusplus

#ifndef GUCE_MYGUIOGRE_CFORMBACKENDIMP_H
#include "guceMyGUIOgre_CFormBackendImp.h"
#define GUCE_MYGUIOGRE_CFORMBACKENDIMP_H
#endif /* GUCE_MYGUIOGRE_CFORMBACKENDIMP_H ? */

#ifndef GUCE_MYGUIOGRE_CWIDGETIMP_H
#include "guceMyGUIOgre_CWidgetImp.h"
#define GUCE_MYGUIOGRE_CWIDGETIMP_H
#endif /* GUCE_MYGUIOGRE_CWIDGETIMP_H ? */

#ifndef GUCE_MYGUIOGRE_CBUTTONIMP_H
#include "guceMyGUIOgre_CButtonImp.h"
#define GUCE_MYGUIOGRE_CBUTTONIMP_H
#endif /* GUCE_MYGUIOGRE_CBUTTONIMP_H ? */

#ifndef GUCE_MYGUIOGRE_CPUSHBUTTONIMP_H
#include "guceMyGUIOgre_CPushButtonImp.h"
#define GUCE_MYGUIOGRE_CPUSHBUTTONIMP_H
#endif /* GUCE_MYGUIOGRE_CPUSHBUTTONIMP_H ? */

#ifndef GUCE_MYGUIOGRE_CEDITBOXIMP_H
#include "guceMyGUIOgre_CEditboxImp.h"
#define GUCE_MYGUIOGRE_CEDITBOXIMP_H
#endif /* GUCE_MYGUIOGRE_CEDITBOXIMP_H ? */

#ifndef GUCE_MYGUIOGRE_CLISTBOXIMP_H
#include "guceMyGUIOgre_CListboxImp.h"
#define GUCE_MYGUIOGRE_CLISTBOXIMP_H
#endif /* GUCE_MYGUIOGRE_CLISTBOXIMP_H ? */

#ifndef GUCE_MYGUIOGRE_CWINDOWIMP_H
#include "guceMyGUIOgre_CWindowImp.h"
#define GUCE_MYGUIOGRE_CWINDOWIMP_H
#endif /* GUCE_MYGUIOGRE_CWINDOWIMP_H ? */

#ifndef GUCE_MYGUIOGRE_CIMAGEFRAMEIMP_H
#include "guceMyGUIOgre_CImageFrameImp.h"
#define GUCE_MYGUIOGRE_CIMAGEFRAMEIMP_H
#endif /* GUCE_MYGUIOGRE_CIMAGEFRAMEIMP_H ? */

#ifndef GUCE_MYGUIOGRE_CHECKBOXIMP_H
#include "guceMyGUIOgre_CCheckboxImp.h"
#define GUCE_MYGUIOGRE_CHECKBOXIMP_H
#endif /* GUCE_MYGUIOGRE_CHECKBOXIMP_H ? */

#ifndef GUCE_MYGUIOGRE_CLABELIMP_H
#include "guceMyGUIOgre_CLabelImp.h"
#define GUCE_MYGUIOGRE_CLABELIMP_H
#endif /* GUCE_MYGUIOGRE_CLABELIMP_H ? */

#ifndef GUCE_MYGUIOGRE_CCOMBOBOXIMP_H
#include "guceMyGUIOgre_CComboboxImp.h"
#define GUCE_MYGUIOGRE_CCOMBOBOXIMP_H
#endif /* GUCE_MYGUIOGRE_CCOMBOBOXIMP_H ? */

#ifndef GUCE_MYGUIOGRE_CTABCONTROLIMP_H
#include "guceMyGUIOgre_CTabControlImp.h"
#define GUCE_MYGUIOGRE_CTABCONTROLIMP_H
#endif /* GUCE_MYGUIOGRE_CTABCONTROLIMP_H ? */

#ifndef GUCE_MYGUIOGRE_CTABCONTENTPANEIMP_H
#include "guceMyGUIOgre_CTabContentPaneImp.h"
#define GUCE_MYGUIOGRE_CTABCONTENTPANEIMP_H
#endif /* GUCE_MYGUIOGRE_CTABCONTENTPANEIMP_H ? */

#ifndef GUCE_MYGUIOGRE_CSPINNERIMP_H
#include "guceMyGUIOgre_CSpinnerImp.h"
#define GUCE_MYGUIOGRE_CSPINNERIMP_H
#endif /* GUCE_MYGUIOGRE_CSPINNERIMP_H ? */

#ifndef GUCE_MYGUIOGRE_CGRIDVIEWIMP_H
#include "guceMyGUIOgre_CGridViewImp.h"
#define GUCE_MYGUIOGRE_CGRIDVIEWIMP_H
#endif /* GUCE_MYGUIOGRE_CGRIDVIEWIMP_H ? */

#ifndef GUCE_MYGUIOGRE_CFILEOPENDIALOGIMP_H
#include "guceMyGUIOgre_CFileOpenDialogImp.h"
#define GUCE_MYGUIOGRE_CFILEOPENDIALOGIMP_H
#endif /* GUCE_MYGUIOGRE_CFILEOPENDIALOGIMP_H ? */

#ifndef GUCE_MYGUIOGRE_CFILESAVEDIALOGIMP_H
#include "guceMyGUIOgre_CFileSaveDialogImp.h"
#define GUCE_MYGUIOGRE_CFILESAVEDIALOGIMP_H
#endif /* GUCE_MYGUIOGRE_CFILESAVEDIALOGIMP_H ? */

#ifndef GUCE_MYGUIOGRE_CFILESYSTEMDIALOGIMP_H
#include "guceMyGUIOgre_CFileSystemDialogImp.h"
#define GUCE_MYGUIOGRE_CFILESYSTEMDIALOGIMP_H
#endif /* GUCE_MYGUIOGRE_CFILESYSTEMDIALOGIMP_H ? */

#ifndef GUCE_MYGUIOGRE_CRENDERCONTEXTIMP_H
#include "guceMyGUIOgre_CRenderContextImp.h"
#define GUCE_MYGUIOGRE_CRENDERCONTEXTIMP_H
#endif /* GUCE_MYGUIOGRE_CRENDERCONTEXTIMP_H ? */

#ifndef GUCE_MYGUIOGRE_CGUIDRIVER_H
#include "guceMyGUIOgre_CGUIDriver.h"
#define GUCE_MYGUIOGRE_CGUIDRIVER_H
#endif /* GUCE_MYGUIOGRE_CGUIDRIVER_H ? */

#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCE_MYGUIOGRE_EXPORT_C const char* GUCE_MYGUIOGRE_ModuleCompileTimeStamp( void );

/*--------------------------------------------------------------------------*/

#endif /* GUCE_MYGUIOGRE_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 21-04-2007 :
       - Made the initial version of the library header.

-----------------------------------------------------------------------------*/
