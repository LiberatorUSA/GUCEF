/*
 *  guidriverRocket: GUI backend using Rocket
 *  Copyright (C) 2002 - 2011.  Dinand Vanvelzen
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

#ifndef GUCEF_GUIDRIVERROCKET_H
#define GUCEF_GUIDRIVERROCKET_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

/*
 *      Library build configuration      
 */
#ifndef GUCEF_GUIDRIVERROCKET_CONFIG_H
#include "guidriverRocket_config.h"
#define GUCEF_GUIDRIVERROCKET_CONFIG_H
#endif /* GUCEF_GUIDRIVERROCKET_CONFIG_H ? */

/*
 *      Library specific macros and generic macros
 */
#ifndef GUCEF_GUIDRIVERROCKET_MACROS_H
#include "guidriverRocket_macros.h"
#define GUCEF_GUIDRIVERROCKET_MACROS_H
#endif /* GUCEF_GUIDRIVERROCKET_MACROS_H ? */

/*
 *      Simple types
 */
#ifndef GUCEF_GUIDRIVERROCKET_ETYPES_H
#include "guidriverRocket_ETypes.h"
#define GUCEF_GUIDRIVERROCKET_ETYPES_H
#endif /* GUCEF_GUIDRIVERROCKET_ETYPES_H ? */

/*---------------------------------------------------------------------------/

        C++ includes

/---------------------------------------------------------------------------*/
#ifdef __cplusplus

#ifndef GUCEF_GUIDRIVERROCKET_CFORMBACKENDIMP_H
#include "guidriverRocket_CFormBackendImp.h"
#define GUCEF_GUIDRIVERROCKET_CFORMBACKENDIMP_H
#endif /* GUCEF_GUIDRIVERROCKET_CFORMBACKENDIMP_H ? */

#ifndef GUCEF_GUIDRIVERROCKET_CWIDGETIMP_H
#include "guidriverRocket_CWidgetImp.h"
#define GUCEF_GUIDRIVERROCKET_CWIDGETIMP_H
#endif /* GUCEF_GUIDRIVERROCKET_CWIDGETIMP_H ? */

#ifndef GUCEF_GUIDRIVERROCKET_CBUTTONIMP_H
#include "guidriverRocket_CButtonImp.h"
#define GUCEF_GUIDRIVERROCKET_CBUTTONIMP_H
#endif /* GUCEF_GUIDRIVERROCKET_CBUTTONIMP_H ? */

#ifndef GUCEF_GUIDRIVERROCKET_CPUSHBUTTONIMP_H
#include "guidriverRocket_CPushButtonImp.h"
#define GUCEF_GUIDRIVERROCKET_CPUSHBUTTONIMP_H
#endif /* GUCEF_GUIDRIVERROCKET_CPUSHBUTTONIMP_H ? */

#ifndef GUCEF_GUIDRIVERROCKET_CEDITBOXIMP_H
#include "guidriverRocket_CEditboxImp.h"
#define GUCEF_GUIDRIVERROCKET_CEDITBOXIMP_H
#endif /* GUCEF_GUIDRIVERROCKET_CEDITBOXIMP_H ? */

#ifndef GUCEF_GUIDRIVERROCKET_CLISTBOXIMP_H
#include "guidriverRocket_CListboxImp.h"
#define GUCEF_GUIDRIVERROCKET_CLISTBOXIMP_H
#endif /* GUCEF_GUIDRIVERROCKET_CLISTBOXIMP_H ? */

#ifndef GUCEF_GUIDRIVERROCKET_CWINDOWIMP_H
#include "guidriverRocket_CWindowImp.h"
#define GUCEF_GUIDRIVERROCKET_CWINDOWIMP_H
#endif /* GUCEF_GUIDRIVERROCKET_CWINDOWIMP_H ? */

#ifndef GUCEF_GUIDRIVERROCKET_CIMAGEFRAMEIMP_H
#include "guidriverRocket_CImageFrameImp.h"
#define GUCEF_GUIDRIVERROCKET_CIMAGEFRAMEIMP_H
#endif /* GUCEF_GUIDRIVERROCKET_CIMAGEFRAMEIMP_H ? */

