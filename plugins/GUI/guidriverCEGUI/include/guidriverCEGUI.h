/*
 *  guidriverCEGUI: glue module for the CEGUI GUI backend
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

#ifndef GUCEF_GUIDRIVERCEGUI_H
#define GUCEF_GUIDRIVERCEGUI_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Library build configuration
 */
#ifndef GUCEF_GUIDRIVERCEGUI_CONFIG_H
#include "guidriverCEGUI_config.h"
#define GUCEF_GUIDRIVERCEGUI_CONFIG_H
#endif /* GUCEF_GUIDRIVERCEGUI_CONFIG_H ? */

/*
 *      Library specific macros and generic macros
 */
#ifndef GUCEF_GUIDRIVERCEGUI_MACROS_H
#include "guidriverCEGUI_macros.h"
#define GUCEF_GUIDRIVERCEGUI_MACROS_H
#endif /* GUCEF_GUIDRIVERCEGUI_MACROS_H ? */

/*
 *      Simple types
 */
#ifndef GUCEF_GUIDRIVERCEGUI_ETYPES_H
#include "guidriverCEGUI_ETypes.h"
#define GUCEF_GUIDRIVERCEGUI_ETYPES_H
#endif /* GUCEF_GUIDRIVERCEGUI_ETYPES_H ? */

/*---------------------------------------------------------------------------/

        C++ includes

/---------------------------------------------------------------------------*/
#ifdef __cplusplus

#ifndef GUCEF_GUIDRIVERCEGUI_CFORMBACKENDIMP_H
#include "guidriverCEGUI_CFormBackendImp.h"
#define GUCEF_GUIDRIVERCEGUI_CFORMBACKENDIMP_H
#endif /* GUCEF_GUIDRIVERCEGUI_CFORMBACKENDIMP_H ? */

#ifndef GUCEF_GUIDRIVERCEGUI_CWIDGETIMP_H
#include "guidriverCEGUI_CWidgetImp.h"
#define GUCEF_GUIDRIVERCEGUI_CWIDGETIMP_H
#endif /* GUCEF_GUIDRIVERCEGUI_CWIDGETIMP_H ? */

#ifndef GUCEF_GUIDRIVERCEGUI_CBUTTONIMP_H
#include "guidriverCEGUI_CButtonImp.h"
#define GUCEF_GUIDRIVERCEGUI_CBUTTONIMP_H
#endif /* GUCEF_GUIDRIVERCEGUI_CBUTTONIMP_H ? */

#ifndef GUCEF_GUIDRIVERCEGUI_CPUSHBUTTONIMP_H
#include "guidriverCEGUI_CPushButtonImp.h"
#define GUCEF_GUIDRIVERCEGUI_CPUSHBUTTONIMP_H
#endif /* GUCEF_GUIDRIVERCEGUI_CPUSHBUTTONIMP_H ? */

#ifndef GUCEF_GUIDRIVERCEGUI_CEDITBOXIMP_H
#include "guidriverCEGUI_CEditboxImp.h"
#define GUCEF_GUIDRIVERCEGUI_CEDITBOXIMP_H
#endif /* GUCEF_GUIDRIVERCEGUI_CEDITBOXIMP_H ? */

#ifndef GUCEF_GUIDRIVERCEGUI_CLISTBOXIMP_H
#include "guidriverCEGUI_CListBoxImp.h"
#define GUCEF_GUIDRIVERCEGUI_CLISTBOXIMP_H
#endif /* GUCEF_GUIDRIVERCEGUI_CLISTBOXIMP_H ? */

#ifndef GUCEF_GUIDRIVERCEGUI_CWINDOWIMP_H
#include "guidriverCEGUI_CWindowImp.h"
#define GUCEF_GUIDRIVERCEGUI_CWINDOWIMP_H
#endif /* GUCEF_GUIDRIVERCEGUI_CWINDOWIMP_H ? */

#ifndef GUCEF_GUIDRIVERCEGUI_CIMAGEFRAMEIMP_H
#include "guidriverCEGUI_CImageFrameImp.h"
#define GUCEF_GUIDRIVERCEGUI_CIMAGEFRAMEIMP_H
#endif /* GUCEF_GUIDRIVERCEGUI_CIMAGEFRAMEIMP_H ? */