#ifndef GUCEF_GUIDRIVERROCKET_CHECKBOXIMP_H
#include "guidriverRocket_CCheckboxImp.h"
#define GUCEF_GUIDRIVERROCKET_CHECKBOXIMP_H
#endif /* GUCEF_GUIDRIVERROCKET_CHECKBOXIMP_H ? */

#ifndef GUCEF_GUIDRIVERROCKET_CLABELIMP_H
#include "guidriverRocket_CLabelImp.h"
#define GUCEF_GUIDRIVERROCKET_CLABELIMP_H
#endif /* GUCEF_GUIDRIVERROCKET_CLABELIMP_H ? */

#ifndef GUCEF_GUIDRIVERROCKET_CCOMBOBOXIMP_H
#include "guidriverRocket_CComboboxImp.h"
#define GUCEF_GUIDRIVERROCKET_CCOMBOBOXIMP_H
#endif /* GUCEF_GUIDRIVERROCKET_CCOMBOBOXIMP_H ? */

#ifndef GUCEF_GUIDRIVERROCKET_CTABCONTROLIMP_H
#include "guidriverRocket_CTabControlImp.h"
#define GUCEF_GUIDRIVERROCKET_CTABCONTROLIMP_H
#endif /* GUCEF_GUIDRIVERROCKET_CTABCONTROLIMP_H ? */

#ifndef GUCEF_GUIDRIVERROCKET_CTABCONTENTPANEIMP_H
#include "guidriverRocket_CTabContentPaneImp.h"
#define GUCEF_GUIDRIVERROCKET_CTABCONTENTPANEIMP_H
#endif /* GUCEF_GUIDRIVERROCKET_CTABCONTENTPANEIMP_H ? */

#ifndef GUCEF_GUIDRIVERROCKET_CSPINNERIMP_H
#include "guidriverRocket_CSpinnerImp.h"
#define GUCEF_GUIDRIVERROCKET_CSPINNERIMP_H
#endif /* GUCEF_GUIDRIVERROCKET_CSPINNERIMP_H ? */

#ifndef GUCEF_GUIDRIVERROCKET_CGRIDVIEWIMP_H
#include "guidriverRocket_CGridViewImp.h"
#define GUCEF_GUIDRIVERROCKET_CGRIDVIEWIMP_H
#endif /* GUCEF_GUIDRIVERROCKET_CGRIDVIEWIMP_H ? */

#ifndef GUCEF_GUIDRIVERROCKET_CFILEOPENDIALOGIMP_H
#include "guidriverRocket_CFileOpenDialogImp.h"
#define GUCEF_GUIDRIVERROCKET_CFILEOPENDIALOGIMP_H
#endif /* GUCEF_GUIDRIVERROCKET_CFILEOPENDIALOGIMP_H ? */

#ifndef GUCEF_GUIDRIVERROCKET_CFILESAVEDIALOGIMP_H
#include "guidriverRocket_CFileSaveDialogImp.h"
#define GUCEF_GUIDRIVERROCKET_CFILESAVEDIALOGIMP_H
#endif /* GUCEF_GUIDRIVERROCKET_CFILESAVEDIALOGIMP_H ? */

#ifndef GUCEF_GUIDRIVERROCKET_CFILESYSTEMDIALOGIMP_H
#include "guidriverRocket_CFileSystemDialogImp.h"
#define GUCEF_GUIDRIVERROCKET_CFILESYSTEMDIALOGIMP_H
#endif /* GUCEF_GUIDRIVERROCKET_CFILESYSTEMDIALOGIMP_H ? */

#ifndef GUCEF_GUIDRIVERROCKET_CRENDERCONTEXTIMP_H
#include "guidriverRocket_CRenderContextImp.h"
#define GUCEF_GUIDRIVERROCKET_CRENDERCONTEXTIMP_H
#endif /* GUCEF_GUIDRIVERROCKET_CRENDERCONTEXTIMP_H ? */

#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_GUIDRIVERROCKET_PUBLIC_C const char* GUCEF_GUIDRIVERROCKET_ModuleCompileTimeStamp( void );

/*--------------------------------------------------------------------------*/

#endif /* GUCEF_GUIDRIVERROCKET_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 21-04-2007 :
       - Made the initial version of the library header.

---------------------------------------------------------------------------*/