#ifndef GUCEF_GUIDRIVERCEGUI_CHECKBOXIMP_H
#include "guidriverCEGUI_CCheckboxImp.h"
#define GUCEF_GUIDRIVERCEGUI_CHECKBOXIMP_H
#endif /* GUCEF_GUIDRIVERCEGUI_CHECKBOXIMP_H ? */

#ifndef GUCEF_GUIDRIVERCEGUI_CLABELIMP_H
#include "guidriverCEGUI_CLabelImp.h"
#define GUCEF_GUIDRIVERCEGUI_CLABELIMP_H
#endif /* GUCEF_GUIDRIVERCEGUI_CLABELIMP_H ? */

#ifndef GUCEF_GUIDRIVERCEGUI_CCOMBOBOXIMP_H
#include "guidriverCEGUI_CComboboxImp.h"
#define GUCEF_GUIDRIVERCEGUI_CCOMBOBOXIMP_H
#endif /* GUCEF_GUIDRIVERCEGUI_CCOMBOBOXIMP_H ? */

#ifndef GUCEF_GUIDRIVERCEGUI_CTABCONTROLIMP_H
#include "guidriverCEGUI_CTabControlImp.h"
#define GUCEF_GUIDRIVERCEGUI_CTABCONTROLIMP_H
#endif /* GUCEF_GUIDRIVERCEGUI_CTABCONTROLIMP_H ? */

#ifndef GUCEF_GUIDRIVERCEGUI_CTABCONTENTPANEIMP_H
#include "guidriverCEGUI_CTabContentPaneImp.h"
#define GUCEF_GUIDRIVERCEGUI_CTABCONTENTPANEIMP_H
#endif /* GUCEF_GUIDRIVERCEGUI_CTABCONTENTPANEIMP_H ? */

#ifndef GUCEF_GUIDRIVERCEGUI_CSPINNERIMP_H
#include "guidriverCEGUI_CSpinnerImp.h"
#define GUCEF_GUIDRIVERCEGUI_CSPINNERIMP_H
#endif /* GUCEF_GUIDRIVERCEGUI_CSPINNERIMP_H ? */

#ifndef GUCEF_GUIDRIVERCEGUI_CGRIDVIEWIMP_H
#include "guidriverCEGUI_CGridViewImp.h"
#define GUCEF_GUIDRIVERCEGUI_CGRIDVIEWIMP_H
#endif /* GUCEF_GUIDRIVERCEGUI_CGRIDVIEWIMP_H ? */

#ifndef GUCEF_GUIDRIVERCEGUI_CFILEOPENDIALOGIMP_H
#include "guidriverCEGUI_CFileOpenDialogImp.h"
#define GUCEF_GUIDRIVERCEGUI_CFILEOPENDIALOGIMP_H
#endif /* GUCEF_GUIDRIVERCEGUI_CFILEOPENDIALOGIMP_H ? */

#ifndef GUCEF_GUIDRIVERCEGUI_CFILESAVEDIALOGIMP_H
#include "guidriverCEGUI_CFileSaveDialogImp.h"
#define GUCEF_GUIDRIVERCEGUI_CFILESAVEDIALOGIMP_H
#endif /* GUCEF_GUIDRIVERCEGUI_CFILESAVEDIALOGIMP_H ? */

#ifndef GUCEF_GUIDRIVERCEGUI_CFILESYSTEMDIALOGIMP_H
#include "guidriverCEGUI_CFileSystemDialogImp.h"
#define GUCEF_GUIDRIVERCEGUI_CFILESYSTEMDIALOGIMP_H
#endif /* GUCEF_GUIDRIVERCEGUI_CFILESYSTEMDIALOGIMP_H ? */

#ifndef GUCEF_GUIDRIVERCEGUI_CRENDERCONTEXTIMP_H
#include "guidriverCEGUI_CRenderContextImp.h"
#define GUCEF_GUIDRIVERCEGUI_CRENDERCONTEXTIMP_H
#endif /* GUCEF_GUIDRIVERCEGUI_CRENDERCONTEXTIMP_H ? */

#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_GUIDRIVERCEGUI_EXPORT_C const char* GUCEF_GUIDRIVERCEGUI_ModuleCompileTimeStamp( void );

/*--------------------------------------------------------------------------*/

#endif /* GUCEF_GUIDRIVERCEGUI_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 21-04-2007 :
       - Made the initial version of the library header.

-----------------------------------------------------------------------------*/
